#include "stdafx.h"
#include "cUITextView.h"


cUITextView::cUITextView()
{
}


cUITextView::~cUITextView()
{
	//SAFE_DELETE(m_pRect);
}

void cUITextView::SetText(tagTextPack* _tagTextPack)
{
	_enTextType = _tagTextPack->_textType;

	if (_tagTextPack->_textType == CONSTCHAR)
	{
		//문자열의 갯수
		m_vTextNumMax = _tagTextPack->_vTextCContents.size();
		//각 내용들
		m_vTextCContents = _tagTextPack->_vTextCContents;
	}

	else if (_tagTextPack->_textType == VARIABLEVALUE)
	{
		//문자열의 갯수
		m_vTextNumMax = _tagTextPack->_vTextIContents.size();
		//각 내용들
		m_vTextIContents = _tagTextPack->_vTextIContents;
	}

	//각각 어디위치할지
	m_vTextPos = _tagTextPack->_vTextPos;
	
	m_vTextSize = _tagTextPack->_vTextSize;
	m_vTextColor = _tagTextPack->_vTextColor;


}

void cUITextView::Render(LPD3DXSPRITE pSprite)
{


	char szTemp[1024];

	m_pRect = new RECT[m_vTextNumMax];



	for (int i = 0; i < m_vTextNumMax; i++)
	{
		//변수일때는 첫번째에 키값을 저장하기때문에 건너뛴다.
		if (_enTextType == VARIABLEVALUE && i == 0)continue;
		if (_enTextType == VARIABLEVALUE && m_vTextIContents[i] == 0) continue;

		if(m_vTextSize[i] == BIG)m_pFont = FONT->GetFont(FONT->FT_GA_BIG);
		else if (m_vTextSize[i] == MID)m_pFont = FONT->GetFont(FONT->FT_GA_MID);
		else if (m_vTextSize[i] == SMALL)m_pFont = FONT->GetFont(FONT->FT_GA_SMALL);
		


		//문자일땐 CContents를 변수일땐 IContents의 넣었던걸 꺼내쓴다.
		if (_enTextType == CONSTCHAR)sprintf_s(szTemp, "%s", m_vTextCContents[i]);

		if (_enTextType == VARIABLEVALUE)sprintf_s(szTemp, "%d", m_vTextIContents[i]);

		SetRect(&m_pRect[i], m_matWorld._41 + m_vTextPos[i].x
			, m_matWorld._42 + m_vTextPos[i].y
			, m_matWorld._41 + m_vTextPos[i].x + 400
			, m_matWorld._42 + m_vTextPos[i].y + 800);

		m_pFont->DrawTextA
			(
			  NULL, szTemp, strlen(szTemp), &m_pRect[i]
			, DT_LEFT | DT_TOP | DT_NOCLIP
			, D3DCOLOR_XRGB((int)m_vTextColor[i].x, (int)m_vTextColor[i].y, (int)m_vTextColor[i].z)
			);
		
	}


}


