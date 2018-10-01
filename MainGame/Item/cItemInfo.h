#pragma once

class cUIObject;
class cUIImageView;
class cUITextView;


class cItemInfo
{
private:
		
	// UI
	LPD3DXSPRITE		m_pSprite;			// 아이템 2d 아이콘
	LPDIRECT3DTEXTURE9	m_pUITexture;		// 
	D3DXIMAGE_INFO		m_stImageInfo;		// 

	cUIImageView* m_pImage;

	

	SYNTHESIZE(cUIObject*, m_pUIRoot, UIRoot);
	SYNTHESIZE(cUIImageView*, m_pUIImage, UIImage);
	SYNTHESIZE(POINT, m_ptDragSize, DragSize);
	SYNTHESIZE(int, m_nCurrentSlotNum, CurrentSlotNum);
	SYNTHESIZE(int, m_nAbilityValue, AbilityValue);
	SYNTHESIZE(tagItemKindAndETC, m_stItemKind, ItemKind);
	SYNTHESIZE(const char*, m_szName, Name);
	SYNTHESIZE(const char*, m_szAbilityKind, szAbilityKind);
	SYNTHESIZE(const char*, m_szExplain, Explain);
	SYNTHESIZE(int, m_nBuyPrice, BuyPrice);
	SYNTHESIZE(int, m_nSalePrice, SalePrice);
	SYNTHESIZE(int, m_nQuickSlotNum, QuickSlotNum);
	SYNTHESIZE(int, m_nPotionCount, PotionCount);
	


	//텍스트관련
	SYNTHESIZE(cUITextView*, m_pText, Text);
		//변수일때
	SYNTHESIZE(int, m_nIdentify, identifyNUM);
		//상수일때
	SYNTHESIZE(const char*, m_szIdentify, IdentifyCHAR);
	SYNTHESIZE(const char*, m_szParentName, ParentName);


public:
	cItemInfo();
	virtual~cItemInfo();

	void Setup(tagTextPack* _tagTextPack, tagItemInfo* _item = NULL);
	void Update();
	void Render();

	void ConnectNode(cUIObject* pParent);
	void TransPos(D3DXVECTOR3 vec3Pos);
	void TransferCopyHere(cUIObject * ObjectOfTheTargetVector);

};

