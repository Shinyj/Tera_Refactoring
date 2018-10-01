#include "stdafx.h"
#include "cCharacterCtrl.h"


cCharacterCtrl::cCharacterCtrl()
{
}


cCharacterCtrl::~cCharacterCtrl()
{
}

void cCharacterCtrl::Setup()
{
	cCharacter::Setup();

	m_fCosVal = 0.0f;
	SetAnimState();
	SetParticle();

}

void cCharacterCtrl::Update()
{
	
	Anim();
	KeyInput();
	Move();

	cCharacter::Update();

	if (m_fHpCur < 0)
	{
		SOUND->Play("PCDie");
		Die();
	}
}

void cCharacterCtrl::Render()
{
	cCharacter::Render();
}

void cCharacterCtrl::SetAnimWorld()
{
	// 이동값이 있는 애니메이션의 스타트 월드 매트릭스를 세팅
	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matAnimWorld = matR * matT;
	m_matWorld = matR * matT;
}

void cCharacterCtrl::SetAnimState()
{
	m_anim = CH_Anim_Wait;
	m_bIsDone = true;

	m_fCurAnimTime = 0.0f;
	m_fTime = 0.0f;
	float animTime[60] =
	{
		16,34,10,34,10,29,58,16,31,46,10,29,49,
		31,25,37,30,28,26,61,91,73,65,1,55,42,54,
		55,25,27,55,21,52,33,25,8,106,29,49,22,43,
		52,52,27,15,34,24,63,17,73,47,76,31,61,24,
		21,19,21,26,61
	};

	memcpy(m_fAnimTime, animTime, sizeof(animTime));

	for (int i = 0; i < CH_Anim_COUNT; i++)
		m_fAnimTime[i] -= 1.0f;
	for (int i = 0; i < CH_Anim_COUNT; i++)
		m_fAnimTime[i] /= 30.0f;


	m_bIsBlend = true;
	m_bDoCombo = false;
}

void cCharacterCtrl::SetParticle()
{
	m_pParticleSet = PARTICLE->GetParticle("gaiaCrash");
	m_pParticleHeal = PARTICLE->GetParticle("Heal");

	PARTICLE->AddChild(m_pParticleSet);
	PARTICLE->AddChild(m_pParticleHeal);

	m_isDoEffect = false;
	m_isDoSkiilSound = false;
}

void cCharacterCtrl::ProcessCombo()
{// 연격 동작을 연결 시켜주는 함수
	// 애니메이션을 끝났을때 실행되며 , 콤보여부 불값에 따라 다음
	// 애니메이션을 적용
	if (m_bDoCombo)
	{
		//m_fRotY = m_fCosVal;
		SetAnimWorld();
	}

	if (m_anim == CH_Anim_combo1)
	{
		if (m_bDoCombo)
		{
			SOUND->Play("WPN_Sword_Swing");
			m_bDoCombo = false;
			m_anim = CH_Anim_combo2;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo2];
			m_bIsDone = false;
			m_bIsBlend = false;
			m_isDoSkiilSound = false;
		}
		else
		{
			m_anim = CH_Anim_combo1R;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo1R];
			m_bIsDone = false;
			m_bIsBlend = false;
		}
	}
	else if (m_anim == CH_Anim_combo1R)
	{
		m_anim = CH_Anim_Wait;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_combo2)
	{
		if (m_bDoCombo)
		{
			SOUND->Play("WPN_Sword_Swing");
			m_bDoCombo = false;
			m_anim = CH_Anim_combo3;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo3];
			m_bIsDone = false;
			m_bIsBlend = false;
			m_isDoSkiilSound = false;
		}
		else
		{
			m_anim = CH_Anim_combo2R;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo2R];
			m_bIsDone = false;
			m_bIsBlend = false;
		}
	}
	else if (m_anim == CH_Anim_combo2R)
	{
		m_anim = CH_Anim_Wait;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_combo3)
	{
		if (m_bDoCombo)
		{
			SOUND->Play("WPN_Sword_Swing");
			m_bDoCombo = false;
			m_anim = CH_Anim_combo4;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo4];
			m_bIsDone = false;
			m_bIsBlend = false;
			m_isDoSkiilSound = false;
		}
		else
		{
			m_anim = CH_Anim_combo3R;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_combo3R];
			m_bIsDone = false;
			m_bIsBlend = false;
		}
	}
}

