#pragma once
class scFieldLoading : public cScene
{
private :
	Loading *		m_pLoading;
public:
	scFieldLoading();
	~scFieldLoading();

	void Setup();
	void Release();
	void Update();
	void Render();

	void TotalLoading();
};

