#include "stdafx.h"
#include "Resource.h"
#include "VREngine.h"
#include "ShaderManager.h"
#include "Tutorial03Shader.h"
#include "Entity.h"
#include "Camera.h"
#include "Scene.h"
#include "MyScene.h"
#include "ElementType.h"
#include "TerrainManager.h"
#include "InputManager.h"
#include "GeometryGenerator.h"
#include "TextureManager.h"
#include "LightManager.h"
#include "RenderState.h"
#include "OffScreenMesh.h"
#include "HorizontalInteraceShader.h"
#include "OffScreenRenderShader.h"
#include "VerticalInteraceShader.h"
#include "SlotType.h"

CVREngine::CVREngine()
{
	m_bViewfrustum = false;
	m_bStreoscopic = true;
	m_fCamInterval = 0.12f;
	m_fShift = 0.015f;
	m_bGameStop = false;
	m_bInterace = true;
}


CVREngine::~CVREngine()
{
	// temp
	//for (auto& entity : m_pObjectList) { Memory::Delete(entity.second); }
	//m_pObjectList.clear();
	Memory::DeleteVector(m_vecCamera);
	Memory::Delete(m_pVerticalInteraceShader);
	Memory::Delete(m_pHorizontalInteraceShader);
	Memory::Delete(m_pOffScreenRenderShader);
	Memory::Delete(m_pScene);		// temp
	//Memory::Delete(m_pCamera);
}

void CVREngine::ProcessInput()
{
	m_pScene->ProcessInput();
}

void CVREngine::Update(const float fTimeElapsed)
{
	MouseCursorCalculate();
	m_pScene->Update(fTimeElapsed);
}

void CVREngine::Render()
{
	if(m_bRenderToTexture)
		m_pImmediateContext->ClearRenderTargetView(m_pOffRenderTargetTextureView, Colors::MidnightBlue);
	else
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	m_pSwapChain->Present(0, 0);
}

void CVREngine::VerticalRenderDual()
{
	ID3D11RenderTargetView* pRTV[1]{ m_pOffRenderTargetTextureView };

	if (m_bRenderToTexture)
	{
		m_pImmediateContext->OMSetRenderTargets(1, pRTV, m_pDepthStencilView);
		m_pImmediateContext->ClearRenderTargetView(m_pOffRenderTargetTextureView, Colors::MidnightBlue);
	}
	else
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	XMVECTOR eye1   = m_vecCamera[0]->GetPositionXM();
	XMVECTOR right1 = m_vecCamera[0]->GetRightVectorXM();
	XMVECTOR look1  = m_vecCamera[0]->GetLookVectorXM();
	right1 = eye1 - m_fCamInterval / 2 * right1;
	look1  = look1 - m_fCamInterval / 2 * right1;
	m_vecCamera[0]->GetProjectionMatrix()._31 = m_fShift;
	m_vecCamera[0]->Orthogonalize();
	m_vecCamera[0]->CreateViewMatrix();
	m_vecCamera[0]->UpdateViewMatrix();
	m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth / 2.0f, m_nWindowHeight);

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	eye1   = m_vecCamera[0]->GetPositionXM();
	right1 = m_vecCamera[0]->GetRightVectorXM();
	look1  = m_vecCamera[0]->GetLookVectorXM();

	right1 = eye1 + m_fCamInterval / 2 * right1;
	look1  = look1 + m_fCamInterval / 2 * right1;
	m_vecCamera[0]->GetProjectionMatrix()._31 = -m_fShift;
	m_vecCamera[0]->Orthogonalize();
	m_vecCamera[0]->CreateViewMatrix();
	m_vecCamera[0]->UpdateViewMatrix();
	m_vecCamera[0]->CreateViewport(m_nWindowWidth / 2.0f + 1, 0, m_nWindowWidth / 2.0f, m_nWindowHeight);

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	if (m_bRenderToTexture)
	{
		m_pVerticalInteraceShader->Render();		// 인터레이스 쉐이더에서 UAV에 제대로 픽셀값들이 들어가지지가 않는듯..?
		// RenderToTexture Shade Render
		ID3D11ShaderResourceView* pSRVNull[1]{ nullptr };
		m_pImmediateContext->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
		m_pImmediateContext->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, &m_pVerticalInteraceShader->m_pOutSRV);
		
		pRTV[0] = m_pRenderTargetView;
		//pRTV[0] = m_pInteraceShader->m_pOutRTV;
		m_pImmediateContext->OMSetRenderTargets(1, pRTV, m_pDepthStencilView);
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
		//m_pInteraceShader->Render();
		m_pOffScreenRenderShader->Render();
	}

	HR(m_pSwapChain->Present(0, 0));
}

