#include "stdafx.h"
#include "Shader.h"


Shader::Shader()
{
}


Shader::~Shader()
{
}

LPD3DXEFFECT Shader::LoadShader(const char * szFolder, const char * szFile)
{
	string sFullPadth(szFolder);
	sFullPadth = sFullPadth + string("/") + string(szFile);
	wchar_t wtext[1024];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, strlen(sFullPadth.c_str()) + 1, sFullPadth.c_str(), strlen(sFullPadth.c_str()));

	DWORD dwShaderFlags = 0;
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER error = NULL;
#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(DEVICE,
		wtext,
		NULL,
		NULL,
		dwShaderFlags,
		NULL,
		&ret, &error
	);

	if (!ret && error)
	{
		int size = error->GetBufferSize();
		void* ack = error->GetBufferPointer();

		if (ack)
		{
			char * str = new char[size];
			strncpy_s(str, size, (const char *)ack, _TRUNCATE);
			//OutputDebugString(str);
			//delegate[] str;
		}
	}
	return ret;
}

LPD3DXEFFECT Shader::LoadShader(wstring szFullPath)
{
	DWORD dwShaderFlags = 0;
	LPD3DXEFFECT ret = NULL;
	LPD3DXBUFFER error = NULL;
#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

	D3DXCreateEffectFromFile(DEVICE,
		szFullPath.c_str(),
		NULL,
		NULL,
		dwShaderFlags,
		NULL,
		&ret, &error
	);

	if (!ret && error)
	{
		int size = error->GetBufferSize();
		void* ack = error->GetBufferPointer();

		if (ack)
		{
			char * str = new char[size];
			strncpy_s(str, size, (const char *)ack, _TRUNCATE);
			//OutputDebugString(str);
			//delegate[] str;
		}
	}
	return ret;
}

LPD3DXMESH Shader::LoadModel(const char * szFolder, const char * szFile)
{
	string sFullPadth(szFolder);
	sFullPadth = sFullPadth + string("/") + string(szFile);
	wchar_t wtext[1024];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, strlen(sFullPadth.c_str()) + 1, sFullPadth.c_str(), strlen(sFullPadth.c_str()));

	LPD3DXMESH ret;
	D3DXLoadMeshFromX(wtext,
		D3DXMESH_MANAGED,
		DEVICE,
		NULL, NULL, NULL, NULL,
		&ret);

	return ret;
}