void cCharacterCtrl::ProcessDamaged()
{
	if (m_anim == CH_Anim_bReactionStart)
	{
		m_anim = CH_Anim_bReactionCom2;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionCom2];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_bReactionStart3)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionLand3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionLand3];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_Death)
	{
		m_anim = CH_Anim_Dearhwait;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_Dearhwait];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_bReactionLand3)
	{
		m_anim = CH_Anim_bReactionCom3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionCom3];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_Dearhwait)
	{
		m_anim == CH_Anim_Dearhwait;
	}
}

void cCharacterCtrl::ProcessGaiaCrash()
{
	if (m_anim == CH_Anim_gaiaCrush01)
	{
		SetAnimWorld();
		m_anim = CH_Anim_gaiaCrush02;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_gaiaCrush02];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
	else if (m_anim == CH_Anim_gaiaCrush02)
	{
		SetAnimWorld();
		m_anim = CH_Anim_gaiaCrush03;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_gaiaCrush03];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
}

void cCharacterCtrl::ProcessBigDamaged()
{
}

void cCharacterCtrl::ProcessDie()
{
}

void cCharacterCtrl::Anim()
{
	m_fTime += TIME->GetEllapsedTime();

	// 애니메이션이 끝났는지 알아보고
	// 각 상황마다 다음 애니메이션 처리
	if (!m_bIsDone)
	{
		if (m_fCurAnimTime - 0.1f <= m_fTime)
		{
			m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
			m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);

			m_fCurAnimTime = 0.0f;
			m_fTime = 0.0f;
			m_bIsDone = false;

			CHARA_ANIM cs = m_anim;

			ProcessCombo();// 연격 처리 함수
			ProcessDamaged();// 데미지, 사망 처리

			ProcessGaiaCrash();

			//죽으면 뻗어야 되는데, 일어나버리니깐 뒤에 && 추가한거야.
			if (cs == m_anim && cs != CH_Anim_Dearhwait)
			{
				m_anim = CH_Anim_Wait;
				m_bIsBlend = false;
			}
		}
		SkillProcess(); // 스킬 이펙트, 타격 처리
		AttSound(); // 공격 사운드 처리
	}
}

