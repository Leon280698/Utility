#pragma once

namespace util::str{
	//to_string

	template<typename T>
	inline std::string to_string(T value){
		return std::to_string(value);
	}

	template<>
	inline std::string to_string<bool>(bool value){
		return value ? "true" : "false";
	}

	template<>
	inline std::string to_string<char>(char value){
		std::string temp;

		temp += value;

		return temp;
	}

	//to_value

	template<>
	inline bool to_value<bool>(const std::string& value){
		std::string lower = to_lower(value);

		if(lower == "true")
			return true;

		if(lower == "false")
			return false;

		if(lower == "0")
			return false;

		return true;
	}

	template<>
	inline int to_value<int>(const std::string& value){
		return std::stoi(value);
	}

	template<>
	inline unsigned int to_value<unsigned int>(const std::string& value){
		return std::stoul(value);
	}

	template<>
	inline long to_value<long>(const std::string& value){
		return std::stol(value);
	}

	template<>
	inline unsigned long to_value<unsigned long>(const std::string& value){
		return std::stol(value);
	}

	template<>
	inline long long to_value<long long>(const std::string& value){
		return std::stoll(value);
	}

	template<>
	inline unsigned long long to_value<unsigned long long>(const std::string& value){
		return std::stoull(value);
	}

	template<>
	inline float to_value<float>(const std::string& value){
		return std::stof(value);
	}

	template<>
	inline double to_value<double>(const std::string& value){
		return std::stod(value);
	}

	template<>
	inline long double to_value<long double>(const std::string& value){
		return std::stold(value);
	}

	template<>
	inline char to_value<char>(const std::string& value){
		return value[0];
	}
}