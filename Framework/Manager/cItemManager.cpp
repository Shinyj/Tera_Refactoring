#include "stdafx.h"
#include "cItemManager.h"
#include "MainGame\Item\cItem.h"
#include "Framework/UI/cUIObject.h"
#include "Framework/UI/cUIInfo.h"
#include "Framework/UI/cUIImageView.h"
#include "Framework/UI/cUIButton.h"
#include "Framework/UI/cUITextView.h"



cItemManager::cItemManager()
	:nItemNum(0)
	, isTemp(false)
	, m_vClickedIndex(0)
	, m_nGold(10000)
{

}


cItemManager::~cItemManager()
{

}

void cItemManager::Setup()
{
	UITextList();

	//슬롯만들기
	SetItemSlot(INVENTORYSLOT);
	SetItemSlot(CONSHOPSLOT);
	SetItemSlot(STATUSSLOT);
	//SetItemSlot(QUICKSLOT);

	//아이템목록에 있는 아이템 생성하기
	CreateItemList();



	ConnectNodeCommand();

	CreateImitationImage();

}

void cItemManager::Update()
{
	//슬롯을 가지고 있는 UI의 위치를 갱신하여
	//슬롯의 위치도 갱신시켜준다.
	UIPosRenewal("Inventory");
	ItemSlotPosRenewal(INVENTORYSLOT, m_vec3RenwalInvenPos);
	UIPosRenewal("ConShop");
	ItemSlotPosRenewal(CONSHOPSLOT, m_vec3RenwalShop);
	UIPosRenewal("Status");
	ItemSlotPosRenewal(STATUSSLOT, m_vec3RenwalStatus);

	//자주쓰는 UI의 충돌렉트를 등록해두고 필요할때마다 쓰는용도
	MainUIInfo();

	//드래그하는 동안의 예외설정
	ExceptionsWhileDragging();

	//정산할때 들어온 아이템들의 가격의 합만큼 계산해줌
	SalesItemCalculator();

	//아이템 설명창 업데이트
	ItemExplaneUpdate();

	//우클릭시 아이템의 반응
	ClickUseItem();

	//드래그앤드랍
	DragAndDrop();

	//슬롯 내 정렬
	SortInSlot();

	//퀵슬롯의 레프트, 탑 위치 갱신
	QuickSlotPosRenewal();

	//퀵슬롯 아이템이 퀵슬롯을 따라다니게 퀵슬롯중 지정된 번호로 계속 따라감
	QuickSlotItemPosRenewal();

	//각자 장소에 있는 아이템 모두 업데이트
	ItemUpdate();

	InventorySynchronize();

	ZeroPotionDelete();

	//char텍스트와 int텍스트 로드

	//ItemInfoCTextRenewal("퀵슬롯텍스트");

	if (isOptionMode)
	{



		ItemInfoCTextRenewal("아이템정보");

		for (int j = 1; j < 16; j++)
		{
			ItemInfoITextRenewal(j);
		}






	}

	//if (KEYMANAGER->IsOnceKeyDown(VK_NUMPAD3))ItemInfoITextRenewal(3);

	//QuickSlotSynchronize();


}

void cItemManager::Render()
{
	ItemRender();

	ItemExplaneRender();

	ImitationIconRender();

	for (int i = 0; i < m_vText.size(); i++)
	{
		m_vText[i]->Render();
	}


	/*if (m_vQuickItem.size() != 0)
	{*/
	//값 찍어보기
	//char szTemp[1024];
	////sprintf_s(szTemp, 1024,
	////	"인벤아이템갯수 : %d, \n 장비창아이템갯수 : %d, \n 샵아이템갯수 : %d, \n 퀵슬롯 아이콘갯수 : %d"
	////	, m_vInvenItem.size(), m_vStatusItem.size(), m_vConShopItem.size(), m_vQuickItem.size());// ,
	//	//(int)m_pVec3SlotPos[0].x, (int)m_pVec3SlotPos[0].y);
	////sprintf_s(szTemp, 1024, "퀵슬롯의 아이템의 포션갯수 = %d", m_vQuickItem[0]->GetPotionCount());
	//sprintf_s(szTemp, 1024, "int.size = %d", vInt.size());

	//RECT rc2;
	//SetRect(&rc2, 100, 200, 800, 400);
	//LPD3DXFONT pFont = FONTMANAGER->GetFont(cFontManager::FT_GA_BIG);
	//pFont->DrawTextA(NULL,
	//	szTemp,
	//	strlen(szTemp),
	//	&rc2,
	//	DT_LEFT | DT_TOP,
	//	D3DCOLOR_XRGB(255, 255, 0));
	//}


}

void cItemManager::Destroy()
{
	SAFE_DELETE(m_pItemTextInfo);

}

void cItemManager::CreateText(tagText _tagText)
{
	int TextDataIndex;
	//해당 키워드로 텍스트팩 내부를 뒤져서 인덱스번호를 리턴함
	if (_tagText.Type == CONSTCHAR) TextDataIndex = UICTextDataIndex(_tagText.szTextName);
	if (_tagText.Type == VARIABLEVALUE)TextDataIndex = UIITextDataIndex(_tagText.nIdentyfyNum);


	m_pItemTextInfo = new cItem;

	tagTextPack _tagTextPack;

	if (_tagText.Type == CONSTCHAR)
	{
		_tagTextPack._textType = _tagText.Type;
		_tagTextPack._textParentName = _tagText.szParrentName;
		_tagTextPack._vTextCContents = m_vPreTextCDataPack[TextDataIndex];
		_tagTextPack._vTextPos = m_vPreTextCPosPack[TextDataIndex];
		_tagTextPack._vTextSize = m_vPreTextCSizePack[TextDataIndex];
		_tagTextPack._vTextColor = m_vPreTextCColorPack[TextDataIndex];
	}

	if (_tagText.Type == VARIABLEVALUE)
	{
		_tagTextPack._textType = _tagText.Type;
		_tagTextPack._textParentName = _tagText.szParrentName;
		_tagTextPack._vTextIContents = m_vPreTextIDataPack[TextDataIndex];
		_tagTextPack._vTextPos = m_vPreTextIPosPack[TextDataIndex];
		_tagTextPack._vTextSize = m_vPreTextISizePack[TextDataIndex];
		_tagTextPack._vTextColor = m_vPreTextIColorPack[TextDataIndex];
	}



	m_pItemTextInfo->Setup(&_tagTextPack);
	m_vText.push_back(m_pItemTextInfo);

}

void cItemManager::CreateUICTextData(vTextCContents _vTextCContents, vTextPos _vTextPos, vTextSize _vTextSize, vTextColor _vTextColor)
{
	m_vPreTextCDataPack.push_back(_vTextCContents);
	m_vPreTextCPosPack.push_back(_vTextPos);
	m_vPreTextCSizePack.push_back(_vTextSize);
	m_vPreTextCColorPack.push_back(_vTextColor);
}

