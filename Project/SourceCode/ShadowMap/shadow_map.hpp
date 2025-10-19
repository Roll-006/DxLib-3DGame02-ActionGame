#pragma once
#include <DxLib.h>
#include "../Base/single_instance_base.hpp"

#include "../Part/transform.hpp"
#include "../Event/event_system.hpp"

class ShadowMap final : public SingleInstanceBase<ShadowMap>
{
public:
	ShadowMap();
	~ShadowMap();

	void SetupDrawShadowMap();
	void EndDrawShadowMap();

	void UseShadowMap();
	void UnuseShadowMap();

	void CreateShadowMap(const ChangeSceneEvent& event);

private:
	static constexpr int	kSizeX			= 8192;
	static constexpr int	kSizeY			= 8192;
	static constexpr VECTOR kMinOffsetPos	= { -700.0f, -10.0f, -700.0f };
	static constexpr VECTOR kMaxOffsetPos	= {  700.0f, 300.0f,  700.0f };

	int		m_size_x;
	int		m_size_y;
	VECTOR	m_min_offset_pos;
	VECTOR	m_max_offset_pos;

	int m_shadow_map_handle;

	friend SingleInstanceBase<ShadowMap>;
};