void cCharacterCtrl::Move()
{
	// 이동에 관련된 함수
	D3DXVECTOR3 beforePos = m_vPosition;
	float		beforeRot = m_fRotY;

	if (KEY->IsStayKeyDown('A') && m_anim == CH_Anim_run)
	{
		m_fRotY = m_fCosVal + D3DX_PI * 1.5f;

		if (!OBJECTMANAGER->IsCollision(this))
			m_vPosition += (m_vDirection * m_fSpeed);
	}
	else if (KEY->IsStayKeyDown('D') && m_anim == CH_Anim_run)
	{
		m_fRotY = m_fCosVal + D3DX_PI * 0.5;
		if (!OBJECTMANAGER->IsCollision(this))
			m_vPosition += (m_vDirection * m_fSpeed);
	}
	if (KEY->IsStayKeyDown('W') && m_anim == CH_Anim_run)
	{
		if (KEY->IsStayKeyDown('A'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 1.75f;
			//m_vPosition += (m_vDirection * m_fSpeed);
		}
		else if (KEY->IsStayKeyDown('D'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 0.25;
			//m_vPosition += (m_vDirection * m_fSpeed);
		}
		else
		{
			m_fRotY = m_fCosVal;
			if (!OBJECTMANAGER->IsCollision(this))
				m_vPosition += (m_vDirection * m_fSpeed);
		}
	}
	if (KEY->IsStayKeyDown('S') && m_anim == CH_Anim_run)
	{
		if (KEY->IsStayKeyDown('A'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 1.25f;
			//m_vPosition += (m_vDirection * m_fSpeed);
		}
		else if (KEY->IsStayKeyDown('D'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 0.75;
			//m_vPosition += (m_vDirection * m_fSpeed);
		}
		else
		{
			m_fRotY = m_fCosVal + D3DX_PI;
			if (!OBJECTMANAGER->IsCollision(this))
				m_vPosition += (m_vDirection * m_fSpeed);
		}
	}

	if (m_fRotY < 0.0f)
		m_fRotY += (D3DX_PI * 2);
	else if (m_fRotY > D3DX_PI * 2)
		m_fRotY -= D3DX_PI * 2;


	D3DXMATRIX mat, matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);

	D3DXMatrixIdentity(&mat);
	D3DXMatrixTranslation(&mat, m_vPosition.x, m_vPosition.y + 15, m_vPosition.z);
	m_pBoundingBox->SetWorld(matR * mat);

	m_pSpere->SetWorld(mat);

	if (OBJECTMANAGER->IsCollision(this))
	{
		m_fRotY = beforeRot;
		m_vPosition = beforePos;
		D3DXMATRIX mat, matR, matT;
		D3DXMatrixRotationY(&matR, m_fRotY);
		D3DXVECTOR3 dir(m_fSpeed, 0, 0);
		D3DXVec3TransformNormal(&dir, &dir, &matR);
		m_vPosition -= dir;
		D3DXMatrixIdentity(&mat);
		D3DXMatrixTranslation(&mat, m_vPosition.x, m_vPosition.y + 15, m_vPosition.z);
		m_pBoundingBox->SetWorld(matR * mat);

		m_pSpere->SetWorld(mat);
	}



	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x < 30.0f)
		m_vPosition += (m_vDirection * (m_vCurAnimPos.x - m_vBeforeAnimPos.x));
	else
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
	}

	//D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;
}

void cCharacterCtrl::KeyInput()
{// 버튼 조작
	if (!isOptionMode)
	{
		if (KEY->IsStayKeyDown(VK_LBUTTON))
		{
			// 일반 연격 시작
			if (m_anim == CH_Anim_combo1 ||
				m_anim == CH_Anim_combo2 ||
				m_anim == CH_Anim_combo3 ||
				m_anim == CH_Anim_combo4)
			{
				// 연격을 하기 위한 콤여 여부 불 값 갱신 
				if (!m_bDoCombo)
				{
					if (m_fTime >= m_fCurAnimTime - 0.35f)
					{
						m_bDoCombo = true;
						m_fRotY = m_fCosVal;
					}
				}
			}
			// 첫타
			else if (m_anim == CH_Anim_Wait || m_anim == CH_Anim_run)
			{
				m_isDoSkiilSound = false;
				// 정면 카메라 방향 위치 이동 애니메이션 시 해야되는 작업
				m_fRotY = m_fCosVal;
				// 위치 이동 애니메이션 시 해야되는 작업
				SetAnimWorld();

				m_anim = CH_Anim_combo1;
				m_fCurAnimTime = m_fAnimTime[CH_Anim_combo1];
				m_fTime = 0.0f;
				m_bIsDone = false;

				SOUND->Play("WPN_Sword_Swing");
			}
		}
		else if (KEY->IsOnceKeyDown(VK_RBUTTON) &&
			(m_anim == CH_Anim_run || m_anim == CH_Anim_Wait ||
				m_anim == CH_Anim_combo1R || m_anim == CH_Anim_combo2R || m_anim == CH_Anim_combo3R))
		{

			GetAngle();

			SetAnimWorld();

			// 구르기
			m_anim = CH_Anim_tumbling;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_tumbling];
			m_fTime = 0.0f;
			m_bIsDone = false;
			m_isRun = false;
			SOUND->Play("PCDamaged");
		}
		else if (KEY->IsOnceKeyDown(VK_SPACE))
		{
			SetAnimWorld();

			SOUND->Play("WPN_Sword_Swing2");

			// 여기는 점프를 만들어야 합니다.
			m_anim = CH_Anim_DwonBlow;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_DwonBlow];
			m_fTime = 0.0f;
			m_bIsDone = false;
		}
		else if (KEY->IsOnceKeyDown('Q') && m_anim == CH_Anim_Wait &&
			m_fMpCur >= 15)
		{
			m_fMpCur -= 15;
			if (m_fMpCur < 0) m_fMpCur = 0;
			// 가이아 크래시
			SetAnimWorld();

			m_anim = CH_Anim_gaiaCrush01;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_gaiaCrush01];
			m_fTime = 0.0f;
			m_bIsDone = false;
			m_isDoEffect = false;
			m_isDoSkiilSound = false;
		}
		else if (KEY->IsOnceKeyDown('E') && m_anim == CH_Anim_Wait&&
			m_fMpCur >= 15)
		{
			m_fMpCur -= 15;
			if (m_fMpCur < 0) m_fMpCur = 0;
			// 커팅슬래시

			SetAnimWorld();

			m_anim = CH_Anim_CuttingSlash;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_CuttingSlash];
			m_fTime = 0.0f;
			m_bIsDone = false;
			m_isDoEffect = false;
			m_isDoSkiilSound = false;
		}
		else if (KEY->IsOnceKeyDown('R') && m_anim == CH_Anim_Wait&&
			m_fMpCur >= 15)
		{
			m_fMpCur -= 15;
			if (m_fMpCur < 0) m_fMpCur = 0;
			// 컷헤드

			SetAnimWorld();

			m_anim = CH_Anim_CutHead;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_CutHead];
			m_fTime = 0.0f;
			m_bIsDone = false;
			m_isDoEffect = false;
			m_isDoSkiilSound = false;
		}
		else if (KEY->IsOnceKeyDown('T') && m_anim == CH_Anim_Wait)
		{
			m_fMpCur -= 15;
			if (m_fMpCur < 0) m_fMpCur = 0;
			SetAnimWorld();

			m_anim = CH_Anim_StingerBlade;
			m_fCurAnimTime = m_fAnimTime[CH_Anim_StingerBlade];
			m_fTime = 0.0f;
			m_bIsDone = false;
			m_isDoEffect = false;
			m_isDoSkiilSound = false;
		}

		// 스킬 셋에 따른 스킬을 나가게 해야해 ( 포션 사용도 )
		else if (m_bIsDone)
		{
			if (m_anim == CH_Anim_run)
				m_bIsBlend = true;
			m_anim = CH_Anim_Wait;
		}
	}
	else
	{
		if (m_bIsDone)
		{
			if (m_anim == CH_Anim_run)
				m_bIsBlend = true;
			m_anim = CH_Anim_Wait;
		}
	}

	// 이동
	if ((KEY->IsStayKeyDown('W') ||
		KEY->IsStayKeyDown('A') ||
		KEY->IsStayKeyDown('D') ||
		KEY->IsStayKeyDown('S')) &&
		(m_anim == CH_Anim_Wait || m_anim == CH_Anim_run))
	{
		if (m_isRun == false)
		{
			m_isRun = true;
			SOUND->Play("Char_Wark_GlassLand");
		}
		else if (m_fTime > 0.5f)
		{
			m_fTime = 0.0f;
			m_isRun = false;
		}
		m_anim = CH_Anim_run;
	}
	else if (KEY->IsOnceKeyUp('W') ||
		KEY->IsOnceKeyUp('A') ||
		KEY->IsOnceKeyUp('D') ||
		KEY->IsOnceKeyUp('S'))
	{
		//m_fTime = 0.0f;
		m_isRun = false;
	}
}

