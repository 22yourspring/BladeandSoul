#include "stdafx.h"
#include "Mesh.h"

#include "Device.h"
#include "FBXLoader.h"

#include "ResMgr.h"
#include "PathMgr.h"

#include "InstancingBuffer.h"

#include "Anim3DMgr.h"

CMesh::CMesh()
	: CResource(RES_TYPE::MESH)
	, m_pBoneFrameData(nullptr)
	, m_pBoneOffset(nullptr)
	, m_iFrameCount(0)
	, m_pLayout(nullptr)
{
}

CMesh::~CMesh()
{
	// SAFE_RELEASE(m_pMaterial);

	for (int i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		for (int j = 0; j < m_vecMeshContainer[i]->vecIB.size(); ++j)
		{
			SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->vecIB[j].pData);
			SAFE_RELEASE(m_vecMeshContainer[i]->vecIB[j].pBuffer);
		}

		SAFE_DELETE_ARRAY(m_vecMeshContainer[i]->tVB.pData);
		SAFE_RELEASE(m_vecMeshContainer[i]->tVB.pBuffer);
		SAFE_DELETE(m_vecMeshContainer[i]);
	}

	m_vecMeshContainer.clear();


	SAFE_DELETE(m_pBoneFrameData);
	SAFE_DELETE(m_pBoneOffset);

	m_vecFrameTrans.clear();
}


bool CMesh::AddVertexBuffer(const void * pVertices, int iVtxSize, int iVtxCount, D3D11_USAGE eVtxUsage, D3D11_PRIMITIVE_TOPOLOGY ePrimitive, tMeshContainer * pContainer)
{
	pContainer->ePrimitive = ePrimitive;

	pContainer->tVB.iSize = iVtxSize;
	pContainer->tVB.iCount = iVtxCount;
	pContainer->tVB.eUsage = eVtxUsage;

	pContainer->tVB.pData = new char[iVtxSize * iVtxCount];
	memcpy(pContainer->tVB.pData, pVertices, iVtxSize * iVtxCount);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iVtxSize * iVtxCount;
	tDesc.Usage = eVtxUsage;

	if (eVtxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (eVtxUsage == D3D11_USAGE_STAGING)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};

	tData.pSysMem = pVertices;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVB.pBuffer)))
		return false;

	// 정점정보를 조사하여 위치를 얻어오고 얻어온 위치를 비교하여 최소값과 최대값을 얻어온다.
	for (int i = 0; i < iVtxCount; ++i)
	{
		unsigned char*	pVtx = ((unsigned char*)pVertices) + (i * iVtxSize);

		Vec3	vPos;
		memcpy(&vPos, pVtx, sizeof(Vec3));

		if (vPos.x < m_vMin.x)
			m_vMin.x = vPos.x;

		else if (vPos.x > m_vMax.x)
			m_vMax.x = vPos.x;

		if (vPos.y < m_vMin.y)
			m_vMin.y = vPos.y;

		else if (vPos.y > m_vMax.y)
			m_vMax.y = vPos.y;

		if (vPos.z < m_vMin.z)
			m_vMin.z = vPos.z;

		else if (vPos.z > m_vMax.z)
			m_vMax.z = vPos.z;
	}

	return true;
}

bool CMesh::AddIndexBuffer(const void * pIndices, int iIdxSize, int iIdxCount, D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, tMeshContainer * pContainer)
{
	tIndexBuffer	tIB = {};

	tIB.eFormat = eFmt;
	tIB.eUsage = eIdxUsage;
	tIB.iSize = iIdxSize;
	tIB.iCount = iIdxCount;

	tIB.pData = new char[iIdxSize * iIdxCount];
	memcpy(tIB.pData, pIndices, iIdxSize * iIdxCount);

	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = iIdxSize * iIdxCount;
	tDesc.Usage = eIdxUsage;

	if (eIdxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (eIdxUsage == D3D11_USAGE_STAGING)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};

	tData.pSysMem = pIndices;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &tIB.pBuffer)))
	{
		assert(false);
		return false;
	}
	pContainer->vecIB.push_back(tIB);

}

