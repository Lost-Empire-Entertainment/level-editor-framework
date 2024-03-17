//Copyright (c) <2024> <Lost Empire Entertainment>

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

//level editor
#include "stringutils.hpp"

using std::istringstream;
using std::ifstream;
using std::cout;

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

	bool StringUtils::FileContainsString(const string& filePath, const string& targetString)
	{
		ifstream file(filePath);
		if (!file.is_open())
		{
			cout << "Error opening file: " << filePath << "\n";
			return false;
		}

		string line;
		while (getline(file, line))
		{
			if (line.find(targetString) != string::npos)
			{
				file.close();
				return true;
			}
		}

		file.close();
		return false;
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