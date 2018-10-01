#include "stdafx.h"
#include "ParticleEdit.h"


ParticleEdit::ParticleEdit()
	: m_pGrid(NULL)
{
	m_type = PTC_TYPE_LOOP;
	m_fTime = 0.0f;
	m_fSpeed = 0.0f;
	m_fAcc = 0.0f;
	m_fAccMin = 0;
	m_fAccMax = 0;

	m_fPositionX = 0;
	m_fRandPosXMin = 0;
	m_fRandPosXMax = 0;

	m_fPositionY = 0;
	m_fRandPosYMin = 0;
	m_fRandPosYMax = 0;

	m_fPositionZ = 0;
	m_fRandPosZMin = 0;
	m_fRandPosZMax = 0;

	m_fDirectionX = 0;
	m_fRandDirXMin = 0;
	m_fRandDirXMax = 0;

	m_fDirectionY = 0;
	m_fRandDirYMin = 0;
	m_fRandDirYMax = 0;

	m_fDirectionZ = 0;
	m_fRandDirZMin = 0;
	m_fRandDirZMax = 0;

	m_szFile[1024];
	char * asd = "none";
	strcpy(m_szFile, asd);

	m_nAlpha = 255;
	m_nColorR = 255;
	m_nColorG = 255;
	m_nColorB = 255;

	isOptionMode = true;
}


ParticleEdit::~ParticleEdit()
{
	SAFE_DELETE(m_pParticleSet);
	//SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pMainImamge);
	SAFE_DELETE(m_pGrid);
}

void ParticleEdit::Setup()
{
	CAMERA->SetType(CAMERA_FIX);
	//m_pSprite = TEXTUREMANAGER->GetSprite("Texture/SceneTexture/BGBlack.png");

	m_pParticleSet = new cParticleSet;

	UISetup();

	m_pGrid = new cGrid;
	m_pGrid->Setup();
}

void ParticleEdit::Release()
{
	SAFE_DELETE(m_pParticleSet);
	//SAFE_DELETE(m_pSprite);
	SAFE_DELETE(m_pMainImamge);
}

void ParticleEdit::Update()
{
	m_pParticleSet->Update();
	UIUpdate();

	// 다시 메인으로 탈출!
	if (KEY->IsOnceKeyDown(VK_ESCAPE))
	{
		//TEXTUREMANAGER->Destroy();
		SCENE->ChangeScene("Main");
		
	}
}

void ParticleEdit::Render()
{
	m_pGrid->Render();
	if (!KEY->IsStayKeyDown(VK_LBUTTON))
	{
		m_pParticleSet->Render();
	}

	UIRender();
}

void ParticleEdit::UISetup()
{
	//m_pMainImamge = TEXTURE->GetSprite("Texture/UI/ParticleEdit/particleEdit_main.png");
	m_pMainImamge = TEXTURE->GetSprite(UiTexPath + L"ParticleEdit/particleEdit_main.png");
	x = 0;
	y = 0;

	// \ui 노가다...

	rc[0] = RectMake(0, 0, 300, 15);
	rc[1] = RectMake(95, 35, 50, 15);
	rc[2] = RectMake(95, 60, 105, 15);
	rc[3] = RectMake(95, 105, 50, 15);

	rc[4] = RectMake(95, 125, 50, 15);
	rc[5] = RectMake(155, 125, 50, 15);
	rc[6] = RectMake(220, 125, 50, 15);

	rc[7] = RectMake(95, 175, 50, 15);
	rc[8] = RectMake(155, 175, 50, 15);
	rc[9] = RectMake(220, 175, 50, 15);

	rc[10] = RectMake(95, 200, 50, 15);
	rc[11] = RectMake(155, 200, 50, 15);
	rc[12] = RectMake(220, 200, 50, 15);

	rc[13] = RectMake(95, 225, 50, 15);
	rc[14] = RectMake(155, 225, 50, 15);
	rc[15] = RectMake(220, 225, 50, 15);

	rc[16] = RectMake(95, 265, 50, 15);
	rc[17] = RectMake(155, 265, 50, 15);
	rc[18] = RectMake(220, 265, 50, 15);

	rc[19] = RectMake(95, 290, 50, 15);
	rc[20] = RectMake(155, 290, 50, 15);
	rc[21] = RectMake(220, 290, 50, 15);

	rc[22] = RectMake(95, 315, 50, 15);
	rc[23] = RectMake(155, 315, 50, 15);
	rc[24] = RectMake(220, 315, 50, 15);

	rc[25] = RectMake(93, 360, 178, 15);

	// ARGB
	rc[26] = RectMake(105, 412, 40, 17);
	rc[27] = RectMake(155, 412, 40, 17);
	rc[28] = RectMake(205, 412, 40, 17);
	rc[29] = RectMake(255, 412, 40, 17);
	// play save Load
	rc[30] = RectMake(10, 500, 90, 20);
	rc[31] = RectMake(110, 500, 90, 20);
	rc[32] = RectMake(205, 500, 90, 20);
}

