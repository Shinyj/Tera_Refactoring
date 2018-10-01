#pragma once

#include "MainGame\Monster\MonsterBase.h"

class GateKeeper : public MonsterBase
{
private:
	enum MON_STATE
	{
		MON_STATE_Walk,
		MON_STATE_Wait,
		MON_STATE_unarmedwait,
		MON_STATE_stun,
		MON_STATE_SideWalk02,
		MON_STATE_SideWalk01,
		MON_STATE_run,
		MON_STATE_ReadyAction,
		MON_STATE_KnockBackStart,
		MON_STATE_KnockBackLoop,
		MON_STATE_KnockBackEnd,
		MON_STATE_jumpevasion02,
		MON_STATE_jumpevasion01,
		MON_STATE_flinch,
		MON_STATE_DyingWait,
		MON_STATE_DownStart,
		MON_STATE_DownLoop,
		MON_STATE_DownEnd,
		MON_STATE_deathwait,
		MON_STATE_Death,
		MON_STATE_atk06,
		MON_STATE_atk05,
		MON_STATE_atk04,
		MON_STATE_atk03,
		MON_STATE_atk02,
		MON_STATE_atk01,
		MON_STATE_aggrochange,
		MON_STATE_COUNT
	};



	cSkinnedMesh*		m_pMonster;
	ST_BONE*			m_pDummyRoot;

	MON_STATE			m_state;
	MON_STATE			m_currState;

	bool				m_bIsDone;

	float				m_fCurAnimTime;
	float				m_fAnimTime[MON_STATE_COUNT];

	float				m_fTime;
	int					m_nTime;

	bool				m_bIsBlend;

	float				m_fCosVal;

	//행동패턴 디자인에 쓰인 임시변수
	D3DXVECTOR3			temp;
	D3DXVECTOR2			tt;

	//공격에 쓰이는 본

	ST_BONE*			m_pHandR;
	ST_BONE*			m_pHandL;
	cSpere*				m_pSphereR;
	cSpere*				m_pSphereL;

	float				m_fFightZone;

public:
	GateKeeper();
	~GateKeeper();

	//------------------------기본함수
	void Setup(D3DXVECTOR3 v);
	void Update();
	void Render();
	
	//--------------------------옵션설정
	bool isUseLocalAnim();

	// 애니메이션 월드 좌표를 세팅
	void SetAnimWorld();

	//---------------------------동작

	void ProcessDamaged();

	void ProcessBigDamaged();
	void ProcessDie();

	void Move();

	void Die();

	void MonoBehavior(void);
	void Roaming(void);

	//몬스터가 캐릭터를 공격하는 함수
	virtual bool Attack(float damage);

	//몬스터가 캐릭터에게 피해를 받는 함수
	void Damaged(float damage, D3DXVECTOR3 pos);
};

