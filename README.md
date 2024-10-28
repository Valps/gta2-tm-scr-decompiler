## Modifications by Valps

- Now IFs, ELSEs, WHILEs, WHILE_EXECs, NOTs, ORs and ANDs are decompiled;
- The "FORWARD" command doesn't appear in missions scripts anymore, only in base script;
- Now the RETURN command doesn't remove indentation if it is in a IF/WHILE block;
- All variables declarations that happens in mission script now have unique (i.e. non-duplicated) names;
- Now structures "DO ... WHILE_TRUE" can be decompiled as well;

## Known issues

- It crashes if you try to decompile Industrial District with all missions at once. However its main script (bil.scr) can be decompiled without problems.

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
