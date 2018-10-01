#include "stdafx.h"
#include "TwoHandClaymore.h"


TwoHandClaymore::TwoHandClaymore()
{
}


TwoHandClaymore::~TwoHandClaymore()
{
	SAFE_DELETE(m_sName);
	SAFE_DELETE(m_pBody);
}

void TwoHandClaymore::Setup()
{
	m_pBody = STATICMESH->GetStaticMesh(WeaponXFilePath + L"TwoHandSword_00.X");
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(
		D3DXVECTOR3(-5, -10, -5),
		D3DXVECTOR3(5, 50, 5));

	m_vOnPos = D3DXVECTOR3(0, 50, 0);
	m_vDownPos = D3DXVECTOR3(0, 0, 0);

	m_pSlashEffectTexture = TEXTURE->GetTexture("Resource/Texture/Effect/SwordShadow.png");

}

void TwoHandClaymore::Update()
{
	cItemBase::Update();
}

void TwoHandClaymore::Rendeer()
{
	cItemBase::Render();
}
