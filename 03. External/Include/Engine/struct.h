#pragma once

#include "define.h"
#include <functional>

struct tResolution
{
	float fWidth;
	float fHeight;
};

// ���� ����ü
struct VTX
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;  // ������
	Vec3 vNormal;   // ���� ����
	Vec3 vBinormal; // �ܹ���

	Vec4 vWeights; // Bone ����ġ
	Vec4 vIndices; // Bone �ε���
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
	Vec3 vWorldPos;		// ��ġ	
	Vec3 vWorldDir;		// �̵� ����

	float m_fCurTime;	// ���� �ð�
	float m_fLifeTime;	// �ִ� �����ֱ�

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
// ������� �޸�
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
	Vec4			m_vDiff; // ���� ���
	Vec4			m_vSpec; // �ݻ籤 ���
	Vec4			m_vEmv;  // �߱� ���

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
	Vec2 vLT;			// UV �»��
	Vec2 vLen;			// UV ����
	Vec2 vOffset;		// Offset Pos
	int  IsAnim2D;		// Animation2D ������Ʈ ��뿩��
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

	// Animation ���� ����
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
	Matrix				matOffset;	// Offset ���(�� -> ��Ʈ ������ ���)
	Matrix				matBone;   // �̰� �Ⱦ�
	vector<tMTKeyFrame>	vecKeyFrame;
};



// 06/01
// ��α� - �޽� ����� �����
struct tVertexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT				iSize;	// ���� 1���� ũ��
	UINT				iCount;	// ���� ��
	D3D11_USAGE		eUsage;
	void*			pData;
};

struct tIndexBuffer
{
	ID3D11Buffer*	pBuffer;
	UINT				iSize;	// ���� 1���� ũ��
	UINT				iCount;	// ���� ��
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

// ��α� 06/09
// Transition

struct tMTAnimClip
{
	wstring		strName; // Ŭ�� �̸�
	list<struct tAnimationTransition*>		TransitionList;

	wstring		strAnimName; // ©�� clip�� �̸�
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;

	double		dStartTime;
	double		dEndTime;
	double		dTimeLength;
	float		fUpdateTime; // �̰� �Ⱦ�



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
