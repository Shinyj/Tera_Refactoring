#include "stdafx.h"
#include "scBossRoomLoading.h"


scBossRoomLoading::scBossRoomLoading()
	: m_pLoading(NULL)
{
}


scBossRoomLoading::~scBossRoomLoading()
{
	SAFE_DELETE(m_pLoading);
}

void scBossRoomLoading::Setup()
{
	m_pLoading = new Loading;

	// ���⼭ ���۷��� ī��Ʈ�� ����
	m_pLoading->Setup("Resource/Texture/SceneTexture/LoadingTexture/LoadingImage_Awaken_02.png");
	this->TotalLoading();

	isOptionMode = false;

	SOUND->AddSound("Loading", "Resource/Sound/Loading/LoadingSound.mp3", true, true);
	//SOUNDMANAGER->Play("Loading");
}

void scBossRoomLoading::Release()
{
	SAFE_DELETE(m_pLoading);
}

void scBossRoomLoading::Update()
{
	m_pLoading->Update();

	if (m_pLoading->LoadingDone())
	{
		CAMERA->SetType(CAMERA_FREE);
		SCENE->ChangeScene("Boss");
	}
}

void scBossRoomLoading::Render()
{
	m_pLoading->Render();
}

void scBossRoomLoading::TotalLoading()
{// �����뿡 �ʿ��� ���ҽ��� ��������

	m_pLoading->InitForSound("BossBGM", BgmSoundPath + L"Boss_Battle.ogg",true,true);

	// �ʿ� ���̴� ����ƽ �޽ø� �ҷ��ɽô�
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"Bottom_ray.X");

	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_Enter_SM_P1.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_Enter_SM_P2.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_Enter_SM_P3.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_Enter_SM_P4.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"GilMark.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"GateLock.X");  // ��

	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_SM_P4.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_SM_P1.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_SM_P2.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_SM_P5.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Room_A_SM_P3.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Cor02_A_02_Floor_SM.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_Cor02_A_02_SM.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_TES_RCELV02_SM_1.X");
	m_pLoading->InitForStaticMesh(BossRoomXFilePath + L"ARG_TES_RCELV02_SM_2.X");
	

	// ���� �޽�
	m_pLoading->InitForSkinnedMesh("Resource/XFile/Monster", "kelsaik3.X");


	// ������ ����ϴ� ��ƼŬ
	m_pLoading->IniteForParticle("IceHand2", ParticleTexPath + L"IceHand2.txt");
	m_pLoading->IniteForParticle("FireHand",	ParticleTexPath + L"FireHand.txt");
	m_pLoading->IniteForParticle("FireEffect",	ParticleTexPath + L"Boss_FireEffect.txt");
	m_pLoading->IniteForParticle("IceEffect",	ParticleTexPath + L"Boss_IceEffect.txt");


	//== Giant Griffin
	m_pLoading->InitForSound("MON_Anim_atk01",		MonsterSoundPath + L"Kelsaik/MON_Anim_atk01(Ori).mp3");
	m_pLoading->InitForSound("MON_Anim_atk02",		MonsterSoundPath + L"Kelsaik/MON_Anim_atk02(Ori).mp3");
	m_pLoading->InitForSound("MON_Anim_heavyatk01", MonsterSoundPath + L"Kelsaik/MON_Anim_heavyatk01.mp3");
	m_pLoading->InitForSound("MON_Anim_modeAlarm",	MonsterSoundPath + L"Kelsaik/MON_Anim_modeAlarm.mp3");
	m_pLoading->InitForSound("MON_Anim_roundAtk02", MonsterSoundPath + L"Kelsaik/MON_Anim_roundAtk02.mp3");
	m_pLoading->InitForSound("MON_Anim_roundAtk01", MonsterSoundPath + L"Kelsaik/MON_Anim_roundAtk01(Ori).mp3");
	m_pLoading->InitForSound("MON_Anim_Death",		MonsterSoundPath + L"Kelsaik/MON_Anim_Death.mp3");

	m_pLoading->InitForSound("MON_Anim_groggy",		MonsterSoundPath + L"Kelsaik/MON_Anim_groggy.mp3");
	m_pLoading->InitForSound("MON_Anim_flinch",		MonsterSoundPath + L"Kelsaik/MON_Anim_flinch.mp3");
	m_pLoading->InitForSound("MON_Anim_Walk",		MonsterSoundPath + L"Kelsaik/MON_Anim_Walk.mp3");
	m_pLoading->InitForSound("GiantGriffin_Groggy", MonsterSoundPath + L"Kelsaik/GiantGriffin_Groggy.mp3");

}
