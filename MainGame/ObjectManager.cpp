#include "stdafx.h"
#include "ObjectManager.h"
#include "Framework/Collider/BoundBox/cBoundingBox.h"
#include "MainGame\cGameObject.h"

ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
}

void ObjectManager::AddObject(cGameObject * obj)
{
	m_vecObject.push_back(obj);
}

void ObjectManager::AddCharaObject(cGameObject * obj)
{
	m_vecCharacter.push_back(obj);
}

void ObjectManager::AddObject(vector<cBoundingObject*> obj)
{
	m_vecObjectB = obj;
}

void ObjectManager::AddMonsterObject(cGameObject * obj)
{
	m_vecMonster.push_back(obj);
}

void ObjectManager::AddItemObject(cGameObject * obj)
{
	for (int i = 0; i < m_vecItemObject.size(); i++)
	{
		if (m_vecItemObject[i]->GetIsUse() == false)
		{
			m_vecItemObject[i]->SetIsUse(true);
			m_vecItemObject[i]->SetPosition(obj->GetPosition());
			m_vecItemObject[i]->SetName(obj->GetName());
			delete obj;
			return;
		}
	}
	m_vecItemObject.push_back(obj);
}

bool ObjectManager::IsCollision(cGameObject * obj1, cGameObject * obj2)
{
	// 구 충돌 여부 부터

	float lengh = D3DXVec3Length(&(obj1->GetSpere()->GetPosition() - obj2->GetSpere()->GetPosition()));
	float distance = obj1->GetSpere()->GetRadius() + obj2->GetSpere()->GetRadius();

	if (lengh >= distance) return false;

	// OBB 충돌 체크을 해야한 합니다.

	return OBBCollision(&obj1->GetBoundingBox()->GetOBB(),
		&obj2->GetBoundingBox()->GetOBB());
}

bool ObjectManager::IsCollision(cGameObject * obj1)
{
	bool result = false;

	for (int i = 0; i < m_vecObject.size(); i++)
	{
		result = IsCollision(obj1, m_vecObject[i]);

		if (result)
			return result;
	}

	for (int i = 0; i < m_vecObjectB.size(); i++)
	{
		result = IsCollision(obj1, m_vecObjectB[i]);

		if (result)
			return result;
	}

	return false;
}

bool ObjectManager::IsCollision(cGameObject * obj1, cBoundingObject* obj2)
{
	// 구 충돌 여부 부터
	float lengh = D3DXVec3Length(&(obj1->GetSpere()->GetPosition() - obj2->GetSpere()->GetPosition()));
	float distance = obj1->GetSpere()->GetRadius() + obj2->GetSpere()->GetRadius();

	if (lengh >= distance) return false;

	// OBB 충돌 체크을 해야만 합니다.

	for (UINT g = 0; g < obj2->GetvecBBoxGroup().size(); g++)
	{
		if (OBBCollision(&obj1->GetBoundingBox()->GetOBB(),
			&obj2->GetvecBBoxGroup()[g]->GetOBB()))
		{
			if (g == obj2->GetvecBBoxGroup().size() - 1) return true;
		}
	}
	return false;
}

bool ObjectManager::GiveDamagedChara(cSpere * spere, float Damage, D3DXVECTOR3 pos)
{
	for (int i = 0; i < m_vecCharacter.size(); i++)
	{

		cGameObject * obj1 = m_vecCharacter[i];

		float lengh = D3DXVec3Length(&(obj1->GetSpere()->GetPosition() - spere->GetPosition()));
		float distance = obj1->GetSpere()->GetRadius() + spere->GetRadius();

		if (lengh < distance)
		{
			obj1->Damaged(Damage, pos);
			return true;
		}
	}
	return false;
}

bool ObjectManager::GiveDamagedChara(cSpere * spere, float Damage, D3DXVECTOR3 pos, CONDITION con, float percent)
{
	for (int i = 0; i < m_vecCharacter.size(); i++)
	{

		cGameObject * obj1 = m_vecCharacter[i];
		D3DXVECTOR3 vec1, vec2;
		vec1 = obj1->GetSpere()->GetPosition();
		vec1.y = 0;
		vec2 = spere->GetPosition();
		vec2.y = 0;
		float lengh = D3DXVec3Length(&(vec1 - vec2));
		float distance = obj1->GetSpere()->GetRadius() + spere->GetRadius();

		if (lengh < distance)
		{
			obj1->Damaged(Damage, pos, con, percent);

			return true;
		}
	}
	return false;
}