void CMesh::Create(void * _pVtxMem, UINT _iVtxSize, UINT _iVtxCount, D3D11_USAGE _eVtxUsage
	, void * _pIdxMem, DXGI_FORMAT _eIdxFormat, UINT _iIdxCount, D3D11_USAGE _IdxUsage)
{
	tMeshContainer*	pContainer = new tMeshContainer;

	tIndexBuffer	tIB = {};

	pContainer->tVB.iSize = _iVtxSize;
	pContainer->tVB.iCount = _iVtxCount;

	pContainer->tVB.eUsage = _eVtxUsage;// m_eVtxUsage = _eVtxUsage;

	tIB.eFormat = _eIdxFormat;
	tIB.eUsage = _IdxUsage;
	tIB.iSize = GetSizeofFormat(tIB.eFormat);
	tIB.iCount = _iIdxCount;

	// Vertex system
	pContainer->tVB.pData = new char[_iVtxSize * _iVtxCount];
	memcpy(pContainer->tVB.pData, _pVtxMem, _iVtxSize * _iVtxCount);

	// Index system
	tIB.pData = new char[tIB.iSize * _iIdxCount];
	memcpy(tIB.pData, _pIdxMem, tIB.iSize * _iIdxCount);

	// 버텍스 정점 버퍼 만들기
	D3D11_BUFFER_DESC	tDesc = {};

	tDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tDesc.Usage = _eVtxUsage;

	if (_eVtxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (_eVtxUsage == D3D11_USAGE_STAGING)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA	tData = {};

	tData.pSysMem = _pVtxMem;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &pContainer->tVB.pBuffer)))
	{
		assert(false);
		return;
	}

	// 정점정보를 조사하여 위치를 얻어오고 얻어온 위치를 비교하여 최소값과 최대값을 얻어온다.
	for (int i = 0; i < _iVtxCount; ++i)
	{
		unsigned char*	pVtx = ((unsigned char*)_pVtxMem) + (i * _iVtxSize);

		Vec3	vPos;
		memcpy(&vPos, pVtx, sizeof(Vec3));

		if (vPos.x < m_vMin.x)
			m_vMin.x = vPos.x;

		else if (vPos.x > m_vMax.x)
			m_vMax.x = vPos.x;

		if (vPos.y < m_vMin.y)
			m_vMin.y = vPos.y;

		else if (vPos.y > m_vMax.y)
			m_vMax.y = vPos.y;

		if (vPos.z < m_vMin.z)
			m_vMin.z = vPos.z;

		else if (vPos.z > m_vMax.z)
			m_vMax.z = vPos.z;
	}

	// return;

	// 인덱스 정점 버퍼 만들기
	tDesc = {};

	tDesc.ByteWidth = tIB.iSize * _iIdxCount;
	tDesc.Usage = _IdxUsage;

	if (_IdxUsage == D3D11_USAGE_DYNAMIC)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	else if (_IdxUsage == D3D11_USAGE_STAGING)
		tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;

	tDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	tData = {};

	tData.pSysMem = _pIdxMem;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, &tData, &tIB.pBuffer)))
	{
		assert(false);
		return;
	}

	pContainer->vecIB.push_back(tIB);

	m_vecMeshContainer.push_back(pContainer);

	// 정점 버퍼 만들기2
	/*D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	tBufferDesc.Usage = _eVtxUsage;
	if(tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	tBufferDesc.ByteWidth = _iVtxSize * _iVtxCount;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _pVtxMem;

	DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pVB);*/

	//// 인덱스 버퍼 만들기
	//tBufferDesc = {};
	//tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//tBufferDesc.Usage = _IdxUsage;

	//if (tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
	//	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	//tBufferDesc.ByteWidth = GetSizeofFormat(_eIdxFormat) * _iIdxCount;

	//tSub = {};
	//tSub.pSysMem = _pIdxMem;

	//DEVICE->CreateBuffer(&tBufferDesc, &tSub, &info.pIB);

	//m_vecIdxInfo.push_back(info);

	return;
}

