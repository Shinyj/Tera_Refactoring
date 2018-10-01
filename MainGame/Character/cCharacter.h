#pragma once
// 캐릭터 부모 클래스

#include "MainGame\cGameObject.h"

#define UIX 0
#define UIY 0

class iMap;

class cCharacter : public cGameObject
{
public:
	float				m_fRotY;
	D3DXVECTOR3			m_vDirection;
	D3DXMATRIX			m_matWorld;

	float				m_fSpeed;

	SYNTHESIZE(iMap*, m_pMap, Map);

	float				m_fCosVal; // 카메라 기준 각도


	D3DXMATRIX			m_matAnimWorld;
	D3DXVECTOR3			m_vCurAnimPos;
	D3DXVECTOR3			m_vBeforeAnimPos;

	float				m_fHpMax;
	float				m_fHpCur;
	float				m_fHpUI;

	float				m_fMpMax;
	float				m_fMpCur;
	float				m_fMpUI;

	float				m_fAttack;
	float				m_fDefense;

	// 프로그래스 바
	ProgressBar*		m_pHpBar;
	ProgressBar*		m_pMpBar;
	cSprite*			m_BackBar;


	cSprite*			m_pBleeding;
	cSprite*			m_pBurnning;
	cSprite*			m_pIcing;

	int					m_pBleedingAlpha;
	int					m_pConditionAlpha;
	// 장비 관련 변수...

	cItemInfo*			m_pEquitWeapon;
	cItemInfo*			m_pEquitBody;
	cItemInfo*			m_pEquitHand;
	cItemInfo*			m_pEquitLeg;

	cParticleSet*		m_pConditionBurn;
	cParticleSet*		m_pConditionIce;

	cParticleSet*		m_pUseHPotion;
	cParticleSet*		m_pUseMpotion;

	float				m_fDotDamagedTime;

	bool				m_isRun;
public:
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	cCharacter();
	~cCharacter();

	void Setup();
	void Update();
	void Render();

	void PlusMapHeight();

	void SetUpStateBar();
	void UpdateUpStateBar();
	void RenderUpStateBar();

	virtual void Damaged();
	virtual void Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per);
	virtual void Damaged(float damage, D3DXVECTOR3 pos, DAMAGED_TYPE type);
	virtual void Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per, DAMAGED_TYPE type);
	// 캐릭터 공격
	virtual bool Attack(float damage);

	// 장비 바꾸기
	virtual int ChangeEquit();

	virtual void Condition_Update();
	virtual void Condition_Render();

	void UseQuickSlot();
};

