#pragma once

#define FONT cFontManager::GetInstance()

class cFontManager
{
public:
	enum eFontType // UI 의 위치나 형태에 따라 사용하는 폰트가 다름
	{
		FT_GA_BIG = 1,
		FT_GA_MID,
		FT_GA_SMALL,
		FT_DEFAULT,
		FT_QUEST,
		TF_UI_TEXT,
		TF_UI_NUMBER,
		TF_UI_NUMBER2,
		TF_MAIN

	};

	SINGLETONE(cFontManager);

	map<eFontType, LPD3DXFONT> m_mapFont;

public:
	// 만들거나 만들어져 있으면 리턴을 (타입에 따라 )
	LPD3DXFONT GetFont(eFontType type);
	void Destroy();
};

// 사용법

/*
char szTemp[1024];
RECT rc;
SetRect(&rc,100,100,200,100);
LPD#DXFONT pFont = g_pFONT->GetFont(cFONT::FT_DEFAULT);
pFont->DrawText(NULL,szTemp,strlen(szTemp),&rc,
DT_LEFT| DT_CENTER| DT_NOCLIP,
D3DCOLOR_XRGB(255,0,255));

*/