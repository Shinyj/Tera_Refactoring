#pragma once

#define MAX_INPUT_KEY 255
#define MAX_INPUT_MOUSE 8

class cKeyManager
{
private:
	cKeyManager();
	~cKeyManager();

	static cKeyManager* instance;

	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];

	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS
	};
public:
	static cKeyManager* Get();

	static void Create();
	static void Delete();

	void Update();

	bool IsOnceKeyDown(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_DOWN; }
	bool IsOnceKeyUp(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_UP; }
	bool IsStayKeyDown(DWORD key) { return keyMap[key] == KEY_INPUT_STATUS_PRESS; }

};

#define  KEY cKeyManager::Get()

