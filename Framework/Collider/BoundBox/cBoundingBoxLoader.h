#pragma once
class cBoundingObject;
class cSpere;

class cBoundingBoxLoader
{
private:
	cBoundingObject* m_pGameObject;
	cSpere* m_pSphere;
public :
	SYNTHESIZE_REF(vector<cBoundingObject*>, m_vecBObject, BObject); //���� �ڽ����� ����ִ°��� ����ü.

public:
	cBoundingBoxLoader();
	~cBoundingBoxLoader();

	void Setup();
	void Update();
	void Render();

	void ObjectLoad();
};