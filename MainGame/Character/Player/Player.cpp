#include "stdafx.h"
#include "Player.h"

#include "MainGame\Item\cItem.h"
#include "MainGame\Item\Weapon\TwoHandClaymore.h"
#include "MainGame\Item\Weapon\TwohandFish.h"
#include "MainGame\Item\Weapon\TwohandIce.h"
#include "MainGame\Item\Weapon\TwohandSky.h"

Player::Player()
	: m_pBody(NULL)
	, m_currState(CH_Anim_AdvLeap)
	, m_pWeaponHand(NULL)
{
}


Player::~Player()
{
	SAFE_DELETE(m_pWeapon);
}

void Player::Setup()
{
	cCharacterCtrl::Setup();

	m_pHair = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Head", "Hair01.X");
	m_pBody = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_00.X");
	m_pHand = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_00.X");
	m_pLeg = SKINNEDMESH->GetSkinnedMesh( "Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_00.X");

	//////////////////////////////////////////////////////////////
	m_pWeapon = new TwoHandClaymore;
	m_pWeapon->Setup();

	// ���� ������ ������ ��
	m_pWeaponHand = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(),
		"R_Sword");

	// ��ġ�� ���� ��Ʈ ��
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(),
		"Dummy_root");

	// �󱼰� �Ӹ��� ������ �� ��
	m_pHead = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(),
		"Bip01-Head");

	// �ٿ�� �ڽ� ����
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(D3DXVECTOR3(-10, -15, -10), D3DXVECTOR3(10, 15, 10));

	// �� �浹 ���� ����
	m_pSpere = new cSpere;
	m_pSpere->Setup(D3DXVECTOR3(0, 0, 0), 20);

	g_vPlayerPos = &m_vPosition;
}

void Player::Update()
{
	// �ʰ����Ա�
	ChangeEquit();

	// �ִϸ��̼��� ���̿� ���̸� ����
	m_pDummyRoot->TransformationMatrix._42 = m_matWorld._42;

	// ī�޶� ���� ���� ����
	D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
	m_fCosVal = D3DXVec3Dot(&g_vCamera, &u);

	// nan �� ������ �ʰ� ����ó��..
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;


	m_fCosVal = acosf(m_fCosVal);

	if (u.z < g_vCamera.z)
		m_fCosVal = D3DX_PI * 2 - m_fCosVal;

	if (m_fCosVal < 0.0f)
		m_fCosVal += D3DX_PI * 2;
	else if (m_fCosVal > D3DX_PI * 2)
		m_fCosVal -= D3DX_PI * 2;

	// �ִϸ��̼� ����
	if (m_currState != m_anim)
	{
		if (m_bIsBlend)
		{
			m_pBody->SetAnimationIndexBlend(m_anim);
			m_pHand->SetAnimationIndexBlend(m_anim);
			m_pLeg->SetAnimationIndexBlend(m_anim);
			m_pHair->SetAnimationIndexBlend(m_anim);
		}
		else
		{
			m_pBody->SetAnimationIndex(m_anim);
			m_pHand->SetAnimationIndex(m_anim);
			m_pLeg->SetAnimationIndex(m_anim);
			m_pHair->SetAnimationIndex(m_anim);
		}

		m_currState = m_anim;

		m_pBody->AnimAdvanceTime();
		m_pHand->AnimAdvanceTime();
		m_pLeg->AnimAdvanceTime();
		m_pHair->AnimAdvanceTime();

		m_fTime += TIME->GetEllapsedTime();
	}

	// �̵����� �ִ� �ִϸ��̼��� ������Ʈ��
	// �ִϸ��̼� ��ŸƮ ������ ���忡�� ���� ������Ʈ
	// �ƴϸ� ���ó�� ����� ���忡�� ������Ʈ
	if (isUseLocalAnim())
	{
		m_matAnimWorld._42 = m_matWorld._42;

		m_pBody->Update(m_matAnimWorld);
		m_pHand->Update(m_matAnimWorld);
		m_pLeg->Update(m_matAnimWorld);
		m_pWeapon->SetWorld(m_pWeaponHand->CombinedTransformationMatrix);
		m_pHair->Update(m_pHead->CombinedTransformationMatrix);
	}
	else
	{
		m_pBody->Update(m_matWorld);
		m_pHand->Update(m_matWorld);
		m_pLeg->Update(m_matWorld);
		m_pWeapon->SetWorld(m_pWeaponHand->CombinedTransformationMatrix);
		m_pHair->Update(m_pHead->CombinedTransformationMatrix);
	}

	// �̵����� �ִ� �ִϸ��̼� ���� ��
	// �ִϸ��̼� ������ ���� ���������� �����Ű�� �������� ��� 
	m_vBeforeAnimPos = m_vCurAnimPos;
	m_vCurAnimPos = D3DXVECTOR3(m_pDummyRoot->TransformationMatrix._41, 0, 0);

	// ���Ⱑ �ִٸ� ���⸦ ������Ʈ
	if (m_pWeapon)
		m_pWeapon->Update();


	// ���� 
	AttackBoundBox();

	// ����
	cCharacterCtrl::Update();

	cGameObject::Update();
}

