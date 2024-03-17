//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <string>
#include <vector>

//external
#include "glm.hpp"

using std::string;
using std::istringstream;
using std::vector;
using glm::vec3;

namespace Utils
{
	class StringUtils
	{
	public:
		/// <summary>
		/// Find and replace all occurences of search in original with replacement.
		/// </summary>
		/// <param name="original">The original string that was inserted</param>
		/// <param name="search">The part that needs to be replaced.</param>
		/// <param name="replacement">The replacement that is added.</param>
		/// <returns>New string with replaced parts.</returns>
		static string StringReplace(
			const string& original, 
			const string& search, 
			const string& replacement);

		/// <summary>
		/// Convert an inserted vector string to a vec3.
		/// </summary>
		/// <param name="original">A vector of string x, y and z positions.</param>
		/// <returns></returns>
		static vec3 StringToVec3(const vector<string>& original)
		{
			vec3 output{};

			istringstream issX(original[0]);
			istringstream issY(original[1]);
			istringstream issZ(original[2]);

			issX >> output.x;
			issY >> output.y;
			issZ >> output.z;

			return output;
		}

		/// <summary>
		/// Split a string into pieces and remove the delimiter.
		/// </summary>
		/// <param name="input">The original string.</param>
		/// <param name="delimiter">Where is the string split from.</param>
		/// <returns>A vector of all string parts that were split.</returns>
		static vector<string> Split(
			const string& input,
			char delimiter);

		/// <summary>
		/// Check if the selected file contains the selected string.
		/// </summary>
		/// <param name="filePath"></param>
		/// <param name="targetString"></param>
		/// <returns></returns>
		static bool FileContainsString(const string& filePath, const string& targetString);

		/// <summary>
		/// Checks whether the inputted string can be converted to float or not.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static bool CanConvertStringToFloat(const string& value);

		/// <summary>
		/// Checks whether the inputted string can be converted to int or not.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		static bool CanConvertStringToInt(const string& value);
	};
}