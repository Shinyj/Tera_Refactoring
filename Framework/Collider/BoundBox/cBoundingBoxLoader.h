#pragma once
class cBoundingObject;
class cSpere;

class cBoundingBoxLoader
{
private:
	cBoundingObject* m_pGameObject;
	cSpere* m_pSphere;
public :
	SYNTHESIZE_REF(vector<cBoundingObject*>, m_vecBObject, BObject); //볼과 박스들이 들어있는것의 집합체.

public:
	cBoundingBoxLoader();
	~cBoundingBoxLoader();

	void Setup();
	void Update();
	void Render();

	void ObjectLoad();
};