void cCharacterCtrl::Damaged(float damage, D3DXVECTOR3 dir)
{
	if (m_anim == CH_Anim_bReactionStart ||
		m_anim == CH_Anim_bReactionStart3 ||
		m_anim == CH_Anim_Dearhwait ||
		m_anim == CH_Anim_Death ||
		m_anim == CH_Anim_groggy1 ||
		m_anim == CH_Anim_tumbling ||
		(m_isPossibleDamaged == false)) return;

	m_isPossibleDamaged = false;
	m_PossbleDamagedTime = 0.0f;

	D3DXVECTOR3 u = dir - m_vPosition;
	u.y = 0;
	D3DXVec3Normalize(&u, &u);
	m_fCosVal = D3DXVec3Dot(&u, &D3DXVECTOR3(1, 0, 0));

	// nan 값 나오지 않게 예외처리..
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;

	m_fRotY = acosf(m_fCosVal);

	cCharacter::Damaged();

	int Damage = damage - m_fDefense;
	if (Damage < 0) Damage = 0;
	m_fHpCur -= Damage;

	if (Damage < m_fHpMax / 10.0f)
	{
		m_anim = CH_Anim_groggy1;
		m_fCurAnimTime = 0.5f;
		m_bIsBlend = true;
		m_bIsDone = false;

		SOUND->Play("PCDamaged");
	}
	else if (Damage< m_fHpMax / 6.0f)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}
	else
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart3];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}

	if (m_fHpCur < 0)
	{
		SOUND->Play("PCDie");
		Die();
	}
}

