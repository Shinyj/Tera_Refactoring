#include "stdafx.h"
#include "cBoundingBoxLoader.h"


cBoundingBoxLoader::cBoundingBoxLoader()
{
}


cBoundingBoxLoader::~cBoundingBoxLoader()
{
}

void cBoundingBoxLoader::Setup()
{
	ObjectLoad();
}

void cBoundingBoxLoader::Update()
{
	for (int b = 0; b < m_vecBObject.size(); b++)
	{
		if (KEY->IsOnceKeyDown('H'))
			m_vecBObject[b]->SetSightBox(!m_vecBObject[b]->GetSightBox());
		if (KEY->IsOnceKeyDown('G'))
			m_vecBObject[b]->SetSightSpere(!m_vecBObject[b]->GetSightSpere());
	}
}

void cBoundingBoxLoader::Render()
{
	if (!m_vecBObject.empty())
	{
		for (auto b : m_vecBObject)
		{
			b->Render();
		}
	}
}

void cBoundingBoxLoader::ObjectLoad()
{
	const string file_name = "Resource/Save_Object.txt";
	ifstream file_load;
	stringstream ss;
	string line;		//한줄씩 읽을 변수
	int tokenNum = 0;	//라인에 따라 변수에 할당한다.
	char frontChar = NULL;
	int value = 0;			//ss값을 받을 변수

	file_load.open(file_name, ifstream::in);

	while (getline(file_load, line)) //한줄씩 읽기
	{
		ss << line;

		ss >> frontChar;
		if (frontChar == '#')
		{
			ss >> tokenNum;
			ss.clear();	ss.str("");
			continue;
		}

		switch (tokenNum)
		{
		case 1: // vector<cGameObject*> 크기
		{
			ss >> value;
			m_vecBObject.resize(value);
		}
		break;
		case 2: //vector<cGameObject*> 에 딸린, vector<cBoundingBox*> 크기
		{
			for (UINT b = 0; b < m_vecBObject.size(); b++)
			{
				ss >> value;
				m_pGameObject = new cBoundingObject;
				m_pGameObject->GetvecBBoxGroup().resize(value);
				m_vecBObject[b] = m_pGameObject;
			}
		}
		break;
		case 3:
		{
			int b;
			float r;
			D3DXVECTOR3 posS;
			m_pSphere = new cSpere;

			ss >> b;
			ss >> posS.x;
			ss >> posS.y;
			ss >> posS.z;
			ss >> r;

			m_pSphere->Setup(posS, r);

			m_vecBObject[b]->SetSpere(m_pSphere);
		}
		break;
		case 4:
		{
			int b;
			int g;
			ss >> b;
			ss >> g;

			cBoundingBox* m_pBoundingBox = new cBoundingBox;

			D3DXVECTOR3 min;
			ss >> min.x;
			ss >> min.y;
			ss >> min.z;

			D3DXVECTOR3 max;
			ss >> max.x;
			ss >> max.y;
			ss >> max.z;

			D3DXVECTOR3 position;
			ss >> position.x;
			ss >> position.y;
			ss >> position.z;
			m_pBoundingBox->m_vPosition = position;

			D3DXVECTOR3 rotation;
			ss >> rotation.x;
			ss >> rotation.y;
			ss >> rotation.z;
			m_pBoundingBox->m_vRoatation = rotation;
			m_pBoundingBox->Setup(min, max);

			m_vecBObject[b]->GetvecBBoxGroup()[g] = m_pBoundingBox;
		}
		break;
		}

		ss.clear();	ss.str("");
	} //while()

	file_load.close();

	for (int b = 0; b < m_vecBObject.size(); b++)
	{
		for (int g = 0; g < m_vecBObject[b]->GetvecBBoxGroup().size(); g++)
		{
			OBJECTMANAGER->AddObject(m_vecBObject);
		}
	}

	for (auto a : m_vecBObject)
	{
		a->Update();
	}
}
