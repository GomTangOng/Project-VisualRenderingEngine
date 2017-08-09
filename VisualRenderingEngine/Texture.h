#pragma once
class CTexture
{
public:
	CTexture();
	~CTexture();

	//void UpdateConstantBuffer();
	void PSSetTextureAndSampler();
	void SetSampler(ID3D11SamplerState *pSampler) { m_pSamplerState = pSampler; }
	void SetSRV(ID3D11ShaderResourceView *pSRV) { m_pSRV = pSRV; }
	//void UpdateConstantBuffer();
private :
	ID3D11ShaderResourceView *m_pSRV{ nullptr };
	ID3D11SamplerState *m_pSamplerState{ nullptr };

	static ID3D11Buffer *m_pcbTextureMtxBuffer;
};

