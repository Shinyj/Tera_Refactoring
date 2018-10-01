#include "stdafx.h"
#include "cGameObject.h"

cGameObject::cGameObject()
	: m_pSpere(NULL)
	, m_pBoundingBox(NULL)
	, SightBox(false)
	, SightSpere(false)
	, m_isPossibleDamaged(true)
	, m_isPicked(false)
	, m_pRimLight(NULL)
{
}


cGameObject::~cGameObject()
{
	SAFE_DELETE(m_pSpere);
	SAFE_DELETE(m_pBoundingBox);

	SAFE_RELEASE(m_pRimLight);

}

void cGameObject::Setup()
{
	m_eCondition = CDT_NORMAL;
}

void cGameObject::Update()
{
	if (KEY->IsOnceKeyDown('H'))
		SightBox = !SightBox;
	if (KEY->IsOnceKeyDown('G'))
		SightSpere = !SightSpere;
}

void cGameObject::Render()
{
	// 바운딩 박스 렌더
	if (SightBox && m_pBoundingBox)
		m_pBoundingBox->Render();

	if (SightSpere && m_pSpere)
	{
		DEVICE->SetTexture(0, NULL);
		m_pSpere->Render();
	}
}

void cGameObject::Damaged(float damage, D3DXVECTOR3 pos)
{
}

void cGameObject::Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per)
{

}

void cGameObject::Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per, DAMAGED_TYPE type)
{
}

void cGameObject::CountPossibleDamaged(float time)
{
	m_PossbleDamagedTime += TIME->GetEllapsedTime();
	if (m_PossbleDamagedTime >= time)
	{
		m_isPossibleDamaged = true;
		m_PossbleDamagedTime = 0.0f;
	}
}


void cGameObject::RimLightSetup(float start, float end, float multiplier,
	float RimColor, float LightDirection, float lightColor, float AmbientColor)
{
	D3DXMATRIX   matView, matWorld, matProj;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixIdentity(&matWorld);
	m_pRimLight->SetMatrix("WorldMatrix", &matWorld);
	m_pRimLight->SetMatrix("ViewMatrix", &matView);
	m_pRimLight->SetMatrix("ProjectionMatrix", &matProj);

	D3DXVECTOR3 v = CAMERA->GetCameraWorld();
	D3DXVECTOR3 camera = CAMERA->GetCameraLocal();
	
	m_pRimLight->SetFloat("Rim_Start", 0.5f);
	m_pRimLight->SetFloat("Rim_End", 1.00f);
	m_pRimLight->SetFloat("Rim_Multiplier", 1.00f);
	m_pRimLight->SetVector("Rim_Color", &D3DXVECTOR4(1, 0, 0, 1));//1, 0.6, 0.16, 1
	m_pRimLight->SetVector("LightDirection", &D3DXVECTOR4(-camera.x,0, -camera.z, 0.0f));
	m_pRimLight->SetVector("LightColor", &D3DXVECTOR4(0.9f, 0.9f, 0.9f, 1.0f));
	m_pRimLight->SetVector("AmbientColor", &D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f));
	m_pRimLight->SetVector("ViewI", &D3DXVECTOR4(v.x, v.y, v.z, 1));
	m_pRimLight->SetFloat("Alpha", 1.0f);
}

void cGameObject::RimLightSetup(D3DXVECTOR4 color, float offset, float Alpha)
{
	D3DXMATRIX   matView, matWorld, matProj;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixIdentity(&matWorld);
	m_pRimLight->SetMatrix("WorldMatrix", &matWorld);
	m_pRimLight->SetMatrix("ViewMatrix", &matView);
	m_pRimLight->SetMatrix("ProjectionMatrix", &matProj);

	D3DXVECTOR3 v = CAMERA->GetCameraWorld();
	D3DXVECTOR3 camera = CAMERA->GetCameraLocal();
	

	m_pRimLight->SetFloat("Rim_Start", 0.5f);
	m_pRimLight->SetFloat("Rim_End", 1.00f);
	m_pRimLight->SetFloat("Rim_Multiplier", 1.00f);
	m_pRimLight->SetVector("Rim_Color", &color);
	m_pRimLight->SetVector("LightDirection", &D3DXVECTOR4(-camera.x, 0, -camera.z, 0.0f));
	m_pRimLight->SetVector("LightColor", &D3DXVECTOR4(0.9f, 0.9f, 0.9f, 1.0f));
	m_pRimLight->SetVector("AmbientColor", &D3DXVECTOR4(0.4f, 0.4f, 0.4f, 1.0f));
	m_pRimLight->SetVector("ViewI", &D3DXVECTOR4(v.x, v.y, v.z, 1));
	m_pRimLight->SetFloat("Alpha", Alpha);
	m_pRimLight->SetFloat("Offset", offset);
}
