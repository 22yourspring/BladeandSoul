#include "stdafx.h"
#include "ResMgr.h"

#include "RenderMgr.h"


void CResMgr::init()
{
	CreateDefaultMesh();

	CreateDefaultShader();

	CreateDefaultMaterial();



	// FMOD 초기화
	InitSound();
}


void CResMgr::CreateDefaultMesh()
{
	vector<VTX> vecVTX;
	vector<UINT> vecIdx;

	Ptr<CMesh> pMesh = nullptr;
	VTX v;

	// ==========
	// Point Mesh
	// ==========
	pMesh = new CMesh;

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);

	UINT iIdx = 0;

	pMesh->Create(&v, sizeof(VTX), 1, D3D11_USAGE_DEFAULT
		, &iIdx, DXGI_FORMAT_R32_UINT, 1, D3D11_USAGE_DEFAULT);

	AddRes(L"PointMesh", pMesh);

	// =============	// 0 --- 1
	// Rect Mesh		// |  \  |
	//					// 3 --- 2
	// =============	
	pMesh = new CMesh;

	// 1. 입력 조립기 단계에 전달할, 정점 3개로 구성된 삼각형 1개
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1); vecIdx.push_back(2);
	vecIdx.push_back(0); vecIdx.push_back(2); vecIdx.push_back(3);

	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"RectMesh", pMesh);

	// =============		// 0 --- 1
	// Collider Rect Mesh	// |     |
	//						// 3 --- 2
	// =============	
	pMesh = new CMesh;

	vecIdx.clear();
	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3); vecIdx.push_back(0);

	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"ColRectMesh", pMesh);

	//				       3
	// ===========	//    /
	// Dir Mesh		//0,2,4 ㅡ ㅡ 5
	//				//    \
	// ===========	//     1
	pMesh = new CMesh;

	vecVTX.clear();
	vecIdx.clear();

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(1.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 1.f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	v.vPos = Vec3(0.f, 0.f, 1.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVTX.push_back(v);

	vecIdx.push_back(0); vecIdx.push_back(1);
	vecIdx.push_back(2); vecIdx.push_back(3);
	vecIdx.push_back(4); vecIdx.push_back(5);

	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"DirMesh", pMesh);

	// ============
	// Circle Mesh
	// ============
	vecVTX.clear();
	vecIdx.clear();

	pMesh = new CMesh;

	// 원의 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = Vec3(0.f, 0.f, -1.f);
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 1.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);

	vecVTX.push_back(v);

	UINT iSliceCount = 32;
	float fRadius = 0.5f;
	float fSliceTheta = XM_2PI / iSliceCount;

	float fCurTheta = 0.f;
	for (UINT i = 0; i < iSliceCount + 1; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fCurTheta), fRadius * sinf(fCurTheta), 0.f);
		v.vColor = Vec4(1.f, 0.2f, 0.2f, 1.f);
		v.vUV = Vec2(0.5f * cosf(fCurTheta), 0.5f * sinf(fCurTheta));

		v.vUV.x += 0.5f;
		v.vUV.y = (0.5f - v.vUV.y);

		fCurTheta += fSliceTheta;

		vecVTX.push_back(v);
	}

	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"CircleMesh", pMesh);

	// ============
	// ColCircle Mesh
	// ============	
	vecIdx.clear();

	pMesh = new CMesh;

	for (UINT i = 1; i < iSliceCount + 2; ++i)
	{
		vecIdx.push_back(i);
	}

	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	AddRes(L"ColCircleMesh", pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// =========
	// Cube Mesh
	// =========
	VTX arrCube[24] = {};

	// 윗면
	arrCube[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[0].vUV = Vec2(0.f, 0.f);
	arrCube[0].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[1].vUV = Vec2(0.f, 0.f);
	arrCube[1].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[2].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[2].vUV = Vec2(0.f, 0.f);
	arrCube[2].vNormal = Vec3(0.f, 1.f, 0.f);

	arrCube[3].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[3].vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	arrCube[3].vUV = Vec2(0.f, 0.f);
	arrCube[3].vNormal = Vec3(0.f, 1.f, 0.f);


	// 아랫 면	
	arrCube[4].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[4].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[4].vUV = Vec2(0.f, 0.f);
	arrCube[4].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[5].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[5].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[5].vUV = Vec2(0.f, 0.f);
	arrCube[5].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[6].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[6].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[6].vUV = Vec2(0.f, 0.f);
	arrCube[6].vNormal = Vec3(0.f, -1.f, 0.f);

	arrCube[7].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[7].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrCube[7].vUV = Vec2(0.f, 0.f);
	arrCube[7].vNormal = Vec3(0.f, -1.f, 0.f);

	// 왼쪽 면
	arrCube[8].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[8].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[8].vUV = Vec2(0.f, 0.f);
	arrCube[8].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[9].vPos = Vec3(-0.5f, 0.5f, -0.5f);
	arrCube[9].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[9].vUV = Vec2(0.f, 0.f);
	arrCube[9].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[10].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[10].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[10].vUV = Vec2(0.f, 0.f);
	arrCube[10].vNormal = Vec3(-1.f, 0.f, 0.f);

	arrCube[11].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[11].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrCube[11].vUV = Vec2(0.f, 0.f);
	arrCube[11].vNormal = Vec3(-1.f, 0.f, 0.f);

	// 오른쪽 면
	arrCube[12].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[12].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[12].vUV = Vec2(0.f, 0.f);
	arrCube[12].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[13].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[13].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[13].vUV = Vec2(0.f, 0.f);
	arrCube[13].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[14].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[14].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[14].vUV = Vec2(0.f, 0.f);
	arrCube[14].vNormal = Vec3(1.f, 0.f, 0.f);

	arrCube[15].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[15].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrCube[15].vUV = Vec2(0.f, 0.f);
	arrCube[15].vNormal = Vec3(1.f, 0.f, 0.f);

	// 뒷 면
	arrCube[16].vPos = Vec3(0.5f, 0.5f, 0.5f);
	arrCube[16].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[16].vUV = Vec2(0.f, 0.f);
	arrCube[16].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[17].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	arrCube[17].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[17].vUV = Vec2(0.f, 0.f);
	arrCube[17].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[18].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	arrCube[18].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[18].vUV = Vec2(0.f, 0.f);
	arrCube[18].vNormal = Vec3(0.f, 0.f, 1.f);

	arrCube[19].vPos = Vec3(0.5f, -0.5f, 0.5f);
	arrCube[19].vColor = Vec4(1.f, 1.f, 0.f, 1.f);
	arrCube[19].vUV = Vec2(0.f, 0.f);
	arrCube[19].vNormal = Vec3(0.f, 0.f, 1.f);

	// 앞 면
	arrCube[20].vPos = Vec3(-0.5f, 0.5f, -0.5f);;
	arrCube[20].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[20].vUV = Vec2(0.f, 0.f);
	arrCube[20].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[21].vPos = Vec3(0.5f, 0.5f, -0.5f);
	arrCube[21].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[21].vUV = Vec2(0.f, 0.f);
	arrCube[21].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[22].vPos = Vec3(0.5f, -0.5f, -0.5f);
	arrCube[22].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[22].vUV = Vec2(0.f, 0.f);
	arrCube[22].vNormal = Vec3(0.f, 0.f, -1.f);

	arrCube[23].vPos = Vec3(-0.5f, -0.5f, -0.5f);
	arrCube[23].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
	arrCube[23].vUV = Vec2(0.f, 0.f);
	arrCube[23].vNormal = Vec3(0.f, 0.f, -1.f);

	// 인덱스
	for (int i = 0; i < 12; i += 2)
	{
		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 1);
		vecIdx.push_back(i * 2 + 2);

		vecIdx.push_back(i * 2);
		vecIdx.push_back(i * 2 + 2);
		vecIdx.push_back(i * 2 + 3);
	}

	pMesh = new CMesh;
	pMesh->Create(arrCube, sizeof(VTX), 24, D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	pMesh->SetName(L"CubeMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();

	// ============
	// Sphere Mesh
	// ============
	fRadius = 1.f;

	// Top
	v.vPos = Vec3(0.f, fRadius, 0.f);
	v.vUV = Vec2(0.5f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();
	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, 1.f);
	vecVTX.push_back(v);

	// Body
	UINT iStackCount = 40; // 가로 분할 개수
	iSliceCount = 40; // 세로 분할 개수

	float fStackAngle = XM_PI / iStackCount;
	float fSliceAngle = XM_2PI / iSliceCount;

	float fUVXStep = 1.f / (float)iSliceCount;
	float fUVYStep = 1.f / (float)iStackCount;

	for (UINT i = 1; i < iStackCount; ++i)
	{
		float phi = i * fStackAngle;

		for (UINT j = 0; j <= iSliceCount; ++j)
		{
			float theta = j * fSliceAngle;

			v.vPos = Vec3(fRadius * sinf(i * fStackAngle) * cosf(j * fSliceAngle)
				, fRadius * cosf(i * fStackAngle)
				, fRadius * sinf(i * fStackAngle) * sinf(j * fSliceAngle));
			v.vUV = Vec2(fUVXStep * j, fUVYStep * i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vNormal = v.vPos;
			v.vNormal.Normalize();

			v.vTangent.x = -fRadius * sinf(phi) * sinf(theta);
			v.vTangent.y = 0.f;
			v.vTangent.z = fRadius * sinf(phi) * cosf(theta);
			v.vTangent.Normalize();

			v.vTangent.Cross(v.vNormal, v.vBinormal);
			v.vBinormal.Normalize();

			vecVTX.push_back(v);
		}
	}

	// Bottom
	v.vPos = Vec3(0.f, -fRadius, 0.f);
	v.vUV = Vec2(0.5f, 1.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vNormal = v.vPos;
	v.vNormal.Normalize();

	v.vTangent = Vec3(1.f, 0.f, 0.f);
	v.vBinormal = Vec3(0.f, 0.f, -1.f);
	vecVTX.push_back(v);

	// 인덱스
	// 북극점
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 몸통
	for (UINT i = 0; i < iStackCount - 2; ++i)
	{
		for (UINT j = 0; j < iSliceCount; ++j)
		{
			// + 
			// | \
			// +--+
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j)+1);

			// +--+
			//  \ |
			//    +
			vecIdx.push_back((iSliceCount + 1) * (i)+(j)+1);
			vecIdx.push_back((iSliceCount + 1) * (i)+(j + 1) + 1);
			vecIdx.push_back((iSliceCount + 1) * (i + 1) + (j + 1) + 1);
		}
	}

	// 남극점
	UINT iBottomIdx = (UINT)vecVTX.size() - 1;
	for (UINT i = 0; i < iSliceCount; ++i)
	{
		vecIdx.push_back(iBottomIdx);
		vecIdx.push_back(iBottomIdx - (i + 2));
		vecIdx.push_back(iBottomIdx - (i + 1));
	}

	pMesh = new CMesh;
	pMesh->Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIdx[0], DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	pMesh->SetName(L"SphereMesh");
	AddRes<CMesh>(pMesh->GetName(), pMesh);

	vecVTX.clear();
	vecIdx.clear();
}


void CResMgr::CreateDefaultShader()
{
	CShader::CreateLayoutDesc();

	//=============
	// TestShader
	// int 1 개 필요
	//=============
	Ptr<CShader> pShader = nullptr;

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Color", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Color", "ps_5_0");
	pShader->AddShaderParam(tShaderParam{ L"Test Value", SHADER_PARAM::INT_0 });
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	AddRes(L"ColorShader", pShader);

	// ==============
	// Texture Shader
	// ==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Tex", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Tex", "ps_5_0");

	// BlendState 설정
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });

	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	AddRes(L"TexShader", pShader);

	// ==============
	// 2D UI Shader
	// ==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_2DUI", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_2DUI", "ps_5_0");

	// DepthStencilState 설정
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));

	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	AddRes(L"2DUIShader", pShader);

	// =================
	// Collider2D Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	AddRes(L"Collider2DShader", pShader);


	// =================
	// Collider3D Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Collider2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Collider2D", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	AddRes(L"Collider3DShader", pShader);


	// =============
	// STD2D Shader
	// =============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_Std2D", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_Std2D", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::FORWARD);

	// BlendState 설정
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	// Parameter 설정
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Anim Tex", SHADER_PARAM::TEX_3 });

	AddRes(L"Std2DShader", pShader);

	// =================
	// 2DShadow Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std.fx", "VS_2DShadow", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std.fx", "PS_2DShadow", "ps_5_0");

	// BlendState 설정
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	// Parameter 설정
	pShader->AddShaderParam(tShaderParam{ L"Output Texture", SHADER_PARAM::TEX_0 });
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	AddRes(L"2DShadowShader", pShader);

	// ==========
	// Dir Shader
	// ==========
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\tool.fx", "VS_Dir", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\tool.fx", "PS_Dir", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	AddRes(L"DirShader", pShader);


	// ============
	// Std3D Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3D", "vs_5_0");
	pShader->CreateVertexInstShader(L"Shader\\std3d.fx", "VS_Std3D_Inst", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3D", "ps_5_0");
//	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"Std3DShader", pShader);

	// ============
	// Std3D Alpha_Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3D", "vs_5_0");
	pShader->CreateVertexInstShader(L"Shader\\std3d.fx", "VS_Std3D_Inst", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3D", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::DEFERRED);

	// Blend State Setting
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	AddRes(L"Std3D_AlphaBlendShader", pShader);

	// ============
	// 3D WireFrame Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3D", "vs_5_0");
	pShader->CreateVertexInstShader(L"Shader\\std3d.fx", "VS_Std3D_Inst", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3D", "ps_5_0");
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"WireFrameShader", pShader);


	// ============
	// Moon Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Std3D", "vs_5_0");
	pShader->CreateVertexInstShader(L"Shader\\std3d.fx", "VS_Std3D_Inst", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Std3D", "ps_5_0");
//	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetRSType(RS_TYPE::CULL_NONE);

	// BlendState 설정
//	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"MoonShader", pShader);


	// ==============
	// Terrain Shader
	// ==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\terrain.fx", "VS_Terrain", "vs_5_0");
	pShader->CreateHullShader(L"Shader\\terrain.fx", "HS_Terrain", "hs_5_0");		//Tess를 위해서 쓰임 이 전
	pShader->CreateDomainShader(L"Shader\\terrain.fx", "DS_Terrain", "ds_5_0");		//Tess를 위해서 쓰임 이 후
	pShader->CreatePixelShader(L"Shader\\terrain.fx", "PS_Terrain", "ps_5_0");
	//	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"TerrainShader", pShader);

	// ==============
	// Terrain WIRE_FRAME Shader
	// ==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\terrain.fx", "VS_Terrain", "vs_5_0");
	pShader->CreateHullShader(L"Shader\\terrain.fx", "HS_Terrain", "hs_5_0");
	pShader->CreateDomainShader(L"Shader\\terrain.fx", "DS_Terrain", "ds_5_0");
	pShader->CreatePixelShader(L"Shader\\terrain.fx", "PS_Terrain", "ps_5_0");
	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	pShader->SetTopology(D3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"TerrainWireFrameShader", pShader);



	// ===============
	// DirLight Shader
	// ===============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_DirLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_DirLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ONEBLEND));

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });


	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	AddRes(L"DirLightShader", pShader);

	// ==================
	// PointLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_PointLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_PointLight", "ps_5_0");

	// One-One Blend
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ONEBLEND));

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));

	// Cull_None
	pShader->SetRSType(RS_TYPE::CULL_FRONT);

	pShader->AddShaderParam(tShaderParam{ L"Light Index", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Normal Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Render Target Resolution", SHADER_PARAM::VEC2_0 });

	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	AddRes(L"PointLightShader", pShader);

	// ==================
	// MergeLight Shader
	// ==================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_MergeLight", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_MergeLight", "ps_5_0");

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));

	pShader->AddShaderParam(tShaderParam{ L"Diffuse Target Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Light Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Specular Target Texture", SHADER_PARAM::TEX_2 });

	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	AddRes(L"MergeLightShader", pShader);

	// =================
	// RayTrace Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\SSLR.fx", "RayTraceVS", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\SSLR.fx", "RayTracePS", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"RayTrace", pShader);

	// =================
	// CombinePS Shader
	// =================
	pShader = new CShader;
	pShader->CreatePixelShader(L"Shader\\SSLR.fx", "CombinePS", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"Combine", pShader);

	// =================
	// Occlussion Shader
	// =================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\SSLR.fx", "Occlussion", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"Occlussion", pShader);

	// =================
	// DepthDownscale Shader
	// =================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\SSAO.fx", "DepthDownscale", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"DepthDownscale", pShader);

	// =================
	// SSAOCompute Shader
	// =================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\SSAO.fx", "SSAOCompute", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::LIGHTING);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddRes(L"SSAOCompute", pShader);


	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\PostDownScaleFX.fx", "DownScaleFirstPass", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"DownScaleFirstPass", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\PostDownScaleFX.fx", "DownScaleSecondPass", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"DownScaleSecondPass", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\PostDownScaleFX.fx", "BloomReveal", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"BloomReveal", pShader);

	/*pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\Blur.fx", "VerticalFilter", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"VerticalFilter", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\Blur.fx", "HorizFilter", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"HorizFilter", pShader);*/

	/*pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\BokehCS.fx", "HighlightScan", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"HighlightScan", pShader);*/

	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\PostFX.fx", "FullScreenQuadVS", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\PostFX.fx", "FinalPassPS", "ps_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"PostFX", pShader);

	/*pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\Bokeh.fx", "BokehVS", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"Bokeh", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\Bokeh.fx", "BokehGS", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"Bokeh", pShader);

	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\Bokeh.fx", "BokehPS", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);
	AddRes(L"Bokeh", pShader);*/

	// ============
	// Skybox Shader
	// ============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\std3d.fx", "VS_Skybox", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\std3d.fx", "PS_Skybox", "ps_5_0");
	pShader->SetRSType(RS_TYPE::CULL_FRONT);
	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	AddRes(L"SkyboxShader", pShader);

	// ==========================
	// Tessellation Test Shader
	// ==========================
	//	pShader = new CShader;
	//	pShader->CreateVertexShader(L"Shader\\terrain.fx", "VS_Tess", "vs_5_0");
	//	pShader->CreateHullShader(L"Shader\\terrain.fx", "HS_Tess", "hs_5_0");
	//	pShader->CreateDomainShader(L"Shader\\terrain.fx", "DS_Tess", "ds_5_0");
	//	pShader->CreatePixelShader(L"Shader\\terrain.fx", "PS_Tess", "ps_5_0");
	//		
	//	pShader->SetRSType(RS_TYPE::WIRE_FRAME);
	//	pShader->SetShaderPOV(SHADER_POV::FORWARD);
	//	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
	//	
	//	AddRes(L"TessShader", pShader);

	// ===============
	// Particle Shader
	// ===============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\particle.fx", "VS_Particle", "vs_5_0");
	pShader->CreateGeometryShader(L"Shader\\particle.fx", "GS_Particle", "gs_5_0");
	pShader->CreatePixelShader(L"Shader\\particle.fx", "PS_Particle", "ps_5_0");

	pShader->SetShaderPOV(SHADER_POV::PARTICLE);
	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_WRITE));

	pShader->AddShaderParam(tShaderParam{ L"Particle Index", SHADER_PARAM::INT_0 });

	AddRes(L"ParticleShader", pShader);

	// ======================
	// Particle Update Shader
	// ======================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\particle.fx", "CS_ParticleUpdate", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::PARTICLE);
	AddRes(L"ParticleUpdateShader", pShader);

	// ==========================
	// Animation3D Update Shader
	// ==========================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\animation.fx", "CS_Animation3D", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::COMPUTE);

	pShader->AddShaderParam(tShaderParam{ L"Bone Count", SHADER_PARAM::INT_0 });
	pShader->AddShaderParam(tShaderParam{ L"Frame Index", SHADER_PARAM::INT_1 });

	AddRes(L"Animaion3DUpdateShader", pShader);

	// =================
	// ShadowMap Shader
	// =================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\light.fx", "VS_ShadowMap", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\light.fx", "PS_ShadowMap", "ps_5_0");

	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::DEFAULT));

	AddRes(L"ShadowMapShader", pShader);

	//===================
	// Distortion Shader
	//===================
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\posteffect.fx", "VS_Distortion", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\posteffect.fx", "PS_Distortion", "ps_5_0");

	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_WRITE));

	// Shader Param
	pShader->AddShaderParam(tShaderParam{ L"Posteffect Target Texture", SHADER_PARAM::TEX_0 });

	// Shader Event Time
	pShader->SetShaderPOV(SHADER_POV::POSTEFFECT);

	AddRes(L"DistortionShader", pShader);



	// =============
	// Decal Shader
	// =============
	pShader = new CShader;
	pShader->CreateVertexShader(L"Shader\\decal.fx", "VS_Decal", "vs_5_0");
	pShader->CreatePixelShader(L"Shader\\decal.fx", "PS_Decal", "ps_5_0");

	// No Depth Test, No Depth Write
	pShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));	// Decal끼리 겹치기 위해서

	// Cull_None
	pShader->SetRSType(RS_TYPE::CULL_FRONT);

	pShader->AddShaderParam(tShaderParam{ L"Decal Texture", SHADER_PARAM::TEX_0 });
	pShader->AddShaderParam(tShaderParam{ L"Position Target Texture", SHADER_PARAM::TEX_1 });
	pShader->AddShaderParam(tShaderParam{ L"Render Target Resolution", SHADER_PARAM::VEC2_0 });

	// Blend State Setting
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	pShader->SetShaderPOV(SHADER_POV::DEFERRED);
	AddRes(L"DecalShader", pShader);
}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ColorShader"));
	AddRes(L"ColorMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TexShader"));
	AddRes(L"TexMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"2DUIShader"));
	AddRes(L"2DUIMtrl", pMtrl);


	int a = 0;
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider3DMtrl_0", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Collider2DShader"));
	a = 1;
	pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	AddRes(L"Collider3DMtrl_1", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"DirShader"));
	AddRes(L"DirMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std3DShader"));
	AddRes(L"Std3DMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Std3D_AlphaBlendShader"));
	AddRes(L"Std3D_AlphaBlendMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"WireFrameShader"));
	AddRes(L"WireFrameMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"MoonShader"));
	AddRes(L"MoonMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TerrainShader"));
	AddRes(L"TerrainMtrl", pMtrl);

	pMtrl = new CMaterial;		//TerrainWireFrameShader
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TerrainWireFrameShader"));
	AddRes(L"TerrainWireFrameShader", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"SkyboxShader"));
	AddRes(L"SkyboxMtrl", pMtrl);

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DirLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"SpecularMtrlTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_2, &pTargetTex);

		AddRes(L"DirLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"PointLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"NormalTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"SpecularMtrlTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_2, &pTargetTex);

		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);

		AddRes(L"PointLightMtrl", pMtrl);
	}

	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"MergeLightShader"));

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"DiffuseTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"LightTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"SpecularTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_2, &pTargetTex);

		AddRes(L"MergeLightMtrl", pMtrl);
	}

	// Tess Mtrl
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"TessShader"));
	AddRes(L"TessMtrl", pMtrl);

	// Particle Mtrl
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	AddRes(L"ParticleMtrl", pMtrl);

	{
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_Bubble50pxParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_Bubble99pxParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_CartoonSmokeParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_HardCircleParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_HardRainParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_SmoothParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_SmokeParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_Snow50pxParticleMtrl", pMtrl);
	//	
	//	pMtrl = new CMaterial;
	//	pMtrl->DisableFileSave();
	//	pMtrl->SetShader(FindRes<CShader>(L"ParticleShader"));
	//	AddRes(L"_SparksParticleMtrl", pMtrl);
	}

	// Particle Update
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ParticleUpdateShader"));

	Ptr<CTexture> pNoiseTex = Load<CTexture>(L"Texture\\noise.png", L"Texture\\noise.png");
	pMtrl->SetData(SHADER_PARAM::TEX_0, &pNoiseTex);
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &Vec2(pNoiseTex->Width(), pNoiseTex->Height()));

	AddRes(L"ParticleUpdateMtrl", pMtrl);

	// Animation3D Update
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"Animaion3DUpdateShader"));
	AddRes(L"Animation3DUpdateMtrl", pMtrl);

	// ShadowMap Material
	pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(FindRes<CShader>(L"ShadowMapShader"));
	AddRes(L"ShadowMapMtrl", pMtrl);


	// Decal Material _Fist
	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));

	//	Ptr<CTexture> pTargetTex = Load<CTexture>(L"Texture\\Decal\\_Fist.tga", L"Texture\\Decal\\_Fist.tga");
	//	pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);

		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);

		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);

		AddRes(L"_FistMtrl", pMtrl);
	}

	// Decal Material _BrushMtrl_Circle
	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));
	
		Ptr<CTexture> pTargetTex = Load<CTexture>(L"Texture\\Brush\\BrushMtrl_1.png", L"Texture\\Brush\\BrushMtrl_1.png");
		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);
	
		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);
	
		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);
	
		AddRes(L"_BrushCircleMtrl", pMtrl);
	}
	
	// Decal Material _BrushMtrl_Ink
	{
		pMtrl = new CMaterial;
		pMtrl->DisableFileSave();
		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));

		Ptr<CTexture> pTargetTex = Load<CTexture>(L"Texture\\Brush\\BrushMtrl_2.png", L"Texture\\Brush\\BrushMtrl_2.png");
		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);

		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);

		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);

		AddRes(L"_BrushInkMtrl", pMtrl);
	}

