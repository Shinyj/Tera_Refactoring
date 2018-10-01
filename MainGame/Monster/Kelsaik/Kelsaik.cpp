#include "stdafx.h"
#include "Kelsaik.h"

Kelsaik::Kelsaik()
	: m_pMonster(NULL)
	, m_currAnim(MON_Anim_Walk)
	, m_fHitCircleRadian(0.0f)
	, m_vHitCirclePos{ { 0,0,0 },{ 0, 0, 0 } }
	, m_fDamagedStack(0.0f)
	, m_isPossibleGroggy(true)
	, m_isPossibleDown(true)
	, m_isPossibleBerserk(true)
{
	D3DXMatrixIdentity(&m_matWorld);

	m_Anim = MON_Anim_Wait;

	//루프애니메이션이 아닐때, 동작이 끝난걸 알려주는 변수.
	m_bIsDone = true;

	m_fCurAnimTime = 0.0f;
	m_fTime = 0.0f;
	float animTime[MON_Anim_COUNT] =
	{
		61, 131, 141, 71, 191, 1, 61,
		196, 256, 171, 111, 111,
		131, 181, 85, 175, 51, 85, 31, 116,
		116, 51, 51, 31, 401, 1, 61, 61
	};

	memcpy(m_fAnimTime, animTime, sizeof(animTime));

	for (int i = 0; i < MON_Anim_COUNT; i++)
		m_fAnimTime[i] -= 1.0f;
	for (int i = 0; i < MON_Anim_COUNT; i++)
		m_fAnimTime[i] /= 30.0f;

	// 블랜딩 처리 할거니
	//블렌드 처리하니깐 제식해버리더라구. 그래서 해제시켰다.
	m_bIsBlend = false;

	//Monster01은 이런 특성을 가지고 있다.
	m_fAreaRadius = 200.0f;
	m_fTracableArea = 1000.0f;
	m_fRunSpeed = 1.5f;
	m_fFightZone = 100.0f;
	m_fHpCur = 0.0f;

	STATE = IDLE;

	// 패턴의 가짓 수
	m_nNumofPattern = 4;
	m_fPatternCostTime = 0.0f;

	m_pEffectCost = true;
	// 사용하는 파티클 등록
	m_pParticleBleeding = PARTICLE->GetParticle("Bleeding");
	PARTICLE->AddChild(m_pParticleBleeding);
	m_pIceHand = PARTICLE->GetParticle("IceHand2");
	PARTICLE->AddChild(m_pIceHand);
	m_pFireHand = PARTICLE->GetParticle("FireHand");
	PARTICLE->AddChild(m_pFireHand);
	m_pIceEffect = PARTICLE->GetParticle("IceEffect");
	PARTICLE->AddChild(m_pIceEffect);
	m_pFireEffect = PARTICLE->GetParticle("FireEffect");
	PARTICLE->AddChild(m_pFireEffect);

	m_pParticleBleeding = PARTICLE->GetParticle("Bleeding");
	PARTICLE->AddChild(m_pParticleBleeding);

	m_pIceHand->Start();
	m_pFireHand->Start();
}


Kelsaik::~Kelsaik()
{
	SAFE_DELETE(m_pMonster);
	SAFE_DELETE(m_pSphereR);
	SAFE_DELETE(m_pSphereL);

	SAFE_RELEASE(m_pHitFlash);
}

void Kelsaik::Setup()
{
	MonsterBase::Setup();

	//SetLight();

	m_fHpMax = 4000.0f;
	m_fHpCur = 0.0f;
	m_fAttack = 150.0f;
	m_fDefense = 10.0f;

	m_pMonster = new cSkinnedMesh;
	//m_pMonster->Setup("XFile/Monster", "Kelsaik.X"); // Kelsaik2 Kelsaik
	m_pMonster = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Monster", "kelsaik3.X");
	m_pMonster->SetAnimationIndexBlend(m_currAnim);


	// 위치를 가진 루트 본
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Dummy_root");

	m_pBIP = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01");

	// 싸대기 때릴 양손 본
	m_pHandR = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-R-Hand");

	m_pHandL = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-L-Hand");

	////양발 판정 구체
	m_pSphereR = new cSpere;
	m_pSphereR->Setup(D3DXVECTOR3(0, 0, 0), 40);

	m_pSphereL = new cSpere;
	m_pSphereL->Setup(D3DXVECTOR3(0, 0, 0), 40);



	//처음 젠되는 위치 설정

	m_fRotY = 4.7f;

	// 바운딩 박스 생성
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(D3DXVECTOR3(-80, -47, -50), D3DXVECTOR3(100, 100, 50));

	// 구 충돌 영역 생성(싸움존 거리)
	m_pSpere = new cSpere;
	m_pSpere->Setup(m_vPosition, m_fFightZone);

	STATE = IDLE;
	m_isDoingPattern = false;
	m_partternCost = true;

	SetUpStateBar();

	// 셰이더 초기화
	m_pHitFlash = Shader::LoadShader("Resource/Shader/Effect/", "HitFlash.fx");
	m_pRimLight = Shader::LoadShader("Resource/Shader/Effect/", "RimFlash.fx");
	m_pRimLight->SetFloat("Offset", 0.2);
	// RimFlash RimLight5
}


