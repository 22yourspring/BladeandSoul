#include "stdafx.h"
#include "ExcelMgr.h"

#include "ExcelMgr.h"

#include <sstream>

#include "FileDirectory.h"
#include "BinaryFile.h"

#include "Animator3D.h"
#include "Anim3DMgr.h"
#include "AnimClip.h"
#include "Mesh.h"
#include "StructuredBuffer.h"

CExcelMgr::CExcelMgr()
{
}


CExcelMgr::~CExcelMgr()
{
}

bool CExcelMgr::CreateBook()
{
	Book* book = xlCreateBook();

	if (nullptr == book)
	{
		return false;
	}

	return true;
}




bool CExcelMgr::LoadMonster(const TCHAR* filename)
{
	Book* book = xlCreateBook();



	if (book->load(L"HK.xls"))
	{
		Sheet* sheet = book->getSheet(0);

		if (nullptr != sheet)
		{
			for (int row = 0; row < sheet->lastRow(); ++row)
			{
				CellType cellType = sheet->cellType(row, 1);

				switch (cellType)
				{
				case CELLTYPE_EMPTY:
					break;
				case CELLTYPE_NUMBER:
				{
					int index = row % 7;
					switch (index)
					{
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					case 6:
						break;
					default:
						BOOM;
						break;

					}
					double d = sheet->readNum(row, 1);
					break;
				}
				case CELLTYPE_STRING:
				{
					const wchar_t* s = sheet->readStr(row, 1);
					break;
				}
				case CELLTYPE_BOOLEAN:
				{
					bool b = sheet->readBool(row, 1);
					break;
				}
				case CELLTYPE_BLANK:
					break;
				case CELLTYPE_ERROR:
					assert(false);
					break;
				}

			}
		}
	}
	else
	{
		assert(false);
	}

	book->release();

	return true;
}

void CExcelMgr::Test()
{
	LoadMonster(TEXT("../../Client2D/Include/HollowKnight.xls"));
}

int CExcelMgr::ReadingTest()
{
	Book* book = xlCreateBook();

	int logoId = book->addPicture(L"logo.png");

	// fonts

	Font* textFont = book->addFont();
	textFont->setSize(8);
	textFont->setName(L"Century Gothic");

	Font* titleFont = book->addFont(textFont);
	titleFont->setSize(38);
	titleFont->setColor(COLOR_GRAY25);

	Font* font12 = book->addFont(textFont);
	font12->setSize(12);

	Font* font10 = book->addFont(textFont);
	font10->setSize(10);

	// formats

	Format* textFormat = book->addFormat();
	textFormat->setFont(textFont);
	textFormat->setAlignH(ALIGNH_LEFT);

	Format* titleFormat = book->addFormat();
	titleFormat->setFont(titleFont);
	titleFormat->setAlignH(ALIGNH_RIGHT);

	Format* companyFormat = book->addFormat();
	companyFormat->setFont(font12);

	Format* dateFormat = book->addFormat(textFormat);
	dateFormat->setNumFormat(book->addCustomNumFormat(L"[$-409]mmmm\\ d\\,\\ yyyy;@"));

	Format* phoneFormat = book->addFormat(textFormat);
	phoneFormat->setNumFormat(
		book->addCustomNumFormat(L"[<=9999999]###\\-####;\\(###\\)\\ ###\\-####")
	);

	Format* borderFormat = book->addFormat(textFormat);
	borderFormat->setBorder();
	borderFormat->setBorderColor(COLOR_GRAY25);
	borderFormat->setAlignV(ALIGNV_CENTER);

	Format* percentFormat = book->addFormat(borderFormat);
	percentFormat->setNumFormat(book->addCustomNumFormat(L"#%_)"));
	percentFormat->setAlignH(ALIGNH_RIGHT);

	Format* textRightFormat = book->addFormat(textFormat);
	textRightFormat->setAlignH(ALIGNH_RIGHT);
	textRightFormat->setAlignV(ALIGNV_CENTER);

	Format* thankFormat = book->addFormat();
	thankFormat->setFont(font10);
	thankFormat->setAlignH(ALIGNH_CENTER);

	Format* dollarFormat = book->addFormat(borderFormat);
	dollarFormat->setNumFormat(
		book->addCustomNumFormat(L"_($* # ##0.00_);_($* (# ##0.00);_($* -??_);_(@_)")
	);

	// actions

	Sheet* sheet = book->addSheet(L"Sales Receipt");

	sheet->setDisplayGridlines(false);

	sheet->setCol(1, 1, 36);
	sheet->setCol(0, 0, 10);
	sheet->setCol(2, 4, 11);

	sheet->setRow(2, 47.25);
	sheet->writeStr(2, 1, L"Sales Receipt", titleFormat);
	sheet->setMerge(2, 2, 1, 4);
	sheet->setPicture(2, 1, logoId);

	sheet->writeStr(4, 0, L"Apricot Ltd.", companyFormat);
	sheet->writeStr(4, 3, L"Date:", textFormat);
	sheet->writeFormula(4, 4, L"TODAY()", dateFormat);

	sheet->writeStr(5, 3, L"Receipt #:", textFormat);
	sheet->writeNum(5, 4, 652, textFormat);

	sheet->writeStr(8, 0, L"Sold to:", textFormat);
	sheet->writeStr(8, 1, L"John Smith", textFormat);
	sheet->writeStr(9, 1, L"Pineapple Ltd.", textFormat);
	sheet->writeStr(10, 1, L"123 Dreamland Street", textFormat);
	sheet->writeStr(11, 1, L"Moema, 52674", textFormat);
	sheet->writeNum(12, 1, 2659872055, phoneFormat);

	sheet->writeStr(14, 0, L"Item #", textFormat);
	sheet->writeStr(14, 1, L"Description", textFormat);
	sheet->writeStr(14, 2, L"Qty", textFormat);
	sheet->writeStr(14, 3, L"Unit Price", textFormat);
	sheet->writeStr(14, 4, L"Line Total", textFormat);

	for (unsigned short row = 15; row < 38; ++row)
	{
		sheet->setRow(row, 15);
		for (unsigned short col = 0; col < 3; ++col)
		{
			sheet->writeBlank(row, col, borderFormat);
		}
		sheet->writeBlank(row, 3, dollarFormat);

		std::wstringstream stream;
		stream << "IF(C" << row + 1 << ">0;ABS(C" << row + 1 << "*D" << row + 1 << ");\"\")";
		sheet->writeFormula(row, 4, stream.str().c_str(), dollarFormat);
	}

	sheet->writeStr(38, 3, L"Subtotal ", textRightFormat);
	sheet->writeStr(39, 3, L"Sales Tax ", textRightFormat);
	sheet->writeStr(40, 3, L"Total ", textRightFormat);
	sheet->writeFormula(38, 4, L"SUM(E16:E38)", dollarFormat);
	sheet->writeNum(39, 4, 0.2, percentFormat);
	sheet->writeFormula(40, 4, L"E39+E39*E40", dollarFormat);
	sheet->setRow(38, 15);
	sheet->setRow(39, 15);
	sheet->setRow(40, 15);

	sheet->writeStr(42, 0, L"Thank you for your business!", thankFormat);
	sheet->setMerge(42, 42, 0, 4);

	// items

	sheet->writeNum(15, 0, 45, borderFormat);
	sheet->writeStr(15, 1, L"Grapes", borderFormat);
	sheet->writeNum(15, 2, 250, borderFormat);
	sheet->writeNum(15, 3, 4.5, dollarFormat);

	sheet->writeNum(16, 0, 12, borderFormat);
	sheet->writeStr(16, 1, L"Bananas", borderFormat);
	sheet->writeNum(16, 2, 480, borderFormat);
	sheet->writeNum(16, 3, 1.4, dollarFormat);

	sheet->writeNum(17, 0, 19, borderFormat);
	sheet->writeStr(17, 1, L"Apples", borderFormat);
	sheet->writeNum(17, 2, 180, borderFormat);
	sheet->writeNum(17, 3, 2.8, dollarFormat);

	book->save(L"receipt.xls");
	book->release();

	return 0;
}

