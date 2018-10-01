#include "stdafx.h"
#include "TwohandIce.h"


TwohandIce::TwohandIce()
{
}


TwohandIce::~TwohandIce()
{
	SAFE_DELETE(m_sName);
	SAFE_DELETE(m_pBody);
}

void TwohandIce::Setup()
{
	m_pBody = STATICMESH->GetStaticMesh(WeaponXFilePath + L"TwoHandSword_04.X");
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(
		D3DXVECTOR3(-5, -10, -5),
		D3DXVECTOR3(5, 50, 5));

	m_vOnPos = D3DXVECTOR3(0, 50, 0);
	m_vDownPos = D3DXVECTOR3(0, 0, 0);

	for (int i = 0; i < 10; i++)
		m_arrPos[i].c = D3DCOLOR_ARGB(100, 0, 0, 255);
}

void TwohandIce::Update()
{
	cItemBase::Update();
}

void TwohandIce::Rendeer()
{
	cItemBase::Render();
}
