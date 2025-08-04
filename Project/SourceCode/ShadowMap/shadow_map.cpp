#include "shadow_map.hpp"

ShadowMap::ShadowMap(const std::shared_ptr<Transform> camera_transform) :
	m_shadow_map_handle	(MakeShadowMap(kSizeX, kSizeY)),
	m_camera_transform	(camera_transform)
{
	SetShadowMapLightDirection(m_shadow_map_handle, VGet(0.5f, -0.5f, 0.5f));

}

ShadowMap::~ShadowMap()
{
	DeleteShadowMap(m_shadow_map_handle);
}

void ShadowMap::SetupDrawShadowMap()
{
	const auto center_pos	= m_camera_transform->GetPos(CoordinateKind::kWorld);
	const auto min_pos		= center_pos + kMinOffsetPos;
	const auto max_pos		= center_pos + kMaxOffsetPos;

	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadow_map_handle, min_pos, max_pos);

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