void ParticleEdit::UIUpdate()
{
	for (int i = 0; i < BTNMAX; i++)
	{
		if (PtInRect(&rc[i], ptMouse))
		{
			if (KEY->IsStayKeyDown(VK_CONTROL))
			{
				if (KEY->IsOnceKeyDown(VK_LBUTTON))
				{
					switch (i)
					{
					case  1:	m_fTime += 0.1f;	break;
					case 2:
						if (m_type == PTC_TYPE_LOOP)
							m_type = PTC_TYPE_ONCE;
						else
							m_type = PTC_TYPE_LOOP;
						break;
					case  3:	m_fSpeed += 0.1f;	break;
					case  4:	m_fAcc += 0.001f;	break;
					case  5:	m_fAccMin++;		break;
					case  6:	m_fAccMax++;		break;
					case  7:	m_fPositionX++;		break;
					case  8:	m_fRandPosXMin++;	break;
					case  9:	m_fRandPosXMax++;	break;
					case 10:	m_fPositionY++;		break;
					case 11:	m_fRandPosYMin++;	break;
					case 12:	m_fRandPosYMax++;	break;
					case 13:	m_fPositionZ++;		break;
					case 14:	m_fRandPosZMin++;	break;
					case 15:	m_fRandPosZMax++;	break;
					case 16:	m_fDirectionX++;	break;
					case 17:	m_fRandDirXMin++;	break;
					case 18:	m_fRandDirXMax++;	break;
					case 19:	m_fDirectionY++;	break;
					case 20:	m_fRandDirYMin++;	break;
					case 21:	m_fRandDirYMax++;	break;
					case 22:	m_fDirectionZ++;	break;
					case 23:	m_fRandDirZMin++;	break;
					case 24:	m_fRandDirZMax++;	break;
					case 25:	LoadTexture();		break;
					case 26:	m_nAlpha++;			break;
					case 27:	m_nColorR++;		break;
					case 28:	m_nColorG++;		break;
					case 29:	m_nColorB++;		break;
					case 30:	Play();				break;
					case 31:	SaveParticle();		break;
					case 32:	LoadParticle();		break;
					}
				}
				else if (KEY->IsOnceKeyDown(VK_RBUTTON))
				{
					switch (i)
					{
					case 2:
						if (m_type == PTC_TYPE_LOOP)
							m_type = PTC_TYPE_ONCE;
						else
							m_type = PTC_TYPE_LOOP;
						break;
					case  3:	m_fSpeed -= 0.1f;	break;
					case  4:	m_fAcc -= 0.001f;	break;
					case  5:	m_fAccMin--;		break;
					case  6:	m_fAccMax--;		break;
					case  7:	m_fPositionX--;		break;
					case  8:	m_fRandPosXMin--;	break;
					case  9:	m_fRandPosXMax--;	break;
					case 10:	m_fPositionY--;		break;
					case 11:	m_fRandPosYMin--;	break;
					case 12:	m_fRandPosYMax--;	break;
					case 13:	m_fPositionZ--;		break;
					case 14:	m_fRandPosZMin--;	break;
					case 15:	m_fRandPosZMax--;	break;
					case 16:	m_fDirectionX--;	break;
					case 17:	m_fRandDirXMin--;	break;
					case 18:	m_fRandDirXMax--;	break;
					case 19:	m_fDirectionY--;	break;
					case 20:	m_fRandDirYMin--;	break;
					case 21:	m_fRandDirYMax--;	break;
					case 22:	m_fDirectionZ--;	break;
					case 23:	m_fRandDirZMin--;	break;
					case 24:	m_fRandDirZMax--;	break;
					case 25:	LoadTexture();		break;
					case 26:	m_nAlpha--;			break;
					case 27:	m_nColorR--;		break;
					case 28:	m_nColorG--;		break;
					case 29:	m_nColorB--;		break;
					}
				}
			}
			else if (KEY->IsStayKeyDown(VK_LBUTTON))
			{
				switch (i)
				{
				case 0:

					break;

				case  1:	m_fTime += 0.1f;	break;
				case 2:
					if (m_type == PTC_TYPE_LOOP)
						m_type = PTC_TYPE_ONCE;
					else
						m_type = PTC_TYPE_LOOP;
					break;
				case  3:	m_fSpeed += 0.1f;	break;
				case  4:	m_fAcc += 0.001f;	break;
				case  5:	m_fAccMin++;		break;
				case  6:	m_fAccMax++;		break;
				case  7:	m_fPositionX++;		break;
				case  8:	m_fRandPosXMin++;	break;
				case  9:	m_fRandPosXMax++;	break;
				case 10:	m_fPositionY++;		break;
				case 11:	m_fRandPosYMin++;	break;
				case 12:	m_fRandPosYMax++;	break;
				case 13:	m_fPositionZ++;		break;
				case 14:	m_fRandPosZMin++;	break;
				case 15:	m_fRandPosZMax++;	break;
				case 16:	m_fDirectionX++;	break;
				case 17:	m_fRandDirXMin++;	break;
				case 18:	m_fRandDirXMax++;	break;
				case 19:	m_fDirectionY++;	break;
				case 20:	m_fRandDirYMin++;	break;
				case 21:	m_fRandDirYMax++;	break;
				case 22:	m_fDirectionZ++;	break;
				case 23:	m_fRandDirZMin++;	break;
				case 24:	m_fRandDirZMax++;	break;
				case 25:	LoadTexture();		break;
				case 26:	m_nAlpha++;			break;
				case 27:	m_nColorR++;		break;
				case 28:	m_nColorG++;		break;
				case 29:	m_nColorB++;		break;
				case 30:	Play();				break;
				case 31:	SaveParticle();		break;
				case 32:	LoadParticle();		break;
				}
			}
			else if (KEY->IsStayKeyDown(VK_RBUTTON))
			{
				switch (i)
				{
				case  1:	m_fTime -= 0.1f;	break;
				case 2:
					if (m_type == PTC_TYPE_LOOP)
						m_type = PTC_TYPE_ONCE;
					else
						m_type = PTC_TYPE_LOOP;
					break;
				case  3:	m_fSpeed -= 0.1f;	break;
				case  4:	m_fAcc -= 0.001f;	break;
				case  5:	m_fAccMin--;		break;
				case  6:	m_fAccMax--;		break;
				case  7:	m_fPositionX--;		break;
				case  8:	m_fRandPosXMin--;	break;
				case  9:	m_fRandPosXMax--;	break;
				case 10:	m_fPositionY--;		break;
				case 11:	m_fRandPosYMin--;	break;
				case 12:	m_fRandPosYMax--;	break;
				case 13:	m_fPositionZ--;		break;
				case 14:	m_fRandPosZMin--;	break;
				case 15:	m_fRandPosZMax--;	break;
				case 16:	m_fDirectionX--;	break;
				case 17:	m_fRandDirXMin--;	break;
				case 18:	m_fRandDirXMax--;	break;
				case 19:	m_fDirectionY--;	break;
				case 20:	m_fRandDirYMin--;	break;
				case 21:	m_fRandDirYMax--;	break;
				case 22:	m_fDirectionZ--;	break;
				case 23:	m_fRandDirZMin--;	break;
				case 24:	m_fRandDirZMax--;	break;
				case 25:	LoadTexture();		break;
				case 26:	m_nAlpha--;			break;
				case 27:	m_nColorR--;		break;
				case 28:	m_nColorG--;		break;
				case 29:	m_nColorB--;		break;
				}
			}
		}
	}

	//////////// UI 창 옮기기

	if (KEY->IsOnceKeyDown(VK_LBUTTON))
	{
		if (PtInRect(&rc[0], ptMouse))
		{
			if (KEY->IsOnceKeyDown(VK_LBUTTON))
			{
				beforeMouse = ptMouse;
				isDrag = true;
			}
		}
	}

	if (isDrag)
	{
		x += ptMouse.x - beforeMouse.x;
		y += ptMouse.y - beforeMouse.y;

		for (int i = 0; i < BTNMAX; i++)
		{
			rc[i].left += ptMouse.x - beforeMouse.x;
			rc[i].top += ptMouse.y - beforeMouse.y;
			rc[i].right += ptMouse.x - beforeMouse.x;
			rc[i].bottom += ptMouse.y - beforeMouse.y;
		}
		beforeMouse = ptMouse;
	}

	if (KEY->IsOnceKeyUp(VK_LBUTTON))
	{
		beforeMouse.x = 0;
		beforeMouse.y = 0;
		isDrag = false;
	}
}

