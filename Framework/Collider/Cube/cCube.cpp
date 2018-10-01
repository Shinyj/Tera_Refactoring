#include "stdafx.h"
#include "cCube.h"


cCube::cCube()
{
}


cCube::~cCube()
{
}

void cCube::Setup()
{
}

void cCube::Setup(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	ST_PCT_VERTEX v;
	D3DCOLOR color(0xff00ff00);

	v.c = color;

	v.p = D3DXVECTOR3(min.x, max.y, min.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);  // -1 1 -1
	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);  // 1 1 -1
	v.p = D3DXVECTOR3(min.x, min.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v); // -1 -1 -1

	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);  // 1 1 -1
	v.p = D3DXVECTOR3(max.x, min.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);   // 1 -1 -1
	v.p = D3DXVECTOR3(min.x, min.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);  // -1 1 -1

	v.p = D3DXVECTOR3(max.x, max.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);  // 1 1 1
	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);  // -1 1 1
	v.p = D3DXVECTOR3(max.x, min.y, max.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);  // -1 1 1
	v.p = D3DXVECTOR3(min.x, min.y, max.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, max.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, max.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	//
	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, max.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, max.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, min.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, min.y, max.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, max.y, min.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, min.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, max.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, max.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, max.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, max.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(max.x, min.y, max.z); v.t = D3DXVECTOR2(0, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, min.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	v.p = D3DXVECTOR3(min.x, min.y, max.z); v.t = D3DXVECTOR2(1, 0); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(min.x, min.y, min.z); v.t = D3DXVECTOR2(1, 1); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(max.x, min.y, min.z); v.t = D3DXVECTOR2(0, 1); m_vecVertex.push_back(v);

	m_vecPtVertex.resize(m_vecVertex.size());

	for (int i = 0; i < m_vecVertex.size(); i++)
	{
		m_vecPtVertex[i].p = m_vecVertex[i].p;
		m_vecPtVertex[i].t = m_vecVertex[i].t;
	}
}

void cCube::Update()
{
}

void cCube::Update(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	m_vecVertex[0].p = D3DXVECTOR3(min.x, min.y, min.z); //앞 0 1 2 3
	m_vecVertex[1].p = D3DXVECTOR3(min.x, max.y, min.z);
	m_vecVertex[2].p = D3DXVECTOR3(max.x, max.y, min.z);

	m_vecVertex[3].p = D3DXVECTOR3(min.x, min.y, min.z);
	m_vecVertex[4].p = D3DXVECTOR3(max.x, max.y, min.z);
	m_vecVertex[5].p = D3DXVECTOR3(max.x, min.y, min.z);

	m_vecVertex[6].p = D3DXVECTOR3(max.x, min.y, max.z); //뒤 4 5 6 7
	m_vecVertex[7].p = D3DXVECTOR3(max.x, max.y, max.z);
	m_vecVertex[8].p = D3DXVECTOR3(min.x, max.y, max.z);

	m_vecVertex[9].p = D3DXVECTOR3(max.x, min.y, max.z);
	m_vecVertex[10].p = D3DXVECTOR3(min.x, max.y, max.z);
	m_vecVertex[11].p = D3DXVECTOR3(min.x, min.y, max.z);

	m_vecVertex[12].p = D3DXVECTOR3(min.x, min.y, max.z); // 좌 7 6 1 0
	m_vecVertex[13].p = D3DXVECTOR3(min.x, max.y, max.z);
	m_vecVertex[14].p = D3DXVECTOR3(min.x, max.y, min.z);

	m_vecVertex[15].p = D3DXVECTOR3(min.x, min.y, max.z);
	m_vecVertex[16].p = D3DXVECTOR3(min.x, max.y, min.z);
	m_vecVertex[17].p = D3DXVECTOR3(min.x, min.y, min.z);

	m_vecVertex[18].p = D3DXVECTOR3(max.x, min.y, min.z); //우 3 2 5 4
	m_vecVertex[19].p = D3DXVECTOR3(max.x, max.y, min.z);
	m_vecVertex[20].p = D3DXVECTOR3(max.x, max.y, max.z);

	m_vecVertex[21].p = D3DXVECTOR3(max.x, min.y, min.z);
	m_vecVertex[22].p = D3DXVECTOR3(max.x, max.y, max.z);
	m_vecVertex[23].p = D3DXVECTOR3(max.x, min.y, max.z);

	m_vecVertex[24].p = D3DXVECTOR3(min.x, max.y, min.z); //상 1 6 5 2
	m_vecVertex[25].p = D3DXVECTOR3(min.x, max.y, max.z);
	m_vecVertex[26].p = D3DXVECTOR3(max.x, max.y, max.z);

	m_vecVertex[27].p = D3DXVECTOR3(min.x, max.y, min.z);
	m_vecVertex[28].p = D3DXVECTOR3(max.x, max.y, max.z);
	m_vecVertex[29].p = D3DXVECTOR3(max.x, max.y, min.z);

	m_vecVertex[30].p = D3DXVECTOR3(min.x, min.y, max.z); //아래 7 0 3 4
	m_vecVertex[31].p = D3DXVECTOR3(min.x, min.y, min.z);
	m_vecVertex[32].p = D3DXVECTOR3(max.x, min.y, min.z);

	m_vecVertex[33].p = D3DXVECTOR3(min.x, min.y, max.z);
	m_vecVertex[34].p = D3DXVECTOR3(max.x, min.y, min.z);
	m_vecVertex[35].p = D3DXVECTOR3(max.x, min.y, max.z);
}

void cCube::Render()
{
	DEVICE->SetFVF(ST_PCT_VERTEX::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecVertex.size() / 3,
		&m_vecVertex[0],
		sizeof(ST_PCT_VERTEX));
}

void cCube::PTRender()
{
	DEVICE->SetFVF(ST_PT_VERTEX::FVF);
	DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
		m_vecPtVertex.size() / 3,
		&m_vecPtVertex[0],
		sizeof(ST_PT_VERTEX));
}
