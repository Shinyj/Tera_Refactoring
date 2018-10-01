#include "stdafx.h"
#include "scFieldLoading.h"


scFieldLoading::scFieldLoading()
	: m_pLoading(NULL)
{
}


scFieldLoading::~scFieldLoading()
{
	SAFE_DELETE(m_pLoading);
}

void scFieldLoading::Setup()
{
	m_pLoading = new Loading;

	// 여기서 레퍼런스 카운트가 증가
	m_pLoading->Setup("Resource/Texture/SceneTexture/LoadingTexture/LoadingImage_TIA_1.png");
	this->TotalLoading();

	isOptionMode = false;
	SOUND->Stop("Main");
	SOUND->AddSound("Loading", "Resource/Sound/Loading/LoadingSound.mp3", true, true);
	//SOUNDMANAGER->Play("Loading");
}

void scFieldLoading::Release()
{
	SAFE_DELETE(m_pLoading);
}

void scFieldLoading::Update()
{
	m_pLoading->Update();

	if (m_pLoading->LoadingDone())
	{
		//SOUNDMANAGER->Stop("Loading");
		//TEXTUREMANAGER->Destroy();
		CAMERA->SetType(CAMERA_FREE);
		SCENE->ChangeScene("Test");
	}
}

void scFieldLoading::Render()
{
	m_pLoading->Render();
}

