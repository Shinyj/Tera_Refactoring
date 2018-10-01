#pragma once
#define TIME cTimeManager::GetInstance()

class cTimeManager
{
private:
	SINGLETONE(cTimeManager);

	DWORD	m_dwLastUpdateTime;
	float	m_fEllapsedTime;

	int		m_nFrameCount;
	float	m_fFrameTime;
	SYNTHESIZE(int, m_nFPS, FPS);
public:
	void Update();
	float GetEllapsedTime();
	float GetLastUpdateTime();

	void Delete();

	void Render();

};

