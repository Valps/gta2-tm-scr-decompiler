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
// Thanks to:
// - B-$hep for the project idea and being helpful.
// - Vike for some of the string arrays.
// - T.B. for noticing data offset bug.
// - Valps for IF/WHILE/WHILE_EXEC/ELSE/NOT/AND/OR decompiling.
//
// Note: If you edit the code, credit yourself above. If you edited the code a lot,
// and you feel you deserve to be credited in the build_credits() function, then
// contact T.M. (tmdevmail at gmail.com).
//
// Note: Do not make unnecessary changes such as code formatting or minor optimizations.
//
// Forum thread: http://gtamp.com/forum/viewtopic.php?f=4&t=447
//
///////////////////////////////////////

//
// Note: To be able to use this as a DLL, you must set both projects to use the
// same setting: "Multi-threaded DLL (/MD)" or it will crash!
//
// If you add a new command, go to "scr_functions.h" and
// add it the same way as the others. then go to "scr_commands.h"
// and find the correct name for the SCR function type integer.
// then use LinkToFunc() below your function to link that name
// to the correct function. Note: you can use same function
// for multiple different SCR function type names.
//
// Make sure you use SCR_CMD_FORMAT macro for all structs for the SCR commands!
// This makes it easier to modify those structs later if needed.
// Also make sure all SCR related structs are in the "scr_structs.h" file! That will
// ensure the structs packing is correct.
//
///////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(disable:4996) // "consider using fopen_safe/sprintf_safe" warnings etc.
#pragma warning(disable:4706) // "assignment within conditional expression" if(fp = fopen()...
#pragma warning(disable:4100) // "unreferenced formal parameter" parameter in function that is not used in the function.
#pragma warning(disable:4127) // "conditional expression is constant" using constant in while(1) etc.
#pragma warning(disable:4201) // "nonstandard extension used : nameless struct/union"
#endif // _MSC_VER

#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <map>

#define SCR_DECOMPILER_EXPORTS
#include "scr_decompiler_api.h"

// these are the strings that will be used when generating variable names,
// each name will have number starting from 1 after it:
SCR_DECOMPILER_API string VARNAME_PLAYER		= "p";
SCR_DECOMPILER_API string VARNAME_SOUND			= "sound";
SCR_DECOMPILER_API string VARNAME_CONVEYOR		= "conv";
SCR_DECOMPILER_API string VARNAME_OBJECT		= "obj";
SCR_DECOMPILER_API string VARNAME_SHOP			= "shop"; // if obj_data has CAR_SHOP set then using this name!
SCR_DECOMPILER_API string VARNAME_GENERATOR		= "gen";
SCR_DECOMPILER_API string VARNAME_CAR			= "auto"; // not using "car" since there are cars named as "car15" etc. (avoids confusion).
SCR_DECOMPILER_API string VARNAME_CHAR			= "chr";
SCR_DECOMPILER_API string VARNAME_DOOR			= "door";
SCR_DECOMPILER_API string VARNAME_RADIO			= "radio";
SCR_DECOMPILER_API string VARNAME_CRANE			= "crane";
SCR_DECOMPILER_API string VARNAME_CRUSHER		= "crusher";
SCR_DECOMPILER_API string VARNAME_DESTRUCTOR	= "des";
SCR_DECOMPILER_API string VARNAME_BONUS			= "bonus";
SCR_DECOMPILER_API string VARNAME_ARROW			= "arrow";
SCR_DECOMPILER_API string VARNAME_TIMER			= "timer";
SCR_DECOMPILER_API string VARNAME_ONSCREEN_COUNTER = "onscreen";
SCR_DECOMPILER_API string VARNAME_LIGHT			= "light";
SCR_DECOMPILER_API string VARNAME_SAVED_COUNTER	= "savecount";
SCR_DECOMPILER_API string VARNAME_THREAD_TRIGGER= "trigger";
SCR_DECOMPILER_API string VARNAME_THREAD_ID		= "thread";
SCR_DECOMPILER_API string VARNAME_COUNTER		= "count";
SCR_DECOMPILER_API string VARNAME_FUNCTION		= "function"; // user defined subroutines.
// other options to use:
SCR_DECOMPILER_API string DEC_STR = ""; // added after the varname (but before the number) if its a declaration. (supposed to be "dec")
SCR_DECOMPILER_API string FUNCSPACE = ""; // for boolean functions string between function name and first brace. (optionally set to " " (one space))
SCR_DECOMPILER_API string SCR_FLOAT_PATTERN = "%.2f"; // change this if you need more precision; don't use less than 2 precision, don't use more than 4 precision (rounding errors might appear after multiple compiling/decompiling).

#define SCR_INCLUDE_DEBUG 5000 // if not defined, code will be faster but unfinished commands (currently only IF/WHILE etc. structures) cannot be seen.

