#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include <ctime>
#include <string>
#include <cstddef>
#include <type_traits>
#include <string_view>

namespace util{
	template<typename T, std::size_t size>
	constexpr std::size_t array_size(T(&)[size]){ return size; }

	class NonCopyable{
	public:
		NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
	};

	/*
	*	Wrapper around an stl container that doesn't allow adding/removing elements
	*	but modifying existing ones using iterators is fine.
	*/
	template<typename ContainerType>
	class TempContainer{
	public:
		TempContainer() : container(nullptr){}
		TempContainer(ContainerType& container) : container(&container){}

		std::size_t size() const noexcept{ return container->size(); }
		typename ContainerType::iterator begin() noexcept{ return container->begin(); }
		typename ContainerType::iterator end() noexcept{ return container->end(); }
		const ContainerType* operator->() const noexcept{ return container; }

	private:
		ContainerType* container;
	};

	/*
	*	Code for loading dynamic libraries and getting the
	*	addresses of their exported symbols
	*/
#ifdef _WIN32
	inline void* load_library(std::string_view name) noexcept{
		return reinterpret_cast<void*>(LoadLibraryA(name.data()));
	}

	inline void free_library(void* ptr) noexcept{
		FreeLibrary(static_cast<HMODULE>(ptr));
	}

	inline void* get_proc_address(void* module, std::string_view name) noexcept{
		return reinterpret_cast<void*>(GetProcAddress(static_cast<HMODULE>(module), name.data()));
	}
#else
	inline void* load_library(std::string_view name) noexcept{
		return dlopen(name.data(), RTLD_NOW);
	}

	inline void free_library(void* ptr) noexcept{
		dlclose(ptr);
	}

	inline void* get_proc_address(void* module, std::string_view name) noexcept{
		return dlsym(module, name.data());
	}
#endif

	/*
	*	Wrapper for std::strftime always using the current time
	*	Formatting options are directly passed down and thus exactly the same
	*/
	inline std::string timestamp(std::string_view fmt){
		char buffer[128];
		std::time_t rawTime = std::time(nullptr);
		std::tm* timeInfo = std::localtime(&rawTime);

		std::strftime(buffer, sizeof(buffer), fmt.data(), timeInfo);

		return buffer;
	}
}