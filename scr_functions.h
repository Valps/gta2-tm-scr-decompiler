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

//
// Output formatting/coding practises:
// - boolean functions will not have a space before first brace. it makes distinct
//   difference which is used in if-structures and which arent, also nicer output.
// - always when possible, using scr_rect() to generate (x,y,z,w,h) in one run.
// - if a function takes 2d or 3d coordinates, using scr_coord() to detect which one
//   to use (NOTE: atm it always outputs 3d coords for avoiding bugs in compiler).
// - if a function takes 2d only, using scr_coord_2d()
// - if a function takes 3d only, using scr_coord_3d()
// - use SCR_f type in structs for single float values.
// - use Sint* only when it can have negative values too, otherwise use Uint* type,
//   and also Uint* when its not known which kind of values it can take.
// - all remaps should use "remap" variable name and should be Sint* type.
// - all angles should use "rot" variable name and should be Uint* type. if a function
//   has multiple rotation variables, then using "_rotation" suffix for the variable name.
// - all float parameters must use scr_float() function for output and thus "%s" pattern.
// - all score values should be Sint* type.
//////////////////////////////////////////////


struct SCR_PLAYER_PED {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_XYZ_f pos;
		uint16_t rot;
		int16_t remap;
	)
};
string read_PLAYER_PED(const FPStruct &params){
	get_data(SCR_PLAYER_PED, data);
	return sprintf_str("PLAYER_PED %s = (%s) %d %d", // must be 3d coords
		setgetname(VARNAME_PLAYER).c_str(), scr_coord_3d(data.pos).c_str(), data.remap, data.rot
	);
}
LinkToFunc(SCRCMD_PLAYER_PED, read_PLAYER_PED);




struct SCR_SOUND_DECSET {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk2;
		SCR_XYZ_f pos;
		uint8_t sound_id;
		uint8_t play_type;
		uint16_t padding;
	)
};
string read_SOUND_DECSET(const FPStruct &params){
	get_data(SCR_SOUND_DECSET, data);
	if(params.header->type == SCRCMD_SOUND_DECSET){
		return sprintf_str("SOUND %s = (%s) %s %s",
			setgetname(VARNAME_SOUND).c_str(), scr_coord_3d(data.pos).c_str(), enum_cstr(SOUNDS,data.sound_id), enum_cstr(PLAY_FOR,data.play_type)
		);
	}else{
		return sprintf_str("%s = CREATE_SOUND (%s) %s %s END",
			getname(data.varname).c_str(), scr_coord_3d(data.pos).c_str(), enum_cstr(SOUNDS,data.sound_id), enum_cstr(PLAY_FOR,data.play_type)
		);
	}
}
LinkToFunc(SCRCMD_SOUND_DECSET, read_SOUND_DECSET);
LinkToFunc(SCRCMD_CREATE_SOUND, read_SOUND_DECSET);

string read_SOUND(const FPStruct &params){ //get_data(SCR_SOUND_DECSET, data); // not needed
	return sprintf_str("SOUND %s", setgetname(VARNAME_SOUND+DEC_STR).c_str());
}
LinkToFunc(SCRCMD_SOUND, read_SOUND);




struct SCR_CONVEYOR {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_Rect_f rect;
		SCR_XY_c speed;
		uint16_t padding;
	)
};
string read_CONVEYOR(const FPStruct &params){
	if(params.header->type == SCRCMD_CONVEYOR_DEC){ //get_data(SCR_CONVEYOR, data); // not needed
		return sprintf_str("CONVEYOR %s", setgetname(VARNAME_CONVEYOR+DEC_STR).c_str());
	}else{
		get_data(SCR_CONVEYOR, data);
		return sprintf_str("CONVEYOR %s = (%s) (%s) %d %d",
			setgetname(VARNAME_CONVEYOR).c_str(), scr_coord(data.rect.pos).c_str(), scr_coord(data.rect.size).c_str(), data.speed.x, data.speed.y
		);
	}
}
LinkToFunc(SCRCMD_CONVEYOR_DECSET1, read_CONVEYOR);
LinkToFunc(SCRCMD_CONVEYOR_DECSET2, read_CONVEYOR);
LinkToFunc(SCRCMD_CONVEYOR_DEC, read_CONVEYOR);





struct SCR_OBJ_DATA {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk2;
		SCR_XYZ_f pos;
		uint16_t obj_id;
		uint16_t rot;
	)
};
string read_OBJ_DATA_DEC(const FPStruct &params){
	if(params.header->type == SCRCMD_OBJ_DEC){ //get_data(SCR_OBJ_DATA, data); // not needed
		return sprintf_str("OBJ_DATA %s", setgetname(VARNAME_OBJECT+DEC_STR).c_str());
	}else{
		get_data(SCR_OBJ_DATA, data);
		if(params.header->type == SCRCMD_OBJ_DECSET_2D || params.header->type == SCRCMD_OBJ_DECSET_3D){
			return sprintf_str("OBJ_DATA %s = (%s) %d %s",
				setgetname(VARNAME_OBJECT).c_str(), scr_coord(data.pos).c_str(), data.rot, enum_cstr(OBJECTS,data.obj_id)
			);
		}else{
			return sprintf_str("%s = CREATE_OBJ (%s) %d %s END",
				getname(data.varname).c_str(), scr_coord(data.pos).c_str(), data.rot, enum_cstr(OBJECTS,data.obj_id)
			);
		}
	}
}
LinkToFunc(SCRCMD_OBJ_DECSET_2D, read_OBJ_DATA_DEC);
LinkToFunc(SCRCMD_OBJ_DECSET_3D, read_OBJ_DATA_DEC);
LinkToFunc(SCRCMD_OBJ_DEC, read_OBJ_DATA_DEC);
LinkToFunc(SCRCMD_CREATE_OBJ_2D_STR, read_OBJ_DATA_DEC); // 2d, (no extra param)
LinkToFunc(SCRCMD_CREATE_OBJ_3D_INT, read_OBJ_DATA_DEC); // 3d, (no extra param)




struct SCR_OBJ_DATA_SHOP {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk3;
		SCR_XYZ_f pos;
		uint16_t obj_id;
		uint16_t rot;
		int16_t remap_or_shoptype;
		uint16_t padding;
	)
};
string getshopvalue(const FPStruct &params, const int16_t &value, const uint16_t &type1, const uint16_t &type2){
	if(params.header->type == type1 || params.header->type == type2){
		switch(value){ // pointless to create array with only 4 items in it and 250 of them empty!
			case 250: return "MACHINEGUN_SHOP";
			case 251: return "OILSLICK_SHOP";
			case 252: return "MINES_SHOP";
			case 254: return "BOMB_SHOP";
			default: {
				#ifdef SCR_INCLUDE_DEBUG
				if(SCR_use_debug){
					write_error_str(
						sprintf_str("Invalid index: '%d' at line '%d' in script file '%s' at cmd '%04X' with type '%04X'",
							value, __LINE__, params.filename.c_str(), uint16_t_flip(params.header->cmd_this), uint16_t_flip(params.header->type)
						)
					);
				}
				#endif
				return "<INVALID_INDEX>";
			}
		}
	}else{
		return sprintf_str("%d", value); // remap
	}
}
string read_OBJ_DATA_DEC_SHOP(const FPStruct &params){
	get_data(SCR_OBJ_DATA_SHOP, data);
	const string &name = (data.obj_id == 130) ? VARNAME_SHOP : VARNAME_OBJECT; // use shop name only if obj_id is "CAR_SHOP"
	return sprintf_str("OBJ_DATA %s = (%s) %d %s %s",
		setgetname(name).c_str(), scr_coord(data.pos).c_str(), data.rot, enum_cstr(OBJECTS,data.obj_id),
		getshopvalue(params,data.remap_or_shoptype,SCRCMD_OBJ_DECSET_2D_STR,SCRCMD_OBJ_DECSET_3D_STR).c_str()
	);
}
LinkToFunc(SCRCMD_OBJ_DECSET_2D_INT, read_OBJ_DATA_DEC_SHOP);
LinkToFunc(SCRCMD_OBJ_DECSET_3D_INT, read_OBJ_DATA_DEC_SHOP);
LinkToFunc(SCRCMD_OBJ_DECSET_2D_STR, read_OBJ_DATA_DEC_SHOP);
LinkToFunc(SCRCMD_OBJ_DECSET_3D_STR, read_OBJ_DATA_DEC_SHOP);

string read_CREATE_OBJECT_SHOP(const FPStruct &params){
	get_data(SCR_OBJ_DATA_SHOP, data);
	return sprintf_str("%s = CREATE_OBJ (%s) %d %s %s END",
		getname(data.varname).c_str(), scr_coord(data.pos).c_str(), data.rot, enum_cstr(OBJECTS,data.obj_id),
		getshopvalue(params,data.remap_or_shoptype,SCRCMD_CREATE_OBJ_3D,SCRCMD_CREATE_OBJ_3D_STR).c_str()
	);
}
LinkToFunc(SCRCMD_CREATE_OBJ_3D, read_CREATE_OBJECT_SHOP); // 2d, shopname
LinkToFunc(SCRCMD_CREATE_OBJ_3D_STR, read_CREATE_OBJECT_SHOP); // 3d, shopname
LinkToFunc(SCRCMD_CREATE_OBJ_2D_INT, read_CREATE_OBJECT_SHOP); // 2d, remap
LinkToFunc(SCRCMD_CREATE_OBJ_2D, read_CREATE_OBJECT_SHOP); // 3d, remap




struct SCR_GENERATOR {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_XYZ_f pos;
		uint16_t rot;
		uint16_t obj_id;
		uint16_t mindelay;
		uint16_t maxdelay;
		uint16_t ammo;
		uint16_t padding;
	)
};
string read_GENERATOR(const FPStruct &params){
	get_data(SCR_GENERATOR, data);
	if(params.header->type == SCRCMD_GENERATOR_DEC){
		return sprintf_str("GENERATOR %s", setgetname(VARNAME_GENERATOR+DEC_STR).c_str());
	}else{
		string optional;
		if(params.header->type == SCRCMD_GENERATOR_DECSET1 || params.header->type == SCRCMD_GENERATOR_DECSET2){
			optional = sprintf_str(" %d", data.ammo);
		}
		return sprintf_str("GENERATOR %s = (%s) %d %s %d %d%s",
			setgetname(VARNAME_GENERATOR).c_str(), scr_coord(data.pos).c_str(), data.rot, enum_cstr(OBJECTS,data.obj_id), data.mindelay, data.maxdelay, optional.c_str()
		);
	}
}
LinkToFunc(SCRCMD_GENERATOR_DECSET1, read_GENERATOR); // ammo 2d
LinkToFunc(SCRCMD_GENERATOR_DECSET2, read_GENERATOR); // ammo 3d
LinkToFunc(SCRCMD_GENERATOR_DECSET3, read_GENERATOR); // normal 2d
LinkToFunc(SCRCMD_GENERATOR_DECSET4, read_GENERATOR); // normal 3d
LinkToFunc(SCRCMD_GENERATOR_DEC, read_GENERATOR);




struct SCR_CAR_DATA_DEC {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk3;
		SCR_XYZ_f pos;
		uint16_t rot;
		int16_t remap;
		uint16_t car_id;
		uint16_t trailer_id; // 0xFFFF if no trailer, 0xFFFE if MINI_CAR.
	)
};
string read_CAR_DATA_DEC(const FPStruct &params){
	get_data(SCR_CAR_DATA_DEC, data);
	if(params.header->type == SCRCMD_CAR_DEC){
		return sprintf_str("CAR_DATA %s", setgetname(VARNAME_CAR+DEC_STR).c_str());
	}else{
		string datatype;
		if(params.header->type >= SCRCMD_PARKED_CAR_DECSET_2D && params.header->type <= SCRCMD_PARKED_CAR_DECSET_3D_STR){
			datatype = "PARKED_CAR_DATA";
		}else{
			datatype = "CAR_DATA";
		}
		string optional;
		if(data.trailer_id == 0xFFFE){ // note: compiler doesnt make MINI_CAR in car declarations!
			optional = " MINI_CAR";
		}else if(data.trailer_id != 0xFFFF){ // note: compiler doesnt make trailer in 2d car declarations!
			optional = string(" ")+enum_cstr(CARS,data.trailer_id);
		}
		return sprintf_str("%s %s = (%s) %d %d %s%s",
			datatype.c_str(), setgetname(VARNAME_CAR).c_str(), scr_coord(data.pos).c_str(), data.remap, data.rot, enum_cstr(CARS,data.car_id), optional.c_str()
		);
	}
}
LinkToFunc(SCRCMD_CAR_DEC, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_CAR_DECSET_2D, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_CAR_DECSET_3D, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_CAR_DECSET_2D_STR, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_CAR_DECSET_3D_STR, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_PARKED_CAR_DECSET_2D_STR, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_PARKED_CAR_DECSET_3D_STR, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_PARKED_CAR_DECSET_2D, read_CAR_DATA_DEC);
LinkToFunc(SCRCMD_PARKED_CAR_DECSET_3D, read_CAR_DATA_DEC);