void cCharacterCtrl::Damaged(float damage, D3DXVECTOR3 dir, DAMAGED_TYPE type)
{
	if (m_anim == CH_Anim_bReactionStart ||
		m_anim == CH_Anim_bReactionStart3 ||
		m_anim == CH_Anim_Dearhwait ||
		m_anim == CH_Anim_Death ||
		m_anim == CH_Anim_groggy1 ||
		m_anim == CH_Anim_tumbling ||
		(m_isPossibleDamaged == false)) return;

	m_isPossibleDamaged = false;
	m_PossbleDamagedTime = 0.0f;

	D3DXVECTOR3 u = dir - m_vPosition;
	u.y = 0;
	D3DXVec3Normalize(&u, &u);
	m_fCosVal = D3DXVec3Dot(&u, &D3DXVECTOR3(1, 0, 0));

	// nan 값 나오지 않게 예외처리..
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;

	m_fRotY = acosf(m_fCosVal);

	cCharacter::Damaged();

	int Damage = damage - m_fDefense;
	if (Damage < 0) Damage = 0;
	m_fHpCur -= Damage;

	if (type == PC_DMG_TYPE_1)
	{
		m_anim = CH_Anim_groggy1;
		m_fCurAnimTime = 0.5f;
		m_bIsBlend = true;
		m_bIsDone = false;

		SOUND->Play("PCDamaged");
	}
	else if (type == PC_DMG_TYPE_2)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}
	else if (type == PC_DMG_TYPE_3)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart3];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}

	if (m_fHpCur < 0)
	{
		SOUND->Play("PCDie");
		Die();
	}
}

void cCharacterCtrl::Damaged(float damage, D3DXVECTOR3 dir, CONDITION con, float per)
{
	if (m_anim == CH_Anim_bReactionStart ||
		m_anim == CH_Anim_bReactionStart3 ||
		m_anim == CH_Anim_Dearhwait ||
		m_anim == CH_Anim_Death ||
		m_anim == CH_Anim_groggy1 ||
		m_anim == CH_Anim_tumbling ||
		(m_isPossibleDamaged == false)) return;

	m_isPossibleDamaged = false;
	m_PossbleDamagedTime = 0.0f;

	D3DXVECTOR3 u = dir - m_vPosition;
	u.y = 0;
	D3DXVec3Normalize(&u, &u);
	m_fCosVal = D3DXVec3Dot(&u, &D3DXVECTOR3(1, 0, 0));

	// nan 값 나오지 않게 예외처리..
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;

	m_fRotY = acosf(m_fCosVal);

	cCharacter::Damaged();

	int Damage = damage - m_fDefense;
	if (Damage < 0) Damage = 0;
	m_fHpCur -= Damage;

	if (Damage < m_fHpMax / 10.0f)
	{
		m_anim = CH_Anim_groggy1;
		m_fCurAnimTime = 0.5f;
		m_bIsBlend = true;
		m_bIsDone = false;

		SOUND->Play("PCDamaged");
	}
	else if (Damage < m_fHpMax / 6.0f)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}
	else
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart3];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}

	if (m_fHpCur < 0)
	{
		SOUND->Play("PCDie");
		Die();
	}

	if (rand() % 100 < per)
	{
		if (con == m_eCondition)
		{
			m_fHpCur = 0;
			Die();
			return;
		}
		else if (con == CDT_BURN && m_eCondition == CDT_ICE)
			m_eCondition = CDT_NORMAL;
		else if (con == CDT_ICE && m_eCondition == CDT_BURN)
			m_eCondition = CDT_NORMAL;
		else
			m_eCondition = con;

		switch (m_eCondition)
		{
		case CDT_NORMAL:
			m_pConditionBurn->End();
			m_pConditionIce->End();
			m_fSpeed = 1.0f;
			break;
		case CDT_BURN:
			m_pConditionBurn->Start();
			break;
		case CDT_ICE:
			m_pConditionIce->Start();
			m_fSpeed = 0.6;
			break;
		case CDT_STURN:
			break;
		default:
			break;
		}
	}
}

