#include "stdafx.h"

void cUIManager::CreateUIList()
{


	//인벤토리
	CreateUI(WINDOW, "Inventory", "Resource/Texture/UIIcon/Inven.png", D3DXVECTOR3(1100, 100, 0), D3DXVECTOR2(0.8,0.1));
	CreateUI(BUTTON, "closeI", "Resource/Texture/UIIcon/closeIdle.png", "Resource/Texture/UIIcon/closeOver.png", "Resource/Texture/UIIcon/closeDown.png",
		D3DXVECTOR3(m_vInventoryPos.x + 340, m_vInventoryPos.y + 5, 0), "Inventory");
	CreateUI(WINDOW, "Pocket", "Resource/Texture/UIIcon/InvenPocket2.png", D3DXVECTOR3(m_vInventoryPos.x - 20, m_vInventoryPos.y - 10, 0), { -1,-1 }, "Inventory");
	CreateUI(WINDOW, "Gold", "Resource/Texture/UIIcon/Gold.png", D3DXVECTOR3(155, 333, 0), { -1, -1 }, "Inventory");
	
	//상점
	CreateUI(WINDOW, "ConShop", "Resource/Texture/UIIcon/Shop.png", D3DXVECTOR3(600, 200, 0), D3DXVECTOR2(0.8, 0.1));
	CreateUI(WINDOW, "ConsumablesIcon", "Resource/Texture/UIIcon/ConsumablesShopIcon.png", D3DXVECTOR3(m_vShopPos.x - 20, m_vShopPos.y - 20, 0), { -1,-1 }, "ConShop");
	CreateUI(BUTTON, "closeC", "Resource/Texture/UIIcon/closeIdle.png", "Resource/Texture/UIIcon/closeOver.png", "Resource/Texture/UIIcon/closeDown.png",
		D3DXVECTOR3(m_vShopPos.x + 340, m_vShopPos.y + 5, 0), "ConShop");
	CreateUI(BUTTON, "SaleBT", "Resource/Texture/UIIcon/TotalSalesAmountIdle.png", "Resource/Texture/UIIcon/TotalSalesAmountOver.png"
		, "Resource/Texture/UIIcon/TotalSalesAmountDown.png", D3DXVECTOR3(m_vShopPos.x + 300, m_vShopPos.y + 255, 0), "ConShop");

	//장비창
	CreateUI(WINDOW, "Status", "Resource/Texture/UIIcon/EquipmentWindow.png", D3DXVECTOR3(200, 200, 0), D3DXVECTOR2(0.8, 0.1));
	CreateUI(BUTTON, "closeS", "Resource/Texture/UIIcon/closeIdle.png", "Resource/Texture/UIIcon/closeOver.png", "Resource/Texture/UIIcon/closeDown.png",
		D3DXVECTOR3(m_vShopPos.x + 255, m_vShopPos.y + 3, 0), "Status");
	CreateUI(WINDOW, "EquipmentPort", "Resource/Texture/UIIcon/EquipmentPort.png", D3DXVECTOR3(-50, -50, 0), { -1,-1 }, "Status");




	CreateUI(WINDOW, "QSDRAGZONE", "Resource/Texture/UIIcon/DragZone.png", D3DXVECTOR3(340, 730, 0), { 1,1 });
	CreateUI(BUTTON, "QSSKILLLOCK", "Resource/Texture/UIIcon/SkillLock.png", "Resource/Texture/UIIcon/SkillLockUp.png", "Resource/Texture/UIIcon/SkillKey.png"
		, D3DXVECTOR3(-17, 17, 0), "QSDRAGZONE");



	for (int i = 0; i < 16; i++)
	{
		szDragZone[i] = "QSDragZone";
		szSkillSlot[i] = "QSSkillSlot";

		_itoa(i, szNum, 10);

		szDragZone[i] += szNum;
		szSkillSlot[i] += szNum;

		szDragZoneC = szDragZone[i].c_str();
		szSkillSlotC = szSkillSlot[i].c_str();

		CreateUI(WINDOW, szDragZoneC, "Resource/Texture/UIIcon/DragZone.png", D3DXVECTOR3((i+1) * 50 - 11, -3, 0), { 1,1 }, "QSDRAGZONE");
		CreateUI(WINDOW, szSkillSlotC, "Resource/Texture/UIIcon/SkillSlot.png", D3DXVECTOR3(-8, 20, 0), { -1,-1 }, szDragZoneC);
		CreateUI(BUTTON, "QSKeyChangeBT", "Resource/Texture/UIIcon/KeyChangeIdle.png", "Resource/Texture/UIIcon/KeyChangeOver.png",
			"Resource/Texture/UIIcon/KeyChangeOver.png", D3DXVECTOR3(22, 0, 0), szDragZoneC);

	}




	//CreateUI(WINDOW, "SkillWnd", "Texture/UIIcon/SkillWnd.png", D3DXVECTOR3(100, 100, 0), D3DXVECTOR2(0.8, 0.1));
	


	


}


