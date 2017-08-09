#pragma once

#define INPUT_MANAGER CInputManager::GetInstance()

class CInputManager : public Singleton<CInputManager>
{
public:
	CInputManager();
	~CInputManager();

	void Init();

	bool IsOnceKeyDown(int key);
	bool IsOnceKeyUp(int key);
	bool IsStayKeyDown(int key);
	//bool IsToggleKey(int key);
private:
	bitset<KEY_MAX> m_keyUp;
	bitset<KEY_MAX> m_keyDown;

public :
	void SetKeyDown(int key, bool state) { m_keyDown.set(key, state); }
	void SetKeyUp(int key, bool state) { m_keyUp.set(key, state); }

	bitset<KEY_MAX> GetKeyUp() { return m_keyUp; }
	bitset<KEY_MAX> GetKeyDown() { return m_keyDown; }
};

