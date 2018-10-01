#include "stdafx.h"
#include "scBossRoom.h"

#include "MainGame\Monster\Kelsaik\Kelsaik.h"
#include "MainGame\Character\Player\Player.h"

#include "MainGame\Map\BossRoom\cBossRoom_Doll.h"
#include "MainGame\Map\BossRoom\cBossRoom_Road.h"
#include "MainGame\Map\BossRoom\cBossRoom_Wall.h"

scBossRoom::scBossRoom()
	: m_nBGBlackAlpha(255)
{
}


scBossRoom::~scBossRoom()
{
	SAFE_DELETE(m_pBossRoom_Wall);
	SAFE_DELETE(m_pBossRoom_Road);
	SAFE_DELETE(m_pBossRoom_Doll);
	SAFE_DELETE(m_pBackGroundBlack);
	SAFE_DELETE(m_pPlayer);
}

void scBossRoom::Setup()
{
	Roader_WallGate();

	m_pBossRoom_Wall = new cBossRoom_Wall;
	m_pBossRoom_Wall->Setup();
	m_pBossRoom_Road = new cBossRoom_Road;
	m_pBossRoom_Road->Setup();
	m_pBossRoom_Doll = new cBossRoom_Doll;
	m_pBossRoom_Doll->Setup();

	m_pPlayer = new Player;
	m_pPlayer->Setup();
	m_pPlayer->SetPosition(D3DXVECTOR3(0, -45.2, 850));// 1162
													   //m_pPopori->SetMap(m_pBossRoom_Wall);
	OBJECTMANAGER->AddCharaObject(m_pPlayer);

	m_pKelsaik = new Kelsaik;
	m_pKelsaik->Setup();
	m_pKelsaik->SetPosition(D3DXVECTOR3(0, -45.2, 0));
	//m_pKelsaik->SetMap(m_pBossRoom_Wall);
	OBJECTMANAGER->AddMonsterObject(m_pKelsaik);

	m_pBackGroundBlack = TEXTURE->GetSprite(SceneTexPath + L"BGBlack.png");

	isOptionMode = false;

	SOUND->Play("BossBGM");
}

void scBossRoom::Release()
{
}

void scBossRoom::Update()
{
	m_pKelsaik->Update();
	m_pPlayer->Update();

	PARTICLE->Update();

	//////////
	if (m_nBGBlackAlpha > 0)
		m_nBGBlackAlpha -= 5;

	UI->Update();
	ITEMMANAGER->Update();
}

void scBossRoom::Render()
{
	Render_Wall();

	m_pKelsaik->Render();

	PARTICLE->Render();
	m_pPlayer->Render();

	/////////////
	UI->Render();
	ITEMMANAGER->Render();

	if (m_nBGBlackAlpha>0)
		m_pBackGroundBlack->AlphaRender(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), m_nBGBlackAlpha);
}

void scBossRoom::Render_Wall()
{
	if (!m_vBossRoom_SingObj.empty())
	{
		for (auto p : m_vBossRoom_SingObj)
		{
			p->Render();
		}
	}

	for (auto p : m_pBossRoom_Wall->m_vecBossRoom_Wall)
	{
		p.Render();
	}

	for (auto p : m_pBossRoom_Road->m_vecBossRoom_Road)
	{
		p.Render();
	}

	for (auto p : m_pBossRoom_Doll->m_vecBossRoom_Doll)
	{
		p.Render();
	}
}

void scBossRoom::Roader_WallGate()
{
	const int size = 6;

	char fileName[size][256] = {
		"Resource/XFile/Boss_Room/ARG_Room_A_Enter_SM_P1.X",
		"Resource/XFile/Boss_Room/ARG_Room_A_Enter_SM_P2.X",
		"Resource/XFile/Boss_Room/ARG_Room_A_Enter_SM_P3.X",
		"Resource/XFile/Boss_Room/ARG_Room_A_Enter_SM_P4.X",
		//"XFile\Boss_Room\ARG_Room_A_Enter_SM_P5.X",
		"Resource/XFile/Boss_Room/GilMark.X",
		"Resource/XFile/Boss_Room/GateLock.X" // ¹®
	};

	for (int i = 0; i < size; i++)
	{
		//cXLoader  ground_meshX;
		//m_vBossRoom_SingObj.push_back(ground_meshX.xFimeLoad(fileName[i]));
		m_vBossRoom_SingObj.push_back(STATICMESH->GetStaticMesh(fileName[i]));
	}
}

void scBossRoom::Destroy()
{
	if (!m_vBossRoom_SingObj.empty())
	{
		for (auto p : m_vBossRoom_SingObj)
		{
			SAFE_DELETE(p);
		}
		delete this;
	}

	for (auto p : m_pBossRoom_Wall->m_vecBossRoom_Wall)
	{
		p.Destroy();
	}
	//delete this;


	for (auto p : m_pBossRoom_Road->m_vecBossRoom_Road)
	{
		p.Destroy();
	}
	//delete this;

	for (auto p : m_pBossRoom_Doll->m_vecBossRoom_Doll)
	{
		p.Destroy();
	}
	//delete this;
}
