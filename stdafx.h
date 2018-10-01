// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "Framework\FrameworkHeaders.h"
#include "MainGame\ObjectManager.h"
// 전역 변수

extern POINT ptMouse;
extern int Wheel;
extern D3DXVECTOR3* g_vPlayerPos;
extern HINSTANCE hInst;
extern HWND hWnd;
extern D3DXVECTOR3 g_vCamera;
extern bool isOptionMode;