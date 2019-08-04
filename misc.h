#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#include <stdlib.h>
#endif

#include <ctime>
#include <memory>
#include <string>
#include <cstddef>
#include <type_traits>
#include <string_view>

namespace util{
	template<typename T, std::size_t size>
	constexpr std::size_t array_size(T(&)[size]) noexcept{ return size; }

	template<typename S, typename T>
	std::uintptr_t offset_of(T S::*member){
		static_assert(std::is_standard_layout<S>{});

		return reinterpret_cast<std::uintptr_t>(std::addressof(reinterpret_cast<S*>(NULL)->*member));
	}

	namespace detail{
		struct No{};

		template<typename T, typename Arg>
		No operator==(const T&, const Arg&);

		template<typename T, typename Arg = T>
		struct HasEqualsOperatorImpl{
			enum{ value = !std::is_same<decltype(*(T*)(0) == *(Arg*)(0)), No>::value };

			constexpr operator bool(){ return static_cast<bool>(value); }
		};
	}

	template<typename T>
	using HasEqualsOperator = detail::HasEqualsOperatorImpl<T>;

#ifdef _WIN32
	inline void* load_library(std::string_view name) noexcept{ return reinterpret_cast<void*>(LoadLibraryA(name.data())); }
	inline void free_library(void* ptr) noexcept{ FreeLibrary(static_cast<HMODULE>(ptr)); }
	inline void* get_proc_address(void* module, std::string_view name) noexcept{ return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(module), name.data())); }

	inline std::string absolute_path(std::string_view path){
		char buffer[512];

		GetFullPathNameA(path.empty() ? "." : path.data(), static_cast<DWORD>(array_size(buffer)), buffer, nullptr);

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

		std::strftime(buffer, array_size(buffer), fmt.data(), timeInfo);

		return buffer;
	}
}