void CVREngine::HorizontalRenderDual()
{
	ID3D11RenderTargetView* pRTV[1]{ m_pOffRenderTargetTextureView };

	if (m_bRenderToTexture)
	{
		m_pImmediateContext->OMSetRenderTargets(1, pRTV, m_pDepthStencilView);
		m_pImmediateContext->ClearRenderTargetView(m_pOffRenderTargetTextureView, Colors::MidnightBlue);
	}
	else
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);


	// Top Screen View Setting 
	XMVECTOR eye1   = m_vecCamera[0]->GetPositionXM();
	XMVECTOR right1 = m_vecCamera[0]->GetRightVectorXM();
	XMVECTOR look1  = m_vecCamera[0]->GetLookVectorXM();
	right1 = eye1 - m_fCamInterval / 2 * right1;
	look1 = look1 - m_fCamInterval / 2 * right1;
	m_vecCamera[0]->GetProjectionMatrix()._31 = m_fShift;
	m_vecCamera[0]->Orthogonalize();
	m_vecCamera[0]->CreateViewMatrix();
	m_vecCamera[0]->UpdateViewMatrix();
	m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight / 2.0f);

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	// Bottom Screen View Setting
	eye1   = m_vecCamera[0]->GetPositionXM();
	right1 = m_vecCamera[0]->GetRightVectorXM();
	look1  = m_vecCamera[0]->GetLookVectorXM();
	right1 = eye1 + m_fCamInterval / 2 * right1;
	look1  = look1 + m_fCamInterval / 2 * right1;
	m_vecCamera[0]->GetProjectionMatrix()._31 = -m_fShift;
	m_vecCamera[0]->Orthogonalize();
	m_vecCamera[0]->CreateViewMatrix();
	m_vecCamera[0]->UpdateViewMatrix();

	m_vecCamera[0]->CreateViewport(0.0f, m_nWindowHeight / 2.0f + 1, m_nWindowWidth, m_nWindowHeight / 2.0f);

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	if (m_bRenderToTexture)
	{
		m_pHorizontalInteraceShader->Render();		// 인터레이스 쉐이더에서 UAV에 제대로 픽셀값들이 들어가지지가 않는듯..?
													// RenderToTexture Shade Render
		ID3D11ShaderResourceView* pSRVNull[1]{ nullptr };
		m_pImmediateContext->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, pSRVNull);
		m_pImmediateContext->PSSetShaderResources(TextureSlot::TEXTURE_RENDER_TEXTURE, 1, &m_pHorizontalInteraceShader->m_pOutSRV);

		pRTV[0] = m_pRenderTargetView;
		//pRTV[0] = m_pInteraceShader->m_pOutRTV;
		m_pImmediateContext->OMSetRenderTargets(1, pRTV, m_pDepthStencilView);
		m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
		m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
		m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
		m_pOffScreenRenderShader->Render();
	}

	//const wchar_t * output = L"Hello World";		// Text Bug... why..?

	//m_spSpriteBatch->Begin();

	//m_fontPos.x = m_nWindowWidth / 2.0f;
	//m_fontPos.y = m_nWindowHeight / 2.0f;

	//XMVECTOR origin = m_spFont->MeasureString(output) / 2.0f;
	//m_spFont->DrawString(m_spSpriteBatch.get(), output, m_fontPos, Colors::Red);

	//m_spSpriteBatch->End();
	HR(m_pSwapChain->Present(0, 0));
}

