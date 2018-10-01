#include "stdafx.h"
#include "cUIManager.h"
#include "Framework/UI/cUIInfo.h"
#include "cItemManager.h"
#include "Framework/UI/cUIObject.h"
#include "Framework/UI/cUIButton.h"
#include "MainGame/Item/cItemInfo.h"
#include "Framework/UI/cUIImageView.h"


cUIManager::cUIManager()
	:m_nAlpha(200)
	, m_isCallQuickSlot(true)
	, m_isQuickSlotLock(false)
{
}


cUIManager::~cUIManager()
{

}

void cUIManager::Setup()
{
	hdc = GetDC(hWnd);

	CreateUIList();
	ConnectNode();

	SOUND->AddSound("CloseInterface", "Sound/Interface/CloseInterface.ogg");
	SOUND->AddSound("EquitArmor", "Sound/Interface/EquitArmor.ogg");
	SOUND->AddSound("EquitWeapon", "Sound/Interface/EquitWeapon.ogg");
	SOUND->AddSound("OpenInterface", "Sound/Interface/OpenInterface.ogg");
	SOUND->AddSound("Trade", "Sound/Interface/Trade.ogg");

}

void cUIManager::Update()
{


	//closeIdel 뒤의 I = Inventory S = Status C = ConsumablesShop
	CloseUI("closeI");
	CloseUI("closeS");
	CloseUI("closeC");

	CallKeyInput();

	if (m_isCallQuickSlot)	CallUIUpdate("QSDRAGZONE");
	if (m_isCallInven)		CallUIUpdate("Inventory");
	if (m_isCallStatus)		CallUIUpdate("Status");
	if (m_isCallConShop)	CallUIUpdate("ConShop");

	QuickSlotCall("QSSKILLLOCK");
	QuickSlotCall("QSSKILLLOCK2");
	QuickSlotOptionHiding();


}

void cUIManager::Render()
{

	if (m_isCallQuickSlot)CallUIRender("QSDRAGZONE");
	if (m_isCallInven)CallUIRender("Inventory");
	if (m_isCallStatus)CallUIRender("Status");
	if (m_isCallConShop)CallUIRender("ConShop");

	//텍스트정보보는함수
	UIInfoTextPopUp(NULL, "Status");
	UIInfoTextPopUp(m_vUI.size());


}

void cUIManager::Destroy()
{
	for (auto p : m_vUI)
	{
		SAFE_DELETE(p);
	}
}

void cUIManager::CreateUI(UIType _UIType, const char* UIName, const char* filePath, D3DXVECTOR3 vec3Pos, D3DXVECTOR2 vec2ReduceDragSize, const char* UIParentName)
{
	m_pUIInfo = new cUI;

	tagUIInfo _tagUIInfo;

	_tagUIInfo._UIType = _UIType;
	_tagUIInfo._UIName = UIName;
	_tagUIInfo._UIPath1 = filePath;
	_tagUIInfo._UIPos = vec3Pos;
	_tagUIInfo._UIParentName = UIParentName;
	_tagUIInfo._UIReduceDragSize = vec2ReduceDragSize;

	if (strncmp(UIName, "QSSkill", 7) == 0)
		m_vQuickSlotUI.push_back(m_pUIInfo);


	m_pUIInfo->Setup(&_tagUIInfo);
	m_vUI.push_back(m_pUIInfo);
}

void cUIManager::CreateUI(UIType _UIType, const char * UIName, const char * filePath1, const char * filePath2, const char * filePath3, D3DXVECTOR3 vec3Pos, const char * UIParentName)
{

	m_pUIInfo = new cUI;

	tagUIInfo _tagUIInfo;

	_tagUIInfo._UIType = _UIType;
	_tagUIInfo._UIName = UIName;
	_tagUIInfo._UIPath1 = filePath1;
	_tagUIInfo._UIPath2 = filePath2;
	_tagUIInfo._UIPath3 = filePath3;
	_tagUIInfo._UIPos = vec3Pos;
	_tagUIInfo._UIParentName = UIParentName;


	m_pUIInfo->Setup(&_tagUIInfo);
	m_vUI.push_back(m_pUIInfo);
}


