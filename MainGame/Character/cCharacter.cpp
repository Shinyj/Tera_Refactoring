#include "stdafx.h"
#include "cCharacter.h"

#include "MainGame\Item\cItemInfo.h"
#include "MainGame\Item\cItem.h"
#include "MainGame\Map\iMap.h"
cCharacter::cCharacter()
	: m_fRotY(0.0f)
	, m_vDirection(1, 0, 0)
	, m_vPosition(0, 0, 0)
	, m_fSpeed(1.0f)
	, m_vBeforeAnimPos(0, 0, 0)
	, m_vCurAnimPos(0, 0, 0)
	, m_fHpMax(300.0f)
	, m_fHpCur(300.0f)
	, m_fMpMax(150.0f)
	, m_fMpCur(150.0f)
	, m_pHpBar(NULL)
	, m_pMpBar(NULL)
	, m_BackBar(NULL)
	, m_pMap(NULL)
	, m_pBleedingAlpha(0)
	, m_fAttack(20.0f)
	, m_fDefense(10.0f)
	, m_fDotDamagedTime(0.0f)
	, m_pConditionAlpha(0)
	, m_isRun(false)
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matAnimWorld);
}


cCharacter::~cCharacter()
{
	SAFE_DELETE(m_pHpBar);
	SAFE_DELETE(m_pMpBar);
	SAFE_DELETE(m_BackBar);
	SAFE_DELETE(m_pBleeding);
	SAFE_DELETE(m_pBurnning);
	SAFE_DELETE(m_pIcing);
}

void cCharacter::Setup()
{
	cGameObject::Setup();

	SetUpStateBar();

	m_pBleeding = TEXTURE->GetSprite(EffectTexPath + L"bleeding.png");
	m_pBurnning = TEXTURE->GetSprite(EffectTexPath + L"fireScreen.png");
	m_pIcing = TEXTURE->GetSprite(EffectTexPath + L"iceScreen.png");

	D3DXMATRIX mat;
	D3DXMatrixScaling(&mat, (float)WINSIZEX / m_pBleeding->textureInfo.Width,
		(float)WINSIZEY / m_pBleeding->textureInfo.Height, 1);
	m_pBleeding->m_pSprite->SetTransform(&mat);
	D3DXMatrixScaling(&mat, (float)WINSIZEX / m_pIcing->textureInfo.Width,
		(float)WINSIZEY / m_pIcing->textureInfo.Height, 1);
	m_pIcing->m_pSprite->SetTransform(&mat);
	D3DXMatrixScaling(&mat, (float)WINSIZEX / m_pBurnning->textureInfo.Width,
		(float)WINSIZEY / m_pBurnning->textureInfo.Height, 1);
	m_pBurnning->m_pSprite->SetTransform(&mat);

	m_pConditionBurn = PARTICLE->GetParticle("CDT_Burn");
	PARTICLE->AddChild(m_pConditionBurn);
	m_pConditionIce = PARTICLE->GetParticle("CDT_Ice");
	PARTICLE->AddChild(m_pConditionIce);

	m_pUseHPotion = PARTICLE->GetParticle("HpPotion");
	PARTICLE->AddChild(m_pUseHPotion);
	m_pUseMpotion = PARTICLE->GetParticle("MpPotion");
	PARTICLE->AddChild(m_pUseMpotion);


}

void cCharacter::Update()
{
	m_fMpCur += 0.005;
	if (m_fMpCur > m_fMpMax)m_fMpCur = m_fMpMax;

	UpdateUpStateBar();
	PlusMapHeight();

	UseQuickSlot();

	if (m_pBleedingAlpha > 0)
		m_pBleedingAlpha -= 2;
	//m_matWorld._42 = m_vPosition.y;

	ITEMMANAGER->SetAttackValue(m_fAttack);
	ITEMMANAGER->SetDefenceValue(m_fDefense);

	Condition_Update();
	CountPossibleDamaged(3.0f);
}

void cCharacter::Render()
{
	RenderUpStateBar();
	//AlphaRender AlphaRenderWinSize
	m_pBleeding->AlphaRender(D3DXVECTOR3(WINSIZEX / 2, 0, 0), D3DXVECTOR3(WINSIZEX / 2, 0, 0), m_pBleedingAlpha);

	switch (m_eCondition)
	{
	case CDT_ICE: m_pIcing->AlphaRender(D3DXVECTOR3(WINSIZEX / 2, 0, 0), D3DXVECTOR3(WINSIZEX / 2, 0, 0), m_pConditionAlpha);
		break;
	case CDT_BURN: m_pBurnning->AlphaRender(D3DXVECTOR3(WINSIZEX / 2, 0, 0), D3DXVECTOR3(WINSIZEX / 2, 0, 0), m_pConditionAlpha);

		break;
	}

}

void cCharacter::PlusMapHeight()
{
	if (m_pMap)
		m_pMap->GetHeight(m_vPosition.x, m_vPosition.y, m_vPosition.z);
}

