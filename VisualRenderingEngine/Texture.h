#pragma once
class CTexture
{
public:
	CTexture();
	~CTexture();

	//void UpdateConstantBuffer();
	void PSSetTextureAndSampler();
	void VSSetTextureAndSampler();
	void HSSetTextureAndSampler();
	void DSSetTextureAndSampler();

	void SetSampler(ID3D11SamplerState *pSampler) { m_pSamplerState = pSampler; }
	void SetSRV(ID3D11ShaderResourceView *pSRV) { m_pSRV = pSRV; }
	void SetTextureSlot(const UINT s) { m_nTextureSlot = s; }
	void SetSamplerSlot(const UINT s) { m_nSamplerSlot = s; }
	//void UpdateConstantBuffer();
	ID3D11ShaderResourceView* GetSRV() { return m_pSRV; }
private :
	UINT m_nTextureSlot{ 0 };
	UINT m_nSamplerSlot{ 0 };
	ID3D11ShaderResourceView *m_pSRV{ nullptr };
	ID3D11SamplerState *m_pSamplerState{ nullptr };

	static ID3D11Buffer *m_pcbTextureMtxBuffer;
};

