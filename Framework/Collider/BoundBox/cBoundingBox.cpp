#include "stdafx.h"
#include "cBoundingBox.h"


cBoundingBox::cBoundingBox()
	: m_pBoundBox(NULL)
	, m_vMinPos(0, 0, 0)
	, m_vMaxPos(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);

}


cBoundingBox::~cBoundingBox()
{
	SAFE_DELETE(m_pBoundBox);
}

void cBoundingBox::Setup(D3DXVECTOR3 min, D3DXVECTOR3 max)
{
	m_pBoundBox = new cCube;
	m_pBoundBox->Setup(min, max);

	m_vMinPos = min;
	m_vMaxPos = max;

}

void cBoundingBox::Update()
{
	D3DXMATRIX pos, rot, mat;
	D3DXMatrixTranslation(&pos, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationY(&rot, m_vRoatation.y);

	m_matWorld = rot * pos;

	m_pBoundBox->Update(m_vMinPos, m_vMaxPos);
}

void cBoundingBox::Render()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);

	DEVICE->SetTexture(0, NULL);

	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	if(m_pBoundBox)
	m_pBoundBox->Render();
	DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

ST_OBB cBoundingBox::GetOBB()
{
	ST_OBB obb;
	
	D3DXVECTOR3 center = (m_vMinPos + m_vMaxPos) / 2.0f;
	D3DXVECTOR3 Min = m_vMinPos;
	D3DXVECTOR3 Max = m_vMaxPos;
	D3DXVec3TransformCoord(&center, &center, &m_matWorld);
	//D3DXVec3TransformCoord(&Min, &Min, &m_matWorld);
	//D3DXVec3TransformCoord(&Max, &Max, &m_matWorld);
	
	
	D3DXVECTOR3 v0(Min.x, Min.y, Min.z);
	
	D3DXVECTOR3 v1(Min.x, Min.y, Max.z);
	D3DXVECTOR3 v2(Max.x, Min.y, Min.z);
	D3DXVECTOR3 v3(Min.x, Max.y, Min.z);
	
	v1 -= v0;
	v2 -= v0;
	v3 -= v0;
	
	D3DXVec3TransformNormal(&v1, &v1, &m_matWorld);
	D3DXVec3TransformNormal(&v2, &v2, &m_matWorld);
	D3DXVec3TransformNormal(&v3, &v3, &m_matWorld);
	
	D3DXVec3Normalize(&obb.vAxisDir[0], &v1);
	D3DXVec3Normalize(&obb.vAxisDir[1], &v2);
	D3DXVec3Normalize(&obb.vAxisDir[2], &v3);
	
	obb.fAxisLen[0] = D3DXVec3Length(&v1)/ 2.0f ;
	obb.fAxisLen[1] = D3DXVec3Length(&v2)/ 2.0f ;
	obb.fAxisLen[2] = D3DXVec3Length(&v3)/ 2.0f ;
	
	obb.vCenterPos = center;
	
	return obb;

	//ST_OBB obb;
	//
	//D3DXVECTOR3 center(0, 0, 0);
	//D3DXVECTOR3 Min = m_vMinPos;
	//D3DXVECTOR3 Max = m_vMaxPos;
	//D3DXVec3TransformCoord(&center, &center, &m_matWorld);
	//D3DXVec3TransformCoord(&Min, &Min, &m_matWorld);
	//D3DXVec3TransformCoord(&Max, &Max, &m_matWorld);
	//
	//
	//D3DXVECTOR3 v0(Min.x, Min.y, Min.z);
	//
	//D3DXVECTOR3 v1(Min.x, Min.y, Max.z);
	//D3DXVECTOR3 v2(Max.x, Min.y, Min.z);
	//D3DXVECTOR3 v3(Min.x, Max.y, Min.z);
	//
	//v1 -= v0;
	//v2 -= v0;
	//v3 -= v0;
	//
	//D3DXVec3Normalize(&obb.vAxisDir[0], &v1);
	//D3DXVec3Normalize(&obb.vAxisDir[1], &v2);
	//D3DXVec3Normalize(&obb.vAxisDir[2], &v3);
	//
	//obb.fAxisLen[0] = D3DXVec3Length(&v1) / 2;
	//obb.fAxisLen[1] = D3DXVec3Length(&v2) / 2;
	//obb.fAxisLen[2] = D3DXVec3Length(&v3) / 2;
	//
	//obb.vCenterPos = center;
	//
	//return obb;

}
