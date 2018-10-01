#include "stdafx.h"
#include "cDropItem.h"

#include "MainGame\cGameObject.h"

#include "MainGame\Map\iMap.h"
#include "MainGame\Item\cItem.h"

#include "MainGame\Item/cItemInfo.h"


cDropItem::cDropItem()
{
}


cDropItem::~cDropItem()
{
}

void cDropItem::Setup(const char * name, D3DXVECTOR3 pos)
{
	m_pTexture = TEXTURE->GetTexture("Resource/Texture/Item/box.jpg");
	m_sName = name;
	m_vPosition = pos;

	m_isUse = true;
	// 바디 생성
	m_body = new cCube;
	m_body->Setup(D3DXVECTOR3(-10, -10, -10), D3DXVECTOR3(10, 10, 10));
	// 구 충돌 영역 생성
	m_pSpere = new cSpere;
	m_pSpere->Setup(D3DXVECTOR3(0, 0, 0), 20);

	m_fOverLand = 20.f;
	
	SetMaterial();
}

void cDropItem::Update()
{
	if (!m_isUse) return;

	PickUp();
	PlusMapHeight(); // 높이맵 적용

	m_fRotY += 0.003f;
	if (m_fRotY > D3DX_PI * 2.0f) m_fRotY -= (D3DX_PI*2.0f);

	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y + m_fOverLand, m_vPosition.z);

	m_matWorld = matR * matT;
	m_pSpere->SetWorld(m_matWorld);


	cGameObject::Update();
}

void cDropItem::Render()
{
	if (!m_isUse) return;

	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
	DEVICE->SetTexture(0, m_pTexture);
	DEVICE->SetMaterial(&m_stMtl);
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	m_body->PTRender();

	cGameObject::Render();
}

void cDropItem::PlusMapHeight()
{
	if (m_pMap)
		m_pMap->GetHeight(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cDropItem::SetMaterial()
{
}

void cDropItem::SetItemInfo(const char * name)
{
	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f); // 환경광
	m_stMtl.Diffuse = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

void cDropItem::PickUp()
{
	float distance = D3DXVec3Length(&(*g_vPlayerPos - m_vPosition));

	if (distance < 20)
	{
		if (KEY->IsOnceKeyDown('F'))
		{
			if (m_sName == "검은마력의옷")
			{
				ITEMMANAGER->AddInvenItem("검은마력의옷", "Resource/Texture/ItemIcon/BlackClothes.png", ARMOR, 75, 3600);
				m_isUse = false;
			}
			else if (m_sName == "검은마력의신발")
			{
				ITEMMANAGER->AddInvenItem("검은마력의신발", "Resource/Texture/ItemIcon/BlackShoes.png", SHOES, 25, 1400);
				m_isUse = false;
			}
			else if (m_sName == "검은마력의장갑")
			{
				ITEMMANAGER->AddInvenItem("검은마력의장갑", "Resource/Texture/ItemIcon/BlackGlove.png", GLOVES, 20, 1100);
				m_isUse = false;
			}
			else if (m_sName == "아이스소드")
			{
				ITEMMANAGER->AddInvenItem("아이스소드", "Resource/Texture/ItemIcon/TwohandIce.png", WEAPON, 150, 4000);
				m_isUse = false;
			}
		}
	}
}