void ParticleEdit::UIRender()
{
	m_pMainImamge->Render(x, y);

	char szTemp[1024];
	LPD3DXFONT pFont;
	pFont = FONT->GetFont(cFontManager::TF_UI_NUMBER2);

	sprintf_s(szTemp, 1024, "%s", m_sName);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[0], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%.1f", m_fTime);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[1], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	if (m_type == PTC_TYPE_LOOP)
		sprintf_s(szTemp, 1024, "Type_Loop");
	else if (m_type == PTC_TYPE_ONCE)
		sprintf_s(szTemp, 1024, "Type_Once");
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[2], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%.1f", m_fSpeed);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[3], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%.3f", m_fAcc);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[4], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fAccMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[5], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fAccMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[6], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fPositionX);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[7], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosXMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[8], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosXMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[9], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fPositionY);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[10], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosYMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[11], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosYMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[12], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fPositionZ);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[13], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosZMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[14], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandPosZMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[15], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fDirectionX);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[16], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirXMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[17], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirXMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[18], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fDirectionY);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[19], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirYMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[20], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirYMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[21], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_fDirectionZ);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[22], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirZMin);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[23], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_fRandDirZMax);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[24], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%s", m_szFile);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[25], DT_RIGHT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

	sprintf_s(szTemp, 1024, "%d", m_nAlpha);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[26], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_nColorR);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[27], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_nColorG);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[28], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));
	sprintf_s(szTemp, 1024, "%d", m_nColorB);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc[29], DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(0, 0, 0));

}

