#include "stdafx.h"
#include "NpcShop.h"
#include "MainGame\Map\iMap.h"

NpcShop::NpcShop()
	: m_pPoalong(NULL)
	, m_currState(IDLE)
	, m_pMap(NULL)
	, m_fRotY(0.0f)
	, m_vDirection(-1, 0, 0)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_state = IDLE;

	m_fCurAnimTime = 0.0f;
	m_fTime = 0.0f;

	//처음에 얘로 셋팅해놓는다.
	//m_vWhere_To_Stay = D3DXVECTOR3(1247, 0, 1578);

	m_fAnimTime = 80.0f / 30.0f;
}

NpcShop::~NpcShop()
{
	SAFE_DELETE(m_pPoalong);
}

void NpcShop::Setup(D3DXVECTOR3 v)
{
	m_vWhere_To_Stay = v;
	m_pPoalong = new cSkinnedMesh;
	m_pPoalong->Setup("Resource/XFile/NPC","Poalong.X");
	m_pPoalong->SetAnimationIndexBlend(m_currState);

	// 위치를 가진 루트 본
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pPoalong->GetFrame(),
		"Dummy_root");

	// 구 생성(말걸수 있는 Zone 생성)
	m_pSpere = new cSpere;
	m_pSpere->Setup(m_vWhere_To_Stay, 40);

	m_pRimLight = Shader::LoadShader(ShaderPath + L"RimFlash.fx");
	m_pRimLight->SetFloat("Offset", 0.2);
}

void NpcShop::Update()
{/*D3DXVECTOR2 temp1;
	temp1.x = m_vWhere_To_Stay.x - g_vPlayerPos->x;
	temp1.y = m_vWhere_To_Stay.z - g_vPlayerPos->z;
	float DialogZone = D3DXVec2Length(&temp1);
	*/
	DialogZone = DistanceXZ(m_vWhere_To_Stay, *g_vPlayerPos);

	m_pDummyRoot->TransformationMatrix._42 = m_matWorld._42;
	m_pPoalong->Update(m_matWorld);

	D3DXMATRIX mat, matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vWhere_To_Stay.x, m_vWhere_To_Stay.y, m_vWhere_To_Stay.z);


	m_matWorld = matR * matT;
	//m_pBoundingBox->SetWorld(m_matWorld);
	m_pSpere->SetWorld(m_matWorld);

	PlusMapHeight();

	cGameObject::Update();

	///////////////////////////상점처리///////////////////////////////

	if (!UI->GetIsCallConShop() && KEY->IsOnceKeyDown('F') && DialogZone < 40)
	{
		// 창 키면 플레이어를 쳐다본다.
		{
			D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
			D3DXVECTOR3 v;
			D3DXVec3Normalize(&v, &(*g_vPlayerPos - m_vWhere_To_Stay));

			m_fCosVal = D3DXVec3Dot(&v, &u);
			m_fCosVal = acosf(m_fCosVal);

			if (m_vWhere_To_Stay.z < g_vPlayerPos->z)
				m_fCosVal = D3DX_PI * 2 - m_fCosVal;
		}

		SOUND->Play("Poarong_DialogStart");

		UI->SetIsCallConShop(true);
		isOptionMode = true;
		CAMERA->SetType(CAMERA_FIX);
		SOUND->Play("OpenInterface");
	}
	else if (UI->GetIsCallConShop() && KEY->IsOnceKeyDown('F') && DialogZone < 40)
	{
		SOUND->Play("Poarong_DialogEnd");
		//창을 끄면 다시 원래 쳐다보던 곳으로.
		{
			m_fCosVal = 0.0f;
			m_vDirection = D3DXVECTOR3(-1, 0, 0);
		}
		UI->SetIsCallConShop(false);
		SOUND->Play("CloseInterface");
	}
	//////////////////////////////////////////////////////////////////

	m_fRotY = m_fCosVal;
}

void NpcShop::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DEVICE->SetTransform(D3DTS_WORLD, &mat);

	RimLightSetup(0, 0, 0, 0, 0, 0, 0);

	if (DialogZone < 40)
	{
		m_pPoalong->Render(NULL);
		m_pPoalong->Render(NULL, m_pRimLight);
	}
	else
		m_pPoalong->Render(NULL);

	cGameObject::Render();
}

void NpcShop::PlusMapHeight()
{
	if (m_pMap)
		m_pMap->GetHeight(m_vWhere_To_Stay.x, m_vWhere_To_Stay.y, m_vWhere_To_Stay.z);
}