void Kelsaik::Update()
{
	//if (KEYMANAGER->IsOnceKeyDown('N'))
	//	m_Anim = (MON_Anim)((int)m_Anim + 1);
	SetTargetAngle();
	m_pDummyRoot->TransformationMatrix._42 = m_matWorld._42;
	//m_pBIP->TransformationMatrix._42 = m_matWorld._42;

	AnimUpdate();				// 애니메이션 진행

	switch (STATE)
	{
	case IDLE:
		Idle_Update();
		break;
	case AWAKE:
		Awake_Update();
		break;
	case BATTLE:
		Battle_Update();
		break;
	case TURN:
		Turn_Update();
		break;
	case WALK:
		Walk_Update();
		break;
	case DIE:
		Death_Update();
	}

	CountPossibleDamaged(1.0f);		// 무적시간
	UpdateWorld();				// 월드 갱신
	ParticleUpdate();			// 파티클 위치 업데이트
	CreatePatternCost();
	UpdateUpStateBar();

	MonsterBase::Update();

	cGameObject::Update();
}

void Kelsaik::Idle_Update()
{
	if (isPlayerInDistance(650.f))
	{
		ChangeState(AWAKE);
	}
}

void Kelsaik::Awake_Update()
{
	if (m_Anim != MON_Anim_modeAlarm)
	{
		ChangeAnim(MON_Anim_modeAlarm, true);

		SOUND->Play("MON_Anim_modeAlarm");
	}
	else if (isEndPattern())
	{
		ChangeState(BATTLE);
	}
	else if (m_fTime > 0.8f)
	{
		if (m_fHpCur < m_fHpMax)
			m_fHpCur += m_fHpMax / 130;
		else
			m_fHpCur = m_fHpMax;
		CAMERA->Shaking(0.06f);
	}

	//if (isPlayerInDistance())
	//	ChangeState(WALK);
}

void Kelsaik::Awake_Chase()
{

	//	m_Anim = MON_Anim_Walk;
	//	// u벡터 -> 기준벡터
	//	D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
	//	D3DXVECTOR3 v;
	//	D3DXVec3Normalize(&v, &temp);
	//
	//	m_fCosVal = D3DXVec3Dot(&v, &u);
	//	m_fCosVal = acosf(m_fCosVal);
	//
	//	if (m_vPosition.z < g_vPlayerPos->z)
	//		m_fCosVal = D3DX_PI * 2 - m_fCosVal;
	//
	//	m_vPosition += (m_fRunSpeed * v);
	//
	//	m_fRotY = m_fCosVal;
}

void Kelsaik::Awake_Battle()
{
	//몬스터 죽는거
	if (m_fHpCur <= 0)
		STATE = DIE;

	//나머지를 부타캐~~~~


}

