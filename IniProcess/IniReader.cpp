#include "pch.h"
#include <stdio.h>
#include <tchar.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>

#include "IniReader.h"

using std::wifstream;
using std::find;
using std::getline;

#define INI_FILE_NAME L"AIConfig.ini"

#define ANNOTATION_SYMBOL1 L"#"
#define ANNOTATION_SYMBOL2 L";"

//#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
//#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
//#endif		

#ifdef WIN32
#define FILE_SEPERATOR L"\\"
#else
#define FILE_SEPERATOR L"/"
#endif


// trim from begin
static inline std::wstring &ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::wstring &rtrim(wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline wstring &trim(wstring &s) {
	return ltrim(rtrim(s));
}

IniReader::IniReader(void) {
	// default
}

IniReader::IniReader(const IniReader &) {
	// default
}

void IniReader::operator =(IniReader const&) {
	// default
}

IniReader& IniReader::getInstance() {
	static IniReader instance;
	return instance;
}


/**
*
* get value by given key.
* @param key			(in)key
* @param valueBuffer	(out)value
* @return
*		-1:error
*		0:success
*		1:not found the key
*
*/
int IniReader::GetParamValue(const wstring &key, wstring &valueBuffer) {
	if (paramMap.empty()) {
		wstring filepath;
		int rtncode = retrieveFilePath(filepath);
		if (rtncode == 0) {
			readFile(filepath);
		}
		else {
			return -1;
		}
	}

	map<wstring, wstring>::iterator iter = paramMap.find(key);
	if (iter != paramMap.end()) {
		valueBuffer = iter->second;
		return 0;
	}
	else {
		return 1;//not found the key.
	}
}

/**
*
* read config file, add <key,value> into map.
* @param filepath (in)line text
* @param return
*		-1:error,invalid line
*		0:success
*
*/
int IniReader::readFile(const wstring &filename) {
	std::string strFilename(filename.begin(), filename.end());
	wifstream infile(strFilename.c_str());
	wstring buffer;
	while (getline(infile, buffer)) {
		parseContentLine(buffer);
	}

	return 0;
}

/**
*
* handle single line text,then add <key,value> into map.
* @param filepath (in)line text
* @param return
*		-1:error,invalid line
*		0:success
*
*/
int IniReader::parseContentLine(wstring &contentLine) {
	contentLine = trim(contentLine);
	if (contentLine.size() < 1) {
		return 0;   // blank line
	}

	if (contentLine.substr(0, 1) == ANNOTATION_SYMBOL1
		|| contentLine.substr(0, 1) == ANNOTATION_SYMBOL2) {
		return 0;   // comment
	}

	wstring::size_type equalPos = contentLine.find_first_of(L"=");
	wstring::size_type startPos = 0;
	wstring::size_type endPos = contentLine.size() - 1;

	if (equalPos <= startPos || equalPos > endPos) {
		return -1; // invalid line
	}

	wstring key = rtrim(contentLine.substr(startPos, equalPos));
	wstring value = ltrim(contentLine.substr(equalPos + 1, endPos));

	paramMap.insert(std::make_pair(key, value));

	//std::wcout <<key <<"\t" << value << std::endl;
	return 0;
}

/**
*
* get config file absolute path.
* @param filepath (out)
* @param return
*		-1:error
*		0:success
*
*/
int IniReader::retrieveFilePath(wstring &filepath) {
	/*
	wstring temppath = L"C:\\";
	if (temppath.size() < 1) {
		return -1;
	}
	wstring temp = FILE_SEPERATOR;
	if (temppath.compare(temppath.size() - 1, 1, FILE_SEPERATOR)) {
		temppath.append(FILE_SEPERATOR);
	}
	temppath.append(INI_FILE_NAME);
	*/
	wstring temppath = INI_FILE_NAME;

	filepath = temppath;
	return 0;
}