CMesh * CMesh::CreateFromContainer(CFBXLoader & _loader)
{
	int iContainerSize = _loader.GetContainerCount();

	CMesh* pMesh = new CMesh;
	pMesh->m_vecMeshContainer.resize(iContainerSize);


	for (int k = 0; k < iContainerSize; ++k)
	{
		const tContainer* container = &_loader.GetContainer(k);

		UINT iVtxCount = (UINT)container->vecPos.size();

		D3D11_BUFFER_DESC tBufferDesc = {};

		tBufferDesc.ByteWidth = sizeof(VTX) * iVtxCount;
		tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		if (D3D11_USAGE_DYNAMIC == tBufferDesc.Usage)
			tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = malloc(tBufferDesc.ByteWidth);
		VTX* pSys = (VTX*)tSub.pSysMem;
		for (UINT i = 0; i < iVtxCount; ++i)
		{
			pSys[i].vPos = container->vecPos[i];
			pSys[i].vUV = container->vecUV[i];
			pSys[i].vColor = Vec4(1.f, 0.f, 1.f, 1.f);
			pSys[i].vNormal = container->vecNormal[i];
			pSys[i].vTangent = container->vecTangent[i];
			pSys[i].vBinormal = container->vecBinormal[i];
			pSys[i].vWeights = container->vecWeights[i];
			pSys[i].vIndices = container->vecIndices[i];
		}

		ID3D11Buffer* pVB = NULL;
		if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, &pVB)))
		{
			return NULL;
		}

		//////////////////////////// 서브셋 세팅
		tMeshContainer*	pContainer = new tMeshContainer;
		pMesh->m_vecMeshContainer[k] = pContainer;

		// 06/17   - 메쉬 컨테이너 이름
		pContainer->strName = container->strName;

		// 버텍스 버퍼 세팅
		pContainer->tVB.pBuffer = pVB;
		pContainer->tVB.iCount = iVtxCount;
		pContainer->tVB.iSize = sizeof(VTX);
		pContainer->tVB.eUsage = tBufferDesc.Usage;
		pContainer->tVB.pData = pSys;

		// 인덱스 정보
		UINT iIdxBufferCount = (UINT)container->vecIdx.size();
		pContainer->vecIB.resize(iIdxBufferCount);

		for (UINT i = 0; i < iIdxBufferCount; ++i)
		{
			// tIndexInfo info;
			pContainer->vecIB[i].iCount = (UINT)container->vecIdx[i].size();
			pContainer->vecIB[i].eFormat = DXGI_FORMAT_R32_UINT;

			memset(&tBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));
			tBufferDesc.ByteWidth = (UINT)container->vecIdx[i].size() * (UINT)GetSizeofFormat(DXGI_FORMAT_R32_UINT);

			tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			if (D3D11_USAGE_DYNAMIC == tBufferDesc.Usage)
				tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

			tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			pContainer->vecIB[i].pData = malloc(tBufferDesc.ByteWidth);
			memcpy(pContainer->vecIB[i].pData, &container->vecIdx[i][0], tBufferDesc.ByteWidth);
			tSub.pSysMem = pContainer->vecIB[i].pData;

			if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, &(pContainer->vecIB[i].pBuffer))))
			{
				return NULL;
			}

			pContainer->vecIB[i].eUsage = tBufferDesc.Usage;
		}

		/*wstring strKey = _loader.GetContainer(0).strName;
		Save(strKey);
*/