string read_CREATE_CAR(const FPStruct &params){
	get_data(SCR_CAR_DATA_DEC, data);
	string cartype;
	if(params.header->type >= SCRCMD_CREATE_CAR_2D && params.header->type <= SCRCMD_CREATE_CAR_3D_STR){
		cartype = "CAR";
	}else{
		cartype = "GANG_CAR";
	}
	string optional;
	if(data.trailer_id == 0xFFFE){
		optional = " MINI_CAR";
	}else if(data.trailer_id != 0xFFFF){
		optional = string(" ")+enum_cstr(CARS,data.trailer_id);
	}
	return sprintf_str("%s = CREATE_%s (%s) %d %d %s%s END",
		getname(data.varname).c_str(), cartype.c_str(), scr_coord(data.pos).c_str(), data.remap, data.rot, enum_cstr(CARS,data.car_id), optional.c_str()
	);
}
LinkToFunc(SCRCMD_CREATE_CAR_2D, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_CAR_3D, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_CAR_2D_STR, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_CAR_3D_STR, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_GANG_CAR1, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_GANG_CAR2, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_GANG_CAR3, read_CREATE_CAR);
LinkToFunc(SCRCMD_CREATE_GANG_CAR4, read_CREATE_CAR);




struct SCR_SET_CAR_GRAPHIC {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk3;
		uint16_t graphic_id;
		uint16_t padding;
	)
};
string read_SET_CAR_GRAPHIC(const FPStruct &params){
	get_data(SCR_SET_CAR_GRAPHIC, data);
	return sprintf_str("SET_CAR_NUMBER_GRAPHIC (%s, %d)", getname(data.varname).c_str(), data.graphic_id);
}
LinkToFunc(SCRCMD_SET_CAR_GRAPHIC, read_SET_CAR_GRAPHIC);





struct SCR_CHAR_DEC { // for "CHAR_DATA var" declarations only.
	SCR_CMD_FORMAT( // this struct has 4 bytes more than CREATE_CHAR struct
		uint16_t unk2;
		uint16_t unk3;
		uint32_t unknown[6]; // these 24 bytes are null, impossible to know the format.
	)
};
struct SCR_CHAR_DATA_DEC {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk3;
		SCR_XYZ_f pos;
		uint16_t rot;
		int16_t remap; // can be -1 (FFFF)
		uint16_t occupation;
		uint16_t padding;
	)
};
string read_CHAR_DATA_DEC(const FPStruct &params){
	if(params.header->type == SCRCMD_CHAR_DEC){ //get_data(SCR_CHAR_DEC, data); // not needed
		return sprintf_str("CHAR_DATA %s", setgetname(VARNAME_CHAR+DEC_STR).c_str());
	}else{
		get_data(SCR_CHAR_DATA_DEC, data);
		return sprintf_str("CHAR_DATA %s = (%s) %d %d %s",
			setgetname(VARNAME_CHAR).c_str(), scr_coord(data.pos).c_str(), data.remap, data.rot, enum_cstr(OCCUPATIONS,data.occupation)
		);
	}
}
LinkToFunc(SCRCMD_CHAR_DECSET_2D, read_CHAR_DATA_DEC);
LinkToFunc(SCRCMD_CHAR_DECSET_3D, read_CHAR_DATA_DEC);
LinkToFunc(SCRCMD_CHAR_DEC, read_CHAR_DATA_DEC);

string read_CREATE_CHAR(const FPStruct &params){
	get_data(SCR_CHAR_DATA_DEC, data);
	return sprintf_str("%s = CREATE_CHAR (%s) %d %d %s END",
		getname(data.varname).c_str(), scr_coord(data.pos).c_str(), data.remap, data.rot, enum_cstr(OCCUPATIONS,data.occupation)
	);
}
LinkToFunc(SCRCMD_CREATE_CHAR_2D, read_CREATE_CHAR);
LinkToFunc(SCRCMD_CREATE_CHAR_3D, read_CREATE_CHAR);



struct SCR_SET_CHAR_GRAPHIC_TYPE {
	SCR_CMD_FORMAT(
		uint16_t varname;
		int16_t remap;
		uint16_t graphic_type;
		uint16_t padding;
	)
};
string read_SET_CHAR_GRAPHIC_TYPE(const FPStruct &params){
	get_data(SCR_SET_CHAR_GRAPHIC_TYPE, data);
	return sprintf_str("SET_CHAR_GRAPHIC_TYPE (%s, %s, %d)",
		getname(data.varname).c_str(), enum_cstr(CAR_OVERLAY,data.graphic_type), data.remap
	);
}
LinkToFunc(SCRCMD_PED_GRAPHIC, read_SET_CHAR_GRAPHIC_TYPE);





struct SCR_ADD_PATROL_POINT {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk3;
		SCR_XYZ_f pos;
	)
};
string read_ADD_PATROL_POINT(const FPStruct &params){
	get_data(SCR_ADD_PATROL_POINT, data);
	return sprintf_str("ADD_PATROL_POINT (%s, %s)",
		getname(data.varname).c_str(), scr_coord_3d(data.pos).c_str()
	);
}
LinkToFunc(SCRCMD_ADD_PATROL_POINT, read_ADD_PATROL_POINT);





struct SCR_DOOR_DATA_DEC {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_XYZ_uc block;
		uint8_t face;
		uint8_t gr_id;
		uint8_t open_type;
		uint8_t close_type;
		uint8_t delay;
		SCR_Rect_f check;
		uint8_t flip;
		uint8_t reversed;
		uint16_t somename; // id of car/model/char or integer value (int is not supported by gta2 according to docs)
	)
};
string get_door_opt_param(const FPStruct &params, const uint16_t &somename, const uint8_t &open_type, const int &type_varmodel, const int &type_intval){
	if(params.header->type == type_varmodel){
		if(open_type == 3){ // ONE_MODEL
			return string(" ")+enum_cstr(CARS,somename); // model
		}else{
			return string(" ")+getname(somename); // var
		}
	}else if(params.header->type == type_intval){
		return sprintf_str(" %d", somename); // int
	}else{
		return "";
	}
}
string read_DOOR_DATA_DEC(const FPStruct &params){
	get_data(SCR_DOOR_DATA_DEC, data);
	string optional, datatype;
	if(params.header->type >= SCRCMD_DOOR_DECLARE_S1 && params.header->type <= SCRCMD_DOOR_DECLARE_S3){
		optional = get_door_opt_param(params, data.somename, data.open_type, SCRCMD_DOOR_DECLARE_S2, SCRCMD_DOOR_DECLARE_S3);
		datatype = "SINGLE";
	}else{
		optional = get_door_opt_param(params, data.somename, data.open_type, SCRCMD_DOOR_DECLARE_D2, SCRCMD_DOOR_DECLARE_D3);
		datatype = "DOUBLE";
	}
	string posrect;
	if(data.check.size.x == 0 && data.check.size.y == 0){
		posrect = "0.0,0.0,0.0,0.0,0.0"; // make tighter packed parameters when there is no user-defined trigger. (easier to read).
	}else{
		posrect = scr_rect(data.check);
	}
	return sprintf_str("DOOR_DATA %s = %s (%s) (%s) %s %d %s %s %d %s %s%s", // must be 3d coords.
		setgetname(VARNAME_DOOR).c_str(), datatype.c_str(), scr_coord(data.block).c_str(), posrect.c_str(), enum_cstr(BLOCK_FACES,data.face), data.gr_id, enum_cstr(DOOR_OPENTYPES,data.open_type),
		enum_cstr(DOOR_CLOSETYPES,data.close_type), data.delay, enum_cstr(DOOR_FLIPTYPES,data.flip), enum_cstr(DOOR_REVERSED,data.reversed), optional.c_str()
	);
}
LinkToFunc(SCRCMD_DOOR_DECLARE_S1, read_DOOR_DATA_DEC);
LinkToFunc(SCRCMD_DOOR_DECLARE_S2, read_DOOR_DATA_DEC);
LinkToFunc(SCRCMD_DOOR_DECLARE_S3, read_DOOR_DATA_DEC);
LinkToFunc(SCRCMD_DOOR_DECLARE_D1, read_DOOR_DATA_DEC);
LinkToFunc(SCRCMD_DOOR_DECLARE_D2, read_DOOR_DATA_DEC);
LinkToFunc(SCRCMD_DOOR_DECLARE_D3, read_DOOR_DATA_DEC);








struct SCR_SET_STATION {
	SCR_CMD_FORMAT(
		uint16_t platform;
		uint8_t num_passenger;
		uint8_t num_freight;
		uint8_t num_boxcar;
		uint8_t padding1;
		uint16_t padding2;
	)
};
string read_SET_STATION(const FPStruct &params){
	get_data(SCR_SET_STATION, data);
	if(params.header->type == SCRCMD_SET_STATION){
		return sprintf_str("SET_STATION_INFO (%s, %d, %d, %d)", get_scr_string(data.platform).c_str(), data.num_passenger, data.num_freight, data.num_boxcar);
	}else{
		return sprintf_str("SET_STATION_INFO (%s, NO_TRAIN)", get_scr_string(data.platform).c_str());
	}
}
LinkToFunc(SCRCMD_SET_STATION, read_SET_STATION);
LinkToFunc(SCRCMD_SET_EMPTY_STATION, read_SET_STATION);



struct SCR_RADIOSTATION_DEC {
	SCR_CMD_FORMAT(
		SCR_XY_f pos;
		uint16_t station_type;
		uint16_t padding;
	)
};
string read_RADIOSTATION_DEC(const FPStruct &params){
	get_data(SCR_RADIOSTATION_DEC, data);
	return sprintf_str("RADIO_STATION %s = %s (%s)",
		setgetname(VARNAME_RADIO).c_str(), enum_cstr(RADIO_STATIONS,data.station_type), scr_coord(data.pos).c_str()
	);
}
LinkToFunc(SCRCMD_RADIOSTATION_DEC, read_RADIOSTATION_DEC);




struct SCR_SET_GANG_INFO {
	SCR_CMD_FORMAT(
		uint16_t gangname;
		int8_t remap;
		uint8_t weapon1;
		uint8_t weapon2;
		uint8_t weapon3;
		uint8_t arrow_id;
		uint8_t killchar;
		SCR_XYZ_f pos;
		uint16_t car_model;
		int16_t car_remap;
	)
};
string read_SET_GANG_INFO(const FPStruct &params){
	get_data(SCR_SET_GANG_INFO, data);
	return sprintf_str("SET_GANG_INFO (%s, %d, %s, %s, %s, %d, %s, %d, %s, %d)",
		get_scr_string(data.gangname).c_str(), data.remap, enum_cstr(WEAPONS,data.weapon1), enum_cstr(WEAPONS,data.weapon2), enum_cstr(WEAPONS,data.weapon3),
		data.arrow_id, scr_coord_3d(data.pos).c_str(), data.killchar, enum_cstr(CARS,data.car_model), data.car_remap
	);
}
LinkToFunc(SCRCMD_SET_GANG_INFO1, read_SET_GANG_INFO);





struct SCR_MAP_ZONE_SET {
	SCR_CMD_FORMAT(
		uint16_t car_density;
		uint16_t goodcar_ratio;
		uint16_t badcar_ratio;
		uint16_t policecar_ratio;
		uint16_t ped_density;
		uint16_t mugger_ratio;
		uint16_t carthief_ratio;
		uint16_t elvis_ratio;
		uint16_t gangchar_ratio;
		uint16_t policeped_ratio;
		uint16_t gangcar_ratio;
		uint16_t padding;
	)
};
string read_MAP_ZONE_SET(const FPStruct &params){
	if(params.header->type == SCRCMD_MAP_ZONE1){ //get_data(SCR_ZERO_PARAMS, data); // not used
		return sprintf_str("MAP_ZONE %s", get_scr_string(params.header->cmd_this).c_str());
	}else{
		get_data(SCR_MAP_ZONE_SET, data);
		string optional;
		if(data.gangcar_ratio > 0){
			optional = sprintf_str(", %d", data.gangcar_ratio);
		}
		return sprintf_str("MAP_ZONE %s = (%d, %d, %d, %d, %d, %d, %d, %d, %d, %d%s)",
			get_scr_string(params.header->cmd_this).c_str(),
			data.car_density, data.goodcar_ratio, data.badcar_ratio, data.policecar_ratio, data.ped_density, data.mugger_ratio,
			data.carthief_ratio, data.elvis_ratio, data.gangchar_ratio, data.policeped_ratio, optional.c_str()
		);
	}
}
LinkToFunc(SCRCMD_MAP_ZONE_SET, read_MAP_ZONE_SET);
LinkToFunc(SCRCMD_MAP_ZONE1, read_MAP_ZONE_SET);







