#include "pch.h"
#include "StudioYCS.h"
#include "StudioRes.h"
#include "afxdialogex.h"

#include <Engine/PathMgr.h>
#include <Engine/ResMgr.h>
#include <Engine/texture.h>
#include <Engine/mesh.h>
#include <Engine/material.h>
#include <Engine/shader.h>
#include <Engine/sound.h>

void CStudioRes::init()
{
	//	CRect rt;
	//	GetClientRect(rt);
	//	CViewTree::CTreeCtrl::SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);

		// Contents Resource Loading
	LoadResource();

	//UI 테마
	SetBkColor(RGB(35, 35, 35));
	SetLineColor(RGB(125, 125, 125));
	SetTextColor(RGB(255, 255, 255));

	//	SetBkColor(RGB(245, 245, 255));		//White
	//	SetLineColor(RGB(125, 125, 125));
	//	SetTextColor(RGB(35, 35, 35));

	// 트리컨트롤 내용 갱신
}

void CStudioRes::update()
{
	ShowScrollBar(SB_HORZ, FALSE);
}

void CStudioRes::Renew()
{
	CViewTree::CTreeCtrl::DeleteAllItems();
	m_mapString.clear();

	HTREEITEM hParent = nullptr;

	HTREEITEM hRoot = InsertItem(_T("Scene Resource"), 0, 0);
	SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		hParent = AddItem(RES_TYPE_NAME[i], hRoot, 0);
		const map<wstring, CResource*>& mapres = CResMgr::GetInst()->GetResource((RES_TYPE)i);
		for (const auto& pair : mapres)
		{
			if (FindItemByString(CPathMgr::GetFileName(pair.first.c_str())) == nullptr)
			{
				AddItem(CPathMgr::GetFileName(pair.first.c_str()), hParent, (DWORD_PTR)pair.second);
				m_mapString.insert(make_pair(CPathMgr::GetFileName(pair.first.c_str()), hParent));
			}
		}
			Expand(hParent, TVE_EXPAND);
	}
	Expand(hRoot, TVE_EXPAND);
}

void CStudioRes::PostNcDestroy()
{
}

void CStudioRes::LoadResource()
{
	CString strContent = CPathMgr::GetResPath();

	LoadResource(strContent);
}

void CStudioRes::LoadResource(const CString & _strFolderPath)
{
	CString strPath = _strFolderPath + L"\\*.*";
	WIN32_FIND_DATA tData = {};
	HANDLE hFindHandle = FindFirstFile(strPath, &tData);
	CString strFileName;

	while (true)
	{
		strFileName = tData.cFileName;

		if (tData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
		//	if (strFileName != L"." && strFileName != L"..")
			if (strFileName != L"." && strFileName != L".." && strFileName != L"AnimClip" && strFileName != L"Excel" && strFileName != L"FBX" && strFileName != L"PSA" && strFileName != L"Mesh" && strFileName != L"MeshData" && strFileName != L"Material")
			{
				LoadResource(_strFolderPath + tData.cFileName + L"\\");
			}
		}
		else
		{
			//size_t iLen = strFileName.length();
			//size_t iIdx = strFileName.rfind(L'.');			
			//wstring strExt = strFileName.substr(iIdx, iLen - iIdx);			
			Load(_strFolderPath + strFileName);
		}


		if (!FindNextFile(hFindHandle, &tData))
			break;
	}

	FindClose(hFindHandle);
}

void CStudioRes::Load(const CString & _strFullPath)
{
	CString strExt = CPathMgr::GetExt(_strFullPath);
	CString strRelativePath = CPathMgr::GetRelativePath(_strFullPath);

	if (strExt == L".png" || strExt == L".tga" || strExt == L".dds" || strExt == L".jpeg" || strExt == L".jpg" || strExt == L".bmp")
	{
		CResMgr::GetInst()->Load<CTexture>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());
	}
	else if (strExt == L".mtrl")
	{
		CResMgr::GetInst()->Load<CMaterial>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());
	}
	else if (strExt == L".mp3" || strExt == L".ogg" || strExt == L".wav")
	{
		CResMgr::GetInst()->Load<CSound>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());
	}
	else if (strExt == L".mesh")
	{
		CResMgr::GetInst()->Load<CMesh>(strRelativePath.GetBuffer(), strRelativePath.GetBuffer());
	}
}

CStudioRes::CStudioRes()
{
}

CStudioRes::~CStudioRes()
{
}
BEGIN_MESSAGE_MAP(CStudioRes, CViewTree)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CStudioRes::OnTvnBegindrag)
END_MESSAGE_MAP()


void CStudioRes::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	return;
	*pResult = 0;
}
