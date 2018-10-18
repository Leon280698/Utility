#pragma once

#include <string>
#include <cctype>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <string_view>

#define UTIL_STR(x) #x
#define UTIL_STRINGIFY(x) UTIL_STR(x)

namespace util::str{
	//Removes path and returns only file name with extension
	inline std::string file_name(const std::string& path){
		std::size_t index = path.find_last_of("\\/");

		if(index == std::string::npos)
			index = 0;
		else
			++index;

		return path.substr(index, path.length());
	}

	//Removes file name and returns only path
	inline std::string path(const std::string& path){
		std::size_t index = path.find_last_of("\\/");

		if(index == std::string::npos || index == path.length())
			index = 0;
		else
			++index;

		return path.substr(0, index);
	}

	//Removes file extension from single file name or path with file name
	inline std::string without_file_extension(const std::string& fileName){
		std::size_t extPos = fileName.find_last_of(".");
		std::size_t pathPos = fileName.find_last_of("\\/");

		if(extPos != std::string::npos){
			if(pathPos == std::string::npos || pathPos < extPos)
				return fileName.substr(0, extPos);
		}

		return fileName;
	}

	//Returns file extension of specified file name or empty string if there was none
	inline std::string file_extension(std::string_view fileName){
		std::size_t exPos = fileName.find_last_of(".");
		std::string ext;

		if(exPos != std::string_view::npos){
			ext = fileName.substr(exPos, fileName.length());

			if(ext.length() == 1)
				ext = "";
		}else{
			ext = "";
		}

		return ext;
	}

	//Joins paths adding separators if needed
	inline std::string join_paths(std::string_view result){
		return result.data();
	}

	template<typename ... Args>
	std::string join_paths(std::string_view p1, std::string_view p2, Args&& ...args){
		std::string joinedPath;

		if(p1.back() != '/' && p1.back() != '\\' && p2[0] != '/' && p2[0] != '\\')
			joinedPath = std::string{p1} + "/" + p2.data();
		else
			joinedPath = std::string{p1} + p2.data();

		return join_paths(joinedPath, args...);
	}

	//Converts string to lower case
	inline std::string to_lower(std::string_view s){
		std::string result;

		result.reserve(s.size());

		for(char c : s)
			result += static_cast<char>(std::tolower(c));

		return result;
	}

	//Converts string to upper case
	inline std::string to_upper(std::string_view s){
		std::string result;

		result.reserve(s.size());

		for(char c : s)
			result += static_cast<char>(std::toupper(c));

		return result;
	}

	//Extracts single words from string and stores them in a vector
	inline std::vector<std::string> split(std::string_view s){
		std::istringstream iss{s.data()};
		std::vector<std::string> result;
		std::string temp;

		while(iss >> temp)
			result.push_back(temp);

		return result;
	}

	//Splits string at specified delimiter and returns vector of separated strings
	inline std::vector<std::string> split_at(std::string_view s, char delimiter){
		std::istringstream iss{s.data()};
		std::vector<std::string> result;
		std::string temp;

		while(std::getline(iss, temp, delimiter)){
			if(!temp.empty())
				result.push_back(temp);
		}

		return result;
	}

	//Type conversion from and to std::string

	template<typename T>
	std::string to_string(T value);

	template<typename T>
	T to_value(const std::string& string);

	inline void to_string_vector(std::vector<std::string>&){} //Dummy used to end recursion

	template<typename T, typename ... Args>
	void to_string_vector(std::vector<std::string>& result, const T& t, Args ...args){
		result.push_back(static_cast<std::ostringstream&&>((std::ostringstream{} << t)).str());
		to_string_vector(result, args...);
	}

	/*
	*	Templated string formatting.
	*	fmt can contain the index of the value to insert within curly braces
	*	e.g. {1} would mean that the first parameter passed to the function will
	*	be added at this position
	*	One index can be used multiple times and the order is completely irrelevant
	*/
	template<typename ... Args>
	std::string format(std::string_view fmt, Args&& ...args){
		std::string result;
		std::vector<std::string> values;

		result.reserve(fmt.size());
		to_string_vector(values, args...);

		for(std::size_t i = 0; i < fmt.size(); ++i){
			if(fmt[i] == '{'){
				char* end;
				std::size_t index = std::strtoul(&fmt[i + 1], &end, 10);
				std::size_t start = i;

				for(; &fmt[i] != end; ++i){}

				if(*end == '}' && index > 0 && index < values.size())
					result += values[index - 1];
				else
					result += std::string_view{&fmt[start], i - start + 1};
			}else{
				result.push_back(fmt[i]);
			}
		}

		return result;
	}

	//Case insensitive hash
	struct CaseInsensitiveHash{
		std::size_t operator()(std::string_view s) const{
			return std::hash<std::string>{}(to_lower(s));
		}
	};

	//Case insensitive comparison
	struct CaseInsensitiveEqual{
		bool operator()(std::string_view s1, std::string_view s2) const{
			return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char a, char b){
				return std::tolower(a) == std::tolower(b);
			});
		}
	};

	//Case insensitive less for ordered containers
	struct CaseInsensitiveLess{
		bool operator()(std::string_view s1, std::string_view s2) const{
			return std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end(), [](char a, char b){
				return std::tolower(a) < std::tolower(b);
			});
		}
	};
}

#include "stringUtil.inl"