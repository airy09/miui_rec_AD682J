#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <libgen.h>
#include <sys/mman.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>


#include "miui_func.hpp"
#include "common.h"

using namespace std;

int miui_func::read_file(string fn, string& results) {
        ifstream file;
        file.open(fn.c_str(), ios::in);
        if (file.is_open()) {
                file >> results;
                file.close();
                return 0;
	}
        LOGI("Cannot find file %s\n", fn.c_str());
        return -1;
}

int miui_func::read_file(string fn, vector<string>& results) {
	ifstream file;
	string line;
	file.open(fn.c_str(), ios::in);
	if (file.is_open()) {
		while (getline(file, line))
			results.push_back(line);
		file.close();
		return 0;
	}
	LOGI("Cannot find file %s\n", fn.c_str());
	return -1;
}

int miui_func::write_file(string fn, string& line) {
	FILE *file;
	file = fopen(fn.c_str(), "w");
	if (file != NULL) {
		fwrite(line.c_str(), line.size(), 1, file);
		fclose(file);
		return 0;
	}
	LOGI("Cannot find file %s\n", fn.c_str());
	return -1;
}

void miui_func::setfn(string fn) {
	md5fn = fn;
}

int miui_func::computeMD5(void) {
	string line;
	struct MD5Context md5c;
	FILE *file;
	int len;
	unsigned char buf[1024];
	MD5Init(&md5c);
	file = fopen(md5fn.c_str(), "rb");
	if (NULL == file) 
		return -1;
	while ((len = fread(buf, 1, sizeof(buf), file)) > 0) {
		MD5Update(&md5c, buf, len);
	}
	MD5Final(md5sum, &md5c);
	return 0;
}

int miui_func::write_md5digest(void) {
	int i;
	string md5string, md5file;
	char hex[3];
	md5file = md5fn + ".md5";
	for (i = 0; i < 16; ++i) {
		snprintf(hex, 3, "%02x", md5sum[i]);
			md5string += hex;
	//	md5string += hex;
	}
	md5string += " ";
	md5string += basename((char*) md5fn.c_str());
	md5string += + "\n";
	write_file(md5file, md5string);
	return 0;
}

int miui_func::read_md5digest(void) {
	string md5file = md5fn + ".md5";
	if (read_file(md5file, line) != 0) 
		return -1;
	return 0;
}

int miui_func::verify_md5digest(void) {
	string buf;
	char hex[3];
	int i;
	string md5string;
	if (read_md5digest() != 0)
		return -1;
	stringstream ss(line);
	vector<string> tokens;
	while (ss >> buf)
		tokens.push_back(buf);
	computeMD5();
	for (i = 0; i < 16; ++i) {
		snprintf(hex, 3, "%02x", md5sum[i]);
		md5string += hex;
	}
	if (tokens.at(0) != md5string)
		return -2;
	return 0;
}



