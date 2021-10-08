#pragma once

#include "define.h"
#include <functional>

struct tResolution
{
	float fWidth;
	float fHeight;
};

// 정점 구조체
struct VTX
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;  // 종법선
	Vec3 vNormal;   // 법선 벡터
	Vec3 vBinormal; // 외법선

	Vec4 vWeights; // Bone 가중치
	Vec4 vIndices; // Bone 인덱스
};

struct tDBG
{
	DBG_TYPE eType;
	wstring  strDBG;
};

struct tRay
{
	Vec3 vStart;
	Vec3 vDir;
};

struct tParticle
{
	Vec3 vWorldPos;		// 위치	
	Vec3 vWorldDir;		// 이동 방향

	float m_fCurTime;	// 현재 시간
	float m_fLifeTime;	// 최대 생명주기

	int  iAlive;
	int  arrPading[3];
};

struct tParticleShared
{
	int iAddCount;
	int arrPading[3];
};

// ===========
// Instancing
// ===========
union uInstID
{
	struct {
		UINT iMesh;
		WORD iMtrl;
		WORD iMtrlIdx;
	};
	ULONG64 llID;
};

class CGameObject;

struct tInstObj
{
	CGameObject*	pObj;
	UINT			iMtrlIdx;
};



// ===============
// 상수버퍼 메모리
// ===============
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;
	Matrix matWV;
	Matrix matWVP;
	Matrix matWorldInv;
	Matrix matViewInv;
	Matrix matProjInv;
};

struct tMtrlParam
{
	Vec4			m_vDiff; // 색상 계수
	Vec4			m_vSpec; // 반사광 계수
	Vec4			m_vEmv;  // 발광 계수

	int				m_arrInt[(UINT)SHADER_PARAM::INT_END - (UINT)SHADER_PARAM::INT_0];
	float			m_arrFloat[(UINT)SHADER_PARAM::FLOAT_END - (UINT)SHADER_PARAM::FLOAT_0];
	Vec2			m_arrVec2[(UINT)SHADER_PARAM::VEC2_END - (UINT)SHADER_PARAM::VEC2_0];
	Vec4			m_arrVec4[(UINT)SHADER_PARAM::VEC4_END - (UINT)SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[(UINT)SHADER_PARAM::MATRIX_END - (UINT)SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	int				m_iArrRWTex[(UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0];
};

struct tAnim2D
{
	Vec2 vLT;			// UV 좌상단
	Vec2 vLen;			// UV 길이
	Vec2 vOffset;		// Offset Pos
	int  IsAnim2D;		// Animation2D 컴포넌트 사용여부
	int  iPadding;
};

struct tLight2D
{
	Vec4 vLightPos;
	Vec4 vLightColor;
	Vec4 vLightDir;
	int   iLightType;
	float fRange;
	float fAngle;
	int   ipadding;
};

struct tLightColor
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
};

struct tLight3D
{
	tLightColor tColor;
	Vec4		vLightPos;
	Vec4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tGlobalValue
{
	tResolution vResolution;
	float fDT;
	float fAccTime;
};

struct tUnPack
{
	Vec4 PerspectiveValues;
	Matrix ViewInv;
};

//================
// Struct of FBX 
//=================
typedef struct _tagMtrlInfo
{
	Vec4	vMtrlDiff;
	Vec4	vMtrlSpec;
	Vec4	vMtrlAmb;
	Vec4	vMtrlEmiv;
	_tagMtrlInfo()
		: vMtrlDiff(1.f, 1.f, 1.f, 1.f)
		, vMtrlSpec(1.f, 1.f, 1.f, 1.f)
		, vMtrlAmb(1.f, 1.f, 1.f, 1.f)
		, vMtrlEmiv(1.f, 1.f, 1.f, 1.f)
	{}

}tMtrlInfo;

typedef struct _tagFbxMat
{
	tMtrlInfo	tMtrl;
	wstring     strMtrlName;
	wstring     strDiff;
	wstring		strNormal;
	wstring		strSpec;
}tFbxMaterial;

typedef struct _tagWeightsAndIndices
{
	int		iBoneIdx;
	double	dWeight;
}tWeightsAndIndices;

typedef struct _tagContainer
{
	wstring								strName;
	vector<Vec3>						vecPos;
	vector<Vec3>						vecTangent;
	vector<Vec3>						vecBinormal;
	vector<Vec3>						vecNormal;
	vector<Vec2>						vecUV;

	vector<Vec4>						vecIndices;
	vector<Vec4>						vecWeights;

	vector<vector<UINT>>				vecIdx;
	vector<tFbxMaterial>				vecMtrl;

	// Animation 관련 정보
	bool								bAnimation;
	vector<vector<tWeightsAndIndices>>	vecWI;

	void Resize(UINT _iSize)
	{
		vecPos.resize(_iSize);
		vecTangent.resize(_iSize);
		vecBinormal.resize(_iSize);
		vecNormal.resize(_iSize);
		vecUV.resize(_iSize);
		vecIndices.resize(_iSize);
		vecWeights.resize(_iSize);
		vecWI.resize(_iSize);
	}

}tContainer;

// Animation
struct tFrameTrans
{
	Vec4	vTranslate;
	Vec4	vScale;
	Vec4	qRot;

	void Clone(tFrameTrans frame)
	{
		vTranslate = frame.vTranslate;
		vScale = frame.vScale;
		qRot = frame.qRot;
	}

	tFrameTrans() {}
	tFrameTrans(Vec4 trans, Vec4 scale, Vec4 rot)
		: vTranslate(trans), vScale(scale), qRot(rot)
	{}
};

struct tMTKeyFrame
{
	double	dTime;
	int		iFrame;
	Vec3	vTranslate;
	Vec3	vScale;
	Vec4	qRot;


};

struct tMTBone
{
	wstring				strBoneName;
	int					iDepth;
	int					iParentIndx;
	Matrix				matOffset;	// Offset 행렬(뼈 -> 루트 까지의 행렬)
	Matrix				matBone;   // 이거 안씀
	vector<tMTKeyFrame>	vecKeyFrame;
};



// 06/01
// 상민규 - 메쉬 서브셋 만들기
struct tVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT				iSize;	// 정점 1개의 크기
	UINT				iCount;	// 정점 수
	D3D11_USAGE		eUsage;
	void*			pData;
};

struct tIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT				iSize;	// 정점 1개의 크기
	UINT				iCount;	// 정점 수
	D3D11_USAGE		eUsage;
	DXGI_FORMAT		eFormat;
	void*			pData;
};

typedef struct tMeshContainer
{
	wstring	strName;
	tVertexBuffer	tVB;
	std::vector<tIndexBuffer>	vecIB;
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitive;
};

// 상민규 06/09
// Transition

struct tMTAnimClip
{
	wstring		strName; // 클립 이름
	list<struct tAnimationTransition*>		TransitionList;

	wstring		strAnimName; // 짤린 clip의 이름
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;

	double		dStartTime;
	double		dEndTime;
	double		dTimeLength;
	float		fUpdateTime; // 이거 안씀



	FbxTime::EMode eMode;
};


struct tAnimationTransition
{
	wstring strTransitionName;
	std::function<bool()> funcTransition;
	list<std::function<void()>>			NotifyList;
	class CAnimClip* pAnimClip;
	bool			bAutoTransition = false;
};



struct tAnimNotify
{
	wstring		strName;
	class CAnimClip*			pAnimClip;
	vector<std::function<void(float)>>	vecFunction;
	float		fTime;
	bool		bCall;

	tAnimNotify() :
		bCall(false),
		fTime(0.f)
	{
	}
};