void cCharacter::SetUpStateBar()
{

	m_BackBar = TEXTURE->GetSprite(CharaInfoTexPath + L"CharacterInfo.png");

	m_pHpBar = new ProgressBar;
	m_pHpBar->Setup("Resource/Texture/CharacterInfo/HP.png",
		"Resource/Texture/CharacterInfo/HPLose.png",
		UIX + 19 + m_BackBar->textureInfo.Width / 7.0f,
		UIY + 26,
		WINSIZEX / 3.0f, WINSIZEY / 30.0f);

	m_pMpBar = new ProgressBar;
	m_pMpBar->Setup("Resource/Texture/CharacterInfo/MP.png",
		"Resource/Texture/CharacterInfo/MPLose.png",
		UIX + 19 + m_BackBar->textureInfo.Width / 7.0f,
		UIY + 32 + WINSIZEY / 30.0f, // 6차이
		WINSIZEX / 3.0f, WINSIZEY / 30.0f);
}

void cCharacter::UpdateUpStateBar()
{
	m_pHpBar->SetGauge(m_fHpCur, m_fHpMax);
	m_pMpBar->SetGauge(m_fMpCur, m_fMpMax);

}

void cCharacter::RenderUpStateBar()
{
	m_BackBar->Render(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(UIX + 20, UIY + 20, 0));
	m_pHpBar->Render();
	m_pMpBar->Render();

	// 체력 숫자 렌더
	char szTemp[1024];
	RECT rc;

	sprintf_s(szTemp, 1024, "%d / %d", (int)m_fHpCur, (int)m_fHpMax);
	SetRect(&rc,
		UIX + 300,
		UIY + 26,
		UIX + 300 + m_BackBar->textureInfo.Width,
		UIY + 26 + 26);

	LPD3DXFONT pFont = FONT->GetFont(cFontManager::TF_UI_NUMBER);
	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
		DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(szTemp, 1024, "%d / %d", (int)m_fMpCur, (int)m_fMpMax);
	SetRect(&rc,
		UIX + 300,
		UIY + 66,
		UIX + 300 + m_BackBar->textureInfo.Width,
		UIY + 66 + 26);

	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
		DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));


	pFont = FONT->GetFont(cFontManager::TF_UI_TEXT);

	sprintf_s(szTemp, 1024, "HP", (int)m_fMpCur, (int)m_fMpMax);
	SetRect(&rc,
		UIX + 120,
		UIY + 26,
		UIX + 350,
		UIY + 26 + 26);

	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
		DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));

	sprintf_s(szTemp, 1024, "MP", (int)m_fMpCur, (int)m_fMpMax);
	SetRect(&rc,
		UIX + 120,
		UIY + 66,
		UIX + 350,
		UIY + 66 + 26);

	pFont->DrawTextA(NULL, szTemp, strlen(szTemp), &rc,
		DT_LEFT | DT_VCENTER, D3DCOLOR_XRGB(255, 255, 255));
}

void cCharacter::Damaged()
{
	m_pBleedingAlpha = 120;
}

void cCharacter::Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per)
{
}

void cCharacter::Damaged(float damage, D3DXVECTOR3 pos, DAMAGED_TYPE type)
{
}

void cCharacter::Damaged(float damage, D3DXVECTOR3 pos, CONDITION con, float per, DAMAGED_TYPE type)
{
}

bool cCharacter::Attack(float damage)
{
	return false;
}

