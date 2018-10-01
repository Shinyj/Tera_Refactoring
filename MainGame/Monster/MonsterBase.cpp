#include "stdafx.h"
#include "MonsterBase.h"
#include "MainGame\Map\iMap.h"
#include "MainGame\Item\cDropItem.h"

MonsterBase::MonsterBase()\
	: m_fSpeed(1.0f)
	, m_vBeforeAnimPos(0, 0, 0)
	, m_vCurAnimPos(0, 0, 0)
	, m_bIsGen(true)
	, m_bAwake(false)
	, m_bFight(false)
	, m_vBehaviorSpot(500, 0, 0)
	, m_pMap(NULL)
	, m_bAnimation(false)
	, m_bEscapeToggle(false)
	, m_bEscapeToggle2(false)
	, m_bIdle(true)
	, m_bAtkTerm(true)
	, m_bAngleLock(false)
	, m_bDeath(false)
	, m_bFlinch(false)
	, m_bWalkOnOff(false)
	, m_bStart(false)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matAnimWorld);
}


MonsterBase::~MonsterBase()
{
}

void MonsterBase::Setup()
{
	cGameObject::Setup();
	SetUpStateBar();
}

void MonsterBase::Update()
{
	cGameObject::Update();

	PlusMapHeight();
	UpdateUpStateBar();
}

void MonsterBase::Render()
{
	cGameObject::Render();

	RenderUpStateBar(m_sName);
}

void MonsterBase::PlusMapHeight()
{
	if (m_pMap)
		m_pMap->GetHeight(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void MonsterBase::MonoBehavior(void)
{
}

bool MonsterBase::Attack(float damage)
{
	return false;
}

void MonsterBase::ItemDrop(const char * itemName)
{
	cDropItem * drItem = new cDropItem;
	drItem->Setup(itemName, m_vPosition);
	
	OBJECTMANAGER->AddItemObject(drItem);
}

void MonsterBase::SetUpStateBar()
{
	m_BackBar = TEXTURE->GetSprite("Resource/Texture/MonsterInfo/MonsterBack.png");

	m_pHpBar = new ProgressBar;
	m_pHpBar->Setup("Resource/Texture/MonsterInfo/MonsterHp.png",
		"Resource/Texture/MonsterInfo/MonsterEmpty.png",
		WINSIZEX / 2 - 75, 40, 600, 16);
}

void MonsterBase::UpdateUpStateBar()
{
	m_pHpBar->SetGauge(m_fHpCur, m_fHpMax);
}

void MonsterBase::RenderUpStateBar(string name)
{
	if (m_isPicked)
	{
		m_BackBar->Render(D3DXVECTOR3(0, 0, 0),
			D3DXVECTOR3(WINSIZEX / 2 - m_BackBar->textureInfo.Width / 2.0f, 10, 0));

		m_pHpBar->Render();

		char szTemp[1024];
		RECT rc;
		sprintf_s(szTemp, 1024, "%s", name.c_str());
		SetRect(&rc,
			WINSIZEX / 2 - 40, 20, WINSIZEX / 2 + 40, 152);

		LPD3DXFONT pFont = FONT->GetFont(cFontManager::TF_UI_NUMBER2);
		pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
			DT_CENTER, D3DCOLOR_XRGB(255, 255, 255));
	}
}