SCR_DECOMPILER_API bool SCR_use_debug = 0; // switch error file writing on/off at runtime (can be used if SCR_INCLUDE_DEBUG defined).
SCR_DECOMPILER_API bool SCR_debug_all_lines = 0; // if 0: using only command names for unknown commands output, if 1: show all commands raw hex data.

#ifndef LINESEP
 #ifdef _WIN32
  #define LINESEP "\n"
 #else
  #define LINESEP "\n"
 #endif
#endif

#define arrlen(arr) (sizeof(arr)/sizeof(*arr)) // get c-array lengths.
#define array_key_exists(find_key, arr) (arr.find(find_key) != arr.end()) // check if std::map array key exists.
#define free_safe(point) if(point != NULL){ free(point); point = NULL; }else (void)0 // else without ending ; to prevent bugs with oneliner if-else's or missing ; marks.

#include "stringfuncs.h"

#include "scr_strings.h"
#include "scr_commands.h"


#pragma pack(push)
#pragma pack(1)
#include "scr_structs.h"
#pragma pack(pop)

#include "scr_helperfuncs.h"

#pragma pack(push)
#pragma pack(1)
#include "scr_functions.h"
#pragma pack(pop)


// notes:
// - max base pointers: 6000 (note: this can be exceeded without a warning; exceeded pointers will be lost).
// - max mission pointers: 1536 (note: this can be exceeded without a warning; exceeded pointers will be lost).
// - max base script size: 45544 (compiler refuses to compile after this has been exceeded, its still possible to make larger scripts by hex editor!)
// - max mission script size: 56260 (using smallest possible base script, note: its not possible to make this large scripts due to low max pointers count!)
// - compiler compiles LAUNCH_MISSION inside a mission, but doesn't store the
//   script filename anywhere, it even adds the LAUNCH_MISSION command in the SCR data! thus, it might be possible to use launch_mission inside a mission.
// - the last command cmd_next value must always be set to FFFF ? tested without it and runs fine in gta2. maybe it speeds it up by skipping unused data?

// - output if-structure format should be like:
//   IF ((((counter1 = 1) // first braces (minus one) will tell how many AND/OR sub commands comes after the IF (4-1=3).
//   AND (counter2 = 1) ) // same here: but zero sub commands (1-1=0). will have extra ) at end with spacing (ends the started "(((" things).
//   AND ((counter3 = 1) OR (counter5 = 1)) ) // same, but 1 extra sub command (2-1=1).
//   AND (counter4 = 1) )
//	     // code here.
//   ENDIF


string build_credits(const string &filename){
	return "// Decompiled from \"" + BaseName(filename,0) + ".scr\" Using Epic GTA2 Script Decompiler by T.M." LINESEP LINESEP;
}

// FORWARD commands doesn't leave any trace of data in there (except the null pointer, which cannot be used since it has multiple meanings).
// so we need to go through only the FUNCTION definition commands, then generate a list of FORWARD commands as the first lines of the script.
// this has to be done because we must know the function names beforehand and you might call a function that is not defined yet since the FORWARD commands are gone.
void collect_functions(FPStruct &params, const int &start_point, const int &end_point, string &output, bool is_mission){
	for(int u = start_point; u < end_point; u++){
		params.point = params.pointers[u];
		params.pointer_index = u;
		params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.point);
		if(params.header->type == SCRCMD_FUNCTION && is_mission == 0){		//  The FORWARD command is forbidden in a mission script
			output += "FORWARD " + read_FUNCTION(params) + LINESEP;
		}
	}
}

uint16_t check_if_or_while( FPStruct &params, uint16_t u ) {            //FPStruct &params, uint16_t u ){
	
	SCR_CMD_HEADER *header2 = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u+3]);
	
	if ( header2->type == SCRCMD_GOTO ) {
		return SCRCMD_WHILE;
		//output += tabs + "ENDWHILE" + LINESEP;
	} else {
		return SCRCMD_IF;
		//output += tabs + "ENDIF" + LINESEP;
	}
}