string get_homecrane_str(const uint16_t &homecrane, const FPStruct &params){
	return (homecrane == 0) ? "NO_HOMECRANE" : getname(homecrane);
}
struct SCR_CRANE_TARGET_DEC {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t homecrane;
		uint16_t unk5;
		SCR_XY_f pos;
		uint16_t home_rotation;
		uint16_t home_cranetype; // "first" or "second"
		SCR_XY_f target_pos;
		uint16_t target_rotation;
		uint16_t unk6;
	)
};
string read_CRANE_DECSET(const FPStruct &params){
	get_data(SCR_CRANE_TARGET_DEC, data);
	string homecrane = get_homecrane_str(data.homecrane, params);
	return sprintf_str("CRANE_DATA %s = (%s) %d %s %s (%s) %d",
		setgetname(VARNAME_CRANE).c_str(), scr_coord(data.pos).c_str(), data.home_rotation, homecrane.c_str(),
		enum_cstr(CRANETYPES,data.home_cranetype), scr_coord(data.target_pos).c_str(), data.target_rotation
	);
}
LinkToFunc(SCRCMD_CRANE_TARGET_DEC, read_CRANE_DECSET);

struct SCR_CRANE2TARGET_DEC {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t homecrane;
		uint16_t unk5;
		SCR_XY_f pos;
		uint16_t home_rotation;
		uint16_t padding;
	)
};
string read_CRANE2TARGET_DEC(const FPStruct &params){
	get_data(SCR_CRANE2TARGET_DEC, data);
	string homecrane = get_homecrane_str(data.homecrane, params);
	return sprintf_str("CRANE_DATA %s = (%s) %d %s",
		setgetname(VARNAME_CRANE).c_str(), scr_coord(data.pos).c_str(), data.home_rotation, homecrane.c_str()
	);
}
LinkToFunc(SCRCMD_CRANE2TARGET_DEC, read_CRANE2TARGET_DEC);

struct SCR_CRANE_BASIC_DEC {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t homecrane;
		uint16_t unk5;
		SCR_XY_f pos;
		uint16_t home_rotation;
		uint16_t home_cranetype; // "first" or "second"
		SCR_XY_f target_pos;
		uint16_t target_rotation;
		uint16_t second_rotation;
		SCR_XY_f second_pos;
	)
};
string read_CRANE_BASIC_DEC(const FPStruct &params){
	get_data(SCR_CRANE_BASIC_DEC, data);
	string homecrane = get_homecrane_str(data.homecrane, params);
	return sprintf_str("CRANE_DATA %s = (%s) %d %s %s (%s) %d SECOND (%s) %d",
		setgetname(VARNAME_CRANE).c_str(), scr_coord(data.pos).c_str(), data.home_rotation, homecrane.c_str(),
		enum_cstr(CRANETYPES,data.home_cranetype), scr_coord(data.target_pos).c_str(), data.target_rotation,
		scr_coord(data.second_pos).c_str(), data.second_rotation
	);
}
LinkToFunc(SCRCMD_CRANE_BASIC_DEC, read_CRANE_BASIC_DEC);

struct SCR_CRANE_DEC {
	SCR_CMD_FORMAT(
		uint16_t unknown[10]; // probably same struct as SCR_CRANE2TARGET_DEC (same size)
	)
};
string read_CRANE_DEC(const FPStruct &params){ //get_data(SCR_CRANE_DEC, data); // not needed
	return sprintf_str("CRANE_DATA %s", setgetname(VARNAME_CRANE).c_str());
}
LinkToFunc(SCRCMD_CRANE_DEC, read_CRANE_DEC);




struct SCR_DECLARE_CRANE_POWERUP {
	SCR_CMD_FORMAT(
		uint16_t crane_name;
		uint16_t generator_name;
		SCR_XYZ_uc pos;
		uint8_t padding;
	)
};
string read_DECLARE_CRANE_POWERUP(const FPStruct &params){
	get_data(SCR_DECLARE_CRANE_POWERUP, data);
	return sprintf_str("DECLARE_CRANE_POWERUP (%s, %s, %s)",
		getname(data.crane_name).c_str(), getname(data.generator_name).c_str(), scr_coord(data.pos).c_str()
	);
}
LinkToFunc(SCRCMD_DO_CRANE_POWERUP, read_DECLARE_CRANE_POWERUP);





struct SCR_CRUSHER_BASIC {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_XY_f pos;
	)
};
string read_CRUSHER_BASIC(const FPStruct &params){
	get_data(SCR_CRUSHER_BASIC, data);
	return sprintf_str("CRUSHER %s = (%s)", setgetname(VARNAME_CRUSHER).c_str(), scr_coord(data.pos).c_str());
}
LinkToFunc(SCRCMD_CRUSHER_BASIC, read_CRUSHER_BASIC);





struct SCR_DESTRUCTOR {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		SCR_Rect_f rect;
	)
};
string read_DESTRUCTOR(const FPStruct &params){
	get_data(SCR_DESTRUCTOR, data);
	if(params.header->type == SCRCMD_DESTRUCTOR_DEC){
		return sprintf_str("DESTRUCTOR %s", setgetname(VARNAME_DESTRUCTOR+DEC_STR).c_str()); // this probably never used.
	}else{
		return sprintf_str("DESTRUCTOR %s = (%s) (%s)", setgetname(VARNAME_DESTRUCTOR).c_str(), scr_coord(data.rect.pos).c_str(), scr_coord(data.rect.size).c_str());
	}
}
LinkToFunc(SCRCMD_DESTRUCTOR_DECSET1, read_DESTRUCTOR);
LinkToFunc(SCRCMD_DESTRUCTOR_DECSET2, read_DESTRUCTOR);
LinkToFunc(SCRCMD_DESTRUCTOR_DEC, read_DESTRUCTOR);
// NOTE: create_destructor doesnt even get compiled into anything! also doesnt even work in gta2 (with hacked SCR).




struct SCR_LOWER_LEVEL {
	SCR_CMD_FORMAT(
		SCR_XY_uc pos1;
		SCR_XY_uc pos2;
	)
};
string read_LOWER_LEVEL(const FPStruct &params){
	get_data(SCR_LOWER_LEVEL, data);
	return sprintf_str("LOWER_LEVEL (%s) (%s)", scr_coord(data.pos1).c_str(), scr_coord(data.pos2).c_str());
}
LinkToFunc(SCRCMD_LOWER_LEVEL, read_LOWER_LEVEL);




struct SCR_EXPLODE {
	SCR_CMD_FORMAT(
		SCR_XYZ_f pos;
	)
};
struct SCR_EXPLODE_WALL {
	SCR_CMD_FORMAT(
		SCR_XYZ_f pos;
		uint16_t face;
		uint16_t padding2;
	)
};
string read_EXPLODE(const FPStruct &params){
	if(params.header->type == SCRCMD_EXPLODE_BUILDING){
		get_data(SCR_EXPLODE_WALL, data);
		return sprintf_str("EXPLODE_WALL (%s) %s", scr_coord_3d(data.pos).c_str(), enum_cstr(BLOCK_FACES,data.face));
	}else{
		get_data(SCR_EXPLODE, data);
		string exptype;
		if(params.header->type == SCRCMD_EXPLODE){
			exptype = "EXPLODE";
		}else if(params.header->type == SCRCMD_EXPLODE_SMALL2){
			exptype = "EXPLODE_SMALL";
		}else if(params.header->type == SCRCMD_EXPLODE_NO_RING2){
			exptype = "EXPLODE_NO_RING";
		}else if(params.header->type == SCRCMD_EXPLODE_LARGE2){
			exptype = "EXPLODE_LARGE";
		}
		return sprintf_str("%s (%s)", exptype.c_str(), scr_coord_3d(data.pos).c_str());
	}
}
LinkToFunc(SCRCMD_EXPLODE, read_EXPLODE);
LinkToFunc(SCRCMD_EXPLODE_SMALL2, read_EXPLODE);
LinkToFunc(SCRCMD_EXPLODE_NO_RING2, read_EXPLODE);
LinkToFunc(SCRCMD_EXPLODE_LARGE2, read_EXPLODE);
LinkToFunc(SCRCMD_EXPLODE_BUILDING, read_EXPLODE);



struct SCR_CHANGE_BLOCK {
	SCR_CMD_FORMAT(
		SCR_XYZ_uc pos;
		uint8_t sidetype;
		union {
			struct {
				uint16_t type:2;
				uint16_t slope:6;
				uint16_t unused:8;
			};
			struct {
				uint16_t tile_id:10;
				uint16_t lighting:2;
				uint16_t flat:1;
				uint16_t flip:1;
				uint16_t rot:2;
			} lid;
			struct {
				uint16_t tile_id:10;
				uint16_t wall_type:1;
				uint16_t bullet_type:1;
				uint16_t flat:1;
				uint16_t flip:1;
				uint16_t rot:2;
			} side;
		} block;
		uint16_t padding2;
	)
};
string read_CHANGE_BLOCK(const FPStruct &params){
	get_data(SCR_CHANGE_BLOCK, data);
	if(params.header->type == SCRCMD_CHANGE_BLOCK_LID){
		return sprintf_str("CHANGE_BLOCK LID (%s) %s %s %d %s %d",
			scr_coord(data.pos).c_str(), enum_cstr(BLOCK_FLATTYPES,data.block.lid.flat), enum_cstr(BLOCK_FLIPTYPES,data.block.lid.flip),
			data.block.lid.lighting, enum_cstr(ROT_LIST,data.block.lid.rot), data.block.lid.tile_id
		);
	}else if(params.header->type == SCRCMD_CHANGE_BLOCK_SIDE){
		return sprintf_str("CHANGE_BLOCK SIDE (%s) %s %s %s %s %s %s %d",
			scr_coord(data.pos).c_str(), enum_cstr(BLOCK_FACES,data.sidetype), enum_cstr(WALL_TYPES,data.block.side.wall_type),
			enum_cstr(BULLETWALL_TYPES,data.block.side.bullet_type), enum_cstr(BLOCK_FLATTYPES,data.block.side.flat),
			enum_cstr(BLOCK_FLIPTYPES,data.block.side.flip), enum_cstr(ROT_LIST,data.block.side.rot), data.block.side.tile_id
		);
	}else{
		return sprintf_str("CHANGE_BLOCK TYPE (%s) %s %d",
			scr_coord(data.pos).c_str(), enum_cstr(BLOCKTYPES,data.block.type), data.block.slope
		);
	}
}
LinkToFunc(SCRCMD_CHANGE_BLOCK_LID, read_CHANGE_BLOCK);
LinkToFunc(SCRCMD_CHANGE_BLOCK_SIDE, read_CHANGE_BLOCK);
LinkToFunc(SCRCMD_CHANGE_BLOCK_TYPE, read_CHANGE_BLOCK);






struct SCR_REMOVE_BLOCK {
	SCR_CMD_FORMAT(
		SCR_XYZ_uc pos;
		uint8_t dropval;
	)
};
string read_REMOVE_BLOCK(const FPStruct &params){
	get_data(SCR_REMOVE_BLOCK, data);
	return sprintf_str("REMOVE_BLOCK (%s, %s)", scr_coord(data.pos).c_str(), enum_cstr(DROPS,data.dropval));
}
LinkToFunc(SCRCMD_REMOVE_BLOCK, read_REMOVE_BLOCK);

struct SCR_ADD_NEW_BLOCK {
	SCR_CMD_FORMAT(
		SCR_XYZ_uc pos;
		uint8_t unk1;
		uint16_t unk2;
		uint16_t unk3;
	)
};
string read_ADD_NEW_BLOCK(const FPStruct &params){
	get_data(SCR_ADD_NEW_BLOCK, data);
	return sprintf_str("ADD_NEW_BLOCK (%s)", scr_coord(data.pos).c_str());
}
LinkToFunc(SCRCMD_ADD_NEW_BLOCK, read_ADD_NEW_BLOCK);