void Kelsaik::Battle_Update()
{

	// 플레이어가 공격범위 밖으로 나가면 추적시작
	if (!isPlayerInDistance(m_fAreaRadius) && !m_isDoingPattern)
	{
		ChangeState(WALK);
		return;
	}
	else if (!m_isDoingPattern)
	{
		// 여기서 자연스럽게 각도를 바꾸면 좋겟다..
		//SetAngleWithPlayer();
		ChangeAnim(MON_Anim_Wait, true);
		//m_partternCost = false;
	}

	// 플레이어가 공격범위 안쪽이면 공격

	if (m_partternCost)
	{
		// 일단 패턴을 배치하고
		m_nPatternNum = rand() % NUMOFPATTERN;

		SetTargetAngle(); // 타겟의 각도를 잰 뒤,


		float rot = m_fTargetAngle - m_fRotY;
		if (rot < 0) rot += D3DX_PI * 2;

		// 타겟의 위치에 따라 고개를 돌리는거임
		if (rot >= D3DX_PI * 0.25 && rot < D3DX_PI * 0.75)
			m_nPatternNum = RIGHTTURN;
		else if (rot >= D3DX_PI * 0.75 && rot < D3DX_PI * 1.25)
			m_nPatternNum = BACKTURN;
		else if (rot >= D3DX_PI * 1.25 && rot < D3DX_PI * 1.75)
			m_nPatternNum = LEFTTURN;

		m_partternCost = false;
		m_isDoingPattern = true;
		m_pEffectCost = true;

	}


	if (m_isDoingPattern) // 패턴 중일때
	{
		switch (m_nPatternNum) // 정해진 패턴을 처리
		{
		case 0:				AttackPattern01();	break;
		case 1:				AttackPattern02();	break;
		case 2:				AttackPattern03();	break;
		case LEFTTURN:		TurnLeft();			break;
		case RIGHTTURN:		TurnRight();		break;
		case BACKTURN:		TurnBack();			break;
		case REACTION:		DamageReaction();	break;
		case REACTIONGRGY:	ReactionGroggy();	break;
		case REACTIONDOWN:	ReactionDown();		break;
		case BERSERK:		Berserk();			break;
		}
	}

	if (KEY->IsOnceKeyDown('N'))
	{
		m_fHpCur = 0;
		ChangeState(DIE);
	}
}

void Kelsaik::Turn_Update()
{
	// 회전 무브 == 51 프레임 -> 51 / 30 초 걸림
	if (m_Anim != MON_Anim_roundmove01 && m_Anim != MON_Anim_roundmove02)
	{
		SetTargetAngle();

		if (m_vPosition.z < g_vPlayerPos->z)
			ChangeAnim(MON_Anim_roundmove01, true);
		else
			ChangeAnim(MON_Anim_roundmove02, true);


		SetAnimWorld();
	}
	else if (!isEndPattern())
	{
		// 매 시간마다 월드 로테이트를 거꾸로 돌려야 될거같은데..
		if (m_Anim == MON_Anim_roundmove01)
		{
			// 빼야해
			m_fRotY -= abs(m_fRotY - m_fTargetAngle) * (TIME->GetEllapsedTime() / m_fCurAnimTime);
			if (m_fRotY < 0)
				m_fRotY += 2 * D3DX_PI;
			if (m_fRotY <= m_fTargetAngle)
			{
				m_fRotY = m_fTargetAngle;
				ChangeState(BATTLE);
				ChangeAnim(MON_Anim_Wait, true);
			}
		}
		else
		{
			// 더해야해
			m_fRotY += abs(m_fRotY - m_fTargetAngle) *  (TIME->GetEllapsedTime() / m_fCurAnimTime);
			if (m_fRotY > 2 * D3DX_PI)
				m_fRotY -= 2 * D3DX_PI;
			if (m_fRotY >= m_fTargetAngle)
			{
				m_fRotY = m_fTargetAngle;
				ChangeState(BATTLE);
				ChangeAnim(MON_Anim_Wait, true);
			}
		}
	}
	//else
	//{
	//	m_fRotY = m_fTargetAngle;
	//	ChangeState(BATTLE);
	//	ChangeAnim(MON_Anim_Wait, false);
	//}

}

void Kelsaik::Walk_Update()
{

	if (isPlayerInDistance(m_fAreaRadius))
	{
		ChangeState(BATTLE);
		return;
	}

	ChangeAnim(MON_Anim_Walk, true);

	SetAngleWithPlayer();

	D3DXVECTOR3 TargetPos = *g_vPlayerPos - m_vPosition;
	D3DXVec3Normalize(&TargetPos, &TargetPos);

	m_vPosition += (m_fRunSpeed * TargetPos);
}

void Kelsaik::Death_Update()
{
	if (m_Anim != MON_Anim_Death && m_Anim != MON_Anim_deathwait)
	{
		ChangeAnim(MON_Anim_Death, true);
		SetAnimWorld();

		SOUND->Play("MON_Anim_Death");
	}
	else
	{
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_deathwait, false);
		}
	}
}

void Kelsaik::Death_Die()
{
	m_fHpCur = 0.0f;

	m_Anim = MON_Anim_Death;
	m_fCurAnimTime = m_fAnimTime[MON_Anim_Death];
	m_bIsBlend = false;

}