void CExcelMgr::LoadPSA(const TCHAR * psaName)
{
	CFileDirectory newDir;

	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"PSA");



	std::vector<CBinaryFile> vecFiles = newDir.DirToAllFile(psaName); // 파일이름으로 PSA에 있는 파일 찾아오기	

	wstring filePath = newDir.GetPath() + L"\\" + vecFiles[0].GetPath();

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, filePath.c_str(), L"rb");

	// Excel File 열기
	Book* book = xlCreateBook();
	wstring fileName = vecFiles[0].FileName() + L".xls";

	book->addSheet(L"AnimInfo");
	Sheet* sheet = book->getSheet(0);

	// sheet->writeNum(16, 0, 12, borderFormat);
	// sheet->writeStr(16, 1, L"Bananas", borderFormat);


	// Chunk Header 읽기
	char chunkID[20] = {};
	fread(chunkID, 1, 20, pFile);
	int iTypeFlag = 0;
	fread(&iTypeFlag, sizeof(int), 1, pFile);
	int iDataSize = 0;
	fread(&iDataSize, sizeof(int), 1, pFile);
	int iDataCount = 0;
	fread(&iDataCount, sizeof(int), 1, pFile);


	// Bones
	// Bones 헤더
	char chunkIDBone[20] = {};
	fread(chunkIDBone, 1, 20, pFile);
	int iTypeFlagBone = 0;
	fread(&iTypeFlagBone, sizeof(int), 1, pFile);
	int iDataSizeBone = 0;
	fread(&iDataSizeBone, sizeof(int), 1, pFile);
	int iDataCountBone = 0;
	fread(&iDataCountBone, sizeof(int), 1, pFile);

	// Bone
	for (int i = 0; i < iDataCountBone; ++i)
	{
		char boneName[64] = {};
		fread(&boneName, 1, 64, pFile);

		int boneFlags = 0;
		fread(&boneFlags, sizeof(int), 1, pFile);
		// 128 + 4 = 132
		// 228 - 132 = 
		int children = 0;
		fread(&children, sizeof(int), 1, pFile);

		int parentIndex = 0;
		fread(&parentIndex, sizeof(int), 1, pFile);

		int rotX = 0;
		fread(&rotX, sizeof(int), 1, pFile);

		int rotY = 0;
		fread(&rotY, sizeof(int), 1, pFile);

		int rotZ = 0;
		fread(&rotZ, sizeof(int), 1, pFile);

		int rotW = 0;
		fread(&rotW, sizeof(int), 1, pFile);

		int posX = 0;
		fread(&posX, sizeof(int), 1, pFile);

		int posY = 0;
		fread(&posY, sizeof(int), 1, pFile);

		int posZ = 0;
		fread(&posZ, sizeof(int), 1, pFile);

		char padding[16] = {};
		fread(&padding, 1, 16, pFile);

	}


	// Info
	// Info 헤더
	char chunkIDInfo[20] = {};
	fread(chunkIDInfo, 1, 20, pFile);
	int iTypeFlagInfo = 0;
	fread(&iTypeFlagInfo, sizeof(int), 1, pFile);
	int iDataSizeInfo = 0;
	fread(&iDataSizeInfo, sizeof(int), 1, pFile);
	int iDataCountInfo = 0;
	fread(&iDataCountInfo, sizeof(int), 1, pFile);

	for (int i = 0; i < iDataCountInfo; ++i)
	{
		char seqName[64] = {};
		fread(&seqName, 1, 64, pFile);

		string tempString = seqName;
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &tempString[0], (int)tempString.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &tempString[0], (int)tempString.size(), &wstrTo[0], size_needed);
		sheet->writeStr(i + 1, 0, wstrTo.c_str());

		char seqGroup[64] = {};
		fread(&seqGroup, 1, 64, pFile);

		int boneCount = 0;
		fread(&boneCount, sizeof(int), 1, pFile);

		int rootInclude = 0;
		fread(&rootInclude, sizeof(int), 1, pFile);

		int compStyle = 0;
		fread(&compStyle, sizeof(int), 1, pFile);

		int keyQuot = 0;
		fread(&keyQuot, sizeof(int), 1, pFile);

		int keyReduc = 0;
		fread(&keyReduc, sizeof(int), 1, pFile);

		int trackTime = 0;
		fread(&trackTime, sizeof(int), 1, pFile);

		int animRate = 0;
		fread(&animRate, sizeof(int), 1, pFile);

		int startBone = 0;
		fread(&startBone, sizeof(int), 1, pFile);

		int firstRawFrame = 0;
		fread(&firstRawFrame, sizeof(int), 1, pFile);

		int rawFrameCount = 0;
		fread(&rawFrameCount, sizeof(int), 1, pFile);
		sheet->writeNum(i + 1, 1, rawFrameCount);
	}

	// Key
	// Key 헤더
	char chunkIDKey[20] = {};
	fread(chunkIDKey, 1, 20, pFile);
	int iTypeFlagKey = 0;
	fread(&iTypeFlagKey, sizeof(int), 1, pFile);
	int iDataSizeKey = 0;
	fread(&iDataSizeKey, sizeof(int), 1, pFile);
	int iDataCountKey = 0;
	fread(&iDataCountKey, sizeof(int), 1, pFile);

	// skip
	for (int i = 0; i < iDataCountKey; ++i)
	{
		char key[32] = {};
		fread(&key, 1, 32, pFile);
	}

	newDir.MoveParent();
	newDir.Move(L"Excel");

	wstring excelPath = newDir.GetPath() + L"\\" + fileName;

	if (false == book->save(excelPath.c_str()))
	{
		assert(false);
		// 애니메이션 정보를 엑셀 저장에 실패했음.
	}

	book->release();
	fclose(pFile);
}

