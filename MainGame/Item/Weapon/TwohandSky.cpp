#include "stdafx.h"
#include "TwohandSky.h"


TwohandSky::TwohandSky()
{
}


TwohandSky::~TwohandSky()
{
	SAFE_DELETE(m_sName);
	SAFE_DELETE(m_pBody);
}

void TwohandSky::Setup()
{
	m_pBody = STATICMESH->GetStaticMesh(WeaponXFilePath + L"TwoHandSword_02.X");
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(
		D3DXVECTOR3(-5, -10, -5),
		D3DXVECTOR3(5, 50, 5));

	m_vOnPos = D3DXVECTOR3(0, 50, 0);
	m_vDownPos = D3DXVECTOR3(0, 0, 0);

	for (int i = 0; i < 10; i++)
		m_arrPos[i].c = D3DCOLOR_ARGB(100, 0, 0, 255);
}

void TwohandSky::Update()
{
	cItemBase::Update();
}

void TwohandSky::Rendeer()
{
	cItemBase::Render();
}
