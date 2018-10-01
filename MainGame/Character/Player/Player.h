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


	void ChangeWeapon(); // �κ�, ��� ���� ���� �Լ�
	void ChangeBody();
	void ChangeHand();
	void ChangeLeg();

	//ĳ���Ͱ� ���͸� �����ϴ� �Լ�
	void AttackBoundBox(); // ���� �ٿ���ڽ��� ����
	virtual bool Attack(float damage);

	// ��� �ٲٱ�
	int ChangeEquit();
};

