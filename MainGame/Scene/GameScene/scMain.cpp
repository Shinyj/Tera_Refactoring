#include "stdafx.h"
#include "scMain.h"


scMain::scMain() 
	: m_pBackGround(NULL)
	, m_pBackGroundBlack(NULL)
	, m_pLoadingBar(NULL)
	, m_nBGBlackAlpha(255)
	, m_isStart(false)
{
	m_colorOver = D3DCOLOR_ARGB(255, 255, 255, 0);
	m_colorDefault = D3DCOLOR_ARGB(255, 0, 0, 0);
}


scMain::~scMain()
{
	SAFE_DELETE(m_pBackGround);
	SAFE_DELETE(m_pBackGroundBlack);
	SAFE_DELETE(m_pBackGroundWhite);
}

void scMain::Setup()
{
	m_pBackGround = TEXTURE->GetSprite(SceneTexPath + L"Loadingimage2.png"); // Loadingimage2 LoadingImage
	m_pBackGroundBlack = TEXTURE->GetSprite(SceneTexPath + L"BGBlack.png");
	m_pBackGroundWhite = TEXTURE->GetSprite(SceneTexPath + L"BGWhite.png");

	rc[0] = RectMakeCenter(WINSIZEX / 2, (float)WINSIZEY / 2.0f, 400, 50);
	rc[1] = RectMakeCenter(WINSIZEX / 2, (float)WINSIZEY / 2.0f + 100, 400, 50);
	rc[2] = RectMakeCenter(WINSIZEX / 2, (float)WINSIZEY / 2.0f + 200, 400, 50);
	rc[3] = RectMakeCenter(WINSIZEX / 2, (float)WINSIZEY / 2.0f + 300, 400, 50);
	rc[4] = RectMakeCenter(WINSIZEX / 2, (float)WINSIZEY / 2.0f - 200, 400, 400);

	m_nBGBlackAlpha = 255;
	m_isStart = false;
	m_colorOver = D3DCOLOR_ARGB(255, 255, 255, 0);
	m_colorDefault = D3DCOLOR_ARGB(255, 0, 0, 0);

	// 메인 씬 사운드 등록
	SOUND->AddSound("Main", MainSoundPath + L"ServerTheme.ogg", true, true);
	SOUND->Play("Main");

	SOUND->AddSound("MouseMove_SelectChange", InterfaceSoundPath + L"MouseMove_SelectChange.mp3");
	SOUND->AddSound("Click_buttom", InterfaceSoundPath + L"Click_buttom.mp3");
	SOUND->AddSound("Click_ItemPickUp", InterfaceSoundPath + L"Click_ItemPickUp.mp3");
	SOUND->AddSound("Click_NewClick", InterfaceSoundPath + L"Click_NewClick.mp3");

}

void scMain::Release()
{
	SAFE_DELETE(m_pBackGround);
	SAFE_DELETE(m_pBackGroundBlack);
	SAFE_DELETE(m_pBackGroundWhite);
}

void scMain::Update()
{
	if (m_isStart == false && m_nBGBlackAlpha > 0)
		m_nBGBlackAlpha -= 5;

	if (KEY->IsOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < 4; i++)
		{
			if (PtInRect(&rc[0], ptMouse))
			{
				SOUND->Play("Click_buttom");
				m_isStart = true;
				m_sNextScene = "LobbyLoading";
			}
			else if (PtInRect(&rc[1], ptMouse))
			{
				SOUND->Play("Click_buttom");
				m_isStart = true;
				m_sNextScene = "PaticleEdit";
			}
			else if (PtInRect(&rc[2], ptMouse))
			{
				SOUND->Play("Click_buttom");
			}
			else if (PtInRect(&rc[3], ptMouse))
			{
				SOUND->Play("Click_buttom");
				DestroyWindow(hWnd);
			}

		}
	}
	if (m_isStart)
	{
		m_nBGBlackAlpha += 5;

		if (m_nBGBlackAlpha > 255)
		{
			SOUND->Play("Main");
			SOUND->Pause("Main");
			SOUND->Stop("Main");
			SCENE->ChangeScene(m_sNextScene.c_str());
		}
	}
}

void scMain::Render()
{
	m_pBackGround->Render(D3DXVECTOR3(m_pBackGround->textureInfo.Width / 2, 0, 0), D3DXVECTOR3(WINSIZEX / 2, 0, 0));

	m_pBackGroundWhite->AlphaRender(rc[4],
		D3DXVECTOR3(0, 0, 0),
		D3DXVECTOR3(rc[4].left, rc[4].top + 350, 0), 80);

	char szTemp[1024];
	LPD3DXFONT pFont;
	pFont = FONT->GetFont(cFontManager::TF_MAIN);

	sprintf_s(szTemp, 1024, "GAME  START");
	if (PtInRect(&rc[0], ptMouse))
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[0], DT_CENTER | DT_VCENTER, m_colorOver);
	else
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[0], DT_CENTER | DT_VCENTER, m_colorDefault);

	sprintf_s(szTemp, 1024, "PARTICLE  EDIT");
	if (PtInRect(&rc[1], ptMouse))
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[1], DT_CENTER | DT_VCENTER, m_colorOver);
	else
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[1], DT_CENTER | DT_VCENTER, m_colorDefault);

	sprintf_s(szTemp, 1024, "OPTION");
	if (PtInRect(&rc[2], ptMouse))
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[2], DT_CENTER | DT_VCENTER, m_colorOver);
	else
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[2], DT_CENTER | DT_VCENTER, m_colorDefault);

	sprintf_s(szTemp, 1024, "EXIT");
	if (PtInRect(&rc[3], ptMouse))
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[3], DT_CENTER | DT_VCENTER, m_colorOver);
	else
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[3], DT_CENTER | DT_VCENTER, m_colorDefault);

	m_pBackGroundBlack->AlphaRender(D3DXVECTOR3(m_pBackGround->textureInfo.Width / 2, 0, 0), D3DXVECTOR3(WINSIZEX / 2, 0, 0), m_nBGBlackAlpha);

}
