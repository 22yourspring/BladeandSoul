
// StudioYCSDoc.cpp: CStudioYCSDoc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "StudioYCS.h"
#endif

#include "StudioYCSDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CStudioYCSDoc

IMPLEMENT_DYNCREATE(CStudioYCSDoc, CDocument)

BEGIN_MESSAGE_MAP(CStudioYCSDoc, CDocument)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CStudioYCSDoc, CDocument)
END_DISPATCH_MAP()

// 참고: IID_IStudioYCS에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다.
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {c762afd6-bdad-46f4-9c8d-586f5a65e180}
static const IID IID_IStudioYCS =
{0xc762afd6,0xbdad,0x46f4,{0x9c,0x8d,0x58,0x6f,0x5a,0x65,0xe1,0x80}};

BEGIN_INTERFACE_MAP(CStudioYCSDoc, CDocument)
	INTERFACE_PART(CStudioYCSDoc, IID_IStudioYCS, Dispatch)
END_INTERFACE_MAP()


// CStudioYCSDoc 생성/소멸

CStudioYCSDoc::CStudioYCSDoc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

	EnableAutomation();

	AfxOleLockApp();
}

CStudioYCSDoc::~CStudioYCSDoc()
{
	AfxOleUnlockApp();
}

BOOL CStudioYCSDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CStudioYCSDoc serialization

void CStudioYCSDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CStudioYCSDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CStudioYCSDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CStudioYCSDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CStudioYCSDoc 진단

#ifdef _DEBUG
void CStudioYCSDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CStudioYCSDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CStudioYCSDoc 명령
