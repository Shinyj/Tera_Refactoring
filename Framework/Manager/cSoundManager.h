#pragma once


#include "../FMOD/inc/fmod.hpp"
#pragma comment(lib,"Framework/FMOD/lib/fmodex_vc.lib")
using namespace FMOD;

#define SOUND cSoundManager::GetInstance()


// 채널 버퍼, 사운드 버퍼
#define EXTRACHANNELBUFFER 100
#define SOUNDBUFFER 100

// 총 사운드 버퍼
#define TOTALSOUNDBUFFER (SOUNDBUFFER + EXTRACHANNELBUFFER)

class cSoundManager
{
private:
	SINGLETONE(cSoundManager);

	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundIter;

	System * m_system;		// fmod의 시스템 클래스
	Sound** m_sound;		// fmod의 사운드 클래스
	Channel** m_channel;	// fmod의 채널   클래스

	arrSounds m_totalSounds;


	float bgmVolume = 0.5f;
	float effectVolume = 0.5f;

	string bgmKey;

	/*
	채널은 사운드의 묶음 상태를 관리
	*/


	// 여기부터 네크로 댄서 관련 변수!!!

	// 일단 벡터를 만들어서 비트를 넣어야해

	vector<int> m_vBeat;
	vector<int>::iterator m_viBeat;

	// 그 벡터를 맵에 보관해서 중복으로 로드되지 않게

	map<string, vector<int>>m_mBeat;
	map<string, vector<int>>::iterator m_miBeat;
	// 벡터에 넣을 비트 정보를 탐은 파일 포인터가 필요해
	FILE * fp;

public:

	void Setup();
	void Release();
	void Update();
	void Render();

	// 사운드 추가 (키값, 파일이름, BGM, LOOP)
	void AddSound(string KeyName, string fileName,
		bool bgm = FALSE, bool LOOP = false);

	void AddSound(string KeyName, wstring fileName,
		bool bgm = FALSE, bool LOOP = false);

	// 사운드 플레이 (키값, 볼륨 ) 0.0 ~ 1.0;
	void Play(string KeyName, float volume = 1.0f);
	// 사운드 정지
	void Stop(string KeyName);
	// 사운드 일시 정지
	void Pause(string KeyName);
	// 사운드 다시 재생
	void resume(string KeyName, float volume);
	// 플레이 중인지 상태 확인
	bool isPlaySound(string KeyName);
	// 일시정지
	bool IspauserSound(string KeyName);

	// 브금 재생
	void PlayBGM(string KeyName);
	// 이펙트 재생
	void PlayEffect(string KeyName);

	// 사운드 볼륨 조절
	void SetVolume(string key, float vol);

	// 존 브금 볼륨 조절

	void SetBGMVolume(string key, float vol);

	// 모든 소리 스탑
	void AllStop();

	//////////////////////////////////
	//
	// 여기 부터는 네크로댄서 관려 함수1
	//
	/////////////////////////////////

	// 네크로 댄서 스테이지 상점과 배경 브금을 같이 트를 함수
	void PlayZoneBGM(string key, float vol);

	void PauseZoneBGM(string key);

	void ResumeZoneBGM(string key, float vol);

	void SetBgm(string key) { bgmKey = key; }

	void PlayBGM() { Play(bgmKey, bgmVolume); }
	void PauseBGM() { Pause(bgmKey); }

	float GetBgmVolum() { return bgmVolume; }
	float GetEffectVolum() { return effectVolume; }

	void SetBgmVolum(float n) { bgmVolume = n; }
	void SetEffectVolum(float n) { effectVolume = n; }

	string GetBgmKey() { return bgmKey; }

};