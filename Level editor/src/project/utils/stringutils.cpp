//Copyright (c) <2024> <Lost Empire Entertainment>

//level loader
#include "stringutils.hpp"

namespace Utils
{
	string StringUtils::StringReplace(
		const string& original,
		const string& search,
		const string& replacement)
	{
		string result = original;
		size_t pos = 0;
		while ((pos = result.find(search, pos)) != string::npos)
		{
			result.replace(pos, search.length(), replacement);
			pos += replacement.length();
		}
		return result;
	}

	vector<string> StringUtils::Split(
		const string& input,
		char delimiter)
	{
		vector<string> none;
		return none;
	}
}