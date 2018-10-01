#include "stdafx.h"
#include "Framework\Manager\cItemManager.h"

void cItemManager::CreateItemList()
{
	//아이템 설명창
	CreateItem("ItemExplaneWindow", "Resource/Texture/UIIcon/ItemInfoWindow.png", NOTANITEM, NULL, NULL, m_vEtcIcon);
	CreateItem("Gold", "Resource/Texture/UIIcon/Gold.png", NOTANITEM, NULL, NULL, m_vEtcIcon, "ItemExplaneWindow");
	CreateItem("바람가르기", "Resource/Texture/ItemIcon/SwordDance.png", SKILLICON, 250, 20000, m_vQuickItem);
	CreateItem("내려치기", "Resource/Texture/ItemIcon/HitDown.png", SKILLICON, 200, 10000, m_vQuickItem);
	CreateItem("만월베기", "Resource/Texture/ItemIcon/SideSlash.png", SKILLICON, 225, 15000, m_vQuickItem);
	CreateItem("찌르기", "Resource/Texture/ItemIcon/Piercing.png", SKILLICON, 175, 5000, m_vQuickItem);

	CreateItem("하급회복물약", "Resource/Texture/ItemIcon/HPSmall.png", HPOTION, 50, 50, m_vConShopItem);
	CreateItem("중급회복물약", "Resource/Texture/ItemIcon/HPMid.png", HPOTION, 100, 100, m_vConShopItem);
	CreateItem("상급회복물약", "Resource/Texture/ItemIcon/HPBig.png", HPOTION, 150, 200, m_vConShopItem);
	CreateItem("하급마나물약", "Resource/Texture/ItemIcon/MPSmall.png", MPOTION, 50, 60, m_vConShopItem);
	CreateItem("중급마나물약", "Resource/Texture/ItemIcon/MPMid.png", MPOTION, 100, 120, m_vConShopItem);
	CreateItem("상급마나물약", "Resource/Texture/ItemIcon/MPBig.png", MPOTION, 150, 240, m_vConShopItem);
	CreateItem("미스테리부적", "Resource/Texture/ItemIcon/MysteryPaper.png", ETCCONSUMABLES, 0, 200 , m_vConShopItem);
	CreateItem("마을귀환서", "Resource/Texture/ItemIcon/CityRecall.png", ETCCONSUMABLES, 0, 400, m_vConShopItem);
	
	
	CreateItem("대검", "Resource/Texture/ItemIcon/Claymore.png", WEAPON, 15, 200, m_vStatusItem);
	//CreateItem("아이스소드", "Texture/ItemIcon/TwohandIce.png", WEAPON, 150, 4000, m_vInvenItem);
	CreateItem("스카이소드", "Resource/Texture/ItemIcon/TwohandSky.png", WEAPON, 100, 3000, m_vInvenItem);
	CreateItem("참치", "Resource/Texture/ItemIcon/TwohandFish.png", WEAPON, 50,2000, m_vInvenItem);
	//
	////블랙셋트
	//CreateItem("검은마력의옷", "Texture/ItemIcon/BlackClothes.png", ARMOR, 75, 3600, m_vInvenItem);
	//CreateItem("검은마력의신발", "Texture/ItemIcon/BlackShoes.png", SHOES, 25 , 1400, m_vInvenItem);
	//CreateItem("검은마력의장갑", "Texture/ItemIcon/BlackGlove.png", GLOVES, 20, 1100, m_vInvenItem);
	
	//레더셋트
	CreateItem("레더아머", "Resource/Texture/ItemIcon/LeatherClothes.png", ARMOR, 50, 2600, m_vInvenItem);
	CreateItem("레더슈즈", "Resource/Texture/ItemIcon/LeatherShoes.png", SHOES, 15, 1100, m_vInvenItem);
	CreateItem("레더글러브", "Resource/Texture/ItemIcon/LeatherGlove.png", GLOVES, 11, 900, m_vInvenItem);
	
	//스카이셋트
	CreateItem("퍼플플랫아머", "Resource/Texture/ItemIcon/PurpleClothes.png", ARMOR, 20, 1000, m_vInvenItem);
	CreateItem("퍼플플랫슈즈", "Resource/Texture/ItemIcon/PurpleShoes.png", SHOES, 5, 200, m_vInvenItem);
	CreateItem("퍼플플랫글러브", "Resource/Texture/ItemIcon/PurpleGlove.png", GLOVES, 3, 160, m_vInvenItem);

	CreateItem("하급회복물약", "Resource/Texture/ItemIcon/HPSmall.png", HPOTION, 50, 50, m_vInvenItem);
	CreateItem("중급회복물약", "Resource/Texture/ItemIcon/HPMid.png", HPOTION, 100, 100, m_vInvenItem);


	// 최대 공격력 170 -> 120
	// 최대 방어력 130 -> 86
}


