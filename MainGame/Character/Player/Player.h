#pragma once
#include "MainGame\Character\cCharacterCtrl.h"
class iMap;
class vItem;
class cItemBase;

#define UIX 0
#define UIY 0 

class Player : public cCharacterCtrl
{
private:

	cSkinnedMesh*			m_pBody;

	cSkinnedMesh*			m_pHair;
	cSkinnedMesh*			m_pHand;
	cSkinnedMesh*			m_pLeg;

	CHARA_ANIM				m_currState;
	ST_BONE*				m_pWeaponHand;
	ST_BONE*				m_pDummyRoot;
	ST_BONE*				m_pHead;

	cItemBase*				m_pWeapon;

	int						m_nWeaponNum = 0;
	int						m_nBodyNum = 0;
	int						m_nHandNum = 0;
	int						m_nLegNum = 0;

public:
	Player();
	~Player();

	void Setup();
	void Update();
	void Render();

	bool isUseLocalAnim();


	void ChangeWeapon(); // 인벤, 장비 이전 예비 함수
	void ChangeBody();
	void ChangeHand();
	void ChangeLeg();

	//캐릭터가 몬스터를 공격하는 함수
	void AttackBoundBox(); // 검의 바운딩박스로 공격
	virtual bool Attack(float damage);

	// 장비 바꾸기
	int ChangeEquit();
};

