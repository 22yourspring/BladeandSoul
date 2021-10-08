
#include "pch.h"
#include "framework.h"

#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "StudioYCS.h"

#include <Engine/GameObject.h>
#include <Engine/SceneMgr.h>
#include <Engine/Script.h>

#include <Engine/Component.h>
#include <Engine/privatefunc.h>

#include <Engine/Core.h>

#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/ResMgr.h>
#include <Engine/Camera.h>
#include <Engine/Animator2D.h>
#include <Engine/Animator3D.h>
#include <Engine/Animation2D.h>
#include <Engine/Light2D.h>
#include <Engine/Light3D.h>
#include <Engine/Terrain.h>
#include <Engine/ParticleSystem.h>
#include <Engine/Script.h>
#include <Script/ScriptMgr.h>

#include "PropGridSlider.h"

#include <Engine/KeyMgr.h>

#include <cmath>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CGameObject* CPropertiesWnd::StaticTarget = nullptr;
CGameObject* CPropertiesWnd::m_pPrevTarget = nullptr;

CPropertiesWnd::CPropertiesWnd() noexcept
	: m_pTarget(nullptr)
	, m_TerrainTarget(nullptr)
	, m_PropertyName(nullptr)
	, m_PropertyLayerTitle(nullptr)
	, m_PropertyLayer(nullptr)
	, m_PropertyTransform(nullptr)
	, m_PropertyTransform_Position(nullptr)
	, m_PropertyTransform_Position_X(nullptr)
	, m_PropertyTransform_Position_Y(nullptr)
	, m_PropertyTransform_Position_Z(nullptr)
	, m_PropertyTransform_Scale(nullptr)
	, m_PropertyTransform_Scale_X(nullptr)
	, m_PropertyTransform_Scale_Y(nullptr)
	, m_PropertyTransform_Scale_Z(nullptr)
	, m_PropertyTransform_Rotation(nullptr)
	, m_PropertyTransform_Rotation_X(nullptr)
	, m_PropertyTransform_Rotation_Y(nullptr)
	, m_PropertyTransform_Rotation_Z(nullptr)
	, m_PropertyMesh_Renderer(nullptr)
	, m_PropertyMesh_MeshTitle(nullptr)
	, m_PropertyMesh_MeshData(nullptr)
	, m_PropertyMesh_Mesh(nullptr)
	, m_PropertyMesh_MaterialTitle(nullptr)
	, m_PropertyMesh_MaterialData(nullptr)
	, m_PropertyMesh_Material(nullptr)
	, m_PropertyCollider2D(nullptr)
	, Offset2D_Pos(nullptr)
	, Offset2D_PosX(nullptr)
	, Offset2D_PosY(nullptr)
	, Offset2D_Scale(nullptr)
	, Offset2D_ScaleX(nullptr)
	, Offset2D_ScaleY(nullptr)
	, m_PropertyCollider3D(nullptr)
	, Offset3D_Pos(nullptr)
	, Offset3D_PosX(nullptr)
	, Offset3D_PosY(nullptr)
	, Offset3D_PosZ(nullptr)
	, Offset3D_Scale(nullptr)
	, Offset3D_ScaleX(nullptr)
	, Offset3D_ScaleY(nullptr)
	, Offset3D_ScaleZ(nullptr)
	, m_Animator2D(nullptr)
	, m_Animation2D(nullptr)
	, m_Animator3D(nullptr)
	, m_Animation3D(nullptr)
	, m_Animation3DCurAnim(nullptr)
	, m_LightScript(nullptr)
	, LightType3D(nullptr)
	, m_LightType(nullptr)
	, m_LightColor(nullptr)
	, m_LightRim(nullptr)
	, m_LightRange(nullptr)
	, m_LightInAngle(nullptr)
	, m_LightOutAngle(nullptr)
	, TransformDisable(nullptr)
	, MeshRendererDisable(nullptr)
	, CameraDisable(nullptr)
	, Offset2DDisable(nullptr)
	, Offset3DDisable(nullptr)
	, Anim2DDisable(nullptr)
	, Anim3DDisable(nullptr)
	, LightDisable(nullptr)
	, TerrainDisable(nullptr)
	, ParticleDisable(nullptr)
	, PrevstrMesh("NULL")
	, PrevstrMtrl("NULL")
	, ShaderClear(true)
	, m_Terrain(nullptr)
	, m_Terrain_Tessellation(nullptr)
	, m_Terrain_TessEnable(nullptr)
	, m_Terrain_TessMin(nullptr)
	, m_Terrain_TessMax(nullptr)
	, m_Terrain_TessPhase(nullptr)
	, m_Terrain_Brush(nullptr)
	, m_Terrain_BrushType(nullptr)
	, m_Terrain_BrushScale(nullptr)
	, m_Terrain_Landscape(nullptr)
	, m_Terrain_LandscapingState(nullptr)
	, m_Terrain_Splatting_Material(nullptr)
	, m_CurOption(TESS_OPTION::DISABLE)
	, m_PrevOption(TESS_OPTION::DISABLE)
	, TessEnable(false)
	, BrushEnable(false)
	, MaterialEnable(false)
	, LightOptionEnable(false)
	, LightColorEnable(false)
	, m_bCameraWeather(false)
	, m_bWeatherEnable(false)
	, m_bCameraRange(false)
	, m_bCameraRatio(false)
	, m_bCameraResolution(false)
	, m_ParticleSystem(nullptr)
	, m_ParticleFile(nullptr)
	, m_ParticleType(nullptr)
	, m_ParticleFrequncy(nullptr)
	, ParticleLifeTime(nullptr)
	, Particle_MinimumLifeTime(nullptr)
	, Particle_MaximumLifeTime(nullptr)
	, ParticleSpeed(nullptr)
	, Particle_MinSpeed(nullptr)
	, Particle_MaxSpeed(nullptr)
	, ParticleSacle(nullptr)
	, Particle_EndScale(nullptr)
	, Particle_StartScale(nullptr)
	, ParticleColor(nullptr)
	, Particle_StartColor(nullptr)
	, Particle_EndColor(nullptr)
	, m_bParticleTime_Enable(false)
	, m_bParticleLife_Enable(false)
	, m_bParticlePace_Enable(false)
	, m_bParticleScale_Enable(false)
	, m_bParticleColor_Enable(false)
	, m_Camera(nullptr)
	, CameraOption(nullptr)
	, WeatherOption(nullptr)
	, WeatherEnable(nullptr)
	, WeatherShaderTexture(nullptr)
	, CameraRange(nullptr)
	, CameraNear(nullptr)
	, CameraFar(nullptr)
	, CameraRatio(nullptr)
	, CameraFOV(nullptr)
	, CameraScale(nullptr)
	, CameraResolution(nullptr)
	, CameraWidth(nullptr)
	, CameraHeight(nullptr)
	, _MOD(CAMERA_MOD::END)
	, Prev_MOD(CAMERA_MOD::END)
	, Prev_Light(true)
	, _ParticleMtrl_Name()
	, _PrevParticleMtrl_Name()

{
	m_nComboHeight = 0;
}

void CPropertiesWnd::SetFocusGameView()
{
	((CWnd*)((CMainFrame*)AfxGetMainWnd())->GetGameView())->SetFocus();
	::SetFocus(((CWnd*)((CMainFrame*)AfxGetMainWnd())->GetGameView())->GetSafeHwnd());	// 이것 써도 됨
}

