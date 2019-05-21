#ifndef INIReader_INIReader_h
#define INIReader_INIReader_h

#include <map>
#include <string>

using std::map;
using std::wstring;


class IniReader {
public:
	static IniReader &getInstance();	// to be used as singleton
	int GetParamValue(const wstring &key, wstring &value);

private:
	IniReader(void);	// hide ctor
	IniReader(IniReader const&);	// avoid copy
	void operator=(IniReader const&);	// avoid assignment

	int readFile(const wstring &filename);
	int retrieveFilePath(wstring  &filepath);
	int parseContentLine(wstring const &contentLine);

	map<wstring, wstring> paramMap;
};


#endif