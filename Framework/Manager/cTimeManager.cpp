#include "stdafx.h"
#include "cTimeManager.h"


cTimeManager::cTimeManager()
	: m_fEllapsedTime(0.0f)
{
	m_dwLastUpdateTime = GetTickCount();
}


cTimeManager::~cTimeManager()
{
}

void cTimeManager::Update()
{
	DWORD dwCurrentTime = GetTickCount();
	m_fEllapsedTime = (dwCurrentTime - m_dwLastUpdateTime) / 1000.0f;
	m_dwLastUpdateTime = dwCurrentTime;

	m_fFrameTime += m_fEllapsedTime;
	m_nFrameCount++;
	while (m_fFrameTime > 1.0f)
	{
		m_fFrameTime -= 1.0f;
		m_nFPS = m_nFrameCount;
		m_nFrameCount = 0;
	}
}

float cTimeManager::GetEllapsedTime()
{
	return m_fEllapsedTime;
}

float cTimeManager::GetLastUpdateTime()
{
	return m_dwLastUpdateTime / 1000.0f;
}

void cTimeManager::Delete()
{
	//if(this)
	//SAFE_RELEASE(instance);
}

void cTimeManager::Render()
{
	char szTemp[1024];
	sprintf_s(szTemp, 1024, "FPS : %d", m_nFPS);
	RECT rc;
	SetRect(&rc, WINSIZEX - 200, 0, WINSIZEX, 100);

	LPD3DXFONT pFont = FONT->GetFont(cFontManager::FT_DEFAULT);
	pFont->DrawTextA(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_VCENTER,
		D3DCOLOR_XRGB(255, 0, 0));
}
