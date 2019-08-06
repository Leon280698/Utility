#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <stdlib.h>
#endif

#include <ctime>
#include <cctype>
#include <memory>
#include <string>
#include <cstddef>
#include <iterator>
#include <typeinfo>
#include <type_traits>
#include <string_view>

#if defined(__clang__)  || defined(__GNUC__)
#include <cxxabi.h>
#endif // __clang__ || __GNUC__

namespace util{
	template<typename T>
	std::string type_name(){
#ifdef _MSC_VER
		auto name = typeid(T).name();
		auto start = name + std::strlen(name);

		while(start != name && !std::isspace(*(start - 1))) // Removing 'struct', 'enum' or 'class' prefix
			--start;

		return start;

#elif defined(__clang__)  || defined(__GNUC__)
		auto name = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, nullptr);

		std::string result{name};

		free(name);

		return result;
#else
	#error Compiler needs implementation for type_name
#endif // _MSC_VER
	}

	template<typename S, typename T>
	std::uintptr_t offset_of(T S::*member){
		static_assert(std::is_standard_layout<S>{});

		return reinterpret_cast<std::uintptr_t>(std::addressof(reinterpret_cast<S*>(NULL)->*member));
	}

	template<typename T, typename EqualTo = T>
	struct HasEqualsOperator{
		template<typename U, typename V>
		static auto test(U*)->decltype(std::declval<U>() == std::declval<V>());
		template<typename, typename>
		static auto test(...)->std::false_type;

		constexpr operator bool() const noexcept{ return std::is_same<bool, decltype(test<T, EqualTo>(0))>{}; }
	};

#ifdef _WIN32
	inline void* load_library(std::string_view name) noexcept{ return reinterpret_cast<void*>(LoadLibraryA(name.data())); }
	inline void free_library(void* ptr) noexcept{ FreeLibrary(static_cast<HMODULE>(ptr)); }
	inline void* get_proc_address(void* module, std::string_view name) noexcept{ return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(module), name.data())); }

	inline std::string absolute_path(std::string_view path){
		char buffer[512];

		GetFullPathNameA(path.empty() ? "." : path.data(), static_cast<DWORD>(std::size(buffer)), buffer, nullptr);

		return buffer;
	}
#else //Only assuming either windows or unix for now...
	inline void* load_library(std::string_view name) noexcept{ return dlopen(name.data(), RTLD_NOW); }
	inline void free_library(void* ptr) noexcept{ dlclose(ptr); }
	inline void* get_proc_address(void* module, std::string_view name) noexcept{ return dlsym(module, name.data()); }

	inline std::string absolute_path(std::string_view path){
		char buffer[512];

		realpath(path.empty() ? "." : path.data(), buffer);

		return buffer;
	}
#endif

	/*
	*	Wrapper around std::strftime always using the current time
	*	Formatting options are directly passed down and thus exactly the same
	*/
	inline std::string timestamp(std::string_view fmt){
		char buffer[128];
		std::time_t rawTime = std::time(nullptr);
		std::tm* timeInfo = std::localtime(&rawTime);

		std::strftime(buffer, std::size(buffer), fmt.data(), timeInfo);

		return buffer;
	}
}