struct SCR_LIGHT {
	SCR_CMD_FORMAT(
		uint16_t varname;
		int16_t unk3;
		SCR_XYZ_f pos;
		SCR_XYZ_uc color;
		uint8_t padding;
		SCR_f radius;
		uint8_t intensity;
		uint8_t on_delay;
		uint8_t off_delay;
		uint8_t random_value;
	)
};
string read_LIGHT(const FPStruct &params){
	get_data(SCR_LIGHT, data);
	string prefix;
	if(params.header->type == SCRCMD_CREATE_LIGHT2 || params.header->type == SCRCMD_LIGHT_DEC){
		prefix = sprintf_str("LIGHT %s =", setgetname(VARNAME_LIGHT).c_str());
	}else{
		prefix = sprintf_str("%s = CREATE_LIGHT", getname(data.varname).c_str());
	}
	string endparams;
	if(params.header->type != SCRCMD_LIGHT_DEC && params.header->type != SCRCMD_CREATE_LIGHT1){
		endparams = sprintf_str(" %d %d %d", data.on_delay, data.off_delay, data.random_value);
	}
	return sprintf_str("%s (%s) %s %d (%s)%s", // no "END" at all if using CREATE_LIGHT! weird.
		prefix.c_str(), scr_coord_3d(data.pos).c_str(), scr_float(data.radius), data.intensity, scr_color_rgb(data.color).c_str(), endparams.c_str()
	);
}
LinkToFunc(SCRCMD_CREATE_LIGHT2, read_LIGHT);
LinkToFunc(SCRCMD_LIGHT_DECSET2, read_LIGHT); // CREATE_LIGHT
LinkToFunc(SCRCMD_LIGHT_DEC, read_LIGHT); // this doesnt have the last 3 variables!
LinkToFunc(SCRCMD_CREATE_LIGHT1, read_LIGHT); // this doesnt have the last 3 variables!


string read_LIGHT_DECSET1(const FPStruct &params){ //get_data(SCR_TWO_PARAMS, data); // not used
	return sprintf_str("LIGHT %s", setgetname(VARNAME_LIGHT+DEC_STR).c_str());
}
LinkToFunc(SCRCMD_LIGHT_DECSET1, read_LIGHT_DECSET1);






string read_SAVED_COUNTER(const FPStruct &params){
	if(params.header->type == SCRCMD_COUNTER_SAVE){ //get_data(SCR_TWO_PARAMS, data); // not sure anymore did this have this struct or something else!
		return sprintf_str("SAVED_COUNTER %s", setgetname(VARNAME_SAVED_COUNTER+DEC_STR).c_str());
	}else{
		get_data(SCR_TWO_PARAMS, data);
		return sprintf_str("SAVED_COUNTER %s = %d", setgetname(VARNAME_SAVED_COUNTER).c_str(), data.sint32val);
	}
}
LinkToFunc(SCRCMD_COUNTER_SAVE, read_SAVED_COUNTER);
LinkToFunc(SCRCMD_COUNTER_SET_SAVE, read_SAVED_COUNTER);




struct SCR_DECLARE_POWERUP_CARLIST {
	SCR_CMD_FORMAT(
		uint8_t car[19];
		uint8_t padding;
	)
};
string read_DECLARE_POWERUP_CARLIST(const FPStruct &params){
	get_data(SCR_DECLARE_POWERUP_CARLIST, data);
	string carlist;
	const int total_cars = arrlen(data.car);
	for(int u = 0; u < total_cars; u++){
		carlist += enum_cstr(CARS,data.car[u]);
		if(u < total_cars-1) carlist += ", ";
	}
	return sprintf_str("DECLARE_POWERUP_CARLIST (%s)", carlist.c_str());
}
LinkToFunc(SCRCMD_DECLARE_CARLIST, read_DECLARE_POWERUP_CARLIST);




struct SCR_SET_AMBIENT {
	SCR_CMD_FORMAT(
		SCR_f level;
		uint16_t time;
		uint16_t padding;
	)
};
string read_SET_AMBIENT(const FPStruct &params){
	get_data(SCR_SET_AMBIENT, data);
	return sprintf_str("SET_AMBIENT_LEVEL (%s, %d)", scr_float(data.level), data.time);
}
LinkToFunc(SCRCMD_SET_AMBIENT, read_SET_AMBIENT);



struct SCR_THREAD_DECLARE1 {
	SCR_CMD_FORMAT(
		uint16_t unknown[4];
	)
};
string read_THREAD_DECLARE1(const FPStruct &params){ //get_data(SCR_THREAD_DECLARE1, data); // not used
	return sprintf_str("THREAD_TRIGGER %s", setgetname(VARNAME_THREAD_TRIGGER+DEC_STR).c_str());
}
LinkToFunc(SCRCMD_THREAD_DECLARE1, read_THREAD_DECLARE1);

string read_THREAD_ID(const FPStruct &params){ //get_data(SCR_THREAD_DECLARE1, data); // not used - note: the struct was same size, probably using same struct.
	return sprintf_str("THREAD_ID %s", setgetname(VARNAME_THREAD_ID).c_str());
}
LinkToFunc(SCRCMD_THREAD_ID, read_THREAD_ID);




struct SCR_POINT_ARROW_3D {
	SCR_CMD_FORMAT(
		SCR_XYZ_f pos;
		uint16_t arrowname;
		uint16_t padding;
	)
};
string read_POINT_ARROW_AT(const FPStruct &params){
	if(params.header->type == SCRCMD_POINT_ARROW_AT){
		get_data(SCR_FOUR_PARAMS, data);
		return sprintf_str("POINT_ARROW_AT (%s, %s)", getname(data.var3).c_str(), getname(data.var1).c_str());
	}else{
		get_data(SCR_POINT_ARROW_3D, data);
		return sprintf_str("POINT_ARROW_AT (%s, %s)", getname(data.arrowname).c_str(), scr_coord_3d(data.pos).c_str());
	}
}
LinkToFunc(SCRCMD_POINT_ARROW_AT, read_POINT_ARROW_AT);
LinkToFunc(SCRCMD_POINT_ARROW_3D, read_POINT_ARROW_AT);




struct SCR_ANSWER_PHONE {
	SCR_CMD_FORMAT(
		uint16_t objname;
		uint16_t unk2;
		uint16_t charname;
		uint16_t unk3;
		int16_t value;
		uint16_t unk4;
	)
};
string read_ANSWER_PHONE(const FPStruct &params){
	get_data(SCR_ANSWER_PHONE, data);
	return sprintf_str("ANSWER_PHONE (%s, %s, %d)", getname(data.charname).c_str(), getname(data.objname).c_str(), data.value);
}
LinkToFunc(SCRCMD_ANSWER_PHONE, read_ANSWER_PHONE);






struct SCR_COLOUR_RADIUS {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk2;
		union { // 32bits
			struct {
				SCR_XYZ_uc color;
				uint8_t padding;
			};
			SCR_f radius;
		};
	)
};
string read_CHANGE_COLOUR(const FPStruct &params){
	get_data(SCR_COLOUR_RADIUS, data);
	return sprintf_str("CHANGE_COLOUR (%s, %s)", getname(data.varname).c_str(), scr_color_rgb(data.color).c_str());
}
LinkToFunc(SCRCMD_CHANGE_COLOUR, read_CHANGE_COLOUR);

string read_CHANGE_RADIUS(const FPStruct &params){
	get_data(SCR_COLOUR_RADIUS, data);
	return sprintf_str("CHANGE_RADIUS (%s, %s)", getname(data.varname).c_str(), scr_float(data.radius));
}
LinkToFunc(SCRCMD_CHANGE_RADIUS, read_CHANGE_RADIUS);




struct SCR_SET_DIR_OF_TVVAN {
	SCR_CMD_FORMAT(
		SCR_XY_f pos;
		uint16_t unk2;
		uint16_t unk3;
	)
};
string read_SET_DIR_OF_TVVAN(const FPStruct &params){
	get_data(SCR_SET_DIR_OF_TVVAN, data);
	return sprintf_str("SET_DIR_OF_TV_VANS (%s)", scr_coord(data.pos).c_str());
}
LinkToFunc(SCRCMD_SET_DIR_OF_TVVAN, read_SET_DIR_OF_TVVAN);




struct SCR_CHAR_INTO_CAR {
	SCR_CMD_FORMAT(
		uint16_t charname;
		uint16_t unk2;
		int16_t remap;
		uint16_t occupation;
		uint16_t carname;
		uint16_t unk3;
	)
};
string read_CHAR_INTO_CAR(const FPStruct &params){
	get_data(SCR_CHAR_INTO_CAR, data);
	return sprintf_str("%s = CREATE_CHAR_INSIDE_CAR (%s) %d %s END",
		getname(data.charname).c_str(), getname(data.carname).c_str(), data.remap, enum_cstr(OCCUPATIONS,data.occupation)
	);
}
LinkToFunc(SCRCMD_CHAR_INTO_CAR, read_CHAR_INTO_CAR);





struct SCR_WARP_CHAR {
	SCR_CMD_FORMAT(
		uint16_t charname;
		uint16_t rot;
		SCR_XYZ_f pos;
	)
};
string read_WARP_CHAR(const FPStruct &params){
	get_data(SCR_WARP_CHAR, data);
	return sprintf_str("WARP_FROM_CAR_TO_POINT (%s, %s, %d)", getname(data.charname).c_str(), scr_coord_3d(data.pos).c_str(), data.rot);
}
LinkToFunc(SCRCMD_WARP_CHAR, read_WARP_CHAR);





struct SCR_LEVEL_END_ARROW2 {
	SCR_CMD_FORMAT(
		SCR_XYZ_f pos;
		uint16_t arrowname;
		uint16_t padding;
	)
};
string read_LEVEL_END_ARROW2(const FPStruct &params){
	get_data(SCR_LEVEL_END_ARROW2, data);
	return sprintf_str("LEVEL_END_POINT_ARROW_AT (%s, %s)", getname(data.arrowname).c_str(), scr_coord_3d(data.pos).c_str());
}
LinkToFunc(SCRCMD_LEVEL_END_ARROW2, read_LEVEL_END_ARROW2);




struct SCR_DO_SAVE_GAME {
	SCR_CMD_FORMAT(
		uint16_t triggername;
		uint16_t unk2;
		SCR_Rect_f rect;
	)
};
string read_DO_SAVE_GAME(const FPStruct &params){
	get_data(SCR_DO_SAVE_GAME, data);
	return sprintf_str("PERFORM_SAVE_GAME (%s, %s)", getname(data.triggername).c_str(), scr_rect(data.rect).c_str());
}
LinkToFunc(SCRCMD_DO_SAVE_GAME, read_DO_SAVE_GAME);





struct SCR_SET_CHAR_OBJ3 {
	SCR_CMD_FORMAT(
		uint16_t charname;
		uint16_t objective;
		SCR_XYZ_f pos;
	)
};
string read_SET_CHAR_OBJ3(const FPStruct &params){
	get_data(SCR_SET_CHAR_OBJ3, data);
	return sprintf_str("SET_CHAR_OBJECTIVE (%s, %s, %s)", getname(data.charname).c_str(), enum_cstr(OBJECTIVES,data.objective), scr_coord_3d(data.pos).c_str());
}
LinkToFunc(SCRCMD_SET_CHAR_OBJ3, read_SET_CHAR_OBJ3);

struct SCR_SET_CHAR_OBJ_FOLLOW {
	SCR_CMD_FORMAT(
		uint16_t charname;
		uint16_t objective;
		uint16_t carname;
		uint16_t rot;
		SCR_f offset;
	)
};
string read_SET_CHAR_OBJ_FOLLOW(const FPStruct &params){
	get_data(SCR_SET_CHAR_OBJ_FOLLOW, data);
	return sprintf_str("SET_CHAR_OBJECTIVE (%s, %s, %s, %d, %s)",
		getname(data.charname).c_str(), enum_cstr(OBJECTIVES,data.objective), getname(data.carname).c_str(), data.rot, scr_float(data.offset)
	);
}
LinkToFunc(SCRCMD_SET_CHAR_OBJ_FOLLOW, read_SET_CHAR_OBJ_FOLLOW);





struct SCR_THREAD_TRIGGER_3_PARAMS {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t unk4;
		uint16_t funcname;
		uint16_t charname;
		uint16_t varname;
	)
};
string read_THREAD_DECLARE4(const FPStruct &params){
	get_data(SCR_THREAD_TRIGGER_3_PARAMS, data);
	string atwhere = (params.header->type == SCRCMD_THREAD_DECLARE4) ? "ANSWER_PHONE" : "CHAR_IN_CAR";
	return sprintf_str("THREAD_TRIGGER %s = THREAD_WAIT_FOR_%s (%s, %s, %s:)",
		setgetname(VARNAME_THREAD_TRIGGER).c_str(), atwhere.c_str(), getname(data.charname).c_str(), getname(data.varname).c_str(), getfuncname(data.funcname).c_str()
	);
}
LinkToFunc(SCRCMD_THREAD_DECLARE4, read_THREAD_DECLARE4);
LinkToFunc(SCRCMD_THREAD_DECLARE2, read_THREAD_DECLARE4);

struct SCR_THREAD_TRIGGER_BLOCK {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t unk4;
		uint16_t funcname;
		uint16_t charname;
		SCR_XYZ_uc pos;
		uint8_t unk5;
		uint16_t unk6;
	)
};
string read_THREAD_DECLARE3(const FPStruct &params){
	get_data(SCR_THREAD_TRIGGER_BLOCK, data);
	return sprintf_str("THREAD_TRIGGER %s = THREAD_WAIT_FOR_CHAR_IN_BLOCK (%s, %s, %s:)",
		setgetname(VARNAME_THREAD_TRIGGER).c_str(), getname(data.charname).c_str(), scr_coord(data.pos).c_str(), getfuncname(data.funcname).c_str()
	);
}
LinkToFunc(SCRCMD_THREAD_DECLARE3, read_THREAD_DECLARE3);

