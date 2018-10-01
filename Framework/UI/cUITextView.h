#pragma once
#include "Framework\UI\cUIObject.h"



class cUITextView : public cUIObject
{
protected :
	cFontManager::eFontType		m_fontType;
	string						m_sText;
	D3DCOLOR					m_dwColor;
	LPD3DXFONT					m_pFont;
	D3DXFONT_DESC				m_pFontDesc;

	
	// 정렬 방식
	enum TextSortType
	{
		TST_LEFT,
		TST_CENTER,
		TST_RIGHT
	};

	int			   m_vTextNumMax;

	SYNTHESIZE(vTextIContents, m_vTextIContents, TextIContents);
	SYNTHESIZE(vTextCContents, m_vTextCContents, TextCContents);
	//SYNTHESIZE(vector<int>, m_vTextValue, TextValue);



	vTextPos		m_vTextPos;
	vTextSize		m_vTextSize;
	vTextColor		m_vTextColor;

	enTextType		_enTextType;

	RECT*	m_pRect;


public:
	cUITextView();
	virtual~cUITextView();


	virtual void SetText(tagTextPack* _tagTextPack);
	virtual void Render(LPD3DXSPRITE pSprite);

};