bool is_it_an_endif_endwhile ( FPStruct &params , uint16_t return_value, uint16_t or_operator , bool is_exec, int u_init ) {
	if ( is_exec == 0 ) {
		//  There is no WHILE_EXEC, so there is no problem
		if ( return_value == 0 && or_operator == 0 ) {
			return true;
		} else {
			return false;
		}
	} else {
		//  There is an WHILE_EXEC
		//  Automatically return_value == 1
		//  So we need to verify if "else_jump_or_endif_index" matches with another IF_JUMP
		//  If yes, it isn't a ENDIF/ENDWHILE
		//  If not, it is a ENDIF/ENDWHILE
		
		//  Suppose that params is already pointing at the IF_JUMP we are analysing
		get_data(SCR_IF_JUMP, data5);
		
		uint16_t exit_point = data5.else_jump_or_endif_index;
		int original_point = params.pointer_index;				//  used to get back at the end
		
		for (int u6 = u_init; params.header->cmd_this != exit_point ; u6++) {
			params.point = params.pointers[u6];
			params.pointer_index = u6;
			params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u6]);
		}
		
		//  Verifying
		if ( params.header->type == SCRCMD_IF_JUMP ) {
			
			//  Return to its original pointer
			params.point = params.pointers[original_point];
			params.pointer_index = original_point;
			params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[original_point]);
			
			return false;
		} else {
			
			//  Return to its original pointer
			params.point = params.pointers[original_point];
			params.pointer_index = original_point;
			params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[original_point]);
			
			return true;
		}
		
	}
}

