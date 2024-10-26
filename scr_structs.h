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
//
// NOTE: THESE STRUCTS MUST BE USING 1 BYTE PACKING: #pragma pack(1)
// or else everything will fuck up.
//
//////////////////////////////////////////

typedef int32_t SCR_f; // use this for single float parameters in functions.

struct SCR_XYZ_f {
	SCR_f x,y,z;
};

struct SCR_XY_f {
	SCR_f x,y;
};

struct SCR_XY_c {
	int8_t x,y;
};

struct SCR_XYZ_c {
	int8_t x,y,z;
};

struct SCR_XY_uc {
	uint8_t x,y;
};

struct SCR_XYZ_uc {
	uint8_t x,y,z;
};

struct SCR_Rect_f {
	SCR_XYZ_f pos;
	SCR_XY_f size;
};

struct SCR_MISSION_HEADER {
	// how many pointers the base script has: (including first nullpointer and the ones
	// the IF commands generates etc. but not the nulls at the end of the pointers list)
	uint16_t base_script_max_pointers;
	uint16_t unk1; // garbage.
	uint16_t mission_script_size; // size in bytes.
	uint16_t unk2; // so far this has been zero.
};

struct SCR_STRING_HEADER {
	uint16_t str_id; // strings are converted into integers, this is reference point to translate integer -> string.
	uint16_t unk1;
	uint16_t datatype; // different commands have different value for this, its not same as the header.type value PLAYER_PED etc.
	uint16_t unk2;
	uint8_t len; // the following string length: (read separately).
};

struct SCR_CMD_HEADER { // copied to every function.
	uint16_t cmd_this; // current pointer index.
	uint16_t type; // which function type, PLAYER_PED etc.
	uint16_t cmd_next; // next pointer index.
	uint16_t return_value; // used by IF_JUMP's and boolean functions, seems to be return value for boolean functions, and endif marker for if_jumps (if zero).
};

// macro to reduce repetition for the header and to give common
// names for vars that can be accessed easily from header var:
#define SCR_CMD_FORMAT(code) code //; SCR_CMD_HEADER header; code; // original method, dont remove the use of SCR_CMD_FORMAT() in case we change functionality again!


///////////////////////////////////////////
//
//   MULTIPURPOSE SCR COMMAND STRUCTS:
//

// 4 bytes:
struct SCR_TWO_PARAMS {
	SCR_CMD_FORMAT(
		union {
			struct {
				uint16_t var1;
				uint16_t var2;
			};
			struct {
				int16_t sintvar1;
				int16_t sintvar2;
			};
			int32_t sint32val;
		};
	)
};

// 8 bytes:
struct SCR_FOUR_PARAMS {
	SCR_CMD_FORMAT(
		union {
			struct {
				uint16_t var1;
				uint16_t var2;
			};
			struct {
				int16_t sintvar1;
				int16_t sintvar2;
			};
		};
		union {
			struct {
				uint16_t var3;
				uint16_t var4;
			};
			struct {
				int16_t sintvar3;
				int16_t sintvar4;
			};
			int32_t sint32val;
			SCR_f floatval;
		};
	)
};

// 12 bytes:
struct SCR_SIX_PARAMS {
	SCR_CMD_FORMAT(
		union{
			struct {
				uint16_t var1;
				uint16_t var2;
				SCR_XY_f size;
			};
			SCR_XYZ_f pos;
		};
	)
};
