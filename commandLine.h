#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>
#include "stringUtil.h"

namespace util{
	class CommandLine{
	public:
		CommandLine() = default;
		CommandLine(int argc, const char* const* const argv){ init({argv, argv + argc}); }
		CommandLine(std::string_view cmd){ init(util::str::split(cmd)); }

		bool has_option(const std::string& arg) const{ return optionValues.find(arg) != optionValues.end(); }
		const std::vector<std::string>& argv() const{ return argvec; }
		const std::vector<std::string>& values_without_option() const{ return values; }

		std::string value_for_option(const std::string& option) const{
			if(has_option(option))
				return optionValues.find(option)->second;

			return "";
		}

		std::string str() const{
			std::string result;

			for(const std::string& s : argvec)
				result += s + " ";

			if(!result.empty())
				result.pop_back(); //Removing trailing whitespace

			return result;
		}

	private:
		std::vector<std::string> argvec;
		std::vector<std::string> values; //Values that don't belong to an option
		std::unordered_map<std::string, std::string> optionValues; //Options with their respective parameter values

		void init(const std::vector<std::string>& argv){
			const char* currentOption = nullptr;

			for(const std::string& s : argv){
				argvec.push_back(s);

				if(s.size() > 1 && s[0] == '-'){
					currentOption = s.c_str() + 1;

					optionValues.insert(std::pair<std::string, std::string>{currentOption, ""});
				}else if(currentOption){
					optionValues[currentOption] = s;
					currentOption = nullptr;
				}else{
					values.push_back(s);
				}
			}
		}
	};
}