// Animation3D
		if (!container->bAnimation)
			continue;

		vector<tBone*>& vecBone = _loader.GetBones();
		UINT iFrameCount = 0;
		for (UINT i = 0; i < vecBone.size(); ++i)
		{
			tMTBone bone = {};
			bone.iDepth = vecBone[i]->iDepth;
			bone.iParentIndx = vecBone[i]->iParentIndx;
			bone.matBone = GetMatrix(vecBone[i]->matBone);
			bone.matOffset = GetMatrix(vecBone[i]->matOffset);
			bone.strBoneName = vecBone[i]->strBoneName;

			for (UINT j = 0; j < vecBone[i]->vecKeyFrame.size(); ++j)
			{
				tMTKeyFrame tKeyframe = {};
				tKeyframe.dTime = vecBone[i]->vecKeyFrame[j].dTime;
				tKeyframe.iFrame = j;
				tKeyframe.vTranslate.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[0];
				tKeyframe.vTranslate.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[1];
				tKeyframe.vTranslate.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetT().mData[2];

				tKeyframe.vScale.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[0];
				tKeyframe.vScale.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[1];
				tKeyframe.vScale.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetS().mData[2];

				tKeyframe.qRot.x = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[0];
				tKeyframe.qRot.y = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[1];
				tKeyframe.qRot.z = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[2];
				tKeyframe.qRot.w = (float)vecBone[i]->vecKeyFrame[j].matTransform.GetQ().mData[3];

				bone.vecKeyFrame.push_back(tKeyframe);
			}

			iFrameCount = max(iFrameCount, bone.vecKeyFrame.size());

			pMesh->m_vecBones.push_back(bone);
		}

		vector<tAnimClip*>& vecAnimClip = _loader.GetAnimClip();

		for (UINT i = 0; i < vecAnimClip.size(); ++i)
		{
			tMTAnimClip tClip = {};



			tClip.strAnimName = vecAnimClip[i]->strName;
			tClip.dStartTime = vecAnimClip[i]->tStartTime.GetSecondDouble();
			tClip.dEndTime = vecAnimClip[i]->tEndTime.GetSecondDouble();
			tClip.dTimeLength = (tClip.dEndTime - tClip.dStartTime);

			tClip.iStartFrame = (int)vecAnimClip[i]->tStartTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iEndFrame = (int)vecAnimClip[i]->tEndTime.GetFrameCount(vecAnimClip[i]->eMode);
			tClip.iFrameLength = tClip.iEndFrame - tClip.iStartFrame;
			tClip.eMode = vecAnimClip[i]->eMode;

			pMesh->m_vecAnimClip.push_back(tClip);


		}

		// 06/10   - Anim Clip 자르기


		// Animation 이 있는 Mesh 경우 BoneTexture 만들어두기
		if (pMesh->IsAnimMesh())
		{
			// BoneOffet 행렬
			vector<vector<tFrameTrans>> vecFrameTrans;
			vector<tFrameTrans> vecFrameTransB;
			vecFrameTransB.resize((UINT)pMesh->m_vecBones.size() * iFrameCount);
			vecFrameTrans.resize((UINT)pMesh->m_vecBones.size());

			int size = (int)pMesh->m_vecBones.size();

			for (int i = 0; i < size; ++i)
			{
				pMesh->m_vecBoneMatrix.push_back(pMesh->m_vecBones[i].matOffset * pMesh->m_vecBones[i].matBone);
				int keyFrameSize = pMesh->m_vecBones[i].vecKeyFrame.size();
				vecFrameTrans[i].resize(keyFrameSize);
				for (int j = 0; j < keyFrameSize; ++j)
				{
					// UINT in = (UINT)m_vecBones.size() * j + i;

					vecFrameTrans[i][j]
						= tFrameTrans(
							Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate)
							, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale)
							, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].qRot)
						);

					/*vecFrameTransB[(UINT)pMesh->m_vecBones.size() * j + i]
						= tFrameTrans(
							Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vTranslate)
							, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].vScale)
							, Vec4(pMesh->m_vecBones[i].vecKeyFrame[j].qRot));*/
				}
			}
			pMesh->m_pBoneOffset = new CStructuredBuffer;
			pMesh->m_pBoneOffset->Create(sizeof(Matrix), (UINT)pMesh->m_vecBoneMatrix.size(), pMesh->m_vecBoneMatrix.data());

			/*pMesh->m_pBoneFrameData = new CStructuredBuffer;
			pMesh->m_pBoneFrameData->Create(sizeof(tFrameTrans)
				, (UINT)pMesh->m_vecBoneMatrix.size() * (UINT)iFrameCount
				, vecFrameTransB.data());
*/
			pMesh->SetKeyFrame(vecFrameTrans);
		}
	}






	//CMesh* pMesh = new CMesh;
	//pMesh->m_pVB = pVB;
	//pMesh->m_iVtxCount = iVtxCount;
	//pMesh->m_iVtxSize = sizeof(VTX);
	//pMesh->m_eVtxUsage = tBufferDesc.Usage;
	//pMesh->m_pVtxSysMem = pSys;





	return pMesh;
}

void CMesh::render(UINT _iSubset, UINT _iContainer)
{
	tMeshContainer* pContainer = m_vecMeshContainer[_iContainer];

	ID3D11Buffer*			 pVB = pContainer->tVB.pBuffer;

	UINT iStride = pContainer->tVB.iSize;
	UINT iOffset = 0;

	CONTEXT->IASetVertexBuffers(0, 1, &pVB, &iStride, &iOffset);


	// 서브셋 수만큼 반복하며 그려낸다.
	/*int	iSubset = pContainer->vecIB.size();

	if (iSubset > 0)
	{
		for (int j = 0; j < iSubset; ++j)
		{
			tIndexBuffer*	pIdxBuffer = &pContainer->vecIB[j];

			CONTEXT->IASetIndexBuffer(pIdxBuffer->pBuffer,
				pIdxBuffer->eFormat, 0);
			CONTEXT->DrawIndexed(pIdxBuffer->iCount, 0, 0);
		}
	}

	else
	{
		CONTEXT->Draw(pContainer->tVB.iCount, 0);
	}*/

	tIndexBuffer*	pIdxBuffer = &pContainer->vecIB[_iSubset];

	CONTEXT->IASetIndexBuffer(pIdxBuffer->pBuffer,
		pIdxBuffer->eFormat, 0);
	CONTEXT->DrawIndexed(pIdxBuffer->iCount, 0, 0);
}

