#include "stdafx.h"
#include "cSoundManager.h"


cSoundManager::cSoundManager()
{
	Setup();
}


cSoundManager::~cSoundManager()
{
}

void cSoundManager::Setup()
{
	// 사운드를 추가하는 것이 아닌 사운드가 출력이 될 환경을 조성해야함
	System_Create(&m_system);

	// 사운드 채널 수 생성
	m_system->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	// 사운드 채널 동적할당
	m_sound = new Sound*[TOTALSOUNDBUFFER];
	m_channel = new Channel*[TOTALSOUNDBUFFER];

	memset(m_sound, 0, sizeof(Sound*)*(TOTALSOUNDBUFFER));
	memset(m_channel, 0, sizeof(Channel*)*(TOTALSOUNDBUFFER));



}

void cSoundManager::Release()
{
	if (m_channel != nullptr || m_sound != nullptr)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_channel != nullptr)
			{
				if (m_channel[i]) m_channel[i]->stop();
			}
			if (m_sound != nullptr)
			{
				if (m_sound[i]) m_sound[i]->release();
			}
		}
	}
	SAFE_DELETE(m_channel);
	SAFE_DELETE(m_sound);

	if (m_system != nullptr)
	{
		m_system->release();
		m_system->close();
	}
}

void cSoundManager::Update()
{
	m_system->update();

}

void cSoundManager::Render()
{

}

void cSoundManager::AddSound(string KeyName, string soundName, bool bgm, bool loop)
{
	if (loop)
	{
		//브금
		if (bgm)
		{
			m_system->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}
		// 이펙트
		else
		{
			m_system->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}
	}
	else
	{
		m_system->createSound(soundName.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSounds.size()]);
	}
	m_totalSounds.insert(make_pair(KeyName, &m_sound[m_totalSounds.size()]));
}

void cSoundManager::AddSound(string KeyName, wstring fileName, bool bgm, bool loop)
{
	string path;
	path.assign(fileName.begin(), fileName.end());

	if (loop)
	{
		//브금
		if (bgm)
		{
			m_system->createStream(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}
		// 이펙트
		else
		{
			m_system->createSound(path.c_str(), FMOD_LOOP_NORMAL, nullptr, &m_sound[m_totalSounds.size()]);
		}
	}
	else
	{
		m_system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &m_sound[m_totalSounds.size()]);
	}
	m_totalSounds.insert(make_pair(KeyName, &m_sound[m_totalSounds.size()]));
}

void cSoundManager::Play(string KeyName, float volume)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_system->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_channel[count]);
			//m_channel[count]->setVolume(volume);
			m_channel[count]->setVolume(volume);
		}
	}

	//SetBit(KeyName);
}

void cSoundManager::PlayZoneBGM(string key, float vol)
{
	int count = 0;
	int c1, c2;
	string key2 = key + "_shopkeeper";
	arrSoundIter iter1 = m_totalSounds.begin();
	arrSoundIter iter2 = m_totalSounds.begin();
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (key == iter->first)
		{
			iter1 = iter;
			c1 = count;
		}
		else if (key2 == iter->first)
		{
			iter2 = iter;
			c2 = count;
		}
	}
	m_system->playSound(FMOD_CHANNEL_FREE, *iter1->second, false, &m_channel[c1]);
	//m_channel[count]->setVolume(volume);
	m_channel[c1]->setVolume(vol);

	m_system->playSound(FMOD_CHANNEL_FREE, *iter2->second, false, &m_channel[c2]);
	//m_channel[count]->setVolume(volume);
	m_channel[c2]->setVolume(vol);

}

void cSoundManager::PauseZoneBGM(string key)
{
	int count = 0;
	int c1, c2;
	string key2 = key + "_shopkeeper";
	arrSoundIter iter1 = m_totalSounds.begin();
	arrSoundIter iter2 = m_totalSounds.begin();
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (key == iter->first)
		{
			iter1 = iter;
			c1 = count;
		}
		else if (key2 == iter->first)
		{
			iter2 = iter;
			c2 = count;
		}
	}
	m_channel[c1]->setPaused(true);
	m_channel[c2]->setPaused(true);

}

void cSoundManager::ResumeZoneBGM(string key, float vol)
{
	int count = 0;
	int c1, c2;
	string key2 = key + "_shopkeeper";
	arrSoundIter iter1 = m_totalSounds.begin();
	arrSoundIter iter2 = m_totalSounds.begin();
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (key == iter->first)
		{
			iter1 = iter;
			c1 = count;
		}
		else if (key2 == iter->first)
		{
			iter2 = iter;
			c2 = count;
		}
	}

	m_channel[c1]->setVolume(vol);
	m_channel[c2]->setVolume(vol);

	m_channel[c1]->setPaused(false);
	m_channel[c2]->setPaused(false);
}

void cSoundManager::Stop(string KeyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_channel[count]->stop();
			break;
		}
	}
}

void cSoundManager::Pause(string KeyName)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_channel[count]->setPaused(true);
			break;
		}
	}
}

void cSoundManager::resume(string KeyName, float volume)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_channel[count]->setVolume(volume);
			m_channel[count]->setPaused(false);
			break;
		}
	}
}

bool cSoundManager::isPlaySound(string KeyName)
{
	int count = 0;
	bool isPlay;

	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_channel[count]->isPlaying(&isPlay);
			break;
		}
	}

	return isPlay;
}

bool cSoundManager::IspauserSound(string KeyName)
{
	int count = 0;
	bool isPause;

	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (KeyName == iter->first)
		{
			m_channel[count]->getPaused(&isPause);
			break;
		}
	}
	return isPause;
}

void cSoundManager::PlayBGM(string KeyName)
{
}

void cSoundManager::PlayEffect(string KeyName)
{
}

void cSoundManager::SetVolume(string key, float vol)
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (key == iter->first)
		{
			m_channel[count]->setPaused(true);
			break;
		}
	}

	m_channel[count]->setVolume(vol);
	m_channel[count]->setPaused(false);
}

void cSoundManager::SetBGMVolume(string key, float vol)
{
	int count = 0;
	int c1, c2 = -1;
	string key2 = key + "_shopkeeper";
	arrSoundIter iter1 = m_totalSounds.begin();
	arrSoundIter iter2 = m_totalSounds.begin();
	arrSoundIter iter = m_totalSounds.begin();

	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		if (key == iter->first)
		{
			iter1 = iter;
			c1 = count;
		}
		else if (key2 == iter->first)
		{
			iter2 = iter;
			c2 = count;
		}
	}

	m_channel[c1]->setPaused(true);

	m_channel[c1]->setVolume(vol);

	m_channel[c1]->setPaused(false);

	if (c2 != -1)
	{
		m_channel[c2]->setPaused(true);
		m_channel[c2]->setVolume(vol);
		m_channel[c2]->setPaused(false);

	}


}

void cSoundManager::AllStop()
{
	int count = 0;
	arrSoundIter iter = m_totalSounds.begin();
	for (iter; iter != m_totalSounds.end(); ++iter, count++)
	{
		m_channel[count]->stop();
	}
}

