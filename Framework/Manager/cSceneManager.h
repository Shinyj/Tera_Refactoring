#pragma once


#define SCENE cSceneManager::GetInstance()

class cScene
{
public :
	cScene() {};
	virtual~cScene() {};

	virtual void Setup() = 0;   
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
};

class cSceneManager
{
private:

	SINGLETONE(cSceneManager);

	typedef map<string, cScene*> mSceneList;
	typedef map<string, cScene*>::iterator miSceneList;

	static cScene* _currentScene;  //���� ��
	mSceneList _mSceneList;		  //�����

	string m_sCurrentSceneName; //lsc

public:

	void Setup();
	void Update();
	void Render();

	string* GetCurrentSceneName() { return &m_sCurrentSceneName; }; //lsc

																	//�� �߰�
	cSceneManager* AddScene(string sceneName, cScene* scene);
	//�� ����
	HRESULT ChangeScene(string sceneName);

	void Destroy();

};
