#include "stdafx.h"
#include "cItemInfo.h"
#include"Framework\UI\cUIObject.h"
#include"Framework\UI\cUIImageView.h"
#include"Framework\UI\cUITextView.h"


cItemInfo::cItemInfo()
	:m_nQuickSlotNum(INDEXNOTSET)
	, m_nPotionCount(1)
{
}


cItemInfo::~cItemInfo()
{
	SAFE_DELETE(m_pUIRoot);


	//m_pSprite->Release();
	//g_pD3DDevice->Release();
}

void cItemInfo::Setup(tagTextPack* _tagTextPack, tagItemInfo* _item)
{
	if (_tagTextPack != NULL)
	{
		m_pText = new cUITextView;
		m_pText->SetText(_tagTextPack);
		if (_tagTextPack->_textType == VARIABLEVALUE)m_nIdentify = _tagTextPack->_vTextIContents[0];
		if (_tagTextPack->_textType == CONSTCHAR)m_szIdentify = _tagTextPack->_vTextCContents[0];
		m_szParentName = _tagTextPack->_textParentName;
	}
	//텍스트팩이 없을땐 아이템이므로 아이템정보를 작성
	else
	{
		if (_item->_itemKind == NOTANITEM)
		{
			m_pUIImage = new cUIImageView;
			m_pUIImage->SetTexture(_item->_itemPath);
			m_pUIImage->SetPosition(D3DXVECTOR3(0, 0, 0));
			m_szName = _item->_itemName;
			m_szParentName = _item->_itemParentName;

			if(!_item->_itemParentName) m_pUIRoot = m_pUIImage;


		}
		else
		{
			m_pUIImage = new cUIImageView;
			m_pUIImage->SetTexture(_item->_itemPath);
			m_pUIImage->SetPosition(D3DXVECTOR3(0, 0, 0));
			m_szName = _item->_itemName;
			m_nBuyPrice = _item->_itemBuyPrice;
			m_nSalePrice = _item->_itemSalePrice;
			m_nAbilityValue = _item->_itemAbilityValue;
			m_stItemKind = _item->_itemKind;
			m_pUIRoot = m_pUIImage;
		}
	}
	
	// UI
	ZeroMemory(&m_stImageInfo, sizeof(D3DXIMAGE_INFO));
	D3DXCreateSprite(DEVICE, &m_pSprite);

}
void cItemInfo::Update()
{

	if (m_pUIRoot)	m_pUIRoot->Update();
	if (m_pUIImage)m_pUIImage->Update();

	
	
}
void cItemInfo::Render()
{

	if (m_pUIRoot)m_pUIRoot->Render(m_pSprite);
	if (m_pUIImage)m_pUIImage->Render(m_pSprite);

}

void cItemInfo::ConnectNode(cUIObject* pParent)
{
	if (m_pText)pParent->AddChild(m_pText);
	else if (m_pUIImage)pParent->AddChild(m_pUIImage);
}

void cItemInfo::TransPos(D3DXVECTOR3 vec3Pos)
{
	m_pUIImage->SetPosition(vec3Pos);
}

void cItemInfo::TransferCopyHere(cUIObject* ObjectOfTheTargetVector)
{
	
}




