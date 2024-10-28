///////////////////////////////////////
//
// LICENSE NOTICE:
//
// Epic GTA2 Script Decompiler, version 1.0.1, November 16th, 2011
//
// Copyright (C) 2011 T.M. (tmdevmail at gmail.com)
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the author(s) be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// excluding commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation is required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software. All versions source code must
//    be made freely available for everyone.
// 3. build_credits() function may not be altered and its output must remain in the
//    decompiler output as the first line of text. Contact the author if you desire
//    to be credited on that line of text.
// 4. This notice may not be removed or altered from any source distribution.
//
///////////////////////////////////////

const SCR_f SCR_EMPTYCOORD = 255*16384; // = 255.0 as SCR-float value.

struct FPStruct {
	SCR_CMD_HEADER *header; // points to the script data.
	int pointer_index;
	int max_total_pointers;
	int used_pointers;
	uint16_t *pointers;
	uint8_t *script;
	uint8_t *strings;
	uint16_t point;
	string filename;
};

// generate hex editor compatible value so we can simply do %04X to match the hex editor value:
uint16_t uint16_t_flip(const uint16_t &value){
	return ((value & 255) << 8) | ((value >> 8) & 255);
}


#ifdef SCR_INCLUDE_DEBUG
string SCR_ERRORS_STR;
void write_error_str(string msg){
	SCR_ERRORS_STR += msg + LINESEP;
}

void write_debug_msg(const FPStruct &params, const string &title, int value){
	if(SCR_use_debug){
		write_error_str(
			sprintf_str("%s: '%d' in script file '%s' at cmd '%04X' with type '%04X'", 
				title.c_str(), value, params.filename.c_str(), uint16_t_flip(params.header->cmd_this), uint16_t_flip(params.header->type)
			)
		);
	}
}

// note: some functions use FFFF for "NONE" values, but not all of them, so we cant simply just make the conversion in this function.
const char *get_string_safe(const char *arr[], const int &index, const int &max, const int &linenum, const FPStruct &params){
	if(index < 0 || index >= max){
		if(SCR_use_debug){
			write_error_str(
				sprintf_str("Invalid index: '%d' (max: %d) at line '%d' in script file '%s' at cmd '%04X' with type '%04X'", 
					index, max, linenum, params.filename.c_str(), uint16_t_flip(params.header->cmd_this), uint16_t_flip(params.header->type)
				)
			);
		}
		return "<INVALID_INDEX>";
	}else{
		if(SCR_use_debug){
			if(arr[index][0] == '\0'){
				write_error_str(
					sprintf_str("Empty enum returned at index: '%d' (max: %d) at line '%d' in script file '%s' at cmd '%04X' with type '%04X'", 
						index, max, linenum, params.filename.c_str(), uint16_t_flip(params.header->cmd_this), uint16_t_flip(params.header->type)
					)
				);
				return "<EMPTY_INDEX>";
			}
		}
	}
	return arr[index];
}
#define enum_cstr(ArrayName, index) get_string_safe(SCR_##ArrayName, index, arrlen(SCR_##ArrayName), __LINE__, params)
#else
// note: some functions use FFFF for "NONE" values, but not all of them! so we cant simply just make the conversion in this function,
// or else we wouldnt be notified by errors since the compiler doesnt accept "NONE" in every function.
const char *get_string_safe(const char *arr[], const int &index, const int &max){
	if(index < 0 || index >= max){
		return "<INVALID_INDEX>";
	}
	return arr[index];
}
#define enum_cstr(ArrayName, index) get_string_safe(SCR_##ArrayName, index, arrlen(SCR_##ArrayName))
#endif

// converts SCR-float value into float:
string scr_fstr(const SCR_f &val){
	return sprintf_str(SCR_FLOAT_PATTERN.c_str(), (float)val/16384.0f);
}
#define scr_float(val) scr_fstr(val).c_str()

