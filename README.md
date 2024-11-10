## Modifications by Valps

- Now IFs, ELSEs, WHILEs, WHILE_EXECs, NOTs, ORs and ANDs are decompiled;
- The "FORWARD" command doesn't appear in missions scripts anymore, only in base script;
- Now the RETURN command doesn't remove indentation if it is in a IF/WHILE block;
- All variables declarations that happens in mission script now have unique (i.e. non-duplicated) names;
- Now structures "DO ... WHILE_TRUE" can be decompiled as well;
- The char objective ENTER_CAR_AS_PASSENGER is no longer misreplaced by ENTER_CAR_AS_DRIVER;
- All scripted lights that have radius 8.00 will be decompiled to 7.99

## Known issues

- If you try to decompile a script that uses the memory changer command (CHANGE_GANG_CHAR_RESPECT), it will give \<*STRNOTFOUND*\> to the first parameter as decompiler will expect it to be a GANG from SET_GANG_INFO instead of a COUNTER.

## Compiling and building

- To compile & build the decompiler, execute "*compile_and_build.bat*" from the root folder. Ensure that you have installed "Python", "Meson Build System", "Ninja" and a C++ compiler. The executable will be created at **%ROOT_FOLDER%/build/** as well as libscrd.dll. Delete the "build" folder if you want to recompile. I particularly have used MinGW-W64 i686-ucrt-posix-dwarf gcc win32 14.2.0 compiler, LLVM 19.1.1. You can download it at https://winlibs.com/ . Remember to put the "bin" folder in PATH system environment variable, for example "C:\MinGW\mingw32\bin\".
- To compile & build the Graphical User Interface (GUI), execute "*compile_build.bat*" from **%ROOT_FOLDER%/gui/**. Ensure that you have installed a Java Development Kit (JDK) in your system as well as Java Runtime Environment (JRE) to run the GUI.
- The GUI file "*GUIInterface.jar*" and the decompiler executable "*scr_decompiler.exe*" as well as "*libscrd.dll*" must be at the same folder in order to work properly.
- You might create an executable for "*GUIInterface.jar*" using a Java Executable Wrapper such as JSmooth ( https://jsmooth.sourceforge.net/ ).

Greetings from Valps.

Original fork: https://gitlab.com/classic-gta/gta2-scr-decompiler/-/tree/master

Original readme below:

# Epic GTA2 Script Decompiler

A C++ library to decompile a GTA2 binary script into its original readable
text form. Uncompiled `.mis` scripts are orginally compiled into binary `.scr`
script files by the official compiler from DMA Design. They are either shipped
with the original game or fan-made.

This library was written by **T.M.** from the [GTAMP Forum](https://gtamp.com/forum),
originally posted in [this thread](https://gtamp.com/forum/viewtopic.php?t=447)
in 2011. This repository contains that same code with only minor changes from
the original, in order to: fix compiler errors/warnings, remove non-standard
code to make the library portable between compilers and systems, add build
system support, and generally clean up the code.

## Building

A basic [Meson](https://mesonbuild.com) build file is included. There are no
dependencies, you will just need a C++ compiler to build the library.

To build, run:

    meson build
    ninja -C build

A shared library, and a basic tool which uses it, should now be built in a
subdirectory named `build`.

Note that Meson can also generate build files for Visual Studio and Xcode.

## Links

The official GTA2 script documentation and compiler from DMA Design (now
Rockstar North) can be found here: <https://gitlab.com/classic-gta/gta-data>.

## License

The code is released under a non-commercial license, so is not open source.
