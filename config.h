#pragma once

#include <map>
#include <cctype>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "misc.h"
#include "stringUtil.h"

namespace util{
	class Config{
	public:
		Config() = default;

		Config(const std::string& fileName){
			load_from_file(fileName, true);
		}

		bool load_from_file(const std::string& fileName, bool clearCache){
			if(clearCache)
				clear();

			if(std::ifstream in{fileName}){
				std::string currentLine, currentSection;

				while(std::getline(in, currentLine)){
					if(!currentLine.empty() && currentLine[0] != ';'){
						if(currentLine[0] == '[' && currentLine.back() == ']'){
							currentSection = currentLine.substr(1, currentLine.size() - 2);
						}else if(!currentSection.empty()){
							std::istringstream iss{currentLine};
							std::string key, value;

							std::getline(iss, key, '=');

							if(!key.empty() && key.size() + 1 < currentLine.size())
								value = currentLine.substr(key.size() + 1);

							set(currentSection, key, value);
						}
					}
				}

				return true;
			}

			return false;
		}

		bool save_to_file(const std::string& fileName) const{
			if(data.empty())
				return true; //Nothing needs to be saved, just returning true

			if(std::ifstream in{fileName}){
				std::string currentLine;
				std::vector<std::string> fileContents;
				ConfigData dataCopy{data};
				ConfigSection* currentSection = nullptr;

				//Saving file but preserving already existing one (except for conflicts)
				while(std::getline(in, currentLine)){
					if(currentLine.empty()){
						fileContents.push_back("");
					}else if(currentLine[0] == '[' && currentLine.back() == ']'){
						if(currentSection){
							if(fileContents.back().empty())
								fileContents.pop_back();

							for(const auto& it : *currentSection)
								fileContents.push_back(it.first + "=" + it.second);

							currentSection->clear();

							fileContents.push_back("");
						}

						currentSection = &dataCopy[currentLine.substr(1, currentLine.size() - 2)];
						fileContents.push_back(currentLine);
					}else if(currentSection){
						std::istringstream iss{currentLine};
						std::string key, value;

						std::getline(iss, key, '=');

						if(!key.empty() && key.size() + 1 < currentLine.size())
							value = currentLine.substr(key.size() + 1);

						std::string validKey{key};

						//Removing whitespaces from key
						validKey.erase(std::remove_if(validKey.begin(), validKey.end(), [](char c){
																							return std::isspace(static_cast<unsigned char>(c));
																						}),
									   validKey.end());

						if(validKey[0] == ';'){ //Line is a comment
							fileContents.push_back(currentLine);
						}else{
							auto it = currentSection->find(validKey);

							if(it != currentSection->end()){
								fileContents.push_back(it->first + "=" + it->second);
								currentSection->erase(it);
							}else{
								fileContents.push_back(validKey + "=" + value);
							}
						}
					}else{
						if(currentLine[0] == ';')
							fileContents.push_back(currentLine);
					}
				}

				for(const auto& it : *currentSection)
					fileContents.push_back(it.first + "=" + it.second);

				currentSection->clear();

				in.close();

				if(std::ofstream out{fileName}){
					for(const std::string& line : fileContents)
						out << line << '\n';

					for(const auto& section : dataCopy){
						if(!section.second.empty()){
							out << "\n[" << section.first << "]\n";

							for(const auto& keyValuePair : section.second)
								out << keyValuePair.first << '=' << keyValuePair.second << '\n';
						}
					}

					return true;
				}

				return false;
			}else{
				std::ofstream out{fileName};

				dump(out); //If file doesn't exist yet we can just print it without checking already existing values

				return static_cast<bool>(out);
			}
		}

		void clear(){
			data.clear();
		}

		void dump(std::ostream& out) const{
			for(const auto& section : data){
				out << '[' << section.first << "]\n";

				for(const auto& keyValuePair : section.second)
					out << keyValuePair.first << '=' << keyValuePair.second << '\n';

				out << '\n';
			}
		}

		std::string get(const std::string& section, const std::string& key, const std::string& defaultValue){
			std::string validKey{key};

			//Removing whitespaces from key
			validKey.erase(std::remove_if(validKey.begin(), validKey.end(), [](char c){
																				return std::isspace(static_cast<unsigned char>(c));
																			}),
						   validKey.end());

			auto configSection = data.find(section);

			if(configSection != data.end()){

				auto keyValuePair = configSection->second.find(validKey);

				if(keyValuePair != configSection->second.end())
					return keyValuePair->second;
			}

			set(section, validKey, defaultValue);

			return defaultValue;
		}

		std::string get(const std::string& section, const std::string& key, const char* defaultValue){
			return get(section, key, std::string{defaultValue});
		}

		void set(const std::string& section, const std::string& key, const std::string& value){
			std::string validKey{key};

			//Removing whitespaces from key
			validKey.erase(std::remove_if(validKey.begin(), validKey.end(), [](char c){
																				return std::isspace(static_cast<unsigned char>(c));
																			}),
						   validKey.end());

			data[section][validKey] = value;
		}

		void set(const std::string& section, const std::string& key, const char* value){
			set(section, key, std::string{value});
		}

		template<typename T>
		T get(const std::string& section, const std::string& key, T defaultValue){
			std::string value{get(section, key, str::to_string(defaultValue))};
			T result;

			try{ //Conversion functions like std::stoi might throw an exception, in that case the default value is returned
				result = str::to_value<T>(value);
			}catch(...){
				result = defaultValue;
			}

			return result;
		}

		template<typename T>
		void set(const std::string& section, const std::string& key, T value){
			set(section, key, str::to_string(value));
		}

	private:
		using ConfigSection = std::map<std::string, std::string, str::CaseInsensitiveLess>;
		using ConfigData = std::map<std::string, ConfigSection, str::CaseInsensitiveLess>;

		ConfigData data;
	};

	//Making sure the right overload is being used for std::string
	template<>
	inline std::string Config::get(const std::string& section, const std::string& key, std::string defaultValue){
		return get(section, key, defaultValue);
	}
}