void cItemManager::CreateUIITextData(vTextIContents _vTextIContents, vTextPos _vTextPos, vTextSize _vTextSize, vTextColor _vTextColor)
{
	m_vPreTextIDataPack.push_back(_vTextIContents);
	m_vPreTextIPosPack.push_back(_vTextPos);
	m_vPreTextISizePack.push_back(_vTextSize);
	m_vPreTextIColorPack.push_back(_vTextColor);
}

int cItemManager::UICTextDataIndex(const char * titleName)
{
	int indexSave;
	for (int i = 0; i < m_vPreTextCDataPack.size(); )
	{
		if (GetPreTextCDataPack()[i].operator[](0) == titleName)
		{
			indexSave = i;
			return indexSave;
		}
		else
		{
			i++;
		}
	}
}

int cItemManager::UIITextDataIndex(int variableName)
{
	int indexSave;
	for (int i = 0; i < m_vPreTextIDataPack.size(); )
	{
		if (GetPreTextIDataPack()[i].operator[](0) == variableName)
		{
			indexSave = i;
			return indexSave;
		}
		else
		{
			i++;
		}
	}

}

void cItemManager::ItemInfoITextRenewal(int sequence)
{

	int index = UIITextDataIndex(sequence);

	//새로 만든 벡터를 데이터갯수에 맞게 재조정한다.
	//팩 벡터 내부의 백터들중에 첫 텍스트에 입력된 정보가 인트 sequence인 녀석을 찾음
	vInt.resize(m_vPreTextIDataPack[index].size());

	for (int i = 0; i < m_vText.size(); i++)
	{
		if (m_vText[i]->GetidentifyNUM() == sequence)
		{



			//0번은 식별자라 1번부터 작성하면된다.

			// 번호가 1일때 아이템툴팁 텍스트
			if (sequence == 1)
			{


				vInt[1] = FindAbilityValue();

				vInt[2] = FindSalePriceValue();
			}



			// 번호가 2번일때 인벤토리 소지금 텍스트
			else if (sequence == 2)
			{
				vInt[1] = m_nGold;

			}
			//스테이터스 텍스트
			else if (sequence == 3)
			{
				vInt[1] = m_nAttackValue;

				vInt[2] = m_nDefenceValue;

			}
			//순서이기도 하고 TEXT에 존재하는 번호이기도 함
			//TEXT하나는 여러곳에서 사용해도 포인터라 두곳에 존재할 수 없음
			else if (sequence == 4)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "하급회복물약");

			}
			else if (sequence == 5)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "중급회복물약");

			}
			else if (sequence == 6)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "상급회복물약");

			}
			else if (sequence == 7)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "하급마나물약");

			}
			else if (sequence == 8)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "중급마나물약");

			}
			else if (sequence == 9)
			{
				vInt[1] = FindPotionCount(m_vInvenItem, "상급마나물약");

			}
			else if (sequence == 10)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "하급회복물약");

			}
			else if (sequence == 11)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "중급회복물약");

			}
			else if (sequence == 12)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "상급회복물약");

			}
			else if (sequence == 13)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "하급마나물약");

			}
			else if (sequence == 14)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "중급마나물약");

			}
			else if (sequence == 15)
			{
				vInt[1] = FindPotionCount(m_vQuickItem, "상급마나물약");


			}

			m_vText[i]->GetText()->SetTextIContents(vInt);
		}

	}

}

