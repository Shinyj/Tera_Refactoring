#include "stdafx.h"
#include "cBossRoom_Road.h"


cBossRoom_Road::cBossRoom_Road()
	: pos(0, 0, 0)
{
	D3DXMatrixIdentity(&matWorld);
}


cBossRoom_Road::~cBossRoom_Road()
{
}

void cBossRoom_Road::Setup()
{
	const int size = 2;

	char fileName[size][256] = {
		"Resource/XFile/Boss_Room/ARG_Cor02_A_02_Floor_SM.X",
		"Resource/XFile/Boss_Room/ARG_Cor02_A_02_SM.X"
	};

	for (int i = 0; i < size; i++)
	{
		//cXLoader  ground_meshX;
		//m_vecRoadParts.push_back(ground_meshX.xFimeLoad(fileName[i]));
		m_vecRoadParts.push_back(STATICMESH->GetStaticMesh(fileName[i]));
	}

	for (int i = 0; i < 5; i++)
	{
		D3DXMATRIX matPos, world;
		D3DXMatrixIdentity(&matPos);
		D3DXMatrixTranslation(&matPos, pos.x, pos.y, pos.z);
		world = matPos;
		matWorld = world;

		m_vecBossRoom_Road.push_back(*this);

		pos.z += 512.0f;
	}

}

void cBossRoom_Road::Render()
{
	if (!m_vecRoadParts.empty())
	{
		DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

		for (auto p : m_vecRoadParts)
		{
			p->Render();
		}
	}
}

void cBossRoom_Road::Destroy()
{
	if (!m_vecRoadParts.empty())
	{
		for (auto p : m_vecRoadParts)
		{
			SAFE_DELETE(p);
		}
		delete this;

	}
}