void cCharacterCtrl::Damaged(float damage, D3DXVECTOR3 dir, CONDITION con, float per, DAMAGED_TYPE type)
{
	if (m_anim == CH_Anim_bReactionStart ||
		m_anim == CH_Anim_bReactionStart3 ||
		m_anim == CH_Anim_Dearhwait ||
		m_anim == CH_Anim_Death ||
		m_anim == CH_Anim_groggy1 ||
		m_anim == CH_Anim_tumbling ||
		(m_isPossibleDamaged == false)) return;

	m_isPossibleDamaged = false;
	m_PossbleDamagedTime = 0.0f;

	D3DXVECTOR3 u = dir - m_vPosition;
	u.y = 0;
	D3DXVec3Normalize(&u, &u);
	m_fCosVal = D3DXVec3Dot(&u, &D3DXVECTOR3(1, 0, 0));

	// nan 값 나오지 않게 예외처리..
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;

	m_fRotY = acosf(m_fCosVal);

	cCharacter::Damaged();

	int Damage = damage - m_fDefense;
	if (Damage < 0) Damage = 0;
	m_fHpCur -= Damage;

	if (type == PC_DMG_TYPE_1)
	{
		m_anim = CH_Anim_groggy1;
		m_fCurAnimTime = 0.5f;
		m_bIsBlend = true;
		m_bIsDone = false;

		SOUND->Play("PCDamaged");
	}
	else if (type == PC_DMG_TYPE_2)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}
	else if (type == PC_DMG_TYPE_3)
	{
		SetAnimWorld();
		m_anim = CH_Anim_bReactionStart3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart3];
		m_bIsDone = false;
		m_bIsBlend = false;
		SOUND->Play("PCBigDamaged");
	}

	if (m_fHpCur < 0)
	{
		SOUND->Play("PCDie");
		Die();
	}

	if (rand() % 100 < per)
	{
		if (con == m_eCondition)
		{
			m_fHpCur = 0;
			Die();
			return;
		}
		else if (con == CDT_BURN && m_eCondition == CDT_ICE)
			m_eCondition = CDT_NORMAL;
		else if (con == CDT_ICE && m_eCondition == CDT_BURN)
			m_eCondition = CDT_NORMAL;
		else
			m_eCondition = con;

		switch (m_eCondition)
		{
		case CDT_NORMAL:
			m_pConditionBurn->End();
			m_pConditionIce->End();
			m_fSpeed = 1.0f;
			break;
		case CDT_BURN:
			m_pConditionBurn->Start();
			break;
		case CDT_ICE:
			m_pConditionIce->Start();
			m_fSpeed = 0.6;
			break;
		case CDT_STURN:
			break;
		default:
			break;
		}
	}
}

void cCharacterCtrl::BigDamaged()
{
	if (KEY->IsOnceKeyDown('E'))
	{
		SetAnimWorld();

		m_anim = CH_Anim_bReactionStart3;
		m_fCurAnimTime = m_fAnimTime[CH_Anim_bReactionStart3];
		m_bIsDone = false;
		m_bIsBlend = false;
	}
}

void cCharacterCtrl::Die()
{
	m_fHpCur = 0.0f;

	SetAnimWorld();
	m_anim = CH_Anim_Death;
	m_fCurAnimTime = m_fAnimTime[CH_Anim_Death];
	m_bIsDone = false;
	m_bIsBlend = false;
}

