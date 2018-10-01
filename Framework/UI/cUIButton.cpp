#include "stdafx.h"
#include "cUIButton.h"


cUIButton::cUIButton()
	: m_buttonState(BT_UP)
	, m_pDelegate(NULL)
	, m_isButton(true)
	, m_nAlpha(UI->GetAlpha())
{
}


cUIButton::~cUIButton()
{


	//SAFE_DELETE_ARRAY(m_arrTexture[BT_STATE_COUNT]);
}

void cUIButton::SetTexture(const char * szUp, const char * szOver, const char * szDown)
{

	
	D3DXIMAGE_INFO stImageInfo;

	m_enClickState = UNMOVABLE;
	
	m_arrTexture[BT_UP] = TEXTURE->GetTexture(szUp,&stImageInfo);
	m_stSize.fHeight = stImageInfo.Height;
	m_stSize.fWidth  = stImageInfo.Width;

	m_arrTexture[BT_OVER] = TEXTURE->GetTexture(szOver, &stImageInfo);
	m_arrTexture[BT_DOWN] = TEXTURE->GetTexture(szDown, &stImageInfo);

	

}

void cUIButton::Update(ST_UI_SIZE dragSize)
{
	cUIObject::Update();

	SetRect(&m_CollisionRectBT,
		m_matWorld._41,
		m_matWorld._42,
		m_matWorld._41 + m_stSize.fWidth,
		m_matWorld._42 + m_stSize.fHeight);

	if (PtInRect(&m_CollisionRectBT, ptMouse))
	{

		if (KEY->IsOnceKeyDown(VK_LBUTTON))
		{
			if (m_buttonState == BT_OVER)
				m_buttonState = BT_DOWN;
		}
		else
		{
			if (BT_DOWN == m_buttonState)
			{
				//if(m_szCallPlace == "Inventory")
			}
			m_buttonState = BT_OVER;
		}
	}
	else
	{

		m_buttonState = BT_UP;
	}

	//D3DXMatrixIdentity(&m_matWorld);
	//m_matWorld._41 = m_vPosition.x;
	//m_matWorld._42 = m_vPosition.y;
	//m_matWorld._43 = m_vPosition.z;


	/*m_move.x = 0;
	m_move.y = 0;
	m_beforeMousePT = { 0, 0 };*/

}

void cUIButton::Render(LPD3DXSPRITE pSprite)
{
	// 스프라이트 그리기
	pSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	// 월드 매트릭스로 출력할 위치를 지정
	pSprite->SetTransform(&m_matWorld);
	// 출력할 렉트 생성
	RECT rc;
	SetRect(&rc, 0, 0, m_stSize.fWidth, m_stSize.fHeight); // 시작점과 너비
														   // 드로우
	
	pSprite->Draw
	(		
		m_arrTexture[m_buttonState],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_ARGB(m_nAlpha, 255, 255, 255)
	);

	pSprite->End();

	cUIObject::Render(pSprite);

	
}






