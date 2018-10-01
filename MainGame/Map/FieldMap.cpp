#include "stdafx.h"
#include "FieldMap.h"


FieldMap::FieldMap()
	: m_pGround(NULL)
	, m_pVertex(NULL)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXVECTOR3 pos(15000, 0, -1500);
	D3DXMatrixTranslation(&m_matWorld, pos.x, pos.y, pos.z);
}

FieldMap::~FieldMap()
{
}


void FieldMap::Setup()
{
	m_pGround = STATICMESH->GetStaticMesh(
		"Resource/XFile/Map/Field/ANC_B_4858_SL(Ori).X");
	//"XFile/Item/Weapon/TwoHandSword_00.X"
	//"XFile/Map/Field/ANC_B_4858_SL.X"
	numOfVertex = m_pGround->GetMesh()->GetNumVertices();

	numOfIndex = m_pGround->GetMesh()->GetNumFaces() * 3;

	LPDIRECT3DVERTEXBUFFER9 pVB;

	m_pGround->GetMesh()->GetVertexBuffer(&pVB);

	LPDIRECT3DINDEXBUFFER9	pIB;
	void* pIndex;
	m_pGround->GetMesh()->GetIndexBuffer(&pIB);
	pIB->Lock(0, sizeof(WORD) * numOfIndex, (void**)&pIndex, 0);

	m_pIndex = (WORD*)pIndex;

	pIB->Unlock();
	pIB->Release();


	void* pVertices;
	pVB->Lock(0, sizeof(ST_PNT_VERTEX) * numOfVertex, (void**)&pVertices, 0);
	m_pVertex = (ST_PNT_VERTEX*)pVertices;

	for (DWORD i = 0; i < numOfVertex; i++)
	{
		D3DXVec3TransformCoord(&m_pVertex[i].p, &m_pVertex[i].p, &m_matWorld);
		//D3DXVec3TransformNormal(&m_pVertex[i].n, &m_pVertex[i].n, &m_matWorld);
	}

	pVB->Unlock();
	pVB->Release();


	D3DXMatrixIdentity(&m_matWorld);
}
void FieldMap::Update()
{

}
void FieldMap::Render()
{
	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pGround->Render();
}

bool FieldMap::GetHeight(IN float x, OUT float& y, IN float z)
{
	D3DXVECTOR3 vRayPos(x, 1000.0f, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (int i = 0; i <numOfIndex; i += 3)
	{
		float u, v, f;
		if (D3DXIntersectTri(&m_pVertex[m_pIndex[i + 0]].p,
			&m_pVertex[m_pIndex[i + 1]].p,
			&m_pVertex[m_pIndex[i + 2]].p,
			&vRayPos,
			&vRayDir,
			&u, &v, &f))
		{
			y = 1000.0f - f;
			return true;
		}
	}
	return false;
}
void FieldMap::IntersectTri(OUT D3DXVECTOR3 & vPickedPosition, OUT float & ray_dis)
{
	//cRay_toCube ray;
	//float distMax = 100000.0f; // ray 최대거리
	//
	//cRay_toCube r = ray.RayAtWorldSpace(ptMouse.x, ptMouse.y);
	//
	//for (UINT i = 0; i <numOfIndex; i += 3)
	//{
	//	float u, v, f;
	//	r.IntersectTri(m_pVertex[m_pIndex[i + 0]].p,
	//		m_pVertex[m_pIndex[i + 1]].p,
	//		m_pVertex[m_pIndex[i + 2]].p,
	//		vPickedPosition,
	//		ray_dis);
	//}
}

//몬스터 경사로 지점에서의 기울기 처리
void FieldMap::GetTerrainSlope(OUT D3DXMATRIX& out, IN float x, IN float z)
{
	//1. 지형 노멀 벡터 구하기
	D3DXVECTOR3& vec = GetNormal(x, z);

	//2. 지형 노멀 벡터와 Up 벡터의 외적으로 회전축 벡터 구하기
	D3DXVECTOR3 rAxis, up = D3DXVECTOR3(0, 1, 0);

	D3DXVec3Cross(&rAxis, &vec, &up);


	//3. 지형 노멀 벡터와 Up 벡터의 내적으로 내적 회전값 구하기
	float dot = D3DXVec3Dot(&vec, &up);

	//4. 회전값을 회전각으로 변환
	float angle = acos(dot);


	//5. 회전축으로 회전각 만큼 행렬 구하기
	D3DXMatrixRotationAxis(&out, &rAxis, angle);
}
D3DXVECTOR3 FieldMap::GetNormal(IN float x, IN float z)
{
	D3DXVECTOR3 v1, v2, normalV;
	D3DXVECTOR3 vRayPos(x, 1000.0f, z);
	D3DXVECTOR3 vRayDir(0, -1, 0);

	for (int i = 0; i <numOfIndex; i += 3)
	{
		float u, v, f;
		if (D3DXIntersectTri(&m_pVertex[m_pIndex[i + 0]].p,
			&m_pVertex[m_pIndex[i + 1]].p,
			&m_pVertex[m_pIndex[i + 2]].p,
			&vRayPos,
			&vRayDir,
			&u, &v, &f))
		{
			v1 = m_pVertex[m_pIndex[i + 1]].p - m_pVertex[m_pIndex[i + 0]].p;
			v2 = m_pVertex[m_pIndex[i + 2]].p - m_pVertex[m_pIndex[i + 0]].p;

			//외적을 이용해서 지면의 법선벡터를 구한다. 만일 지면의 법선벡터가 아래로 향한다면 순서를 바꿔서 다시 구한다.
			D3DXVec3Cross(&normalV, &v1, &v2);
			if (normalV.y < 0)
				D3DXVec3Cross(&normalV, &v2, &v1);
			return normalV;
		}
	}
}