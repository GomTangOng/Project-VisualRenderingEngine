#pragma once
#include "stdafx.h"
#include <comdef.h>
#include <fstream>
#include "DXException.h"




inline std::wstring AnsiToWString(const std::string& str)
{
	WCHAR buffer[512];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buffer, 512);
	return std::wstring(buffer);
}

#ifndef HR
#define HR(x)														  \
{                                                                     \
    HRESULT hr__ = (x);                                               \
    std::wstring wfn = AnsiToWString(__FILE__);                       \
    if(FAILED(hr__)) { throw CDXException(hr__, L#x, wfn, __LINE__); } \
}
#endif

namespace Memory
{
	template <typename T>
	inline void Release(T *t)
	{
		if(t) t->Release();
	}

	template <typename  T>
	inline void Delete(T* t)
	{
		delete t;
	}

	template <typename T>
	inline void DeleteArray(T* t)
	{
		delete[] t;
	}
	
	template <typename T>
	inline void DeleteVector(T& vec)
	{
		for (auto& t : vec)
		{
			Delete(t);
		}
		vec.clear();
	}
}