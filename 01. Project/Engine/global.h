#pragma once


// simd 사용 안함
#define _XM_NO_INTRINSICS_
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include "SimpleMath.h"

#pragma comment(lib, "d3d11")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

using namespace DirectX;
using namespace DirectX::PackedVector;


// Fbx Loader
#include "FBXLoader/fbxsdk.h"

#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

// Std
#include <string>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using std::vector;
using std::list;
using std::map;
using std::string;
using std::wstring;
using std::make_pair;
using std::sort;

#include "define.h" // 매크로, enum
#include "struct.h" // 구조체 설계
#include "extern.h" // extern 변수
#include "func.h"   // 전역 함수
