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

string sprintf_str(const char *fmt, ...){
	const size_t bufsize = 1024; // very unlikely to exceed this.
	char buf[bufsize];
	int outsize;
	va_list ap;
	va_start(ap, fmt);
	outsize = vsnprintf(buf, bufsize, fmt, ap);
	va_end(ap);
	if(outsize != -1){
		return string(buf, outsize);
	}else{
		return string(buf, bufsize); // error: output too large, just cut output to maximum buffer size.
	}
}

string BaseName(const string &filename, bool leave_extension = 1){
	int startpos;
	if((startpos = filename.find_last_of('\\')) == -1){
		startpos = filename.find_last_of('/'); // support \ and / folder separators. if error, will start from -1+1 = 0 (returns whole string).
	}
	if(leave_extension){
		return filename.substr(startpos+1);
	}else{
		string basen = filename.substr(startpos+1);
		int extpos = basen.find_last_of('.');
		if(extpos == -1){
			return basen; // no dot, return whole basefilename.
		}else{
			return basen.substr(0, extpos);
		}
	}
}

FILE *fopen(const string &filename, const string &mode){
	return fopen(filename.c_str(), mode.c_str());
}

bool save_data(const string &filename, const string &buf, const string &rights = "wb"){
	if(FILE *save = fopen(filename, rights)){
		fwrite(buf.c_str(), buf.length(), 1, save);
		fclose(save);
		return 1;
	}
	return 0;
}
