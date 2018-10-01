#include "stdafx.h"
#include "TwohandFish.h"


TwohandFish::TwohandFish()
{
}


TwohandFish::~TwohandFish()
{
	SAFE_DELETE(m_sName);
	SAFE_DELETE(m_pBody);
}

void TwohandFish::Setup()
{
	m_pBody = STATICMESH->GetStaticMesh(WeaponXFilePath + L"TwoHandSword_03.X");
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(
		D3DXVECTOR3(-5, -10, -5),
		D3DXVECTOR3(5, 50, 5));

	m_vOnPos = D3DXVECTOR3(0, 50, 0);
	m_vDownPos = D3DXVECTOR3(0, 0, 0);

	for (int i = 0; i < 10; i++)
		m_arrPos[i].c = D3DCOLOR_ARGB(100, 255, 255, 255);
}

void TwohandFish::Update()
{
	cItemBase::Update();
}

void TwohandFish::Rendeer()
{
	cItemBase::Render();
}
