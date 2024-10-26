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
// Notes:
// - Do not define SCR_DECOMPILER_EXPORTS if you are using the DLL from another project!
// - SCR_DECOMPILER_EXPORTS should be defined only in (this) DLL project.
// - Set both projects to use the same setting: "Multi-threaded DLL (/MD)" or it will crash!
////////////////////////////

#include <string>
#include <vector>
using namespace std;

/** External function API linkage. */
#ifndef SCR_DECOMPILER_API
# if defined(_WIN32) || defined(__CYGWIN__)
#  if defined(SCR_DECOMPILER_EXPORTS)
#   define SCR_DECOMPILER_API __declspec(dllexport)
#  else
#   define SCR_DECOMPILER_API __declspec(dllimport)
#  endif
# else
#  if __GNUC__ >= 4
#   define SCR_DECOMPILER_API __attribute__((visibility("default")))
#  else
#   define SCR_DECOMPILER_API
#  endif
# endif
#endif

SCR_DECOMPILER_API int decompile_scr(const string &base_script, string &base_script_output,
									 const vector<string> &mission_scripts, vector<string> &mission_scripts_output);

extern SCR_DECOMPILER_API string VARNAME_PLAYER;
extern SCR_DECOMPILER_API string VARNAME_SOUND;
extern SCR_DECOMPILER_API string VARNAME_CONVEYOR;
extern SCR_DECOMPILER_API string VARNAME_OBJECT;
extern SCR_DECOMPILER_API string VARNAME_SHOP;
extern SCR_DECOMPILER_API string VARNAME_GENERATOR;
extern SCR_DECOMPILER_API string VARNAME_CAR;
extern SCR_DECOMPILER_API string VARNAME_CHAR;
extern SCR_DECOMPILER_API string VARNAME_DOOR;
extern SCR_DECOMPILER_API string VARNAME_RADIO;
extern SCR_DECOMPILER_API string VARNAME_CRANE;
extern SCR_DECOMPILER_API string VARNAME_CRUSHER;
extern SCR_DECOMPILER_API string VARNAME_DESTRUCTOR;
extern SCR_DECOMPILER_API string VARNAME_BONUS;
extern SCR_DECOMPILER_API string VARNAME_ARROW;
extern SCR_DECOMPILER_API string VARNAME_TIMER;
extern SCR_DECOMPILER_API string VARNAME_ONSCREEN_COUNTER;
extern SCR_DECOMPILER_API string VARNAME_LIGHT;
extern SCR_DECOMPILER_API string VARNAME_SAVED_COUNTER;
extern SCR_DECOMPILER_API string VARNAME_THREAD_TRIGGER;
extern SCR_DECOMPILER_API string VARNAME_THREAD_ID;
extern SCR_DECOMPILER_API string VARNAME_COUNTER;
extern SCR_DECOMPILER_API string VARNAME_FUNCTION;

extern SCR_DECOMPILER_API string DEC_STR;
extern SCR_DECOMPILER_API string FUNCSPACE;
extern SCR_DECOMPILER_API string SCR_FLOAT_PATTERN;

extern SCR_DECOMPILER_API bool SCR_use_debug;
extern SCR_DECOMPILER_API bool SCR_debug_all_lines;