void cItemManager::ItemInfoCTextRenewal(const char * szFindText)
{
	for (int i = 0; i < m_vText.size(); i++)
	{
		if (m_vText[i]->GetIdentifyCHAR() == szFindText)
		{
			vector<const char*> vChar;

			vChar.resize(m_vPreTextCDataPack[UICTextDataIndex(szFindText)].size());

			if (szFindText == "퀵슬롯텍스트")
			{
				for (int i = 0; i < m_vAllItem.size(); i++)
				{
					char szTemp[30];
					_itoa(m_vAllItem[i]->GetQuickSlotNum(), szTemp, 30);

					vChar[0] = szTemp;
				}
				m_vText[i]->GetText()->SetTextCContents(vChar);
			}
			else
			{
				for (int i = 0; i < m_vAllItem.size(); i++)
				{
					if (m_vAllItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
					{
						vChar[0] = m_vAllItem[i]->GetName();

						if (m_vAllItem[i]->GetItemKind() == ETCCONSUMABLES)
						{
							vChar[1] = "기능아이템";
						}

						else if (m_vAllItem[i]->GetItemKind() == HPOTION || m_vAllItem[i]->GetItemKind() == MPOTION)
						{
							vChar[1] = "회복력";
						}

						else if (m_vAllItem[i]->GetItemKind() == WEAPON)
						{
							vChar[1] = "공격력";
						}

						else if (m_vAllItem[i]->GetItemKind() == SKILLICON)
						{
							vChar[1] = "위력";
						}
						else
						{
							vChar[1] = "방어력";
						}

						vChar[2] = "설명";

						vChar[3] = textExplane.find(m_vAllItem[i]->GetName())->second;

						vChar[4] = FindItemPos();
					}
				}



			}


			m_vText[i]->GetText()->SetTextCContents(vChar);

		}
	}
}

int cItemManager::FindAbilityValue()
{
	for (int i = 0; i < m_vAllItem.size(); i++)
	{
		//부딪혔을때
		if (m_vAllItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{
			//부딪힌 녀석의 어빌리티값을 리턴한다.

			return m_vAllItem[i]->GetAbilityValue();
		}
	}
}

int cItemManager::FindItemExplaneWndIndex(const char* szItemExplaneName)
{
	for (int i = 0; i < m_vAllItem.size(); i++)
	{
		if (m_vAllItem[i]->GetName() == szItemExplaneName)
		{
			return i;
		}
	}
}


int cItemManager::FindSalePriceValue()
{
	for (int i = 0; i < m_vAllItem.size(); i++)
	{

		if (m_vAllItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{

			int shopIDX = _UI->FindUIRootIndex("ConShop");
			RECT itemRc = m_vAllItem[i]->GetUIRoot()->GetCollisionRect();
			RECT shopUIRc = _UI->GetVUI()[shopIDX]->GetUIRoot()->GetCollisionRect();
			RECT tempRc;

			RECT shopUIRcResize;
			SetRect(&shopUIRcResize,
				shopUIRc.left,
				shopUIRc.top,
				shopUIRc.right,
				shopUIRc.bottom * 3 / 4);

			if (IntersectRect(&tempRc, &itemRc, &shopUIRcResize))
			{
				return m_vAllItem[i]->GetBuyPrice();
			}
			else return m_vAllItem[i]->GetSalePrice();
		}
	}

}

const char * cItemManager::FindItemPos()
{
	for (int i = 0; i < m_vAllItem.size(); i++)
	{
		if (m_vAllItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{

			//if (m_vAllItem[i]->GetItemKind() != SKILLICON) return NULL;

			int shopIDX = _UI->FindUIRootIndex("ConShop");
			RECT shopUIRc = _UI->GetVUI()[shopIDX]->GetUIRoot()->GetCollisionRect();

			RECT shopUIRcResize;
			SetRect(&shopUIRcResize,
				shopUIRc.left,
				shopUIRc.top,
				shopUIRc.right,
				shopUIRc.bottom * 3 / 4);

			RECT itemRc = m_vAllItem[i]->GetUIRoot()->GetCollisionRect();

			RECT tempRc;


			if (_UI->GetIsCallConShop() && IntersectRect(&tempRc, &itemRc, &shopUIRcResize))
			{
				return "구입가격";
			}
			else if (m_vAllItem[i]->GetItemKind() == SKILLICON) return "습득비용";

			else return "상점매입가";
		}
	}

}

void cItemManager::ItemExplaneUpdate()
{

	RECT explaneRc = m_vEtcIcon[0]->GetUIRoot()->GetCollisionRect();
	m_nExpWidth = explaneRc.right - explaneRc.left;
	m_nExpHeight = explaneRc.bottom - explaneRc.top;

	if (ptMouse.x + m_nExpWidth> WINSIZEX - 100) m_nExpWidth = ptMouse.x + m_nExpWidth - (WINSIZEX - 100);
	else  m_nExpWidth = 0;

	if (ptMouse.y + m_nExpHeight > WINSIZEY - 100) m_nExpHeight = ptMouse.y + m_nExpHeight - (WINSIZEY - 100);
	else  m_nExpHeight = 0;



	for (int i = 0; i < m_vItemImitation.size(); i++)
	{
		m_vItemImitation[i]->Update();
		m_vItemImitation[i]->GetUIImage()->SetPosition(D3DXVECTOR3(ptMouse.x + 27 - m_nExpWidth, ptMouse.y + 35 - m_nExpHeight, 0));

	}

	//툴팁창
	m_vEtcIcon[0]->Update();
	m_vEtcIcon[0]->GetUIRoot()->SetPosition(D3DXVECTOR3(ptMouse.x + 20 - m_nExpWidth, ptMouse.y + 20 - m_nExpHeight, 0));
	m_vEtcIcon[1]->GetUIImage()->SetPosition(D3DXVECTOR3(110, 200, 0));
}

void cItemManager::CreateItem(const char* itemName, const char* filePath, tagItemKindAndETC itemType, int itemAbility, int itemSalePrice, vItem& vPlaceItem, const char* szParrentName)
{
	//새로운 iteminfo클래스 셋팅
	m_pItemInfo = new cItem;

	//iteminfo에 보내줄 내용의 형식
	tagItemInfo _tagItemInfo;

	_tagItemInfo._itemPath = filePath;
	_tagItemInfo._itemPos = D3DXVECTOR3(0, 0, 0);
	_tagItemInfo._itemName = itemName;
	_tagItemInfo._itemKind = itemType;
	_tagItemInfo._itemAbilityValue = itemAbility;
	_tagItemInfo._itemBuyPrice = itemSalePrice * 5;
	_tagItemInfo._itemSalePrice = itemSalePrice;
	_tagItemInfo._itemParentName = szParrentName;

	//아이템인포에 정보를 전달
	m_pItemInfo->Setup(NULL, &_tagItemInfo);

	//this클래스의 벡터에 포인터로 저장
	vPlaceItem.push_back(m_pItemInfo);

	//아이템의 종류가 아닐땐 관리벡터에 넣지 않는다.
	if (itemType != NOTANITEM)
	{

		m_vtagItemInfo.push_back(_tagItemInfo);

		//관리벡터에 넣음
		m_vAllItem.push_back(m_pItemInfo);

		nItemNum += 1;



	}


}

void cItemManager::SetItemSlot(eSlotType itemSlotType)
{

	if (itemSlotType == STATUSSLOT)
	{
		tagItemPos _tagItemPos;

		_tagItemPos.SetSlotType(itemSlotType);

		m_vec3StatusPos.push_back(D3DXVECTOR3(63, 184, 0));
		m_vec3StatusPos.push_back(D3DXVECTOR3(208, 86, 0));
		m_vec3StatusPos.push_back(D3DXVECTOR3(44, 86, 0));
		m_vec3StatusPos.push_back(D3DXVECTOR3(188, 184, 0));


		for (int i = 0; i < 4; i++)
		{
			_tagItemPos.vec3Pos = m_vec3StatusPos[i];
			SetRect(&_tagItemPos.rc, m_vec3StatusPos[i].x, m_vec3StatusPos[i].y

				, m_vec3StatusPos[i].x + 37, m_vec3StatusPos[i].y + 37);

			m_vStatusSlot.push_back(_tagItemPos);
		}
	}

	else
	{

		for (int x = 0; x < 7; x++)
		{
			for (int y = 0; y < 8;)
			{
				//아이템아이콘이 위치 정보 작성에 필요한 구조체
				tagItemPos _tagItemPos;

				_tagItemPos.SetSlotType(itemSlotType);

				//꼭짓점을 기준으로 렉트와 위치를 작성
				_tagItemPos.vec2Vertex.x = 18 + 43 * y;
				_tagItemPos.vec2Vertex.y = 79 + 42 * x;

				//UI에서 아이템 배치 간격을 UI에서의 슬롯 간격으로 미리 설정해둔다.
				_tagItemPos.vec2interval.x = 37;
				_tagItemPos.vec2interval.y = 37;

				//위치할곳
				_tagItemPos.vec3Pos = D3DXVECTOR3(_tagItemPos.vec2Vertex.x, _tagItemPos.vec2Vertex.y, 0);


				SetRect(&_tagItemPos.rc
					, _tagItemPos.vec2Vertex.x
					, _tagItemPos.vec2Vertex.y
					, _tagItemPos.vec2Vertex.x + _tagItemPos.vec2interval.x
					, _tagItemPos.vec2Vertex.y + _tagItemPos.vec2interval.y);



				//해당하는 벡터에 담아둔다
				if (itemSlotType == INVENTORYSLOT)
				{
					if (x < 5)
					{
						y += 1;
						m_vInvenSlot.push_back(_tagItemPos);
					}
					else break;

				}
				else if (itemSlotType == CONSHOPSLOT)
				{
					if (x < 4)
					{
						y += 4;
						m_vShopSlot.push_back(_tagItemPos);
					}
					//else if( x >= 4 && x <= 5) continue;

					else if (x > 4)
					{
						y += 1;
						m_vShopSlot.push_back(_tagItemPos);
					}
					else break;
				}

			}
		}

	}
}


void cItemManager::ItemSlotPosRenewal(eSlotType itemSlotType, D3DXVECTOR3 placePos)
{
	if (itemSlotType == INVENTORYSLOT)
	{
		for (int x = 0; x < 5; x++)
		{
			for (int y = 0; y < 8; y++)
			{

				m_vInvenSlot[8 * x + y].vec2Vertex.x = placePos.x + 18 + 43 * y;

				m_vInvenSlot[8 * x + y].vec2Vertex.y = placePos.y + 79 + 42 * x;

				m_vInvenSlot[8 * x + y].vec3Pos = D3DXVECTOR3(m_vInvenSlot[8 * x + y].vec2Vertex.x, m_vInvenSlot[8 * x + y].vec2Vertex.y, 0);

				SetRect(&m_vInvenSlot[8 * x + y].rc
					, m_vInvenSlot[8 * x + y].vec2Vertex.x
					, m_vInvenSlot[8 * x + y].vec2Vertex.y
					, m_vInvenSlot[8 * x + y].vec2Vertex.x + m_vInvenSlot[8 * x + y].vec2interval.x
					, m_vInvenSlot[8 * x + y].vec2Vertex.y + m_vInvenSlot[8 * x + y].vec2interval.y);


			}
		}
	}

	else if (itemSlotType == CONSHOPSLOT)
	{
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 2; y++)
			{

				m_vShopSlot[2 * x + y].vec2Vertex.x = placePos.x + 18 + 43 * y * 4;

				m_vShopSlot[2 * x + y].vec2Vertex.y = placePos.y + 79 + 42 * x;

				m_vShopSlot[2 * x + y].vec3Pos = D3DXVECTOR3(m_vShopSlot[2 * x + y].vec2Vertex.x, m_vShopSlot[2 * x + y].vec2Vertex.y, 0);

				SetRect(&m_vShopSlot[2 * x + y].rc
					, m_vShopSlot[2 * x + y].vec2Vertex.x
					, m_vShopSlot[2 * x + y].vec2Vertex.y
					, m_vShopSlot[2 * x + y].vec2Vertex.x + m_vShopSlot[2 * x + y].vec2interval.x
					, m_vShopSlot[2 * x + y].vec2Vertex.y + m_vShopSlot[2 * x + y].vec2interval.y);


			}
		}
		for (int x = 5; x < 7; x++)
		{
			for (int y = 0; y < 8; y++)
			{


				//m_vShopSlot[8 * x + y + 8].vec3Pos = D3DXVECTOR3(100, 100, 0);


				m_vShopSlot[8 * x + y - 32].vec2Vertex.x = placePos.x + 18 + 43 * y;
				m_vShopSlot[8 * x + y - 32].vec2Vertex.y = placePos.y + 79 + 42 * x;
				m_vShopSlot[8 * x + y - 32].vec3Pos =
					D3DXVECTOR3(m_vShopSlot[8 * x + y - 32].vec2Vertex.x
						, m_vShopSlot[8 * x + y - 32].vec2Vertex.y
						, 0);

				SetRect(&m_vShopSlot[8 * x + y - 32].rc
					, m_vShopSlot[8 * x + y - 32].vec2Vertex.x
					, m_vShopSlot[8 * x + y - 32].vec2Vertex.y
					, m_vShopSlot[8 * x + y - 32].vec2Vertex.x + m_vShopSlot[8 * x + y - 32].vec2interval.x
					, m_vShopSlot[8 * x + y - 32].vec2Vertex.y + m_vShopSlot[8 * x + y - 32].vec2interval.y);

			}
		}
	}
	else if (itemSlotType == STATUSSLOT)
	{
		for (int i = 0; i < 4; i++)
		{
			//버텍스는 포즈에서 z만 없는거
			m_vStatusSlot[i].vec3Pos = m_vec3StatusPos[i] + placePos;

			SetRect(&m_vStatusSlot[i].rc,
				m_vStatusSlot[i].vec3Pos.x,
				m_vStatusSlot[i].vec3Pos.y,
				m_vStatusSlot[i].vec3Pos.x + 37,
				m_vStatusSlot[i].vec3Pos.y + 37);
		}

	}
}

void cItemManager::DragAndDrop()
{

	if (KEY->IsOnceKeyUp(VK_LBUTTON))
	{
		//소속아이템으로부터 szDestination으로 배달
		SendItemAtoPlaceB(m_vInvenItem);
		SendItemAtoPlaceB(m_vConShopItem);
		SendItemAtoPlaceB(m_vStatusItem);

		QuickSlotFunc();
	}
}

void cItemManager::ClickUseItem()
{
	if (KEY->IsOnceKeyDown(VK_RBUTTON))
	{
		if (_UI->GetIsCallInven() && ClickUseItemThisPlace(m_vInvenItem));
		else if (_UI->GetIsCallStatus() && ClickUseItemThisPlace(m_vStatusItem));
		else if (_UI->GetIsCallConShop() && ClickUseItemThisPlace(m_vConShopItem));
	}
}

void cItemManager::MovementInQuickSlot()
{

	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		if (m_vQuickItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{
			for (int j = 0; j < _UI->GetVQuickSlotUI().size(); j++)
			{
				RECT QSUIRc = _UI->GetVQuickSlotUI()[j]->GetUIImage()->GetResizeRect();
				if (PtInRect(&QSUIRc, ptMouse))
				{
					m_vQuickItem[i]->SetQuickSlotNum(j);
					break;
				}
			}



		}
	}
	/*for (int i = 0; i < m_vQuickItem.size(); i++)
	{
	if (m_vQuickItem[i]->GetUIRoot()->GetIsCollisionPT())
	{
	for (int j = 0; j < _UI->GetVQuickSlotUI().size(); j++)
	{
	RECT UIRc = _UI->GetVQuickSlotUI()[j]->GetUIImage()->GetResizeRect();
	if (PtInRect(&UIRc, ptMouse))
	{
	m_vQuickItem[i]->SetQuickSlotNum(j);
	break;
	}
	}
	}
	}*/


}

void cItemManager::QuickSlotFunc()
{
	QuickSlotRegist();
	if (!SwapInQuickSlot())
	{
		MovementInQuickSlot();
	}

}

void cItemManager::ConnectNodeCommand()
{
	//텍스트와 아이템 설명창의 연결
	for (int i = 0; i < m_vText.size(); i++)
	{
		if (m_vText[i]->GetParentName() == "ItemExplaneWindow")
		{
			m_vText[i]->ConnectNode(m_vEtcIcon[0]->GetUIRoot());


		}
	}


	//텍스트와 ui의 연결
	for (int i = 0; i < m_vText.size(); i++)
	{
		for (int j = 0; j < _UI->GetVUI().size(); j++)
		{
			if (m_vText[i]->GetParentName() == _UI->GetVUI()[j]->GetName())
			{
				m_vText[i]->ConnectNode(_UI->GetVUI()[j]->GetUIRoot());
			}
		}
	}

	//itemAssistant끼리의 연결
	for (int i = 0; i < m_vEtcIcon.size(); i++)
	{
		for (int j = 0; j < m_vEtcIcon.size(); j++)
		{
			if (m_vEtcIcon[i]->GetParentName() == m_vEtcIcon[j]->GetName())
			{
				m_vEtcIcon[i]->ConnectNode(m_vEtcIcon[j]->GetUIRoot());
			}
		}
	}

	//텍스트와 아이템과의 연결
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < m_vInvenItem.size(); j++)
		{
			if (m_vText[i]->GetParentName() == m_vInvenItem[j]->GetName())
			{
				m_vText[i]->ConnectNode(m_vInvenItem[j]->GetUIRoot());

				//m_vText[i]->SetParentName(NULL);
			}
		}
	}


}

