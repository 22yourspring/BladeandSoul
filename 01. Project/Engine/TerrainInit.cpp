#include "stdafx.h"
#include "Terrain.h"

#include "RenderMgr.h"

#include "Camera.h"
#include "Transform.h"

#include "ResMgr.h"
#include "MeshRender.h"
#include "Shader.h"

void CTerrain::init()
{
	ChangeFaceCount(m_iFaceX, m_iFaceZ);

	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::INT_0, &m_iFaceX);
	pMtrl->SetData(SHADER_PARAM::INT_1, &m_iFaceZ);

	// 높이맵 텍스쳐 Create
	m_pHeightMap = CResMgr::GetInst()->CreateTexture(L"HeightMap", m_iFaceX * pow(2.f, m_fMaxTess)
		, m_iFaceZ * pow(2.f, m_fMaxTess), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
		, DXGI_FORMAT_R32_FLOAT, D3D11_USAGE_DEFAULT);
	Vec2 vHeightMapRes = Vec2(m_pHeightMap->Width(), m_pHeightMap->Height());

	// 가중치 텍스쳐 Create
	m_pWeightTex = CResMgr::GetInst()->CreateTexture(L"WeightTex", m_iFaceX * pow(2.f, m_fMaxTess)
		, m_iFaceZ * pow(2.f, m_fMaxTess), D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_USAGE_DEFAULT);
	Vec2 vWeightRes = Vec2(m_pWeightTex->Width(), m_pWeightTex->Height());


	// 타일 텍스쳐
	Ptr<CTexture> pTile = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_01.tga");
	Ptr<CTexture> pTile_n = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_01_N.tga");

	Ptr<CTexture> pTile2 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_02.tga");
	Ptr<CTexture> pTile_n2 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_02_N.tga");

	Ptr<CTexture> pTile3 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_05.tga");
	Ptr<CTexture> pTile_n3 = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_05_N.tga");

	pMtrl->SetData(SHADER_PARAM::TEX_2, &m_pHeightMap);
	pMtrl->SetData(SHADER_PARAM::TEX_3, &m_pWeightTex);
	pMtrl->SetData(SHADER_PARAM::VEC2_0, &vHeightMapRes);
	pMtrl->SetData(SHADER_PARAM::VEC2_1, &vWeightRes);
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fMaxTess);

	pMtrl->SetData(SHADER_PARAM::TEX_0, &pTile);
	pMtrl->SetData(SHADER_PARAM::TEX_1, &pTile_n);

	pMtrl->SetData(SHADER_PARAM::TEX_4, &pTile2);
	pMtrl->SetData(SHADER_PARAM::TEX_5, &pTile_n2);

	pMtrl->SetData(SHADER_PARAM::TEX_6, &pTile3);
	pMtrl->SetData(SHADER_PARAM::TEX_7, &pTile_n3);

	// 컴퓨트 쉐이더 및 메테리얼 생성
	CreateComputeShader();
}


void CTerrain::CreateComputeShader()
{
//	vector <Ptr<CTexture>> vecTex;
//	vecTex.push_back(CResMgr::GetInst()->Load<CTexture>(L"Brush0Tex", L"Texture\\Brush\\Brush_0.png"));
//	vecTex.push_back(CResMgr::GetInst()->Load<CTexture>(L"Brush1Tex", L"Texture\\Brush\\Brush_1.png"));
//	vecTex.push_back(CResMgr::GetInst()->Load<CTexture>(L"Brush2Tex", L"Texture\\Brush\\Brush_2.png"));
//	Ptr<CTexture> pArrTex = CResMgr::GetInst()->CreateArrayTexture(L"BrushArrayTex", vecTex);
//	pArrTex->Save(L"Texture\\Brush\\BrushArr.dds");

	Ptr<CTexture> pArrTex = CResMgr::GetInst()->Load<CTexture>(L"Brush0Tex", L"Texture\\Brush\\BrushArr.dds");

	// =================
	// Height Map Shader
	// =================
	Ptr<CShader> pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\compute.fx", "CS_HeightMap", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::COMPUTE);
	CResMgr::GetInst()->AddRes<CShader>(L"HeightMapShader", pShader);

	m_pComputeMtrl = new CMaterial;
	m_pComputeMtrl->SetShader(pShader);
	m_pComputeMtrl->DisableFileSave();
	m_pComputeMtrl->SetData(SHADER_PARAM::RWTEX_0, &m_pHeightMap);


	Vec2 vResolution(m_pHeightMap->Width(), m_pHeightMap->Height());
	m_iBrushIdx = 1;	//Brush Type //이승훈
	m_pComputeMtrl->SetData(SHADER_PARAM::INT_0, &m_iBrushIdx);
	m_pComputeMtrl->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale);
	m_pComputeMtrl->SetData(SHADER_PARAM::VEC2_2, &vResolution);
	m_pComputeMtrl->SetData(SHADER_PARAM::TEXARR_0, &pArrTex);


	//
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::INT_2, &m_iBrushIdx);
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale);		//이승훈
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEXARR_0, &pArrTex);
	//


	CResMgr::GetInst()->AddRes<CMaterial>(L"HeightMapMtrl", m_pComputeMtrl);


	// =====================
	// Weight Texture Shader
	// =====================
	pShader = new CShader;
	pShader->CreateComputeShader(L"Shader\\compute.fx", "CS_Weight", "cs_5_0");
	pShader->SetShaderPOV(SHADER_POV::COMPUTE);
	CResMgr::GetInst()->AddRes<CShader>(L"WeightShader", pShader);

	m_pWeightMtrl = new CMaterial;
	m_pWeightMtrl->SetShader(pShader);
	m_pWeightMtrl->DisableFileSave();

	m_pWeightMtrl->SetData(SHADER_PARAM::RWTEX_0, &m_pWeightTex); // g_rwtex_0 : 출력 텍스쳐 (weight texture)
	m_pWeightMtrl->SetData(SHADER_PARAM::TEXARR_0, &pArrTex); // g_texarr_0 : brush	
	m_pWeightMtrl->SetData(SHADER_PARAM::VEC2_2, &Vec2(m_pWeightTex->Width(), m_pWeightTex->Height())); // 가중치 해상도	

	m_iTileIdx = 1;
	m_iBrushIdx = 1;
	m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)
	m_pWeightMtrl->SetData(SHADER_PARAM::INT_3, &m_iBrushIdx);// g_int_2 : Brush Index (브러쉬 종류)

	m_pWeightMtrl->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale); // g_vec2_1 : 브러쉬 크기	

	CResMgr::GetInst()->AddRes<CMaterial>(L"WeightMtrl", m_pWeightMtrl);

	//
	Vec2 vFaceCount = Vec2((float)m_iFaceX, (float)m_iFaceZ);
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_2, &vFaceCount);		//이승훈
	//









}