void Player::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DEVICE->SetTransform(D3DTS_WORLD, &mat);

	m_pBody->Render(NULL);
	m_pHand->Render(NULL);
	m_pLeg->Render(NULL);
	m_pHair->Render(NULL);

	m_pWeapon->Render();

	cGameObject::Render();

	cCharacterCtrl::Render();
}


bool Player::isUseLocalAnim()
{
	if (
		m_anim == CH_Anim_combo1 ||
		m_anim == CH_Anim_combo2 ||
		m_anim == CH_Anim_combo3 ||
		m_anim == CH_Anim_combo4 ||
		m_anim == CH_Anim_tumbling ||
		m_anim == CH_Anim_DwonBlow ||
		m_anim == CH_Anim_bReactionStart ||
		m_anim == CH_Anim_bReactionStart3 ||
		m_anim == CH_Anim_Death ||
		m_anim == CH_Anim_bReactionLand3 ||
		m_anim == CH_Anim_gaiaCrush01 ||
		m_anim == CH_Anim_gaiaCrush02 ||
		m_anim == CH_Anim_gaiaCrush03 ||
		m_anim == CH_Anim_CuttingSlash ||
		m_anim == CH_Anim_CutHead ||
		m_anim == CH_Anim_StingerBlade
		)
		return true;

	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x > 30.0f)
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		return true;
	}

	return false;
}

void Player::ChangeWeapon()
{
	if (m_pWeapon)
		SAFE_DELETE(m_pWeapon);

	if (m_pEquitWeapon->GetName() == "���")
	{
		m_pWeapon = new TwoHandClaymore;
		m_pWeapon->Setup();
	}
	else if (m_pEquitWeapon->GetName() == "��ī�̼ҵ�")
	{
		m_pWeapon = new TwohandIce;
		m_pWeapon->Setup();
	}
	else if (m_pEquitWeapon->GetName() == "��ġ")
	{
		m_pWeapon = new TwohandFish;
		m_pWeapon->Setup();

	}
	else if (m_pEquitWeapon->GetName() == "���̽��ҵ�")
	{
		m_pWeapon = new TwohandIce;
		m_pWeapon->Setup();
	}
}

void Player::ChangeBody()
{
	float position = m_pBody->GetAnimPosition();
	//m_pEquitBody->GetName();

	if (m_pEquitBody == NULL)
	{
		m_pBody = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_00.X");
		m_pBody->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitBody->GetName() == "�����Ƹ�")
	{
		m_pBody = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_01.X");
		m_pBody->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitBody->GetName() == "���������ǿ�")
	{
		m_pBody = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_02.X");
		m_pBody->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitBody->GetName() == "�����÷��Ƹ�")
	{
		m_pBody = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_03.X");
		m_pBody->SetAnimationIndex(m_currState);
	}

	m_pWeaponHand = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(), "R_Sword");
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(), "Dummy_root");
	m_pHead = (ST_BONE*)D3DXFrameFind(m_pBody->GetFrame(), "Bip01-Head");

	m_pBody->SetAnimPosition(position);
}

