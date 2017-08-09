// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"comctl32.lib")
#pragma comment(lib,"../Lib/DirectXTK.lib")


#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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
//#include "../DirectXTK-master/Inc/DDSTextureLoader.h"

using namespace DirectX;

// ETC
#include "Utility.h"
#include "Singleton.h"
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#define KEY_MAX 256
#define MAX_LIGHT 16