Vec2 CTerrain::GetCollisionPos()
{
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	tRay ray = pMainCam->GetRay();

	float fXLen = m_iFaceX * Transform()->GetWorldScale().x;
	float fZLen = m_iFaceZ * Transform()->GetWorldScale().z;

	Vec3 vLBPos = Transform()->GetWorldPos();
	Vec3 vLTPos = vLBPos + Vec3(0.f, 0.f, fZLen);
	Vec3 vRTPos = vLTPos + Vec3(fXLen, 0.f, 0.f);
	Vec4 vPlane = XMPlaneFromPoints(vLBPos, vLTPos, vRTPos);

	float T = -(ray.vStart.Dot(vPlane) + vPlane.w) / ray.vDir.Dot(vPlane);
	Vec3 vIntersectPos = ray.vDir * T + ray.vStart;

	return Vec2(vIntersectPos.x - vLTPos.x, vLTPos.z - vIntersectPos.z) / Vec2(fXLen, fZLen);
}

void CTerrain::ChangeFaceCount(UINT _iFaceX, UINT _iFaceZ)
{
	m_iFaceX = _iFaceX;
	m_iFaceZ = _iFaceZ;

	CreateTerrainMesh();
}


void CTerrain::CreateTerrainMesh()
{
	vector<VTX> vecVtx;
	vector<UINT> vecIdx;

	Ptr<CMesh> pMesh = nullptr;

	// =============	
	// Terrain Mesh		
	// =============	
	pMesh = new CMesh;

	VTX v;

	for (UINT i = 0; i < m_iFaceZ + 1; ++i)
	{
		for (UINT j = 0; j < m_iFaceX + 1; ++j)
		{
			v.vPos = Vec3((float)j, 0.f, (float)i);
			v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
			v.vUV = Vec2((float)j, (float)(m_iFaceZ - i));
			v.vNormal = Vec3(0.f, 1.f, 0.f);
			v.vTangent = Vec3(1.f, 0.f, 0.f);
			v.vBinormal = Vec3(0.f, 0.f, 1.f);
			vecVtx.push_back(v);
		}
	}

	for (UINT i = 0; i < m_iFaceZ; ++i)
	{
		for (UINT j = 0; j < m_iFaceX; ++j)
		{
			// 0       
			// | \  
			// 2--1 
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_iFaceX + 1) * i + j + 1);
			vecIdx.push_back((m_iFaceX + 1) * i + j);

			// 1--2
			//  \ |
			//	  0
			vecIdx.push_back((m_iFaceX + 1) * (i)+(j + 1));
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j);
			vecIdx.push_back((m_iFaceX + 1) * (i + 1) + j + 1);
		}
	}

	pMesh->Create(vecVtx.data(), sizeof(VTX), (UINT)vecVtx.size(), D3D11_USAGE_DEFAULT
		, vecIdx.data(), DXGI_FORMAT_R32_UINT, (UINT)vecIdx.size(), D3D11_USAGE_DEFAULT);

	Ptr<CMesh> pOriginMesh = CResMgr::GetInst()->FindRes<CMesh>(L"TerrainMesh");
	if (nullptr != pOriginMesh)
	{
		CResMgr::GetInst()->DestroyResource<CMesh>(L"TerrainMesh");
	}

	CResMgr::GetInst()->AddRes<CMesh>(L"TerrainMesh", pMesh);

	MeshRender()->SetMesh(pMesh);
}