bool ObjectManager::GiveDamagedChara(cSpere * spere, float Damage, D3DXVECTOR3 pos, CONDITION con, float percent, DAMAGED_TYPE type)
{
	for (int i = 0; i < m_vecCharacter.size(); i++)
	{
		cGameObject * obj1 = m_vecCharacter[i];
		D3DXVECTOR3 vec1, vec2;
		vec1 = obj1->GetSpere()->GetPosition();
		vec1.y = 0;
		vec2 = spere->GetPosition();
		vec2.y = 0;
		float lengh = D3DXVec3Length(&(vec1 - vec2));
		float distance = obj1->GetSpere()->GetRadius() + spere->GetRadius();

		if (lengh < distance)
		{
			obj1->Damaged(Damage, pos, con, percent, type);

			return true;
		}
	}
	return false;
}

bool ObjectManager::GiveDamagedMonster(cBoundingBox * box, float Damage)
{
	for (int i = 0; i < m_vecMonster.size(); i++)
	{

		cGameObject * obj1 = m_vecMonster[i];

		if (OBBCollision(&(obj1->GetBoundingBox()->GetOBB()), &(box->GetOBB())))
		{
			//몬스터에서는 넉백을 고려안했으므로 그냥 일단 벡터값 아무거나 줬엉.
			obj1->Damaged(Damage, D3DXVECTOR3(0, 0, 0));
			return true;
		}
	}
	return false;
}

bool ObjectManager::OBBCollision(ST_OBB * Box1, ST_OBB * Box2)
{
	double c[3][3];
	double absC[3][3];
	double d[3];

	double r0, r1, r;
	int i;

	const double cutoff = 0.999999;
	bool existsParallelPair = false;

	D3DXVECTOR3 diff = Box1->vCenterPos - Box2->vCenterPos;

	for (i = 0; i < 3; ++i)
	{
		c[0][i] = D3DXVec3Dot(&Box1->vAxisDir[0], &Box2->vAxisDir[i]);
		absC[0][i] = abs(c[0][i]);
		if (absC[0][i] > cutoff)
			existsParallelPair = true;
	}
	d[0] = D3DXVec3Dot(&diff, &Box1->vAxisDir[0]);
	r = abs(d[0]);
	r0 = Box1->fAxisLen[0];
	r1 = Box2->fAxisLen[0] * absC[0][0] + Box2->fAxisLen[1] * absC[0][1] + Box2->fAxisLen[2] * absC[0][2];

	if (r > r0 + r1)
		return FALSE;

	for (i = 0; i < 3; ++i)
	{
		c[1][i] = D3DXVec3Dot(&Box1->vAxisDir[1], &Box2->vAxisDir[i]);
		absC[1][i] = abs(c[1][i]);
		if (absC[1][i] > cutoff)
			existsParallelPair = true;
	}
	d[1] = D3DXVec3Dot(&diff, &Box1->vAxisDir[1]);
	r = abs(d[1]);
	r0 = Box1->fAxisLen[1];
	r1 = Box2->fAxisLen[0] * absC[1][0] + Box2->fAxisLen[1] * absC[1][1] + Box2->fAxisLen[2] * absC[1][2];

	if (r > r0 + r1)
		return FALSE;

	for (i = 0; i < 3; ++i)
	{
		c[2][i] = D3DXVec3Dot(&Box1->vAxisDir[2], &Box2->vAxisDir[i]);
		absC[2][i] = abs(c[2][i]);
		if (absC[2][i] > cutoff)
			existsParallelPair = true;
	}
	d[2] = D3DXVec3Dot(&diff, &Box1->vAxisDir[2]);
	r = abs(d[2]);
	r0 = Box1->fAxisLen[2];
	r1 = Box2->fAxisLen[0] * absC[2][0] + Box2->fAxisLen[1] * absC[2][1] + Box2->fAxisLen[2] * absC[2][2];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[0]));
	r0 = Box1->fAxisLen[0] * absC[0][0] + Box1->fAxisLen[1] * absC[1][0] + Box1->fAxisLen[2] * absC[2][0];
	r1 = Box2->fAxisLen[0];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[1]));
	r0 = Box1->fAxisLen[0] * absC[0][1] + Box1->fAxisLen[1] * absC[1][1] + Box1->fAxisLen[2] * absC[2][1];
	r1 = Box2->fAxisLen[1];

	if (r > r0 + r1)
		return FALSE;

	r = abs(D3DXVec3Dot(&diff, &Box2->vAxisDir[2]));
	r0 = Box1->fAxisLen[0] * absC[0][2] + Box1->fAxisLen[1] * absC[1][2] + Box1->fAxisLen[2] * absC[2][2];
	r1 = Box2->fAxisLen[2];

	if (r > r0 + r1)
		return FALSE;

	if (existsParallelPair == true)
		return TRUE;

	r = abs(d[2] * c[1][0] - d[1] * c[2][0]);
	r0 = Box1->fAxisLen[1] * absC[2][0] + Box1->fAxisLen[2] * absC[1][0];
	r1 = Box2->fAxisLen[1] * absC[0][2] + Box2->fAxisLen[2] * absC[0][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][1] - d[1] * c[2][1]);
	r0 = Box1->fAxisLen[1] * absC[2][1] + Box1->fAxisLen[2] * absC[1][1];
	r1 = Box2->fAxisLen[0] * absC[0][2] + Box2->fAxisLen[2] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[2] * c[1][2] - d[1] * c[2][2]);
	r0 = Box1->fAxisLen[1] * absC[2][2] + Box1->fAxisLen[2] * absC[1][2];
	r1 = Box2->fAxisLen[0] * absC[0][1] + Box2->fAxisLen[1] * absC[0][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][0] - d[2] * c[0][0]);
	r0 = Box1->fAxisLen[0] * absC[2][0] + Box1->fAxisLen[2] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[1][2] + Box2->fAxisLen[2] * absC[1][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][1] - d[2] * c[0][1]);
	r0 = Box1->fAxisLen[0] * absC[2][1] + Box1->fAxisLen[2] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[1][2] + Box2->fAxisLen[2] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[0] * c[2][2] - d[2] * c[0][2]);
	r0 = Box1->fAxisLen[0] * absC[2][2] + Box1->fAxisLen[2] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[1][1] + Box2->fAxisLen[1] * absC[1][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][0] - d[0] * c[1][0]);
	r0 = Box1->fAxisLen[0] * absC[1][0] + Box1->fAxisLen[1] * absC[0][0];
	r1 = Box2->fAxisLen[1] * absC[2][2] + Box2->fAxisLen[2] * absC[2][1];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][1] - d[0] * c[1][1]);
	r0 = Box1->fAxisLen[0] * absC[1][1] + Box1->fAxisLen[1] * absC[0][1];
	r1 = Box2->fAxisLen[0] * absC[2][2] + Box2->fAxisLen[2] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	r = abs(d[1] * c[0][2] - d[0] * c[1][2]);
	r0 = Box1->fAxisLen[0] * absC[1][2] + Box1->fAxisLen[1] * absC[0][2];
	r1 = Box2->fAxisLen[0] * absC[2][1] + Box2->fAxisLen[1] * absC[2][0];
	if (r > r0 + r1)
		return FALSE;

	return TRUE;
}