void ParticleEdit::LoadTexture()
{
	TCHAR _curDirectoryPath[256];
	GetCurrentDirectory(256, _curDirectoryPath);



	OPENFILENAME OFN;

	ZeroMemory(&OFN, sizeof(OFN));
	OFN.lStructSize = sizeof(OFN);

	char str[300];
	char lpstrFile[MAX_PATH] = "";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = L"PNG(*.png)\0*.png\0Bit(*.bit)\0*.bit\0JPG(*.jpg)\0*.jpg\0";
	OFN.lpstrFile = (LPWSTR)lpstrFile;
	OFN.lpstrFile[0] = '\0';
	OFN.nMaxFile = 256;

	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	//OFN.Flags = OFN_NOCHANGEDIR;                       
	OFN.lpstrInitialDir = L".\\Texture\\Particle";

	GetOpenFileName(&OFN);

	MessageBox(NULL, OFN.lpstrFile, L"File Name", MB_OK);
	WideCharToMultiByte(CP_ACP, 0, OFN.lpstrFile, -1, str, 1024, NULL, NULL);

	strcpy(m_szFile, str);

	SetCurrentDirectory(_curDirectoryPath);

}

void ParticleEdit::Play()
{
	m_pParticleSet->Setup(
		m_type,
		m_fTime, m_fSpeed,
		m_fAcc, m_fAccMin, m_fAccMax,
		m_fPositionX, m_fRandPosXMin, m_fRandPosXMax,
		m_fPositionY, m_fRandPosYMin, m_fRandPosYMax,
		m_fPositionZ, m_fRandPosZMin, m_fRandPosZMax,
		m_fDirectionX, m_fRandDirXMin, m_fRandDirXMax,
		m_fDirectionY, m_fRandDirYMin, m_fRandDirYMax,
		m_fDirectionZ, m_fRandDirZMin, m_fRandDirZMax,
		m_szFile
		, D3DCOLOR_ARGB(m_nAlpha, m_nColorR, m_nColorG, m_nColorB));
		
	m_pParticleSet->Start();
}

