#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"

class CCollider3D :
	public CComponent
{
private:
	static			UINT  g_iColID;

private:
	Vec3			m_vOffsetPos;
	Vec3			m_vOffsetScale;
	COLLIDER3D_TYPE m_eType;
	int				m_iCollisionCount;	// 충돌 횟수

	Ptr<CMesh>      m_pColMesh;			// 충돌체를 그리기 위한 Mesh
	Ptr<CMaterial>	m_pColMtrl;			// 충돌체를 그리기 위한 Material

	Matrix			m_matColWorld;		// 충돌체 월드 행렬

	UINT			m_iColID;			// 충돌체 고유 ID 값

public:
	virtual void update();
	virtual void finalupdate();
	void render();

	void OnCollisionEnter(CCollider3D* _pOther); // 충돌체가 처음 충돌
	void OnCollision(CCollider3D* _pOther);      // 충돌 중
	void OnCollisionExit(CCollider3D* _pOther);  // 충돌체가 서로 떨어질 때

public:
	void SetOffsetPos(const Vec3& _vPos) { m_vOffsetPos = _vPos; }
	void SetOffsetScale(const Vec3& _vScale) { m_vOffsetScale = _vScale; }

	const Vec3& GetOffsetPos() { return m_vOffsetPos; }
	const Vec3& GetOffsetScale() { return m_vOffsetScale; }
	COLLIDER3D_TYPE GetColliderType() { return m_eType; }

	void SetCollider3DType(COLLIDER3D_TYPE _eType);
	UINT GetColID() { return m_iColID; }
	const Matrix& GetColliderWorldMat() { return m_matColWorld; }

public:
	CLONE(CCollider3D);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	void operator = (const CCollider3D& _other);

public:
	CCollider3D();
	CCollider3D(const CCollider3D& _other);
	virtual ~CCollider3D();
};

