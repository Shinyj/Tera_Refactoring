#include "stdafx.h"
#include "cRay.h"

cRay::cRay()
	: m_vOriginal(0,0,0)
	, m_vDirection(0,0,-1)
{
}


cRay::~cRay()
{
}

cRay cRay::RayAtViewSpace(int nScreenX, int nScreenY)
{
	D3DVIEWPORT9 viewPort;
	DEVICE->GetViewport(&viewPort);

	D3DXMATRIXA16 matProjection;
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProjection);

	cRay r;
	r.m_vDirection.x = ((2.0f * nScreenX) / viewPort.Width - 1.0f) / matProjection._11;
	r.m_vDirection.y = ((-2.0f * nScreenY) / viewPort.Height + 1.0f) / matProjection._22;
	r.m_vDirection.z = 1.0f;

	return r;
}

cRay cRay::RayAtWorldSpace(int nScreenX, int nScreenY)
{
	cRay r = cRay::RayAtViewSpace(nScreenX, nScreenY);

	D3DXMATRIXA16 matView, matInvView;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matInvView, 0, &matView);

	D3DXVec3TransformCoord(&r.m_vOriginal, &r.m_vOriginal, &matInvView);
	D3DXVec3TransformNormal(&r.m_vDirection, &r.m_vDirection, &matInvView);
	D3DXVec3Normalize(&r.m_vDirection, &r.m_vDirection);

	return r;
}

float cRay::IsPicked(cSpere * spere)
{
	int result = 0.0f;

	cRay r = (*this);

	D3DXMATRIXA16 matInvWorld;
	D3DXMatrixIdentity(&matInvWorld);
	matInvWorld._41 = -spere->GetPosition().x;
	matInvWorld._42 = -spere->GetPosition().y;
	matInvWorld._43 = -spere->GetPosition().z;

	D3DXVec3TransformCoord(&r.m_vOriginal, &r.m_vOriginal, &matInvWorld);
	D3DXVec3TransformNormal(&r.m_vDirection, &r.m_vDirection, &matInvWorld);

	result = abs( D3DXVec3Length(&(spere->GetPosition() - m_vOriginal)));

	float vv = D3DXVec3Dot(&r.m_vDirection, &r.m_vDirection);
	float qv = D3DXVec3Dot(&r.m_vOriginal, &r.m_vDirection);
	float qq = D3DXVec3Dot(&r.m_vOriginal, &r.m_vOriginal);
	float rr = spere->GetRadius() * spere->GetRadius();

	// result가 0.0f 아니면 충돌, 값은 거리
	if (qv * qv - vv * (qq - rr) >= 0)
		return result;
	else
		return 0.0f;
}

// 충돌이 됐다면 그 위치도 넣어주는 착한 함수
bool cRay::IntersectTri(IN D3DXVECTOR3 & v0, IN D3DXVECTOR3 & v1, IN D3DXVECTOR3 & v2, OUT D3DXVECTOR3 & vPickedPosition)
{
	float u, v, t;
	bool b = D3DXIntersectTri(&v0, &v1, &v2, &m_vOriginal, &m_vDirection, &u, &v, &t);
	vPickedPosition = m_vOriginal + (t * m_vDirection);
	return b;
}