void CVREngine::MouseCursorCalculate()
{
	static POINT mouseOldPoint{ 0, 0 };
	RECT clientRect;
	GetWindowRect(m_hWnd, &clientRect);
	
	POINT p{ 0, 0 };
	POINT leftUpScreenPoint, rightBottomScreenPoint;
	leftUpScreenPoint.x = clientRect.left;
	leftUpScreenPoint.y = clientRect.top;
	rightBottomScreenPoint.x = clientRect.right;
	rightBottomScreenPoint.y = clientRect.bottom;

	ScreenToClient(m_hWnd, &leftUpScreenPoint);
	ScreenToClient(m_hWnd, &rightBottomScreenPoint);
	
	GetCursorPos(&p);
	ScreenToClient(m_hWnd, &p);
	
	if (MouseScreenBoundaryCheck(p, mouseOldPoint, leftUpScreenPoint, rightBottomScreenPoint))
	{
		ClientToScreen(m_hWnd, &p);
		SetCursorPos(p.x, p.y);
	}
	
	int x = p.x;
	int y = p.y;

	GetInstance()->m_fMouseAngleY = (x - mouseOldPoint.x) / 1.8f;
	GetInstance()->m_fMouseAngleX = (y - mouseOldPoint.y) / 1.8f;

	mouseOldPoint.x = x;
	mouseOldPoint.y = y;
}

bool CVREngine::MouseScreenBoundaryCheck(POINT &curr_pos, POINT& old_pos, POINT& lu, POINT& rb)
{
	bool flag{ false };
	lu.x -= 10;
	rb.x -= 10;
	cout << "curr_pos.x : " << curr_pos.x << endl;
	if (curr_pos.x < lu.x)
	{
		curr_pos.x = rb.x;
		old_pos.x = curr_pos.x;
		flag = true;
	}
	else if (curr_pos.x > rb.x)
	{
		curr_pos.x = lu.x;
		old_pos.x = curr_pos.x;
		flag = true;
	}
	//if (curr_pos.y < lu.y)
	//{
	//	curr_pos.y = rb.y;
	//	old_pos.y = curr_pos.y;
	//	flag = true;
	//}
	//else if (curr_pos.y > rb.y)
	//{
	//	curr_pos.y = lu.y;
	//	old_pos.y = curr_pos.y;
	//	flag = true;
	//}
	return flag;
}

HRESULT CVREngine::InitWindow(HINSTANCE hInstance, int nCmdShow, const int width, const int height)
{
	// Register class
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = this->WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VISUALRENDERINGENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = L"VR Engine";
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	// Create window
	m_hInstance = hInstance;
	m_nWindowHeight = height;
	m_nWindowWidth = width;
	RECT rc = { 0, 0, width, height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	m_hWnd = CreateWindow(L"VR Engine", L"VR Engine",
		WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_BORDER,
		CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, hInstance,
		nullptr);
	if (!m_hWnd)
		return E_FAIL;

	ShowWindow(m_hWnd, nCmdShow);

	return S_OK;
}

HRESULT CVREngine::InitDevice()
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		m_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pDevice, &m_featureLevel, &m_pImmediateContext);

		if (hr == E_INVALIDARG)
		{
			// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
			hr = D3D11CreateDevice(nullptr, m_driverType, nullptr, createDeviceFlags, &featureLevels[1], numFeatureLevels - 1,
				D3D11_SDK_VERSION, &m_pDevice, &m_featureLevel, &m_pImmediateContext);
		}

		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return hr;

	// Obtain DXGI factory from device (since we used nullptr for pAdapter above)
	IDXGIFactory1* dxgiFactory = nullptr;
	{
		IDXGIDevice* dxgiDevice = nullptr;
		hr = m_pDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice));
		if (SUCCEEDED(hr))
		{
			IDXGIAdapter* adapter = nullptr;
			hr = dxgiDevice->GetAdapter(&adapter);
			if (SUCCEEDED(hr))
			{
				hr = adapter->GetParent(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&dxgiFactory));
				adapter->Release();
			}
			dxgiDevice->Release();
		}
	}
	if (FAILED(hr))
		return hr;

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	hr = dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		hr = m_pDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&m_pDevice1));
		if (SUCCEEDED(hr))
		{
			(void)m_pImmediateContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&m_pImmediateContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		hr = dxgiFactory2->CreateSwapChainForHwnd(m_pDevice, m_hWnd, &sd, nullptr, nullptr, &m_pSwapChain1);
		if (SUCCEEDED(hr))
		{
			hr = m_pSwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(&m_pSwapChain));
		}

		dxgiFactory2->Release();
	}
	else
	{
		// DirectX 11.0 systems
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = m_hWnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;

		hr = dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	}

	// Note this tutorial doesn't handle full-screen swapchains so we block the ALT+ENTER shortcut
	//dxgiFactory->MakeWindowAssociation(m_hWnd, DXGI_MWA_NO_ALT_ENTER);
	
	dxgiFactory->Release();

	if (FAILED(hr))
		return hr;

	HR(CreateRenderTargetDepthStencilView());

	// Setup the viewport
	/*D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);*/

	return S_OK;
}