int cCharacter::ChangeEquit()
{
	vector<cItemInfo*> vec = (ITEMMANAGER->GetStatusItem());

	bool isEquitWeapon = false;
	bool isEquitArmor = false;
	bool isEquitHand = false;
	bool isEquitLeg = false;

	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i]->GetItemKind() == WEAPON)
		{
			isEquitWeapon = true;

			if (m_pEquitWeapon)
			{
				if (m_pEquitWeapon->GetItemKind() == vec[i]->GetItemKind())
					if (strcmp(m_pEquitWeapon->GetName(), vec[i]->GetName()) != 0)
					{
						m_fAttack -= m_pEquitWeapon->GetAbilityValue();
						m_pEquitWeapon = vec[i];
						m_fAttack += m_pEquitWeapon->GetAbilityValue();
						return 1;
					}
			}
			else
			{
				m_pEquitWeapon = vec[i];
				m_fAttack += m_pEquitWeapon->GetAbilityValue();
				return 1;

			}
		}
		if (vec[i]->GetItemKind() == ARMOR)
		{
			isEquitArmor = true;

			if (m_pEquitBody)
			{
				if (m_pEquitBody->GetItemKind() == vec[i]->GetItemKind())
					if (strcmp(m_pEquitBody->GetName(), vec[i]->GetName()) != 0)
					{
						m_fDefense -= m_pEquitBody->GetAbilityValue();
						m_pEquitBody = vec[i];
						m_fDefense += m_pEquitBody->GetAbilityValue();
						return 2;
					}
			}
			else
			{
				m_pEquitBody = vec[i];
				m_fDefense += m_pEquitBody->GetAbilityValue();
				return 2;
			}
		}
		if (vec[i]->GetItemKind() == GLOVES)
		{
			isEquitHand = true;

			if (m_pEquitHand)
			{
				if (m_pEquitHand->GetItemKind() == vec[i]->GetItemKind())
					if (strcmp(m_pEquitHand->GetName(), vec[i]->GetName()) != 0)
					{
						m_fDefense -= m_pEquitHand->GetAbilityValue();
						m_pEquitHand = vec[i];
						m_fDefense += m_pEquitHand->GetAbilityValue();
						return 3;
					}
			}
			else
			{
				m_pEquitHand = vec[i];
				m_fDefense += m_pEquitHand->GetAbilityValue();
				return 3;
			}
		}
		if (vec[i]->GetItemKind() == SHOES)
		{
			isEquitLeg = true;

			if (m_pEquitLeg)
			{
				if (m_pEquitLeg->GetItemKind() == vec[i]->GetItemKind())
					if (strcmp(m_pEquitLeg->GetName(), vec[i]->GetName()) != 0)
					{
						m_fDefense -= m_pEquitLeg->GetAbilityValue();
						m_pEquitLeg = vec[i];
						m_fDefense += m_pEquitLeg->GetAbilityValue();
						return 4;
					}
			}
			else
			{
				m_pEquitLeg = vec[i];
				m_fDefense += m_pEquitLeg->GetAbilityValue();
				return 4;
			}
		}
	}

	if (isEquitWeapon == false && m_pEquitWeapon != NULL)
	{
		m_fAttack -= m_pEquitWeapon->GetAbilityValue();
		m_pEquitWeapon = NULL;
		return 1;
	}
	if (isEquitArmor == false && m_pEquitBody != NULL)
	{
		m_fDefense -= m_pEquitBody->GetAbilityValue();
		m_pEquitBody = NULL;
		return 2;
	}
	if (isEquitHand == false && m_pEquitHand != NULL)
	{
		m_fDefense -= m_pEquitHand->GetAbilityValue();
		m_pEquitHand = NULL;
		return 3;
	}
	if (isEquitLeg == false && m_pEquitLeg != NULL)
	{
		m_fDefense -= m_pEquitLeg->GetAbilityValue();
		m_pEquitLeg = NULL;
		return 4;
	}

	return 0;
}

void cCharacter::Condition_Update()
{
	if (m_pConditionAlpha > 0)
		m_pConditionAlpha -= 2;

	switch (m_eCondition)
	{
	case CDT_ICE:
		if (m_fHpCur != 0)
		{
			m_fDotDamagedTime += TIME->GetEllapsedTime();
			if (m_fDotDamagedTime > 1.0f)
			{
				m_fDotDamagedTime = 0.0f;
				m_pConditionAlpha = 120;
				m_fHpCur -= (m_fHpMax * 0.004);
			}
			m_pConditionIce->SetWorld(m_matWorld);
		}
		break;
	case CDT_BURN:
		if (m_fHpCur != 0)
		{
			m_fDotDamagedTime += TIME->GetEllapsedTime();
			if (m_fDotDamagedTime > 1.0f)
			{
				m_fDotDamagedTime = 0.0f;
				m_pConditionAlpha = 120;
				m_fHpCur -= (m_fHpMax * 0.008);
			}
			m_pConditionBurn->SetWorld(m_matWorld);
		}
		break;
	}

}

void cCharacter::Condition_Render()
{
}

void cCharacter::UseQuickSlot()
{
	vector<cItemInfo*> m_vItem = ITEMMANAGER->GetQuickItem();

	for (int i = 0; i < m_vItem.size(); i++)
	{
		if (KEY->IsOnceKeyDown('5'))
		{
			int a = 10;
		}
		if (KEY->IsOnceKeyDown(m_vItem[i]->GetQuickSlotNum() + 1 + '0'))
		{
			if (m_vItem[i]->GetItemKind() == HPOTION && m_vItem[i]->GetPotionCount() > 0)
			{
				m_fHpCur += m_vItem[i]->GetAbilityValue();
				if (m_fHpCur > m_fHpMax) m_fHpCur = m_fHpMax;
				// 파티클 효과
				m_pUseHPotion->SetWorld(m_matWorld);
				m_pUseHPotion->Start();

				int cnt = m_vItem[i]->GetPotionCount();
				cnt--;
				if (cnt < 0) cnt = 0;
				m_vItem[i]->SetPotionCount(cnt);

				SOUND->Play("Use_PotionDrink");
			}
			else if (m_vItem[i]->GetItemKind() == MPOTION && m_vItem[i]->GetPotionCount() > 0)
			{
				m_fMpCur += m_vItem[i]->GetAbilityValue();
				if (m_fMpCur > m_fMpMax) m_fMpCur = m_fMpMax;
				// 파티클 효과
				m_pUseMpotion->SetWorld(m_matWorld);
				m_pUseMpotion->Start();

				int cnt = m_vItem[i]->GetPotionCount();
				cnt--;
				if (cnt < 0) cnt = 0;
				m_vItem[i]->SetPotionCount(cnt);

				SOUND->Play("Use_PotionDrink");
			}

		}
	}
}