void Kelsaik::AnimUpdate()
{
	// 애니메이션 진행

	// 만약 현재 진행중인 애니와 진행해야될 애니가 다르다면 교체해야함
	if (m_currAnim != m_Anim)
	{
		if (m_bIsBlend) // 블렌드 처리를 해야 한다면
			m_pMonster->SetAnimationIndexBlend(m_Anim);
		else
			m_pMonster->SetAnimationIndex(m_Anim);

		m_currAnim = m_Anim;						// 현재 애니를 바뀔 애니로

		m_pMonster->AnimAdvanceTime();				// 한번 애니메이션을 진행시켜야함;
		m_fTime += TIME->GetEllapsedTime();	// 진행했으니 시간도 올려야지
	}

	// 이동값이 있는 애니메이션의 업데이트면
	// 애니메이션 스타트 지점의 월드에서 부터 업데이트
	// 아니면 평소처럼 적용된 월드에서 업데이트
	if (isUseLocalAnim())
		m_pMonster->Update(m_matAnimWorld);
	else
		m_pMonster->Update(m_matWorld);
	m_fTime += TIME->GetEllapsedTime();
	// 로컬에서 위치의 변화량이 있는 애니메이션을 위한 처리
	m_vBeforeAnimPos = m_vCurAnimPos;
	m_vCurAnimPos = D3DXVECTOR3(m_pDummyRoot->TransformationMatrix._41, 0, 0);


	m_pSphereR->SetWorld(m_pHandR->CombinedTransformationMatrix);
	m_pSphereL->SetWorld(m_pHandL->CombinedTransformationMatrix);

	// 이동값이 있는 애니메이션 적용 시
	// 애니메이션 로컬을 현재 포지션으로 적용시키는 증가량을 계산 

	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x < 30.f)
		m_vPosition += (m_vDirection * (m_vCurAnimPos.x - m_vBeforeAnimPos.x));
	else
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
	}
}

// 현재 위치, 각도 정보를 통해 월드를 갱신하는 함수
void Kelsaik::UpdateWorld()
{
	D3DXMATRIX mat, matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;
	m_pBoundingBox->SetWorld(m_matWorld);
	m_pSpere->SetWorld(m_matWorld);
}

void Kelsaik::ParticleUpdate()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	mat._41 = m_pHandL->CombinedTransformationMatrix._41;
	mat._42 = m_pHandL->CombinedTransformationMatrix._42;
	mat._43 = m_pHandL->CombinedTransformationMatrix._43;
	m_pIceHand->SetWorld(mat);
	mat._41 = m_pHandR->CombinedTransformationMatrix._41;
	mat._42 = m_pHandR->CombinedTransformationMatrix._42;
	mat._43 = m_pHandR->CombinedTransformationMatrix._43;
	m_pFireHand->SetWorld(mat);
}

// 렌더
void Kelsaik::Render()
{
	RenderUpStateBar();

	//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DEVICE->SetTransform(D3DTS_WORLD, &mat);

	// 히트 박스 렌더
	//HitCircleRender();

	RimLightSetup(0, 0, 0, 0, 0, 0, 0);

	if (m_isPossibleDamaged)
	{
		if (m_isPicked)
			m_pMonster->Render(NULL, m_pRimLight);
		else
			m_pMonster->Render(NULL); // 요건 평상시의 렌더
	}
	else
	{						// 1, 0.6, 0.16, 1
		RimLightSetup(D3DXVECTOR4(1, 0, 0, 1), m_PossbleDamagedTime*0.2, 1.0f);
		m_pMonster->Render(NULL, m_pRimLight);// 요건 맞았을때 히트 플래쉬 렌더!
	}

	cGameObject::Render();

	if (SightSpere && m_pSphereR)
		m_pSphereR->Render();
	if (SightSpere && m_pSphereL)
		m_pSphereL->Render();
}

void Kelsaik::HitCircleRender()
{
	DrawPickCircle(m_vHitCirclePos[0], m_fHitCircleRadian);
	DrawPickCircle(m_vHitCirclePos[1], m_fHitCircleRadian);
}

// 이동값이 있는 애니메이션의 스타트 월드 매트릭스를 세팅
void Kelsaik::SetAnimWorld()
{
	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matAnimWorld = matR * matT;
	m_matWorld = matR * matT;
}

