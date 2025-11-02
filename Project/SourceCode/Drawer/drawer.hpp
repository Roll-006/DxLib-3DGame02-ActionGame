#pragma once
#include <EffekseerForDXLib.h>

#include "../Base/single_instance_base.hpp"
#include "../Interface/i_scene.hpp"

#include "../UI/ui_drawer.hpp"
#include "../Tab/tab_drawer.hpp"
#include "../ShadowMap/shadow_map.hpp"
#include "../Part/screen_filter.hpp"
#include "../Part/scene_fader.hpp"

// MEMO : DXLibÇÃñΩñºÇ…çáÇÌÇπÇƒRenderÇ≈ÇÕÇ»Ç≠DrawerÇ∆Ç∑ÇÈ
class Drawer final : public SingleInstanceBase<Drawer>
{
public:
	Drawer();
	~Drawer();

	void DrawToShadowMap(const std::shared_ptr<IScene>& current_scene) const;
	void Draw			(const std::shared_ptr<IScene>& current_scene) const;

private:
	std::unique_ptr<ShadowMap>		m_shadow_map;
	std::shared_ptr<ScreenFilter>	m_screen_filter;
};