// 06/11   - 애니메이션 자르기 사용법
// 1. 인자 = psa 정보가 담긴 excel파일 이름, 전체의 애니메이션이 담긴 mesh
// 2. CAnimClip을 새로 만들어서 excel파일에 있는 정보를 담고, mesh의 AnimClip 정보를 잘라서 넣어준다.
// 3. 만든 CAnimClip을 모두 AnimationMgr에 넣는다.
// 4. AnimationMgr에 담긴 모든 AnimClip을 ".clip" 확장자를 가진 파일로 저장한다.
void CExcelMgr::SavePSAToAnimClipFile(const TCHAR * _psaName, Ptr<CMesh> _pMesh)
{
	// 1. 엑셀 파일을 불러 온다.
	Book* book = xlCreateBook();

	// Mesh에서 tMTAnimClip과 pMesh->m_pBoneFrameData 받아오기
	tMTAnimClip mtAnimClip = _pMesh->GetAnimClip()->at(0);
	double dTotalTime = mtAnimClip.dEndTime;
	const vector<vector<tFrameTrans>>* tempKeyFrame = _pMesh->GetKeyFrame();

	int iBoneCount = tempKeyFrame->size();
	int iFrameCount = _pMesh->GetFrameCount();

	//vector<tFrameTrans> vecKeyFrame;

	//int totalSize = (int)tempKeyFrame->size();

	//for (int i = 0; i < totalSize; ++i)
	//{
	//	tFrameTrans temp = {};

	//	temp.Clone(tempKeyFrame->at(i));

	//	vecKeyFrame.push_back(temp);
	//}

	CFileDirectory newDir;

	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"Excel");

	wstring fileName = _psaName;
	wstring fileFullPath = newDir.GetPath() + L"\\" + fileName + L".xls";

	int index = 0;
	int start = 0;

	bool isLoaded = book->load(fileFullPath.c_str());

	//if (false == isLoaded)
	//{
	//	book->save(fileFullPath.c_str());
	//}

	if (true == isLoaded)
	{
		Sheet* sheet = book->getSheet(0);

		if (nullptr != sheet)
		{
			int totalY = sheet->lastRow();
			int totalX = sheet->lastCol();

			for (int row = 1; row < totalY; ++row)
			{
				CAnimClip* pAnimClip = new CAnimClip();

				pAnimClip->m_strName = fileName;



				for (int col = 0; col < totalX; ++col)
				{
					CellType cellType = sheet->cellType(row, col);

					switch (cellType)
					{
					case CELLTYPE_EMPTY:
						break;
					case CELLTYPE_NUMBER:
					{
						pAnimClip->m_iStartFrame = 0;
						pAnimClip->m_iEndFrame = sheet->readNum(row, col) - 1;
						break;
					}
					case CELLTYPE_STRING:
					{
						pAnimClip->m_strClipName = sheet->readStr(row, col);
						break;
					}
					case CELLTYPE_BOOLEAN:
					{
						break;
					}
					case CELLTYPE_BLANK:
						break;
					case CELLTYPE_ERROR:
						assert(false);
						break;
					}
				}

				pAnimClip->m_iFrameLength = pAnimClip->m_iEndFrame;
				pAnimClip->m_eMode = mtAnimClip.eMode;
				////////////////////////////////////////////////////////////
				// 프레임 자르기
				vector<tFrameTrans> vecKeyFrame;

				int totalSize = pAnimClip->m_iFrameLength + 1;
				start = index;

				vecKeyFrame.resize(iBoneCount * totalSize);


				// 본 개수
				for (int k = 0; k < iBoneCount; ++k)
				{
					vector<tFrameTrans> temp = tempKeyFrame->at(k);

					// 프레임 개수
					for (index = start; index < start + totalSize; ++index)
					{
						if (0 >= (int)temp.size())
						{
							continue;
						}

						if (index >= iFrameCount)
						{
							break;
						}

						tFrameTrans data = tFrameTrans(
							Vec4(temp[index].vTranslate)
							, Vec4(temp[index].vScale)
							, Vec4(temp[index].qRot)
						);

						int in = iBoneCount * (index - start) + k;
						vecKeyFrame[in] = data;
					}
				}

				////////////////////////////////////////////////////////////
				pAnimClip->m_vecKeyFrame = vecKeyFrame;

				pAnimClip->m_iFrameSize = (UINT)_pMesh->GetBoneMatrix()->size() * pAnimClip->m_iFrameLength;

				// pAnimClip->m_pBoneKeyFrame = new CStructuredBuffer;
				// pAnimClip->m_pBoneKeyFrame->Create(sizeof(tFrameTrans), pAnimClip->m_iFrameSize, vecKeyFrame.data());

				pAnimClip->m_dStartTime = 0;
				pAnimClip->m_dEndTime = ((double)(totalSize - 1) / (double)30);
				pAnimClip->m_dTimeLength = pAnimClip->m_dEndTime;

				CAnim3DMgr::GetInst()->AddAnimClip(pAnimClip);
			}
		}
	}

	CAnim3DMgr::GetInst()->SaveAll();
	book->release();
}

