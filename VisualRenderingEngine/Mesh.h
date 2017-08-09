#pragma once
class CMesh
{
public:
	CMesh();
	virtual ~CMesh();


	void AssembleToVertexBuffer(const int nBuffers, 
		                        ID3D11Buffer **ppBuffers, 
		                        UINT * pnBufferStrides, 
		                        UINT * pnBufferOffsets);

	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);

	void CreateMesh(ID3D11Device *pDevice,
					const int nVertices, 
				    const D3D11_PRIMITIVE_TOPOLOGY primitiveTopology,  
			        const XMFLOAT3 * const pPosition);

protected :
	D3D11_PRIMITIVE_TOPOLOGY		m_primitiveTopology;
	DXGI_FORMAT						m_dxgiIndexFormat;
	UINT							m_nType;
	ID3D11Buffer*					m_pPositionBuffer;
	ID3D11Buffer*					m_pIndexBuffer;

	ID3D11Buffer**					m_ppVertexBuffers;
	UINT*							m_pnVertexStrides;
	UINT*							m_pnVertexOffsets;
	
	int								m_nVertices;
	int								m_nBuffers;
	UINT							m_nSlot;
	UINT							m_nStartVertex;
	
	UINT							m_nIndices;
	UINT							m_nStartIndex;
	int								m_nBaseVertex;
	UINT							m_nIndexOffset;

	//XMFLOAT3						*m_pPositions;
	//UINT							*m_pIndices;
public :
	void SetVertexCount(const UINT n) { m_nVertices = n; }
	void SetIndexCount(const UINT n) { m_nIndices = n; }
	void SetDXGIIndexFormat(const DXGI_FORMAT f) { m_dxgiIndexFormat = f; }
	void SetPrimitiveTopology(const D3D11_PRIMITIVE_TOPOLOGY p) { m_primitiveTopology = p; }

	UINT GetType() { return(m_nType); }

	void SetPositionBuffer(ID3D11Buffer *pBuffer) { m_pPositionBuffer = pBuffer; }
	void SetIndexBuffer(ID3D11Buffer *pBuffer) { m_pIndexBuffer = pBuffer; }
};

