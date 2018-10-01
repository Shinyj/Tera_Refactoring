#include "stdafx.h"
#include "Framework\Manager\cItemManager.h"




void cItemManager::UITextList()
{

	//내용(1), 위치(3), 글씨크기(2), 컬러(3)
#pragma region 소모품상점
	CreateUICTextData
	(
		{ "소모품상점", "정산", "하급회복물약", "중급회복물약", "상급회복물약", "하급마나물약", "중급마나물약", "상급마나물약", "미스테리부적", "마을귀환서" }
		, { { 200,10,0 },{ 310, 256, 0 },{ 65, 93, 0 },{ 240, 93, 0 },{ 65, 132, 0 },{ 241, 129, 0 },{ 65,172,0 },{ 241,172,0 },{ 65, 210, 0 },{ 240, 210, 0 } }
		, {  MID, SMALL, MID, MID, MID, MID, MID, MID, MID, MID }
		, { { 211, 254, 255 },{ 211, 254, 255 },{ 200, 50, 55 },{ 200, 50, 55 },{ 200, 50, 55 } ,{ 200, 50, 55 } ,{ 200, 50, 55 } ,{ 200, 50, 55 },{ 200, 50, 55 },{ 200, 50, 55 } }
	);
	tagText t1;
	t1.Type = CONSTCHAR;
	t1.szTextName = "소모품상점";
	t1.szParrentName = "ConShop";
	CreateText(t1);

#pragma endregion

#pragma region 아이템툴팁
	CreateUICTextData
	(
		{ "아이템정보","능력", "설명:", "설명의내용", "판매또는구매가격" }
		, { { 50,10,0 },{ 50,40,0 },{ 10,90,0 },{ 30, 120, 0 },{ 30, 200,0 } }
		, { MID, MID, MID, MID, SMALL }
		, { { 135,132,255 },{ 200,200,200 },{ 128,128,128 },{ 128,128,128 },{ 255,255,255 } }
	);

	tagText t2;
	t2.Type = CONSTCHAR;
	t2.szTextName = "아이템정보";
	t2.szParrentName = "ItemExplaneWindow";
	CreateText(t2);


	CreateUIITextData
	(
		{ ITEMVALUE, ZEROVALUE, ZEROVALUE }
		, { { 0, 0, 0 },{ 155, 38, 0 },{ 130, 200, 0 } }
		, { MID, MID, SMALL }
		, { { 0, 0, 0 },{ 230, 50, 230 },{ 255, 255, 255 } }
	);

	tagText t3;
	t3.Type = VARIABLEVALUE;
	t3.nIdentyfyNum = ITEMVALUE;
	t3.szParrentName = "ItemExplaneWindow";
	CreateText(t3);
#pragma endregion

#pragma region 인벤토리
	CreateUIITextData
	(
		//인벤토리
		{ GOLD, m_nGold }
		, { { 50,10,0 },{ 200,334,0 } }
		, { MID, SMALL }
		, { { 128,128,128 },{ 255,255,255 } }
	);

	tagText t4;
	t4.Type = VARIABLEVALUE;
	t4.nIdentyfyNum = GOLD;
	t4.szParrentName = "Inventory";
	CreateText(t4);
#pragma endregion

#pragma region 스테이터스창
	CreateUICTextData
	(
		{ "엘린", "공격력", "방어력" }
		, { {129,9,0}, {15,261,0}, {145,261,0} }
		, { MID, MID, MID }
		, { {236,139,255}, {50,79,255},{ 50,79,255} }
	);

	tagText t5;
	t5.Type = CONSTCHAR;
	t5.szTextName = "엘린";
	t5.szParrentName = "Status";
	CreateText(t5);

	CreateUIITextData
	(
		{ CHARACTERSPEC, ZEROVALUE, ZEROVALUE }
		, { {0,0,0},		{92,261,0},		{230 ,261,0 } }
		, { MID, MID, MID }
		, { {0,0,0},		{6,255,54},		{6,255,54} }
	);

	tagText t6;
	t6.Type = VARIABLEVALUE;
	t6.nIdentyfyNum = CHARACTERSPEC;
	t6.szParrentName = "Status";
	CreateText(t6);
	

#pragma endregion





#pragma region 포션카운트

	PotionCountTextThisName("하급회복물약");
	PotionCountTextThisName("중급회복물약");
	PotionCountTextThisName("상급회복물약");
	PotionCountTextThisName("하급마나물약");
	PotionCountTextThisName("중급마나물약");
	PotionCountTextThisName("상급마나물약");

	PotionCountTextThisName("하급회복물약");
	PotionCountTextThisName("중급회복물약");
	PotionCountTextThisName("상급회복물약");
	PotionCountTextThisName("하급마나물약");
	PotionCountTextThisName("중급마나물약");
	PotionCountTextThisName("상급마나물약");



#pragma endregion

#pragma region 스킬슬롯텍스트




#pragma endregion

	//QuickSlotTextThisName("Status");



	TextExplane();
}

void cItemManager::TextExplane()
{



	textExplane["하급회복물약"] = "소량의 HP를 \n 회복시킨다";
	textExplane["중급회복물약"] = "적정량의 HP를\n 회복시킨다";
	textExplane["상급회복물약"] = "대량의 HP를\n 회복시킨다";
	textExplane["하급마나물약"] = "소량의 MP를\n 회복시킨다";
	textExplane["중급마나물약"] = "적정량의 MP를\n 회복시킨다";
	textExplane["상급마나물약"] = "대량의 MP를\n 회복시킨다";
	textExplane["미스테리부적"] = "사용시 신비한\n 일이 일어난다";
	textExplane["마을귀환서"] = "가까운 마을로\n 귀환한다";
	textExplane["대검"] = "커다란 검, \n굉장히 무겁다.";
	textExplane["참치"] = "칼날로 된 참치,\n 먹을 수 없다";
	textExplane["아이스소드"] = "얼음을 형상화해 \n 만든 검";
	textExplane["스카이소드"] = "하늘의 모습을\n 담아만든 검";
	textExplane["검은마력의옷"] = "고대 마력의 비밀\n을 담은 옷";
	textExplane["검은마력의신발"] = "고대 마력의 비밀\n을 담은 신발";
	textExplane["검은마력의장갑"] = "고대 마력의 비밀\n을 담은 장갑";
	textExplane["레더아머"] = "가죽을 덧대어\n 만든 갑옷";
	textExplane["레더슈즈"] = "가죽을 덧대어를\n 만든 신발";
	textExplane["레더글러브"] = "가죽을 덧대어\n 만든 장갑";
	textExplane["퍼플플랫아머"] = "보라색 천을 \n덧대어 만든 옷";
	textExplane["퍼플플랫슈즈"] = "보라색 천을 \n덧대어 만든 신발";
	textExplane["퍼플플랫글러브"] = "보라색 천을 \n덧대어 만든 장갑";

	textExplane["바람가르기"] = "공중으로 뛰어올라 \n 회전하며 회전력을 \n 검에 집중시킨다";
	textExplane["내려치기"] = "검을 높게 들어올려 \n 검의무게에 힘을더해 \n 강하게 공격한다";
	textExplane["만월베기"] = "검을 종으로 회전시켜 \n 만월을 그리며 벤다";
	textExplane["찌르기"] = "예리한 검의 끝에 \n 모든 힘을 실어 \n 강하게 찌른다";
	

	 


}