void cItemManager::ExceptionsWhileDragging()
{



	for (int i = 0; i < m_vAllItem.size(); i++)
	{
		for (int j = 0; j < m_vAllItem.size(); j++)
		{
			m_vAllItem[i]->GetUIRoot()->SetIsMove(false);

			if (m_vAllItem[i]->GetUIRoot()->GetClickState() == CLICK)
			{
				if (i != j)m_vAllItem[j]->GetUIRoot()->SetClickState(UNMOVABLE);
			}
		}
	}

	if (KEY->IsOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < m_vAllItem.size(); i++)
		{
			m_vAllItem[i]->GetUIRoot()->SetClickState(NON);
		}
	}

}

void cItemManager::SortInSlot()
{
	for (int i = 0; i < m_vStatusItem.size(); i++)
	{
		if (m_vStatusItem[i]->GetItemKind() == SHOES)
			m_vStatusItem[i]->TransPos(m_vStatusSlot[0].vec3Pos);

		else if (m_vStatusItem[i]->GetItemKind() == ARMOR)
			m_vStatusItem[i]->TransPos(m_vStatusSlot[1].vec3Pos);

		else if (m_vStatusItem[i]->GetItemKind() == WEAPON)
			m_vStatusItem[i]->TransPos(m_vStatusSlot[2].vec3Pos);

		else if (m_vStatusItem[i]->GetItemKind() == GLOVES)
			m_vStatusItem[i]->TransPos(m_vStatusSlot[3].vec3Pos);

	}

	for (int i = 0; i < m_vConShopItem.size(); i++)
	{
		m_vConShopItem[i]->TransPos(m_vShopSlot[i].vec3Pos);
	}

	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		m_vInvenItem[i]->TransPos(m_vInvenSlot[i].vec3Pos);
	}
}

