#include "shadow_map.hpp"

ShadowMap::ShadowMap() :
	m_size_x			(0),
	m_size_y			(0),
	m_min_offset_pos	(v3d::GetZeroV()),
	m_max_offset_pos	(v3d::GetZeroV()),
	m_shadow_map_handle	(MakeShadowMap(kSizeX, kSizeY))
{
	SetShadowMapLightDirection(m_shadow_map_handle, v3d::GetNormalizedV(VGet(0.0f, 1.5f, 1.0f)));

	// イベント登録
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>(this, &ShadowMap::CreateShadowMap);
}

ShadowMap::~ShadowMap()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>(this, &ShadowMap::CreateShadowMap);

	DeleteShadowMap(m_shadow_map_handle);
}

void ShadowMap::SetupDrawShadowMap()
{
	const auto center_pos	= GetCameraPosition();
	const auto min_pos		= center_pos + m_min_offset_pos;
	const auto max_pos		= center_pos + m_max_offset_pos;

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

void ShadowMap::CreateShadowMap(const ChangeSceneEvent& event)
{
	DeleteShadowMap(m_shadow_map_handle);
	m_shadow_map_handle = MakeShadowMap(kSizeX, kSizeY);

	switch (event.next_scene_kind)
	{
	case SceneKind::kTitle:
		m_min_offset_pos = { -50.0f,  0.0f, -50.0f };
		m_max_offset_pos = {  50.0f, 30.0f,  50.0f };
		SetShadowMapLightDirection(m_shadow_map_handle, v3d::GetNormalizedV(VGet(0.0f, 1.5f, 1.0f)));
		break;

	case SceneKind::kPlay:
		m_min_offset_pos = { -700.0f, -10.0f, -700.0f };
		m_max_offset_pos = {  700.0f, 300.0f,  700.0f };
		SetShadowMapLightDirection(m_shadow_map_handle, v3d::GetNormalizedV(VGet(0.5f, -0.5f, 0.5f)));
		break;

	default:
		break;
	}

	// TODO : 後に最も強い光源に影響を受けるように変更
}
