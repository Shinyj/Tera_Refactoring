#pragma once
//인덱스가 존재하지않음
#define INDEXNOTSET -1
#define ZEROVALUE 0
#define NUMBERNOTSET -1

enum eTextSize
{
	BIG,
	MID,
	SMALL
};


enum enTextType
{
	CONSTCHAR,
	VARIABLEVALUE
};


struct tagText
{
	enTextType  Type;
	const char* szTextName;
	int			nIdentyfyNum;
	const char* szParrentName;
};



enum UIType
{
	WINDOW,
	BUTTON
};



struct tagUIInfo
{
	UIType		 _UIType;
	const char*	 _UIName;
	const char*	 _UIParentName;
	D3DXVECTOR3  _UIPos;
	const char*	 _UIPath1;
	const char*	 _UIPath2;
	const char*	 _UIPath3;
	D3DXVECTOR2	 _UIReduceDragSize;	
};



//미리작성한 텍스트라는뜻
typedef vector<const char*> vTextCContents;
typedef vector<int>			vTextIContents;
typedef vector<D3DXVECTOR3> vTextPos;
typedef vector<eTextSize>	vTextSize;
typedef vector<D3DXVECTOR3> vTextColor;

struct tagTextPack
{
	enTextType		_textType;
	const char*    _textParentName;
	vTextCContents  _vTextCContents;
	vTextIContents  _vTextIContents;
	vTextPos	   _vTextPos;
	vTextSize	   _vTextSize;
	vTextColor	   _vTextColor;
};


enum tagItemKindAndETC
{
	HELMET,
	ARMOR,
	WEAPON,
	GLOVES,
	SHOES,
	HPOTION,
	MPOTION,
	ETCCONSUMABLES,
	NOTANITEM,
	SKILLICON
};

struct tagItemInfo
{
	const char*			 _itemName;
	tagItemKindAndETC	 _itemKind;
	D3DXVECTOR3			 _itemPos;
	const char*			 _itemPath;
	int					 _itemAbilityValue;
	int					 _itemSalePrice;
	int					 _itemBuyPrice;
	const char*			 _itemExplain;
	const char*			 _itemParentName;
};