void CPropertiesWnd::update()
{
	if (m_pTarget)
	{
		StaticTarget = m_pTarget;

		if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod())	// Scene Play
		{
			// Terrain 선택 검출
			if (GetTarget()->Terrain())
			{
				GetTarget()->Terrain()->SetTargetSelectTerrain(true);
			}
			else if (m_pPrevTarget != nullptr && m_pPrevTarget->Terrain())
			{
				m_pPrevTarget->Terrain()->SetTargetSelectTerrain(false);
			}
		}
		else
		{
			if (GetTarget()->Terrain())
			{
				GetTarget()->Terrain()->SetTargetSelectTerrain(false);
			}
		}

		if (m_pTarget != m_pPrevTarget) // 공유 매테리얼 선택 픽킹된 오브젝트 바꾸는것 
		{
			// Terrain 선택 검출
			if (GetTarget()->Terrain())
			{
				GetTarget()->Terrain()->SetTargetSelectTerrain(true);
			}
			else if (m_pPrevTarget != nullptr && m_pPrevTarget->Terrain())
			{
				m_pPrevTarget->Terrain()->SetTargetSelectTerrain(false);
			}

			if (m_pPrevTarget != nullptr && m_pTarget->GetLayerName() != L"Decal" && m_pPrevTarget->GetLayerName() != L"Decal")
			{
				m_PropertyName->SetValue((_variant_t)(m_pTarget->GetName().c_str()));

				m_PropertyLayer->SetValue((_variant_t)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTarget->GetLayerIdx())->GetName().c_str()));

				if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod())
				{
					if (!m_pTarget->Camera() && !m_pTarget->Light3D() && !m_pTarget->Terrain())
					{
						if (m_pTarget->MeshRender())
						{
							int MaterialContainerSize = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

							for (size_t i = 0; i < MaterialContainerSize; ++i)
							{
								int MaterialSubSetSize = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

								for (size_t j = 0; j < MaterialSubSetSize; ++j)
								{
									(((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"WireFrameShader"));
								}
							}
						}
					}

					if (m_pPrevTarget != nullptr && !m_pPrevTarget->Camera() && !m_pPrevTarget->Light3D() && !m_pTarget->Camera() && !m_pPrevTarget->Terrain())
					{
						if (m_pPrevTarget->MeshRender())
						{
							int MaterialContainerSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

							for (size_t i = 0; i < MaterialContainerSize2; ++i)
							{
								int MaterialSubSetSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

								for (size_t j = 0; j < MaterialSubSetSize2; ++j)
								{
									if (m_pTarget->MeshRender())
									{
										if ((((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i)) != (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i)))
										{
											(((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
										}
									}
									else
									{
										(((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
									}
								}
							}
						}
					}
					else if (m_pPrevTarget != nullptr && (m_pTarget->Camera() || m_pTarget->Light3D()) && !m_pPrevTarget->Terrain())
					{
						if (m_pPrevTarget->MeshRender() && m_pPrevTarget->Light3D() == nullptr)
						{
							int MaterialContainerSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

							for (size_t i = 0; i < MaterialContainerSize2; ++i)
							{
								int MaterialSubSetSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

								for (size_t j = 0; j < MaterialSubSetSize2; ++j)
								{
									(((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
								}
							}
						}
					}

					ShaderClear = false;
				}
			}
		}

		if (m_TerrainTarget != nullptr && SCENE_MOD::SCENE_PLAY == CCore::GetInst()->GetSceneMod())
		{
			TessOptionDisable(m_TerrainTarget);
			m_TerrainTarget->Terrain()->SetTerrainOption(TERRAIN_OPTION::DISABLE);
		}
		if (SCENE_MOD::SCENE_PLAY == CCore::GetInst()->GetSceneMod() && !ShaderClear)
		{
			if (!m_pTarget->Camera() && !m_pTarget->Light3D() && !m_pTarget->Terrain())
			{
				int MaterialContainerSize = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

				for (size_t i = 0; i < MaterialContainerSize; ++i)
				{
					int MaterialSubSetSize = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

					for (size_t j = 0; j < MaterialSubSetSize; ++j)
					{
						(((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
					}
				}
			}

			if (m_pPrevTarget != nullptr && !m_pPrevTarget->Camera() && !m_pPrevTarget->Light3D() && !m_pTarget->Camera() && !m_pPrevTarget->Terrain())
			{
				int MaterialContainerSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

				for (size_t i = 0; i < MaterialContainerSize2; ++i)
				{
					int MaterialSubSetSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

					for (size_t j = 0; j < MaterialSubSetSize2; ++j)
					{
						if ((((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->GetShader() != (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->GetShader())
						{
							(((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
						}
					}
				}
			}

			ShaderClear = true;
		}


		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{
			if (nullptr == m_pTarget->GetComponent((COMPONENT_TYPE)i))
			{
				switch ((COMPONENT_TYPE)i)
				{
				case COMPONENT_TYPE::TRANSFORM:
					if (m_pPrevTarget != m_pTarget)
					{
						if (TransformDisable == nullptr)
						{
							if (m_PropertyTransform_Position != nullptr)
							{
								m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Position);
							}
							if (m_PropertyTransform_Scale != nullptr)
							{
								m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Scale);
							}
							if (m_PropertyTransform_Rotation != nullptr)
							{
								m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Rotation);
							}
							// Transform
							TransformDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							TransformDisable->AddOption(L"Enable");
							m_PropertyTransform->AddSubItem(TransformDisable);

							m_PropertyMesh_Renderer->Expand(FALSE);
							m_PropertyMesh_Renderer->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (TransformDisable && TransformDisable->IsModified())
						{
							m_pTarget->AddComponent(new CTransform());
						}
					}
					break;
				case COMPONENT_TYPE::MESHRENDER:

					if (m_pPrevTarget != m_pTarget)
					{
						if (MeshRendererDisable == nullptr)
						{
							if (m_PropertyMesh_Mesh != nullptr)
							{
								m_PropertyMesh_Renderer->RemoveSubItem(m_PropertyMesh_MeshTitle);
							}
							if (m_PropertyMesh_Material != nullptr)
							{
								m_PropertyMesh_Renderer->RemoveSubItem(m_PropertyMesh_MaterialTitle);
							}
							// MeshRender
							MeshRendererDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							MeshRendererDisable->AddOption(L"Enable");
							m_PropertyMesh_Renderer->AddSubItem(MeshRendererDisable);

							m_PropertyMesh_Renderer->Expand(FALSE);
							m_PropertyMesh_Renderer->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (MeshRendererDisable && MeshRendererDisable->IsModified())
						{
							m_pTarget->AddComponent(new CMeshRender());
						}
					}
					break;
				case COMPONENT_TYPE::CAMERA:
					if (m_pPrevTarget != m_pTarget)
					{
						if (CameraDisable == nullptr)
						{
							m_Camera->RemoveSubItem(CameraOption);
							if (WeatherOption != nullptr)
							{
								if (WeatherEnable->IsSelected() == false && WeatherShaderTexture->IsSelected() == false)
								{
									m_Camera->RemoveSubItem(WeatherOption);
								}
								else
									m_bCameraWeather = true;
							}

							if (CameraRange != nullptr)
							{								
								if (CameraNear->IsSelected() == false && CameraFar->IsSelected() == false)
								{
									m_Camera->RemoveSubItem(CameraRange);
								}
								else
									m_bCameraRange = true;
							}

							if (CameraRatio != nullptr)
							{
								if (CameraFOV->IsSelected() == false && CameraScale->IsSelected() == false)
								{
									m_Camera->RemoveSubItem(CameraRatio);
								}
								else
									m_bCameraRatio = true;
							}

							if (CameraResolution != nullptr)
							{
								if (CameraWidth->IsSelected() == false && CameraHeight->IsSelected() == false)
								{
									m_Camera->RemoveSubItem(CameraResolution);
								}
								else
									m_bCameraResolution = true;
							}

							// Camera
							CameraDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							CameraDisable->AddOption(L"Enable");
							m_Camera->AddSubItem(CameraDisable);

							m_Camera->Expand(FALSE);
							m_Camera->Expand(TRUE);

							if (m_bCameraRange || m_bCameraRatio || m_bCameraResolution)
							{
								m_Camera->Expand(FALSE);
							}
						}
					}
					break;
				case COMPONENT_TYPE::COLLIDER2D:
					if (m_pPrevTarget != m_pTarget)
					{
						if (Offset2DDisable == nullptr)
						{
							if (Offset2D_Pos != nullptr)
							{
								m_PropertyCollider2D->RemoveSubItem(Offset2D_Pos);
							}
							if (Offset2D_Scale != nullptr)
							{
								m_PropertyCollider2D->RemoveSubItem(Offset2D_Scale);
							}
							// Collider 2D
							Offset2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							Offset2DDisable->AddOption(L"Enable");
							m_PropertyCollider2D->AddSubItem(Offset2DDisable);

							m_PropertyCollider2D->Expand(FALSE);
							m_PropertyCollider2D->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (Offset2DDisable && Offset2DDisable->IsModified())
						{
							//		m_pTarget->AddComponent(new CCollider2D());
						}
					}
					break;
				case COMPONENT_TYPE::COLLIDER3D:
					if (m_pPrevTarget != m_pTarget)
					{
						if (Offset3DDisable == nullptr)
						{
							if (Offset3D_Pos != nullptr)
							{
								m_PropertyCollider3D->RemoveSubItem(Offset3D_Pos);
							}
							if (Offset3D_Scale != nullptr)
							{
								m_PropertyCollider3D->RemoveSubItem(Offset3D_Scale);
							}
							// Collider 3D
							Offset3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							Offset3DDisable->AddOption(L"Enable");
							m_PropertyCollider3D->AddSubItem(Offset3DDisable);

							m_PropertyCollider3D->Expand(FALSE);
							m_PropertyCollider3D->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (Offset3DDisable && Offset3DDisable->IsModified())
						{
							//	m_pTarget->AddComponent(new CCollider3D());
						}
					}
					break;
				case COMPONENT_TYPE::ANIMATOR2D:
					if (m_pPrevTarget != m_pTarget)
					{
						if (Anim2DDisable == nullptr)
						{
							if (m_Animation2D != nullptr)
							{
								m_Animator2D->RemoveSubItem(m_Animation2D);
							}

							// Animation 2D
							Anim2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							Anim2DDisable->AddOption(L"Enable");
							m_Animator2D->AddSubItem(Anim2DDisable);

							m_Animator2D->Expand(FALSE);
							m_Animator2D->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (Anim2DDisable && Anim2DDisable->IsModified())
						{
							//	m_pTarget->AddComponent(new CAnimator2D());
						}
					}
					break;
				case COMPONENT_TYPE::ANIMATOR3D:
					if (m_pPrevTarget != m_pTarget)
					{
						if (Anim3DDisable == nullptr)
						{
							if (m_Animation3D != nullptr)
							{
								m_Animator3D->RemoveSubItem(m_Animation3D);
							}

							if (m_Animation3DCurAnim != nullptr)
							{
								m_Animator3D->RemoveSubItem(m_Animation3DCurAnim);
							}

							// Animation 3D
							Anim3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							Anim3DDisable->AddOption(L"Enable");
							m_Animator3D->AddSubItem(Anim3DDisable);

							m_Animator3D->Expand(FALSE);
							m_Animator3D->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (Anim3DDisable->IsModified())
						{
							//	m_pTarget->AddComponent(new CAnimator3D());
						}
					}
					break;
				case COMPONENT_TYPE::LIGHT2D:
					break;
				case COMPONENT_TYPE::LIGHT3D:
					if (m_pPrevTarget != m_pTarget)
					{
						if (LightDisable == nullptr)
						{
							if (LightType3D != nullptr)
							{			
								if (m_LightType->IsSelected() == false &&
									m_LightRim->IsSelected() == false &&
									m_LightRange->IsSelected() == false &&
									m_LightInAngle->IsSelected() == false &&
									m_LightOutAngle->IsSelected() == false)
								{
									if (m_LightColor_R->IsSelected() == false && m_LightColor_G->IsSelected() == false && m_LightColor_B->IsSelected() == false)
									{
										LightType3D->RemoveSubItem(m_LightColor);
										m_LightScript->RemoveSubItem(LightType3D);
									}
									else
									{
										LightColorEnable = true;
									}									
								}
								else
								{
									LightOptionEnable = true;
								}
							}

							// 3D Light
							LightDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							LightDisable->AddOption(L"Enable");
							m_LightScript->AddSubItem(LightDisable);

							m_LightScript->Expand(FALSE);
							m_LightScript->Expand(TRUE);
						}
					}
					else
					{
						// Idle update;
						if (LightDisable && LightDisable->IsModified())
						{
							//		m_pTarget->AddComponent(new CLight3D());
						}
					}
					break;
				case COMPONENT_TYPE::TERRAIN:
					if (m_pPrevTarget != m_pTarget)
					{
						if (TerrainDisable == nullptr)
						{
							if (m_Terrain_Tessellation != nullptr)
							{
								m_Terrain_Tessellation->RemoveSubItem(m_Terrain_TessEnable);
								if (m_Terrain_TessMin->IsSelected() == false && m_Terrain_TessMax->IsSelected() == false && m_Terrain_TessPhase->IsSelected() == false)
								{
									m_Terrain->RemoveSubItem(m_Terrain_Tessellation);
								}
								else
									TessEnable = true;
							}
							if (m_Terrain_Brush != nullptr)
							{
								m_Terrain_Brush->RemoveSubItem(m_Terrain_BrushType);
								if (m_Terrain_BrushScale->IsSelected() == false)
								{
									m_Terrain->RemoveSubItem(m_Terrain_Brush);
								}
								else
									BrushEnable = true;
							}
							if (m_Terrain_Landscape != nullptr)
							{
								m_Terrain_Landscape->RemoveSubItem(m_Terrain_LandscapingState);
								if (m_Terrain_Splatting_Material->IsSelected() == false)
								{
									m_Terrain->RemoveSubItem(m_Terrain_Landscape);
								}
								else
									MaterialEnable = true;
							}

							// Terrain
							TerrainDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							TerrainDisable->AddOption(L"Enable");
							m_Terrain->AddSubItem(TerrainDisable);

							m_Terrain->Expand(FALSE);
							m_Terrain->Expand(TRUE);

							if (TessEnable || BrushEnable || MaterialEnable)
							{
								m_Terrain->Expand(FALSE);
							}
						}
					}
					else
					{
						// Idle update;
						if (TerrainDisable && TerrainDisable->IsModified())
						{
							m_pTarget->AddComponent(new CTerrain());
						}
					}
					break;
				case COMPONENT_TYPE::PARTICLESYSTEM:
					if (m_pPrevTarget != m_pTarget)
					{
						if (ParticleDisable == nullptr)
						{
							if (m_ParticleSystem != nullptr)
							{
								if (m_ParticleFile->IsSelected() == false)
								{
									m_ParticleSystem->RemoveSubItem(ParticleFrequencyTitle);
								}
								else
									m_bParticleTime_Enable = true;
							}

							if (m_ParticleSystem != nullptr)
							{
								//	if (m_ParticleFrequncy->IsSelected() == false) 문제 있음
								//	{
								//		m_ParticleSystem->RemoveSubItem(ParticleFrequencyTitle);
								//	}
								//	else
								//		m_bParticleTime_Enable = true;
							}

							if (m_ParticleSystem != nullptr)
							{
								if (Particle_MinimumLifeTime->IsSelected() == false && Particle_MaximumLifeTime->IsSelected() == false)
								{
									m_ParticleSystem->RemoveSubItem(ParticleLifeTime);
								}
								else
									m_bParticleLife_Enable = true;
							}

							if (m_ParticleSystem != nullptr)
							{
								if (Particle_MinSpeed->IsSelected() == false && Particle_MaxSpeed->IsSelected() == false)
								{
									m_ParticleSystem->RemoveSubItem(ParticleSpeed);
								}
								else
									m_bParticlePace_Enable = true;
							}

							if (m_ParticleSystem != nullptr)
							{
								if (Particle_StartScale->IsSelected() == false && Particle_EndScale->IsSelected() == false)
								{
									m_ParticleSystem->RemoveSubItem(ParticleSacle);
								}
								else
									m_bParticleScale_Enable = true;
							}

							if (m_ParticleSystem != nullptr)
							{
								if (Particle_StartColor->IsSelected() == false &&
									Particle_EndColor->IsSelected() == false)
								{
									bool StartDeleteCheck = false;
									bool EndDeleteCheck = false;
									if (Particle_StartColor_R->IsSelected() == false &&
										Particle_StartColor_G->IsSelected() == false &&
										Particle_StartColor_B->IsSelected() == false &&
										Particle_StartColor_A->IsSelected() == false)
									{
										ParticleColor->RemoveSubItem(Particle_StartColor);
										StartDeleteCheck = true;
									}
									else
										m_bParticleColor_Enable = true;

									if (Particle_EndColor_R->IsSelected() == false &&
										Particle_EndColor_G->IsSelected() == false &&
										Particle_EndColor_B->IsSelected() == false &&
										Particle_EndColor_A->IsSelected() == false)
									{
										ParticleColor->RemoveSubItem(Particle_EndColor);
										EndDeleteCheck = true;
									}
									else
										m_bParticleColor_Enable = true;

									if (EndDeleteCheck && StartDeleteCheck)
									{
										m_ParticleSystem->RemoveSubItem(ParticleColor);
									}
								}					
							}
	
							// Particle System
							ParticleDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
							ParticleDisable->AddOption(L"Enable");
							m_ParticleSystem->AddSubItem(ParticleDisable);

							m_ParticleSystem->Expand(FALSE);
							m_ParticleSystem->Expand(TRUE);

							if (m_bParticleTime_Enable || m_bParticleLife_Enable || m_bParticlePace_Enable || m_bParticleScale_Enable || m_bParticleColor_Enable)
							{
								m_ParticleSystem->Expand(FALSE);
							}
						}						
					}
					else
					{
						// Idle update;
						if (ParticleDisable && ParticleDisable->IsModified())
						{
							m_pTarget->AddComponent(new CParticleSystem());
						}
					}
					break;
				case COMPONENT_TYPE::END:
					break;
				case COMPONENT_TYPE::SCRIPT:
					break;
				default:
					break;
				}
			}
			else
			{
				switch ((COMPONENT_TYPE)i)
				{
				case COMPONENT_TYPE::TRANSFORM:

					if (TransformDisable != nullptr)	//아직 정보 활성화가 안된 경우
					{
						m_PropertyName->SetValue((_variant_t)_T("이름"));
						m_PropertyLayer->SetValue((_variant_t)_T("Layer"));

						Vec3 arrTransform[3] = { GetTarget()->Transform()->GetWorldPos()
						, GetTarget()->Transform()->GetLocalScale()
						, GetTarget()->Transform()->GetLocalRot() };

						arrTransform[2] = (arrTransform[2] / XM_PI) * 180.f;

						CString str;

						m_PropertyTransform->RemoveSubItem(TransformDisable);

						// Transform

						//Position
						m_PropertyTransform_Position = new CMFCPropertyGridProperty(_T("Position"));
						str.Format(L"%f", arrTransform[0][0]);
						m_PropertyTransform_Position_X = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)(str), _T("Position X 입니다."));
						str.Format(L"%f", arrTransform[0][1]);
						m_PropertyTransform_Position_Y = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)(str), _T("Position Y 입니다."));
						str.Format(L"%f", arrTransform[0][2]);
						m_PropertyTransform_Position_Z = new CMFCPropertyGridProperty(_T("Position Z"), (_variant_t)(str), _T("Position Z 입니다."));
						m_PropertyTransform->AddSubItem(m_PropertyTransform_Position);

						m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_X);
						m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Y);
						m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Z);

						//Scale
						m_PropertyTransform_Scale = new CMFCPropertyGridProperty(_T("Scale"));
						str.Format(L"%f", arrTransform[1][0]);
						m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
						str.Format(L"%f", arrTransform[1][1]);
						m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
						str.Format(L"%f", arrTransform[1][2]);
						m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));
						m_PropertyTransform->AddSubItem(m_PropertyTransform_Scale);

						m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
						m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
						m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);

						//Rotation
						m_PropertyTransform_Rotation = new CMFCPropertyGridProperty(_T("Rotation"));
						str.Format(L"%f", arrTransform[2][0]);
						m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
						str.Format(L"%f", arrTransform[2][1]);
						m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
						str.Format(L"%f", arrTransform[2][2]);
						m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));
						m_PropertyTransform->AddSubItem(m_PropertyTransform_Rotation);

						m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
						m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
						m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

						m_PropertyTransform->Expand(FALSE);
						m_PropertyTransform->Expand();
					}
					else	//정보 활성화가 된 이후 업데이트.
					{
						UpdateCtrl_Transform();
					}

					break;
				case COMPONENT_TYPE::MESHRENDER:
					if (MeshRendererDisable != nullptr)
					{
						if (m_pTarget->Light3D() == NULL)
						{
							m_PropertyMesh_Renderer->RemoveSubItem(MeshRendererDisable);

							m_PropertyMesh_MeshTitle = new CMFCPropertyGridProperty(_T("Mesh"));
							//Mesh Render

							//Mesh Data
							m_PropertyMesh_MeshData = new CMFCPropertyGridProperty(_T("Subset"), _T("Mesh Data"));
							m_PropertyMesh_MeshData->Enable(TRUE);

							vector<tMeshContainer*>& meshSubSet = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMesh())->GetSubset();

							for (auto pair : meshSubSet)
							{
								m_PropertyMesh_MeshData->AddOption(pair->strName.c_str());
							}
							m_PropertyMesh_MeshData->AllowEdit(FALSE);

							//Mesh Res
							m_PropertyMesh_Mesh = new CMFCPropertyGridProperty(_T("Resource"), _T("Mesh List"));
							m_PropertyMesh_Mesh->Enable(TRUE);

							const map<wstring, CResource*>& meshmap = CResMgr::GetInst()->GetResource(RES_TYPE::MESH);

							for (const auto & pair : meshmap)
							{
								m_PropertyMesh_Mesh->AddOption(pair.first.c_str());
							}
							m_PropertyMesh_Mesh->AllowEdit(FALSE);

							//
							m_PropertyMesh_Renderer->AddSubItem(m_PropertyMesh_MeshTitle);
							m_PropertyMesh_MeshTitle->AddSubItem(m_PropertyMesh_MeshData);
							m_PropertyMesh_MeshTitle->AddSubItem(m_PropertyMesh_Mesh);


							//Material
							m_PropertyMesh_MaterialTitle = new CMFCPropertyGridProperty(_T("Material"));

							//Material Conatiner
							m_PropertyMesh_MaterialData = new CMFCPropertyGridProperty(_T("Container"), _T("Mtrl List"));
							m_PropertyMesh_MaterialData->Enable(TRUE);

							vector<vector<Ptr<CMaterial>>>* MtrlContainer = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetVectorMaterial());
							vector<vector<Ptr<CMaterial>>>::iterator iterBeginner = MtrlContainer->begin();

							for (vector<vector<Ptr<CMaterial>>>::iterator iterContainer = MtrlContainer->begin(); iterContainer < MtrlContainer->end(); ++iterContainer)
							{
								for (vector<Ptr<CMaterial>>::iterator iterSubset = iterContainer->begin(); iterSubset < iterContainer->end(); ++iterSubset)
								{
									int MeshIndex = iterContainer - iterBeginner;

									wstring MtrlSubSetName = iterSubset->GetPointer()->GetName();
									wstring MtrlContainerName = meshSubSet[MeshIndex]->strName;

									m_PropertyMesh_MaterialData->AddOption((MtrlContainerName + L" / " + MtrlSubSetName).c_str());
								}
							}
							m_PropertyMesh_MaterialData->AllowEdit(FALSE);


							//Material List
							m_PropertyMesh_Material = new CMFCPropertyGridProperty(_T("Resource"), _T("Mtrl List"));
							m_PropertyMesh_Material->Enable(TRUE);



							const map<wstring, CResource*>& Mtrlmap = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);

							for (const auto & pair : Mtrlmap)
							{
								m_PropertyMesh_Material->AddOption(pair.first.c_str());
							}
							m_PropertyMesh_Material->AllowEdit(FALSE);





							//
							m_PropertyMesh_Renderer->AddSubItem(m_PropertyMesh_MaterialTitle);
							m_PropertyMesh_MaterialTitle->AddSubItem(m_PropertyMesh_MaterialData);
							m_PropertyMesh_MaterialTitle->AddSubItem(m_PropertyMesh_Material);

							m_PropertyMesh_Renderer->Expand(FALSE);
							m_PropertyMesh_Renderer->Expand();
						}
					}
					else
					{
						// Mtrl Modif
						if (m_PropertyMesh_MeshData->IsModified())
						{
							MtrL_To_apply = m_PropertyMesh_MeshData->GetValue().bstrVal;
						}

						// update
						if (m_PropertyMesh_Mesh->IsModified())
						{
							CString SelectingMesh = m_PropertyMesh_Mesh->GetValue().bstrVal;

							Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(SelectingMesh.GetBuffer());
							//	GetTarget()->MeshRender()->SetMesh(pMesh);			//Set이 아닌 Add로 바꿔줘야 함. 0708
							(((CMeshRender*)(GetTarget()->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMesh())->AddContainer(pMesh->GetSubset(), *pMesh.GetPointer());	//0709
							strMesh = pMesh->GetName().c_str();
							strMtrl = "";

							m_PropertyMesh_MeshTitle->RemoveSubItem(m_PropertyMesh_Mesh);
							m_PropertyMesh_MeshTitle->RemoveSubItem(m_PropertyMesh_MeshData);
							m_PropertyMesh_MaterialTitle->RemoveSubItem(m_PropertyMesh_Material);

							//Mesh Data
							m_PropertyMesh_MeshData = new CMFCPropertyGridProperty(_T("Subset"), _T("Mesh Data"));
							m_PropertyMesh_MeshData->Enable(TRUE);

							vector<tMeshContainer*>& meshSubSet = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMesh())->GetSubset();

							//	for (vector<tMeshContainer*>::iterator iter = meshSubSet.begin(); i < meshSubSet.size(); ++iter)
							for (auto pair : meshSubSet)
							{
								m_PropertyMesh_MeshData->AddOption(pair->strName.c_str());
							}
							m_PropertyMesh_MeshData->AllowEdit(FALSE);

							//Mesh Render
							m_PropertyMesh_Mesh = new CMFCPropertyGridProperty(_T("Resource"), _T("Mesh List"));
							m_PropertyMesh_Mesh->Enable(TRUE);
							const map<wstring, CResource*>& meshmap = CResMgr::GetInst()->GetResource(RES_TYPE::MESH);
							for (const auto & pair : meshmap)
							{
								m_PropertyMesh_Mesh->AddOption(pair.first.c_str());
							}


							m_PropertyMesh_Mesh->AllowEdit(FALSE);
							m_PropertyMesh_MeshTitle->AddSubItem(m_PropertyMesh_MeshData);
							m_PropertyMesh_MeshTitle->AddSubItem(m_PropertyMesh_Mesh);

							//Material
							m_PropertyMesh_Material = new CMFCPropertyGridProperty(_T("Material"), _T("Mtrl List"));
							m_PropertyMesh_Material->Enable(TRUE);

							const map<wstring, CResource*>& Mtrlmap = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
							for (const auto & pair : Mtrlmap)
							{
								m_PropertyMesh_Material->AddOption(pair.first.c_str());
							}

							m_PropertyMesh_Material->AllowEdit(FALSE);
							m_PropertyMesh_MaterialTitle->AddSubItem(m_PropertyMesh_Material);


							m_PropertyMesh_Renderer->Expand(FALSE);
							m_PropertyMesh_Renderer->Expand();

						}
						if (m_PropertyMesh_Material->IsModified())
						{
							MtrL_To_apply = m_PropertyMesh_MeshData->GetValue().bstrVal;

							if (MtrL_To_apply == MtrL_Origin)
							{
								m_PropertyMesh_MaterialTitle->RemoveSubItem(m_PropertyMesh_Material);

								m_PropertyMesh_Material = new CMFCPropertyGridProperty(_T("Material"), _T("Mtrl List"));
								m_PropertyMesh_Material->Enable(TRUE);

								const map<wstring, CResource*>& Mtrlmap = CResMgr::GetInst()->GetResource(RES_TYPE::MATERIAL);
								for (const auto & pair : Mtrlmap)
								{
									m_PropertyMesh_Material->AddOption(pair.first.c_str());
								}

								m_PropertyMesh_Material->AllowEdit(FALSE);
								m_PropertyMesh_MaterialTitle->AddSubItem(m_PropertyMesh_Material);


								m_PropertyMesh_Renderer->Expand(FALSE);
								m_PropertyMesh_Renderer->Expand();
								break;
							}

							wstring AddingMaterial = m_PropertyMesh_Material->GetValue().bstrVal;	//추가할 Material
														//	MtrL_To_apply;	추가할 Containner 

							Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(AddingMaterial);


							vector<vector<Ptr<CMaterial>>>* MtrlContainer = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetVectorMaterial());
							vector<vector<Ptr<CMaterial>>>::iterator iterBeginner = MtrlContainer->begin();
							vector<tMeshContainer*>& meshSubSet = (((CMeshRender*)(m_pTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMesh())->GetSubset();

							for (vector<vector<Ptr<CMaterial>>>::iterator iterContainer = MtrlContainer->begin(); iterContainer < MtrlContainer->end(); ++iterContainer)
							{
								for (vector<Ptr<CMaterial>>::iterator iterSubset = iterContainer->begin(); iterSubset < iterContainer->end(); ++iterSubset)
								{
									int MeshIndex = iterContainer - iterBeginner;

									wstring MtrlSubSetName = iterSubset->GetPointer()->GetName();
									wstring MtrlContainerName = meshSubSet[MeshIndex]->strName;

									if (MtrL_To_apply == MtrlContainerName)
									{
										iterContainer->push_back(pMtrl);			//	갖고 있는 서브셋 중 메테리얼 바꾸는 방식으로 변경해야 함.
										break;
									}
								}
							}
						}

						UpdateCtrl_MeshRender();
					}
					break;
				case COMPONENT_TYPE::CAMERA:
					if (CameraDisable != nullptr)
					{
						CString str;
						float m_fData;

						m_Camera->RemoveSubItem(CameraDisable);
						CameraDisable = nullptr;
						
						
						CAMERA_MOD _eMod = CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD;
							
						if (_eMod == CAMERA_MOD::CLIENT_MOD)
							CameraOption = new CMFCPropertyGridProperty(_T("Option"), (_variant_t)(_T("Client MOD")), _T("Camera Option."));
						else
							CameraOption = new CMFCPropertyGridProperty(_T("Option"), (_variant_t)(_T("Engine MOD")), _T("Camera Option."));

						CameraOption->AddOption(L"Engine MOD");
						CameraOption->AddOption(L"Client MOD");

						m_Camera->AddSubItem(CameraOption);
						//
						//Weather
						WeatherOption = new CMFCPropertyGridProperty(_T("Climate Option"));
						m_Camera->AddSubItem(WeatherOption);

						bool m_bCurWeatherEnable = CSceneMgr::GetInst()->GetCurScene()->GetWeatherEnable();
						if (m_bCurWeatherEnable)
							WeatherEnable = new CMFCPropertyGridProperty(_T("Climate"), (_variant_t)(_T("Climate On")), _T("Climate Control."));
						else
							WeatherEnable = new CMFCPropertyGridProperty(_T("Climate"), (_variant_t)(_T("Climate Off")), _T("Climate Control."));
						
						static const TCHAR szFilter[] = _T("아이콘 파일(*.png)|*.png|모든 파일(*.*)|*.*||");
						WeatherShaderTexture = new CMFCPropertyGridFileProperty(_T("Climate Mtrl"), TRUE, _T(""), _T("png"), 0, szFilter, _T("Particle mtrl을 지정합니다."));

						WeatherEnable->AddOption(L"Climate On");
						WeatherEnable->AddOption(L"Climate Off");

						WeatherOption->AddSubItem(WeatherEnable);
						WeatherOption->AddSubItem(WeatherShaderTexture);

						//Range
						CameraRange = new CMFCPropertyGridProperty(_T("Range"));
						m_Camera->AddSubItem(CameraRange);

						m_fData = GetTarget()->Camera()->GetNear();
						str.Format(L"%f", m_fData);
						CameraNear = new CPropGridSlider(_T("Near"), (_variant_t)(str), _T("Projection Near."), 1.f, 999.0f, 1000);
						m_fData = GetTarget()->Camera()->GetFar();
						str.Format(L"%f", m_fData);
						CameraFar = new CPropGridSlider(_T("Far"), (_variant_t)(str), _T("Projection Far."), 2.f, 1000.f, 1000);

						CameraRange->AddSubItem(CameraNear);
						CameraRange->AddSubItem(CameraFar);

						//Ratio
						CameraRatio = new CMFCPropertyGridProperty(_T("Proportion"));
						m_Camera->AddSubItem(CameraRatio);

						m_fData = GetTarget()->Camera()->GetFOV();
						str.Format(L"%f", m_fData * _RADIAN);
						CameraFOV = new CPropGridSlider(_T("FOV"), (_variant_t)(str), _T("Camera FOV."), 1.f, XM_PI / 4.f * _RADIAN, 1000);
						m_fData = GetTarget()->Camera()->GetScale();
						str.Format(L"%f", m_fData);
						CameraScale = new CPropGridSlider(_T("Scale"), (_variant_t)(str), _T("Camera Scale."), 0.01f, 1.f, 1000);

						CameraRatio->AddSubItem(CameraFOV);
						CameraRatio->AddSubItem(CameraScale);


						//Resolution
						CameraResolution = new CMFCPropertyGridProperty(_T("Camera Resolution"));
						m_Camera->AddSubItem(CameraResolution);

						m_fData = GetTarget()->Camera()->GetWidth();
						str.Format(L"%f", m_fData);
						CameraWidth = new CPropGridSlider(_T("Camera Width"), (_variant_t)(str), _T("Camera FOV."), 512.f * 1.71421356f, 3000.f, 1000);
						m_fData = GetTarget()->Camera()->GetHeight();
						str.Format(L"%f", m_fData);
						CameraHeight = new CPropGridSlider(_T("Camera Height"), (_variant_t)(str), _T("Camera Scale."), 512.f, 3000.f, 1000);

						CameraResolution->AddSubItem(CameraWidth);
						CameraResolution->AddSubItem(CameraHeight);
						//

						m_Camera->Expand(FALSE);
						m_Camera->Expand();
					}
					else
					{
						UpdateCtrl_Camera();
					}
					break;
				case COMPONENT_TYPE::COLLIDER2D:
					if (Offset2DDisable != nullptr)
					{
						m_PropertyCollider2D->RemoveSubItem(Offset2DDisable);
						Offset2DDisable = nullptr;


						// Collider 2D

						// Collider Pos
						Offset2D_Pos = new CMFCPropertyGridProperty(_T("Offset Position"));
						m_PropertyCollider2D->AddSubItem(Offset2D_Pos);

						Offset2D_PosX = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)_T("vPos x"), _T("Offset Position X 입니다."));
						Offset2D_PosY = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)_T("vPos y"), _T("Offset Position Y 입니다."));

						Offset2D_Pos->AddSubItem(Offset2D_PosX);
						Offset2D_Pos->AddSubItem(Offset2D_PosY);


						// Collider Scale
						Offset2D_Scale = new CMFCPropertyGridProperty(_T("Offset Scale"));
						m_PropertyCollider2D->AddSubItem(Offset2D_Scale);

						Offset2D_ScaleX = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)_T("vScale x"), _T("Scale X 입니다."));
						Offset2D_ScaleY = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)_T("vScale y"), _T("Scale Y 입니다."));

						Offset2D_Scale->AddSubItem(Offset2D_ScaleX);
						Offset2D_Scale->AddSubItem(Offset2D_ScaleY);


						m_PropertyCollider2D->Expand(FALSE);
						m_PropertyCollider2D->Expand();
					}
					else
					{
						UpdateCtrl_Collider2D();
					}
					break;
				case COMPONENT_TYPE::COLLIDER3D:
					if (Offset3DDisable != nullptr)
					{
						m_PropertyCollider3D->RemoveSubItem(Offset3DDisable);
						Offset3DDisable = nullptr;

						// Collider 3D

						// Collider Pos
						Offset3D_Pos = new CMFCPropertyGridProperty(_T("Offset Position"));
						m_PropertyCollider3D->AddSubItem(Offset3D_Pos);

						Offset3D_PosX = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)_T("NULL"), _T("Offset Position X 입니다."));
						Offset3D_PosY = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)_T("NULL"), _T("Offset Position Y 입니다."));
						Offset3D_PosZ = new CMFCPropertyGridProperty(_T("Position Z"), (_variant_t)_T("NULL"), _T("Offset Position Z 입니다."));

						Offset3D_Pos->AddSubItem(Offset3D_PosX);
						Offset3D_Pos->AddSubItem(Offset3D_PosY);
						Offset3D_Pos->AddSubItem(Offset3D_PosZ);


						// Collider Scale
						Offset3D_Scale = new CMFCPropertyGridProperty(_T("Offset Scale"));
						m_PropertyCollider3D->AddSubItem(Offset3D_Scale);

						Offset3D_ScaleX = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)_T("1"), _T("Scale X 입니다."));
						Offset3D_ScaleY = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)_T("1"), _T("Scale Y 입니다."));
						Offset3D_ScaleZ = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)_T("1"), _T("Scale Z 입니다."));

						Offset3D_Scale->AddSubItem(Offset3D_ScaleX);
						Offset3D_Scale->AddSubItem(Offset3D_ScaleY);
						Offset3D_Scale->AddSubItem(Offset3D_ScaleZ);

						m_PropertyCollider3D->Expand(FALSE);
						m_PropertyCollider3D->Expand();
					}
					else
					{
						UpdateCtrl_Collider3D();
					}
					break;
				case COMPONENT_TYPE::ANIMATOR2D:
					if (Anim2DDisable != nullptr)
					{
						m_Animator2D->RemoveSubItem(Anim2DDisable);
						Anim2DDisable = nullptr;

						//Animation 2D

						m_Animation2D = new CMFCPropertyGridProperty(_T("2D Anim"), _T("Anim List"));
						m_Animation2D->Enable(TRUE);

						if (m_pTarget)
						{
							const map<wstring, CAnimation2D*>& mapAnim = m_pTarget->Animator2D()->GetAnimations();

							for (const auto & pair : mapAnim)
							{
								m_Animation2D->AddOption(pair.first.c_str());
							}
						}
						m_Animation2D->AllowEdit(FALSE);
						m_Animator2D->AddSubItem(m_Animation2D);

						m_Animator2D->Expand(FALSE);
						m_Animator2D->Expand();
					}
					else
					{
						UpdateCtrl_Animator2D();
					}
					break;
				case COMPONENT_TYPE::ANIMATOR3D:
					if (Anim3DDisable != nullptr)
					{
						m_Animator3D->RemoveSubItem(Anim3DDisable);
						Anim3DDisable = nullptr;


						//Animation 3D

						m_Animation3D = new CMFCPropertyGridProperty(_T("3D Anim"), _T("Anim List"));
						m_Animation3D->Enable(TRUE);

						m_Animation3DCurAnim = new CMFCPropertyGridProperty(_T("Present Anim"), _T("Animation"));
						m_Animation3DCurAnim->Enable(TRUE);

						// 3D 클립 구조 이해 후 추가 해야함

						if (m_pTarget)
						{
							std::vector<struct CAnimClip*>::iterator iter = m_pTarget->Animator3D()->GetVecAnimClip()->begin();
							std::vector<struct CAnimClip*>::iterator iterEnd = m_pTarget->Animator3D()->GetVecAnimClip()->end();

							std::vector<struct CAnimClip*>* temp = m_pTarget->Animator3D()->GetVecAnimClip();

							for (; iter != iterEnd; ++iter)
							{
								m_Animation3D->AddOption((*iter)->GetClipName().c_str());

								if ((*iter)->GetClipName() != m_pTarget->Animator3D()->GetAnimClip()->GetClipName())
								{
									continue;
								}
								else
								{
									AnimState = (*iter)->GetClipName();
									m_Animation3DCurAnim->SetValue((*iter)->GetClipName().c_str());

									++iter;
									if (iter != iterEnd)
									{
										wstring NextAnimName = (*iter)->GetClipName();
										m_pTarget->Animator3D()->SetCurrentAnimClip((*iter)->GetClipName().c_str());
									}
									else
									{
										break;
									}
								}
							}
						}

						m_Animation3D->AllowEdit(FALSE);

						//
						m_Animator3D->AddSubItem(m_Animation3DCurAnim);
						m_Animator3D->AddSubItem(m_Animation3D);


						m_Animator3D->Expand(FALSE);
						m_Animator3D->Expand();
					}
					else
					{
						UpdateCtrl_Animator3D();
					}
					break;
				case COMPONENT_TYPE::LIGHT2D:
					break;
				case COMPONENT_TYPE::LIGHT3D:
					if (LightDisable != nullptr)
					{
						m_LightScript->RemoveSubItem(LightDisable);
						LightDisable = nullptr;


													
						if (m_LightScript != nullptr && LightColorEnable == false)
						{
							if (LightType3D != nullptr)
							{
								LightType3D->RemoveSubItem(m_LightType);
							}

							if (LightType3D != nullptr)
							{
								m_LightScript->RemoveSubItem(LightType3D);
							}
						}							
					//	LightColorEnable = false;							
												
						if (LightColorEnable == false || LightType3D == nullptr)
						{
							CString str;

							LightColorEnable = false;

							LightType3D = new CMFCPropertyGridProperty(_T("3D Light"));
							m_LightScript->AddSubItem(LightType3D);


							m_LightType = new CMFCPropertyGridProperty(_T("Type"), (_variant_t)_T("Light List"), _T("Light Type 입니다."));
							m_LightColor = new CMFCPropertyGridProperty(_T("Color"));

							if (m_pTarget->Light3D()->GetRimLight())							
								str = L"RimLight On";
							else
								str = L"RimLight Off";

							m_LightRim = new CMFCPropertyGridProperty(_T("Rim Right"), (_variant_t)(str), _T("Rim Right 입니다."));
							m_LightRange = new CMFCPropertyGridProperty(_T("Range"), (_variant_t)_T("0"), _T("Light Range 입니다."));
							m_LightInAngle = new CMFCPropertyGridProperty(_T("InAngle"), (_variant_t)_T("0"), _T("InAngle 입니다."));
							m_LightOutAngle = new CMFCPropertyGridProperty(_T("OutAngle"), (_variant_t)_T("0"), _T("OutAngle 입니다."));

							LightType3D->AddSubItem(m_LightType);
							LightType3D->AddSubItem(m_LightColor);
							LightType3D->AddSubItem(m_LightRim);
							LightType3D->AddSubItem(m_LightRange);
							LightType3D->AddSubItem(m_LightInAngle);
							LightType3D->AddSubItem(m_LightOutAngle);

							m_LightType->AddOption(L"Directional Light");
							m_LightType->AddOption(L"Point Light");
							m_LightType->AddOption(L"Spot Light");

							m_LightColor_R = new CMFCPropertyGridProperty(_T("Red"), (_variant_t)_T("75"), _T("Red Value 입니다."));
							m_LightColor_G = new CMFCPropertyGridProperty(_T("Green"), (_variant_t)_T("75"), _T("Green Value 입니다."));
							m_LightColor_B = new CMFCPropertyGridProperty(_T("Blue"), (_variant_t)_T("75"), _T("Blue Value 입니다."));

							m_LightColor->AddSubItem(m_LightColor_R);
							m_LightColor->AddSubItem(m_LightColor_G);
							m_LightColor->AddSubItem(m_LightColor_B);

							m_LightRim->AddOption(L"RimLight On");
							m_LightRim->AddOption(L"RimLight Off");
						}
						
						m_LightScript->Expand(FALSE);
						m_LightScript->Expand();
					}
					else
					{
						UpdateCtrl_Light3D();
					}
					break;
				case COMPONENT_TYPE::TERRAIN:
					if (TerrainDisable != nullptr)
					{
						m_Terrain->RemoveSubItem(TerrainDisable);
						TerrainDisable = nullptr;

						if (TessEnable)
						{
							m_Terrain->RemoveSubItem(m_Terrain_Tessellation);
							TessEnable = false;
						}
						if (BrushEnable)
						{
							m_Terrain->RemoveSubItem(m_Terrain_Brush);
							BrushEnable = false;
						}
						if (MaterialEnable)
						{
							m_Terrain->RemoveSubItem(m_Terrain_Landscape);
							MaterialEnable = false;
						}

						CString str;
						wstring OptionData;
						if (GetTarget()->Terrain()->GetTerrainOption() == TERRAIN_OPTION::DISABLE)
						{
							OptionData = L"Disable";
						}
						else
						{
							OptionData = L"Enable";
						}

						//Tess
						m_Terrain_Tessellation = new CMFCPropertyGridProperty(_T("Tessellation"));
						m_Terrain_Tessellation->Enable(TRUE);
						m_Terrain_TessEnable = new CMFCPropertyGridProperty(_T("Option Confirm"), (_variant_t)OptionData.c_str(), _T("Tessellation Confirm"));
						m_Terrain_TessEnable->AddOption(L"Disable");
						m_Terrain_TessEnable->AddOption(L"Enable");

						Vec3 TessLevelData;
						TessLevelData = GetTarget()->Terrain()->GetTessLevel();
						str.Format(L"%f", TessLevelData.x * 0.0001f);
						m_Terrain_TessMin = new CPropGridSlider(_T("Near Range"), (_variant_t)(str), _T("Tessellation Min Level 입니다."), 0.f, 8.0f, 1000);
						str.Format(L"%f", TessLevelData.y * 0.0001f);
						m_Terrain_TessMax = new CPropGridSlider(_T("Far Range"), (_variant_t)(str), _T("Tessellation Max Level 입니다."), 0.f, 8.0f, 1000);
						str.Format(L"%f", TessLevelData.z);
						m_Terrain_TessPhase = new CPropGridSlider(_T("Detail Phase"), (_variant_t)(str), _T("Tessellation 단계 입니다."), 0.f, 10.0f, 1000);


						float m_fBrushRange = GetTarget()->Terrain()->GetBrushScale();
						str.Format(L"%f", m_fBrushRange * 100.f);

						//Terrain Brush
						m_Terrain_Brush = new CMFCPropertyGridProperty(_T("Brush"));
						m_Terrain_BrushType = new CMFCPropertyGridProperty(_T("Brush Type"), (_variant_t)_T("Ink"), _T("사용할 Brush의 종류입니다."));
						m_Terrain_BrushScale = new CPropGridSlider(_T("Brush Range"), (_variant_t)(str), _T("Brush Size 입니다."), 1.f, 5.0f, 1000);

						m_Terrain_Landscape = new CMFCPropertyGridProperty(_T("Landscaping"));
						m_Terrain_Brush->Enable(TRUE);	// 밑으로 뺴야함

						m_Terrain_LandscapingState = new CMFCPropertyGridProperty(_T("Landscape State"), (_variant_t)_T("Disable"), _T("Height Map / Weight Map 입니다."));
						m_Terrain_Splatting_Material = new CMFCPropertyGridColorProperty(_T("Splat Material"), RGB(0, 0, 0), nullptr, _T("Texture Splatting 입니다."));
						m_Terrain_Splatting_Material->EnableOtherButton(_T("ETC.."));
						m_Terrain_Splatting_Material->EnableAutomaticButton(_T("Del Mtrl"), ::GetSysColor(COLOR_3DFACE));


						m_Terrain->AddSubItem(m_Terrain_Tessellation);
						m_Terrain->AddSubItem(m_Terrain_Brush);
						m_Terrain->AddSubItem(m_Terrain_Landscape);

						m_Terrain_Tessellation->AddSubItem(m_Terrain_TessEnable);
						m_Terrain_Tessellation->AddSubItem(m_Terrain_TessMin);
						m_Terrain_Tessellation->AddSubItem(m_Terrain_TessMax);
						m_Terrain_Tessellation->AddSubItem(m_Terrain_TessPhase);
						m_Terrain_Brush->AddSubItem(m_Terrain_BrushType);
						m_Terrain_Brush->AddSubItem(m_Terrain_BrushScale);
						m_Terrain_Landscape->AddSubItem(m_Terrain_LandscapingState);
						m_Terrain_Landscape->AddSubItem(m_Terrain_Splatting_Material);

						m_Terrain_BrushType->AddOption(L"Ink");
						m_Terrain_BrushType->AddOption(L"Circle");


						m_Terrain_LandscapingState->AddOption(L"Disable");
						m_Terrain_LandscapingState->AddOption(L"Height Map");
						m_Terrain_LandscapingState->AddOption(L"Weight Map");



						m_Terrain_Brush->AllowEdit(FALSE);

						m_Terrain->Expand(FALSE);
						m_Terrain->Expand();
					}
					else
					{
						UpdateCtrl_Terrain();
					}
					break;
				case COMPONENT_TYPE::PARTICLESYSTEM:
					if (ParticleDisable != nullptr)
					{
						CString str;
						float m_fData;
						Vec4 m_vData;

						m_ParticleSystem->RemoveSubItem(ParticleDisable);
						ParticleDisable = nullptr;

						if (m_bParticleTime_Enable)
						{
							m_ParticleSystem->RemoveSubItem(ParticleFrequencyTitle);
							m_bParticleTime_Enable = false;
						}
						if (m_bParticleLife_Enable)
						{
							m_ParticleSystem->RemoveSubItem(ParticleLifeTime);
							m_bParticleLife_Enable = false;
						}
						if (m_bParticlePace_Enable)
						{
							m_ParticleSystem->RemoveSubItem(ParticleSpeed);
							m_bParticlePace_Enable = false;
						}
						if (m_bParticleScale_Enable)
						{
							m_ParticleSystem->RemoveSubItem(ParticleSacle);
							m_bParticleScale_Enable = false;
						}
						if (m_bParticleColor_Enable)
						{
							m_ParticleSystem->RemoveSubItem(ParticleColor);
							m_bParticleColor_Enable = false;
						}




						ParticleFrequencyTitle = new CMFCPropertyGridProperty(_T("Particle Time"));
						ParticleLifeTime = new CMFCPropertyGridProperty(_T("Particle Life"));
						ParticleSpeed = new CMFCPropertyGridProperty(_T("Particle Pace"));
						ParticleSacle = new CMFCPropertyGridProperty(_T("Particle Sacle"));
						ParticleColor = new CMFCPropertyGridProperty(_T("Particle Color"));

						m_ParticleSystem->AddSubItem(ParticleFrequencyTitle);
						m_ParticleSystem->AddSubItem(ParticleColor);
						m_ParticleSystem->AddSubItem(ParticleLifeTime);
						m_ParticleSystem->AddSubItem(ParticleSpeed);
						m_ParticleSystem->AddSubItem(ParticleSacle);

						static const TCHAR szFilter[] = _T("아이콘 파일(*.png)|*.png|모든 파일(*.*)|*.*||");
						m_ParticleFile = new CMFCPropertyGridFileProperty(_T("Particle Mtrl"), TRUE, _T(""), _T("png"), 0, szFilter, _T("Particle mtrl을 지정합니다."));

						PARTICLE_TYPE ParticleType = GetTarget()->Particlesystem()->GetParticleType();
						
						if (ParticleType == PARTICLE_TYPE::EXPLOSION)
						{
							str = L"Explosion";
						}
						else if (ParticleType == PARTICLE_TYPE::FLUTTER)
						{
							str = L"Flutter";
						}
						else if (ParticleType == PARTICLE_TYPE::FALL)
						{
							str = L"Fall";
						}
						m_ParticleType = new CMFCPropertyGridProperty(_T("Particle Type"), (_variant_t)(str), _T("사용할 Brush의 종류입니다."));
						m_ParticleType->AddOption(L"Explosion");
						m_ParticleType->AddOption(L"Flutter");
						m_ParticleType->AddOption(L"Fall");

						m_fData = GetTarget()->Particlesystem()->GetFrequency();
						str.Format(L"%f", m_fData);
						m_ParticleFrequncy = new CPropGridSlider(_T("Frequncy"), (_variant_t)(str), _T("생성 주기입니다."), 0.00001f, 1.0f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetMinLifeTime();
						str.Format(L"%f", m_fData);
						Particle_MinimumLifeTime = new CPropGridSlider(_T("Min Life"), (_variant_t)(str), _T("Particle 최소 생존 시간입니다."), 0.f, 100.f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetMaxLifeTime();
						str.Format(L"%f", m_fData);
						Particle_MaximumLifeTime = new CPropGridSlider(_T("Max Life"), (_variant_t)(str), _T("Particle 최대 생존 시간입니다."), 0.f, 100.f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetMinSpeed();
						str.Format(L"%f", m_fData);
						Particle_MinSpeed = new CPropGridSlider(_T("Begin Pace"), (_variant_t)(str), _T("Particle 시작 속도입니다."), 0.f, 500.0f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetMaxSpeed();
						str.Format(L"%f", m_fData);
						Particle_MaxSpeed = new CPropGridSlider(_T("End Pace"), (_variant_t)(str), _T("Particle 종료 속도입니다."), 0.f, 500.0f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetStartScale();
						str.Format(L"%f", m_fData);
						Particle_StartScale = new CPropGridSlider(_T("Begin Scale"), (_variant_t)(str), _T("particle 시작 크기입니다."), 0.f, 500.0f, 1000);

						m_fData = GetTarget()->Particlesystem()->GetEndScale();
						str.Format(L"%f", m_fData);
						Particle_EndScale = new CPropGridSlider(_T("End Scale"), (_variant_t)(str), _T("Particle 종료 크기입니다."), 0.f, 500.0f, 1000);

						Particle_StartColor = new CMFCPropertyGridProperty(_T("Begin Color"));
						Particle_EndColor = new CMFCPropertyGridProperty(_T("End Color"));

						ParticleFrequencyTitle->AddSubItem(m_ParticleFile);
						ParticleFrequencyTitle->AddSubItem(m_ParticleType);
						ParticleFrequencyTitle->AddSubItem(m_ParticleFrequncy);
						ParticleColor->AddSubItem(Particle_StartColor);
						ParticleColor->AddSubItem(Particle_EndColor);

						ParticleLifeTime->AddSubItem(Particle_MinimumLifeTime);
						ParticleLifeTime->AddSubItem(Particle_MaximumLifeTime);
						ParticleSpeed->AddSubItem(Particle_MinSpeed);
						ParticleSpeed->AddSubItem(Particle_MaxSpeed);
						ParticleSacle->AddSubItem(Particle_StartScale);
						ParticleSacle->AddSubItem(Particle_EndScale);
										
						m_vData = GetTarget()->Particlesystem()->GetStartColor();
						str.Format(L"%f", m_vData.x);
						Particle_StartColor_R = new CPropGridSlider(_T("Red"), (_variant_t)(str), _T("Particle Begin Red입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.y);
						Particle_StartColor_G = new CPropGridSlider(_T("Green"), (_variant_t)(str), _T("Particle Begin Green입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.z);
						Particle_StartColor_B = new CPropGridSlider(_T("Blue"), (_variant_t)(str), _T("Particle Begin Blue입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.w);
						Particle_StartColor_A = new CPropGridSlider(_T("Alpha"), (_variant_t)(str), _T("Particle Begin Alpha입니다."), 0.f, 255.0f, 1000);
						
						m_vData = GetTarget()->Particlesystem()->GetEndColor();
						str.Format(L"%f", m_vData.x);
						Particle_EndColor_R = new CPropGridSlider(_T("Red"), (_variant_t)(str), _T("Particle End Red입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.y);
						Particle_EndColor_G = new CPropGridSlider(_T("Green"), (_variant_t)(str), _T("Particle End Green입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.z);
						Particle_EndColor_B = new CPropGridSlider(_T("Blue"), (_variant_t)(str), _T("Particle End Blue입니다."), 0.f, 10.f, 1000);
						str.Format(L"%f", m_vData.w);
						Particle_EndColor_A = new CPropGridSlider(_T("Alpha"), (_variant_t)(str), _T("Particle End Alpha입니다."), 0.f, 255.0f, 1000);


						Particle_StartColor->AddSubItem(Particle_StartColor_R);
						Particle_StartColor->AddSubItem(Particle_StartColor_G);
						Particle_StartColor->AddSubItem(Particle_StartColor_B);
						Particle_StartColor->AddSubItem(Particle_StartColor_A);
						Particle_EndColor->AddSubItem(Particle_EndColor_R);
						Particle_EndColor->AddSubItem(Particle_EndColor_G);
						Particle_EndColor->AddSubItem(Particle_EndColor_B);
						Particle_EndColor->AddSubItem(Particle_EndColor_A);

						m_ParticleSystem->Expand(FALSE);
						m_ParticleSystem->Expand();
					}
					else
					{
						UpdateCtrl_ParticleSystem();
					}
					break;
				case COMPONENT_TYPE::END:
					break;
				case COMPONENT_TYPE::SCRIPT:
					break;
				default:
					break;
				}
			}
		}
	}
	else
	{
		if (m_pPrevTarget != nullptr && !m_pPrevTarget->Light3D() && !m_pPrevTarget->Camera() && !m_pPrevTarget->Terrain())
		{
			if (m_pPrevTarget->MeshRender())
			{
				int MaterialContainerSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

				for (size_t i = 0; i < MaterialContainerSize2; ++i)
				{
					int MaterialSubSetSize2 = (((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

					for (size_t j = 0; j < MaterialSubSetSize2; ++j)
					{
						(((CMeshRender*)(m_pPrevTarget->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3DShader"));
					}
				}
			}
		}

		if (!TransformDisable)
		{
			// Name
			m_PropertyName->SetValue((_variant_t)_T("이름"));
			m_PropertyLayer->SetValue((_variant_t)_T("Layer"));

			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Z);

			m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
			m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
			m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Position);
			m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Scale);
			m_PropertyTransform->RemoveSubItem(m_PropertyTransform_Rotation);

			TransformDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			m_PropertyTransform->AddSubItem(TransformDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);

			//제일 아래로 초기화
		}
		if (!MeshRendererDisable)
		{
			m_PropertyMesh_MeshTitle->RemoveSubItem(m_PropertyMesh_Mesh);
			m_PropertyMesh_MaterialTitle->RemoveSubItem(m_PropertyMesh_Material);

			m_PropertyMesh_Renderer->RemoveSubItem(m_PropertyMesh_MeshTitle);
			m_PropertyMesh_Renderer->RemoveSubItem(m_PropertyMesh_MaterialTitle);

			MeshRendererDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			m_PropertyMesh_Renderer->AddSubItem(MeshRendererDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!Offset2DDisable)
		{
			Offset2D_Pos->RemoveSubItem(Offset2D_PosX);
			Offset2D_Pos->RemoveSubItem(Offset2D_PosY);

			Offset2D_Scale->RemoveSubItem(Offset2D_ScaleX);
			Offset2D_Scale->RemoveSubItem(Offset2D_ScaleY);

			m_PropertyCollider2D->RemoveSubItem(Offset2D_Pos);
			m_PropertyCollider2D->RemoveSubItem(Offset2D_Scale);


			Offset2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			Offset2DDisable->AddOption(L"Enable");
			m_PropertyCollider2D->AddSubItem(Offset2DDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!Offset3DDisable)
		{
			Offset3D_Pos->RemoveSubItem(Offset3D_PosX);
			Offset3D_Pos->RemoveSubItem(Offset3D_PosY);
			Offset3D_Pos->RemoveSubItem(Offset3D_PosZ);

			Offset3D_Scale->RemoveSubItem(Offset3D_ScaleX);
			Offset3D_Scale->RemoveSubItem(Offset3D_ScaleY);
			Offset3D_Scale->RemoveSubItem(Offset3D_ScaleZ);

			m_PropertyCollider3D->RemoveSubItem(Offset3D_Pos);
			m_PropertyCollider3D->RemoveSubItem(Offset3D_Scale);


			Offset3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			Offset3DDisable->AddOption(L"Enable");
			m_PropertyCollider3D->AddSubItem(Offset3DDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!Anim2DDisable)
		{
			Anim2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			Anim2DDisable->AddOption(L"Enable");
			m_Animator2D->AddSubItem(Anim2DDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!Anim3DDisable)
		{
			Anim3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			Anim3DDisable->AddOption(L"Enable");
			m_Animator3D->AddSubItem(Anim3DDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!LightDisable)
		{
			LightDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			LightDisable->AddOption(L"Enable");
			m_LightScript->AddSubItem(LightDisable);

			m_LightScript->Expand(FALSE);
			m_LightScript->Expand(TRUE);
		}
		if (!TerrainDisable)
		{
			m_Terrain_Tessellation->RemoveSubItem(m_Terrain_TessEnable);
			m_Terrain_Brush->RemoveSubItem(m_Terrain_BrushType);
			m_Terrain_Landscape->RemoveSubItem(m_Terrain_LandscapingState);

			m_Terrain->RemoveSubItem(m_Terrain_Tessellation);
			m_Terrain->RemoveSubItem(m_Terrain_Brush);
			m_Terrain->RemoveSubItem(m_Terrain_Landscape);

			TerrainDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
			m_Terrain->AddSubItem(TerrainDisable);

			m_Terrain->Expand(FALSE);
			m_Terrain->Expand(TRUE);
		}
	}

	/*if (m_pPrevTarget != m_pTarget)
	{
		m_LightScript->Expand(FALSE);
		m_LightScript->Expand();
	}*/
	m_pPrevTarget = m_pTarget;

}
void CPropertiesWnd::ReSetParticle()
{
	CString str;
	float m_fData;
	Vec4 m_vData;

	{
		m_ParticleSystem->RemoveSubItem(ParticleFrequencyTitle);
	}
	{
		m_ParticleSystem->RemoveSubItem(ParticleLifeTime);
	}
	{
		m_ParticleSystem->RemoveSubItem(ParticleSpeed);
	}
	{
		m_ParticleSystem->RemoveSubItem(ParticleSacle);
	}
	{
		m_ParticleSystem->RemoveSubItem(ParticleColor);
	}




	ParticleFrequencyTitle = new CMFCPropertyGridProperty(_T("Particle Time"));
	ParticleLifeTime = new CMFCPropertyGridProperty(_T("Particle Life"));
	ParticleSpeed = new CMFCPropertyGridProperty(_T("Particle Pace"));
	ParticleSacle = new CMFCPropertyGridProperty(_T("Particle Sacle"));
	ParticleColor = new CMFCPropertyGridProperty(_T("Particle Color"));

	m_ParticleSystem->AddSubItem(ParticleFrequencyTitle);
	m_ParticleSystem->AddSubItem(ParticleColor);
	m_ParticleSystem->AddSubItem(ParticleLifeTime);
	m_ParticleSystem->AddSubItem(ParticleSpeed);
	m_ParticleSystem->AddSubItem(ParticleSacle);

	//	m_ParticleFile = new CMFCPropertyGridFileProperty(_T("폴더"), _T("c:\\"));
	static const TCHAR szFilter[] = _T("아이콘 파일(*.png)|*.png|모든 파일(*.*)|*.*||");
	m_ParticleFile = new CMFCPropertyGridFileProperty(_T("Particle Mtrl"), TRUE, _T(""), _T("png"), 0, szFilter, _T("Particle mtrl을 지정합니다."));

	PARTICLE_TYPE ParticleType = GetTarget()->Particlesystem()->GetParticleType();
	if (ParticleType == PARTICLE_TYPE::EXPLOSION)
	{
		str = L"Explosion";
	}
	else if (ParticleType == PARTICLE_TYPE::FLUTTER)
	{
		str = L"Flutter";
	}
	else if (ParticleType == PARTICLE_TYPE::FALL)
	{
		str = L"Fall";
	}
	m_ParticleType = new CMFCPropertyGridProperty(_T("Particle Type"), (_variant_t)(str), _T("사용할 Brush의 종류입니다."));
	m_ParticleType->AddOption(L"Explosion");
	m_ParticleType->AddOption(L"Flutter");
	m_ParticleType->AddOption(L"Fall");

	m_fData = GetTarget()->Particlesystem()->GetFrequency();
	str.Format(L"%f", m_fData);
	m_ParticleFrequncy = new CPropGridSlider(_T("Frequncy"), (_variant_t)(str), _T("생성 주기입니다."), 0.00001f, 1.0f, 0.0001);

	m_fData = GetTarget()->Particlesystem()->GetMinLifeTime();
	str.Format(L"%f", m_fData);
	Particle_MinimumLifeTime = new CPropGridSlider(_T("Min Life"), (_variant_t)(str), _T("Particle 최소 생존 시간입니다."), 0.f, 100.f, 1000);

	m_fData = GetTarget()->Particlesystem()->GetMaxLifeTime();
	str.Format(L"%f", m_fData);
	Particle_MaximumLifeTime = new CPropGridSlider(_T("Max Life"), (_variant_t)(str), _T("Particle 최대 생존 시간입니다."), 0.f, 100.f, 1000);

	m_fData = GetTarget()->Particlesystem()->GetMinSpeed();
	str.Format(L"%f", m_fData);
	Particle_MinSpeed = new CPropGridSlider(_T("Begin Pace"), (_variant_t)(str), _T("Particle 시작 속도입니다."), 0.f, 500.0f, 1000);

	m_fData = GetTarget()->Particlesystem()->GetMaxSpeed();
	str.Format(L"%f", m_fData);
	Particle_MaxSpeed = new CPropGridSlider(_T("End Pace"), (_variant_t)(str), _T("Particle 종료 속도입니다."), 0.f, 500.0f, 1000);

	m_fData = GetTarget()->Particlesystem()->GetStartScale();
	str.Format(L"%f", m_fData);
	Particle_StartScale = new CPropGridSlider(_T("Begin Scale"), (_variant_t)(str), _T("particle 시작 크기입니다."), 0.f, 500.0f, 1000);

	m_fData = GetTarget()->Particlesystem()->GetEndScale();
	str.Format(L"%f", m_fData);
	Particle_EndScale = new CPropGridSlider(_T("End Scale"), (_variant_t)(str), _T("Particle 종료 크기입니다."), 0.f, 500.0f, 1000);

	Particle_StartColor = new CMFCPropertyGridProperty(_T("Begin Color"));
	Particle_EndColor = new CMFCPropertyGridProperty(_T("End Color"));

	ParticleFrequencyTitle->AddSubItem(m_ParticleFile);
	ParticleFrequencyTitle->AddSubItem(m_ParticleType);
	ParticleFrequencyTitle->AddSubItem(m_ParticleFrequncy);
	ParticleColor->AddSubItem(Particle_StartColor);
	ParticleColor->AddSubItem(Particle_EndColor);

	ParticleLifeTime->AddSubItem(Particle_MinimumLifeTime);
	ParticleLifeTime->AddSubItem(Particle_MaximumLifeTime);
	ParticleSpeed->AddSubItem(Particle_MinSpeed);
	ParticleSpeed->AddSubItem(Particle_MaxSpeed);
	ParticleSacle->AddSubItem(Particle_StartScale);
	ParticleSacle->AddSubItem(Particle_EndScale);

	m_vData = GetTarget()->Particlesystem()->GetStartColor();
	str.Format(L"%f", m_vData.x);
	Particle_StartColor_R = new CPropGridSlider(_T("Red"), (_variant_t)(str), _T("Particle Begin Red입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.y);
	Particle_StartColor_G = new CPropGridSlider(_T("Green"), (_variant_t)(str), _T("Particle Begin Green입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.z);
	Particle_StartColor_B = new CPropGridSlider(_T("Blue"), (_variant_t)(str), _T("Particle Begin Blue입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.w);
	Particle_StartColor_A = new CPropGridSlider(_T("Alpha"), (_variant_t)(str), _T("Particle Begin Alpha입니다."), 0.f, 255.0f, 1000);

	m_vData = GetTarget()->Particlesystem()->GetEndColor();
	str.Format(L"%f", m_vData.x);
	Particle_EndColor_R = new CPropGridSlider(_T("Red"), (_variant_t)(str), _T("Particle End Red입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.y);
	Particle_EndColor_G = new CPropGridSlider(_T("Green"), (_variant_t)(str), _T("Particle End Green입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.z);
	Particle_EndColor_B = new CPropGridSlider(_T("Blue"), (_variant_t)(str), _T("Particle End Blue입니다."), 0.f, 10.f, 1000);
	str.Format(L"%f", m_vData.w);
	Particle_EndColor_A = new CPropGridSlider(_T("Alpha"), (_variant_t)(str), _T("Particle End Alpha입니다."), 0.f, 255.0f, 1000);


	Particle_StartColor->AddSubItem(Particle_StartColor_R);
	Particle_StartColor->AddSubItem(Particle_StartColor_G);
	Particle_StartColor->AddSubItem(Particle_StartColor_B);
	Particle_StartColor->AddSubItem(Particle_StartColor_A);
	Particle_EndColor->AddSubItem(Particle_EndColor_R);
	Particle_EndColor->AddSubItem(Particle_EndColor_G);
	Particle_EndColor->AddSubItem(Particle_EndColor_B);
	Particle_EndColor->AddSubItem(Particle_EndColor_A);

	m_ParticleSystem->Expand(FALSE);
	m_ParticleSystem->Expand();
}
CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar 메시지 처리기

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == nullptr || (AfxGetMainWnd() != nullptr && AfxGetMainWnd()->IsIconic()))
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//	int cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	//	m_wndObjectCombo.SetWindowPos(nullptr, rectClient.left, rectClient.top, rectClient.Width(), m_nComboHeight, SWP_NOACTIVATE | SWP_NOZORDER);
	//	m_wndToolBar.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	m_wndPropList.SetWindowPos(nullptr, rectClient.left, rectClient.top + m_nComboHeight + 0, rectClient.Width(), rectClient.Height() - (m_nComboHeight + 0), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 콤보 상자를 만듭니다.
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	//if (!m_wndObjectCombo.Create(dwViewStyle, rectDummy, this, 1))
	//{
	//	TRACE0("속성 콤보 상자를 만들지 못했습니다. \n");
	//	return -1;      // 만들지 못했습니다.
	//}

	//m_wndObjectCombo.AddString(_T("응용 프로그램"));
	//m_wndObjectCombo.AddString(_T("속성 창"));
	//m_wndObjectCombo.SetCurSel(0);

	CRect rectCombo;
	//	m_wndObjectCombo.GetClientRect (&rectCombo);

	m_nComboHeight = rectCombo.Height();

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("속성 표를 만들지 못했습니다. \n");
		return -1;      // 만들지 못했습니다.
	}

	InitPropList();

	//	m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, IDR_PROPERTIES);
	//	m_wndToolBar.LoadToolBar(IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);
	//	m_wndToolBar.CleanUpLockedImages();
	//	m_wndToolBar.LoadBitmap(theApp.m_bHiColorIcons ? IDB_PROPERTIES_HC : IDR_PROPERTIES, 0, 0, TRUE /* 잠금 */);

	//	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
	//	m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
	//	m_wndToolBar.SetOwner(this);

		// 모든 명령은 부모 프레임이 아닌 이 컨트롤을 통해 라우팅됩니다.
	//	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();

	m_PrevActive[(UINT)COMPONENT_TYPE::TRANSFORM] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::MESHRENDER] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::CAMERA] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::COLLIDER2D] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::COLLIDER3D] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::ANIMATOR2D] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::LIGHT2D] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::LIGHT3D] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::TERRAIN] = true;
	m_PrevActive[(UINT)COMPONENT_TYPE::PARTICLESYSTEM] = true;


	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* /* pCmdUI */)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CPropertiesWnd::InitPropList()
{	//UI 테마
	m_wndPropList.SetCustomColors(RGB(35, 35, 35), RGB(255, 255, 255), RGB(60, 60, 60), RGB(170, 180, 180), RGB(30, 30, 30), RGB(255, 255, 255), RGB(35, 35, 35));
	//배경			//글자색			     //그룹배경		  //Layer그룹 글씨		    //아래 x				   			//라인

//	m_wndPropList.SetCustomColors(RGB(250, 250, 255), RGB(35, 35, 35), RGB(230, 230, 245), RGB(50, 60, 60), RGB(245, 245, 255), RGB(35, 35, 35), RGB(250, 250, 255));	//white
	//배경			//글자색			     //그룹배경		  //Layer그룹 글씨		    //아래 x				   			//라인

//SetBkColor(RGB(255, 255, 255));
//SetLineColor(RGB(125, 125, 125));
//SetTextColor(RGB(35, 35, 35));

	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	// Name
	m_PropertyName = new CMFCPropertyGridProperty(_T("Name"), (_variant_t)_T("이름"), _T("현재 Object의 이름를 표시합니다."));
	m_wndPropList.AddProperty(m_PropertyName);

	// Layer
	m_PropertyLayerTitle = new CMFCPropertyGridProperty(_T("Layer"));
	m_PropertyLayer = new CMFCPropertyGridProperty(_T("Layer"), (_variant_t)_T("Layer"), _T("현재 Layer의 정보를 표시합니다."));
	m_PropertyLayerTitle->AddSubItem(m_PropertyLayer);

	m_wndPropList.AddProperty(m_PropertyLayerTitle);


	// Transform
	m_PropertyTransform = new CMFCPropertyGridProperty(_T("Transform"));
	TransformDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	TransformDisable->AddOption(L"Enable");
	m_PropertyTransform->AddSubItem(TransformDisable);
	m_wndPropList.AddProperty(m_PropertyTransform);

	//Mesh Render
	m_PropertyMesh_Renderer = new CMFCPropertyGridProperty(_T("Mesh Renderer"));
	MeshRendererDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	MeshRendererDisable->AddOption(L"Enable");
	m_PropertyMesh_Renderer->AddSubItem(MeshRendererDisable);
	m_wndPropList.AddProperty(m_PropertyMesh_Renderer);

	//Camera
	m_Camera = new CMFCPropertyGridProperty(_T("Engine Camera"));
	CameraDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	CameraDisable->AddOption(L"Enable");
	m_Camera->AddSubItem(CameraDisable);
	m_wndPropList.AddProperty(m_Camera);

	// Collider 2D
	m_PropertyCollider2D = new CMFCPropertyGridProperty(_T("2D Collider"));
	Offset2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	Offset2DDisable->AddOption(L"Enable");
	m_PropertyCollider2D->AddSubItem(Offset2DDisable);
	m_wndPropList.AddProperty(m_PropertyCollider2D);

	m_PropertyCollider2D->Expand(TRUE);


	// Collider 3D
	m_PropertyCollider3D = new CMFCPropertyGridProperty(_T("3D Collider"));
	Offset3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	Offset3DDisable->AddOption(L"Enable");
	m_PropertyCollider3D->AddSubItem(Offset3DDisable);
	m_wndPropList.AddProperty(m_PropertyCollider3D);

	m_PropertyCollider3D->Expand(TRUE);


	//Animation 2D
	m_Animator2D = new CMFCPropertyGridProperty(_T("2D Animation"));
	Anim2DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	Anim2DDisable->AddOption(L"Enable");
	m_Animator2D->AddSubItem(Anim2DDisable);
	m_wndPropList.AddProperty(m_Animator2D);


	//Animation 3D
	m_Animator3D = new CMFCPropertyGridProperty(_T("3D Animation"));
	Anim3DDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	Anim3DDisable->AddOption(L"Enable");
	m_Animator3D->AddSubItem(Anim3DDisable);
	m_wndPropList.AddProperty(m_Animator3D);


	//Light
	m_LightScript = new CMFCPropertyGridProperty(_T("Light"));

	LightDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	LightDisable->AddOption(L"Enable");
	m_LightScript->AddSubItem(LightDisable);
	m_wndPropList.AddProperty(m_LightScript);
	m_LightScript->Expand(TRUE);

	//Terrain
	m_Terrain = new CMFCPropertyGridProperty(_T("Landscape"));

	TerrainDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	TerrainDisable->AddOption(L"Enable");
	m_Terrain->AddSubItem(TerrainDisable);
	m_wndPropList.AddProperty(m_Terrain);
	m_Terrain->Expand(TRUE);

	//ParticleSystem
	m_ParticleSystem = new CMFCPropertyGridProperty(_T("Particle"));

	ParticleDisable = new CMFCPropertyGridProperty(_T("Component"), (_variant_t)_T("Disable"), _T("현재 해당 Component를 가지고 있지 않습니다."));
	ParticleDisable->AddOption(L"Enable");
	m_ParticleSystem->AddSubItem(ParticleDisable);
	m_wndPropList.AddProperty(m_ParticleSystem);
	m_ParticleSystem->Expand(TRUE);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
	//	m_wndObjectCombo.SetFont(&m_fntPropList);
}

void CPropertiesWnd::UpdateCtrl_Transform()
{
	CString str;

	str = m_pTarget->GetName().c_str();
	m_PropertyName->SetValue((_variant_t)str);
	str = m_pTarget->GetLayerName().c_str();
	m_PropertyLayer->SetValue((_variant_t)str);

	arrTrans[0] = { GetTarget()->Transform()->GetWorldPos() };
	arrTrans[1] = { GetTarget()->Transform()->GetLocalScale() };
	arrTrans[2] = { GetTarget()->Transform()->GetLocalRot() };

	arrTrans[2] = arrTrans[2] / XM_PI * 180.f;


	// Position
	if (m_PropertyTransform_Position_X->IsSelected() == false && m_PropertyTransform_Position_Y->IsSelected() == false && m_PropertyTransform_Position_Z->IsSelected() == false)
	{
		if (PrevTrans[0][0] != arrTrans[0][0])
		{
			str.Format(L"%f", arrTrans[0][0]);
			m_PropertyTransform_Position_X->SetValue((_variant_t)(str));
		}

		arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
		str = m_PropertyTransform_Position_X->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[0][0]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			arrTrans[0][0] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position_X->SetValue((_variant_t)(str));
		}
	}
	else
	{
		if (m_PropertyTransform_Position_X->IsModified())
		{
			str = m_PropertyTransform_Position_X->GetValue();
			arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
			arrTrans[0][0] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Z);


			str.Format(L"%f", arrTrans[0][0]);
			m_PropertyTransform_Position_X = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)(str), _T("Position X 입니다."));
			str.Format(L"%f", arrTrans[0][1]);
			m_PropertyTransform_Position_Y = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)(str), _T("Position Y 입니다."));
			str.Format(L"%f", arrTrans[0][2]);
			m_PropertyTransform_Position_Z = new CMFCPropertyGridProperty(_T("Position Z"), (_variant_t)(str), _T("Position Z 입니다."));
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Z);

			m_PropertyTransform_Position->Expand(FALSE);
			m_PropertyTransform_Position->Expand(TRUE);
		}
	}

	if (m_PropertyTransform_Position_X->IsSelected() == false && m_PropertyTransform_Position_Y->IsSelected() == false && m_PropertyTransform_Position_Z->IsSelected() == false)
	{
		if (PrevTrans[0][1] != arrTrans[0][1])
		{
			str.Format(L"%f", arrTrans[0][1]);
			m_PropertyTransform_Position_Y->SetValue((_variant_t)(str));
		}

		arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
		str = m_PropertyTransform_Position_Y->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[0][1]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			arrTrans[0][1] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position_Y->SetValue((_variant_t)(str));

		}
	}
	else
	{
		if (m_PropertyTransform_Position_Y->IsModified())
		{
			str = m_PropertyTransform_Position_Y->GetValue();
			arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
			arrTrans[0][1] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Z);


			str.Format(L"%f", arrTrans[0][0]);
			m_PropertyTransform_Position_X = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)(str), _T("Position X 입니다."));
			str.Format(L"%f", arrTrans[0][1]);
			m_PropertyTransform_Position_Y = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)(str), _T("Position Y 입니다."));
			str.Format(L"%f", arrTrans[0][2]);
			m_PropertyTransform_Position_Z = new CMFCPropertyGridProperty(_T("Position Z"), (_variant_t)(str), _T("Position Z 입니다."));
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Z);

			m_PropertyTransform_Position->Expand(FALSE);
			m_PropertyTransform_Position->Expand(TRUE);
		}
	}
	if (m_PropertyTransform_Position_X->IsSelected() == false && m_PropertyTransform_Position_Y->IsSelected() == false && m_PropertyTransform_Position_Z->IsSelected() == false)
	{
		if (PrevTrans[0][2] != arrTrans[0][2])
		{
			str.Format(L"%f", arrTrans[0][2]);
			m_PropertyTransform_Position_Z->SetValue((_variant_t)(str));
		}

		arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
		str = m_PropertyTransform_Position_Z->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[0][2]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			arrTrans[0][2] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position_Z->SetValue((_variant_t)(str));

		}
	}
	else
	{
		if (m_PropertyTransform_Position_Z->IsModified())
		{
			str = m_PropertyTransform_Position_Z->GetValue();
			arrTrans[0] = m_pTarget->Transform()->GetWorldPos();
			arrTrans[0][2] = _tstof(str);
			m_pTarget->Transform()->SetLocalPos(arrTrans[0]);

			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->RemoveSubItem(m_PropertyTransform_Position_Z);


			str.Format(L"%f", arrTrans[0][0]);
			m_PropertyTransform_Position_X = new CMFCPropertyGridProperty(_T("Position X"), (_variant_t)(str), _T("Position X 입니다."));
			str.Format(L"%f", arrTrans[0][1]);
			m_PropertyTransform_Position_Y = new CMFCPropertyGridProperty(_T("Position Y"), (_variant_t)(str), _T("Position Y 입니다."));
			str.Format(L"%f", arrTrans[0][2]);
			m_PropertyTransform_Position_Z = new CMFCPropertyGridProperty(_T("Position Z"), (_variant_t)(str), _T("Position Z 입니다."));
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_X);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Y);
			m_PropertyTransform_Position->AddSubItem(m_PropertyTransform_Position_Z);

			m_PropertyTransform_Position->Expand(FALSE);
			m_PropertyTransform_Position->Expand(TRUE);
		}
	}






	// Scale
	if (m_PropertyTransform_Scale_X->IsSelected() == false && m_PropertyTransform_Scale_Y->IsSelected() == false && m_PropertyTransform_Scale_Z->IsSelected() == false)
	{
		if (PrevTrans[1][0] != arrTrans[1][0])
		{
			str.Format(L"%f", arrTrans[1][0]);
			m_PropertyTransform_Scale_X->SetValue((_variant_t)(str));
		}

		arrTrans[1] = m_pTarget->Transform()->GetLocalScale();
		str = m_PropertyTransform_Scale_X->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[1][0]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			if (_tstof(str) > 0)
			{
				arrTrans[1][0] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][0] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}

			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Scale_X->IsModified())
		{
			str = m_PropertyTransform_Scale_X->GetValue();
			arrTrans[1] = m_pTarget->Transform()->GetLocalScale();

			if (_tstof(str) > 0)
			{
				arrTrans[1][0] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][0] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}

	if (m_PropertyTransform_Scale_X->IsSelected() == false && m_PropertyTransform_Scale_Y->IsSelected() == false && m_PropertyTransform_Scale_Z->IsSelected() == false)
	{
		if (PrevTrans[1][1] != arrTrans[1][1])
		{
			str.Format(L"%f", arrTrans[1][1]);
			m_PropertyTransform_Scale_Y->SetValue((_variant_t)(str));
		}

		arrTrans[1] = m_pTarget->Transform()->GetLocalScale();
		str = m_PropertyTransform_Scale_Y->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[1][1]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			if (_tstof(str) > 0)
			{
				arrTrans[1][1] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][1] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}

			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Scale_Y->IsModified())
		{
			str = m_PropertyTransform_Scale_Y->GetValue();
			arrTrans[1] = m_pTarget->Transform()->GetLocalScale();

			if (_tstof(str) > 0)
			{
				arrTrans[1][1] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][1] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}

			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}

	if (m_PropertyTransform_Scale_X->IsSelected() == false && m_PropertyTransform_Scale_Y->IsSelected() == false && m_PropertyTransform_Scale_Z->IsSelected() == false)
	{
		if (PrevTrans[1][2] != arrTrans[1][2])
		{
			str.Format(L"%f", arrTrans[1][2]);
			m_PropertyTransform_Scale_Z->SetValue((_variant_t)(str));
		}

		arrTrans[1] = m_pTarget->Transform()->GetLocalScale();
		str = m_PropertyTransform_Scale_Z->GetValue();
		if (fabs(COMPARED_INTEGER(arrTrans[1][2]) - COMPARED_INTEGER(_tstof(str))) > 2)
		{
			if (_tstof(str) > 0)
			{
				arrTrans[1][2] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][2] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}

			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Scale_Z->IsModified())
		{
			str = m_PropertyTransform_Scale_Z->GetValue();
			arrTrans[1] = m_pTarget->Transform()->GetLocalScale();

			if (_tstof(str) > 0)
			{
				arrTrans[1][2] = _tstof(str);
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}
			else
			{
				arrTrans[1][2] = 0.f;
				m_pTarget->Transform()->SetLocalScale(arrTrans[1]);

				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->RemoveSubItem(m_PropertyTransform_Scale_Z);


				str.Format(L"%f", arrTrans[1][0]);
				m_PropertyTransform_Scale_X = new CMFCPropertyGridProperty(_T("Scale X"), (_variant_t)(str), _T("Scale X 입니다."));
				str.Format(L"%f", arrTrans[1][1]);
				m_PropertyTransform_Scale_Y = new CMFCPropertyGridProperty(_T("Scale Y"), (_variant_t)(str), _T("Scale Y 입니다."));
				str.Format(L"%f", arrTrans[1][2]);
				m_PropertyTransform_Scale_Z = new CMFCPropertyGridProperty(_T("Scale Z"), (_variant_t)(str), _T("Scale Z 입니다."));

				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_X);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Y);
				m_PropertyTransform_Scale->AddSubItem(m_PropertyTransform_Scale_Z);
			}

			m_PropertyTransform_Scale->Expand(FALSE);
			m_PropertyTransform_Scale->Expand(TRUE);
		}
	}


	// Rotation
	if (m_PropertyTransform_Rotation_X->IsSelected() == false && m_PropertyTransform_Rotation_Y->IsSelected() == false && m_PropertyTransform_Rotation_Z->IsSelected() == false)
	{
		if (PrevTrans[2][0] != arrTrans[2][0] / 180.f * XM_PI)
		{
			str.Format(L"%f", arrTrans[2][0]);
			m_PropertyTransform_Rotation_X->SetValue((_variant_t)(str));
		}

		arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
		str = m_PropertyTransform_Rotation_X->GetValue();
		if (fabs(arrTrans[2][0] * _RADIAN - _tstof(str)) > 2.f)
		{
			arrTrans[2][0] = _tstof(str);
			arrTrans[2][0] = arrTrans[2][0] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Rotation_X->IsModified())
		{
			str = m_PropertyTransform_Rotation_X->GetValue();
			arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
			arrTrans[2][0] = _tstof(str);
			arrTrans[2][0] = arrTrans[2][0] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}

	if (m_PropertyTransform_Rotation_X->IsSelected() == false && m_PropertyTransform_Rotation_Y->IsSelected() == false && m_PropertyTransform_Rotation_Z->IsSelected() == false)
	{
		if (PrevTrans[2][1] != arrTrans[2][1])
		{
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y->SetValue((_variant_t)(str));
		}

		Vec3 Rot = m_pTarget->Transform()->GetLocalRot();

		arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
		str = m_PropertyTransform_Rotation_Y->GetValue();

		if (fabs(arrTrans[2][1] * _RADIAN - _tstof(str)) > 2.f)
		{
			arrTrans[2][1] = _tstof(str);
			arrTrans[2][1] = arrTrans[2][1] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Rotation_Y->IsModified())
		{
			str = m_PropertyTransform_Rotation_Y->GetValue();
			arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
			arrTrans[2][1] = _tstof(str);
			arrTrans[2][1] = arrTrans[2][1] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}

	if (m_PropertyTransform_Rotation_X->IsSelected() == false && m_PropertyTransform_Rotation_Y->IsSelected() == false && m_PropertyTransform_Rotation_Z->IsSelected() == false)
	{
		if (PrevTrans[2][2] != arrTrans[2][2])
		{
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z->SetValue((_variant_t)(str));
		}

		arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
		str = m_PropertyTransform_Rotation_Z->GetValue();
		if (fabs(arrTrans[2][2] * _RADIAN - _tstof(str)) > 2.f)
		{
			arrTrans[2][2] = _tstof(str);
			arrTrans[2][2] = arrTrans[2][2] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}
	else
	{
		if (m_PropertyTransform_Rotation_Z->IsModified())
		{
			str = m_PropertyTransform_Rotation_Z->GetValue();
			arrTrans[2] = m_pTarget->Transform()->GetLocalRot();
			arrTrans[2][2] = _tstof(str);
			arrTrans[2][2] = arrTrans[2][2] / 180.f * XM_PI;

			m_pTarget->Transform()->SetLocalRot(arrTrans[2]);

			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->RemoveSubItem(m_PropertyTransform_Rotation_Z);


			str.Format(L"%f", arrTrans[2][0] * _RADIAN);
			m_PropertyTransform_Rotation_X = new CMFCPropertyGridProperty(_T("Rotation X"), (_variant_t)(str), _T("Rotation X 입니다."));
			str.Format(L"%f", arrTrans[2][1] * _RADIAN);
			m_PropertyTransform_Rotation_Y = new CMFCPropertyGridProperty(_T("Rotation Y"), (_variant_t)(str), _T("Rotation Y 입니다."));
			str.Format(L"%f", arrTrans[2][2] * _RADIAN);
			m_PropertyTransform_Rotation_Z = new CMFCPropertyGridProperty(_T("Rotation Z"), (_variant_t)(str), _T("Rotation Z 입니다."));

			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_X);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Y);
			m_PropertyTransform_Rotation->AddSubItem(m_PropertyTransform_Rotation_Z);

			m_PropertyTransform_Rotation->Expand(FALSE);
			m_PropertyTransform_Rotation->Expand(TRUE);
		}
	}

	PrevTrans[0] = arrTrans[0];
	PrevTrans[1] = arrTrans[1];
	PrevTrans[2] = arrTrans[2];
	// 컨트롤 업데이트 확인
	CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_MeshRender()
{
	if (m_pPrevTarget != m_pTarget)
	{
		Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
		if (pMesh != nullptr)
		{
			strMesh = pMesh->GetName().c_str();
		}
		m_PropertyMesh_MeshData->SetValue((_variant_t)(strMesh));

		Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetSharedMaterial();
		if (pMtrl != nullptr)
		{
			strMtrl = pMtrl->GetName().c_str();
		}
		m_PropertyMesh_MaterialData->SetValue((_variant_t)(strMtrl));

	}
	else
	{
		if (PrevstrMesh != strMesh)
		{
			Ptr<CMesh> pMesh = GetTarget()->MeshRender()->GetMesh();
			if (pMesh != nullptr)
			{
				strMesh = pMesh->GetName().c_str();
			}
			m_PropertyMesh_MeshData->SetValue((_variant_t)(strMesh));

			MtrL_Origin = m_PropertyMesh_MeshData->GetValue().bstrVal; // Default 메쉬이름
		}

		if (PrevstrMtrl != strMtrl)
		{
			Ptr<CMaterial> pMtrl = GetTarget()->MeshRender()->GetSharedMaterial();
			if (pMtrl != nullptr)
			{
				strMtrl = pMtrl->GetName().c_str();
			}
			m_PropertyMesh_MaterialData->SetValue((_variant_t)(strMtrl));
		}
	}

	PrevstrMesh = strMesh;
	PrevstrMtrl = strMtrl;
	CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Camera()
{
	// Data Casting 과정
	COleVariant GridData;
	VARIANT castingClass;
	float CastedData;
	wstring GridDataString;
	wstring GridDataStringIf_;

	GridData = CameraOption->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	if (SCENE_MOD::SCENE_PLAY != CCore::GetInst()->GetSceneMod())
	{
		if (KEY_TAB(KEY_TYPE::KEY_ESC))
		{
			CameraOption->SetValue((_variant_t)(_T("Engine MOD")));
		}
		if (KEY_TAB(KEY_TYPE::KEY_F6))
		{
			CameraOption->SetValue((_variant_t)(_T("Client MOD")));
		}

		if (GridDataString == L"Engine MOD")
		{
			CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD = CAMERA_MOD::ENGINE_MOD;
		}
		else
		{
			CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD = CAMERA_MOD::CLIENT_MOD;
		}
	}
	else
	{
//		CameraOption->SetValue((_variant_t)(_T("Client MOD")));
		CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD = CAMERA_MOD::CLIENT_MOD;
	}

//	이 주석부분 안에 수정
//		현재 MOD를 표시하고, 선택시 바뀌도록
////////////////////////////////

	GridData = CameraOption->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	float OriginNear = GetTarget()->Camera()->GetNear();
	float OriginFar = GetTarget()->Camera()->GetFar();
	float OriginFOV = GetTarget()->Camera()->GetFOV();
	float OriginScale = GetTarget()->Camera()->GetScale();
	float OriginWidth = GetTarget()->Camera()->GetWidth();
	float OriginHeight = GetTarget()->Camera()->GetHeight();

	float ValueNear;
	float ValueFar;
	float ValueFOV;
	float ValueScale;
	float ValueWidth;
	float ValueHeight;

	//Weather
	GridData = WeatherEnable->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;
	GridDataStringIf_ = castingClass.bstrVal;
	
	if (GridDataString == L"Climate On")
		CSceneMgr::GetInst()->GetCurScene()->SetWeatherEnable(true);
	else
	{
		CSceneMgr::GetInst()->GetCurScene()->SetWeatherEnable(false);
		m_bWeatherEnable = false;
	}

	GridData = WeatherShaderTexture->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;
	_WeatherMtrl_Name = GridDataString;

	if (GridDataStringIf_ == L"Climate On")
	{
		if ((_WeatherMtrl_Name != _PrevWeatherMtrl_Name) && _WeatherMtrl_Name != L"" && m_bWeatherEnable)
		{
			wstring Str = CPathMgr::GetTexturePath(_WeatherMtrl_Name);
			CSceneMgr::GetInst()->GetCurScene()->SetWeatherTextureName(Str);
		
			_WeatherMtrl_Name = Str;

			WeatherShaderTexture->SetValue(Str.c_str());
			MessageBox(L"Please release to mouse focusing.");

			WeatherOption->Expand(FALSE);
			WeatherOption->Expand();
		}
		else if (m_bWeatherEnable == false)
		{
			WeatherShaderTexture->SetValue(L"");
			m_bWeatherEnable = true;

			WeatherOption->Expand(FALSE);
			WeatherOption->Expand();
		}
	}

	_PrevWeatherMtrl_Name = _WeatherMtrl_Name;

	//Near
	GridData = CameraNear->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueNear = CastedData;

	if (fabs(ValueNear - OriginNear) <= 0.01f)
	{
		m_pTarget->Camera()->SetNear(ValueNear);
	}

	//Far
	GridData = CameraFar->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueFar = CastedData;

	if (fabs(ValueFar - OriginFar) <= 0.01f)
	{
		m_pTarget->Camera()->SetFar(ValueFar);
	}

	//FOV
	GridData = CameraFOV->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueFOV = CastedData;

	if (fabs(ValueFOV - OriginFOV) <= 0.01f)
	{
		m_pTarget->Camera()->SetFOV(ValueFOV);
	}

	//Scale
	GridData = CameraScale->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueScale = CastedData;

	if (fabs(ValueScale - OriginScale) <= 0.01f)
	{
		m_pTarget->Camera()->SetScale(ValueScale);
	}

	//Width
	GridData = CameraWidth->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueWidth = CastedData;

	if (fabs(ValueWidth - OriginWidth) <= 0.01f)
	{
		m_pTarget->Camera()->SetWidth(ValueWidth);
	}

	//Height
	GridData = CameraHeight->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;
	ValueHeight = CastedData;

	if (fabs(ValueHeight - OriginHeight) <= 0.01f)
	{
		m_pTarget->Camera()->SetHeight(ValueHeight);
	}


	CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Collider2D()
{
}

void CPropertiesWnd::UpdateCtrl_Collider3D()
{
	CGameObject* Target = GetTarget();
}

void CPropertiesWnd::UpdateCtrl_Animator2D()
{
	//m_cbAnim.ResetContent();

	//const map<wstring, CAnimation2D*>& mapAnim = GetTarget()->Animator2D()->GetAnimations();
	//CAnimation2D* pCurAnim = GetTarget()->Animator2D()->GetCurAnimation();

	//map<wstring, CAnimation2D*>::const_iterator iter = mapAnim.begin();
	//int iIdx = 0;
	//for (int i = 0; iter != mapAnim.end(); ++iter, ++i)
	//{
	//	if (nullptr != pCurAnim && pCurAnim->GetName() == iter->first)
	//	{
	//		iIdx = i;
	//	}
	//	m_cbAnim.AddString(iter->first.c_str());
	//}

	//m_cbAnim.SetCurSel(iIdx);

	//CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Animator3D()
{
	std::vector<struct CAnimClip*>::iterator iter;
	std::vector<struct CAnimClip*>::iterator iterEnd;

	bool isAnimListChange = false;


	iter = m_pTarget->Animator3D()->GetVecAnimClip()->begin();
	iterEnd = m_pTarget->Animator3D()->GetVecAnimClip()->end();

	if (m_pTarget != m_pPrevTarget)
	{
		if (m_Animation3D != nullptr)
		{
			m_Animator3D->RemoveSubItem(m_Animation3D);
		}
		if (m_Animation3DCurAnim != nullptr)
		{
			m_Animator3D->RemoveSubItem(m_Animation3DCurAnim);
		}

		isAnimListChange = true;
	}


	if (Prev_Prev_AnimState != AnimState)
	{
		for (; iter != iterEnd; ++iter)
		{
			if (isAnimListChange)
			{
				if (m_Animation3D != nullptr)
				{
					m_Animation3D->AddOption((*iter)->GetClipName().c_str());
				}
			}

			wstring itername = (*iter)->GetClipName();
			wstring CurAnim = m_pTarget->Animator3D()->GetAnimClip()->GetClipName();
			if (itername != CurAnim)
			{
				continue;
			}
			else
			{
				AnimState = (*iter)->GetClipName();
				if (m_Animation3DCurAnim != nullptr)
				{
					m_Animation3DCurAnim->SetValue((*iter)->GetClipName().c_str());
				}
				break;
			}
		}

		Prev_Prev_AnimState = Prev_AnimState;
		Prev_AnimState = AnimState;

		if (Prev_Prev_AnimState == Prev_AnimState && Prev_AnimState == AnimState)
		{
			Prev_Prev_AnimState = L"";
		}
	}
}

void CPropertiesWnd::UpdateCtrl_Light2D()
{
	//Vec3 vColor = GetTarget()->Light2D()->GetLightColor();
	//float fRange = GetTarget()->Light2D()->GetLightRange();

	//CString str;
	//str.Format(L"%d", (BYTE)(vColor.x * 255.f));
	//m_R.SetWindowTextW(str);

	//str.Format(L"%d", (BYTE)(vColor.y * 255.f));
	//m_G.SetWindowTextW(str);

	//str.Format(L"%d", (BYTE)(vColor.z * 255.f));
	//m_B.SetWindowTextW(str);

	//str.Format(L"%.2f", fRange);
	//m_Range.SetWindowTextW(str);

	//CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Light3D()
{
	// Data Casting 과정
	COleVariant GridData;
	VARIANT castingClass;
	float CastedData;
	wstring GridDataString;


	GridData = m_LightType->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	// Light Type
	tLight3D LightInfo = m_pTarget->Light3D()->GetLightType();
	if (GridDataString == L"Light List")
	{
		if ((LIGHT_TYPE)LightInfo.iLightType == LIGHT_TYPE::DIR)
		{
			m_LightType->SetValue(L"Directional Light");
		}
		else if ((LIGHT_TYPE)LightInfo.iLightType == LIGHT_TYPE::POINT)
		{
			m_LightType->SetValue(L"Point Light");
		}

	}
	else
	{
		if (GridDataString == L"Directional Light")
		{
			m_pTarget->Light3D()->SetLightType(LIGHT_TYPE::DIR);
		}
		else if (GridDataString == L"Point Light")
		{
			m_pTarget->Light3D()->SetLightType(LIGHT_TYPE::POINT);
		}
	}


	// Lim Light

	Rim_Light = m_pTarget->Light3D()->GetRimLight();

	if (Rim_Light != Prev_Light)
	{
		if (m_pTarget->Light3D()->GetRimLight())		
			m_LightRim->SetValue(L"RimLight On");
		
		else		
			m_LightRim->SetValue(L"RimLight Off");
	}

	Prev_Light = Rim_Light;

	GridData = m_LightRim->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;


	if (GridDataString == L"RimLight On")
	{
		m_pTarget->Light3D()->SetRimLight(true);
	}
	else if (GridDataString == L"RimLight Off")
	{
		m_pTarget->Light3D()->SetRimLight(false);
	}

	//
	RimLightName = GridDataString;
	if (RimLightName != PrevRimLightName)
	{
		LightType3D->RemoveSubItem(m_LightRim);
		LightType3D->RemoveSubItem(m_LightRange);
		LightType3D->RemoveSubItem(m_LightInAngle);
		LightType3D->RemoveSubItem(m_LightOutAngle);

		CString str;
		if (m_pTarget->Light3D()->GetRimLight())
			str = L"RimLight On";
		else
			str = L"RimLight Off";

		m_LightRim = new CMFCPropertyGridProperty(_T("Rim Right"), (_variant_t)(str), _T("Rim Right 입니다."));
		m_LightRange = new CMFCPropertyGridProperty(_T("Range"), (_variant_t)_T("0"), _T("Light Range 입니다."));
		m_LightInAngle = new CMFCPropertyGridProperty(_T("InAngle"), (_variant_t)_T("0"), _T("InAngle 입니다."));
		m_LightOutAngle = new CMFCPropertyGridProperty(_T("OutAngle"), (_variant_t)_T("0"), _T("OutAngle 입니다."));

		m_LightRim->AddOption(L"RimLight On");
		m_LightRim->AddOption(L"RimLight Off");

		LightType3D->AddSubItem(m_LightRim);
		LightType3D->AddSubItem(m_LightRange);
		LightType3D->AddSubItem(m_LightInAngle);
		LightType3D->AddSubItem(m_LightOutAngle);

		LightType3D->Expand(FALSE);
		LightType3D->Expand();
	}	
	PrevRimLightName = RimLightName;
	//


	CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Script()
{
	//CString strScriptName = CScriptMgr::GetScriptName(m_pTargetScript);

	//m_ScriptName.SetWindowTextW(strScriptName);

	//CtrlUpdate();
}

void CPropertiesWnd::UpdateCtrl_Terrain()
{
	m_TerrainTarget = m_pTarget;

	// Data Casting 과정
	COleVariant GridData;
	VARIANT castingClass;
	float CastedData;
	wstring GridDataString;

	bool ActiveTess = false;

	// Tess Level Update
	Vec3 TessModif;
	Vec3 TessValue = GetTarget()->Terrain()->GetTessLevel();
	{
		//Min
		GridData = m_Terrain_TessMin->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;
		TessModif.x = CastedData;

		//Max
		GridData = m_Terrain_TessMax->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;
		TessModif.y = CastedData;

		//MaxLevel
		GridData = m_Terrain_TessPhase->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;
		TessModif.z = CastedData;

		TessValue.x *= .0001f;
		TessValue.y *= .0001f;

		if (fabs(TessModif.x - TessValue.x) <= 0.01f)
		{
			ActiveTess = true;
		}
		else if (fabs(TessModif.y - TessValue.y) <= 0.01f)
		{
			ActiveTess = true;
		}
		else if (fabs(TessModif.z - TessValue.z) <= 0.01f)
		{
			ActiveTess = true;
		}
	}

	if (ActiveTess == true)
	{
		if (TessModif.x > TessModif.y)
		{
			TessModif.x = TessModif.y;
		}

		TessModif.x *= 10000.f;
		TessModif.y *= 10000.f;

		GetTarget()->Terrain()->SetTessLevel(TessModif);
	}



	// Tess 옵션
	GridData = m_Terrain_TessEnable->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	if (GridDataString == L"Enable")
	{
		m_CurOption = TESS_OPTION::ENABLE;
		if (m_PrevOption != m_CurOption)
		{
			TessOptionEnable(m_TerrainTarget);
			m_pTarget->Terrain()->SetTerrainOption(TERRAIN_OPTION::ENABLE);
		}
	}
	else
	{
		m_CurOption = TESS_OPTION::DISABLE;
		if (m_PrevOption != m_CurOption)
		{
			TessOptionDisable(m_TerrainTarget);
			m_pTarget->Terrain()->SetTerrainOption(TERRAIN_OPTION::DISABLE);
		}
	}


	if (SCENE_MOD::SCENE_PLAY == CCore::GetInst()->GetSceneMod() && m_CurOption == TESS_OPTION::ENABLE)
	{
		m_CurOption = TESS_OPTION::DISABLE;
		TessOptionDisable(m_TerrainTarget);
	}

	if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod() && GridDataString == L"Enable")
	{
		m_CurOption = TESS_OPTION::ENABLE;
		TessOptionEnable(m_TerrainTarget);
	}

	m_PrevOption = m_CurOption;

	// Data Casting 과정
	GridData = m_Terrain_BrushScale->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	//
	if (KEY_HOLD(KEY_TYPE::KEY_UP))
	{
		CastedData += 10.f * DT;
		m_Terrain_BrushScale->SetValue((_variant_t)(CastedData));
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_DOWN))
	{
		CastedData -= 10.f * DT;
		m_Terrain_BrushScale->SetValue((_variant_t)(CastedData));
	}
	//

	//Brush Scale 설정
	CastedData *= 0.01f;
	GetTarget()->Terrain()->SetBrushScale(CastedData);



	/*-------------------------------------*/
	GridData = m_Terrain_BrushType->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	if (GridDataString == L"Ink")
	{
		GetTarget()->Terrain()->SetBrushType(BRUSH_TYPE::INK);
	}
	else
	{
		GetTarget()->Terrain()->SetBrushType(BRUSH_TYPE::CIRCLE);
	}

	GridData = m_Terrain_LandscapingState->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	if (GridDataString == L"Disable")
	{
		GetTarget()->Terrain()->SetSplatting(BRUSH_STATE::DISABLE);
	}
	else if (GridDataString == L"Height Map" && (KEY_HOLD(KEY_TYPE::KEY_M) == false))
	{
		GetTarget()->Terrain()->SetSplatting(BRUSH_STATE::HEIGHT_MAP);
	}
	else if (GridDataString == L"Weight Map" && (KEY_HOLD(KEY_TYPE::KEY_H) == false))
	{
		GetTarget()->Terrain()->SetSplatting(BRUSH_STATE::WEIGHT_MAP);
	}


	if (KEY_HOLD(KEY_TYPE::KEY_M) && GridDataString != L"Disable")
	{
		GetTarget()->Terrain()->SetSplatting(BRUSH_STATE::WEIGHT_MAP);
	}
	else if (KEY_HOLD(KEY_TYPE::KEY_H) && GridDataString != L"Disable")
	{
		GetTarget()->Terrain()->SetSplatting(BRUSH_STATE::HEIGHT_MAP);
	}

	COLORREF GridColorData = m_Terrain_Splatting_Material->GetColor(); //32896

	switch (GridColorData)
	{
	case 0:
		GetTarget()->Terrain()->SetLandscape_Material(LANDSCAPE_MATERIAL::DISABLE);
		break;
	case 128:
		GetTarget()->Terrain()->SetLandscape_Material(LANDSCAPE_MATERIAL::TILE1);
		break;
	case 32768:
		GetTarget()->Terrain()->SetLandscape_Material(LANDSCAPE_MATERIAL::TILE3);
		break;
	case 32896:
		GetTarget()->Terrain()->SetLandscape_Material(LANDSCAPE_MATERIAL::TILE2);
		break;
	case 8388608:
		GetTarget()->Terrain()->SetLandscape_Material(LANDSCAPE_MATERIAL::TILE3);
		break;
	default:
		break;
	}
}

void CPropertiesWnd::UpdateCtrl_ParticleSystem()
{
	// Reset Particle Value
	if (m_pTarget != m_pPrevTarget) // 갱신
	{		
		ReSetParticle();		
	}

	COleVariant GridData;
	VARIANT castingClass;
	float CastedData;
	wstring GridDataString;

	// Particle Material
	GridData = m_ParticleFile->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;
	_ParticleMtrl_Name = GridDataString;

//	_ParticleMtrl_Name = m_pTarget->Particlesystem()->GetParticleName();

	if ((_ParticleMtrl_Name != _PrevParticleMtrl_Name) && _ParticleMtrl_Name != L"")
	{
		wstring Str = CPathMgr::GetTexturePath(_ParticleMtrl_Name);
		m_pTarget->Particlesystem()->SetParticleMtrl(&Str);

		m_ParticleFile->SetValue(Str.c_str());
		_ParticleMtrl_Name = Str;
		MessageBox(L"Please release to mouse focusing.");
	}

	_PrevParticleMtrl_Name = _ParticleMtrl_Name;

	// ParticleType
	GridData = m_ParticleType->GetValue();
	castingClass = GridData.Detach();
	GridDataString = castingClass.bstrVal;

	if (GridDataString == L"Explosion")
	{
		m_pTarget->Particlesystem()->SetParticleType(PARTICLE_TYPE::EXPLOSION);
	}
	else if (GridDataString == L"Flutter")
	{
		m_pTarget->Particlesystem()->SetParticleType(PARTICLE_TYPE::FLUTTER);
	}
	else if (GridDataString == L"Fall")
	{
		m_pTarget->Particlesystem()->SetParticleType(PARTICLE_TYPE::FALL);
	}

	// ParticleFrequncy
	GridData = m_ParticleFrequncy->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetFrequency(CastedData);

	// Particle MinLifeTime
	GridData = Particle_MinimumLifeTime->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetMinLifeTime(CastedData);

	// Particle MaxLifeTime
	GridData = Particle_MaximumLifeTime->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetMaxLifeTime(CastedData);

	// Particle MinSpeed
	GridData = Particle_MinSpeed->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetMinSpeed(CastedData);

	// Particle MaxSpeed
	GridData = Particle_MaxSpeed->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetMaxSpeed(CastedData);

	// Particle Start Scale
	GridData = Particle_StartScale->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetStartScale(CastedData);

	// Particle End Scale
	GridData = Particle_EndScale->GetValue();
	castingClass = GridData.Detach();
	CastedData = (float)castingClass.fltVal;

	m_pTarget->Particlesystem()->SetEndScale(CastedData);

	
	// Particle Start Color
	{
		GridData = Particle_StartColor_R->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetStartColor_Red(CastedData);

		GridData = Particle_StartColor_G->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetStartColor_Green(CastedData);

		GridData = Particle_StartColor_B->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetStartColor_Blue(CastedData);

		GridData = Particle_StartColor_A->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetStartColor_Alpha(CastedData);
	}

	// Particle End Color
	{
		GridData = Particle_EndColor_R->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetEndColor_Red(CastedData);

		GridData = Particle_EndColor_G->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetEndColor_Green(CastedData);

		GridData = Particle_EndColor_B->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetEndColor_Blue(CastedData);

		GridData = Particle_EndColor_A->GetValue();
		castingClass = GridData.Detach();
		CastedData = (float)castingClass.fltVal;

		m_pTarget->Particlesystem()->SetEndColor_Alpha(CastedData);
	}

}

void CPropertiesWnd::TessOptionEnable(CGameObject* _TargetObj)
{
	int MaterialContainerSize = (((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

	for (size_t i = 0; i < MaterialContainerSize; ++i)
	{
		int MaterialSubSetSize = (((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

		for (size_t j = 0; j < MaterialSubSetSize; ++j)
		{
			(((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TerrainWireFrameShader"));
		}
	}
}

void CPropertiesWnd::TessOptionDisable(CGameObject* _TargetObj)
{
	int MaterialContainerSize = (((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());

	for (size_t i = 0; i < MaterialContainerSize; ++i)
	{
		int MaterialSubSetSize = (((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));

		for (size_t j = 0; j < MaterialSubSetSize; ++j)
		{
			(((CMeshRender*)(_TargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"TerrainShader"));
		}
	}
}
