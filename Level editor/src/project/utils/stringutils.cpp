//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>
#include <sstream>
#include <algorithm>

//level editor
#include "stringutils.hpp"

using std::istringstream;

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
		vector<string> tokens;
		string token;
		istringstream tokenStream(input);
		while (getline(tokenStream, token, delimiter))
		{
			tokens.push_back(token);
		}
		return tokens;
	}

	bool StringUtils::CanConvertStringToFloat(const string& value)
	{
		size_t pos;
		float convertedValue = stof(value, &pos);

		return pos == value.length();
	}

	bool StringUtils::CanConvertStringToInt(const string& value)
	{
		size_t pos;
		int convertedValue = stoi(value, &pos);

		return pos == value.length();
	}
}