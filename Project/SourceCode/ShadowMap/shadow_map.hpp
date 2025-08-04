#pragma once
#include <DxLib.h>
#include "../Base/one_instance_singleton_base.hpp"

class ShadowMap final : public OneInstanceSingletonBase<ShadowMap>
{
public:
	ShadowMap();
	~ShadowMap();

	void PrepareDrawShadowMap();
	void EndDrawShadowMap();

	void UseShadowMap();
	void UnuseShadowMap();

private:
	static constexpr int	kSizeX		= 4096;
	static constexpr int	kSizeY		= 4096;
	static constexpr VECTOR kMaxDrawPos = { 400.0f, 100.0f, 400.0f };
	static constexpr VECTOR kMinDrawPos = { -400.0f, -10.0f, -400.0f };

	int m_shadow_map_handle;

	friend OneInstanceSingletonBase<ShadowMap>;
};