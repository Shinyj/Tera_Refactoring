#pragma once
class Shader
{
public:
	Shader();
	~Shader();

	static LPD3DXEFFECT LoadShader(const char* szFolder, const char* szFile);
	static LPD3DXEFFECT LoadShader(wstring szFullPath);

	static LPD3DXMESH	LoadModel(const char* szFolder, const char* szFile);
};