void CExcelMgr::SavePSAToAnimClipFile(const TCHAR* _psaJob, const TCHAR* _psaName, Ptr<class CMesh> _pMesh)
{

	// Mesh에서 tMTAnimClip과 pMesh->m_pBoneFrameData 받아오기
	tMTAnimClip mtAnimClip = _pMesh->GetAnimClip()->at(0);
	double dTotalTime = mtAnimClip.dEndTime;
	const vector<vector<tFrameTrans>>* tempKeyFrame = _pMesh->GetKeyFrame();

	int iBoneCount = tempKeyFrame->size();
	int iFrameCount = _pMesh->GetFrameCount();

	//vector<tFrameTrans> vecKeyFrame;

	//int totalSize = (int)tempKeyFrame->size();

	//for (int i = 0; i < totalSize; ++i)
	//{
	//	tFrameTrans temp = {};

	//	temp.Clone(tempKeyFrame->at(i));

	//	vecKeyFrame.push_back(temp);
	//}

	int index = 0;
	int start = 0;

	wstring fileName = _psaName;

	//if (false == isLoaded)
	//{
	//	book->save(fileFullPath.c_str());
	//}

	wstring psaNameArr[210];
	int psaNumberArr[210];		

	psaNameArr[0] = L"AA_A_None_Immune_TypeA";
	psaNameArr[1] = L"AA_A_None_Immune_TypeB";
	psaNameArr[2] = L"AA_A_None_Immune_TypeC";
	psaNameArr[3] = L"AA_A_None_Skl_SidestepLeft_Fire";
	psaNameArr[4] = L"AA_A_None_Skl_SidestepRight_Fire";
	psaNameArr[5] = L"BK_A_Bighit_G1_Skl_1000hand_Exec1";
	psaNameArr[6] = L"BK_A_Bighit_G1_Skl_1000hand_Exec2";
	psaNameArr[7] = L"BK_A_Bighit_G1_Skl_1000hand_Finish1";
	psaNameArr[8] = L"BK_A_Bighit_G1_Skl_1000hand_Finish2";
	psaNameArr[9] = L"BK_A_Bighit_G1_Skl_1000hand_Swing1";
	psaNameArr[10] = L"BK_A_Bighit_G1_Skl_1000hand_Swing2";
	psaNameArr[11] = L"BK_A_Bighit_G1_Skl_fullpower_End";
	psaNameArr[12] = L"BK_A_Bighit_G1_Skl_fullpower_Exec";
	psaNameArr[13] = L"BK_A_Bighit_G1_Skl_fullpower_Swing";
	psaNameArr[14] = L"BK_A_Bighit_G1_Skl_wind_kick1_End";
	psaNameArr[15] = L"BK_A_Bighit_G1_Skl_wind_kick1_Exec";
	psaNameArr[16] = L"BK_A_Bighit_G1_Skl_wind_kick1_Swing";
	psaNameArr[17] = L"BK_A_Bighit_G1_Skl_wind_kick2_End";
	psaNameArr[18] = L"BK_A_Bighit_G1_Skl_wind_kick2_Exec";
	psaNameArr[19] = L"BK_A_Bighit_G1_Skl_wind_kick2_Swing";
	psaNameArr[20] = L"BK_A_Bighit_G1_Skl_wind_kick3_End";
	psaNameArr[21] = L"BK_A_Bighit_G1_Skl_wind_kick3_Exec";
	psaNameArr[22] = L"BK_A_Bighit_G1_Skl_wind_kick3_Swing";
	psaNameArr[23] = L"BK_A_Bighit_G1_Skl_wind_kick4_End";
	psaNameArr[24] = L"BK_A_Bighit_G1_Skl_wind_kick4_Exec";
	psaNameArr[25] = L"BK_A_Bighit_G1_Skl_wind_kick4_Swing";
	psaNameArr[26] = L"BK_A_Bighit_G1_Skl_wind_mode_End";
	psaNameArr[27] = L"BK_A_Bighit_G1_Skl_wind_mode_Exec";
	psaNameArr[28] = L"BK_A_Bighit_G1_Skl_wind_mode_Swing";
	psaNameArr[29] = L"BK_A_Bighit_G2_Skl_fire_mode_End";
	psaNameArr[30] = L"BK_A_Bighit_G2_Skl_fire_mode_Exec";
	psaNameArr[31] = L"BK_A_Bighit_G2_Skl_fire_mode_Swing";
	psaNameArr[32] = L"BK_A_Bighit_G2_Skl_fire_punch1_End";
	psaNameArr[33] = L"BK_A_Bighit_G2_Skl_fire_punch1_Swing";
	psaNameArr[34] = L"BK_A_Bighit_G2_Skl_fire_punch2_End";
	psaNameArr[35] = L"BK_A_Bighit_G2_Skl_fire_punch2_Swing";
	psaNameArr[36] = L"BK_A_Bighit_G2_Skl_fire_punch3_End";
	psaNameArr[37] = L"BK_A_Bighit_G2_Skl_fire_punch3_Swing";
	psaNameArr[38] = L"BK_A_Bighit_G2_Skl_fire_punch4_End";
	psaNameArr[39] = L"BK_A_Bighit_G2_Skl_fire_punch4_Swing";
	psaNameArr[40] = L"BK_A_Bighit_G2_Skl_ForceRoda9_End";
	psaNameArr[41] = L"BK_A_Bighit_G2_Skl_ForceRoda9_Exec";
	psaNameArr[42] = L"BK_A_Bighit_G2_Skl_ForceRoda9_Swing";
	psaNameArr[43] = L"BK_A_Bighit_G2_Skl_fullpower_End";
	psaNameArr[44] = L"BK_A_Bighit_G2_Skl_fullpower_Exec";
	psaNameArr[45] = L"BK_A_Bighit_G2_Skl_fullpower_Swing";
	psaNameArr[46] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Combo2_End";
	psaNameArr[47] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Combo2_Exec1";
	psaNameArr[48] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Combo2_Swing";
	psaNameArr[49] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Combo3_Exec";
	psaNameArr[50] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Combo3_Swing";
	psaNameArr[51] = L"BK_A_Bighit_G3_Skl_Attack_Arang_End";
	psaNameArr[52] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Exec";
	psaNameArr[53] = L"BK_A_Bighit_G3_Skl_Attack_Arang_Swing";
	psaNameArr[54] = L"BK_A_Bighit_G3_Skl_Attack_ChangeIronFist_Exec";
	psaNameArr[55] = L"BK_A_Bighit_G3_Skl_Attack_ChangeIronFist_Swing";
	psaNameArr[56] = L"BK_A_Bighit_G3_Skl_Attack_ChangeWolf_Cast";
	psaNameArr[57] = L"BK_A_Bighit_G3_Skl_Attack_ChangeWolf_Swing";
	psaNameArr[58] = L"BK_A_Bighit_G3_Skl_Attack_ChangeWolfEnd_Exec";
	psaNameArr[59] = L"BK_A_Bighit_G3_Skl_Attack_CrushGround_Exec";
	psaNameArr[60] = L"BK_A_Bighit_G3_Skl_Attack_CrushGround_Swing";
	psaNameArr[61] = L"BK_A_Bighit_G3_Skl_Attack_DoublePalmAttack_Exec";
	psaNameArr[62] = L"BK_A_Bighit_G3_Skl_Attack_DoublePalmAttack_Swing";
	psaNameArr[63] = L"BK_A_Bighit_G3_Skl_Attack_LeftCounterR_Exec";
	psaNameArr[64] = L"BK_A_Bighit_G3_Skl_Attack_LeftCounterR_Swing";
	psaNameArr[65] = L"BK_A_Bighit_G3_Skl_Attack_PalmAttack_Exec";
	psaNameArr[66] = L"BK_A_Bighit_G3_Skl_Attack_PalmAttack_Swing";
	psaNameArr[67] = L"BK_A_Bighit_G3_Skl_Attack_PalmAttackL_Exec";
	psaNameArr[68] = L"BK_A_Bighit_G3_Skl_Attack_PalmAttackL_Swing";
	psaNameArr[69] = L"BK_A_Bighit_G3_Skl_Attack_Spinkick_Exec";
	psaNameArr[70] = L"BK_A_Bighit_G3_Skl_Attack_Spinkick_Swing";
	psaNameArr[71] = L"BK_A_Bighit_G3_Skl_Attack_WindWolfKick_End";
	psaNameArr[72] = L"BK_A_Bighit_G3_Skl_Attack_WindWolfKick_Exec";
	psaNameArr[73] = L"BK_A_Bighit_G3_Skl_Attack_WindWolfKick_Swing";
	psaNameArr[74] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronCannon_Exec1";
	psaNameArr[75] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronCannon_Exec2";
	psaNameArr[76] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronCannon_Swing1";
	psaNameArr[77] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronCannon_Swing2";
	psaNameArr[78] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronFist_Exec";
	psaNameArr[79] = L"BK_A_Bighit_G3_Skl_DarkSoul_IronFist_Swing";
	psaNameArr[80] = L"BK_A_Bighit_G3_Skl_DarkSoul_LeftCounter_Exec";
	psaNameArr[81] = L"BK_A_Bighit_G3_Skl_DarkSoul_LeftCounter_Swing";
	psaNameArr[82] = L"BK_A_Bighit_G3_Skl_DarkSoul_Parry_End";
	psaNameArr[83] = L"BK_A_Bighit_G3_Skl_DarkSoul_Parry_Looping";
	psaNameArr[84] = L"BK_A_Bighit_G3_Skl_DarkSoul_Parry_Start";
	psaNameArr[85] = L"BK_A_Bighit_G3_Skl_DarkSoul_SoulAttack_Exec";
	psaNameArr[86] = L"BK_A_Bighit_G3_Skl_DarkSoul_SoulAttack_Swing";
	psaNameArr[87] = L"BK_A_Bighit_G3_Skl_DarkSoul_Strike_Exec";
	psaNameArr[88] = L"BK_A_Bighit_G3_Skl_DarkSoul_Strike_Swing";
	psaNameArr[89] = L"BK_A_Bighit_G3_Skl_DarkSoul_Tornado_Exec1";
	psaNameArr[90] = L"BK_A_Bighit_G3_Skl_DarkSoul_Tornado_Exec2";
	psaNameArr[91] = L"BK_A_Bighit_G3_Skl_DarkSoul_Tornado_Swing1";
	psaNameArr[92] = L"BK_A_Bighit_G3_Skl_DarkSoul_Tornado_Swing2";
	psaNameArr[93] = L"BK_A_Bighit_G3_Skl_DarkSoul_Wave_Exec";
	psaNameArr[94] = L"BK_A_Bighit_G3_Skl_DarkSoul_Wave_Swing";
	psaNameArr[95] = L"BK_A_Bighit_Skl_1000hand_Exec1";
	psaNameArr[96] = L"BK_A_Bighit_Skl_1000hand_Exec2";
	psaNameArr[97] = L"BK_A_Bighit_Skl_1000hand_Finish1";
	psaNameArr[98] = L"BK_A_Bighit_Skl_1000hand_Finish2";
	psaNameArr[99] = L"BK_A_Bighit_Skl_1000hand_Swing1";
	psaNameArr[100] = L"BK_A_Bighit_Skl_1000hand_Swing2";
	psaNameArr[101] = L"BK_A_bighit_Skl_4hitkick";
	psaNameArr[102] = L"BK_A_bighit_Skl_AbdomenAttack_Exec";
	psaNameArr[103] = L"BK_A_bighit_Skl_AbdomenAttack_Fire";
	psaNameArr[104] = L"BK_A_Bighit_Skl_AirMartialdance1_Exec";
	psaNameArr[105] = L"BK_A_Bighit_Skl_AirMartialdance1_Fire";
	psaNameArr[106] = L"BK_A_Bighit_Skl_AirMartialdance2_Exec";
	psaNameArr[107] = L"BK_A_Bighit_Skl_AirMartialdance2_Fire";
	psaNameArr[108] = L"BK_A_Bighit_Skl_AirMartialdance3_Exec";
	psaNameArr[109] = L"BK_A_Bighit_Skl_AirMartialdance3_Fire";
	psaNameArr[110] = L"BK_A_Bighit_Skl_AirMartialdance4_Exec";
	psaNameArr[111] = L"BK_A_Bighit_Skl_AirMartialdance4_Fire";
	psaNameArr[112] = L"Bk_A_Bighit_Skl_Backstep";
	psaNameArr[113] = L"BK_A_bighit_Skl_BurstAbdomen";
	psaNameArr[114] = L"Bk_A_Bighit_Skl_BurstPunch_1st";
	psaNameArr[115] = L"Bk_A_Bighit_Skl_BurstPunch_2nd";
	psaNameArr[116] = L"Bk_A_Bighit_Skl_BurstPunch_3rd";
	psaNameArr[117] = L"Bk_A_Bighit_Skl_BurstSpinKick";
	psaNameArr[118] = L"Bk_A_Bighit_Skl_BurstSpinKick_Short";
	psaNameArr[119] = L"BK_A_Bighit_Skl_Chang2Com";
	psaNameArr[120] = L"BK_A_Bighit_Skl_Command_Front_Side_End";
	psaNameArr[121] = L"BK_A_Bighit_Skl_Command_Front_Side_Fire";
	psaNameArr[122] = L"BK_A_Bighit_Skl_Command_Left_Side_Exec";
	psaNameArr[123] = L"BK_A_Bighit_Skl_Command_Left_Side_Fire";
	psaNameArr[124] = L"BK_A_Bighit_Skl_Command_Right_Side_Exec";
	psaNameArr[125] = L"BK_A_Bighit_Skl_Command_Right_Side_Fire";
	psaNameArr[126] = L"BK_A_Bighit_Skl_Counterenemy_Exec";
	psaNameArr[127] = L"BK_A_Bighit_Skl_counterenemy_finish";
	psaNameArr[128] = L"BK_A_Bighit_Skl_counterenemy_ready";
	psaNameArr[129] = L"BK_A_Bighit_Skl_counterenemy_standby";
	psaNameArr[130] = L"Bk_A_Bighit_Skl_CrushBlow";
	psaNameArr[131] = L"Bk_A_Bighit_Skl_CrushBlow_Cast";
	psaNameArr[132] = L"Bk_A_Bighit_Skl_CrushBlow_Exec";
	psaNameArr[133] = L"Bk_A_Bighit_Skl_CrushBlow_Short";
	psaNameArr[134] = L"BK_A_bighit_Skl_DashUpper_Exec";
	psaNameArr[135] = L"BK_A_bighit_Skl_DashUpper_Fire";
	psaNameArr[136] = L"BK_A_Bighit_Skl_Downcounter_Exec";
	psaNameArr[137] = L"BK_A_Bighit_Skl_Downcounter_finish";
	psaNameArr[138] = L"BK_A_Bighit_Skl_Downcounter_ready";
	psaNameArr[139] = L"BK_A_Bighit_Skl_Downcounter_standby";
	psaNameArr[140] = L"Bk_A_Bighit_Skl_Dragon_Fly_Cast";
	psaNameArr[141] = L"Bk_A_Bighit_Skl_Dragon_Fly_Exec";
	psaNameArr[142] = L"BK_A_Bighit_Skl_DragonKick_Exec";
	psaNameArr[143] = L"BK_A_Bighit_Skl_DragonKick_Fire";
	psaNameArr[144] = L"BK_A_Bighit_Skl_Elbowattack";
	psaNameArr[145] = L"BK_A_Bighit_Skl_escape";
	psaNameArr[146] = L"Bk_A_Bighit_Skl_Firedash_Cast";
	psaNameArr[147] = L"Bk_A_Bighit_Skl_Firedash_Exec";
	psaNameArr[148] = L"Bk_A_Bighit_Skl_Firedash_Fire";
	psaNameArr[149] = L"BK_A_Bighit_Skl_Flytakedown";
	psaNameArr[150] = L"BK_A_Bighit_Skl_Heavyattack";
	psaNameArr[151] = L"BK_A_Bighit_Skl_heavypunch_Cast";
	psaNameArr[152] = L"BK_A_Bighit_Skl_heavypunch_Cast2";
	psaNameArr[153] = L"BK_A_Bighit_Skl_Heavypunch_Exec";
	psaNameArr[154] = L"BK_A_Bighit_Skl_Heavypunch_Swing";
	psaNameArr[155] = L"BK_A_Bighit_Skl_Heavypunch_Swing2";
	psaNameArr[156] = L"BK_A_bighit_Skl_IceBlast_Kick";
	psaNameArr[157] = L"BK_A_bighit_Skl_IceBlast_Kick_Exec";
	psaNameArr[158] = L"BK_A_bighit_Skl_IceBlast_Kick_Finish";
	psaNameArr[159] = L"Bk_A_Bighit_Skl_IceDragon_Fist_Cast";
	psaNameArr[160] = L"Bk_A_Bighit_Skl_IceDragon_Fist_Exec";
	psaNameArr[161] = L"Bk_A_Bighit_Skl_Inject_Energy_Cast";
	psaNameArr[162] = L"Bk_A_Bighit_Skl_Inject_Energy_Exec";
	psaNameArr[163] = L"BK_A_Bighit_Skl_Jumppunch_Exec";
	psaNameArr[164] = L"BK_A_Bighit_Skl_Jumppunch_Fire";
	psaNameArr[165] = L"BK_A_Bighit_Skl_Justicepunch";
	psaNameArr[166] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Cast";
	psaNameArr[167] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Exec1";
	psaNameArr[168] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Exec2";
	psaNameArr[169] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Exec3";
	psaNameArr[170] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Exec4";
	psaNameArr[171] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Exec5";
	psaNameArr[172] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Finish";
	psaNameArr[173] = L"BK_A_Bighit_Skl_KungfuFighter_MasterSkill_01_Swing";
	psaNameArr[174] = L"Bk_A_Bighit_Skl_Linked_escape";
	psaNameArr[175] = L"BK_A_Bighit_Skl_lowkick";
	psaNameArr[176] = L"BK_A_Bighit_Skl_Martialdance1_Exec";
	psaNameArr[177] = L"BK_A_Bighit_Skl_Martialdance1_Fire";
	psaNameArr[178] = L"BK_A_Bighit_Skl_Martialdance2_Exec";
	psaNameArr[179] = L"BK_A_Bighit_Skl_Martialdance2_Fire";
	psaNameArr[180] = L"BK_A_Bighit_Skl_Martialdance3_Exec";
	psaNameArr[181] = L"BK_A_Bighit_Skl_Martialdance3_Fire";
	psaNameArr[182] = L"BK_A_Bighit_Skl_Martialdance4_Exec";
	psaNameArr[183] = L"BK_A_Bighit_Skl_Martialdance4_Fire";
	psaNameArr[184] = L"BK_A_Bighit_Skl_Nudge";
	psaNameArr[185] = L"BK_A_Bighit_Skl_PowerAttack_Cast";
	psaNameArr[186] = L"BK_A_Bighit_Skl_PowerAttack_Exec";
	psaNameArr[187] = L"BK_A_Bighit_Skl_PowerAttack_Finish";
	psaNameArr[188] = L"BK_A_Bighit_Skl_PowerAttack_Mov_Finish";
	psaNameArr[189] = L"BK_A_Bighit_Skl_PowerfulAttack_Cast";
	psaNameArr[190] = L"BK_A_Bighit_Skl_PowerfulAttack_Exec";
	psaNameArr[191] = L"BK_A_Bighit_Skl_PowerfulAttack_Swing";
	psaNameArr[192] = L"Bk_A_Bighit_Skl_SidestepLeft_Exec";
	psaNameArr[193] = L"Bk_A_Bighit_Skl_SidestepLeft_Fire";
	psaNameArr[194] = L"Bk_A_Bighit_Skl_SidestepRight_Exec";
	psaNameArr[195] = L"Bk_A_Bighit_Skl_SidestepRight_Fire";
	psaNameArr[196] = L"BK_A_Bighit_Skl_skykick";
	psaNameArr[197] = L"BK_A_Bighit_Skl_skytakedown";
	psaNameArr[198] = L"BK_A_bighit_Skl_SpinHighKick";
	psaNameArr[199] = L"BK_A_bighit_Skl_SpinLowKick";
	psaNameArr[200] = L"BK_A_Bighit_Skl_strongAttack";
	psaNameArr[201] = L"BK_A_Bighit_Skl_strongpunch";
	psaNameArr[202] = L"BK_A_Bighit_Skl_trampledown";
	psaNameArr[203] = L"BK_A_Bighit_Skl_triplekick";
	psaNameArr[204] = L"BK_A_Bighit_Skl_uppercut";
	psaNameArr[205] = L"BK_A_Bighit_Skl_uppercut2";
	psaNameArr[206] = L"BK_A_Bighit_Skl_Violencepunch_Cast";
	psaNameArr[207] = L"BK_A_Bighit_Skl_Violencepunch_Exec";
	psaNameArr[208] = L"BK_A_Bighit_Skl_Windmill";
	psaNameArr[209] = L"BK_B_Bighit_Mov_idle";



	psaNumberArr[0] = 2;
	psaNumberArr[1] = 2;
	psaNumberArr[2] = 2;
	psaNumberArr[3] = 40;
	psaNumberArr[4] = 40;
	psaNumberArr[5] = 4;
	psaNumberArr[6] = 10;
	psaNumberArr[7] = 60;
	psaNumberArr[8] = 56;
	psaNumberArr[9] = 4;
	psaNumberArr[10] = 4;
	psaNumberArr[11] = 33;
	psaNumberArr[12] = 19;
	psaNumberArr[13] = 13;
	psaNumberArr[14] = 30;
	psaNumberArr[15] = 4;
	psaNumberArr[16] = 4;
	psaNumberArr[17] = 27;
	psaNumberArr[18] = 4;
	psaNumberArr[19] = 4;
	psaNumberArr[20] = 21;
	psaNumberArr[21] = 7;
	psaNumberArr[22] = 7;
	psaNumberArr[23] = 21;
	psaNumberArr[24] = 7;
	psaNumberArr[25] = 10;
	psaNumberArr[26] = 24;
	psaNumberArr[27] = 7;
	psaNumberArr[28] = 13;
	psaNumberArr[29] = 24;
	psaNumberArr[30] = 7;
	psaNumberArr[31] = 13;
	psaNumberArr[32] = 18;
	psaNumberArr[33] = 19;
	psaNumberArr[34] = 24;
	psaNumberArr[35] = 19;
	psaNumberArr[36] = 18;
	psaNumberArr[37] = 19;
	psaNumberArr[38] = 33;
	psaNumberArr[39] = 19;
	psaNumberArr[40] = 24;
	psaNumberArr[41] = 42;
	psaNumberArr[42] = 19;
	psaNumberArr[43] = 33;
	psaNumberArr[44] = 19;
	psaNumberArr[45] = 13;
	psaNumberArr[46] = 21;
	psaNumberArr[47] = 15;
	psaNumberArr[48] = 3;
	psaNumberArr[49] = 33;
	psaNumberArr[50] = 6;
	psaNumberArr[51] = 18;
	psaNumberArr[52] = 9;
	psaNumberArr[53] = 6;
	psaNumberArr[54] = 48;
	psaNumberArr[55] = 9;
	psaNumberArr[56] = 60;
	psaNumberArr[57] = 9;
	psaNumberArr[58] = 36;
	psaNumberArr[59] = 35;
	psaNumberArr[60] = 9;
	psaNumberArr[61] = 33;
	psaNumberArr[62] = 9;
	psaNumberArr[63] = 35;
	psaNumberArr[64] = 6;
	psaNumberArr[65] = 27;
	psaNumberArr[66] = 6;
	psaNumberArr[67] = 27;
	psaNumberArr[68] = 6;
	psaNumberArr[69] = 27;
	psaNumberArr[70] = 9;
	psaNumberArr[71] = 21;
	psaNumberArr[72] = 15;
	psaNumberArr[73] = 9;
	psaNumberArr[74] = 5;
	psaNumberArr[75] = 33;
	psaNumberArr[76] = 8;
	psaNumberArr[77] = 5;
	psaNumberArr[78] = 39;
	psaNumberArr[79] = 6;
	psaNumberArr[80] = 39;
	psaNumberArr[81] = 6;
	psaNumberArr[82] = 15;
	psaNumberArr[83] = 42;
	psaNumberArr[84] = 6;
	psaNumberArr[85] = 36;
	psaNumberArr[86] = 6;
	psaNumberArr[87] = 39;
	psaNumberArr[88] = 6;
	psaNumberArr[89] = 5;
	psaNumberArr[90] = 33;
	psaNumberArr[91] = 8;
	psaNumberArr[92] = 5;
	psaNumberArr[93] = 48;
	psaNumberArr[94] = 6;
	psaNumberArr[95] = 4;
	psaNumberArr[96] = 10;
	psaNumberArr[97] = 60;
	psaNumberArr[98] = 56;
	psaNumberArr[99] = 4;
	psaNumberArr[100] = 4;
	psaNumberArr[101] = 69;
	psaNumberArr[102] = 60;
	psaNumberArr[103] = 6;
	psaNumberArr[104] = 38;
	psaNumberArr[105] = 5;
	psaNumberArr[106] = 38;
	psaNumberArr[107] = 5;
	psaNumberArr[108] = 40;
	psaNumberArr[109] = 5;
	psaNumberArr[110] = 53;
	psaNumberArr[111] = 5;
	psaNumberArr[112] = 19;
	psaNumberArr[113] = 54;
	psaNumberArr[114] = 66;
	psaNumberArr[115] = 60;
	psaNumberArr[116] = 54;
	psaNumberArr[117] = 52;
	psaNumberArr[118] = 46;
	psaNumberArr[119] = 16;
	psaNumberArr[120] = 13;
	psaNumberArr[121] = 16;
	psaNumberArr[122] = 4;
	psaNumberArr[123] = 16;
	psaNumberArr[124] = 4;
	psaNumberArr[125] = 16;
	psaNumberArr[126] = 42;
	psaNumberArr[127] = 21;
	psaNumberArr[128] = 9;
	psaNumberArr[129] = 40;
	psaNumberArr[130] = 67;
	psaNumberArr[131] = 30;
	psaNumberArr[132] = 46;
	psaNumberArr[133] = 62;
	psaNumberArr[134] = 48;
	psaNumberArr[135] = 5;
	psaNumberArr[136] = 48;
	psaNumberArr[137] = 19;
	psaNumberArr[138] = 9;
	psaNumberArr[139] = 40;
	psaNumberArr[140] = 28;
	psaNumberArr[141] = 48;
	psaNumberArr[142] = 57;
	psaNumberArr[143] = 4;
	psaNumberArr[144] = 46;
	psaNumberArr[145] = 63;
	psaNumberArr[146] = 15;
	psaNumberArr[147] = 39;
	psaNumberArr[148] = 19;
	psaNumberArr[149] = 76;
	psaNumberArr[150] = 54;
	psaNumberArr[151] = 41;
	psaNumberArr[152] = 26;
	psaNumberArr[153] = 41;
	psaNumberArr[154] = 49;
	psaNumberArr[155] = 49;
	psaNumberArr[156] = 51;
	psaNumberArr[157] = 35;
	psaNumberArr[158] = 18;
	psaNumberArr[159] = 18;
	psaNumberArr[160] = 38;
	psaNumberArr[161] = 29;
	psaNumberArr[162] = 30;
	psaNumberArr[163] = 29;
	psaNumberArr[164] = 23;
	psaNumberArr[165] = 43;
	psaNumberArr[166] = 10;
	psaNumberArr[167] = 22;
	psaNumberArr[168] = 19;
	psaNumberArr[169] = 25;
	psaNumberArr[170] = 25;
	psaNumberArr[171] = 46;
	psaNumberArr[172] = 49;
	psaNumberArr[173] = 7;
	psaNumberArr[174] = 62;
	psaNumberArr[175] = 33;
	psaNumberArr[176] = 27;
	psaNumberArr[177] = 5;
	psaNumberArr[178] = 27;
	psaNumberArr[179] = 5;
	psaNumberArr[180] = 27;
	psaNumberArr[181] = 5;
	psaNumberArr[182] = 47;
	psaNumberArr[183] = 5;
	psaNumberArr[184] = 45;
	psaNumberArr[185] = 6;
	psaNumberArr[186] = 25;
	psaNumberArr[187] = 31;
	psaNumberArr[188] = 31;
	psaNumberArr[189] = 30;
	psaNumberArr[190] = 35;
	psaNumberArr[191] = 42;
	psaNumberArr[192] = 13;
	psaNumberArr[193] = 33;
	psaNumberArr[194] = 13;
	psaNumberArr[195] = 33;
	psaNumberArr[196] = 56;
	psaNumberArr[197] = 71;
	psaNumberArr[198] = 62;
	psaNumberArr[199] = 40;
	psaNumberArr[200] = 54;
	psaNumberArr[201] = 44;
	psaNumberArr[202] = 26;
	psaNumberArr[203] = 51;
	psaNumberArr[204] = 61;
	psaNumberArr[205] = 81;
	psaNumberArr[206] = 30;
	psaNumberArr[207] = 48;
	psaNumberArr[208] = 70;
	psaNumberArr[209] = 40;


	for (size_t i = 0; i < 210; ++i)
	{
		CAnimClip* pAnimClip = new CAnimClip();

		pAnimClip->m_strName = fileName;

		pAnimClip->m_strClipName = psaNameArr[i];
		pAnimClip->m_iStartFrame = 0;
		pAnimClip->m_iEndFrame = psaNumberArr[i] - 1;

		pAnimClip->m_iFrameLength = pAnimClip->m_iEndFrame;
		pAnimClip->m_eMode = mtAnimClip.eMode;
		////////////////////////////////////////////////////////////
		// 프레임 자르기
		vector<tFrameTrans> vecKeyFrame;

		int totalSize = pAnimClip->m_iFrameLength + 1;
		start = index;

		vecKeyFrame.resize(iBoneCount * totalSize);


		// 본 개수
		for (int k = 0; k < iBoneCount; ++k)
		{
			vector<tFrameTrans> temp = tempKeyFrame->at(k);

			// 프레임 개수
			for (index = start; index < start + totalSize; ++index)
			{
				if (0 >= (int)temp.size())
				{
					continue;
				}

				if (index >= iFrameCount)
				{
					break;
				}

				tFrameTrans data = tFrameTrans(
					Vec4(temp[index].vTranslate)
					, Vec4(temp[index].vScale)
					, Vec4(temp[index].qRot)
				);

				int in = iBoneCount * (index - start) + k;
				vecKeyFrame[in] = data;
			}
		}

		////////////////////////////////////////////////////////////
		pAnimClip->m_vecKeyFrame = vecKeyFrame;

		pAnimClip->m_iFrameSize = (UINT)_pMesh->GetBoneMatrix()->size() * pAnimClip->m_iFrameLength;

		// pAnimClip->m_pBoneKeyFrame = new CStructuredBuffer;
		// pAnimClip->m_pBoneKeyFrame->Create(sizeof(tFrameTrans), pAnimClip->m_iFrameSize, vecKeyFrame.data());

		pAnimClip->m_dStartTime = 0;
		pAnimClip->m_dEndTime = ((double)(totalSize - 1) / (double)30);
		pAnimClip->m_dTimeLength = pAnimClip->m_dEndTime;

		CAnim3DMgr::GetInst()->AddAnimClip(pAnimClip);
	}

	CAnim3DMgr::GetInst()->SaveAll();
}