void ObjectManager::VecClear()
{
	m_vecObject.clear();
	m_vecCharacter.clear();
	m_vecMonster.clear();
}

void ObjectManager::PickObject()
{
	// 오브 젝트들의 벡터를 순회하면서 피킹할 것임
	// 왜냐! 피킹되면 타겟된걸로 간주! 림라이트를 씌울 것이다
	// 중복 처리까지 해야함

	if (!isOptionMode)
	{
		cRay * temp = new cRay;
		cRay r = temp->RayAtWorldSpace(WINSIZEX / 2, WINSIZEY / 2);
		bool noOne = false;
		int min = 1000000;
		int index = -1;
		int distance;
		for (int i = 0; i < m_vecMonster.size();i++)
		{
			m_vecMonster[i]->SetIsPicked(false);
			if (distance = r.IsPicked(m_vecMonster[i]->GetSpere()) > 0)
			{
				if (min > distance)
				{
					index = i;
					min = distance;
				}
				noOne = true;
			}
		}
		if (index >= 0)
			m_vecMonster[index]->SetIsPicked(true);
	}

}

void ObjectManager::Update()
{
	for (int i = 0; i < m_vecItemObject.size(); i++)
	{
		if (m_vecItemObject[i])
			m_vecItemObject[i]->Update();
	}

	for (int i = 0; i < m_vecMonster.size(); i++)
			m_vecMonster[i]->Update();
}

void ObjectManager::Render()
{
	for (int i = 0; i < m_vecItemObject.size(); i++)
		m_vecItemObject[i]->Render();

	for (int i = 0; i < m_vecMonster.size(); i++)
			m_vecMonster[i]->Render();
}
