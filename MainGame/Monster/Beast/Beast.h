#pragma once
#include "MainGame\Monster\MonsterBase.h"

class Beast : public MonsterBase
{
protected:

	enum MON_MODE
	{
		IDLE,
		AWAKE,
		CHASE,
		BATTLE,
		RETURN,
		DEATH,
		DISAPPEAR,
		REBIRTH
	};

	enum MON_STATE
	{
		MON_STATE_Walk,
		MON_STATE_Wait,
		MON_STATE_unarmedwait,
		MON_STATE_run,
		MON_STATE_KnockBackStart,
		MON_STATE_KnockBackLoop,
		MON_STATE_KnockBackEnd,
		MON_STATE_jumpevasion,
		MON_STATE_Idle,
		MON_STATE_DownStart,
		MON_STATE_DownLoop,
		MON_STATE_DownEnd,
		MON_STATE_deathwait,
		MON_STATE_Death,
		MON_STATE_commandreceive,
		MON_STATE_atk03,
		MON_STATE_atk02,
		MON_STATE_atk01,
		MON_STATE_COUNT
	};

	MON_MODE			MODE;	//얘에 따라서 호출하는 하위업데이트가 다르다.

	cSkinnedMesh*		m_pMonster;						// 몬스터 스켈레톤 메시
	ST_BONE*			m_pDummyRoot;					// 루트 본
	ST_BONE*			m_pBIP;							// 고정된 본

	MON_STATE			m_state;							// 재생중 애니메이션
	MON_STATE			m_currState;						// 바뀔 애니메이션

	bool				m_bIsDone;						// 애니메이션이 끝났니?

	float				m_fCurAnimTime;					// 재생된 시간 애니메이션
	float				m_fAnimTime[MON_STATE_COUNT];	// 재생돼야할 총 시간
	float				m_fTime;						// 애니 걸린시간
	int					m_nTime;						// 데스쉐이딩에 쓸 시간변수.
	bool				m_bIsBlend;						// 애니 블렌딩 처리여부
	float				m_fCosVal;						// 봐야할 각도

														//공격에 쓰이는 본

	ST_BONE*			m_pHandR;
	ST_BONE*			m_pHandL;
	cSpere*				m_pSphereR;
	cSpere*				m_pSphereL;

	float				m_fFightZone;

	//피격처리에 쓰는 변수

	bool				GetDamaged = false;;
	float				DamageTerm = 0.0f;


public:
	Beast();
	~Beast();

	void Setup(D3DXVECTOR3 v);

	/////////Update Function//////////
	void Update();
	void Idle();
	void Awake();
	void Chase();
	void Battle();
	void Return();
	void Death();
	void Disappear();
	void Rebirth();

	//////////////////////////////////

	void Render();

	////////Rendering Function///////

	void SetAnimWorld();
	bool isUseLocalAnim();


	//몬스터가 캐릭터를 공격하는 함수
	virtual bool Attack(float damage);

	//몬스터가 캐릭터에게 피해를 받는 함수
	void Damaged(float damage, D3DXVECTOR3 pos);

};