void CMesh::render_particle(UINT _iInstCount, UINT _iSubset)
{
	UINT ContainerSize = m_vecMeshContainer.size();
	tMeshContainer* pContainer;

	UINT iStride;
	UINT iOffset = 0;

	for (size_t i = 0; i < ContainerSize; ++i)
	{
		pContainer = m_vecMeshContainer[i];
		iStride = pContainer->tVB.iSize;
		ID3D11Buffer* pVB = pContainer->tVB.pBuffer;

		CONTEXT->IASetVertexBuffers(0, 1, &pVB, &iStride, &iOffset);
		
		UINT SubSetSize = pContainer->vecIB.size();
		
		CONTEXT->IASetIndexBuffer(pContainer->vecIB[_iSubset].pBuffer, pContainer->vecIB[_iSubset].eFormat, 0);
		CONTEXT->DrawIndexedInstanced(pContainer->vecIB[_iSubset].iCount, _iInstCount, 0, 0, 0);
	}



	//tMeshContainer* pContainer = m_vecMeshContainer[_iSubset];

	//ID3D11Buffer*			 pVB = pContainer->tVB.pBuffer;

	//UINT iStride = pContainer->tVB.iSize;
	//UINT iOffset = 0;

	//CONTEXT->IASetVertexBuffers(0, 1, &pVB, &iStride, &iOffset);


	//// 서브셋 수만큼 반복하며 그려낸다.
	//int	iSubset = pContainer->vecIB.size();

	//if (iSubset > 0)
	//{
	//	for (int j = 0; j < iSubset; ++j)
	//	{
	//		tIndexBuffer*	pIdxBuffer = &pContainer->vecIB[j];

	//		CONTEXT->IASetIndexBuffer(pIdxBuffer->pBuffer,
	//			pIdxBuffer->eFormat, 0);
	//		CONTEXT->DrawIndexed(pIdxBuffer->iCount, 0, 0);
	//	}
	//}

	//else
	//{
	//	CONTEXT->Draw(pContainer->tVB.iCount, 0);
	//}
}

void CMesh::render_instancing(UINT _iSubset, UINT _iContainer)
{
	/*if (_iSubset >= m_vecIdxInfo.size())
		assert(nullptr);*/

	tMeshContainer* pContainer = m_vecMeshContainer[_iContainer];

	ID3D11Buffer*			 pVB = pContainer->tVB.pBuffer;


	ID3D11Buffer* arrBuffer[2] = { pVB	, CInstancingBuffer::GetInst()->GetBuffer() };
	UINT		  iStride[2] = { pContainer->tVB.iSize	, sizeof(tInstancingData) };
	UINT		  iOffset[2] = { 0, 0 };

	// 그려낼 버텍스 버퍼를 지정한다.
	CONTEXT->IASetVertexBuffers(0, 2, arrBuffer, iStride, iOffset);

	// 서브셋 수만큼 반복하며 그려낸다.
	if (!pContainer->vecIB.empty())
	{
		if (_iSubset < pContainer->vecIB.size())
		{
			tIndexBuffer*	pIdxBuffer = &pContainer->vecIB[_iSubset];

			CONTEXT->IASetIndexBuffer(pIdxBuffer->pBuffer,
				pIdxBuffer->eFormat, 0);
			CONTEXT->DrawIndexedInstanced(pIdxBuffer->iCount
				, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);
		}
	}

	else
	{
		CONTEXT->DrawInstanced(pContainer->tVB.iCount, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0);
	}




	/*CONTEXT->IASetIndexBuffer(m_vecIdxInfo[_iSubset].pIB, m_vecIdxInfo[_iSubset].eFormat, 0);
	CONTEXT->DrawIndexedInstanced(m_vecIdxInfo[_iSubset].iIdxCount
		, CInstancingBuffer::GetInst()->GetInstanceCount(), 0, 0, 0);*/
}

