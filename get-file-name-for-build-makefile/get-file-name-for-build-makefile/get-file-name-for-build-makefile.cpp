// get-file-name-for-build-makefile.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define BUF_LENGTH 256
using namespace std;
void listFiles(const char * dir, FILE* file);

int main()
{
	char dir[BUF_LENGTH];
	cout << "###############   Tip!   ###############" << endl;
	cout << "	Input: NULL,	Auto Get Program Root Dir!" << endl;
	cout << "	Input: exit or quit,	End Of Job!" << endl;
	cout << "###############   END!   ###############\n" << endl;

	while (true)
	{
		cin.getline(dir, BUF_LENGTH);//输入为空时，将程序根目录输入
		if (strcmp(dir, "") == 0) {
			getcwd(dir, BUF_LENGTH);//获取当前路径
		}
		else if (strcmp(dir, "quit") == 0 || strcmp(dir, "exit") == 0){
			return 0;
		}
		cout << dir << endl;

		char file_name[BUF_LENGTH];	//文件操作
		strcpy(file_name, dir);
		strcat(file_name, "/makefile");
		FILE* fp = fopen(file_name, "w");
		if (fp == NULL) {
			cout << "Input Error Dir!\n\n";
			continue;
		}

		char sub_file[BUF_LENGTH];	 // 遍历.c
		strcpy(sub_file, dir);
		strcat(sub_file, "/*.c");
		listFiles(sub_file, fp);

		char sub_dir[BUF_LENGTH];   // 遍历子目录
		strcpy(sub_dir, dir);
		strcat(sub_dir, "/*.");
		listFiles(sub_dir, fp);

		fclose(fp);
		cout << "Sub makefile Done!\n" << endl;
	}
	return 0;
}

void listFiles(const char * dir, FILE* file)
{
	intptr_t handle;
	_finddata_t findData;

	handle = _findfirst(dir, &findData);    // 查找目录中的第一个文件
	if (handle == -1)
	{
		cout << "Failed to find first file!\n";
		return;
	}
	do
	{
		if (findData.attrib & _A_SUBDIR) {
			if (strcmp(findData.name, ".") != 0 && strcmp(findData.name, "..") != 0) {    // 是否是子目录并且不为"."或".."
				cout << findData.name << "\t<dir>\n";
				//cout << "obj-y += " <<findData.name << "/\n";
				char temp[BUF_LENGTH] = "obj-y += ";
				strcat(temp, findData.name);
				strcat(temp, "/\n");
				fputs(temp, file);
			}
		}
		else {
			//cout << findData.name << "\t" << findData.size << endl;
			string file_name = findData.name;
			if (file_name != "makefile") {
				file_name[strlen(findData.name) - 1] = 'o';
				cout << "obj-y += " << file_name.c_str() << endl;
				char temp[BUF_LENGTH] = "obj-y += ";
				strcat(temp, file_name.c_str());
				strcat(temp, "\n");
				fputs(temp, file);
			}
		}
	} while (_findnext(handle, &findData) == 0);    // 查找目录中的下一个文件
	_findclose(handle);    // 关闭搜索句柄
}