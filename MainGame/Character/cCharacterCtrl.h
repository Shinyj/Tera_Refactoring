#pragma once
#include "MainGame\Character\cCharacter.h"

// 상태 패턴 , 조작에 관련된 것

class cCharacterCtrl : public cCharacter
{
protected :
	enum CHARA_ANIM
	{
		CH_Anim_AdvLeap,
		CH_Anim_BackAttackShot,
		CH_Anim_BackAttackStart,
		CH_Anim_BehindCartchShot,
		CH_Anim_BehindCatchStart,
		CH_Anim_bReactionCom2,
		CH_Anim_bReactionCom3,
		CH_Anim_bReactionDown2,
		CH_Anim_bReactionDown3,
		CH_Anim_bReactionLand3,
		CH_Anim_bReactionRolling3,
		CH_Anim_bReactionStart,
		CH_Anim_bReactionStart3,
		CH_Anim_combo1,
		CH_Anim_combo1R,
		CH_Anim_combo2,
		CH_Anim_combo2R,
		CH_Anim_combo3,
		CH_Anim_combo3R,
		CH_Anim_combo4,
		CH_Anim_CutHead,
		CH_Anim_CuttingSlash,
		CH_Anim_Death,
		CH_Anim_Dearhwait,
		CH_Anim_DwonBlow,
		CH_Anim_DrawSword,
		CH_Anim_DrawSwordCharge,
		CH_Anim_DrawSwordEnd,
		CH_Anim_DrawSwordLoop,
		CH_Anim_DrawSwordMove,
		CH_Anim_FlatBlade,
		CH_Anim_gaiaCrush01,
		CH_Anim_gaiaCrush02,
		CH_Anim_gaiaCrush03,
		CH_Anim_groggy1,
		CH_Anim_GuardReaction1,
		CH_Anim_handySlash,
		CH_Anim_inweapon,
		CH_Anim_JawBreaker,
		CH_Anim_outweapon,
		CH_Anim_RagingStrike,
		CH_Anim_RetreatKick,
		CH_Anim_RisingAttack,
		CH_Anim_run,
		CH_Anim_slidingCharge,
		CH_Anim_slidingEnd,
		CH_Anim_slidingSlash,
		CH_Anim_slidingSlash01Shot,
		CH_Anim_sReaction,
		CH_Anim_StingerBlade,
		CH_Anim_swordDanceLoop,
		CH_Anim_swordDanceShot,
		CH_Anim_swordDanceStart,
		CH_Anim_tumbling,
		CH_Anim_Wait,
		CH_Anim_WindCutter1,
		CH_Anim_WindCutter1R,
		CH_Anim_WindCutter2,
		CH_Anim_WindCutter2R,
		CH_Anim_WindCutter3,
		CH_Anim_COUNT
	};

	enum CHARA_STATE
	{
		IDLE,
		RUN,
		ATTACK,
		DAMAGED,
		DIE
	};

	CHARA_STATE			m_state;
	CHARA_ANIM			m_anim;

	bool				m_bIsDone;

	float				m_fCurAnimTime;
	float				m_fAnimTime[CH_Anim_COUNT];

	float				m_fTime;

	bool				m_bIsBlend;

	bool				m_bDoCombo; // 연격중 콤보 입력여부

	cParticleSet*		m_pParticleSet;
	cParticleSet*		m_pParticleAura;
	cParticleSet*		m_pParticleHeal;

	bool				m_isDoEffect;
	bool				m_isDoSkiilSound;
public:
	cCharacterCtrl();
	~cCharacterCtrl();

	void Setup();
	void Update();
	void Render();

	// 애니메이션 월드 좌표를 세팅
	void SetAnimWorld();

	void SetAnimState();
	void SetParticle();

	void ProcessCombo();
	void ProcessDamaged();
	void ProcessGaiaCrash();

	void ProcessBigDamaged();
	void ProcessDie();

	void Anim(); // 애니메이션이 끝났을 떄 다음 애니로 바꾸기
	void Move(); // 움직임에 관련된 처리
	void KeyInput(); // 키 입력에 따른 처리

	// 데미지의 충격에 따른 맞는 모션 설정
	void Damaged(float damage, D3DXVECTOR3 pos);
	// 맞는 모션 지정 설정
	void Damaged(float damage, D3DXVECTOR3 dir, DAMAGED_TYPE type);
	void Damaged(float damage, D3DXVECTOR3 dir, CONDITION con, float per);
	void Damaged(float damage, D3DXVECTOR3 dir, CONDITION con, float per, DAMAGED_TYPE type);
	void BigDamaged();
	void Die();

	// 스킬 이펙트 처리.. 나중에 리펙토링 하자
	void SkillProcess();
	// 평타, 스킬 사운드 처리
	void AttSound();
	// 키 입력에 따른 각도 구하기
	void GetAngle();
};

