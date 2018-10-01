#include "stdafx.h"
#include "MainGame\Map\BossRoom\cBossRoom_Doll.h"

cBossRoom_Doll::cBossRoom_Doll()
	: rotY(D3DX_PI / 4)
{
	D3DXMatrixIdentity(&matWorld);
}

cBossRoom_Doll::~cBossRoom_Doll()
{
}

void cBossRoom_Doll::Setup()
{
	const int size = 2;

	char fileName[size][256] = {
		"Resource/XFile/Boss_Room/ARG_TES_RCELV02_SM_1.X",
		"Resource/XFile/Boss_Room/ARG_TES_RCELV02_SM_2.X"
	};

	for (int i = 0; i < size; i++)
	{
		m_vecDollParts.push_back(STATICMESH->GetStaticMesh(fileName[i]));
	}

	for (int i = 0; i < 7; i++)
	{
		D3DXMATRIX matRotY, world;
		D3DXMatrixRotationY(&matRotY, rotY);
		world = matRotY;
		matWorld = world;

		m_vecBossRoom_Doll.push_back(*this);

		rotY += D3DX_PI / 4;
	}
}

void cBossRoom_Doll::Render()
{
	if (!m_vecDollParts.empty())
	{
		DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

		for (auto p : m_vecDollParts)
		{
			p->Render();
		}
	}
}

void cBossRoom_Doll::Destroy()
{
	if (!m_vecDollParts.empty())
	{
		for (auto p : m_vecDollParts)
		{
			SAFE_DELETE(p);
		}
		delete this;

	}
}