void cItemManager::ItemUpdate()
{
	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		m_vInvenItem[i]->Update();
	}

	for (int i = 0; i < m_vConShopItem.size(); i++)
	{
		m_vConShopItem[i]->Update();
	}

	for (int i = 0; i < m_vStatusItem.size(); i++)
	{
		m_vStatusItem[i]->Update();
	}

	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		m_vQuickItem[i]->Update();
	}

}

void cItemManager::ItemRender()
{

	if (_UI->GetIsCallInven())
	{
		//인벤토리 아이템 렌더
		for (int i = 0; i < m_vInvenItem.size(); i++)
		{
			m_vInvenItem[i]->Render();
		}
	}

	//상점 아이템 렌더
	if (_UI->GetIsCallConShop())
	{
		for (int i = 0; i < m_vConShopItem.size(); i++)
		{
			m_vConShopItem[i]->Render();
		}
	}

	if (_UI->GetIsCallStatus())
	{
		for (int i = 0; i < m_vStatusItem.size(); i++)
		{
			m_vStatusItem[i]->Render();
		}
	}

	if (_UI->GetIsCallQuickSlot())
	{
		for (int i = 0; i < m_vQuickItem.size(); i++)
		{
			m_vQuickItem[i]->Render();
		}
	}
}

void cItemManager::ItemRenewalThisPlace(vItem& _vVectorName)
{
	for (int i = 0; i < _vVectorName.size(); i++)
	{
		_vVectorName[i]->GetUIImage()->SetAlpha(200);
	}
}

void cItemManager::ConditionalExcutionWearBack(cItemInfo* _newWear)
{
	//스테이터스 슬롯에 있다는것은 장비하고 있다는 뜻
	for (int i = 0; i < m_vStatusItem.size(); i++)
	{

		if (m_vStatusItem[i]->GetItemKind() == _newWear->GetItemKind())

		{
			m_vInvenItem.push_back(m_vStatusItem[i]);

			m_vStatusItem.erase(m_vStatusItem.begin() + i);
		}

	}
}

void cItemManager::SalesItemCalculator()
{
	int saleBT = _UI->FindUIRootIndex("SaleBT");


	if (KEY->IsOnceKeyDown(VK_LBUTTON))
	{
		if (_UI->GetIsCallConShop() &&
			_UI->GetVUI()[saleBT]->GetUIButtonImage()->GetIsColDragRcAndPT())
		{
			int saleSlot = 8;

			int countItem = m_vConShopItem.size();

			for (int i = saleSlot; i < countItem; i++)
			{
				if (m_vConShopItem[i]->GetItemKind() == HPOTION ||
					m_vConShopItem[i]->GetItemKind() == MPOTION)
				{
					CalculatorGold(m_vConShopItem[i]->GetPotionCount() * m_vConShopItem[i]->GetSalePrice());

				}
				else CalculatorGold(m_vConShopItem[i]->GetSalePrice());
			}
			m_vConShopItem.erase(m_vConShopItem.begin() + 8, m_vConShopItem.end());
		}
	}
}

void cItemManager::BuyConsumables(int collisionNum)
{




	if (m_vInvenItem.size() <= 39)
	{
		if (collisionNum % 8 == 0 && m_nGold >= 250 && !FindSamePotion("하급회복물약"))
			CreateItem("하급회복물약", "Texture/ItemIcon/HPSmall.png", HPOTION, 50, 50, m_vInvenItem), CalculatorGold(-250);
		if (collisionNum % 8 == 1 && m_nGold >= 500 && !FindSamePotion("중급회복물약"))
			CreateItem("중급회복물약", "Texture/ItemIcon/HPMid.png", HPOTION, 100, 100, m_vInvenItem), CalculatorGold(-500);
		if (collisionNum % 8 == 2 && m_nGold >= 1000 && !FindSamePotion("상급회복물약"))
			CreateItem("상급회복물약", "Texture/ItemIcon/HPBig.png", HPOTION, 150, 200, m_vInvenItem), CalculatorGold(-1000);
		if (collisionNum % 8 == 3 && m_nGold >= 300 && !FindSamePotion("하급마나물약"))
			CreateItem("하급마나물약", "Texture/ItemIcon/MPSmall.png", MPOTION, 50, 60, m_vInvenItem), CalculatorGold(-300);
		if (collisionNum % 8 == 4 && m_nGold >= 600 && !FindSamePotion("중급마나물약"))
			CreateItem("중급마나물약", "Texture/ItemIcon/MPMid.png", MPOTION, 100, 120, m_vInvenItem), CalculatorGold(-600);
		if (collisionNum % 8 == 5 && m_nGold >= 1200 && !FindSamePotion("상급마나물약"))
			CreateItem("상급마나물약", "Texture/ItemIcon/MPBig.png", MPOTION, 150, 240, m_vInvenItem), CalculatorGold(-1200);
		if (collisionNum % 8 == 6 && m_nGold >= 1000 && !FindSamePotion("미스테리부적"))
			CreateItem("미스테리부적", "Texture/ItemIcon/MysteryPaper.png", ETCCONSUMABLES, 0, 200, m_vInvenItem), CalculatorGold(-1000);
		if (collisionNum % 8 == 7 && m_nGold >= 2000 && !FindSamePotion("마을귀환서"))
			CreateItem("마을귀환서", "Texture/ItemIcon/CityRecall.png", ETCCONSUMABLES, 0, 400, m_vInvenItem), CalculatorGold(-2000);

		InvenTextReconnection();
		QuickSlotSynchronize();
	}
}

