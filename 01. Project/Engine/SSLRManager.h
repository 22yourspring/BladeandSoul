#pragma once

#include "global.h"

class CSSLRManager
{
	SINGLE(CSSLRManager);
public:

	void Init(UINT width, UINT height);
	void Deinit();

	// Render the screen space light rays on top of the scene
	void Render(ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pLightAccumRTV, ID3D11ShaderResourceView* pMiniDepthSRV, const Vec3& vSunDir, const Vec3& vSunColor);
	
	ID3D11ShaderResourceView* m_pLightRaysSRV;
	ID3D11RenderTargetView* m_pLightRaysRTV;
	ID3D11Texture2D* m_pLightRaysTex;

private:

	// Prepare the occlusion map
	void PrepareOcclusion(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* pMiniDepthSRV);

	// Ray trace the occlusion map to generate the rays
	void RayTrace(ID3D11DeviceContext* pd3dImmediateContext, const Vec2& vSunPosSS, const Vec3& vSunColor);

	// Combine the rays with the scene
	void Combine(ID3D11DeviceContext* pd3dImmediateContext, ID3D11RenderTargetView* pLightAccumRTV);

	UINT m_nWidth;
	UINT m_nHeight;
	float m_fInitDecay;
	float m_fDistDecay;
	float m_fMaxDeltaLen;

	ID3D11Texture2D* m_pOcclusionTex;
	ID3D11UnorderedAccessView* m_pOcclusionUAV;
	ID3D11ShaderResourceView* m_pOcclusionSRV;

	
	

	// Shaders
	ID3D11Buffer* m_pOcclusionCB;
	ID3D11ComputeShader* m_pOcclusionCS;
	ID3D11Buffer* m_pRayTraceCB;
	ID3D11VertexShader* m_pFullScreenVS;
	ID3D11PixelShader* m_pRayTracePS;
	ID3D11PixelShader* m_pCombinePS;

	// Additive blend state to add the light rays on top of the scene lights
	ID3D11BlendState* m_pAdditiveBlendState;
};