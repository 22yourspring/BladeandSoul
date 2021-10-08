#pragma once

class Matrix;

class CBoneSocket
{
	friend class CAnimator3D;

private:
	CBoneSocket();
	~CBoneSocket();

private:
	wstring		m_strName;
	wstring		m_strBoneName;
	list<class CComponent*>	m_ComponentList;
	Vec3	m_vOffset;
	Vec3	m_vRotation;
	int			m_iBoneIndex;

public:
	int GetBoneIndex()	const;
	const wstring& GetName() const { return m_strName; }

public:
	void SetInfo(const wstring& strName, const wstring& strBoneName,
		int iBoneIndex);
	void SetOffset(const Vec3& vOffset);
	void SetRotation(const Vec3& vRotation);
	void AddComponent(class CComponent* pCom);
	void AddObject(class CGameObject* pObj);
	void DeleteObject(class CGameObject* pObj);
	void DeleteObject(class CComponent* pCom);
	void DeleteObject();

	// 소켓에서 오브젝트를 없앤후, 반환
	class CGameObject* TakeOutObject(CGameObject * pObj);

public:
	bool Init();
	void Update(float fTime, const Matrix & matBone);
};

