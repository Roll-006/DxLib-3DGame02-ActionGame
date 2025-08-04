#pragma once
#include <DxLib.h>
#include "../Base/one_instance_singleton_base.hpp"

#include "../Part/transform.hpp"

class ShadowMap final : public OneInstanceSingletonBase<ShadowMap>
{
public:
	ShadowMap(const std::shared_ptr<Transform> camera_transform);
	~ShadowMap();

	void SetupDrawShadowMap();
	void EndDrawShadowMap();

	void UseShadowMap();
	void UnuseShadowMap();

private:
	static constexpr int	kSizeX			= 8192;
	static constexpr int	kSizeY			= 8192;
	static constexpr VECTOR kMinOffsetPos	= { -700.0f, -10.0f, -700.0f };
	static constexpr VECTOR kMaxOffsetPos	= { 700.0f, 100.0f, 700.0f };

	int m_shadow_map_handle;
	std::shared_ptr<Transform> m_camera_transform;

	friend OneInstanceSingletonBase<ShadowMap>;
};