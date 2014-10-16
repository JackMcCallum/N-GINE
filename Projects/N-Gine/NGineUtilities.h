#ifndef __NGINE_UTILITIES_H_
#define __NGINE_UTILITIES_H_

#include "NGineCommon.h"
#include <string>

namespace NGine
{
	/*
	@author


	@description

	*/
	class Utilities
	{
	public:
		// Puts a external file into a string
		static bool fileToString(const std::string& fname, std::string& output);

		// Split a path file into multiple strings
		static void splitPath(std::string path, std::string* dir, std::string* fname, std::string* ext);

		// Convert a hex color "RRGGBB" to rgb 0-1. If str legnth != 6, it will fail
		static bool hexColorString(std::string str, float& red, float &green, float &blue);

		// Generate a hash value using ELFHash
		static uint32 hash(const std::string& key);

		// Show a simple message
		static void showMessage(const std::string& message);
		static void showMessage(const std::string& title, const std::string& message);

		// Copy memory into specified layout
		// Be very careful using this function
		// src must be a memory location that contains space enough for sizeof(T)*count*stride 
		// and stride must be greater than or equal to sizeof(T)
		template<typename T>
		static void memcpy(T* src, void* dest, size_t count, size_t stride, size_t offset)
		{
			assert(src);
			assert(dest);

			for (uint32 i = 0; i < count; i++)
			{
				T* destc = (T*)((size_t)dest + (i * stride + offset));
				*destc = src[i];
			}
		}

		// Hash a type
		template<typename T>
		static size_t hashType()
		{
			return typeid(T).hash_code();
		}
	};
};

#endif