void cItemManager::QuickSlotPosRenewal()
{
	for (int i = 0; i < _UI->GetVQuickSlotUI().size(); i++)
	{
		//UI의 위치를 받아와서
		/*quickRc[i] = _UI->GetVQuickSlotUI()[i]->GetUIImage()->GetCollisionRect();*/

		quickRc[i] = _UI->GetVQuickSlotUI()[i]->GetUIImage()->GetResizeRect();
		//계속 갱신되는 위치
		//16
		m_pVec3SlotPos[i].x = quickRc[i].left;
		m_pVec3SlotPos[i].y = quickRc[i].top;
		m_pVec3SlotPos[i].z = 0;
	}



}

void cItemManager::QuickSlotItemPosRenewal()
{


	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		if (m_vQuickItem[i]->GetName() == "바람가르기")m_vQuickItem[i]->SetQuickSlotNum(12);
		else if (m_vQuickItem[i]->GetName() == "내려치기")m_vQuickItem[i]->SetQuickSlotNum(13);
		else if (m_vQuickItem[i]->GetName() == "만월베기")m_vQuickItem[i]->SetQuickSlotNum(14);
		else if (m_vQuickItem[i]->GetName() == "찌르기")m_vQuickItem[i]->SetQuickSlotNum(15);

		m_vQuickItem[i]->TransPos(m_pVec3SlotPos[m_vQuickItem[i]->GetQuickSlotNum()]);
	}
}

void cItemManager::MainUIInfo()
{
	int ShopIdx = _UI->FindUIRootIndex("ConShop");
	int StatusIdx = _UI->FindUIRootIndex("Status");
	int InventoryIdx = _UI->FindUIRootIndex("Inventory");

	tempRc;
	m_ShopRc = _UI->GetVUI()[ShopIdx]->GetUIRoot()->GetCollisionRect();
	m_InvenRc = _UI->GetVUI()[InventoryIdx]->GetUIRoot()->GetCollisionRect();
	m_StatusRc = _UI->GetVUI()[StatusIdx]->GetUIRoot()->GetCollisionRect();

	for (int i = 0; i < _UI->GetVQuickSlotUI().size(); i++)
	{
		m_QuickRc[i] = _UI->GetVQuickSlotUI()[i]->GetUIImage()->GetResizeRect();
	}
}

void cItemManager::PotionOverlap()
{
	//int potionCount = 0;
	//for (int i = 0; i < m_vInvenItem.size(); i++)
	//{
	//	//포션일경우
	//	if (m_vInvenItem[i]->GetItemKind() == POTION)
	//	{
	//		//포션 카운트가 0인경우에만
	//		if (m_vInvenItem[i]->GetPotionCount() == 0)
	//		{

	//			//1로 셋팅해준다.
	//			m_vInvenItem[i]->SetPotionCount(1);
	//			potionCount += 1;
	//		}
	//	}
	//}





}

bool cItemManager::FindSamePotion(const char* szPotionName)
{
	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		if (m_vInvenItem[i]->GetName() == szPotionName)
		{
			m_vInvenItem[i]->SetPotionCount(m_vInvenItem[i]->GetPotionCount() + 1);
			if (szPotionName == "하급회복물약") CalculatorGold(-250);
			else if (szPotionName == "중급회복물약") CalculatorGold(-500);
			else if (szPotionName == "상급회복물약") CalculatorGold(-1000);
			else if (szPotionName == "하급마나물약") CalculatorGold(-300);
			else if (szPotionName == "중급마나물약") CalculatorGold(-600);
			else if (szPotionName == "상급마나물약") CalculatorGold(-1200);

			return true;
		}

	}
	return false;

}

void cItemManager::InvenTextReconnection()
{
	//텍스트와 아이템과의 연결
	for (int i = 0; i < m_vText.size() - 6; i++)
	{
		for (int j = 0; j < m_vInvenItem.size(); j++)
		{
			if (m_vText[i]->GetParentName() == m_vInvenItem[j]->GetName())
			{
				m_vText[i]->ConnectNode(m_vInvenItem[j]->GetUIRoot());

				//m_vText[i]->SetParentName(NULL);
			}
		}
	}
}

void cItemManager::QuickTextReconnection()
{


	for (int i = 10; i < 16; i++)
	{
		for (int j = 0; j < m_vQuickItem.size(); j++)
		{
			if (m_vText[i]->GetParentName() == m_vQuickItem[j]->GetName())
			{
				m_vText[i]->ConnectNode(m_vQuickItem[j]->GetUIRoot());

				//m_vText[i]->SetParentName(NULL);
			}
		}
	}

}

