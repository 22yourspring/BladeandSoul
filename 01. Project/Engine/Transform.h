#pragma once
#include "Component.h"

class CTransform :
	public CComponent
{
private:
	Vec3		m_vLocalPos;	// GameObject 좌표
	Vec3		m_vLocalScale;	// GameObject 크기
	Vec3		m_vLocalRot;	// GameObject 회전량

	Vec3		m_CameraTargetPos;	// 카메라가 공전하는 대상
	Vec3		m_TargetRot;	// 카메라가 공전하는 대상

	Vec3		m_vBonePos;
	Vec3		m_vBoneScale;
	Vec3		m_vBoneRot;

	Vec3		m_vLocalDir[(UINT)DIR_TYPE::END]; // GameObject 의 Front, Up, Right 방향
	Vec3		m_vWorldDir[(UINT)DIR_TYPE::END]; // GameObject 의 Front, Up, Right 방향

	Matrix		m_matWorld;
	Matrix		m_matWorldInv;
	Matrix		m_matBone;

	Matrix		m_matMainCamera;

	bool m_bBone;
public:
	virtual void finalupdate();

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	// 06/03  
	// Bone Socket 관련
	void SetBoneMatrix(const Matrix & matBone);
	void SetBonePos(const Vec3& vPos);
	void SetBoneRot(const Vec3& vRot);
	void SetBoneScale(const Vec3& vScale);
	void ComputeWorld();

	Vec3 GetCameraTargetPos() { return m_CameraTargetPos; }
	
	void _SetOrbitalTargetRot(Vec3 _SetRot) { m_TargetRot = _SetRot; }
	Vec3 _GetOrbitalTargetRot() { return m_TargetRot; }

	const Vec3& GetLocalPos() { return m_vLocalPos; }
	Vec3 GetWorldPos() { return m_matWorld.Translation(); }

	const Vec3& GetLocalScale() { return m_vLocalScale; }
	Vec3 GetWorldScale();

	const Vec3& GetLocalRot() { return m_vLocalRot; }
	const Matrix& GetWorldMat() { return m_matWorld; }

	const Matrix& GetWorldInvMat() { return m_matWorldInv; }

	const Vec3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	const Vec3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }

	void SetLocalPos(const Vec3& _vPos) { if (m_vLocalPos != _vPos) { m_vLocalPos = _vPos; Changed(); } }
	void SetLocalScale(const Vec3& _vScale) { if (m_vLocalScale != _vScale) { m_vLocalScale = _vScale; Changed(); } }
	void SetLocalRot(const Vec3& _vRot) { if (m_vLocalRot != _vRot) { m_vLocalRot = _vRot; Changed(); } }

	void LookAt(const Vec3& _vLook);

	bool IsCasting(const Vec3& _vPos);
	void UpdateData();

	float GetMaxScale();

	Matrix GetMainCamMatrix() { return m_matMainCamera; };
	void SetMainCamMatrix(Vec3 _ParentRot, Vec3 _ParentPos);


	void RotationMatrix(const Matrix & _matRot);
	
public:
	virtual CTransform* Clone() { return new CTransform(*this); }

public:
	CTransform();
	virtual ~CTransform();
};

