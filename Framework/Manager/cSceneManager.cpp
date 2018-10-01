#include "stdafx.h"
#include "cSceneManager.h"

cSceneManager::cSceneManager()
{
}


cSceneManager::~cSceneManager()
{
	miSceneList iter = _mSceneList.begin();
	for (iter; iter != _mSceneList.end();)
	{
		//삭제
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene)
				SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mSceneList.clear();
}

cScene* cSceneManager::_currentScene = NULL;

void cSceneManager::Setup()
{
}

//void cSCENE::Release()
//{
//	miSceneList iter = _mSceneList.begin();
//	for (iter; iter != _mSceneList.end();)
//	{
//		//삭제
//		if (iter->second != NULL)
//		{
//			if (iter->second == _currentScene)
//				delete iter->second;
//			SAFE_DELETE(iter->second);
//			iter = _mSceneList.erase(iter);
//		}
//		else
//		{
//			++iter;
//		}
//	}
//	_mSceneList.clear();
//
//	delete this;
//}

void cSceneManager::Update()
{
	if (_currentScene) _currentScene->Update();
}

void cSceneManager::Render()
{
	if (_currentScene) _currentScene->Render();
}

cSceneManager * cSceneManager::AddScene(string sceneName, cScene * scene)
{
	if (!scene) return nullptr;

	_mSceneList.insert(make_pair(sceneName, scene));

	return nullptr;
}

HRESULT cSceneManager::ChangeScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);
	if (find == _mSceneList.end()) return E_FAIL;
	if (find->second == _currentScene) return E_FAIL;

	find->second->Setup();

	// 원래 이부분은 위의 find->second->Setup(); 이 성공해야 실행되는 곳
	if (_currentScene) _currentScene->Release();
	_currentScene = find->second;
	return S_OK;

	return E_FAIL;
}

void cSceneManager::Destroy()
{
	miSceneList iter = _mSceneList.begin();
	for (iter; iter != _mSceneList.end();)
	{
		//삭제
		if (iter->second != NULL)
		{
			if (iter->second == _currentScene)
				delete iter->second;
			//SAFE_RELEASE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	_mSceneList.clear();

	//delete this;
}

void cScene::Setup()
{
}

void cScene::Release()
{
}

void cScene::Update()
{
}

void cScene::Render()
{
}
