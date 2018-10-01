// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <vector>
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <map>

#include <sstream>
#include <xnamath.h>
#include <fstream>

#include <d3dx9.h>
#include <d3d9.h>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

using namespace std;

#include "MainGame/Path.h"

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "Framework\FrameworkHeaders.h"
#include "MainGame\ObjectManager.h"
// ���� ����

extern POINT ptMouse;
extern int Wheel;
extern D3DXVECTOR3* g_vPlayerPos;
extern HINSTANCE hInst;
extern HWND hWnd;
extern D3DXVECTOR3 g_vCamera;
extern bool isOptionMode;