HRESULT CVREngine::CreateRenderTargetDepthStencilView()
{
	HRESULT hr;
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return hr;
	
	// TEMP
	////////////////////////////////////////////////////////////////////////
	//m_pRenderTexture = pBackBuffer;
	//
	//D3D11_TEXTURE2D_DESC td;
	//ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	//m_pRenderTexture->GetDesc(&td);
	//
	//HR(m_pDevice->CreateTexture2D(&td, nullptr, &m_pFinalTexture));
	//HR(m_pDevice->CreateRenderTargetView(m_pRenderTexture, nullptr, &m_pRenderTargetViewFinal));
	////////////////////////////////////////////////////////////////////////
	hr = m_pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	
	pBackBuffer->Release();
	if (FAILED(hr))
		return hr;



	//m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, nullptr);

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = m_nWindowWidth;
	descDepth.Height = m_nWindowHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = m_pDevice->CreateTexture2D(&descDepth, nullptr, &m_pDepthStencilBuffer);
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_pDevice->CreateDepthStencilView(m_pDepthStencilBuffer, &descDSV, &m_pDepthStencilView);
	if (FAILED(hr))
		return hr;

	CreateOffScreen(m_nWindowWidth, m_nWindowHeight);
	m_bRenderToTexture = true;
	if(m_bRenderToTexture)
		m_pImmediateContext->OMSetRenderTargets(1, &m_pOffRenderTargetTextureView, m_pDepthStencilView);
	else
		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	return hr;
}

void CVREngine::CreateOffScreen(const float nWidth, const float nHeight)
{
	Memory::Release(m_pOffRenderTargetTextureView);
	Memory::Release(m_pOffScreenSRV);
	Memory::Release(m_pOffScreenUAV);

	ID3D11Texture2D* pOffScreenRenderTexture;
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(D3D11_TEXTURE2D_DESC));
	td.Width = nWidth;
	td.Height = nHeight;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;

	HR(m_pDevice->CreateTexture2D(&td, nullptr, &pOffScreenRenderTexture));
	HR(m_pDevice->CreateShaderResourceView(pOffScreenRenderTexture, nullptr, &m_pOffScreenSRV));
	HR(m_pDevice->CreateRenderTargetView(pOffScreenRenderTexture, nullptr, &m_pOffRenderTargetTextureView));
	HR(m_pDevice->CreateUnorderedAccessView(pOffScreenRenderTexture, nullptr, &m_pOffScreenUAV));
	Memory::Release(pOffScreenRenderTexture);
}

bool CVREngine::InitObjects()
{
	SHADER_MANAGER->Initalize(m_pDevice);
	TEXTURE_MANAGER->Initalize(m_pDevice);
	//TERRAIN_MANAGER->Initalize(m_pDevice);
	LIGHT_MANAGER->Initalize(m_pDevice);
	RENDER_STATE->Initalize();
	
	m_spFont = make_shared<SpriteFont>(m_pDevice, L"../Assets/Fonts/myfile.spritefont");
	m_spSpriteBatch = make_shared<SpriteBatch>(m_pImmediateContext);

	m_pOffScreenRenderShader = new COffScreenRenderShader();
	m_pOffScreenRenderShader->BuildObject();
	m_pVerticalInteraceShader = new CVerticalInteraceShader();
	m_pVerticalInteraceShader->BuildObject();
	m_pHorizontalInteraceShader = new CHorizontalInteraceShader();
	m_pHorizontalInteraceShader->BuildObject();

	CCamera *pCamera = new CCamera();

	pCamera->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight, 0.0f, 5000.0f, false);
	pCamera->Initalize();
	m_vecCamera.push_back(pCamera);

	m_pGameTimer = new CGameTimer();
	m_pGameTimer->Start();

	m_pScene = new CMyScene();
	m_pScene->Initalize();
	m_pScene->SetCamera(pCamera);
	m_pScene->BuildObjects();
	
	return true;
}