void CMesh::SetKeyFrame(vector<vector<tFrameTrans>>& vecFrameTrans)
{/*
	size_t size = vecFrameTrans.size();
	size_t subSize = */

	m_vecFrameTrans = vecFrameTrans;
}

void CMesh::AddContainer(vector<tMeshContainer*> _OtherContainer, CMesh &_Mesh)
{
	for (auto pair : _OtherContainer)
	{
		if (pair->strName == L"")
		{
			pair->strName = _Mesh.GetName();
		}
		m_vecMeshContainer.push_back(pair);	//0708
	}
}


void CMesh::Load(const wstring & _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// 키값
	SetName(LoadWString(pFile));

	// 상대경로
	SetPath(LoadWString(pFile));

	// 로드 (신)
	int	iContainer = 0;

	fread(&iContainer, sizeof(int), 1, pFile);

	for (int i = 0; i < iContainer; ++i)
	{
		tMeshContainer*	pContainer = new tMeshContainer;
		m_vecMeshContainer.push_back(pContainer);

		int	iVtxSize = 0;
		int	iVtxCount = 0;
		D3D11_USAGE	eUsage;

		// 06/17   - 메쉬 컨테이너 이름
		pContainer->strName = LoadWString(pFile);

		fread(&pContainer->ePrimitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY), 1, pFile);
		fread(&iVtxSize, sizeof(int), 1, pFile);
		fread(&iVtxCount, sizeof(int), 1, pFile);
		fread(&eUsage, sizeof(D3D11_USAGE), 1, pFile);
		char*	pData = new char[iVtxSize * iVtxCount];
		fread(pData, iVtxSize, iVtxCount, pFile);

		/*FILE*	pFileUV = nullptr;

		fopen_s(&pFileUV, "UV.txt", "wt");

		if (pFileUV)
		{
			PVertexBumpAnim3D	pVtx = (PVertexBumpAnim3D)pData;

			for (int i = 0; i < iVtxCount; ++i)
			{
				char	strLine[256] = {};

				sprintf_s(strLine, "x : %.5f, y : %.5f\n", pVtx->vUV.x, pVtx->vUV.y);

				fputs(strLine, pFileUV);

				++pVtx;
			}

			fclose(pFileUV);
		}*/

		AddVertexBuffer(pData, iVtxSize, iVtxCount, eUsage,
			pContainer->ePrimitive, pContainer);

		SAFE_DELETE_ARRAY(pData);

		int	iIdxCount = 0;

		fread(&iIdxCount, sizeof(int), 1, pFile);

		for (int j = 0; j < iIdxCount; ++j)
		{
			DXGI_FORMAT	eFmt;
			int	iIdxSize = 0;
			int	iIdxCount = 0;
			fread(&eFmt, sizeof(DXGI_FORMAT), 1, pFile);
			fread(&iIdxSize, sizeof(int), 1, pFile);
			fread(&iIdxCount, sizeof(int), 1, pFile);
			fread(&eUsage, sizeof(D3D11_USAGE), 1, pFile);

			pData = new char[iIdxSize * iIdxCount];

			fread(pData, iIdxSize, iIdxCount, pFile);

			AddIndexBuffer(pData, iIdxSize, iIdxCount, eUsage, eFmt, pContainer);

			SAFE_DELETE_ARRAY(pData);
		}
	}


	// Animation3D 정보 읽기

	int iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		tMTAnimClip tClip = {};

		tClip.strAnimName = LoadWString(pFile);
		fread(&tClip.dStartTime, sizeof(double), 1, pFile);
		fread(&tClip.dEndTime, sizeof(double), 1, pFile);
		fread(&tClip.dTimeLength, sizeof(double), 1, pFile);
		fread(&tClip.eMode, sizeof(int), 1, pFile);
		fread(&tClip.fUpdateTime, sizeof(float), 1, pFile);
		fread(&tClip.iStartFrame, sizeof(int), 1, pFile);
		fread(&tClip.iEndFrame, sizeof(int), 1, pFile);
		fread(&tClip.iFrameLength, sizeof(int), 1, pFile);

		m_vecAnimClip.push_back(tClip);
	}

	iCount = 0;
	fread(&iCount, sizeof(int), 1, pFile);
	m_vecBones.resize(iCount);

	UINT _iFrameCount = 0;
	for (int i = 0; i < iCount; ++i)
	{
		m_vecBones[i].strBoneName = LoadWString(pFile);
		fread(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fread(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fread(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		UINT iFrameCount = 0;
		fread(&iFrameCount, sizeof(int), 1, pFile);
		m_vecBones[i].vecKeyFrame.resize(iFrameCount);
		_iFrameCount = max(_iFrameCount, iFrameCount);
		for (int j = 0; j < iFrameCount; ++j)
		{
			fread(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}
	m_iFrameCount = _iFrameCount / iContainer;
	// _iFrameCount /= iContainer;

	//// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	//if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	//{
	//	wstring strBone = GetName();

	//	// BoneOffet 행렬
	//	// vector<Matrix> vecOffset;
	//	vector<tFrameTrans> vecFrameTrans;
	//	vecFrameTrans.resize((UINT)m_vecBones.size() * _iFrameCount);

	//	// TEST로, bone 1개만 keyFrame 추출
	//	for (int i = 0; i < m_vecBones.size(); ++i)
	//	{
	//		m_vecBoneMatrix.push_back(m_vecBones[i].matOffset * m_vecBones[i].matBone);
	//		int keyFrameSize = m_vecBones[i].vecKeyFrame.size();
	//		for (int j = 0; j < keyFrameSize; ++j)
	//		{
	//			UINT in = (UINT)m_vecBones.size() * j + i;

	//			vecFrameTrans[(UINT)m_vecBones.size() * j + i]
	//				= tFrameTrans( 
	//				  Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate)
	//				, Vec4(m_vecBones[i].vecKeyFrame[j].vScale)
	//				, Vec4(m_vecBones[i].vecKeyFrame[j].qRot)
	//				);
	//		}			
	//	}
	//	m_pBoneOffset = new CStructuredBuffer;
	//	m_pBoneOffset->Create(sizeof(Matrix), (UINT)m_vecBoneMatrix.size(), m_vecBoneMatrix.data());

	//	/*m_pBoneFrameData = new CStructuredBuffer;
	//	m_pBoneFrameData->Create(sizeof(tFrameTrans)
	//		, (UINT)m_vecBoneMatrix.size() * (UINT)_iFrameCount
	//		, vecFrameTrans.data());*/
	//	SetKeyFrame(vecFrameTrans);
	//}



	// Animation 이 있는 Mesh 경우 Bone StructuredBuffer 만들기
	if (m_vecAnimClip.size() > 0 && m_vecBones.size() > 0)
	{
		wstring strBone = GetName();

		// BoneOffet 행렬
		// vector<Matrix> vecOffset;
		vector<vector<tFrameTrans>> vecFrameTrans;
		vector<tFrameTrans> vecFrameTransB;
		vecFrameTrans.resize((UINT)m_vecBones.size());

		for (int i = 0; i < m_vecBones.size(); ++i)
		{
			m_vecBoneMatrix.push_back(m_vecBones[i].matOffset * m_vecBones[i].matBone);
			int keyFrameSize = m_vecBones[i].vecKeyFrame.size();
			vecFrameTrans[i].resize(keyFrameSize);
			vecFrameTransB.resize((UINT)m_vecBones.size() * _iFrameCount);
			for (int j = 0; j < keyFrameSize; ++j)
			{
				// UINT in = (UINT)m_vecBones.size() * j + i;

				vecFrameTrans[i][j]
					= tFrameTrans(
						Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate)
						, Vec4(m_vecBones[i].vecKeyFrame[j].vScale)
						, Vec4(m_vecBones[i].vecKeyFrame[j].qRot)
					);

				vecFrameTransB[(UINT)m_vecBones.size() * j + i]
					= tFrameTrans(
						Vec4(m_vecBones[i].vecKeyFrame[j].vTranslate)
						, Vec4(m_vecBones[i].vecKeyFrame[j].vScale)
						, Vec4(m_vecBones[i].vecKeyFrame[j].qRot));
			}
		}
		m_pBoneOffset = new CStructuredBuffer;
		m_pBoneOffset->Create(sizeof(Matrix), (UINT)m_vecBoneMatrix.size(), m_vecBoneMatrix.data());

		m_pBoneFrameData = new CStructuredBuffer;
		m_pBoneFrameData->Create(sizeof(tFrameTrans)
			, (UINT)m_vecBoneMatrix.size() * (UINT)_iFrameCount
			, vecFrameTransB.data());

		SetKeyFrame(vecFrameTrans);
	}

	fclose(pFile);
}

void CMesh::Save(FILE * _pFile, const wstring & _strPath)
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += _strPath;
	SetPath(_strPath);

	FILE* pFile = _pFile;
	// errno_t err = _wfopen_s(&pFile, strFileName.c_str(), L"wb");

	assert(pFile);

	// 키값
	SaveWString(pFile, GetName().c_str());

	// 상대 경로
	SaveWString(pFile, GetPath().c_str());

	// 저장(신)
	int	iContainer = (int)m_vecMeshContainer.size();

	fwrite(&iContainer, sizeof(int), 1, pFile);

	for (int i = 0; i < iContainer; ++i)
	{
		tMeshContainer*	pContainer = m_vecMeshContainer[i];

		// 06/17   - 메쉬 컨테이너 이름
		SaveWString(pFile, pContainer->strName.c_str());

		fwrite(&pContainer->ePrimitive, sizeof(D3D11_PRIMITIVE_TOPOLOGY),
			1, pFile);

		fwrite(&pContainer->tVB.iSize, sizeof(int), 1, pFile);
		fwrite(&pContainer->tVB.iCount, sizeof(int), 1, pFile);
		fwrite(&pContainer->tVB.eUsage, sizeof(D3D11_USAGE), 1, pFile);
		fwrite(pContainer->tVB.pData, pContainer->tVB.iSize,
			pContainer->tVB.iCount, pFile);

		int	iIdxCount = (int)pContainer->vecIB.size();

		fwrite(&iIdxCount, sizeof(int), 1, pFile);

		for (int j = 0; j < iIdxCount; ++j)
		{
			fwrite(&pContainer->vecIB[j].eFormat, sizeof(DXGI_FORMAT),
				1, pFile);

			pContainer->vecIB[j].iSize = GetSizeofFormat(pContainer->vecIB[j].eFormat);

			fwrite(&pContainer->vecIB[j].iSize, sizeof(int), 1, pFile);
			fwrite(&pContainer->vecIB[j].iCount, sizeof(int), 1, pFile);
			fwrite(&pContainer->vecIB[j].eUsage, sizeof(D3D11_USAGE), 1, pFile);
			fwrite(pContainer->vecIB[j].pData, pContainer->vecIB[j].iSize,
				pContainer->vecIB[j].iCount, pFile);
		}
	}



	// Animation3D 정보 
	int iCount = (int)m_vecAnimClip.size();
	fwrite(&iCount, sizeof(int), 1, pFile);
	for (int i = 0; i < iCount; ++i)
	{
		SaveWString(pFile, m_vecAnimClip[i].strAnimName.c_str());
		fwrite(&m_vecAnimClip[i].dStartTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dEndTime, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].dTimeLength, sizeof(double), 1, pFile);
		fwrite(&m_vecAnimClip[i].eMode, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].fUpdateTime, sizeof(float), 1, pFile);
		fwrite(&m_vecAnimClip[i].iStartFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iEndFrame, sizeof(int), 1, pFile);
		fwrite(&m_vecAnimClip[i].iFrameLength, sizeof(int), 1, pFile);
	}

	iCount = (int)m_vecBones.size();
	fwrite(&iCount, sizeof(int), 1, pFile);

	for (int i = 0; i < iCount; ++i)
	{
		SaveWString(pFile, m_vecBones[i].strBoneName.c_str());
		fwrite(&m_vecBones[i].iDepth, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].iParentIndx, sizeof(int), 1, pFile);
		fwrite(&m_vecBones[i].matBone, sizeof(Matrix), 1, pFile);
		fwrite(&m_vecBones[i].matOffset, sizeof(Matrix), 1, pFile);

		int iFrameCount = (int)m_vecBones[i].vecKeyFrame.size();
		fwrite(&iFrameCount, sizeof(int), 1, pFile);

		for (int j = 0; j < m_vecBones[i].vecKeyFrame.size(); ++j)
		{
			fwrite(&m_vecBones[i].vecKeyFrame[j], sizeof(tMTKeyFrame), 1, pFile);
		}
	}

	// fclose(pFile);
}