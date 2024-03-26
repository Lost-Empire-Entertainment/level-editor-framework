//Copyright (c) <2024> <Lost Empire Entertainment>

#pragma once

#include <string>

using std::string;

namespace File
{
	class FileExplorer
	{
	public:
		enum class SearchType
		{
			txt,
			asset
		};

		static string Select(const SearchType& searchType);
	};
}