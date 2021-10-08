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
	// Y�� BillBoard - �Ļ���

//	Vec3 modelPosition = Transform()->GetWorldPos();
//	Vec3 cameraPosition = pCamera->Transform()->GetWorldPos();
//	
//	// atan2 �Լ��� �̿��Ͽ� ������ �𵨿� ����� ȸ������ ����մϴ�. �̷��� �Ͽ� ������ ���� ���� ī�޶� ��ġ�� �ٶ󺸰� �մϴ�. 
//	double angle = atan2(modelPosition.x - cameraPosition.x, modelPosition.z - cameraPosition.z) * (180.0 / 3.1415926535897932384626);
//	
//	// ȸ�������� �������� ��ȯ�մϴ�. 
//	float rotation = (float)angleY * 0.0174532925f;
//	
//	// ���� ����� �̿��Ͽ� ���������� �������� ȸ������ �����մϴ�. 
//	Vec3 Angle = Transform()->GetLocalRot();
//	Angle.y = rotationY + (180.f * _DEGREE);
//	
//	Transform()->SetLocalRot(Angle);



	// X, Y�� Bill - Board
	m_pMainCam = CRenderMgr::GetInst()->GetMainCam()->Object();

	Vec3 vCamDir = Transform()->GetWorldPos() - m_pMainCam->Transform()->GetWorldPos();
	vCamDir.Normalize();
	Transform()->LookAt(vCamDir);

	Vec3 vUp = Transform()->GetLocalDir(DIR_TYPE::UP);
	Matrix matRot = XMMatrixRotationAxis(vUp, -XM_PI);

	Transform()->RotationMatrix(matRot);

}