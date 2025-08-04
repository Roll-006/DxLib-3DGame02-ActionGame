#include "shadow_map.hpp"

ShadowMap::ShadowMap() : 
	m_shadow_map_handle(MakeShadowMap(kSizeX, kSizeY))
{
	SetShadowMapLightDirection(m_shadow_map_handle, VGet(0.5f, -0.5f, 0.5f));

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadow_map_handle, kMinDrawPos, kMaxDrawPos);
}

ShadowMap::~ShadowMap()
{
	DeleteShadowMap(m_shadow_map_handle);
}

void ShadowMap::PrepareDrawShadowMap()
{
	ShadowMap_DrawSetup(m_shadow_map_handle);
}

void ShadowMap::EndDrawShadowMap()
{
	ShadowMap_DrawEnd();
}

void ShadowMap::UseShadowMap()
{
	SetUseShadowMap(0, m_shadow_map_handle);
}

void ShadowMap::UnuseShadowMap()
{
	SetUseShadowMap(0, -1);
}
