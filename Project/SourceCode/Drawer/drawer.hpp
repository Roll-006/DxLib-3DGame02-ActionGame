#pragma once
#include <EffekseerForDXLib.h>

#include "../Base/one_instance_singleton_base.hpp"
#include "../Interface/i_scene.hpp"

#include "../UI/ui_drawer.hpp"
#include "../ShadowMap/shadow_map.hpp"
#include "../Part/screen_creator.hpp"

class Drawer final : public OneInstanceSingletonBase<Drawer>
{
public:
	Drawer(const std::shared_ptr<Transform> camera_transform);
	~Drawer();

	void DrawToShadowMap(const std::shared_ptr<IScene> current_scene, const std::shared_ptr<IScene> share_scene) const;
	void Draw			(const std::shared_ptr<IScene> current_scene, const std::shared_ptr<IScene> share_scene) const;

private:
	std::unique_ptr<ShadowMap>		m_shadow_map;
	std::unique_ptr<ScreenCreator>	m_screen_creator;
};