void scFieldLoading::TotalLoading()
{
	// 파티클
	m_pLoading->IniteForParticle("explosion",	ParticleTexPath + L"explosion.txt");
	m_pLoading->IniteForParticle("iceExplosion",ParticleTexPath + L"iceExplosion.txt");
	m_pLoading->IniteForParticle("aura",		ParticleTexPath + L"aura1.txt");
	m_pLoading->IniteForParticle("gaiaCrash",	ParticleTexPath + L"gaiaCrash.txt");
	m_pLoading->IniteForParticle("PortalEffect",ParticleTexPath + L"PortalEffect.txt");
	m_pLoading->IniteForParticle("Heal",		ParticleTexPath + L"heal.txt");
	m_pLoading->IniteForParticle("Bleeding",	ParticleTexPath + L"Bleeding.txt");
	
	m_pLoading->IniteForParticle("CDT_Ice",		ParticleTexPath + L"CDT_Ice.txt");
	m_pLoading->IniteForParticle("CDT_Burn",	ParticleTexPath + L"CDT_Burn.txt");
	
	m_pLoading->IniteForParticle("HpPotion",	ParticleTexPath + L"HpHeal.txt");
	m_pLoading->IniteForParticle("MpPotion",	ParticleTexPath + L"MpHeal.txt");
	
	// x 파일
	//m_pLoading->InitForStaticMesh(MapXFilePath + L"Field/ANC_B_4858_SL.X"); //무거운 파일을 먼저 로딩한다. 늦게하면 메모리에 안올라간다.
	
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Head/Hair01.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Body/Body_00.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Hand/Hand_00.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Leg/Leg_00.X");
	//
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Body/Body_01.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Hand/Hand_01.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Leg/Leg_01.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Body/Body_02.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Hand/Hand_02.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Leg/Leg_02.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Body/Body_03.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Hand/Hand_03.X");
	//m_pLoading->InitForSkinnedMesh(CharaXFilePath + L"poporiClass03/Armor/Leg/Leg_03.X");
	
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Head", "Hair01.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_00.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_00.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_00.X");

	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_01.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_01.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_01.X");

	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_02.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_02.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_02.X");

	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Body", "Body_03.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Hand", "Hand_03.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Character/poporiClass03/Armor/Leg", "Leg_03.X");
	
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Monster", "Moster01.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Monster", "Monster02.X");
	m_pLoading->InitForSkinnedMesh("Resource/XFile/NPC", "Poalong.X");


	m_pLoading->InitForStaticMesh(WeaponXFilePath + L"TwoHandSword_00.X");
	m_pLoading->InitForStaticMesh(WeaponXFilePath + L"TwoHandSword_02.X");
	m_pLoading->InitForStaticMesh(WeaponXFilePath + L"TwoHandSword_03.X");
	m_pLoading->InitForStaticMesh(WeaponXFilePath + L"TwoHandSword_04.X");
	
	m_pLoading->InitForSound("Field", BgmSoundPath + L"Ambience_Field26.mp3", true, true); // Volume 20%
	
	m_pLoading->InitForSound("PCAtt01",					CharaSoundPath + L"PCAtt01.ogg");
	m_pLoading->InitForSound("PCAtt02",					CharaSoundPath + L"PCAtt02.ogg");
	m_pLoading->InitForSound("PCAtt03",					CharaSoundPath + L"PCAtt03.ogg");
	m_pLoading->InitForSound("PCAtt04",					CharaSoundPath + L"PCAtt04.ogg");
	m_pLoading->InitForSound("PCSkill01",				CharaSoundPath + L"PCSkill01.ogg");
	
	m_pLoading->InitForSound("EquitArmor",				CharaSoundPath + L"EquitArmor.ogg");
	m_pLoading->InitForSound("EquitWeapon",				CharaSoundPath + L"EquitWeapon.ogg");
	m_pLoading->InitForSound("PCDamaged",				CharaSoundPath + L"PCDamaged.ogg");
	m_pLoading->InitForSound("PCBigDamaged",			CharaSoundPath + L"PCBigDamaged.ogg");
	m_pLoading->InitForSound("PCDie",					CharaSoundPath + L"PCDie.ogg");
	
	// 사운드 인자값으로 브금인지 루프할것인지 물어봄
	
	//== AbandonedAutomatedGuardian
	m_pLoading->InitForSound("M1_MON_STATE_atk01",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_atk01.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_atk02",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_atk02.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_aggrochange",MonsterSoundPath + L"GateKeeper/M1_MON_STATE_aggrochange.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_Damage",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_Damage.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_Death",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_Death.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_Walk",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_Walk.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_Wait",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_Wait.mp3");
	m_pLoading->InitForSound("M1_MON_STATE_run",		MonsterSoundPath + L"GateKeeper/M1_MON_STATE_run.mp3");
	
	//== Beast
	m_pLoading->InitForSound("M2_MON_STATE_atk01",		MonsterSoundPath + L"Beast/M2_MON_STATE_atk01.mp3");
	m_pLoading->InitForSound("M2_MON_STATE_Walk",		MonsterSoundPath + L"Beast/M2_MON_STATE_Walk.mp3");
	m_pLoading->InitForSound("M2_MON_STATE_Wait",		MonsterSoundPath + L"Beast/M2_MON_STATE_Wait.mp3");
	m_pLoading->InitForSound("M2_MON_STATE_Idle",		MonsterSoundPath + L"Beast/M2_MON_STATE_Idle.mp3");
	m_pLoading->InitForSound("M2_MON_STATE_Death",		MonsterSoundPath + L"Beast/M2_MON_STATE_Death.mp3");
	m_pLoading->InitForSound("M2_MON_STATE_Damage",		MonsterSoundPath + L"Beast/M2_MON_STATE_Damage.mp3");
	
	//== npc 상인 poarong  
	m_pLoading->InitForSound("Poarong_DialogStart",		MainSoundPath + L"Poarong_DialogStart02.mp3");
	m_pLoading->InitForSound("Poarong_DialogEnd",		MainSoundPath + L"Poarong_DialogEnd01.mp3");
	
	//== 기타
	m_pLoading->InitForSound("WPN_Hit",					MainSoundPath + L"WPN_Hit.mp3");
	m_pLoading->InitForSound("WPN_Sword_Attack",		MainSoundPath + L"WPN_Sword_Attack.mp3");
	m_pLoading->InitForSound("WPN_Sword_Swing",			MainSoundPath + L"WPN_Sword_Swing.mp3");
	m_pLoading->InitForSound("WPN_Sword_Swing2",		MainSoundPath + L"WPN_Sword_Swing2.mp3");
	m_pLoading->InitForSound("Char_Wark_GlassLand",		MainSoundPath + L"Char_Wark_GlassLand.mp3");
	m_pLoading->InitForSound("Char_BodyFall",			MainSoundPath + L"Char_BodyFall.mp3");
	
	//== 사용/이용
	m_pLoading->InitForSound("Char_Skill_Explosion",	MainSoundPath + L"Char_Skill_Explosion.mp3");
	m_pLoading->InitForSound("Use_Drink",				MainSoundPath + L"Use_Drink.mp3");
	m_pLoading->InitForSound("Use_PotionDrink",			MainSoundPath + L"Use_PotionDrink.mp3");
	m_pLoading->InitForSound("Use_TeleportWarf",		MainSoundPath + L"Use_TeleportWarf.mp3");
	
	//== 시스템 사운드
	m_pLoading->InitForSound("Drop_Coin",				InterfaceSoundPath + L"Drop_Coin.mp3");
	m_pLoading->InitForSound("Drop_Gold",				InterfaceSoundPath + L"Drop_Gold.mp3");
	m_pLoading->InitForSound("Drop_Normal",				InterfaceSoundPath + L"Drop_Normal.mp3");
	m_pLoading->InitForSound("Drop_Metal",				InterfaceSoundPath + L"Drop_Metal.mp3");
	m_pLoading->InitForSound("Drop_Ore",				InterfaceSoundPath + L"Drop_Ore.mp3");
	m_pLoading->InitForSound("Drop_Potion",				InterfaceSoundPath + L"Drop_Potion.mp3");
	
	m_pLoading->InitForSound("MouseMove_SelectChange",	InterfaceSoundPath + L"MouseMove_SelectChange.mp3");
	m_pLoading->InitForSound("Click_buttom",			InterfaceSoundPath + L"Click_buttom.mp3");
	m_pLoading->InitForSound("Click_ItemPickUp",		InterfaceSoundPath + L"Click_ItemPickUp.mp3");
	m_pLoading->InitForSound("Click_NewClick",			InterfaceSoundPath + L"Click_NewClick.mp3");
	m_pLoading->InitForSound("Click_Menu",				InterfaceSoundPath + L"Click_Menu.mp3");
}