struct SCR_THREAD_TRIGGER_AREA {
	SCR_CMD_FORMAT(
		uint16_t unk2;
		uint16_t unk3;
		uint16_t unk4;
		uint16_t funcname;
		SCR_Rect_f rect;
		uint16_t charname;
		uint16_t unk5;
	)
};
string read_THREAD_DECLARE5(const FPStruct &params){
	get_data(SCR_THREAD_TRIGGER_AREA, data);
	string means = (params.header->type == SCRCMD_CHAR_AREA_ANY_MEANS) ? "_ANY_MEANS" : "";
	return sprintf_str("THREAD_TRIGGER %s = THREAD_WAIT_FOR_CHAR_IN_AREA%s (%s, %s, %s:)",
		setgetname(VARNAME_THREAD_TRIGGER).c_str(), means.c_str(), getname(data.charname).c_str(), scr_rect(data.rect).c_str(), getfuncname(data.funcname).c_str()
	);
}
LinkToFunc(SCRCMD_THREAD_DECLARE5, read_THREAD_DECLARE5);
LinkToFunc(SCRCMD_CHAR_AREA_ANY_MEANS, read_THREAD_DECLARE5);





struct SCR_START_BASIC_KF_TEMPLATE {
	SCR_CMD_FORMAT(
		uint16_t triggername;
		uint16_t brief_id;
		uint16_t objname;
		uint16_t playername;
		uint16_t weapon;
		uint16_t unk2;
	)
};
string read_START_BASIC_KF_TEMPLATE(const FPStruct &params){
	get_data(SCR_START_BASIC_KF_TEMPLATE, data);
	return sprintf_str("START_BASIC_KF_TEMPLATE (%s, %d, %s, %s, %s)",
		getname(data.triggername).c_str(), data.brief_id, getname(data.objname).c_str(), getname(data.playername).c_str(), enum_cstr(WEAPONS,data.weapon)
	);
}
LinkToFunc(SCRCMD_START_BASIC_KF, read_START_BASIC_KF_TEMPLATE);

struct SCR_DO_BASIC_KF_TEMPLATE {
	SCR_CMD_FORMAT(
		uint16_t bonusname;
		uint16_t timername;
		uint16_t time_limit;
		uint16_t unk2;
		uint16_t onscreenname;
		uint16_t countername;
		uint16_t target_total;
		uint16_t brief_id;
		uint16_t playername;
		uint16_t rewardtype;
		uint32_t rewardvalue;
	)
};
string read_DO_BASIC_KF_TEMPLATE(const FPStruct &params){
	get_data(SCR_DO_BASIC_KF_TEMPLATE, data);
	return sprintf_str("DO_BASIC_KF_TEMPLATE (%s, %s, %d, %s, %s, %d, %d, %s, %s, %d)",
		getname(data.bonusname).c_str(), getname(data.timername).c_str(), data.time_limit, getname(data.onscreenname).c_str(), getname(data.countername).c_str(),
		data.target_total, data.brief_id, getname(data.playername).c_str(), enum_cstr(BONUS_WHICH,data.rewardtype), data.rewardvalue
	);
}
LinkToFunc(SCRCMD_DO_BASIC_KF, read_DO_BASIC_KF_TEMPLATE);





struct SCR_START_BONUS {
	SCR_CMD_FORMAT(
		uint16_t zonename;
		struct {
			uint8_t is_chartype:1;
			uint8_t is_gangtype:1;
			uint8_t unk2:6;
		};
		int8_t remap;
		uint8_t count;
		uint8_t exclusive;
		union {
			uint16_t gangname;
			uint16_t occupation;
			uint16_t target_model;
		};
		uint16_t time;
		uint16_t car_model;
		int32_t score;
		uint16_t varname;
		uint16_t by_weapon;
	)
};
string read_START_BONUS(const FPStruct &params){
	get_data(SCR_START_BONUS, data);
	string zonename = (data.zonename == 0xFFFF) ? "NO_ZONE" : get_scr_string(data.zonename);
	string extra1;
	string type = data.is_chartype ? "CHAR" : "CAR";
	if(data.is_gangtype){
		extra1 = (data.gangname == 0xFFFF) ? "NONE" : get_scr_string(data.gangname);
	}else{
		if(data.is_chartype){
			extra1 = (data.occupation == 0xFFFF) ? "NONE" : enum_cstr(OCCUPATIONS,data.occupation);
		}else{
			extra1 = (data.target_model == 0xFFFF) ? "NONE" : enum_cstr(CARS,data.target_model);
		}
	}
	string lastparams;
	if(params.header->type == SCRCMD_START_BONUS1){
		lastparams = extra1;
	}else if(params.header->type == SCRCMD_START_BONUS2){
		lastparams = extra1 + ", NONE";
	}else if(params.header->type == SCRCMD_START_BONUS3){
		string remap = (data.remap == 0xFE) ? "NONE" : sprintf_str("%d", data.remap);
		lastparams = extra1 + ", " + remap;
	}
	string car_model = (data.car_model == 0xFFFF) ? "NONE" : enum_cstr(CARS,data.car_model);
	return sprintf_str("%s = START_BONUS_CHECK (%s, %d, %d, %d, %s, %s, %s, %s, %s)",
		getname(data.varname).c_str(), zonename.c_str(), data.time, data.count, data.score, type.c_str(),
		enum_cstr(EXCLUSIVE,data.exclusive), enum_cstr(KILLED_BY,data.by_weapon), car_model.c_str(), lastparams.c_str()
	);
}
LinkToFunc(SCRCMD_START_BONUS1, read_START_BONUS);
LinkToFunc(SCRCMD_START_BONUS2, read_START_BONUS);
LinkToFunc(SCRCMD_START_BONUS3, read_START_BONUS);
//LinkToFunc(SCRCMD_START_BONUS4, read_START_BONUS); // havent been able to produce this command in SCR!






struct SCR_PHONE_TEMPLATE {
	SCR_CMD_FORMAT(
		uint16_t brief_id;
		uint16_t counter1;
		uint16_t counter3b; // DO_PHONE_TEMPLATE has different counters in different order in the data! (maybe should make own struct for it)
		uint16_t mission2;
		uint16_t counter2;
		uint16_t counter3;
		uint16_t counter4;
		uint16_t counter5;
		uint16_t gangname;
		uint16_t respect;
		uint16_t mission1;
		uint16_t padding;
	)
};
string read_PHONE_TEMPLATE(const FPStruct &params){
	get_data(SCR_PHONE_TEMPLATE, data);
	if(params.header->type == SCRCMD_DO_EASY_PHONE){
		return sprintf_str("DO_EASY_PHONE_TEMPLATE (%d, %s, %s, %s, %s, %s, %s, %s, %d)",
			data.brief_id, get_scr_string(data.mission1).c_str(), getname(data.counter1).c_str(), getname(data.counter2).c_str(), getname(data.counter3).c_str(),
			getname(data.counter4).c_str(), getname(data.counter5).c_str(), get_scr_string(data.gangname).c_str(), data.respect
		);
	}else{
		return sprintf_str("DO_PHONE_TEMPLATE (%d, %s, %s, %s, %s, %s, %s, %s, %s, %s, %d)",
			data.brief_id, get_scr_string(data.mission1).c_str(), get_scr_string(data.mission2).c_str(), getname(data.counter1).c_str(),
			getname(data.counter2).c_str(), getname(data.counter3b).c_str(), getname(data.counter3).c_str(), getname(data.counter4).c_str(),
			getname(data.counter5).c_str(), get_scr_string(data.gangname).c_str(), data.respect
		);
	}
}
LinkToFunc(SCRCMD_DO_EASY_PHONE, read_PHONE_TEMPLATE);
LinkToFunc(SCRCMD_PHONE_TEMPLATE, read_PHONE_TEMPLATE);




// this command is undocumented and Sektor said its not doing anything in gta2:
struct SCR_SEND_CHAR {
	SCR_CMD_FORMAT(
		uint16_t charname;
		uint16_t unk2;
		SCR_XYZ_f pos;
	)
};
string read_SEND_CHAR(const FPStruct &params){
	get_data(SCR_SEND_CHAR, data);
	string suffix = (params.header->type == SCRCMD_SEND_CHAR_FOOT) ? "ON_FOOT" : "BY_CAR";
	return sprintf_str("SEND_CHAR_%s (%s, %s)", suffix.c_str(), getname(data.charname).c_str(), scr_coord_3d(data.pos).c_str());
}
LinkToFunc(SCRCMD_SEND_CHAR_FOOT, read_SEND_CHAR);
LinkToFunc(SCRCMD_SEND_CHAR_CAR, read_SEND_CHAR);




// functions with zero params:
ZERO_PARAMS_LAYOUT(LEVELSTART,		"LEVELSTART");
ZERO_PARAMS_LAYOUT(LEVELEND,		"LEVELEND");
ZERO_PARAMS_LAYOUT(MISSIONSTART,	"MISSIONSTART");
ZERO_PARAMS_LAYOUT(MISSIONEND,		"MISSIONEND");
ZERO_PARAMS_LAYOUT(DO_NOWT,			"DO_NOWT");
ZERO_PARAMS_LAYOUT(FUNCTION,		sprintf_str("%s:", setgetfuncname(VARNAME_FUNCTION).c_str()));
ZERO_PARAMS_LAYOUT(RETURN,			"RETURN");
ZERO_PARAMS_LAYOUT(SAVE_GAME,		"SAVE_GAME ()");
ZERO_PARAMS_LAYOUT(FINISH_MISSION,	"MISSION_HAS_FINISHED ()");
ZERO_PARAMS_LAYOUT(CLEAR_BRIEFS,	"CLEAR_ALL_BRIEFS ()");
ZERO_PARAMS_LAYOUT(START_EXEC,		"EXEC"); // unknown commands, not documented!
ZERO_PARAMS_LAYOUT(STOP_EXEC,		"ENDEXEC"); // generates weird next_cmd pointer, jumps off the next command, something to do with WHILE_EXEC perhaps.

// functions with two params (originally SCR_TWO_PARAMS but those variables arent referenced at all here, so
// we hide errors by using SCR_ZERO_PARAMS since they ignore the param reading, so change these to TWO_PARAMS_LAYOUT when you use the other data from it):
ZERO_PARAMS_LAYOUT(BONUS_DECLARE,		sprintf_str("BONUS %s",				setgetname(VARNAME_BONUS).c_str()));
ZERO_PARAMS_LAYOUT(ARROW_DEC,			sprintf_str("ARROW_DATA %s",		setgetname(VARNAME_ARROW).c_str()));
ZERO_PARAMS_LAYOUT(TIMER_DECLARE,		sprintf_str("TIMER_DATA %s",		setgetname(VARNAME_TIMER).c_str()));
ZERO_PARAMS_LAYOUT(ONSCREEN_COUNTER_DEC,sprintf_str("ONSCREEN_COUNTER %s",	setgetname(VARNAME_ONSCREEN_COUNTER).c_str()));