void CVREngine::ChangeWindowSize(const int nWidth, const int nHeight)
{
	m_nWindowHeight = nHeight;
	m_nWindowWidth = nWidth;

	m_pImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);
	Memory::Release(m_pDepthStencilBuffer);
	Memory::Release(m_pDepthStencilView);
	Memory::Release(m_pRenderTargetView);
	HR(m_pSwapChain->ResizeBuffers(1, m_nWindowWidth, m_nWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	HR(CreateRenderTargetDepthStencilView());

	CreateOffScreen(nWidth, nHeight);
	m_pVerticalInteraceShader->BuildViews(nWidth, nHeight);		
	m_pHorizontalInteraceShader->BuildViews(nWidth, nHeight);
	//m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight);	 // TEMP
}

void CVREngine::ChangeCameraMode(const UCHAR cameraDualMode)
{
	if (m_cameraDualMode == cameraDualMode) return;

	if (cameraDualMode == CameraDualMode::INTERACE) { SetInterace(true); }
	else
	{
		m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight);
		SetInterace(false);
	}
}

void CVREngine::CleanupDevice()
{
	if (m_pImmediateContext) m_pImmediateContext->ClearState();

	Memory::Release(m_pRenderTargetView);
	Memory::Release(m_pSwapChain1);
	Memory::Release(m_pSwapChain);
	Memory::Release(m_pImmediateContext1);
	Memory::Release(m_pImmediateContext);
	Memory::Release(m_pDevice1);
	Memory::Release(m_pDevice);
	Memory::Release(m_pOffRenderTargetTextureView);
	//Memory::Release(m_pOffScreenRenderTexture);
}

void CVREngine::CleanupManager()
{
	Memory::Delete(SHADER_MANAGER);
	Memory::Delete(TERRAIN_MANAGER);
	Memory::Delete(INPUT_MANAGER);
	Memory::Delete(TEXTURE_MANAGER);	
	Memory::Delete(LIGHT_MANAGER);
	Memory::Delete(RENDER_STATE);
	Memory::Delete(CGeometryGenerator::GetInstance());
}

void CVREngine::CleanupObjects()
{
	m_spFont.reset();
	m_spSpriteBatch.reset();
	Memory::Delete(m_pGameTimer);
}

LRESULT CVREngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static POINT mouseDownPoint{ 0, 0 };
	switch (message)
	{
	//case WM_COMMAND:
	//{
	//	int wmId = LOWORD(wParam);
	//	// 메뉴 선택을 구문 분석합니다.
	//	switch (wmId)
	//	{
	//	case IDM_ABOUT:
	//		DialogBox(m_hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
	//		break;
	//	case IDM_EXIT:
	//		DestroyWindow(hWnd);
	//		break;
	//	default:
	//		return DefWindowProc(hWnd, message, wParam, lParam);
	//	}
	//}
	//break;
	case WM_CREATE:
	{
		//ShowCursor(false);
		//SetCapture(GetInstance()->m_hWnd);
//		SetCursorPos(GetInstance()->GetWindowWidth() / 2.0f, GetInstance()->GetWindowHeight() / 2.0f);
	}break;
	case WM_SIZE:
	{
		if (VR_ENGINE->GetDevice())
		{
			VR_ENGINE->ChangeWindowSize(LOWORD(lParam), HIWORD(lParam));
		}		
	}break;
	case WM_MOUSEMOVE:
	{
		//if (GetCapture() == GetInstance()->m_hWnd)
		//{
		
		//}
	}break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

