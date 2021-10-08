#pragma once

#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"
#include "AnimClip.h"


class CStructuredBuffer;

class CAnimator3D : public CComponent
{
private:
	const vector<tMTBone>*		m_pVecBones;
	Ptr<CMaterial>				m_pBoneMtrl; // BoneData Update Compute Shader

	const vector<tMTAnimClip>*	m_pVecClip;


	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // �ؽ��Ŀ� ������ ���� �������
	int							m_iFrameCount; // 30
	double						m_dCurTime;
public:
	int							m_iCurClip; // Ŭ�� �ε���	
	int							m_iFrameIdx; // Ŭ���� ���� ������

	bool AnimIsFinish();

private:
	int							m_iFrameNextIdx; // Ŭ���� ���� ������
	int							m_iBoneCount;

	CStructuredBuffer*			m_pBoneFinalMat;  // Ư�� �������� ���� ���
	CStructuredBuffer*			m_pBoneKeyFrame;
	CStructuredBuffer*			m_pBoneNextKeyFrame;
	bool						m_bFinalMatUpdate; // ������� ���� ���࿩��

	// 06/03  
	// ���� ���� ��� ����
	std::list<class CBoneSocket*> m_SocketList;
	Ptr<CMesh> m_pMesh;

	// 06/10  
	// Ʈ������ ����
	std::vector<struct CAnimClip*>	m_vecClip;
	std::vector<struct tAnimNotify*> m_vecNotify;
	struct tAnimationTransition* m_pTransition;
	class CAnimClip* m_pCurrentAnimClip;

	bool					m_bStart; // Ʈ������ ù ����
	bool					m_bEnd;
	float					m_fAnimationGlobalTime;
	float					m_fClipProgress;
	float					m_fChangeTime;
	float					m_fChangeLimitTime;

	bool m_bStatic;
public:


	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetCurrentAnimClip(const wchar_t* _clipName);
	void UpdateData();
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	CStructuredBuffer* GetFinalBoneMat();

	void ClearData();

public:
	std::vector<struct CAnimClip*>* GetVecAnimClip();

private:
	void check_mesh(Ptr<CMesh> _pMesh);


public:
	virtual void awake() {};
	virtual void start() {};
	virtual void update();
	virtual void lateupdate();
	virtual void finalupdate();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	CAnimator3D* Clone() { return new CAnimator3D(*this); }

public:
	CAnimator3D();
	virtual ~CAnimator3D();




public:
	// 06/18  
	// ���⸦ A���Ͽ��� B�������� �̵���Ű��
	void MoveWeaponFromSocket(const wchar_t* strSocketNameA, const wchar_t* strSocketNameB, class CGameObject* pObj);

	// 06/03  
	// Bone Socket ���� ���� �Լ�...
	bool AddSocket(const wchar_t* strSocketName, const wchar_t* strBoneName);
	bool SetSocketObject(const wchar_t* strSocketName, class CGameObject* pObj);
	// bool SetSocketObject(const wchar_t* strSocketName, class CSceneComponent* pCom);
	bool DeleteSocketObject(const wchar_t* strSocketName, class CGameObject* pObj);
	// bool DeleteSocketObject(const wchar_t* strSocketName, class CSceneComponent* pCom);
	bool DeleteSocketObject(const wchar_t* strSocketName);
	bool SetSocketOffset(const wchar_t* strSocketName, const Vec3& vOffset);
	bool SetSocketRotation(const wchar_t* strSocketName, const Vec3& vRotation);
	class CBoneSocket* FindSocket(const wchar_t* strName);
	std::list<class CBoneSocket*>* GetSocketList() { return &m_SocketList; }

	// ���� ������Ʈ
	void SocketUpdate(float fTime, const Matrix& matWorld, int _iCur, int _iNext, bool _bTransition);

	int GetBoneCount()	const;
	const tMTBone* GetBone(int iIndex = 0)	const;
	const tMTBone* GetBone(const wstring& strName)	const;
	int GetBoneIndex(const wstring& strName)	const;

	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);

	// Ʈ������ ���� �Լ�
	class CAnimClip* GetAnimClip()	const;
	void SetPlayTime(const wstring& strClipName, float fPlayTime);
	bool AddAnimClip(const wstring& strClipName, const wstring& strInfoName);
	bool AddAnimClip(const wstring& strClipName);

	void SetStatic();

	CAnimClip* FindAnimClip(const wchar_t* strName);
	int FindAnimClipIndex(CAnimClip* _clip);

	void AddTransition(const wstring& strTransitionName, const wstring& strName,
		bool(*pFunc)(), const wstring& strNextName);
	void AutoTransition(const wstring& strInfoName, const wstring& strName, const wstring& strNextName);


	template <typename T>
	void AddTransition(const wstring& strTransitionName, const wstring& strName, T* pObj, bool(T::*pFunc)(), const wstring& strNextName)
	{
		CAnimClip*	pAnimClip = FindAnimClip(strName.c_str());

		if (!pAnimClip)
			return;

		CAnimClip*	pAnimNextClip = FindAnimClip(strNextName.c_str());

		if (!pAnimNextClip)
			return;

		tAnimationTransition*	pTransition = new tAnimationTransition;

		pTransition->pAnimClip = pAnimNextClip;
		pTransition->funcTransition = std::bind(pFunc, pObj);
		pTransition->strTransitionName = strTransitionName;


		pAnimClip->m_TransitionList.push_back(pTransition);
	}

	void AddTransitionNotify(const wstring& strName, const wstring& strInfoName, void(*pFunc)());

	template <typename T>
	void AddTransitionNotify(const wstring& _strName, const wstring& _strTransitionName, T* pObj, void(T::*pFunc)())
	{
		CAnimClip*	pAnimClip = FindAnimClip(_strName.c_str());

		if (!pAnimClip)
			return;

		auto	iter = pAnimClip->m_TransitionList.begin();
		auto	iterEnd = pAnimClip->m_TransitionList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strTransitionName == _strTransitionName)
			{
				(*iter)->NotifyList.push_back(std::bind(pFunc, pObj));
				break;
			}
		}
	}

	bool CreateNotify(const wstring& strSequenceName, const wstring& strNotifyName, float fTime);
	bool AddNotifyFunction(const wstring& strNotifyName, void(*pFunc)(float));

	template <typename T>
	bool AddNotifyFunction(const wstring& strNotifyName, T* pObj, void(T::*pFunc)(float))
	{
		auto	iter = m_vecNotify.begin();
		auto	iterEnd = m_vecNotify.end();

		tAnimNotify*	pNotify = nullptr;

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->strName == strNotifyName)
			{
				pNotify = *iter;
				break;
			}
		}

		if (!pNotify)
			return false;

		pNotify->vecFunction.push_back(std::bind(pFunc, pObj, std::placeholders::_1));

		return true;
	}
};