// functions with max 2 params:
TWO_PARAMS_LAYOUT(1_PASSED_FLAG,		sprintf_str("DECLARE_GANG_ONE_MISSIONS_PASSED_FLAG (%s)",	getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(2_PASSED_FLAG,		sprintf_str("DECLARE_GANG_TWO_MISSIONS_PASSED_FLAG (%s)",	getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(3_PASSED_FLAG,		sprintf_str("DECLARE_GANG_THREE_MISSIONS_PASSED_FLAG (%s)", getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DECLARE_MISSION,		sprintf_str("DECLARE_MISSION_FLAG (%s, %s)",				getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DECLARE_POLICE,		sprintf_str("DECLARE_POLICELEVEL (%d)",						data.var2));
TWO_PARAMS_LAYOUT(DEC_GANG_1_FLAG,		sprintf_str("DECLARE_GANG_ONE_MISSION_FLAG (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DEC_GANG_2_FLAG,		sprintf_str("DECLARE_GANG_TWO_MISSION_FLAG (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DEC_GANG_3_FLAG,		sprintf_str("DECLARE_GANG_THREE_MISSION_FLAG (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(FINISH_SCORE,			sprintf_str("DECLARE_FINISH_SCORE (%d)",					data.sint32val));
TWO_PARAMS_LAYOUT(GANG_1_MISSION_TOTAL, sprintf_str("DECLARE_GANG_ONE_MISSIONS_TOTAL (%d)",			data.var2));
TWO_PARAMS_LAYOUT(GANG_2_MISSION_TOTAL, sprintf_str("DECLARE_GANG_TWO_MISSIONS_TOTAL (%d)",			data.var2));
TWO_PARAMS_LAYOUT(GANG_3_MISSION_TOTAL, sprintf_str("DECLARE_GANG_THREE_MISSIONS_TOTAL (%d)",		data.var2));
TWO_PARAMS_LAYOUT(PASSED_FLAG,			sprintf_str("DECLARE_MISSIONS_PASSED_FLAG (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SECRETS_FAILED,		sprintf_str("DECLARE_SECRETS_FAILED_FLAG (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SECRETS_PASSED,		sprintf_str("DECLARE_SECRETS_PASSED_FLAG (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_MIN_ALIVE,		sprintf_str("SET_MIN_MEMBERS_BEFORE_GROUP_SPLITS (%s, %d)", getname(data.var1).c_str(), data.var2));
TWO_PARAMS_LAYOUT(TOTAL_MISSIONS,		sprintf_str("DECLARE_TOTAL_MISSIONS (%d)",					data.var2));
TWO_PARAMS_LAYOUT(TOTAL_SECRETS,		sprintf_str("DECLARE_TOTAL_SECRETS (%d)",					data.var2));
TWO_PARAMS_LAYOUT(ADD_CHAR_TO_GANG,		sprintf_str("ADD_CHAR_TO_GANG (%s, %s)",			getname(data.var1).c_str(), get_scr_string(data.var2).c_str()));
TWO_PARAMS_LAYOUT(ADD_CHAR_TO_GROUP,	sprintf_str("ADD_EXISTING_CHAR_TO_GROUP (%s, %s)",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(ADD_GROUP,			sprintf_str("ADD_GROUP_TO_CHARACTER (%s, %d)",		getname(data.var1).c_str(), data.var2));
TWO_PARAMS_LAYOUT(ADD_LIVES,			sprintf_str("ADD_LIVES (%s, %d)",					getname(data.var1).c_str(), data.var2)); // 0–99 by docs
TWO_PARAMS_LAYOUT(ADD_MULTIPLIER,		sprintf_str("ADD_MULTIPLIER (%s, %d)",				getname(data.var1).c_str(), data.var2)); // 0–99 by docs
TWO_PARAMS_LAYOUT(ADD_ONSCREEN_COUNTER, sprintf_str("ADD_ONSCREEN_COUNTER (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(ALTER_WANTED_LEVEL,	sprintf_str("ALTER_WANTED_LEVEL_NO_DROP (%s, %d)",	getname(data.var1).c_str(), data.var2));
TWO_PARAMS_LAYOUT(ALT_WANTED_LEVEL,		sprintf_str("ALTER_WANTED_LEVEL (%s, %d)",			getname(data.var1).c_str(), data.var2));
TWO_PARAMS_LAYOUT(ARROW_COLOUR,			sprintf_str("SET_ARROW_COLOUR (%s, %s)",			getname(data.var1).c_str(), enum_cstr(COLOURS,data.var2)));
TWO_PARAMS_LAYOUT(CAR_BULLETPROOF,		sprintf_str("SET_CAR_BULLETPROOF (%s, %s)",			getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(CAR_DRIVE_AWAY,		sprintf_str("MAKE_CAR_DRIVE_AWAY (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CAR_FLAMEPROOF,		sprintf_str("SET_CAR_FLAMEPROOF (%s, %s)",			getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(CAR_ROCKETPROOF,		sprintf_str("SET_CAR_ROCKETPROOF (%s, %s)",			getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(CHANGE_CAR_LOCK,		sprintf_str("CHANGE_CAR_LOCK (%s, %s)",				getname(data.var1).c_str(), enum_cstr(LOCKS,data.var2)));
TWO_PARAMS_LAYOUT(CHANGE_CAR_REMAP,		sprintf_str("CHANGE_CAR_REMAP (%s, %d)",			getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT(CHANGE_CHAR_REMAP,	sprintf_str("CHANGE_CHAR_REMAP (%s, %d)",			getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT(CHANGE_INTENSITY,		sprintf_str("CHANGE_INTENSITY (%s, %d)",			getname(data.var1).c_str(), data.var2)); // intensity = integer! (docs said float, but it wont compile)
TWO_PARAMS_LAYOUT(CHANGE_POLICE,		sprintf_str("CHANGE_POLICE_LEVEL (%d)",				data.var2));
TWO_PARAMS_LAYOUT(CHAR_DO_NOTHING,		sprintf_str("MAKE_CHAR_DO_NOTHING (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CHAR_DRIVE_AGGR,		sprintf_str("SET_CHAR_DRIVE_AGGRESSION (%s, %s)",	getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(CHAR_INVINCIBLE,		sprintf_str("SET_CHAR_INVINCIBLE (%s, %s)",			getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(CHAR_TO_BACKDOOR,		sprintf_str("ORDER_CHAR_TO_BACKDOOR (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(CHAR_TO_DRIVE_CAR,	sprintf_str("ORDER_CHAR_TO_DRIVE_CAR (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_CLOCK_ONLY,		sprintf_str("CLEAR_CLOCK_ONLY (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_COUNTER,		sprintf_str("CLEAR_ONSCREEN_COUNTER (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_KF_WEAPON,		sprintf_str("CLEAR_KF_WEAPON (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_NO_COLLIDE,		sprintf_str("CLEAR_CAR_NO_COLLIDE (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_TIMERS,			sprintf_str("CLEAR_TIMER (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLEAR_WANTED_LEVEL,	sprintf_str("CLEAR_WANTED_LEVEL (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(CLOSE_DOOR,			sprintf_str("CLOSE_DOOR (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(COUNTER,				sprintf_str("COUNTER %s = %d",						setgetname(VARNAME_COUNTER).c_str(), data.sint32val));
TWO_PARAMS_LAYOUT(CREATE_THREAD,		sprintf_str("%s = CREATE_THREAD %s:",				getname(data.var2).c_str(), getfuncname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DEATH_ARR_STATE,		sprintf_str("SET_DEATHARREST_STATE (%s, %s)",		getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(DECIDE_POWERUP,		sprintf_str("DECIDE_POWERUP_FOR_CRANE (%s, %s)",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DELAY_HERE,			sprintf_str("DELAY_HERE (%d)",						data.var2));
TWO_PARAMS_LAYOUT(DEL_GROUP_IN_CAR,		sprintf_str("DELETE_GROUP_IN_CAR (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DESTROY_GROUP,		sprintf_str("DESTROY_GROUP (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DISABLE_CRANE,		sprintf_str("DISABLE_CRANE (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DISABLE_THREAD,		sprintf_str("DISABLE_THREAD_TRIGGER (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(DISPLAY_BRIEF,		sprintf_str("DISPLAY_BRIEF (%d)",					data.var1));
TWO_PARAMS_LAYOUT(DISPLAY_BRIEF_NOW,	sprintf_str("DISPLAY_BRIEF_NOW (%d)",				data.var1));
TWO_PARAMS_LAYOUT(DISPLAY_BRIEF_SOON,	sprintf_str("DISPLAY_BRIEF_SOON (%d)",				data.var1));
TWO_PARAMS_LAYOUT(DISPLAY_MESSAGE,		sprintf_str("DISPLAY_MESSAGE (%d)",					data.var1));
TWO_PARAMS_LAYOUT(DISPLAY_TIMER,		sprintf_str("DISPLAY_TIMER (%s, %d)",				getname(data.var1).c_str(), data.var2)); // 0-32767 or it wont display timer.
TWO_PARAMS_LAYOUT(DRIVER_OUT_CAR,		sprintf_str("ORDER_DRIVER_OUT_CAR (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(EMERG_LIGHTS,			sprintf_str("SET_CAR_EMERG_LIGHTS (%s, %s)",		getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(ENABLE_CRANE,			sprintf_str("ENABLE_CRANE (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(ENABLE_THREAD,		sprintf_str("ENABLE_THREAD_TRIGGER (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(EXPLODE_ITEM,			sprintf_str("EXPLODE (%s)",							getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(EXPLODE_LARGE1,		sprintf_str("EXPLODE_LARGE (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(EXPLODE_NO_RING1,		sprintf_str("EXPLODE_NO_RING (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(EXPLODE_SMALL1,		sprintf_str("EXPLODE_SMALL (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(FINISH_LEVEL,			sprintf_str("FINISH_LEVEL (%s)",					enum_cstr(BONUS,data.var2)));
TWO_PARAMS_LAYOUT(FORCE_CLEANUP,		sprintf_str("FORCE_CLEANUP (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(GET_CAR_SPEED,		sprintf_str("STORE_CAR_CURRENT_SPEED (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_CHAR_CAR_SPEED,	sprintf_str("STORE_CHAR_CAR_CURRENT_SPEED (%s, %s)",getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_LAST_PUNCHED,		sprintf_str("STORE_LAST_CHAR_PUNCHED (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_MAX_SPEED,		sprintf_str("STORE_CAR_MAX_SPEED (%s, %s)",			getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_MULT,				sprintf_str("STORE_MULTIPLIER (%s, %s)",			getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_NUM_LIVES,		sprintf_str("STORE_NUM_LIVES (%s, %s)",				getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GET_SCORE,			sprintf_str("STORE_SCORE (%s, %s)",					getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(GIVE_CAR_ALARM,		sprintf_str("GIVE_CAR_ALARM (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(GIVE_DRIVER_BRAKE,	sprintf_str("GIVE_DRIVER_AND_BRAKE (%s)",			getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(GOSUB,				sprintf_str("GOSUB %s:",							getfuncname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(KILL_ALL_PASSENG,		sprintf_str("KILL_ALL_PASSENGERS (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(KILL_CHAR,			sprintf_str("KILL_CHAR (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(LAUNCH_MISSION,		sprintf_str("LAUNCH_MISSION (%s)",					get_scr_string(data.var1).c_str()));
TWO_PARAMS_LAYOUT(MAKE_CAR_DUMMY,		sprintf_str("MAKE_CAR_A_DUMMY (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(MAKE_LEADER,			sprintf_str("MAKE_NEW_LEADER_OF_GROUP (%s, %s)",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(MAKE_MUGGERS,			sprintf_str("MAKE_ALL_CHARS_MUGGERS (%s)",			enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(NO_CHARS_OFF_BUS,		sprintf_str("STOP_CHARS_GETTING_OFF_BUS (%s, %s)",	getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(OPEN_DOOR,			sprintf_str("OPEN_DOOR (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(PARK,					sprintf_str("PARK (%s, %s)",						getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(PARK_NO_RESPAWN,		sprintf_str("PARK_NO_RESPAWN (%s, %s)",				getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(PUT_CAR_ON_TRAILER,	sprintf_str("PUT_CAR_ON_TRAILER (%s, %s)",			getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(REMOTE_CONTROL,		sprintf_str("TAKE_REMOTE_CONTROL_OF_CAR (%s, %s)",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(REMOVE_ARROW,			sprintf_str("REMOVE_ARROW (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(REMOVE_CHAR,			sprintf_str("REMOVE_CHAR_FROM_GROUP (%s, %s)",		getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(REMOVE_WEAPON,		sprintf_str("REMOVE_WEAPONS (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_ALL_CONTROLS,		sprintf_str("SET_ALL_CONTROLS_STATUS (%s, %s)",		getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(SET_BONUS_RATING,		sprintf_str("SET_BONUS_RATING_TEXT_ID (%d)",		data.var2));
TWO_PARAMS_LAYOUT(SET_CAR_JAMMED,		sprintf_str("SET_CAR_JAMMED_ACCELERATOR (%s, %s)",	getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(SET_CHAR_BRAVERY,		sprintf_str("SET_CHAR_BRAVERY_LEVEL (%s, %s)",		getname(data.var1).c_str(), enum_cstr(BRAVERY,data.var2)));
TWO_PARAMS_LAYOUT(SET_CHAR_SHOOT,		sprintf_str("SET_CHAR_SHOOTING_SKILL (%s, %s)",		getname(data.var1).c_str(), enum_cstr(ACCURACY,data.var2)));
TWO_PARAMS_LAYOUT(SET_COUNTER_INT,		sprintf_str("SET %s = %d",							getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT(SET_COUNTER_VAR,		sprintf_str("SET %s = %s",							getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(SET_DOOR_AUTO,		sprintf_str("MAKE_DOOR_AUTOMATIC (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_DOOR_MANUAL,		sprintf_str("MAKE_DOOR_MANUAL (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_ENTER_STATUS,		sprintf_str("SET_ENTER_CONTROL_STATUS (%s, %s)",	getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(SET_MODEL_WANTED,		sprintf_str("SET_RECYCLE_MODEL_WANTED (%s)",		(data.var2 == 0xFFFF) ? "NONE" : enum_cstr(CARS,data.var2)));
TWO_PARAMS_LAYOUT(SET_NO_COLLIDE,		sprintf_str("SET_CAR_NO_COLLIDE (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_PHONE_DEAD,		sprintf_str("SET_PHONE_DEAD (%s)",					getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_SHADING_LEV,		sprintf_str("SET_SHADING_LEVEL (%d)",				data.var2));
TWO_PARAMS_LAYOUT(SET_CHAR_MOM_FAT,		sprintf_str("SET_CHAR_MOM_FAT (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_STAY_IN_CAR,		sprintf_str("SET_CHAR_TO_STAY_IN_CAR (%s, %s)",		getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(SET_USE_CAR_WEAPON,	sprintf_str("SET_CHAR_TO_USE_CAR_WEAPON (%s, %s)",	getname(data.var1).c_str(), enum_cstr(ONOFF,data.var2)));
TWO_PARAMS_LAYOUT(STOP_CAR_DRIVE,		sprintf_str("STOP_CHAR_DRIVING (%s)",				getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(STOP_PHONE_RING,		sprintf_str("STOP_PHONE (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(STOP_THREAD,			sprintf_str("STOP_THREAD (%s)",						getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(STORE_BONUS,			sprintf_str("STORE_BONUS_COUNT (%s, %s)",			getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(STORE_CAR_INFO,		sprintf_str("STORE_CAR_CHARACTER_IS_IN (%s, %s)",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(SUPPRESS_MODEL,		sprintf_str("SUPPRESS_THIS_CAR_MODEL (%s)",			(data.var1 == 0xFFFF) ? "NONE" : enum_cstr(CARS,data.var1)));
TWO_PARAMS_LAYOUT(UPDATE_DOOR,			sprintf_str("UPDATE_DOOR_TARGET (%s, %s)",			getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DELETE_ITEM,			sprintf_str("DELETE_ITEM (%s)",						getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(SET_THREAT_SEARCH,	sprintf_str("SET_CHAR_THREAT_SEARCH (%s, %s)",		getname(data.var1).c_str(), enum_cstr(THREAT_SEARCH,data.var2)));
TWO_PARAMS_LAYOUT(SET_THREAT_REACT,		sprintf_str("SET_CHAR_THREAT_REACTION (%s, %s)",	getname(data.var1).c_str(), enum_cstr(THREAT_REACTION,data.var2)));
TWO_PARAMS_LAYOUT(SET_CHAR_OBJ1,		sprintf_str("SET_CHAR_OBJECTIVE (%s, %s)",			getname(data.var1).c_str(), enum_cstr(OBJECTIVES,data.var2)));
TWO_PARAMS_LAYOUT(SWITCH_GENERATOR1,	sprintf_str("SWITCH_GENERATOR (%s, %s)",			getname(data.var1).c_str(), enum_cstr(ONOFF,(data.var2==0xFFFF))));
TWO_PARAMS_LAYOUT(SWITCH_GENERATOR2,	sprintf_str("SWITCH_GENERATOR (%s, %d)",			getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT(GIVE_WEAPON1,			sprintf_str("GIVE_WEAPON (%s, %s)",					getname(data.var1).c_str(), enum_cstr(WEAPONS,data.var2)));

// functions with max 4 params:
FOUR_PARAMS_LAYOUT(CHANGE_RESPECT,		sprintf_str("CHANGE_GANG_CHAR_RESPECT (%s, %s, %d)",			get_scr_string(data.var1).c_str(), getname(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT(CHANGE_GANG_RESP,	sprintf_str("CHANGE_GANG_CHAR_RESPECT_AND_UPDATE (%s, %s, %d)", get_scr_string(data.var1).c_str(), getname(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT(DEC_DEATH_BASE_1,	sprintf_str("DECLARE_GANG_ONE_DEATH_BASE_BRIEF (%s, %d)",		getname(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(DEC_DEATH_BASE_2,	sprintf_str("DECLARE_GANG_TWO_DEATH_BASE_BRIEF (%s, %d)",		getname(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(DEC_DEATH_BASE_3,	sprintf_str("DECLARE_GANG_THREE_DEATH_BASE_BRIEF (%s, %d)",		getname(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(ADDSCORE_NO_MULT,	sprintf_str("ADD_SCORE_NO_MULT (%s, %d)",			getname(data.var1).c_str(), data.sint32val));
FOUR_PARAMS_LAYOUT(ADD_SCORE1,			sprintf_str("ADD_SCORE (%s, %d)",					getname(data.var1).c_str(), data.sint32val));
FOUR_PARAMS_LAYOUT(ADD_SCORE2,			sprintf_str("ADD_SCORE (%s, %s)",					getname(data.var1).c_str(), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(ADD_TIME,			sprintf_str("ADD_TIME_TO_TIMER (%s, %d)",			getname(data.var1).c_str(), data.sintvar3));  // possibly sint, maybe possible to add negative values.
FOUR_PARAMS_LAYOUT(CHAR_DRIVE_SPEED,	sprintf_str("SET_CHAR_MAX_DRIVESPEED (%s, %s)",		getname(data.var1).c_str(), scr_float(data.floatval)));
FOUR_PARAMS_LAYOUT(LEVEL_END_ARROW1,	sprintf_str("LEVEL_END_POINT_ARROW_AT (%s, %s)",	getname(data.var3).c_str(), getname(data.var1).c_str()));
FOUR_PARAMS_LAYOUT(SET_BAD_CAR,			sprintf_str("SET_BADCAR_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_CARTHIEF,		sprintf_str("SET_CARTHIEF_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_CAR_DENSITY,		sprintf_str("SET_CAR_DENSITY (%s, %d)",				get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_CHAR_OCCUPATION, sprintf_str("SET_CHAR_OCCUPATION (%s, %s)",			getname(data.var1).c_str(), enum_cstr(OCCUPATIONS,data.var3)));
FOUR_PARAMS_LAYOUT(SET_DOOR_INFO,		sprintf_str("DECLARE_DOOR_INFO (%d, %d, %d)",		data.var1, data.var2, data.var3));
FOUR_PARAMS_LAYOUT(SET_ELVIS,			sprintf_str("SET_ELVIS_RATIO (%s, %d)",				get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_FAV_CAR,			sprintf_str("SET_FAVOURITE_MODEL (%s, %s)",			getname(data.var1).c_str(), enum_cstr(CARS,data.var3)));
FOUR_PARAMS_LAYOUT(SET_GANG,			sprintf_str("SET_GANG_RATIO (%s, %d)",				get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_GANGCARRATIO,	sprintf_str("SET_GANGCAR_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_GANG_RESPECT,	sprintf_str("SET_GANG_KILL_REACTION (%s, %s, %d)",	get_scr_string(data.var1).c_str(), get_scr_string(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT(SET_GOOD_CAR,		sprintf_str("SET_GOODCAR_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_GROUP_TYPE,		sprintf_str("SET_GROUP_TYPE (%s, %s)",				getname(data.var1).c_str(), enum_cstr(FOLLOWER_TYPE,data.var3)));
FOUR_PARAMS_LAYOUT(SET_KF_WEAPON,		sprintf_str("SET_KF_WEAPON (%s, %s)",				getname(data.var1).c_str(), enum_cstr(WEAPONS,data.var3)));
FOUR_PARAMS_LAYOUT(SET_MUGGER,			sprintf_str("SET_MUGGER_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_PED_DENSITY,		sprintf_str("SET_PEDDENSITY (%s, %d)",				get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_POLICE_CAR,		sprintf_str("SET_POLICECAR_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_POLICE_PED,		sprintf_str("SET_POLICEPED_RATIO (%s, %d)",			get_scr_string(data.var1).c_str(), data.var3));
FOUR_PARAMS_LAYOUT(SET_RUN_SPEED,		sprintf_str("SET_CHAR_MAX_RUNSPEED (%s, %s)",		getname(data.var1).c_str(), scr_float(data.floatval)));
FOUR_PARAMS_LAYOUT(SET_CHAR_OBJ2,		sprintf_str("SET_CHAR_OBJECTIVE (%s, %s, %s)",		getname(data.var1).c_str(), enum_cstr(OBJECTIVES,data.var2), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(GIVE_WEAPON2,		sprintf_str("GIVE_WEAPON (%s, %s, %d)",				getname(data.var1).c_str(), enum_cstr(WEAPONS,data.var2), data.sintvar3));

// math functions: (note: S_MINUS_I wont compile!)
TWO_PARAMS_LAYOUT(S_PLUS_I,  sprintf_str("%s + %d", getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT(I_PLUS_S,  sprintf_str("%d + %s", data.sintvar1, getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(I_MINUS_S, sprintf_str("%d - %s", data.sintvar1, getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(S_PLUS_S,  sprintf_str("%s + %s", getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(S_MINUS_S, sprintf_str("%s - %s", getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT(DECREMENT, sprintf_str("--%s",	getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT(INCREMENT, sprintf_str("++%s",	getname(data.var1).c_str()));

FOUR_PARAMS_LAYOUT(S_IS_S_MINUS_I,  sprintf_str("SET %s = (%s - %d)",	getname(data.var1).c_str(), getname(data.var2).c_str(), data.sintvar3));
FOUR_PARAMS_LAYOUT(S_IS_S_PLUS_I,	sprintf_str("SET %s = (%s + %d)",	getname(data.var1).c_str(), getname(data.var2).c_str(), data.sintvar3));
FOUR_PARAMS_LAYOUT(S_IS_S_DIV_I,	sprintf_str("SET %s = (%s / %d)",	getname(data.var1).c_str(), getname(data.var2).c_str(), data.sintvar3));
FOUR_PARAMS_LAYOUT(S_IS_S_MULT_I,	sprintf_str("SET %s = (%s * %d)",	getname(data.var1).c_str(), getname(data.var2).c_str(), data.sintvar3));
FOUR_PARAMS_LAYOUT(S_IS_S_MOD_I,	sprintf_str("SET %s = (%s MOD %d)", getname(data.var1).c_str(), getname(data.var2).c_str(), data.sintvar3));
FOUR_PARAMS_LAYOUT(S_IS_S_MINUS_S,	sprintf_str("SET %s = (%s - %s)",	getname(data.var1).c_str(), getname(data.var2).c_str(), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(S_IS_S_PLUS_S,	sprintf_str("SET %s = (%s + %s)",	getname(data.var1).c_str(), getname(data.var2).c_str(), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(S_IS_S_DIV_S,	sprintf_str("SET %s = (%s / %s)",	getname(data.var1).c_str(), getname(data.var2).c_str(), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(S_IS_S_MULT_S,	sprintf_str("SET %s = (%s * %s)",	getname(data.var1).c_str(), getname(data.var2).c_str(), getname(data.var3).c_str()));
FOUR_PARAMS_LAYOUT(S_IS_S_MOD_S,	sprintf_str("SET %s = (%s MOD %s)",	getname(data.var1).c_str(), getname(data.var2).c_str(), getname(data.var3).c_str()));

// boolean functions with zero params:
ZERO_PARAMS_LAYOUT_BOOL(CHECK_DEATH_ARR,sprintf_str("CHECK_DEATHARREST_EXECUTED%s()",	FUNCSPACE.c_str()));
ZERO_PARAMS_LAYOUT_BOOL(PARK_FINISHED,	sprintf_str("HAS_PARK_FINISHED%s()",			FUNCSPACE.c_str()));
ZERO_PARAMS_LAYOUT_BOOL(MODEL_CHECK,	sprintf_str("HAS_MODELCHECK_HAPPENED%s()",		FUNCSPACE.c_str()));
ZERO_PARAMS_LAYOUT_BOOL(BRIEF_ONSCREEN, sprintf_str("IS_BRIEF_ONSCREEN%s()",			FUNCSPACE.c_str()));

// boolean comparison functions with max 2 params:
TWO_PARAMS_LAYOUT_BOOL(S_EQUAL_I,			sprintf_str("%s = %d",	getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(S_GEQUAL_I,			sprintf_str("%s >= %d", getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(S_GREATER_I,			sprintf_str("%s > %d",	getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(S_LESS_I,			sprintf_str("%s < %d",	getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(S_LEQUAL_I,			sprintf_str("%s <= %d", getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(S_EQUAL_S,			sprintf_str("%s = %s",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(S_GEQUAL_S,			sprintf_str("%s >= %s", getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(S_GREATER_S,			sprintf_str("%s > %s",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(S_LESS_S,			sprintf_str("%s < %s",	getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(S_LEQUAL_S,			sprintf_str("%s <= %s",	getname(data.var1).c_str(), getname(data.var2).c_str()));

// boolean functions with max 2 params:
TWO_PARAMS_LAYOUT_BOOL(ANY_WEAPON_HIT_CAR,	sprintf_str("CHECK_ANY_WEAPON_TYPE_HIT_CAR%s(%s)",	FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(BEEN_PUNCHED_BY,		sprintf_str("CHECK_CHAR_BEEN_PUNCHED_BY%s(%s, %s)", FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_MULT,			sprintf_str("CHECK_MULTIPLIER_GREATER%s(%s, %d)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2)); // not supported by original compiler! but seems to be supported by gta2.
TWO_PARAMS_LAYOUT_BOOL(CARBOMB_ACTIVE,		sprintf_str("IS_CARBOMB_ACTIVE%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CAR_DAMAGE_POS,		sprintf_str("CHECK_CAR_DAMAGE_POSITION%s(%s, %s)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(CARDAMAGEPOS,data.var2)));
TWO_PARAMS_LAYOUT_BOOL(CAR_GOT_DRIVER,		sprintf_str("HAS_CAR_GOT_DRIVER%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CAR_IN_AIR,			sprintf_str("IS_CAR_IN_AIR%s(%s)",					FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CAR_SUNK,			sprintf_str("HAS_CAR_JUST_SUNK%s(%s)", 				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHAR_ARRESTED,		sprintf_str("HAS_CHAR_BEEN_ARRESTED%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHAR_IN_AIR,			sprintf_str("IS_CHAR_FALLING%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHAR_SUNK,			sprintf_str("HAS_CHAR_JUST_SUNK%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_BONUS1,		sprintf_str("HAS_BONUS_PASSED%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_BONUS2,		sprintf_str("HAS_BONUS_FAILED%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_BONUS3,		sprintf_str("HAS_BONUS_FINISHED%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_CAR_DAMAGE,	sprintf_str("CHECK_CAR_DAMAGE_LEVEL%s(%s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_CAR_DRIVER,	sprintf_str("CHECK_CAR_HAS_DRIVER%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_CAR_MODEL,		sprintf_str("CHECK_CAR_MODEL%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(CARS,data.var2)));
TWO_PARAMS_LAYOUT_BOOL(CHECK_CAR_REMAP,		sprintf_str("CHECK_CAR_REMAP%s(%s, %d)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_HEADS,			sprintf_str("CHECK_HEADS_GREATER%s(%s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_HEALTH,		sprintf_str("CHECK_CHARACTER_HEALTH%s(%s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_MAX_PASS,		sprintf_str("IS_CHAR_CAR_CAPACITY%s(%s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_NUM_ALIVE,		sprintf_str("CHECK_NUMBER_ALIVE_IN_GROUP%s(%s, %d)",FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_NUM_LIVES,		sprintf_str("CHECK_NUM_LIVES_GREATER%s(%s, %d)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(CHECK_PHONE,			sprintf_str("CHECK_ANSWERED_PHONE%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(CHECK_PHONETIMER,	sprintf_str("CHECK_FAIL_PHONE_TIMER%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(DELAY,				sprintf_str("DELAY%s(%d)",							FUNCSPACE.c_str(), data.var2));
TWO_PARAMS_LAYOUT_BOOL(EMERG_LIGHTS_ON,		sprintf_str("ARE_EMERG_LIGHTS_ON%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(GET_CAR_FROM_CRANE,	sprintf_str("GET_CAR_INFO_FROM_CRANE%s(%s, %s)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(GET_PASSENGER_NUM,	sprintf_str("CHECK_PASSENGER_COUNT%s(%s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sintvar2));
TWO_PARAMS_LAYOUT_BOOL(HAS_CAR_WEAPON,		sprintf_str("HAS_CAR_GOT_WEAPON%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(WEAPONS,data.var2)));
TWO_PARAMS_LAYOUT_BOOL(HAS_CHAR_DIED,		sprintf_str("HAS_CHARACTER_DIED%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_BUS_FULL,			sprintf_str("IS_BUS_FULL%s(%s)",					FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CAR_CRUSHED,		sprintf_str("IS_CAR_CRUSHED%s(%s)",					FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CAR_ON_TRAIL,		sprintf_str("IS_CAR_ON_TRAILER%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CAR_WRECKED,		sprintf_str("IS_CAR_WRECKED%s(%s)",					FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_FIRE_ONSCREEN,sprintf_str("IS_CHAR_FIRING_ONSCREEN%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_HORN,		sprintf_str("IS_CHAR_PRESSING_HORN%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_MOM_FAT,		sprintf_str("IS_CHARACTER_MOTHER_FAT%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_IN_ANY_CAR,	sprintf_str("IS_CHARACTER_IN_ANY_CAR%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_IN_CAR,		sprintf_str("IS_CHARACTER_IN_CAR%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_IN_GANG,		sprintf_str("IS_CHAR_IN_GANGZONE%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), get_scr_string(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_IN_MODEL,	sprintf_str("IS_CHARACTER_IN_MODEL%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(CARS,data.var2)));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_IN_ZONE,		sprintf_str("IS_CHAR_IN_ZONE%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), get_scr_string(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_OBJ_FAIL,	sprintf_str("IS_CHAR_OBJECTIVE_FAILED%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_OBJ_PASS,	sprintf_str("IS_CHAR_OBJECTIVE_PASSED%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_ON_FIRE,		sprintf_str("IS_CHAR_ON_FIRE%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_STOPPED,		sprintf_str("IS_CHARACTER_STOPPED%s(%s)",			FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_CHAR_STUNNED,		sprintf_str("IS_CHAR_STUNNED%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_GROUP_IN_CAR,		sprintf_str("IS_GROUP_IN_CAR%s(%s)",				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_ITEM_ONSCREEN,	sprintf_str("IS_ITEM_ONSCREEN%s(%s)", 				FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(IS_TRAILER_ATT,		sprintf_str("IS_TRAILER_ATTACHED%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str()));
TWO_PARAMS_LAYOUT_BOOL(ONSCREEN_ACCURACY,	sprintf_str("IS_ITEM_ACCURATELY_ONSCREEN%s(%s)",	FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(PUNCHED_SOMEONE,		sprintf_str("HAS_CHAR_PUNCHED_SOMEONE%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));
TWO_PARAMS_LAYOUT_BOOL(SETUP_MODEL_CHECK,	sprintf_str("SETUP_MODELCHECK_DESTROY%s(%s)",		FUNCSPACE.c_str(), enum_cstr(CARS,data.var1)));
TWO_PARAMS_LAYOUT_BOOL(SPOTTED_PLAYER,		sprintf_str("HAS_CHAR_SPOTTED_PLAYER%s(%s)",		FUNCSPACE.c_str(), getname(data.var1).c_str()));

// boolean functions with max 4 params:
FOUR_PARAMS_LAYOUT_BOOL(CHECK_CAR_BOTH,			sprintf_str("CHECK_CAR_MODEL_AND_REMAP%s(%s, %s, %d)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(CARS,data.var2), data.sintvar3));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_CAR_SPEED,		sprintf_str("CHECK_CAR_SPEED%s(%s, %d)",				FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sint32val)); // -1 possible!
FOUR_PARAMS_LAYOUT_BOOL(CHECK_CURRENT_WEAPON,	sprintf_str("CHECK_CHAR_CURR_WEAPON%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(WEAPONS,data.var3)));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_OBJ_MODEL,		sprintf_str("CHECK_OBJ_MODEL%s(%s, %s)",				FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(OBJECTS,data.var3)));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_RESPECT_GREATER,	sprintf_str("CHECK_RESPECT_GREATER%s(%s, %s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), get_scr_string(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_RESPECT_IS,		sprintf_str("CHECK_RESPECT_EQUAL%s(%s, %s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), get_scr_string(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_RESPECT_LESS,		sprintf_str("CHECK_RESPECT_LOWER%s(%s, %s, %d)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), get_scr_string(data.var3).c_str(), data.sintvar2));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_SCORE,			sprintf_str("CHECK_SCORE_GREATER%s(%s, %d)",			FUNCSPACE.c_str(), getname(data.var1).c_str(), data.sint32val));
FOUR_PARAMS_LAYOUT_BOOL(CHECK_WEAPONHIT,		sprintf_str("CHECK_WEAPON_TYPE_HIT_CHAR%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(KILLED_BY,data.var3)));
FOUR_PARAMS_LAYOUT_BOOL(WEAP_HIT_CAR,			sprintf_str("CHECK_WEAPON_TYPE_HIT_CAR%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.var1).c_str(), enum_cstr(KILLED_BY,data.var3)));

// other boolean functions:
CUSTOM_STRUCT_LAYOUT_BOOL(POINT_ONSCREEN,	SCR_SIX_PARAMS, sprintf_str("IS_POINT_ONSCREEN%s(%s)",							FUNCSPACE.c_str(), scr_coord_3d(data.pos).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOC_SEC_CHAR_ANY, SCR_SIX_PARAMS, sprintf_str("LOCATE_ANOTHER_CHARACTER_ANY_MEANS%s(%s, %s, %s)", FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str(), scr_coord(data.size).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOC_SEC_CHAR_CAR, SCR_SIX_PARAMS, sprintf_str("LOCATE_ANOTHER_CHARACTER_BY_CAR%s(%s, %s, %s)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str(), scr_coord(data.size).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOC_SECOND_CHAR,	SCR_SIX_PARAMS, sprintf_str("LOCATE_ANOTHER_CHARACTER_ON_FOOT%s(%s, %s, %s)",	FUNCSPACE.c_str(), getname(data.var1).c_str(), getname(data.var2).c_str(), scr_coord(data.size).c_str()));

struct SCR_ONEVAR_RECT {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk2;
		SCR_Rect_f rect;
	)
};
CUSTOM_STRUCT_LAYOUT_BOOL(CAR_WRECK_IN_LOCATION,SCR_ONEVAR_RECT, sprintf_str("CHECK_CAR_WRECKED_IN_AREA%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(IS_CHAR_FIRING_AREA,	SCR_ONEVAR_RECT, sprintf_str("IS_CHAR_FIRING_IN_AREA%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOCATE_CHAR_ANY,		SCR_ONEVAR_RECT, sprintf_str("LOCATE_CHARACTER_ANY_MEANS%s(%s, %s)",		FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOCATE_CHAR_BY_CAR,	SCR_ONEVAR_RECT, sprintf_str("LOCATE_CHARACTER_BY_CAR%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(LOCATE_CHAR_ONFOOT,	SCR_ONEVAR_RECT, sprintf_str("LOCATE_CHARACTER_ON_FOOT%s(%s, %s)",			FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(STOP_LOCATE_CHAR_ANY, SCR_ONEVAR_RECT, sprintf_str("LOCATE_STOPPED_CHARACTER_ANY_MEANS%s(%s, %s)",FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(STOP_LOCATE_CHAR_FOOT,SCR_ONEVAR_RECT, sprintf_str("LOCATE_STOPPED_CHARACTER_ON_FOOT%s(%s, %s)",	FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(STOP_LOCATE_CHAR_CAR, SCR_ONEVAR_RECT, sprintf_str("LOCATE_STOPPED_CHARACTER_BY_CAR%s(%s, %s)",	FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(GROUP_IN_AREA,		SCR_ONEVAR_RECT, sprintf_str("IS_GROUP_IN_AREA%s(%s, %s)",					FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str())); // undocumented, not tested.

struct SCR_IS_CAR_IN_BLOCK {
	SCR_CMD_FORMAT(
		uint16_t varname;
		uint16_t unk2;
		SCR_XYZ_f pos;
	)
};
CUSTOM_STRUCT_LAYOUT_BOOL(IS_CAR_IN_BLOCK, SCR_IS_CAR_IN_BLOCK, sprintf_str("IS_CAR_IN_BLOCK%s(%s, %s)", FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_coord_3d(data.pos).c_str()));
CUSTOM_STRUCT_LAYOUT_BOOL(CAR_IN_AREA,	   SCR_ONEVAR_RECT,		sprintf_str("IS_CAR_IN_BLOCK%s(%s, %s)", FUNCSPACE.c_str(), getname(data.varname).c_str(), scr_rect(data.rect).c_str()));




struct SCR_IF_JUMP {
	SCR_CMD_FORMAT(
		//uint16_t and_logical_operator;       // First dword: 0 or 1. It's 1 for AND command.
		uint16_t or_logical_operator;        // Second dword: 0 or 1. It's 1 for OR command.
		uint16_t else_jump_or_endif_index;   // If the 1st and 2nd dwords are 0 it will be "else_jump_index", where will it jump if check was false. Otherwise it will be the ENDIF index
	)
};
string read_IF_JUMP(const FPStruct &params){
	get_data(SCR_IF_JUMP, data);
	if( params.header->return_value == 0 && data.or_logical_operator == 0 ){  // data.and_logical_operator == 0 && data.or_logical_operator == 0
	    //  It's the end of a IF or a WHILE
		//return sprintf_str("ENDIF %s", hexdump(params,0).c_str());
		if ( 1 ) {
			return "ENDIF_ZZZZZ";
		} else {
			return "ENDWHILE";
		}
		
	}else{
		return sprintf_str("%s", hexdump(params,1).c_str());
	}
}
LinkToFunc(SCRCMD_IF_JUMP, read_IF_JUMP);

//ZERO_PARAMS_LAYOUT_BOOL(NOT, "NOT"); // this will be wrapped around the previous comparison command or function with braces! (is not included atm because it causes parse errors in the compiler).