int cItemManager::SendItemAtoPlaceB(vector<cItemInfo*>& placeItem)
{
	if (placeItem == m_vConShopItem)
		if (!_UI->GetIsCallConShop())return 0;

	if (placeItem == m_vInvenItem)
		if (!_UI->GetIsCallInven())return 0;

	if (placeItem == m_vStatusItem)
		if (!_UI->GetIsCallStatus())return 0;

	vector<cItemInfo*>* vDestination[3];

	vDestination[0] = &m_vConShopItem;
	vDestination[1] = &m_vStatusItem;
	vDestination[2] = &m_vInvenItem;

	RECT UIRc[3];

	UIRc[0] = m_ShopRc;
	UIRc[1] = m_StatusRc;
	UIRc[2] = m_InvenRc;



	for (int i = 0; i < placeItem.size(); i++)
	{
		if (placeItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{
			/*		if (_strnicmp("con", szDestination, 3) == 0) vDestination = &m_vConShopItem;
			else if (_strnicmp("inv", szDestination, 3) == 0) vDestination = &m_vInvenItem;
			else if (_strnicmp("sta", szDestination, 3) == 0) vDestination = &m_vStatusItem;*/

			RECT itemRc = placeItem[i]->GetUIRoot()->GetCollisionRect();

			for (int j = 0; j < 3; j++)
			{
				if (placeItem == m_vInvenItem && j == 2)continue;
				else if (placeItem == m_vConShopItem && j == 0)continue;
				else if (placeItem == m_vStatusItem && j == 1)continue;




				RECT temp;
				//도착지의 colRect와 보내는 itemRc의 렉트가 충돌하면
				if (IntersectRect(&temp, &UIRc[j], &itemRc))
				{
					if (placeItem == m_vStatusItem)ConditionalExcutionWearBack(placeItem[i]);
					else if (placeItem == m_vConShopItem && i < 8)
					{
						BuyConsumables(i);
						return 1;
					}

					if (vDestination[j] == &m_vConShopItem && (*vDestination)[j].size() >= 24)return 1;
					else if (vDestination[j] == &m_vInvenItem && (*vDestination)[j].size() >= 40) return 1;

					(*vDestination)[j].push_back(placeItem[i]);
					placeItem.erase(placeItem.begin() + i);
				}
			}
		}
	}
}

int cItemManager::SendItemAtoPlaceB(vector<cItemInfo*>& placeItem, eSlotType _eSlotType)
{
	RECT QSRc;


	for (int i = 0; i < placeItem.size(); i++)
	{
		if (placeItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{

			RECT itemRc = placeItem[i]->GetUIRoot()->GetCollisionRect();

			RECT temp;

			for (int j = 0; j < _UI->GetVQuickSlotUI().size(); j++)
			{
				QSRc = _UI->GetVQuickSlotUI()[j]->GetUIImage()->GetCollisionRect();
				if (IntersectRect(&temp, &QSRc, &itemRc))
				{

					cItemInfo* itemInfo = placeItem[i];

					m_vQuickItem.push_back(itemInfo);

					return 1;

				}
			}
			return 0;
		}
	}
}




POINT cItemManager::FindPlaceAndIndex(vector<cItemInfo*> vPlaceItem)
{
	for (int i = 0; i < vPlaceItem.size(); i++)
	{
		if (vPlaceItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
		{
			if (vPlaceItem == m_vInvenItem)return{ INVENTORYSLOT, i };
			else if (vPlaceItem == m_vConShopItem)return{ CONSHOPSLOT, i };
			else if (vPlaceItem == m_vStatusItem)return{ STATUSSLOT, i };

		}
	}
	return{ 0,0 };
}

int cItemManager::FindPotionCount(vector<cItemInfo*> vPlaceItem, const char* szName)
{


	for (int i = 0; i < vPlaceItem.size(); i++)
	{
		if (vPlaceItem[i]->GetName() == szName)
		{
			return vPlaceItem[i]->GetPotionCount();
		}
	}
	if (vPlaceItem != m_vQuickItem)
	{

		for (int i = 8; i < m_vConShopItem.size(); i++)
		{
			if (m_vConShopItem[i]->GetName() == szName)
			{
				return m_vConShopItem[i]->GetPotionCount();
			}
		}
	}


}

void cItemManager::PotionCountTextThisName(const char * szPotionName)
{
	//m_nExcutionNum = POTIONCOUNT;

	CreateUIITextData
	(
	{ m_nExcutionNum , 999 },
	{ { 0,0,0 },{ 25,20,0 } },
	{ SMALL, SMALL },
	{ { 0,0,0 },{ 128,128,128 } }
	);
	tagText t7;
	t7.Type = VARIABLEVALUE;
	t7.nIdentyfyNum = m_nExcutionNum;
	t7.szParrentName = szPotionName;
	CreateText(t7);

	m_nExcutionNum += 1;

}

void cItemManager::QuickSlotTextThisName(const char * szSlotName)
{
	CreateUICTextData
	(
	{ "퀵슬롯텍스트" }
		, { { 0,0,0 } }
		, { BIG }
		, { { 100,100,100 } }
	);

	tagText t7;
	t7.Type = CONSTCHAR;
	t7.szTextName = "퀵슬롯텍스트";
	t7.szParrentName = szSlotName;
	CreateText(t7);

}

bool cItemManager::ClickUseItemThisPlace(vector<cItemInfo*>& sendItem)
{
	if (FindPlaceAndIndex(sendItem).x != 0)
	{
		int index;

		//위 함수에서 뱉어낸 인덱스
		index = FindPlaceAndIndex(sendItem).y;


		//인벤의 아이템인경우
		if (sendItem == m_vInvenItem)
		{	//상점이 온이면 상점으로
			if (_UI->GetIsCallConShop())
			{
				if (m_vConShopItem.size() >= 24) return false;
				m_vConShopItem.push_back(sendItem[index]);
			}
			//오프면 상태창으로
			else
			{
				if (sendItem[index]->GetItemKind() == HPOTION || sendItem[index]->GetItemKind() == MPOTION) return false;

				ConditionalExcutionWearBack(sendItem[index]);
				m_vStatusItem.push_back(sendItem[index]);

			}
			//삭제는 공통
			sendItem.erase(sendItem.begin() + index);
		}
		else if (sendItem == m_vConShopItem)
		{
			if (index < 8)
			{
				BuyConsumables(index);
			}
			else
			{
				m_vInvenItem.push_back(sendItem[index]);
				sendItem.erase(sendItem.begin() + index);
			}
		}
		else if (sendItem == m_vStatusItem)
		{
			if (sendItem[index]->GetItemKind() == WEAPON) return false;

			m_vInvenItem.push_back(sendItem[index]);
			sendItem.erase(sendItem.begin() + index);
		}

		return true;
	}
	else return false;



}

void cItemManager::AddInvenItem(const char * itemName, const char * filePath, tagItemKindAndETC itemType, int itemAbility, int itemSalePrice)
{
	//새로운 iteminfo클래스 셋팅
	m_pItemInfo = new cItem;

	//iteminfo에 보내줄 내용의 형식
	tagItemInfo _tagItemInfo;

	_tagItemInfo._itemPath = filePath;
	_tagItemInfo._itemPos = D3DXVECTOR3(0, 0, 0);
	_tagItemInfo._itemName = itemName;
	_tagItemInfo._itemKind = itemType;
	_tagItemInfo._itemAbilityValue = itemAbility;
	_tagItemInfo._itemBuyPrice = itemSalePrice * 5;
	_tagItemInfo._itemSalePrice = itemSalePrice;
	_tagItemInfo._itemParentName = NULL;

	//아이템인포에 정보를 전달
	m_pItemInfo->Setup(NULL, &_tagItemInfo);

	//this클래스의 벡터에 포인터로 저장
	m_vInvenItem.push_back(m_pItemInfo);

	//아이템의 종류가 아닐땐 관리벡터에 넣지 않는다.
	if (itemType != NOTANITEM)
	{
		m_vtagItemInfo.push_back(_tagItemInfo);

		//관리벡터에 넣음
		m_vAllItem.push_back(m_pItemInfo);

		nItemNum += 1;
	}
}


void cItemManager::ItemExplaneRender()
{

	if (isOptionMode)
	{
		for (int i = 0; i < m_vAllItem.size(); i++)
		{
			if (m_vAllItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
			{
				RECT itemRc = m_vAllItem[i]->GetUIRoot()->GetCollisionRect();

				for (int j = 0; j < _UI->GetVQuickSlotUI().size(); j++)
				{

					if ((_UI->GetIsCallConShop() && IntersectRect(&tempRc, &itemRc, &m_ShopRc)) ||
						(_UI->GetIsCallInven() && IntersectRect(&tempRc, &itemRc, &m_InvenRc)) ||
						(_UI->GetIsCallStatus() && IntersectRect(&tempRc, &itemRc, &m_StatusRc)) ||
						(_UI->GetIsCallQuickSlot() && IntersectRect(&tempRc, &itemRc, &m_QuickRc[j])))
					{
						isPlaceItemCollision = true;
						m_vEtcIcon[0]->Render();
						return;
					}
					else isPlaceItemCollision = false;

				}

			}
		}

	}
}

void cItemManager::ImitationIconRender()
{
	if (isOptionMode)
	{
		for (int i = 0; i < m_vItemImitation.size(); i++)
		{
			for (int j = 0; j < m_vAllItem.size(); j++)
			{
				if (m_vAllItem[j]->GetUIRoot()->GetIsColDragRcAndPT())
				{
					if (isPlaceItemCollision)
					{
						if (m_vItemImitation[i]->GetName() == m_vAllItem[j]->GetName())
						{
							m_vItemImitation[i]->Render();
						}

					}
				}
			}
		}
	}
}

void cItemManager::CreateImitationImage()
{
	//아이템인포에 정보를 전달
	for (int i = 0; i < m_vtagItemInfo.size(); i++)
	{
		m_pItemInfo = new cItem;
		m_pItemInfo->Setup(NULL, &m_vtagItemInfo[i]);
		m_vItemImitation.push_back(m_pItemInfo);
	}
}

int cItemManager::QuickSlotRegist()
{


	int itemIdx = INDEXNOTSET;
	int quickIdx = INDEXNOTSET;

	//마우스 해제시 아이템과 마우스가 충돌중이고
	//그것이 또 퀵슬롯과도 충돌중일때 아이템 인덱스와 퀵슬롯 인덱스를 알아냄
	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		for (int j = 0; j < _UI->GetVQuickSlotUI().size(); j++)
		{
			if (m_vInvenItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
			{
				if (PtInRect(&_UI->GetVQuickSlotUI()[j]->GetUIImage()->GetCollisionRect(), ptMouse))
				{
					for (int k = 0; k < m_vQuickItem.size(); k++)
					{
						if (m_vQuickItem[k]->GetQuickSlotNum() == j)
						{
							m_vQuickItem.erase(m_vQuickItem.begin() + k);
						}
					}


					itemIdx = i;
					quickIdx = j;
				}
			}
		}
	}
	//충돌하지 않았다면 초기값 그대로이므로 리턴함
	if (itemIdx == INDEXNOTSET) return 0;

	//char str[20] = "C";



	//char str[2] = "C";

	//vector<tagItemInfo> vItemInfoTemp;
	//
	//vItemInfoTemp = m_vtagItemInfo;

	for (int k = 0; k < m_vtagItemInfo.size(); k++)
	{

		//vChar.push_back((char)str);

		if (m_vInvenItem[itemIdx]->GetQuickSlotNum() == INDEXNOTSET && m_vInvenItem[itemIdx]->GetName() == m_vtagItemInfo[k]._itemName)
		{
			m_pItemInfo = new cItem;

			//	char szTemp[20];

			//	sprintf_s(szTemp, 20, "%s%s", vItemInfoTemp[k]._itemName, m_szStr);


			//strcat((char*)m_vtagItemInfo[k]._itemName, vChar[k]);
			//	vItemInfoTemp[k]._itemName = szTemp;

			m_pItemInfo->Setup(NULL, &m_vtagItemInfo[k]);
			m_vAllItem.push_back(m_pItemInfo);
			m_vQuickItem.push_back(m_pItemInfo);
			m_vInvenItem[itemIdx]->SetQuickSlotNum(quickIdx);
			m_vQuickItem.back()->SetQuickSlotNum(quickIdx);
			QuickTextReconnection();
			QuickSlotSynchronize();
			//InvenTextReconnection();
			return 1;
		}
	}


}

bool cItemManager::SwapInQuickSlot()
{
	RECT itemA;
	RECT itemB;

	int QSNumA;
	int QSNumB;


	//퀵슬롯의 아이템중에 선택된 녀석은 A
	//아이템끼리 충돌했을때
	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		for (int j = 0; j < m_vQuickItem.size(); j++)
		{
			if (j != i)
			{

				if (m_vQuickItem[j]->GetUIRoot()->GetIsColDragRcAndPT() &&
					m_vQuickItem[i]->GetUIRoot()->GetIsColDragRcAndPT())
				{
					QSNumA = m_vQuickItem[i]->GetQuickSlotNum();
					QSNumB = m_vQuickItem[j]->GetQuickSlotNum();

					m_vQuickItem[i]->SetQuickSlotNum(QSNumB);
					m_vQuickItem[j]->SetQuickSlotNum(QSNumA);

					return true;
				}

			}
		}
	}
	return false;
}

void cItemManager::QuickSlotSynchronize()
{
	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		for (int j = 0; j < m_vInvenItem.size(); j++)
		{
			if (m_vQuickItem[i]->GetItemKind() == HPOTION || m_vQuickItem[i]->GetItemKind() == MPOTION)
			{
				if (m_vQuickItem[i]->GetName() == m_vInvenItem[j]->GetName())
				{
					m_vQuickItem[i]->SetPotionCount(m_vInvenItem[j]->GetPotionCount());

					break;
				}
			}
		}
	}


}

void cItemManager::InventorySynchronize()
{
	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		for (int j = 0; j < m_vQuickItem.size(); j++)
		{
			if (m_vInvenItem[i]->GetItemKind() == HPOTION || m_vInvenItem[i]->GetItemKind() == MPOTION)
			{
				if (m_vInvenItem[i]->GetName() == m_vQuickItem[j]->GetName())
				{
					m_vInvenItem[i]->SetPotionCount(m_vQuickItem[j]->GetPotionCount());

					break;
				}
			}
		}
	}
}

void cItemManager::ZeroPotionDelete()
{
	for (int i = 0; i < m_vInvenItem.size(); i++)
	{
		if (m_vInvenItem[i]->GetPotionCount() == 0)
		{
			m_vInvenItem.erase(m_vInvenItem.begin() + i);
		}
	}

	for (int i = 0; i < m_vQuickItem.size(); i++)
	{
		if (m_vQuickItem[i]->GetPotionCount() == 0)
		{
			m_vQuickItem.erase(m_vQuickItem.begin() + i);
		}
	}
}



//특정 UI위치가 바뀌면 그 UI내의 아이템이 옮겨간다
void cItemManager::UIPosRenewal(const char* placeName)
{
	for (int i = 0; i < _UI->GetVUI().size(); i++)
	{
		if (_UI->GetVUI()[i]->GetName() == placeName)
		{
			D3DXVECTOR3 receivePos;
			receivePos.x = _UI->GetVUI()[i]->GetUIRoot()->GetMatWorld()._41;
			receivePos.y = _UI->GetVUI()[i]->GetUIRoot()->GetMatWorld()._42;
			receivePos.z = 0;

			if (placeName == "Inventory")m_vec3RenwalInvenPos = receivePos;
			else if (placeName == "ConShop")m_vec3RenwalShop = receivePos;
			else if (placeName == "Status")m_vec3RenwalStatus = receivePos;



		}
	}
}



