#include "stdafx.h"
#include "BillboardScript.h"

#include <Engine/Core.h>
#include <Engine/Camera.h>


CBillboardScript::CBillboardScript()
	: CScript((UINT)SCRIPT_TYPE::BILLBOARDSCRIPT)
{

}

CBillboardScript::~CBillboardScript()
{
}

void CBillboardScript::awake()
{

}

void CBillboardScript::update()
{
	// Y축 BillBoard - 식생용

//	Vec3 modelPosition = Transform()->GetWorldPos();
//	Vec3 cameraPosition = pCamera->Transform()->GetWorldPos();
//	
//	// atan2 함수를 이용하여 빌보드 모델에 적용될 회전값을 계산합니다. 이렇게 하여 빌보드 모델이 현재 카메라 위치를 바라보게 합니다. 
//	double angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / 3.1415926535897932384626);
//	
//	// 회전각도를 라디안으로 변환합니다. 
//	float rotation = (float)angleY * 0.0174532925f;
//	
//	// 월드 행렬을 이용하여 원점에서의 빌보드의 회전값을 설정합니다. 
//	Vec3 Angle = Transform()->GetLocalRot();
//	Angle.y = rotationY + (180.f * _DEGREE);
//	
//	Transform()->SetLocalRot(Angle);



	// X, Y축 Bill - Board
	m_pMainCam = CRenderMgr::GetInst()->GetMainCam()->Object();

	Vec3 vCamDir = Transform()->GetWorldPos() - m_pMainCam->Transform()->GetWorldPos();
	vCamDir.Normalize();
	Transform()->LookAt(vCamDir);

	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Matrix matRot = XMMatrixRotationAxis(vUp, -XM_PI);

	Transform()->RotationMatrix(matRot);

}