void Player::ChangeHand()
{
	float position = m_pHand->GetAnimPosition();

	if (m_pEquitHand == NULL)
	{
		m_pHand = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_00.X");
		m_pHand->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitHand->GetName() == "�����۷���")
	{
		m_pHand = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_01.X");
		m_pHand->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitHand->GetName() == "�����������尩")
	{
		m_pHand = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_02.X");
		m_pHand->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitHand->GetName() == "�����÷��۷���")
	{
		m_pHand = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_03.X");
		m_pHand->SetAnimationIndex(m_currState);
	}

	m_pHand->SetAnimPosition(position);
}

void Player::ChangeLeg()
{
	float position = m_pLeg->GetAnimPosition();

	if (m_pEquitLeg == NULL)
	{
		m_pLeg = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_00.X");
		m_pLeg->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitLeg->GetName() == "��������")
	{
		m_pLeg = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_01.X");
		m_pLeg->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitLeg->GetName() == "���������ǽŹ�")
	{
		m_pLeg = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_02.X");
		m_pLeg->SetAnimationIndex(m_currState);
	}
	else if (m_pEquitLeg->GetName() == "�����÷�����")
	{
		m_pLeg = SKINNEDMESH->GetSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_03.X");
		m_pLeg->SetAnimationIndex(m_currState);
	}

	m_pLeg->SetAnimPosition(position);
}

void Player::AttackBoundBox()
{
	// �� �ֵθ��� Ÿ�̹��� ���缭 ������ �ؾ���.. ( �غ��ڼ� �� ��Ʈ ������ �����ʰ� )
	
	if (m_anim == CH_Anim_combo1 && m_fTime > m_fAnimTime[CH_Anim_combo1] - 0.5f)
	{
		Attack(m_fAttack);

	}
	else if (m_anim == CH_Anim_combo2 && m_fTime >= m_fAnimTime[CH_Anim_combo2] - 0.5f)
	{
		Attack(m_fAttack);
		//SOUND->Play("WPN_Sword_Swing");
	}
	else if (m_anim == CH_Anim_combo3 && m_fTime >= m_fAnimTime[CH_Anim_combo3] - 0.4f)
	{
		Attack(m_fAttack);
		//SOUND->Play("WPN_Sword_Swing2");
	}
	else if (m_anim == CH_Anim_combo4 && m_fTime >= m_fAnimTime[CH_Anim_combo4] - 1.2f)
	{
		Attack(m_fAttack);
		//SOUND->Play("WPN_Sword_Swing");
	}
	else if (m_anim == CH_Anim_CutHead)
	{
		Attack(m_fAttack);
		//SOUND->Play("WPN_Sword_Swing2");
	}
	else if (m_anim == CH_Anim_StingerBlade)
	{
		Attack(m_fAttack);
		//SOUND->Play("WPN_Sword_Swing");
	}
}

bool Player::Attack(float damage)
{
	if (OBJECTMANAGER->GiveDamagedMonster(m_pWeapon->GetBoundingBox(), damage))
		return true;
	return false;
}

int Player::ChangeEquit()
{
	int n = 0;
	n = cCharacter::ChangeEquit();

	if (n)
	{
		switch (n)
		{
		case 1:
			SOUND->Play("EquitWeapon");
			ChangeWeapon();
			break;
		case 2:
			SOUND->Play("EquitArmor");
			ChangeBody();
			break;
		case 3:
			SOUND->Play("EquitArmor");
			ChangeHand();
			break;
		case 4:
			SOUND->Play("EquitArmor");
			ChangeLeg();
			break;
		}
	}
	return 0;
}