// decompiles the script by using range of pointers.
// note: end_point should be a count value, such as max_pointers etc.
void process_pointers(FPStruct &params, const int &start_point, const int &end_point, string &output){
	string cmdstr;
	FuncMapType::const_iterator it;
	string tabs;
	bool building_if_while = 0;			//  this = 1 means that that IF/WHILE contains at least an AND or OR, and will create lines for each boolean check
	bool inside_if_while = 0;			//  whenever this = 1, it will prevent the RETURN command from removing function indents
	bool not_operator = 0;				//  this = 1 means that there is a NOT
	bool skip = 0;					    //  workaround to use "continue" outside a DO...WHILE;
	bool is_not = 0;					//  this = 1 means that the next boolean check is undergoing a NOT
	bool is_endif = 0;					//  WHILE_EXEC
	bool is_exec = 0;					//  this = 1 means that there is an EXEC acquired from a WHILE_EXEC
	
	int else_count = 0;				    //  counts the number of ELSEs in a IF/WHILE
	int line = 1;						//  The line of an AND or OR below an IF/WHILE
	
	uint16_t while_or_if = 0;			//  it is used when there are AND and ORs in a IF or WHILE
	
	uint16_t else_point[20];			//  pointers of ELSEs    (20 ELSEs per IF/WHILE block is enough)
	uint16_t next_point;
	uint16_t not_next_point;
	
	uint16_t access_command_index = 0;
	
	string not_init = "";
	string not_end = "";
	
	string str_exec = "";		   //  this = "" if there is a WHILE;    this = "_EXEC" if there is a WHILE_EXEC
	
	string and_or_order = "";      //   0 = and, 1 = or, e.g.  "0010" =  ( ( ( (...) AND (...) ) AND (...) ) OR (...) ) AND (...)   [it does not include NOTs]
	string str_bracket = "";       //   this = "(" or "( (" or "( ( (" etc.
	
	for(int u = start_point; u < end_point; u++){
		params.point = params.pointers[u];
		params.pointer_index = u;
		params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.point);

		#ifdef SCR_INCLUDE_DEBUG			//   For now all IF/WHILE decompiler code is in debug. When it is finished, I will restore debug outputs
		if(params.point == 0){
			if(SCR_debug_all_lines){
				output += tabs + sprintf_str("// (0) = (%05d)" LINESEP, u);
			}
		}else{
			if((it = Functions.find(params.header->type)) != Functions.end()){
				
				if ( params.header->type == SCRCMD_NOT ) {
					continue;
				}
				
				const string &retval = it->second(params);
				
				int conditions = 1;			   //  number of boolean checkings
				
				//  Checking if it is a boolean function or a NOT
				if ( is_bool_function(params.header->type) ) {
					
					inside_if_while = 1;
					
					//  Check if it is being executed by an WHILE_EXEC       TODO: NOT CONFUSE WITH "AND"
					if ( params.header->return_value == 1 ) {
						is_exec = 1;
						str_exec = "_EXEC";
					} else {
						is_exec = 0;
						str_exec = "";
					}
					
					//  Check if there is a NOT right after it-
					not_next_point = params.header->cmd_next;
					
					
					int u0 = u;
					for (u0 = u; params.header->cmd_this != not_next_point ; u0++) {
						params.point = params.pointers[u0];
						params.pointer_index = u0;
						params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u0]);
					}
					
					if ( params.header->type == SCRCMD_NOT ) {
						is_not = 1;
						not_operator = 1;
						not_init = "NOT ( ";
						not_end = " )";
					} else {
						is_not = 0;
						not_operator = 0;
						not_init = "";
						not_end = "";
					}
					
					//  return to the previous pointer
					
					params.point = params.pointers[u];
					params.pointer_index = u;
					params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.point);
					
					
					//  It's the first check of a IF/WHILE?
					if ( building_if_while == 0 ) {		//  building_if_while == 0
						building_if_while = 1;
						
						next_point = params.header->cmd_next;
						
						//  forward to the next pointer
						int u2;
						for (u2 = u; params.header->cmd_this != next_point ; u2++) {
							params.point = params.pointers[u2];
							params.pointer_index = u2;
							params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u2]);
						}
						--u2;    //  correction
						
						//SCR_CMD_HEADER *header3 = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.point);
						
						//  If there is no NOT operator and if the script was coded correctly, the next index will always be an IF_JUMP
						
						do {
						if ( params.header->type == SCRCMD_IF_JUMP ) {
							
							get_data(SCR_IF_JUMP, data);
							
							
							
							//  Checking if this IF/WHILE has only one check (i.e. there is no AND or OR) so it will jump at the end
							if( is_it_an_endif_endwhile( params, params.header->return_value, data.or_logical_operator, is_exec, u ) ){	 //   params.header->return_value == 0 && data.or_logical_operator == 0
								
								if ( check_if_or_while(params, u2) == SCRCMD_WHILE ) {
									output += tabs + "WHILE" + str_exec + " ( " + not_init + retval + not_end + " )" + LINESEP;
								} else {
									output += tabs + "IF ( " + not_init + retval + not_end + " )" + LINESEP;    //   + sprintf_str("%i", u2)
								}
								
								tabs += "\t\t\t";			//  TABS
								building_if_while = 0;		//  Finish  IF/WHILE line
								
								//  Checking if there is an ELSE
								if ( data.else_jump_or_endif_index < params.header->cmd_this ) {
									//  There is an ELSE
									else_point[else_count] = data.else_jump_or_endif_index;
									++else_count;
								}
								
								//  Cleaning the NOT string
								if ( not_operator == 1 || is_not == 1 ) {
									not_operator = 0;
									is_not = 0;
									not_init = "";
									not_end = "";
									
									//  back to the original point
									params.point = params.pointers[u2];
									params.pointer_index = u2;
									params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u2]);
								}
								
								skip = 1;
								//continue;
							} else {
								//  This is a link
								conditions = 2;     
								
								// reset brackets
								str_bracket = "( ";
								
								//  Count the number of brackets and the order of ORs and ANDs
								for (int brackets = 1; brackets < conditions; brackets++) {
									
									get_data(SCR_IF_JUMP, data2);
									//  Checking if it is a AND or OR
									//  is_it_an_endif_endwhile( params, params.header->return_value, data2.or_logical_operator, is_exec, u )
									if ( data2.or_logical_operator == 0 ) {	
										// it is an AND
										and_or_order += "0";
									} else {
										// it is an OR
										and_or_order += "1";
									}
									
									
									//  forward to the next IF_JUMP pointer
									
									int u5 = u;
									do {
										next_point = params.header->cmd_next;
										for (u5 = u; params.header->cmd_this != next_point ; u5++) {
											params.point = params.pointers[u5];
											params.pointer_index = u5;
											params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u5]);
										}
										--u5;    //  correction
									} while ( params.header->type != SCRCMD_IF_JUMP );
									
									
									//  Checking if it is an ENDIF/ENDWHILE or another link
									get_data(SCR_IF_JUMP, data);
									if ( is_it_an_endif_endwhile( params, params.header->return_value, data.or_logical_operator, is_exec, u ) ) {					///   TODO
										// it is an ENDIF/ENDWHILE
										//  Checking if it s a IF or WHILE
										if ( check_if_or_while(params, u5) == SCRCMD_WHILE ) {
											while_or_if = SCRCMD_WHILE;
										} else {
											while_or_if = SCRCMD_IF;
										}
										// store the index of the first command in IF/WHILE block
										access_command_index = params.header->cmd_next;
										
										// check for ELSEs
										//  Checking if there is an ELSE
										if ( data.else_jump_or_endif_index < params.header->cmd_this ) {
											//  There is an ELSE
											else_point[else_count] = data.else_jump_or_endif_index;
											++else_count;
										}
										break;
										
									} else {
										//  it is another link
										++conditions;
										str_bracket += "( ";
									}
									
									
								}
								
								//  The brackets counting has ended
								//  Now go constructing that IF/WHILE
								
								// There is a NOT?
								if ( is_not == 1 ) {
									not_init = "NOT ( ";
									not_end = " )";
									is_not = 0;
								} else {
									not_init = "";
									not_end = "";
								}
								
								// Constructing the first line
								if ( while_or_if == SCRCMD_IF ) {
									output += tabs + "IF ( " + str_bracket + not_init + retval + not_end + " )" + LINESEP;
								} else {
									output += tabs + "WHILE" + str_exec + " ( " + str_bracket + not_init + retval + not_end + " )" + LINESEP;
								}
								
								tabs += "\t\t\t";			//  TABS
								
								//  Go constructing the other lines in comment """"Resume the construction of the IF/WHILE""""
								building_if_while = 1;		//  Just to ensure
								skip = 1;
								break;
							}
							
							
							
						} else {
							//  It can be a NOT or another checking boolean (format error)
							if ( params.header->type == SCRCMD_NOT ) {
								not_operator = 1;
								//retval = "NOT ( " + retval + " )";
								
								not_init = "NOT ( ";
								not_end = " )";
								
								//  go to IF_JUMP
								
								next_point = params.header->cmd_next;
								int u4;
								for (u4 = u; params.header->cmd_this != next_point ; u4++) {
									params.point = params.pointers[u4];
									params.pointer_index = u4;
									params.header = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.pointers[u4]);
								}
								--u4;    //  correction
								u2 = u4;
								
							} else {
								//  The script wasn't coded correctly (i.e. the order of ORs and ANDs brackets isn't correct)
								output += string("// CMD: Formating error: ") + retval + LINESEP;
								building_if_while = 0;
							}
							
							
						}
						
						} while ( not_operator == 1 );
						
						//  I can't put a 'continue' before because of DO...WHILE, so skip there
						if ( skip == 1 ) {
							skip = 0;
							continue;
						}
					} else {
						//  building_if_while == 1
						//  Resume the construction of the IF/WHILE
						conditions = and_or_order.length() + 1;
						
						if ( is_not == 1 ) {
							not_init = "NOT ( ";
							not_end = " )";
							is_not = 0;
						} else {
							not_init = "";
							not_end = "";
						}
						
						if (and_or_order[line-1] == '0') {
							//  it is an AND
							output += tabs + "AND ( " + not_init + retval + not_end + " ) )" + LINESEP;
						} else {
							//  it is an OR
							output += tabs + "OR ( " + not_init + retval + not_end + " ) )" + LINESEP;
						}
						
						++line;
						
						//  If all conditions were printed...   finish IF/WHILE
						if ( line == conditions ) {
							//  IF/WHILE finished
							output += LINESEP;
							//  Go clean variables
							building_if_while = 0;
							and_or_order = "";
							str_bracket = "";
							inside_if_while = 1;
							not_operator = 0;
							while_or_if = 0;
							conditions = 1;
							line = 1;
						}
						/*
						for (int line = 1; line < conditions; line++) {
							//  checkinf if it is a AND or OR
							if (and_or_order[line-1] == '0') {
								//  it is an AND
								output += tabs + "AND ( " + retval + " ) )" + LINESEP;		//  TODO:  include NOT
							} else {
								//  it is an OR
								output += tabs + "OR ( " + retval + " ) )" + LINESEP;		//  TODO:  include NOT
							}
						}
						*/
						
						
						//  Now jump to the next command
						continue;
					}
						
				}
				
				
				
				//  Checking if it is a IF_JUMP
				if ( params.header->type == SCRCMD_IF_JUMP ) {
					
					//  Detecting if this IF_JUMP is a AND / OR link, or if it is an ENDIF/ENDWHILE
					get_data(SCR_IF_JUMP, data);
					
					if( is_it_an_endif_endwhile( params, params.header->return_value, data.or_logical_operator, is_exec, u ) ){					///   TODO
						//  It's the end of a IF or a WHILE
						//  Now we need to check if there is a GOTO in current_pointer + 3
						
						tabs = tabs.substr(0, tabs.length()-3);   //  Remove TABS
						
						if ( check_if_or_while(params, u) == SCRCMD_WHILE ) {
							output += tabs + "ENDWHILE" + LINESEP;                   //   + sprintf_str("%i", u)
						} else {
							output += tabs + "ENDIF" + LINESEP;
						}
						
						
						
						/*
						uint16_t point2 = params.pointers[u+3];
						
						SCR_CMD_HEADER *header2 = get_scr_typepoint(SCR_CMD_HEADER, params.script, point2);
						
						if ( header2->type == SCRCMD_GOTO ) {
							output += tabs + "ENDWHILE" + LINESEP;
						} else {
							output += tabs + "ENDIF" + LINESEP;
						}
						*/
						
						//is_endif = 0;	//  reset
						
						continue;    //  go to next pointer
						
					} else {
						//  It's a link between boolean checks
					}
					

				}
				
				// indents handling:
				int tabslen = tabs.length();
				
				//  If all IFs or WHILEs are ended
				if ( tabslen < 2 ) {
					inside_if_while = 0;
					else_count = 0;			//  reset
				}
				
				if ( inside_if_while == 1 ) {
					//  Check if it has reached an ELSE
					if ( else_count > 0 ) {
						for (int else_index = 0; else_index < else_count; else_index++ ) {
							if ( params.header->cmd_this == else_point[else_index] ) {
								tabs = tabs.substr(0, tabs.length()-3);   //  Remove TABS
								output += tabs + "ELSE" + LINESEP;
								tabs += "\t\t\t";						  //  restaure TABS
								break;
							}
						}
					}
				}
					
				
				//   TODO       skipping IF_JUMP for now
				if ( params.header->type == SCRCMD_IF_JUMP ) {
					continue;
				}
				
				if(params.header->type == SCRCMD_FUNCTION){
					output += LINESEP;
				}else if(params.header->type == SCRCMD_RETURN){
					if(tabslen > 0 && inside_if_while == 0){			//  It will remove the function indent only it the RETURN command isn't in a IF/WHILE
						tabs = tabs.substr(0, tabs.length()-1);
					}
				}
				
				
				if(SCR_debug_all_lines){
					if(is_bool_function(params.header->type)){
						cmdstr = hexdump(params, 0); // show all data for boolean functions, but don't display the command name string.
					}else{
						cmdstr = hexdump_short(params); // shows only "(cmd_this -> cmd_next)"
					}
					output += tabs + retval + " " + cmdstr + LINESEP; // full debug info     
				}else{
					output += tabs + retval + LINESEP; // no debug info for known commands!
				}
				

				// indents handling:
				if(params.header->type == SCRCMD_FUNCTION){
					tabs += "\t";
				}else if(params.header->type == SCRCMD_RETURN){
					if(tabslen > 0 && inside_if_while == 0){ // don't add space if it wasnt inside a function.
						output += LINESEP;
					}
				}
			}else{
				if(SCR_debug_all_lines){
					output += tabs + hexdump(params) + LINESEP; // output unknown commands with their hex values.
				}else{
					if(params.header->type != 0){
						//  Deactivated for now							TODO
						//output += tabs + string("// CMD:") + get_scr_cmd(params.header->type) + LINESEP; // show only command names.
					}
				}
			}
		}
		#else
		if(params.point != 0){
			if((it = Functions.find(params.header->type)) != Functions.end()){
				
				//  Checking if it is a IF_JUMP
				if ( params.header->type != SCRCMD_IF_JUMP ) {
					const string &retval = it->second(params);	   //  Resume normal behavior
				} else {
					
					//  Detecting if this IF_JUMP is a AND / OR link, or if it is an ENDIF/ENDWHILE
					get_data(SCR_IF_JUMP, data);
					
					if( params.header->return_value == 0 && data.or_logical_operator == 0 ){
						//  It's the end of a IF or a WHILE
						//  Now we need to check if there is a GOTO in current_pointer + 3
						
						SCR_CMD_HEADER header2 = get_scr_typepoint(SCR_CMD_HEADER, params.script, params.point+3);
						
						if ( header2->type == SCRCMD_GOTO ) {
							output += tabs + "ENDWHILE" + LINESEP;
						} else {
							output += tabs + "ENDIF" + LINESEP;
						}
						
						continue;    //  go to next pointer
						
					}
					

				}
					
					
				// indents handling:
				int tabslen = tabs.length();
				
				// function identing
				if(params.header->type == SCRCMD_FUNCTION){
					output += LINESEP;
				}else if(params.header->type == SCRCMD_RETURN){
					if(tabslen > 0){
						tabs = tabs.substr(0, tabs.length()-1);
					}
				}
				
				// IF identing
				/*
				if(params.header->type == SCRCMD_IF){
					output += LINESEP;
				}else if(params.header->type == SCRCMD_IF_JUMP && ){		//   TODO  /////////////////////////////////////////////////////
					if(tabslen > 0){
						tabs = tabs.substr(0, tabs.length()-1);
					}
				}
				*/

				output += tabs + retval + LINESEP; // execute corresponding function to params.type.

				// indents handling:
				if(params.header->type == SCRCMD_FUNCTION){
					tabs += "\t";
				}else if(params.header->type == SCRCMD_RETURN){
					if(tabslen > 0){ // don't add space if it wasnt inside a function.
						output += LINESEP;
					}
				}
			}
		}
		#endif
	}
}

