#pragma once

template <typename T>
class Singleton
{
public :
	static T* GetInstance()
	{
		static T* m_instance = new T();
		return m_instance;
	}	
};