// 로컬에서 위치 변화가 있는 애니메이션은 변화량을 따로 빼놔야함
bool Kelsaik::isUseLocalAnim()
{

	if (
		m_Anim == MON_Anim_atk01 ||
		m_Anim == MON_Anim_atk02 ||
		m_Anim == MON_Anim_roundmove02 ||
		m_Anim == MON_Anim_roundmove01 ||
		m_Anim == MON_Anim_ReactonAtk ||
		m_Anim == MON_Anim_Death ||
		m_Anim == MON_Anim_groggy
		)
		return true;

	//이게 진짜 중요!!! 애니메이션 자체에 움직임이 심한경우 이처리를 해준다.
	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x > 30.0f)
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		return true;
	}


	return false;
}

void Kelsaik::ChangeState(MON_STATE state)
{
	STATE = state;
}

void Kelsaik::ChangeAnim(MON_Anim anim, bool isBlend, float Time)
{
	m_fTime = 0.0f;

	m_bIsBlend = isBlend;

	m_Anim = anim;
	m_fCurAnimTime = m_fAnimTime[anim] * Time;

}

bool Kelsaik::isPlayerInDistance(float distance)
{
	if (KEY->IsOnceKeyDown('B'))
		int a = 10;
	float Distance_Player_Monster = D3DXVec3Length(&(*g_vPlayerPos - m_vPosition));
	if (Distance_Player_Monster < distance)
		return true;
	return false;
}

bool Kelsaik::isEndPattern()
{
	if ((m_fCurAnimTime - 0.05 <= m_fTime))
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		return true;
	}
	return false;
}

void Kelsaik::CreatePatternCost()
{
	if (!m_partternCost && !m_isDoingPattern)
	{
		m_fPatternCostTime += TIME->GetEllapsedTime();
		if (m_fPatternCostTime >= 4.0f)
		{
			m_partternCost = true;
			m_fPatternCostTime = 0.0f;
		}
	}
}

void Kelsaik::SetAngleWithPlayer()
{

	D3DXVECTOR3 TargetPos = *g_vPlayerPos - m_vPosition;
	TargetPos.y = 0;
	D3DXVec3Normalize(&TargetPos, &TargetPos);
	m_fCosVal = D3DXVec3Dot(&TargetPos, &D3DXVECTOR3(1, 0, 0));
	m_fCosVal = acosf(m_fCosVal);

	if (m_vPosition.z < g_vPlayerPos->z)
		m_fCosVal = D3DX_PI * 2 - m_fCosVal;

	m_fRotY = m_fCosVal;
}

void Kelsaik::SetTargetAngle()
{
	D3DXVECTOR3 TargetPos = *g_vPlayerPos - m_vPosition;
	TargetPos.y = 0;

	D3DXVec3Normalize(&TargetPos, &TargetPos);
	m_fTargetAngle = D3DXVec3Dot(&TargetPos, &D3DXVECTOR3(1, 0, 0));
	m_fTargetAngle = acosf(m_fTargetAngle);

	if (m_vPosition.z < g_vPlayerPos->z)
		m_fTargetAngle = D3DX_PI * 2 - m_fTargetAngle;
}

// 불 속성
void Kelsaik::AttackPattern01()
{
	if (m_Anim != MON_Anim_atk01)
	{
		// 패턴 처음 시작
		ChangeAnim(MON_Anim_atk01, true);
		SetAngleWithPlayer();
		SetAnimWorld();
		m_fHitCircleRadian = 0.0f;

	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
		else // 파티클 출력과 함께 데미지를 주자!
		{
			if (m_fTime >= 2.28f && m_pEffectCost)
			{
				m_pEffectCost = false;
				D3DXMATRIX mat;
				D3DXMatrixIdentity(&mat);
				mat._41 = m_pHandR->CombinedTransformationMatrix._41;
				mat._42 = m_vPosition.y;
				mat._43 = m_pHandR->CombinedTransformationMatrix._43;
				m_pFireEffect->SetWorld(mat);
				m_pFireEffect->Start();
				CAMERA->Shaking(0.3f);
				m_vHitCirclePos[0] = D3DXVECTOR3(mat._41, -45, mat._43);

				SOUND->Play("MON_Anim_atk01");
			}
			else if (m_fTime <= 2.8 && !m_pEffectCost)
			{
				m_fHitCircleRadian += 4.5f;
				cSpere hitCircle;
				hitCircle.Setup(m_vHitCirclePos[0], m_fHitCircleRadian);
				OBJECTMANAGER->GiveDamagedChara(&hitCircle, m_fAttack, m_vPosition, CDT_BURN, 100.0f, PC_DMG_TYPE_1);
			}
		}
	}
}