// generate coordinate, either by 2d or 3d depending if z-coord was used or not:
// Note: some functions doesnt use this because they only support 3d coords!
string scr_coord(const SCR_XYZ_f &pos){
	// (at the moment its forced to 3d coords since there is a lot of bugs in compiler for 2d coords!)
	//if(pos.z == SCR_EMPTYCOORD){
	//	return sprintf_str("%s, %s", scr_float(pos.x), scr_float(pos.y));
	//}else{
		return sprintf_str("%s, %s, %s", scr_float(pos.x), scr_float(pos.y), scr_float(pos.z));
	//}
}
// overload for other possibilities:
string scr_coord(const SCR_XY_f &pos){
	return sprintf_str("%s, %s", scr_float(pos.x), scr_float(pos.y));
}
string scr_coord(const SCR_XYZ_uc &pos){ // integer positions always 3d
	return sprintf_str("%d, %d, %d", pos.x, pos.y, pos.z);
}
string scr_coord(const SCR_XY_uc &pos){
	return sprintf_str("%d, %d", pos.x, pos.y);
}
string scr_coord_3d(const SCR_XYZ_f &pos){
	return sprintf_str("%s, %s, %s", scr_float(pos.x), scr_float(pos.y), scr_float(pos.z));
}
string scr_coord_2d(const SCR_XYZ_f &pos){
	return sprintf_str("%s, %s", scr_float(pos.x), scr_float(pos.y));
}

string scr_rect(const SCR_Rect_f &rect){
	return scr_coord_3d(rect.pos)+", "+scr_coord(rect.size);
}

string scr_color_rgb(const SCR_XYZ_uc &color){
	return sprintf_str("%d, %d, %d", color.z, color.y, color.x); // stored in BGR format.
}

#define get_scr_typepoint(T, script, point) (T *)((script)+(point)) // get pointer of type T
#define read_scr_type(T, script, point) (*get_scr_typepoint(T, script, point)) // get first element of type T
#define get_data(T, data) const T &data = read_scr_type(T, params.script, params.point+sizeof(SCR_CMD_HEADER)) // note: skips the header reading (not in structs anymore!)


typedef string (*FuncPtrType)(const FPStruct&);
typedef map<int, FuncPtrType> FuncMapType;
FuncMapType Functions;

int LinkToFuncForReal(const int &type, const FuncPtrType &func){
	Functions[type] = func;
	return 1;
}
#define LinkToFunc(type, func) int INIT_##type = LinkToFuncForReal(type, func) // create dummy variable to be able to execute the function in any place of the code.

typedef map<uint16_t, int, greater<uint16_t> > uint16_tIntMap;
uint16_tIntMap BoolFunctions;

int IsBoolFuncForReal(const uint16_t &type){
	BoolFunctions[type] = 1;
	return 1;
}
#define IsBoolFunc(type) int SETBOOL_##type = IsBoolFuncForReal(type) // create dummy variable to be able to execute the function in any place of the code.

bool is_bool_function(const uint16_t &type){
	return array_key_exists(type, BoolFunctions);
}

/////////////////////////    variable names stuff    1

typedef map<uint16_t, string, greater<uint16_t> > uint16_tStrMap;
typedef map<string, int, greater<string> > StrIntMap;
uint16_tStrMap varnamearray;
StrIntMap nameprefixcounter;
uint16_tStrMap funcnamearray; // functions use .cmd_this instead of .point
StrIntMap funcnameprefixcounter;

// this function is used only in SCR command reading functions.
string set_and_get_varname_from_params(const FPStruct &params, const string &str){
	string fullname;
	uint16_tStrMap::const_iterator it = varnamearray.find(params.point);
	if(it == varnamearray.end()){
		// if not found, add new name:
		nameprefixcounter[str]++;
		fullname = str + sprintf_str("%d", nameprefixcounter[str]);
		varnamearray[params.point] = fullname;
	}else{
		fullname = it->second; // use existing name
	}
	return fullname;
}

// get variable name from custom pointer_index, used in SCR command parameters.
string get_varname_from_custom_point(const FPStruct &params, const uint16_t &pointer_index){
	uint16_t &point = params.pointers[pointer_index]; // get the pointer value from pointers array. variables are linked to the index of pointers array.
	uint16_tStrMap::const_iterator it = varnamearray.find(point);
	if(it != varnamearray.end()){
		return it->second; // use existing name
	}else{
		#ifdef SCR_INCLUDE_DEBUG
		write_debug_msg(params, "Var not found", point);
		#endif
		return "<UNKNOWN_VAR>"; // in case of error (shouldnt be possible)
	}
}

// these macros are used inside the read_? functions:
// sets the name for the variable and returns it, or returns already existing name:
#define setgetname(str) set_and_get_varname_from_params(params, str)
// only returns already existing name:
#define getname(point) get_varname_from_custom_point(params, point)

// function names are linked by the cmd value:
// this function is used only in SCR command reading functions.
string set_and_get_funcname_from_params(const FPStruct &params, const string &str){
	string fullname;
	uint16_tStrMap::const_iterator it = funcnamearray.find(params.header->cmd_this);
	if(it == funcnamearray.end()){
		// if not found, add new name:
		funcnameprefixcounter[str]++;
		fullname = str + sprintf_str("%d", funcnameprefixcounter[str]);
		funcnamearray[params.header->cmd_this] = fullname;
	}else{
		fullname = it->second; // use existing name
	}
	return fullname;
}