void ParticleEdit::SaveParticle()
{
	TCHAR _curDirectoryPath[256];
	GetCurrentDirectory(256, _curDirectoryPath);

	HANDLE file;
	FILE* fp;
	OPENFILENAME OFN;

	ZeroMemory(&OFN, sizeof(OFN));
	OFN.lStructSize = sizeof(OFN);

	char str[300];
	char lpstrFile[MAX_PATH] = "";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = L"Text(*.txt)\0*.txt\0";
	OFN.lpstrFile = (LPWSTR)lpstrFile;
	OFN.lpstrFile[0] = '\0';
	OFN.nMaxFile = 256;
	OFN.lpstrInitialDir = L"Particle\resource";
	OFN.Flags = OFN_OVERWRITEPROMPT;

	GetOpenFileName(&OFN);

	WideCharToMultiByte(CP_ACP, 0, OFN.lpstrFile, -1, str, 1024, NULL, NULL);
	string szfile = (string)str + ".txt";

	fopen_s(&fp, szfile.c_str(), "w");

	fprintf(fp, "%.1f\n", m_fTime);
	fprintf(fp, "%d\n", m_type);
	fprintf(fp, "%.1f\n", m_fSpeed);

	fprintf(fp, "%.3f\n", m_fAcc);
	fprintf(fp, "%d\n", m_fAccMin);
	fprintf(fp, "%d\n", m_fAccMax);

	fprintf(fp, "%d\n", m_fPositionX);
	fprintf(fp, "%d\n", m_fRandPosXMin);
	fprintf(fp, "%d\n", m_fRandPosXMax);

	fprintf(fp, "%d\n", m_fPositionY);
	fprintf(fp, "%d\n", m_fRandPosYMin);
	fprintf(fp, "%d\n", m_fRandPosYMax);

	fprintf(fp, "%d\n", m_fPositionZ);
	fprintf(fp, "%d\n", m_fRandPosZMin);
	fprintf(fp, "%d\n", m_fRandPosZMax);

	fprintf(fp, "%d\n", m_fDirectionX);
	fprintf(fp, "%d\n", m_fRandDirXMin);
	fprintf(fp, "%d\n", m_fRandDirXMax);

	fprintf(fp, "%d\n", m_fDirectionY);
	fprintf(fp, "%d\n", m_fRandDirYMin);
	fprintf(fp, "%d\n", m_fRandDirYMax);

	fprintf(fp, "%d\n", m_fDirectionZ);
	fprintf(fp, "%d\n", m_fRandDirZMin);
	fprintf(fp, "%d\n", m_fRandDirZMax);


	fprintf(fp, "%d\n", m_nAlpha);
	fprintf(fp, "%d\n", m_nColorR);
	fprintf(fp, "%d\n", m_nColorG);
	fprintf(fp, "%d\n", m_nColorB);

	fprintf(fp, "%s", m_szFile);

	fclose(fp);
	SetCurrentDirectory(_curDirectoryPath);
}

