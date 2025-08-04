#pragma once
#include <EffekseerForDXLib.h>

#include "../Base/one_instance_singleton_base.hpp"
#include "../Base/scene_base.hpp"

#include "../UI/ui_drawer.hpp"
#include "../ShadowMap/shadow_map.hpp"

class Drawer final : public OneInstanceSingletonBase<Drawer>
{
public:
	Drawer(const std::shared_ptr<Transform> camera_transform);
	~Drawer();

	void DrawToShadowMap(std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> current_scene) const;
	void Draw			(std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> current_scene) const;

private:
	std::unique_ptr<ShadowMap> m_shadow_map;
};