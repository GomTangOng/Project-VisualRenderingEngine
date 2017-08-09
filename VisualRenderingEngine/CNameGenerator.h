#pragma once
#include "stdafx.h"
#include "Singleton.h"

#define NAME_GENERATOR CNameGenerator::GetInstance()

class CNameGenerator : public Singleton<CNameGenerator>
{
public :
	CNameGenerator() : m_prefix("NULL"){ }
	~CNameGenerator() { m_prefix.clear();}

	string Generate()
	{
		stringstream s;
		s << m_prefix << m_index++;
		return s.str();
	}

	void Reset()
	{
		m_index = 1;
	}

	void SetNextIndex(const UINT val)
	{
		m_index = val;
	}

	UINT GetNextIndex() { return m_index; }
private :
	string m_prefix;
	UINT m_index;
};
