#pragma once
#include "Singleton.h"

#define TEXTURE_MANAGER CTextureManager::GetInstance()

class CTexture;
class CTextureManager : public Singleton<CTextureManager>
{
public:
	CTextureManager();
	~CTextureManager();

	bool Initalize(ID3D11Device* pDevice);
	void ShutDown();

	CTexture* LoadTextureFromFile(wstring file_name, ID3D11Resource *pTex2D = nullptr);
	
	void AddTexture(wstring tex_name, CTexture *pTex);
	void RemoveTexture(wstring tex_name);
	void AddSampler(wstring sam_name, ID3D11SamplerState *pSampler);
	void RemoveSampler(wstring sam_name);

	CTexture *GetTexture(wstring tex_name);
	ID3D11SamplerState *GetSampler(wstring sam_name) { return m_mapSamplers[sam_name]; }
private :

	ID3D11Device *m_pDevice;
	unordered_map<wstring, CTexture *> m_mapTextures;
	unordered_map<wstring, ID3D11SamplerState *> m_mapSamplers;
};