// updates the max pointers that the base+mis script combination has got at the moment:
// this is used to optimize some functions and to know exactly how much
// should we output (its visible if using SCR_INCLUDE_DEBUG). and also to be able to
// validate the base script amount of pointers compared to the value the mission script told.
// call this every time the pointers array has changed!
void update_max_used_pointers(FPStruct &params){
	for(int u = params.max_total_pointers-1; u >= 0; u--){
		if(params.pointers[u] != 0){
			params.used_pointers = u+1;
			break;
		}
	}
}

// note: fp may be NULL after file was closed/couldn't be opened!
#define mission_error_handler(errcode, errstr) \
	if(elems == 1){ \
		free_safe(pointers); \
		free_safe(script); \
		free_safe(strings); \
		if(fp != NULL) fclose(fp); \
		return errcode; \
	}else{ \
		mission_scripts_output.push_back(errstr); \
		if(fp != NULL) fclose(fp); \
		continue; \
	}

////////////////////////////////////////
// main decompiler function.
//
// return 0; // Success!
// return 1; // Error: choose base script.
// return 2; // Error: could not open base file (file not found).
// return 3; // Error: not enough memory (or skipped allocating it for some weirdo reason).
// return 4; // Error: wrong base script file!
// return 5; // Error: mission script not found or couldn't be opened for reading.
// return 6; // Error: mission script too large!
////////////////////////////////////////
SCR_DECOMPILER_API int decompile_scr(const string &base_script,	string &base_script_output, 
									 const vector<string> &mission_scripts, vector<string> &mission_scripts_output)
{
	#ifdef SCR_INCLUDE_DEBUG
	if(SCR_use_debug){
		SCR_ERRORS_STR = ""; // clear errors
	}
	#endif

	// predefined constants:
	const int max_mission_pointers = 1536; // static.
	const int max_base_pointers = 6000; // static.
	const int max_script_size = 65536; // static. note: base+mission sizes can't exceed this!
	const int max_strings_size = 5120; // static. note: mission scripts doesn't have this!
	// set our decompiler memory larger so we can fit mission in it.
	const int max_total_pointers = max_base_pointers+max_mission_pointers; // allocate base+mis amount just in case if we exceed (havent tested, and doesn't take much more mem).
	const int max_total_pointers_size = max_total_pointers*sizeof(uint16_t);
	const int max_base_pointers_size = max_base_pointers*sizeof(uint16_t);
	const int max_mission_pointers_size = max_mission_pointers*sizeof(uint16_t);

	uint16_t *pointers = NULL; // can contain base+mis data.
	uint8_t *script = NULL; // can contain base+mis data.
	uint8_t *strings = NULL; // only read from base script file.

	FILE *fp = NULL;

	// read data:
	if((fp = fopen(base_script, "rb"))){
		// read 2 bytes determining if its a mission script or not:
		uint16_t base_script_max_pointers;
		fread(&base_script_max_pointers, sizeof(uint16_t), 1, fp);
		// if it is a mission script:
		if(base_script_max_pointers > 0){
			// don't allow reading mission script as the base_script!
			// tell the user to input the base script here instead:
			fclose(fp);
			return 1; // error: choose base script.
		}else{
			// allocate memory for base+mis:
			pointers = (uint16_t *)malloc(max_total_pointers_size);
			memset(pointers, 0, max_total_pointers_size); // be sure there's no garbage, since we allocated more than we read!
			script = (uint8_t *)malloc(max_script_size);
			strings = (uint8_t *)malloc(max_strings_size);
			// read all the base-script data to shared buffers:
			fread(pointers+1, 1, max_base_pointers_size-sizeof(uint16_t), fp); // read to next item in array, we already read the first pointer!
			fread(script, 1, max_script_size, fp); // read whole script
			fread(strings, 1, max_strings_size, fp); // note: missions don't have strings array.
			pointers[0] = base_script_max_pointers; // place the value back we skipped. (probably should just set to zero since it SHOULD be always zero).
			// build up the array for accessing the zone/gang etc. strings:
			build_scr_strings_array(strings);
			fclose(fp);
		}
	}else{
		return 2; // error: could not open base file (file not found).
	}

	// just in case:
	if(pointers == NULL || script == NULL || strings == NULL){
		free_safe(pointers);
		free_safe(script);
		free_safe(strings);
		return 3; // error: not enough memory (or skipped allocating it for some weirdo reason).
	}

	// some functions needs these:
	FPStruct params;
	params.filename = base_script;
	params.script = script;
	params.pointers = pointers;
	params.strings = strings;
	params.used_pointers = max_base_pointers; // varies depending on current base+mis pointers.
	params.max_total_pointers = max_total_pointers; // 6k+1.5k

	// find max index: to strip out the nulls at end of output,
	// and to check if mission script says the correct value for base script pointers:
	update_max_used_pointers(params);

	///////////////////////////////////////////////
	//
	//   Start parsing:
	//

	// reset global arrays:
	varnamearray.clear();
	nameprefixcounter.clear();
	funcnamearray.clear();
	funcnameprefixcounter.clear();

	// empty the arrays to prevent user from messing up by having already filled array:
	mission_scripts_output.clear();

	////////////////////////////////////
	//
	//   Base script:
	//

	// decompile base script:
	base_script_output = build_credits(params.filename);
	collect_functions(params, 1, params.used_pointers, base_script_output, 0); // 1 = skip the nullpointer
	process_pointers(params, 1, params.used_pointers, base_script_output);

	int base_script_used_pointers = params.used_pointers; // params.used_pointers will vary if using missions!
	
	/////////////////////////    variable names stuff    2
	
	// store a copy of the base script variable/function names:
	uint16_tStrMap varnamearray_base;
	StrIntMap nameprefixcounter_base;
	uint16_tStrMap funcnamearray_base;
	StrIntMap funcnameprefixcounter_base;
	// if there is more than 1 mission read at once, there is a possibility that the previous mission file increments
	// the counters for the variable/function names, but because mission scripts can't access each other,
	// so they should always add ONLY to the base script names, process_pointers() modifies these arrays.
	bool reset_varname_arrays = (mission_scripts.size() > 1); // to know that we have some data copied in these arrays.
	if(reset_varname_arrays){
		// these are used to reset before loading the second mission:
		varnamearray_base = varnamearray;
		nameprefixcounter_base = nameprefixcounter;
		funcnamearray_base = funcnamearray;
		funcnameprefixcounter_base = funcnameprefixcounter;
	}
	
	// Testing
	//reset_varname_arrays = false;     //  Testing

	////////////////////////////////////
	//
	//   Mission scripts:
	//

	// decompile mission scripts into separate string buffers (if any exists):
	int elems = mission_scripts.size();
	for(int u = 0; u < elems; u++){
		params.filename = mission_scripts[u]; // update internal filename (might be used by some functions).
		
		int mission_number = (int) (u+1)/2;
		
		//  To give non-duplicated names to variables
		
		VARNAME_SOUND = 			"m_" + sprintf_str("%i", mission_number) + "_sound";
		VARNAME_OBJECT = 			"m_" + sprintf_str("%i", mission_number) + "_obj";
		VARNAME_CAR = 				"m_" + sprintf_str("%i", mission_number) + "_auto";
		VARNAME_CHAR = 				"m_" + sprintf_str("%i", mission_number) + "_chr";
		VARNAME_BONUS = 			"m_" + sprintf_str("%i", mission_number) + "_bonus";
		VARNAME_ARROW = 			"m_" + sprintf_str("%i", mission_number) + "_arrow";
		VARNAME_TIMER = 			"m_" + sprintf_str("%i", mission_number) + "_timer";
		VARNAME_ONSCREEN_COUNTER =  "m_" + sprintf_str("%i", mission_number) + "_onscreen";
		VARNAME_LIGHT = 			"m_" + sprintf_str("%i", mission_number) + "_light";
		VARNAME_COUNTER = 			"m_" + sprintf_str("%i", mission_number) + "_count";
		VARNAME_FUNCTION = 			"m_" + sprintf_str("%i", mission_number) + "_function";

		if((fp = fopen(params.filename, "rb"))){
			// read header:
			SCR_MISSION_HEADER header;
			fread(&header, sizeof(SCR_MISSION_HEADER), 1, fp);

			// check if pointers count doesn't match base script:
			if(header.base_script_max_pointers != base_script_used_pointers){
				mission_error_handler(4, "// Error: Wrong base script file!");
			}
			// read mission script data directly after the base script data:
			// and because we read always same amount of pointers that has nulls padded in end of it,
			// we don't need to care about cleaning the previous pointers from messing up anything.
			int max_basepointers = header.base_script_max_pointers;
			fread(pointers+max_basepointers, 1, max_mission_pointers_size, fp);
			update_max_used_pointers(params);
			// read the script after the base script, no need to care about garbage data!
			// find out the position where we should paste it by taking the first pointer value from mission script:
			int script_start_offset = pointers[max_basepointers];
			int script_size = header.mission_script_size;
			// check script size since it can change in the header:
			if(script_start_offset+script_size > max_script_size){
				mission_error_handler(6, "// Error: Mission script too large!");
			}
			fread(script+script_start_offset, 1, script_size, fp);
			fclose(fp);

			// copy the base script variable/function counter settings only when its needed:
			if(reset_varname_arrays && u > 0){
				varnamearray = varnamearray_base;
				nameprefixcounter = nameprefixcounter_base;
				funcnamearray = funcnamearray_base;
				funcnameprefixcounter = funcnameprefixcounter_base;
			}
			// process _only_ the amount of pointers we readed from the mission script:
			string mission_output = build_credits(params.filename);
			collect_functions(params, max_basepointers, params.used_pointers, mission_output, 1);
			process_pointers(params, max_basepointers, params.used_pointers, mission_output);

			// add to output array:
			mission_scripts_output.push_back(mission_output);
		}else{
			mission_error_handler(5, "// Error: File not found or couldn't be opened for reading.");
		}
	}

	free_safe(pointers);
	free_safe(script);
	free_safe(strings);

	#ifdef SCR_INCLUDE_DEBUG
	if(SCR_ERRORS_STR.length() > 0){
		string errorfile = sprintf_str("ERROR_%s.txt", BaseName(base_script,0).c_str()); // "ERROR_ste.txt"
		save_data(errorfile, SCR_ERRORS_STR);
	}
	#endif

	return 0; // success!
}
