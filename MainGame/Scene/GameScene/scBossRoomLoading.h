#pragma once
class scBossRoomLoading : public cScene
{
private:
	Loading *				m_pLoading;
public:
	scBossRoomLoading();
	virtual~scBossRoomLoading();

	void Setup();
	void Release();
	void Update();
	void Render();

	void TotalLoading();
};