// get function name from cmd ID used in SCR command parameters.
string get_funcname_from_custom_point(
	#ifdef SCR_INCLUDE_DEBUG
	const FPStruct &params,
	#endif
	const uint16_t &cmd)
{
	uint16_tStrMap::const_iterator it = funcnamearray.find(cmd);
	if(it != funcnamearray.end()){
		return it->second; // use existing name
	}else{
		#ifdef SCR_INCLUDE_DEBUG
		write_debug_msg(params, "Function not found", cmd);
		#endif
		return "<UNKNOWN_FUNCTION>"; // in case of error (shouldnt be possible)
	}
}

// sets the name for the function and returns it, or returns already existing name:
#define setgetfuncname(str) set_and_get_funcname_from_params(params, str)
// only returns already existing name:
#ifdef SCR_INCLUDE_DEBUG
#define getfuncname(cmd) get_funcname_from_custom_point(params, cmd)
#else
#define getfuncname(cmd) get_funcname_from_custom_point(cmd) // dont carry the params if not needed.
#endif

struct StringMapStruct {
	uint16_t datatype;
	string str;
	StringMapStruct(){}
	StringMapStruct(uint16_t datatype, string str) : datatype(datatype), str(str) {}
};
typedef map<uint16_t, StringMapStruct, greater<uint16_t> > StringMapStructMap;
StringMapStructMap StringMap;

// reads the array in the 5120 bytes block,
// note: the items in this array are saved in reverse order: latest string is read first here.
void build_scr_strings_array(uint8_t *strings){
	StringMap.clear();
	int data_len = (int)read_scr_type(uint16_t, strings, 0);
	int curpos = sizeof(uint16_t); // set reading pos after header.
	while(curpos < data_len){
		SCR_STRING_HEADER &data = read_scr_type(SCR_STRING_HEADER, strings, curpos);
		curpos += sizeof(SCR_STRING_HEADER);
		string str;
		for(int u = 0; u < data.len; u++){
			char &chr = read_scr_type(char, strings, curpos+u);
			if(chr == '\0') break;
			str += chr;
		}
		StringMap[data.str_id] = StringMapStruct(data.datatype, str);
		curpos += data.len;
	}
}


// used to return zonenames, gangnames, mission filenames, and perhaps some other strings.
// note: variablenames arent stored at all, those are generated automatically.
string get_scr_string_for_real(
	#ifdef SCR_INCLUDE_DEBUG
	const FPStruct &params,
	#endif
	const uint16_t &str_id)
{
	StringMapStructMap::const_iterator it = StringMap.find(str_id);
	if(it != StringMap.end()){
		return it->second.str;
	}else{
		#ifdef SCR_INCLUDE_DEBUG
		write_debug_msg(params, "String not found", str_id);
		#endif
		return "<STRNOTFOUND>"; // in case of error. note: this is possible if using LAUNCH_MISSION (or etc that uses strings array) inside a mission script!
	}
}

#ifdef SCR_INCLUDE_DEBUG
#define get_scr_string(str_id) get_scr_string_for_real(params, str_id) // need macro so we can use params struct for error reporting.
#else
#define get_scr_string(str_id) get_scr_string_for_real(str_id) // dont carry the params if not needed.
#endif


typedef map<uint16_t, int, greater<uint16_t> > uint16_tIntMap;
uint16_tIntMap CommandLens;

int find_next_non_null_pointer_value(const FPStruct &params){
	int u = params.pointer_index+1;
	while(u < params.used_pointers){
		if(params.pointers[u] != 0){
			return (int)params.pointers[u];
		}
		u++;
	}
	return -1;
}

string hexdumpline(const FPStruct &params, const int &blocklen, bool addname = 1){
	if(blocklen < 0) return "<ERROR@BLOCKLEN>"; // just in case
	string hexstr;
	if(blocklen < 6){
		hexstr = "[";
		for(int u = 0; u < blocklen; u+=2){
			hexstr += sprintf_str("%05d", read_scr_type(uint16_t, params.script, params.point+u));
			if(u < blocklen-2) hexstr += " ";
		}
		hexstr += "]";
	}else{
		hexstr = sprintf_str("(%05d -> %05d) = [", params.header->cmd_this, params.header->cmd_next);
		for(int u = 6; u < blocklen; u+=2){
			hexstr += sprintf_str("%05d", read_scr_type(uint16_t, params.script, params.point+u));
			if(u < blocklen-2) hexstr += " ";
		}
		hexstr += "]";
	}
	if(addname){
		return sprintf_str("// %s = %s", get_scr_cmd(params.header->type), hexstr.c_str());
	}else{
		return sprintf_str("// %s", hexstr.c_str());
	}
}