// 얼음 속성
void Kelsaik::AttackPattern02()
{
	if (m_Anim != MON_Anim_atk02)
	{
		// 패턴 처음 시작
		ChangeAnim(MON_Anim_atk02, true);
		SetAngleWithPlayer();
		SetAnimWorld();
		m_fHitCircleRadian = 0.0f;
	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
		else // 파티클 출력과 함께 데미지를 주자!
		{
			if (m_fTime >= 2.3f && m_pEffectCost)
			{
				m_pEffectCost = false;
				D3DXMATRIX mat;
				D3DXMatrixIdentity(&mat);
				mat._41 = m_pHandL->CombinedTransformationMatrix._41;
				mat._42 = m_vPosition.y;
				mat._43 = m_pHandL->CombinedTransformationMatrix._43;
				m_pIceEffect->SetWorld(mat);
				m_pIceEffect->Start();
				CAMERA->Shaking(0.3f);
				m_vHitCirclePos[0] = D3DXVECTOR3(mat._41, -45, mat._43);

				SOUND->Play("MON_Anim_atk02");
			}
			else if (m_fTime <= 2.8 && !m_pEffectCost)
			{
				m_fHitCircleRadian += 4.5f;
				cSpere hitCircle;
				hitCircle.Setup(m_vHitCirclePos[0], m_fHitCircleRadian);
				OBJECTMANAGER->GiveDamagedChara(&hitCircle, m_fAttack, m_vPosition, CDT_ICE, 100.0f, PC_DMG_TYPE_1);

			}
		}
	}
}

void Kelsaik::AttackPattern03()
{
	if (m_Anim != MON_Anim_ReactonAtk)
	{
		// 패턴 처음 시작
		ChangeAnim(MON_Anim_ReactonAtk, true);
		SetAngleWithPlayer();
		SetAnimWorld();
		m_fHitCircleRadian = 0.0f;
	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
		else // 파티클 출력과 함께 데미지를 주자!
		{
			if (m_fTime >= 1.95f && m_pEffectCost)
			{
				m_pEffectCost = false;
				D3DXMATRIX mat;
				D3DXMatrixIdentity(&mat);
				mat._41 = m_pHandL->CombinedTransformationMatrix._41;
				mat._42 = m_vPosition.y;
				mat._43 = m_pHandL->CombinedTransformationMatrix._43;
				m_pIceEffect->SetWorld(mat);
				m_pIceEffect->Start();
				m_vHitCirclePos[0] = D3DXVECTOR3(mat._41, -45, mat._43);

				mat._41 = m_pHandR->CombinedTransformationMatrix._41;
				mat._42 = m_vPosition.y;
				mat._43 = m_pHandR->CombinedTransformationMatrix._43;
				m_pFireEffect->SetWorld(mat);
				m_pFireEffect->Start();
				m_vHitCirclePos[1] = D3DXVECTOR3(mat._41, -45, mat._43);
				CAMERA->Shaking(0.3f);

				SOUND->Play("MON_Anim_roundAtk01");
			}
			else if (m_fTime <= 2.45 && !m_pEffectCost)
			{
				m_fHitCircleRadian += 4.5f;
				cSpere hitCircle;
				hitCircle.Setup(m_vHitCirclePos[0], m_fHitCircleRadian);
				OBJECTMANAGER->GiveDamagedChara(&hitCircle, m_fAttack, m_vPosition, CDT_ICE, 100.0f, PC_DMG_TYPE_1);
				hitCircle.Setup(m_vHitCirclePos[1], m_fHitCircleRadian);
				OBJECTMANAGER->GiveDamagedChara(&hitCircle, m_fAttack, m_vPosition, CDT_BURN, 100.0f, PC_DMG_TYPE_1);

			}
		}
	}
}

void Kelsaik::TurnLeft()
{
	if (m_Anim != MON_Anim_roundmove01)
	{
		// 패턴 처음 시작
		ChangeAnim(MON_Anim_roundmove01, true);
		SetAnimWorld();
	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			SetAngleWithPlayer();
			//m_fRotY += D3DX_PI;
			//if (m_fRotY > D3DX_PI * 2)
			//	m_fRotY -= D3DX_PI * 2;
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
	}
}

