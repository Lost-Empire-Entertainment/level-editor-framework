//Copyright (c) <2024> <Greenlaser>

#pragma once

#include <string>

using std::string;

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
	};
}