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

CVREngine::CVREngine()
{
	m_bViewfrustum = true;
	m_bStreoscopic = true;
	m_fCamInterval = 0.12;
	m_fShift = 0.015;
	m_bGameStop = false;
}


CVREngine::~CVREngine()
{
	// temp
	//for (auto& entity : m_pObjectList) { Memory::Delete(entity.second); }
	//m_pObjectList.clear();
	Memory::DeleteVector(m_vecCamera);
	Memory::Delete(m_pScene);		// temp
	//Memory::Delete(m_pCamera);
}

void CVREngine::ProcessInput()
{
	m_pScene->ProcessInput();
}

void CVREngine::Update(const float fTimeElapsed)
{
	m_pScene->Update(fTimeElapsed);
}

void CVREngine::Render()
{
	for (auto& cam : m_vecCamera) cam->UpdateViewMatrix();

	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	
	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();
	//SHADER_MANAGER->Render();

	m_pSwapChain->Present(0, 0);
}

void CVREngine::RenderDual()
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, Colors::MidnightBlue);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	XMVECTOR eye1   = m_vecCamera[0]->GetPositionXM();
	XMVECTOR right1 = m_vecCamera[0]->GetRightVectorXM();
	XMVECTOR look1  = m_vecCamera[0]->GetLookVectorXM();
	m_vecCamera[0]->GetProjectionMatrix()._31 = m_fShift;
	right1 = eye1 - m_fCamInterval / 2 * right1;
	look1 = look1 - m_fCamInterval / 2 * right1;
	m_vecCamera[0]->Orthogonalize();
	m_vecCamera[0]->CreateViewMatrix();
	m_vecCamera[0]->UpdateViewMatrix();
	
	if (m_bInterace)
	{
		m_vecCamera[0]->SetInterace(true);
		m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth / 2.0f, m_nWindowHeight);
	}

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[0]);
	else
		m_pScene->Render();

	eye1 = m_vecCamera[1]->GetPositionXM();
	right1 = m_vecCamera[1]->GetRightVectorXM();
	look1 = m_vecCamera[1]->GetLookVectorXM();

	m_vecCamera[1]->GetProjectionMatrix()._31 = -m_fShift;
	right1 = eye1 + m_fCamInterval / 2 * right1;
	look1 = look1 + m_fCamInterval / 2 * right1;
	m_vecCamera[1]->Orthogonalize();
	m_vecCamera[1]->CreateViewMatrix();
	m_vecCamera[1]->UpdateViewMatrix();

	if (m_bInterace)
	{
		m_vecCamera[1]->SetInterace(true);
		m_vecCamera[1]->CreateViewport(m_nWindowWidth / 2.0f + 1, 0, m_nWindowWidth / 2.0f, m_nWindowHeight);
	}

	if (m_bViewfrustum)
		m_pScene->Render(m_vecCamera[1]);
	else
		m_pScene->Render();

	//D3D11_BOX box;
	//box.left   = 0;
	//box.right  = m_nWindowWidth / 2.0f;
	//box.top    = 0;
	//box.bottom = m_nWindowHeight / 2.0f;
	//box.front  = 0;
	//box.back   = 1;

	m_pSwapChain->Present(0, 0);
}

//void CVREngine::AddObject(CEntity * pEntity)
//{
//	//m_pObjectList.insert(make_pair(m_nObjectCount++, pEntity));
//}

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

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
}

bool CVREngine::InitObjects()
{
	SHADER_MANAGER->Initalize(m_pDevice);
	TERRAIN_MANAGER->Initalize(m_pDevice);
	TEXTURE_MANAGER->Initalize(m_pDevice);
	LIGHT_MANAGER->Initalize(m_pDevice);

	CCamera *pCamera = new CCamera();

	for (int i = 0; i < 2; ++i)
	{	
		pCamera->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight, 0.0f, 500.0f);
		pCamera->Initalize();
		m_vecCamera.push_back(pCamera);
	}

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

	m_vecCamera[0]->CreateViewport(0, 0, m_nWindowWidth, m_nWindowHeight);	 // TEMP
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
	Memory::Release(m_pRenderTargetViewFinal);
	Memory::Release(m_pFinalTexture);
	Memory::Release(m_pRenderTexture);
}

void CVREngine::CleanupManager()
{
	Memory::Delete(SHADER_MANAGER);
	Memory::Delete(TERRAIN_MANAGER);
	Memory::Delete(INPUT_MANAGER);
	Memory::Delete(TEXTURE_MANAGER);	
	Memory::Delete(LIGHT_MANAGER);
	Memory::Delete(CGeometryGenerator::GetInstance());
}

void CVREngine::CleanupObjects()
{
	Memory::Delete(m_pGameTimer);
}

LRESULT CVREngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
	case WM_SIZE:
	{
		if(VR_ENGINE->GetDevice())
			VR_ENGINE->ChangeWindowSize(LOWORD(lParam), HIWORD(lParam));
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