void Kelsaik::TurnRight()
{
	if (m_Anim != MON_Anim_roundmove02)
	{
		// 패턴 처음 시작
		ChangeAnim(MON_Anim_roundmove02, true);
		SetAnimWorld();
	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			SetAngleWithPlayer();
			//m_fRotY += D3DX_PI;
			//if (m_fRotY > D3DX_PI * 2)
			//	m_fRotY -= D3DX_PI * 2;
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
	}
}

void Kelsaik::TurnBack()
{
	if (m_Anim != MON_Anim_roundAtk01 && m_Anim != MON_Anim_roundAtk02)
	{
		// 패턴 처음 시작
		if (rand() % 2)
		{
			//m_Anim = MON_Anim_roundAtk01;
			ChangeAnim(MON_Anim_roundAtk01, true);
		}
		else
		{
			//	m_Anim = MON_Anim_roundAtk02;
			ChangeAnim(MON_Anim_roundAtk02, true);
		}
		SetAnimWorld();

		SOUND->Play("MON_Anim_roundAtk02");
	}
	else
	{
		// 패턴이 끝났다면
		if (isEndPattern())
		{
			//SetAngleWithPlayer();
			m_fRotY += D3DX_PI;
			if (m_fRotY > D3DX_PI * 2)
				m_fRotY -= D3DX_PI * 2;
			ChangeAnim(MON_Anim_Wait, false);
			m_isDoingPattern = false;
		}
		else // 파티클 출력과 함께 데미지를 주자!
		{
			if (m_Anim == MON_Anim_roundAtk01)
				OBJECTMANAGER->GiveDamagedChara(m_pSphereR, m_fAttack, m_vPosition, CDT_BURN, 100, PC_DMG_TYPE_2);
			else if (m_Anim == MON_Anim_roundAtk02)
				OBJECTMANAGER->GiveDamagedChara(m_pSphereL, m_fAttack, m_vPosition, CDT_ICE, 100, PC_DMG_TYPE_2);
		}
	}
}

void Kelsaik::Berserk()
{
	if (m_Anim != MON_Anim_heavyatk01)
	{
		ChangeAnim(MON_Anim_heavyatk01, true);

		SOUND->Play("MON_Anim_heavyatk01");
	}
	else if (isEndPattern())
	{
		m_fAttack *= 2.0f;
		m_isDoingPattern = false;
		ChangeAnim(MON_Anim_Wait, false);
	}
	else if (m_fTime > 0.8f)
	{
		CAMERA->Shaking(0.06f);
	}
}

void Kelsaik::SetReactionPattern(int patternNum)
{
	m_fDamagedStack = 0;

	m_partternCost = false;
	m_isDoingPattern = true;
	m_pEffectCost = true;

	m_nPatternNum = patternNum;
}

void Kelsaik::DamageReaction()
{
	if (m_Anim != MON_Anim_flinch)
	{
		ChangeAnim(MON_Anim_flinch, true);

		SOUND->Play("MON_Anim_flinch");
	}
	else
	{
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_Wait, true);
			m_isDoingPattern = false;
		}
	}
}

void Kelsaik::ReactionGroggy()
{
	if (m_Anim != MON_Anim_groggy)
	{
		ChangeAnim(MON_Anim_groggy, true);
		SetAnimWorld();

		SOUND->Play("GiantGriffin_Groggy");
	}
	else
	{
		if (isEndPattern())
		{
			ChangeAnim(MON_Anim_Wait, true);
			m_isDoingPattern = false;
		}
	}
}

void Kelsaik::ReactionDown()
{
	if (m_Anim != MON_Anim_ReactionStart &&
		m_Anim != MON_Anim_ReactionLoop &&
		m_Anim != MON_Anim_ReactionEnd)
	{
		ChangeAnim(MON_Anim_ReactionStart, true);
		SetAnimWorld();
	}
	else
	{
		if (isEndPattern())
		{
			if (m_Anim == MON_Anim_ReactionStart)
				ChangeAnim(MON_Anim_ReactionLoop, false, 2.0f);
			else if (m_Anim == MON_Anim_ReactionLoop)
				ChangeAnim(MON_Anim_ReactionEnd, false);
			else
			{
				ChangeAnim(MON_Anim_Wait, true);
				m_isDoingPattern = false;
			}
		}
	}
}