//	// Decal Material _Landslide
//	{
//		pMtrl = new CMaterial;
//		pMtrl->DisableFileSave();
//		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));
//	
//		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"_Landslide");
//		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);
//	
//		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
//		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);
//	
//		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
//		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);
//	
//		AddRes(L"_LandslideMtrl", pMtrl);
//	}
//	
//	// Decal Material _Offshoot
//	{
//		pMtrl = new CMaterial;
//		pMtrl->DisableFileSave();
//		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));
//	
//		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"_Offshoot");
//		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);
//	
//		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
//		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);
//	
//		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
//		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);
//	
//		AddRes(L"_OffshootMtrl", pMtrl);
//	}
//	
//	
//	// Decal Material _LightningSpirit
//	{
//		pMtrl = new CMaterial;
//		pMtrl->DisableFileSave();
//		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));
//	
//		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"_LightningSpirit");
//		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);
//	
//		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
//		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);
//	
//		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
//		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);
//	
//		AddRes(L"_LightningSpiritMtrl", pMtrl);
//	}
//	
//	
//	// Decal Material _Machslash
//	{
//		pMtrl = new CMaterial;
//		pMtrl->DisableFileSave();
//		pMtrl->SetShader(FindRes<CShader>(L"DecalShader"));
//	
//		Ptr<CTexture> pTargetTex = FindRes<CTexture>(L"_Machslash");
//		pMtrl->SetData(SHADER_PARAM::TEX_0, &pTargetTex);
//	
//		pTargetTex = FindRes<CTexture>(L"PositionTargetTex");
//		pMtrl->SetData(SHADER_PARAM::TEX_1, &pTargetTex);
//	
//		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();
//		pMtrl->SetData(SHADER_PARAM::VEC2_0, &vResolution);
//	
//		AddRes(L"_MachslashMtrl", pMtrl);
//	}
}