string hexdump(const FPStruct &params, bool addname = 1){
	int p1 = (int)params.pointers[params.pointer_index];
	int p2;
	if(p1 != 0 && (p2 = find_next_non_null_pointer_value(params)) != -1){
		int blocklen = p2-p1;
		if(blocklen >= 0) CommandLens[params.header->type] = blocklen;
		return hexdumpline(params, blocklen, addname);
	}else{
		if(array_key_exists(params.header->type, CommandLens)){
			return hexdumpline(params, CommandLens[params.header->type], addname); // if we cant find next non null, lets hope the command was already used so we know its length.
		}else{
			return sprintf_str("// %s", get_scr_cmd(params.header->type)); // unknown data length.
		}
	}
}

string hexdumpline_short(const FPStruct &params, int blocklen){
	if(blocklen < 0) return "<ERROR@BLOCKLEN>"; // just in case
	string hexstr;
	if(blocklen < 6){
		for(int u = 0; u < blocklen; u+=2){
			hexstr += sprintf_str("%05d", read_scr_type(uint16_t, params.script, params.point+u));
			if(u < blocklen-2) hexstr += " ";
		}
	}else{
		hexstr = sprintf_str("(%05d -> %05d)", params.header->cmd_this, params.header->cmd_next);
	}
	return sprintf_str("// %s", hexstr.c_str());
}
string hexdump_short(const FPStruct &params){
	int p1 = (int)params.pointers[params.pointer_index];
	int p2;
	if(p1 != 0 && (p2 = find_next_non_null_pointer_value(params)) != -1){
		int blocklen = p2-p1;
		if(blocklen >= 0) CommandLens[params.header->type] = blocklen;
		return hexdumpline_short(params, blocklen);
	}else{
		if(array_key_exists(params.header->type, CommandLens)){
			return hexdumpline_short(params, CommandLens[params.header->type]); // if we cant find next non null, lets hope the command was already used so we know its length.
		}else{
			return sprintf_str("// %s", get_scr_cmd(params.header->type)); // unknown data length.
		}
	}
}

#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#if __has_attribute(unused)
#define ATTR_UNUSED __attribute__((unused))
#else
#define ATTR_UNUSED
#endif

//////////////////////////////////////////////////////////
// ONLY USED FOR FUNCTIONS THAT _RETURNS_ BOOL VALUE!!
// NOTE: For nicer debugging: IsBoolFunc() must be called for all bool functions.
//
#define ZERO_PARAMS_LAYOUT_BOOL(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(ATTR_UNUSED const FPStruct &params){ \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME); \
	IsBoolFunc(SCRCMD_##ENUMNAME)

#define TWO_PARAMS_LAYOUT_BOOL(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(const FPStruct &params){ \
		get_data(SCR_TWO_PARAMS, data); \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME); \
	IsBoolFunc(SCRCMD_##ENUMNAME)

#define FOUR_PARAMS_LAYOUT_BOOL(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(const FPStruct &params){ \
		get_data(SCR_FOUR_PARAMS, data); \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME); \
	IsBoolFunc(SCRCMD_##ENUMNAME)

#define CUSTOM_STRUCT_LAYOUT_BOOL(ENUMNAME, structname, stringoutput) \
	string read_##ENUMNAME(const FPStruct &params){ \
		get_data(structname, data); \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME); \
	IsBoolFunc(SCRCMD_##ENUMNAME)


///////////////////////////////////////////////////////////
// ONLY USED FOR FUNCTIONS THAT DOESNT RETURN ANYTHING!!
//
#define ZERO_PARAMS_LAYOUT(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(ATTR_UNUSED const FPStruct &params){ \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME)

#define TWO_PARAMS_LAYOUT(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(const FPStruct &params){ \
		get_data(SCR_TWO_PARAMS, data); \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME)

#define FOUR_PARAMS_LAYOUT(ENUMNAME, stringoutput) \
	string read_##ENUMNAME(const FPStruct &params){ \
		get_data(SCR_FOUR_PARAMS, data); \
		return stringoutput; \
	} \
	LinkToFunc(SCRCMD_##ENUMNAME, read_##ENUMNAME)
