#pragma once
#include "cUIObject.h"

enum enCloseCallPlace
{
	INVENTORYCLOSE,
	SHOPCLOSE,
	STATUSCLOSE

};



class iButtonDelegate
{
public :
	virtual void OnClick(cUIButton* pSender) = 0;// 누가 호출을 하였는가?
};

class cUIButton : public cUIObject
{
protected :
	enum eButtonSate
	{
		BT_UP,
		BT_OVER,
		BT_DOWN,
		BT_STATE_COUNT
	};

	LPDIRECT3DTEXTURE9		m_arrTexture[BT_STATE_COUNT];
	eButtonSate				m_buttonState;

	RECT m_CollisionRectBT;

	iButtonDelegate * m_pDelegate;
	SYNTHESIZE(int, m_nAlpha, Alpha);

	SYNTHESIZE(bool, m_isButton, Button);

	SYNTHESIZE(const char*, m_szCallPlace, CallPlace);
	//SYNTHESIZE(enCloseCallPlace, m_szCallPlace, CallPlace);

	HDC hdc;

public:
	cUIButton();
	virtual~cUIButton();

	virtual void SetTexture(const char * szUp,const char * szOver, const char * szDown );
	virtual void Update(ST_UI_SIZE dragSize = { 1,1 });
	virtual void Render(LPD3DXSPRITE pSprite);

		
};