void cUIManager::ConnectNode()
{
	for (int i = 0; i < m_vUI.size(); i++)
	{
		for (int j = 0; j < m_vUI.size(); j++)
		{
			//부모이름과 같은 네임을 가진 녀석을 찾고
			//부모이름 가진녀석에게 루트를 설정해줌
			if (m_vUI[i]->GetParentName() == m_vUI[j]->GetName())
			{
				if (!m_vUI[j]->GetParentName())m_vUI[i]->ConnectNode(m_vUI[j]->GetUIRoot());
				else m_vUI[i]->ConnectNode(m_vUI[j]->GetUIImage());
			}
		}
	}
}


void cUIManager::CallUIUpdate(const char* UIname)
{
	for (int i = 0; i < m_vUI.size(); i++)
	{

		if (strncmp(m_vUI[i]->GetName(), UIname, 4) == 0)
		{
			m_vUI[i]->Update();
		}

	}

}

void cUIManager::CallUIRender(const char* UIname)
{
	for (int i = 0; i < m_vUI.size(); i++)
	{
		if (strncmp(m_vUI[i]->GetName(), UIname, 4) == 0)
		{
			m_vUI[i]->Render();
		}
	}
}

void cUIManager::CallKeyInput()
{
	if (KEY->IsOnceKeyDown(VK_ESCAPE))
	{
		if (isOptionMode)
		{
			if (m_isCallInven || m_isCallStatus)
				SOUND->Play("CloseInterface");
			isOptionMode = false;
			CAMERA->SetType(CAMERA_FREE);
			m_isCallInven = false;
			m_isCallConShop = false;
			m_isCallStatus = false;
		}
		else
		{
			isOptionMode = true;
			CAMERA->SetType(CAMERA_FIX);
		}
	}

	if (!m_isCallInven && KEY->IsOnceKeyDown(VK_NUMPAD3))
	{
		m_isCallInven = true;
		isOptionMode = true;
		CAMERA->SetType(CAMERA_FIX);
		SOUND->Play("OpenInterface");
	}
	else if (m_isCallInven && KEY->IsOnceKeyDown(VK_NUMPAD3))
	{
		m_isCallInven = false;
		SOUND->Play("CloseInterface");
	}
	if (!m_isCallConShop && KEY->IsOnceKeyDown(VK_NUMPAD2))
	{
		m_isCallConShop = true;
		isOptionMode = true;
		CAMERA->SetType(CAMERA_FIX);
		SOUND->Play("OpenInterface");
	}
	else if (m_isCallConShop && KEY->IsOnceKeyDown(VK_NUMPAD2))
	{
		m_isCallConShop = false;
		SOUND->Play("CloseInterface");
	}
	if (!m_isCallStatus && KEY->IsOnceKeyDown(VK_NUMPAD1))
	{
		m_isCallStatus = true;
		isOptionMode = true;
		CAMERA->SetType(CAMERA_FIX);
		SOUND->Play("OpenInterface");
	}
	else if (m_isCallStatus && KEY->IsOnceKeyDown(VK_NUMPAD1))
	{
		m_isCallStatus = false;
		SOUND->Play("CloseInterface");
	}
	if (!m_isCallQuickSlot && KEY->IsOnceKeyDown(VK_NUMPAD4))
	{
		m_isCallQuickSlot = true;
		isOptionMode = true;
		CAMERA->SetType(CAMERA_FIX);
		SOUND->Play("OpenInterface");
	}
	else if (m_isCallQuickSlot && KEY->IsOnceKeyDown(VK_NUMPAD4))
	{
		m_isCallQuickSlot = false;
		SOUND->Play("CloseInterface");
	}
}

void cUIManager::CloseUI(const char* szUIName)
{
	int Index = FindUIRootIndexFull(szUIName);

	RECT rc = m_vUI[Index]->GetUIButtonImage()->GetCollisionRect();

	if (PtInRect(&rc, ptMouse))
	{
		if (KEY->IsOnceKeyDown(VK_LBUTTON))
		{
			if (szUIName == "closeI") m_isCallInven = false;
			else if (szUIName == "closeS") m_isCallStatus = false;
			else if (szUIName == "closeC") m_isCallConShop = false;
		}
		else if (KEY->IsOnceKeyDown(VK_LBUTTON))
		{
			if (szUIName == "closeI") m_isCallInven = true;
			else if (szUIName == "closeS") m_isCallStatus = true;
			else if (szUIName == "closeC") m_isCallConShop = true;
		}
	}
}

