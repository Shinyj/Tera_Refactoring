#include "stdafx.h"
#include "cGrid.h"


cGrid::cGrid()
{
	D3DXMatrixIdentity(&m_matWorld);
}


cGrid::~cGrid()
{
}

void cGrid::Setup()
{
	int nNumLine = 50;
	float fInterval = 1.0f;
	float fMax = fInterval * nNumLine;

	m_vAxisX = D3DXVECTOR3(fMax, 0, 0);
	m_vAxisZ = D3DXVECTOR3(0, 0, fMax);

	ST_PC_VERTEX v;

	v.c = D3DCOLOR_ARGB(255, 0, 0, 0);
	for (int i = 1; i <= nNumLine; i++)
	{
		v.p = D3DXVECTOR3(-fMax, 0, i);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, i);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(-fMax, 0, -i);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(fMax, 0, -i);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(i, 0, -fMax);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(i, 0, fMax);
		m_vecVertex.push_back(v);

		v.p = D3DXVECTOR3(-i, 0, -fMax);
		m_vecVertex.push_back(v);
		v.p = D3DXVECTOR3(-i, 0, fMax);
		m_vecVertex.push_back(v);
	}

	v.p = D3DXVECTOR3(-fMax, 0, 0);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(fMax, 0, 0);
	m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(0, 0, -fMax);
	m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(0, 0, fMax);
	m_vecVertex.push_back(v);
}

void cGrid::Render()
{
	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	DEVICE->SetFVF(ST_PC_VERTEX::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_LINELIST,
		m_vecVertex.size() / 2,
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}