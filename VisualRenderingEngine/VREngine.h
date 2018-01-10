#pragma once

#define VR_ENGINE CVREngine::GetInstance()

#include "GameTimer.h"

class CScene;
class CCamera;
class CHorizontalInteraceShader;
class COffScreenRenderShader;
class CVerticalInteraceShader;
class CVREngine : public Singleton<CVREngine>
{
public:
	CVREngine();
	~CVREngine();

	// Widnow
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow, const int width, const int height);
	HRESULT InitDevice();
	HRESULT CreateRenderTargetDepthStencilView();
	void    CreateOffScreen(const float nWidth, const float nHeight);
	bool    InitObjects();
	void    ChangeWindowSize(const int nWidth, const int nHeight);
	void    ChangeCameraMode(const UCHAR cameraDualMode);
		  
	void    CleanupDevice();
	void    CleanupManager();
	void    CleanupObjects();

	void ProcessInput();
	void Update(const float fTimeElapsed);
	void Render();
	void VerticalRenderDual();
	void HorizontalRenderDual();
	void MouseCursorCalculate();
	bool MouseScreenBoundaryCheck(POINT &curr_pos, POINT& old_pos, POINT& lu, POINT& rb);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
private :
	HINSTANCE m_hInstance;
	HWND      m_hWnd;
	int       m_nWindowWidth;
	int       m_nWindowHeight;
	float     m_fMouseAngleX;
	float     m_fMouseAngleY;

	D3D_DRIVER_TYPE			   m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL		   m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*			   m_pDevice = nullptr;
	ID3D11Device1*			   m_pDevice1 = nullptr;
	ID3D11DeviceContext*	   m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*	   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*			   m_pSwapChain = nullptr;
	IDXGISwapChain1*		   m_pSwapChain1 = nullptr;
	ID3D11RenderTargetView*	   m_pRenderTargetView = nullptr;
	ID3D11RenderTargetView*	   m_pOffRenderTargetTextureView = nullptr;
	ID3D11Texture2D*		   m_pDepthStencilBuffer = nullptr;
	ID3D11DepthStencilView*	   m_pDepthStencilView = nullptr;
	ID3D11ShaderResourceView*  m_pOffScreenSRV = nullptr;
	ID3D11UnorderedAccessView* m_pOffScreenUAV = nullptr;
	
	CVerticalInteraceShader*   m_pVerticalInteraceShader;
	CHorizontalInteraceShader* m_pHorizontalInteraceShader;
	COffScreenRenderShader*    m_pOffScreenRenderShader;
	
	CGameTimer* m_pGameTimer;
	CScene*     m_pScene;
	vector<CCamera *> m_vecCamera;
	shared_ptr<SpriteFont> m_spFont;	// TEMP
	XMFLOAT2 m_fontPos;
	shared_ptr<SpriteBatch> m_spSpriteBatch;

	bool  m_bGameStop;
	bool  m_bVerticalRender = true;
	UCHAR m_cameraDualMode;
	bool  m_bInterace = true;
	float m_fCamInterval;
	float m_fShift;
	bool  m_bViewfrustum;
	bool  m_bStreoscopic;
	bool  m_bRenderToTexture = false;
	bool  m_bRenderOrderFlag = false;
public :
	void SetRenderOrderFlag(const bool bFlag) { m_bRenderOrderFlag = bFlag; }
	void ClearMousePoint() { m_fMouseAngleX = m_fMouseAngleY = 0.0f; }
	void SetVerticalRenderFlag(const bool flag) { m_bVerticalRender = flag;}
	void SetInterace(const bool interace) { m_bInterace = interace; }
	void SetStreoscopic(const bool flag) { m_bStreoscopic = flag; }
	void SetGameStop(const bool flag) { m_bGameStop = flag; }
	ID3D11UnorderedAccessView** GetUAV() { return &m_pOffScreenUAV; }
	ID3D11DepthStencilView* GetDSV() { return m_pDepthStencilView; }
	ID3D11RenderTargetView** GetRTV() { return &m_pRenderTargetView; }
	ID3D11ShaderResourceView** GetOffScreenSRV() { return &m_pOffScreenSRV; }
	bool IsGameStop() const { return m_bGameStop; }
	bool GetInterace() const { return m_bInterace; }
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetDeviceContext() { return m_pImmediateContext; }
	CGameTimer* GetGameTimer() { return m_pGameTimer; }
	HWND GetHWnd() const { return m_hWnd; }
	int GetWindowWidth() const { return m_nWindowWidth; }
	int GetWindowHeight() const { return m_nWindowHeight; }
	IDXGISwapChain* GetSwapChain() { return m_pSwapChain; }
	CCamera* GetCamera(const int idx) { return m_vecCamera[idx]; }
	bool IsStereoscopic() { return m_bStreoscopic; }
	bool IsVerticalRender() const { return m_bVerticalRender; }
	float GetMouseAngleX() const { return m_fMouseAngleX; }
	float GetMouseAngleY() const { return m_fMouseAngleY; }
	bool IsRenderOrderFlag() const { return m_bRenderOrderFlag; }
};

