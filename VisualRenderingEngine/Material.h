#pragma once

class CMaterial
{
public:
	CMaterial();
	~CMaterial();

	void UpdateConstantBuffer();

	static ID3D11Buffer *m_pcbMaterialColors;
private :
	XMFLOAT4 m_ambient;
	XMFLOAT4 m_diffuse;
	XMFLOAT4 m_specular;
	XMFLOAT4 m_reflect;

public :
	void SetAmbient(XMFLOAT4& a) { m_ambient = a; }
	void SetDiffuse(XMFLOAT4& d) { m_diffuse = d; }
	void SetSpecular(XMFLOAT4& s) { m_specular = s; }
	void SetReflect(XMFLOAT4& r) { m_reflect = r; }
};

