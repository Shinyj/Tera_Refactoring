#include "stdafx.h"
#include "cSpere.h"


cSpere::cSpere()
	: m_pSpere(NULL)
	, m_vPosition(0, 0, 0)
	, m_fRadius(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
}


cSpere::~cSpere()
{
	m_pSpere->Release();
}

void cSpere::Setup(D3DXVECTOR3 pos, float r)
{
	//m_vPosition = pos;
	m_fRadius = r;
	D3DXCreateSphere(
		DEVICE, r, 20, 20, &m_pSpere, NULL);

	m_matWorld._41 = pos.x;
	m_matWorld._42 = pos.y;
	m_matWorld._43 = pos.z;

	//D3DXMatrixTranslation(&m_matWorld, m_vPosition.x, m_vPosition.y, m_vPosition.z);

}

void cSpere::Update()
{
}

void cSpere::Render()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pSpere->DrawSubset(0);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

}
