#include "stdafx.h"
#include "InputManager.h"


CInputManager::CInputManager()
{
}


CInputManager::~CInputManager()
{
}

void CInputManager::Init()
{
	for (int i = 0; i < KEY_MAX; ++i)
	{
		GetKeyUp().set(i, false);
		GetKeyDown().set(i, false);
	}
}

bool CInputManager::IsOnceKeyDown(int key)
{
	// GetAsyncKeyState(key) & 0x8000 : ������ �������� ���� ȣ�� �� �������� ���� ���¸� TRUE
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!GetKeyDown()[key])
		{
			SetKeyDown(key, true);
			return true;
		}
	}
	else
	{
		SetKeyDown(key, false);
	}
	return false;
}

// Ű�� �����ٰ� ��������
bool CInputManager::IsOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
	{
		SetKeyUp(key, true);
	}
	else
	{
		if (GetKeyUp()[key])
		{
			SetKeyUp(key, false);
			return true;
		}
	}
	return false;
}


bool CInputManager::IsStayKeyDown(int key)
{
	if (GetAsyncKeyState(key) & 0x8000)
		return true;
	return false;
}
