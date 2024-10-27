/*
 * This is a simple tool to demonstrate and test the SCR decompiler library.
 * The other source files for the library are not open source, but this file
 * is released into the public domain. Written by Oliver Galvin, 2021.
 */

#include <scr_decompiler_api.h>

#include <stdio.h>	  //  para pausar

#include <fstream>
#include <bits/stl_vector.h>
#include <filesystem>
namespace fs = std::filesystem;

int
main(int argc, char **argv)
{
	if(argc < 3) {
		printf("Usage: %s OUT_DIR SCRIPT_PATH SCRIPT_DIR\n", argv[0]);
		getchar();
		getchar();
		return EXIT_FAILURE;
	}
	int ret;
	string out_dir = argv[1];
	string base_script = argv[2];
	string base_script_output;
	vector<string> mission_scripts;
	vector<string> mission_scripts_output;
	printf("Base script: '%s'.\n", base_script.c_str());
	if(argc > 3) {
		string subscript_dir = argv[3];
		for(const auto & entry : fs::directory_iterator(subscript_dir)) {
			printf("Found mission script: '%s'\n", entry.path().filename().c_str());
			mission_scripts.push_back( entry.path().generic_string() );		//                                    :(
		}
		printf("Found %zu scripts in the mission script directory '%s'.\n", mission_scripts.size(), subscript_dir.c_str());
	} else {
		printf("No mission script directory given.\n");
	}
	printf("Decompiling... ");
	ret = decompile_scr(base_script, base_script_output, mission_scripts, mission_scripts_output);
	printf("done.\n");

	if(fs::is_directory(out_dir)) {
		printf("Output directory '%s' already exists.\n", out_dir.c_str());
	} else if(fs::exists(out_dir)) {
		printf("Cannot save - file exists at the given output directory path!\n");
	} else {
		printf("Creating output directory '%s'.\n", out_dir.c_str());
		fs::create_directory(out_dir);
	}
	printf("Saving... ");
	fs::path out_path = out_dir;
	fs::path old_script = base_script;
	old_script.replace_extension(".mis");
	ofstream script_out(out_path / old_script.filename());
	script_out << base_script_output;
	script_out.close();

	for(size_t i=0; i<mission_scripts.size(); i++) {
		fs::path old_subscript = mission_scripts[i];
		string subscript = mission_scripts_output[i];
		old_subscript.replace_extension(".mis");
		ofstream subscript_out(out_path / old_subscript.filename());
		subscript_out << subscript;
		subscript_out.close();
	}
	printf("done.\n");
	//getchar();
	//getchar();
	return ret;
}