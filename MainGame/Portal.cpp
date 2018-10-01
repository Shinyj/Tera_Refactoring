#include "stdafx.h"
#include "Portal.h"


Portal::Portal()
{
}


Portal::~Portal()
{
	SAFE_DELETE(m_pSpere);
	SAFE_DELETE(m_pBoundingBox);
}

void Portal::Setup(D3DXVECTOR3 pos)
{
	m_vPosition = pos;

	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));

	//D3DXMatrixTranslation(&m_matWorld, )

	// 备 面倒 康开 积己
	m_pSpere = new cSpere;
	m_pSpere->Setup(D3DXVECTOR3(0, 0, 0), 120);

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	D3DXMatrixTranslation(&mat, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_pBoundingBox->SetWorld(mat);
	m_pSpere->SetWorld(mat);
}

void Portal::Update()
{
	cGameObject::Update();
}

void Portal::Render()
{
	cGameObject::Render();
}

bool Portal::IsInRange()
{
	cGameObject*	player = OBJECTMANAGER->GetPlayer();
	float lengh = D3DXVec3Length(&(this->GetSpere()->GetPosition() - player->GetSpere()->GetPosition()));
	float distance = this->GetSpere()->GetRadius() + player->GetSpere()->GetRadius();

	if (lengh < distance)
	{
		if (KEY->IsOnceKeyDown('F'))
		{
			SOUND->Play("Use_TeleportWarf");
			return true;
		}
	}
	return false;
}