void cCharacterCtrl::SkillProcess()
{
	D3DXMATRIX matR, matT;

	// 이펙트 있는 스킬의 타격, 파티클, 사운드 처리
	if (!m_isDoEffect)
	{
		D3DXMatrixRotationY(&matR, m_fRotY);
		D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

		if (m_anim == CH_Anim_gaiaCrush02)
		{
			if (m_fTime >= m_fAnimTime[CH_Anim_gaiaCrush02] - 0.85f)
			{
				m_isDoEffect = true;
				m_pParticleSet->SetWorld(matR * matT);
				m_pParticleSet->Start();

				// 바운딩 박스 만들어서 몹과 충돌
				cBoundingBox hitBox;
				hitBox.Setup(D3DXVECTOR3(0, 0, -60), D3DXVECTOR3(100, 10, 60));
				hitBox.SetWorld(matR * matT);
				OBJECTMANAGER->GiveDamagedMonster(&hitBox, m_fAttack * 3.0f);
				CAMERA->Shaking(0.275f);
				//SOUND->Play("PCSkill01");
				SOUND->Play("Char_Skill_Explosion");
			}

		}
		else if (m_anim == CH_Anim_CuttingSlash && m_fTime >= m_fAnimTime[CH_Anim_CuttingSlash] - 1.1f)
		{
			m_isDoEffect = true;
			m_pParticleSet->SetWorld(matR * matT);
			m_pParticleSet->Start();

			// 바운딩 박스 만들어서 몹과 충돌
			cBoundingBox hitBox;
			hitBox.Setup(D3DXVECTOR3(0, 0, -60), D3DXVECTOR3(100, 10, 60));
			hitBox.SetWorld(matR * matT);
			OBJECTMANAGER->GiveDamagedMonster(&hitBox, m_fAttack * 3.0f);
			CAMERA->Shaking(0.275f);
			SOUND->Play("Char_Skill_Explosion");
		}
	}
}

void cCharacterCtrl::AttSound()
{
	if (!m_isDoSkiilSound)
	{
		if (m_anim == CH_Anim_combo1 && m_fTime >= m_fAnimTime[CH_Anim_combo1] - 0.5f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCAtt01");
		}
		else if (m_anim == CH_Anim_combo2 && m_fTime >= m_fAnimTime[CH_Anim_combo2] - 0.5f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCAtt02");
		}
		else if (m_anim == CH_Anim_combo3 && m_fTime >= m_fAnimTime[CH_Anim_combo3] - 0.4f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCAtt03");
		}
		else if (m_anim == CH_Anim_combo4 && m_fTime >= m_fAnimTime[CH_Anim_combo4] - 1.2f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCAtt04");
		}
		else if (m_anim == CH_Anim_gaiaCrush02 && m_fTime >= m_fAnimTime[CH_Anim_gaiaCrush02] - 1.6f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCSkill01");
		}
		else if (m_anim == CH_Anim_CuttingSlash && m_fTime >= m_fAnimTime[CH_Anim_CuttingSlash] - 1.5f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCSkill01");
		}
		else if (m_anim == CH_Anim_CutHead && m_fTime >= m_fAnimTime[CH_Anim_CutHead] - 1.5f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCSkill01");
		}
		else if (m_anim == CH_Anim_StingerBlade && m_fTime >= m_fAnimTime[CH_Anim_StingerBlade] - 1.5f)
		{
			m_isDoSkiilSound = true;
			SOUND->Play("PCSkill01");
		}
	}
}

void cCharacterCtrl::GetAngle()
{
	if (KEY->IsStayKeyDown('A'))
	{
		m_fRotY = m_fCosVal + D3DX_PI * 1.5f;
	}
	else if (KEY->IsStayKeyDown('D'))
	{
		m_fRotY = m_fCosVal + D3DX_PI * 0.5;
	}
	if (KEY->IsStayKeyDown('W'))
	{
		if (KEY->IsStayKeyDown('A'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 1.75f;
		}
		else if (KEY->IsStayKeyDown('D'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 0.25;
		}
		else
		{
			m_fRotY = m_fCosVal;
		}
	}
	if (KEY->IsStayKeyDown('S'))
	{
		if (KEY->IsStayKeyDown('A'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 1.25f;
		}
		else if (KEY->IsStayKeyDown('D'))
		{
			m_fRotY = m_fCosVal + D3DX_PI * 0.75;
		}
		else
		{
			m_fRotY = m_fCosVal + D3DX_PI;
		}
	}

	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matR * matT;
}
