/*
 * Calculate the MD5 from file
 * Verification the MD5 from file
 */
#include <string>
#include <stdio.h>
#include "miui_func.hpp"

using namespace std;
//class miui_func;
//class utils {
class utils : public miui_func {
	public:
		utils();
		~utils();
		void get_file_in_folder(const char *backup_path);
		bool Make_MD5(string backup_path);
		bool Check_MD5(string backup_path);
	
	private:
	//	miui_func md5sum;
		 int count;
		 int f_alloc;
		 char **filename;
		// void free_string_array(char** arrary);


};