//	bool HKExcelManager::LoadStage(const TCHAR * path, class HKTileMap* tileMap)
//	{
//		Book* book = xlCreateBook();
//
//		if (book->load(path))
//		{
//			Sheet* sheet = book->getSheet(0);
//
//			int totalY = sheet->lastRow();
//			int totalX = sheet->lastCol();
//
//			tileMap->SetInit(totalX, totalY);
//
//			if (nullptr != sheet)
//			{
//				for (int row = 0; row < totalY; ++row)
//				{
//					for (int col = 0; col < totalX; ++col)
//					{
//						CellType cellType = sheet->cellType(row, col);
//
//						int index = row * totalX + col;
//
//						switch (cellType)
//						{
//						case CELLTYPE_EMPTY:
//							break;
//						case CELLTYPE_NUMBER:
//						{
//							int num = sheet->readNum(row, col);
//
//							if (1 == num)
//							{
//								tileMap->CreateOneTile(col, row, 50.f, 50.f, index);
//							}
//							else
//							{
//								int a = 0;
//							}
//
//							break;
//						}
//						case CELLTYPE_STRING:
//						{
//							BOOM;
//							break;
//						}
//						case CELLTYPE_BOOLEAN:
//						{
//							BOOM;
//							break;
//						}
//						case CELLTYPE_BLANK:
//
//						case CELLTYPE_ERROR:
//						{
//							BOOM;
//							break;
//						}
//						}
//					}
//				}
//			}
//		}
//		else
//		{
//			BOOM
//		}
//
//		book->release();
//
//		return true;
//	}
//}