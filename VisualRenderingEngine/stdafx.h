// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
//#pragma comment(lib,"../Lib/DirectXTK.lib")

#if defined(_DEBUG)
	#pragma comment(lib, "../DirectXTK-master/Bin/Desktop_2015/Win32/Debug/DirectXTK.lib")
#else
	#pragma comment(lib, "../DirectXTK-master/Bin/Desktop_2015/Win32/Release/DirectXTK.lib")
#endif

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <unordered_map>
#include <functional>
#include <chrono>
#include <bitset>
#include <memory>
using namespace std;
using namespace std::chrono;

// DirectX

#include <d3d11.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>
//#include "../DirectXTK-master/Inc/DDSTextureLoader.h"

using namespace DirectX;
using namespace DirectX::PackedVector;	

// DirectXTK
#include "SpriteFont.h"
#include "SpriteBatch.h"

// ETC
#include "Utility.h"
#include "Singleton.h"
// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define KEY_MAX 256
#define MAX_LIGHT 16