void cUIManager::QuickSlotCall(const char* callName)
{
	if (KEY->IsOnceKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < m_vUI.size(); i++)
		{
			if (m_vUI[i]->GetName() == callName)
			{
				if (m_vUI[i]->GetUIButtonImage()->GetIsColDragRcAndPT())
				{
					for (int j = 0; j < m_vUI[i]->GetUIButtonImage()->GetParent()->GetVecChild().size(); j++)
					{
						m_vUI[i]->GetUIButtonImage()->GetParent()->GetVecChild()[j]->
							SetMove({ 0,0 });
					}
				}
			}
		}
	}
}

int cUIManager::FindUIRootIndex(const char* szFindIndex)
{


	for (int i = 0; i < m_vUI.size(); i++)
	{
		if (strncmp(m_vUI[i]->GetName(), szFindIndex, 4) == 0)
		{
			return i;
		}
	}
}

int cUIManager::FindUIRootIndexFull(const char * szFindIndex)
{
	for (int i = 0; i < m_vUI.size(); i++)
	{
		if (m_vUI[i]->GetName() == szFindIndex)
		{
			return i;
		}
	}

}

void cUIManager::UIInfoTextPopUp(int oneValue, const char* szNecessaryPlace)
{
	char szTemp[1024];

	if (szNecessaryPlace != NULL)
	{
		int FindIndex = FindUIRootIndex(szNecessaryPlace);
		sprintf_s(szTemp, 1024, "x = %d \t y = %d", (int)m_vUI[FindIndex]->GetUIRoot()->GetMatWorld()._41 - ptMouse.x
			, (int)m_vUI[FindIndex]->GetUIRoot()->GetMatWorld()._42 - ptMouse.y);
	}

	else
	{
		sprintf_s(szTemp, 1024, "%d", oneValue);
	}


	RECT rc;
	SetRect(&rc, 100, 100, 800, 200);

	LPD3DXFONT pFont = FONT->GetFont(cFontManager::FT_GA_BIG);
	pFont->DrawTextA(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP,
		D3DCOLOR_XRGB(255, 255, 0));

}

void cUIManager::UIMoveControl()
{
	for (int i = 0; i < m_vUI.size(); i++)
	{
		int InvenIndex = FindUIRootIndex("Inventory");
		int StatusIndex = FindUIRootIndex("Status");
		int ConShop = FindUIRootIndex("ConShop");

		if (i == InvenIndex || i == StatusIndex || i == ConShop) continue;
		m_vUI[i]->GetUIRoot()->SetIsMove(false);
	}
}

void cUIManager::QuickSlotOptionHiding()
{
	if (!isOptionMode)
	{
		for (int i = 0; i < m_vUI.size(); i++)
		{
			if (strncmp(m_vUI[i]->GetName(), "QS", 2) == 0)
			{
				if (m_vUI[i]->GetUIRoot())
				{
					m_vUI[i]->GetUIRoot()->SetAlpha(0);
					for (int j = 0; j < m_vUI[i]->GetUIRoot()->GetVecChild().size(); j++)
					{
						m_vUI[i]->GetUIRoot()->GetVecChild()[j]->SetAlpha(0);
					}
				}
				else if (m_vUI[i]->GetUIButtonImage())m_vUI[i]->GetUIButtonImage()->SetAlpha(0);
			}
		}
	}
	else
	{
		for (int i = 0; i < m_vUI.size(); i++)
		{
			if (strncmp(m_vUI[i]->GetName(), "QS", 2) == 0)
			{
				if (m_vUI[i]->GetUIRoot())
				{
					m_vUI[i]->GetUIRoot()->SetAlpha(m_nAlpha);
					for (int j = 0; j < m_vUI[i]->GetUIRoot()->GetVecChild().size(); j++)
					{
						m_vUI[i]->GetUIRoot()->GetVecChild()[j]->SetAlpha(m_nAlpha);
					}
				}
				else if (m_vUI[i]->GetUIButtonImage())m_vUI[i]->GetUIButtonImage()->SetAlpha(m_nAlpha);
			}
		}
	}
}
