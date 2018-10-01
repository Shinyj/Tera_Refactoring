#include "stdafx.h"
#include "scField.h"

#include "MainGame\Portal.h"
#include "MainGame\Monster\Beast\Beast.h"
#include "MainGame\Character\NPC\NpcShop.h"
#include "MainGame\Monster\GateKeeper\GateKeeper.h"
#include "MainGame\Map\Object\House.h"
#include "MainGame\Map\FieldMap.h"
#include "MainGame\Character\Player\Player.h"

scField::scField()
	: m_pBackGroundBlack(NULL)
	, m_pPlayer(NULL)
	, m_pPortal(NULL)
	, m_pPoalong(NULL)
	, m_nBGBlackAlpha(255)
{
}


scField::~scField()
{
	SAFE_DELETE(m_pBackGroundBlack);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pPortal);


	SAFE_DELETE(m_pPoalong);
	SAFE_DELETE(m_pMap);

	m_pTown_House->Destroy();
	SAFE_DELETE(m_pBoxLoader);
}

void scField::Setup()
{
	m_pTown_House = new House;
	m_pTown_House->Setup();

	m_pMap = new FieldMap;
	m_pMap->Setup();


	m_pPlayer = new Player;
	m_pPlayer->Setup();
	m_pPlayer->SetPosition(D3DXVECTOR3(1200, 0, 1400));
	m_pPlayer->SetMap(m_pMap);
	OBJECTMANAGER->AddCharaObject(m_pPlayer);
	OBJECTMANAGER->SetPlayer(m_pPlayer);

	m_pPortal = new Portal;
	m_pPortal->Setup(D3DXVECTOR3(969, -406, 4157));
	m_pBackGroundBlack = TEXTURE->GetSprite("Resource/Texture/SceneTexture/BGBlack.png");


	//필드보스
	GateKeeper * monster = new GateKeeper;
	monster->Setup(D3DXVECTOR3(1200, 0, 3550));
	monster->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(monster);

	Beast * beast = new Beast;
	beast = new Beast;
	beast->Setup(D3DXVECTOR3(1247, 0, 2078));
	beast->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(beast);

	beast = new Beast;
	beast->Setup(D3DXVECTOR3(1230, 0, 2048));
	beast->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(beast);

	beast = new Beast;
	beast->Setup(D3DXVECTOR3(1200, 0, 2000));
	beast->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(beast);

	beast = new Beast;
	beast->Setup(D3DXVECTOR3(1150, 0, 1800));
	beast->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(beast);

	beast = new Beast;
	beast->Setup(D3DXVECTOR3(1050, 0, 2150));
	beast->SetMap(m_pMap);
	OBJECTMANAGER->AddMonsterObject(beast);

	m_pPoalong = new NpcShop;
	m_pPoalong->Setup(D3DXVECTOR3(972, 0, 3813));
	m_pPoalong->SetMap(m_pMap);

	m_pBoxLoader = new cBoundingBoxLoader; //lsc
	m_pBoxLoader->Setup();

	OBJECTMANAGER->AddObject(m_pPortal);
	SOUND->Play("Field");

	m_isGoBoss = false;
}

void scField::Release()
{
	SAFE_DELETE(m_pBackGroundBlack);
	SAFE_DELETE(m_pPlayer);
	SAFE_DELETE(m_pPortal);
	SAFE_DELETE(m_pPoalong);
	SAFE_DELETE(m_pMap);

	SAFE_DELETE(m_pBoxLoader); //lsc
	m_pTown_House->Destroy();
}

void scField::Update()
{
	OBJECTMANAGER->Update();

	m_pPoalong->Update();

	m_pBoxLoader->Update();

	m_pPlayer->Update();

	PARTICLE->Update();

	UI->Update();
	ITEMMANAGER->Update();

	if (m_pPortal->IsInRange())
		m_isGoBoss = true;

	if (m_nBGBlackAlpha > 0 && !m_isGoBoss)
		m_nBGBlackAlpha -= 5;
	else if (m_nBGBlackAlpha < 255 && m_isGoBoss)
		m_nBGBlackAlpha += 5;


	if (m_isGoBoss && m_nBGBlackAlpha > 250)
	{
		OBJECTMANAGER->VecClear();
		SOUND->Stop("Field");
		SCENE->ChangeScene("BossLoading");
	}
}

void scField::Render()
{
	m_pPortal->Render();
	m_pTown_House->Render();

	m_pPoalong->Render();


	OBJECTMANAGER->Render();

	m_pBoxLoader->Render();

	//포포리가 제일 나중에 렌더되어야 한다.
	PARTICLE->Render();
	m_pPlayer->Render();


	UI->Render();
	ITEMMANAGER->Render();

	/////////////
	if (m_nBGBlackAlpha>0)
		m_pBackGroundBlack->AlphaRender(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), m_nBGBlackAlpha);
}
