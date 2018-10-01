#include "stdafx.h"
#include "cItemBase.h"


cItemBase::cItemBase()
	: m_sName(NULL)
	, m_pBody(NULL)
	, m_nIndex(0)
	, m_fTime(0.0f)
	, m_pSlashEffectTexture(NULL)
	, m_vOnPos(0, 0, 0)
	, m_vDownPos(0, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);

	ST_PCT_VERTEX v;
	//v.p = D3DXVECTOR3(0, 0, 0);
	//D3DCOLOR color(0x880000ff);
	//v.c = color;
	//
	for (int i = 0; i < 10; i++)
		m_arrPos[i].c = D3DCOLOR_ARGB(50, 255, 255, 255);

}


cItemBase::~cItemBase()
{
	SAFE_DELETE(m_sName);
	SAFE_DELETE(m_pBody);
}

void cItemBase::Setup()
{
}

void cItemBase::Update()
{
	m_pBoundingBox->SetWorld(m_matWorld);

	m_fTime += TIME->GetEllapsedTime();
	if (m_fTime > 0.00005)
	{
		m_fTime = 0.0f;

		D3DXVec3TransformCoord(&m_arrPos[m_nIndex].p, &m_vOnPos, &m_matWorld);
		m_nIndex = (m_nIndex + 1) % 20;
		D3DXVec3TransformCoord(&m_arrPos[m_nIndex].p, &m_vDownPos, &m_matWorld);
		m_nIndex = (m_nIndex + 1) % 20;

		m_vecVertex.clear();

		D3DXVECTOR3	v;

		for (int i = 0; i < 18; i += 2)
		{
		m_arrPos[i + 0].t = D3DXVECTOR2(0, 0);
		m_vecVertex.push_back(m_arrPos[i + 0]);
		m_arrPos[i + 2].t = D3DXVECTOR2(0, 1);
		m_vecVertex.push_back(m_arrPos[i + 2]);
		m_arrPos[i + 1].t = D3DXVECTOR2(1, 0);
		m_vecVertex.push_back(m_arrPos[i + 1]);

		m_arrPos[i + 2].t = D3DXVECTOR2(0, 1);
		m_vecVertex.push_back(m_arrPos[i + 2]);
		m_arrPos[i + 3].t = D3DXVECTOR2(1, 1);
		m_vecVertex.push_back(m_arrPos[i + 3]);
		m_arrPos[i + 1].t = D3DXVECTOR2(1, 0);
		m_vecVertex.push_back(m_arrPos[i + 1]);
		}

		//D3DXVec3CatmullRom(
		//	D3DXVECTOR3 *pOut,
		//	CONST D3DXVECTOR3 *pV0,
		//	CONST D3DXVECTOR3 *pV1,
		//	CONST D3DXVECTOR3 *pV2,
		//	CONST D3DXVECTOR3 *pV3,
		//	FLOAT s
		//);
		//
		
		D3DXVECTOR3 UpBefore;
		D3DXVECTOR3 UpNext;
		D3DXVECTOR3 UpP1;
		D3DXVECTOR3 UpP2;

		D3DXVECTOR3 DownBefore;
		D3DXVECTOR3 DownNext;
		D3DXVECTOR3 DownP1;
		D3DXVECTOR3 DownP2;

		for (int i = 0; i < 20; i += 2)
		{
			// 윗 동선
			if (i == 0)
			{
				UpBefore = D3DXVECTOR3(0, 0, 0);
				DownBefore = D3DXVECTOR3(0, 0, 0);
			}
			else
			{
				UpBefore = m_arrPos[i - 2].p;
				DownBefore = m_arrPos[i - 1].p;
			}

			if (i == 18)
			{
				UpNext = D3DXVECTOR3(0, 0, 0);
				DownBefore = D3DXVECTOR3(0, 0, 0);
			}
			else
			{
				UpNext = m_arrPos[i + 4].p;
				DownBefore = m_arrPos[i + 5].p;
			}

			UpP1 = m_arrPos[i + 2].p;
			UpP2 = m_arrPos[i + 3].p;

			DownP1 = m_arrPos[i + 2].p;
			DownP2 = m_arrPos[i + 3].p;

			D3DXVECTOR3 StartUp = UpP1;
			D3DXVECTOR3	StartDown = DownP1;

			ST_PCT_VERTEX v;
			v.c = D3DCOLOR_ARGB(125, 255, 255, 255);
			for (float i = 0.0f; i <= 1.0f; i += 0.1f)
			{
			D3DXVECTOR3 pUp;
			D3DXVECTOR3 pDwon;

			D3DXVec3CatmullRom(&pUp, &UpBefore, &UpP1, &UpP2, &UpNext, i);
			D3DXVec3CatmullRom(&pDwon, &UpBefore, &UpP1, &UpP2, &UpNext, i);

			v.p = StartUp;
			v.t = D3DXVECTOR2(0, 0);
			m_vecVertex.push_back(v);

			v.p = pUp;
			v.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(v);

			v.p = StartDown;
			v.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(v);

			v.p = pUp;
			v.t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(v);

			v.p = pDwon;
			v.t = D3DXVECTOR2(1, 1);
			m_vecVertex.push_back(v);

			v.p = StartDown;
			v.t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(v);

			StartDown = pDwon;
			StartUp = pUp;
			}
		}
		
	}

	cGameObject::Update();
}

void cItemBase::Render()
{

	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pBody->Render();

	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DEVICE->SetTransform(D3DTS_WORLD, &mat);
	if (m_vecVertex.size() != 0)
	{
		// 알파
		// 텍스처 알파 옵션 셋팅
		DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);



		// 알파블랜딩 방식 결정
		DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		//g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		//g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		DEVICE->SetRenderState(D3DRS_LIGHTING, false);
		DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

		DEVICE->SetTexture(0, m_pSlashEffectTexture);
		DEVICE->SetRenderState(D3DRS_LIGHTING, false);
		DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		DEVICE->SetFVF(ST_PCT_VERTEX::FVF);
		//g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		DEVICE->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_vecVertex.size() / 3,
			&m_vecVertex[0],
			sizeof(ST_PCT_VERTEX));
		DEVICE->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		DEVICE->SetTexture(0, NULL);

		DEVICE->SetRenderState(D3DRS_LIGHTING, true);
		DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
		DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);
		//g_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	}

	cGameObject::Render();
}