void ParticleEdit::LoadParticle()
{
	TCHAR _curDirectoryPath[256];
	GetCurrentDirectory(256, _curDirectoryPath);

	OPENFILENAME OFN;

	ZeroMemory(&OFN, sizeof(OFN));
	OFN.lStructSize = sizeof(OFN);

	char str[300];
	char lpstrFile[MAX_PATH] = "";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hWnd;
	OFN.lpstrFilter = L"Text(*.txt)\0 * .txt\0";
	OFN.lpstrFile = (LPWSTR)lpstrFile;
	OFN.lpstrFile[0] = '\0';
	OFN.nMaxFile = 256;
	OFN.lpstrInitialDir = L"Particle\resource";
	OFN.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&OFN);

	//MessageBox(NULL, OFN.lpstrFile, L"File Name", MB_OK);
	WideCharToMultiByte(CP_ACP, 0, OFN.lpstrFile, -1, str, 1024, NULL, NULL);

	FILE* fp;
	fopen_s(&fp, str, "r");

	char s[1024];

	fgets(s, 1024, fp);		m_fTime = atof(s);
	fgets(s, 1024, fp);		m_type = (PARTICLE_TYPE)atoi(s);
	fgets(s, 1024, fp);		m_fSpeed = atof(s);

	fgets(s, 1024, fp);		m_fAcc = atof(s);
	fgets(s, 1024, fp);		m_fAccMin = atoi(s);
	fgets(s, 1024, fp);		m_fAccMax = atoi(s);

	fgets(s, 1024, fp);		m_fPositionX = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosXMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosXMax = atoi(s);

	fgets(s, 1024, fp);		m_fPositionY = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosYMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosYMax = atoi(s);

	fgets(s, 1024, fp);		m_fPositionZ = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosZMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandPosZMax = atoi(s);

	fgets(s, 1024, fp);		m_fDirectionX = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirXMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirXMax = atoi(s);

	fgets(s, 1024, fp);		m_fDirectionY = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirYMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirYMax = atoi(s);

	fgets(s, 1024, fp);		m_fDirectionZ = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirZMin = atoi(s);
	fgets(s, 1024, fp);		m_fRandDirZMax = atoi(s);

	fgets(s, 1024, fp);		m_nAlpha = atoi(s);
	fgets(s, 1024, fp);		m_nColorR = atoi(s);
	fgets(s, 1024, fp);		m_nColorG = atoi(s);
	fgets(s, 1024, fp);		m_nColorB = atoi(s);

	fgets(s, 1024, fp);		strcpy(m_szFile, s);

	fclose(fp);

	m_pParticleSet->Setup(
		m_type,
		m_fTime, m_fSpeed,
		m_fAcc, m_fAccMin, m_fAccMax,
		m_fPositionX, m_fRandPosXMin, m_fRandPosXMax,
		m_fPositionY, m_fRandPosYMin, m_fRandPosYMax,
		m_fPositionZ, m_fRandPosZMin, m_fRandPosZMax,
		m_fDirectionX, m_fRandDirXMin, m_fRandDirXMax,
		m_fDirectionY, m_fRandDirYMin, m_fRandDirYMax,
		m_fDirectionZ, m_fRandDirZMin, m_fRandDirZMax,
		m_szFile, D3DCOLOR_ARGB(m_nAlpha, m_nColorR, m_nColorG, m_nColorB));

	SetCurrentDirectory(_curDirectoryPath);
}
