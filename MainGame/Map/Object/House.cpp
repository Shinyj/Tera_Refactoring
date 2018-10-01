#include "stdafx.h"
#include "House.h"


House::House()
{
}


House::~House()
{
}

void House::Setup()
{
	const int size = 6;

	char fileName[size][256] = {
		"Resource/XFile/Map/Field/ANC_B_4858_SL.X",
		"Resource/XFile/Map/Field/Skydom2048_1024.X",
		"Resource/XFile/Town/House01.X",
		"Resource/XFile/Town/House02.X",
		"Resource/XFile/Town/House03.X",
		"Resource/XFile/Town/WarpGate.X"

	};

	for (int i = 0; i < size; i++)
	{
		m_vHouse.push_back(STATICMESH->GetStaticMesh(fileName[i]));
	}

	// 파티클 설치
	m_pParticleWarp = PARTICLE->GetParticle("PortalEffect");
	D3DXMATRIX mat;
	D3DXMatrixTranslation(&mat, 969, -406, 4157);
	m_pParticleWarp->SetWorld(mat);
	m_pParticleWarp->Start();

	PARTICLE->AddChild(m_pParticleWarp);
}

void House::Update()
{
}

void House::Render()
{
	D3DXMATRIX matWorld, matS;
	D3DXMatrixScaling(&matS, 1.0f, 1.0f, 1.0f);  // 1/256 = 0.00390625
												 //D3DXMatrixRotationX(&matR, -D3DX_PI / 2.0f);
	matWorld = matS;
	DEVICE->SetTransform(D3DTS_WORLD, &matWorld);

	if (!m_vHouse.empty())
	{
		for (auto p : m_vHouse)
		{
			p->Render();
		}
	}
}

void House::Destroy()
{
	for (auto a : m_vHouse)
	{
		SAFE_DELETE(a);
	}
	delete this;
}