void Kelsaik::Damaged(float Damaged, D3DXVECTOR3 pos)
{
	if (m_isPossibleDamaged == false || STATE == DIE)
		return;

	SOUND->Play("WPN_Sword_Attack");
	CAMERA->Shaking(0.1f);

	m_isPossibleDamaged = false;
	m_PossbleDamagedTime = 0.0f;

	Damaged = Damaged - m_fDefense;
	if (Damaged < 0) Damaged = 0;

	m_fHpCur -= Damaged;

	// 블리딩 터트릴 좌표를 만들어서 세팅하고 시작
	D3DXMATRIX matTS, matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	float x = 100 + m_vPosition.x;
	float y = rand() % 10 + 10 + m_vPosition.y;
	float z = rand() % 60 - 30 + m_vPosition.z;
	D3DXMatrixTranslation(&matT, x, y, z);
	m_pParticleBleeding->SetWorld(matR * matT);
	m_pParticleBleeding->Start();

	// 피격시 순간 번쩍임을 위해 만들었는데 이 방법은 아닌듯..
	//g_pD3DDevice->LightEnable(50, true);

	if (m_fHpCur < m_fHpMax * 0.1f && m_isPossibleBerserk)
	{
		m_isPossibleBerserk = false;
		SetReactionPattern(BERSERK);
	}
	else if (m_fHpCur < m_fHpMax * 0.3f && m_isPossibleDown)
	{
		m_isPossibleDown = false;
		SetReactionPattern(REACTIONDOWN);
	}
	else if (m_fHpCur < m_fHpMax * 0.5f && m_isPossibleGroggy)
	{
		m_isPossibleGroggy = false;
		SetReactionPattern(REACTIONGRGY);
	}
	// 누적데미지 계산, 500을 넘었을 시 경직 모션을 재생
	else if (m_fDamagedStack > 500 && (m_nPatternNum != REACTIONGRGY) && (m_nPatternNum != REACTIONDOWN))
	{
		SetReactionPattern(REACTION);
	}
	else // 500 아니면 축적
		m_fDamagedStack += Damaged;


	if (m_fHpCur < 0)
	{
		m_fHpCur = 0;
		ChangeState(DIE);
	}
}

void Kelsaik::SetUpStateBar()
{
	m_BackBar = TEXTURE->GetSprite(MonsterInfoTexPath + L"BossHpBack.png");

	m_pHpBar = new ProgressBar;
	m_pHpBar->Setup("Resource/Texture/MonsterInfo/BossHp.png",
		"Resource/Texture/MonsterInfo/BossEmpty.png",
		562, 135, 460, 16);

}

void Kelsaik::UpdateUpStateBar()
{
	m_pHpBar->SetGauge(m_fHpCur, m_fHpMax);
}

void Kelsaik::RenderUpStateBar()
{
	if (STATE == IDLE)
		return;

	m_BackBar->Render(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(WINSIZEX / 2 - m_BackBar->textureInfo.Width / 2.0f, 120, 0));
	m_pHpBar->Render();

	char szTemp[1024];
	RECT rc;

	sprintf_s(szTemp, 1024, "%.0f%%", ((float)m_fHpCur / (float)m_fHpMax)*100.0f);
	SetRect(&rc,
		WINSIZEX / 2 - 20, 137, WINSIZEX / 2 + 20, 152);

	LPD3DXFONT pFont = FONT->GetFont(cFontManager::TF_UI_NUMBER2);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
		DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
}

void Kelsaik::SetLight()
{
	D3DXVECTOR3 position = m_vPosition;

	D3DXVECTOR3 vecDir;
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_POINT;
	light.Ambient.r = light.Diffuse.r = 1.0f;
	light.Ambient.g = light.Diffuse.g = 1.0f;
	light.Ambient.b = light.Diffuse.b = 1.0f;

	light.Attenuation0 = 0.2f;
	light.Attenuation1 = 0.1f;
	light.Attenuation2 = 0.1f;

	light.Position = position;

	vecDir = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);
	light.Range = 2000.0f;
	DEVICE->SetLight(50, &light);
}

void Kelsaik::HitShaderUpdate(float offset)
{
	D3DXMATRIX   matView, matWorld, matProj;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixIdentity(&matWorld);
	m_pHitFlash->SetMatrix("gWorldMatrix", &matWorld);
	m_pHitFlash->SetMatrix("gViewMatrix", &matView);
	m_pHitFlash->SetMatrix("gProjectionMatrix", &matProj);
	m_pHitFlash->SetFloat("Offset", offset);
}

