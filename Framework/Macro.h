#pragma once

#define WINSIZEX 1600 // 1600
#define WINSIZEY 900 // 900

#define SAFE_DELETE(p)			{if(p) {delete(p); (p) = nullptr;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = nullptr;}}
#define SAFE_RELEASE(p)	{ if (p) p->Release(); p = NULL; }


#define SINGLETONE(class_name)\
private:\
	class_name();\
	virtual~class_name();\
public:\
	static class_name * GetInstance()\
	{\
		static class_name instance;\
		return &instance;\
	}

#define SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: varType Get##funName(void) { return varName; }\
public: void Set##funName(varType var) { varName = var; }

#define SYNTHESIZE_REF(varType, varName, funName)\
protected: varType varName;\
public: varType& Get##funName(void) { return varName; }\
public: void Set##funName(varType& var) { varName = var; }


// 경로 부분 매크로

#define FONTPATH "Resource/Font/"

#define UIPATH (string)"Resource/Texture/UI/ParticleEdit/"

