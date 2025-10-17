#include "drawer.hpp"

Drawer::Drawer(const std::shared_ptr<Transform>& camera_transform) :
	m_shadow_map	(std::make_unique<ShadowMap>(camera_transform)),
	m_screen_filter	(std::make_shared<ScreenFilter>())
{
	
}

Drawer::~Drawer()
{

}

void Drawer::DrawToShadowMap(const std::shared_ptr<IScene> current_scene, const std::shared_ptr<IScene> share_scene) const
{
	m_shadow_map->SetupDrawShadowMap();

	current_scene	->DrawToShadowMap();
	share_scene		->DrawToShadowMap();

	m_shadow_map->EndDrawShadowMap();
}

void Drawer::Draw(const std::shared_ptr<IScene> current_scene, const std::shared_ptr<IScene> share_scene) const
{
	m_screen_filter->UseFilter();
	m_shadow_map->UseShadowMap();

	current_scene	->Draw();
	share_scene		->Draw();

	m_shadow_map->UnuseShadowMap();
	DrawEffekseer3D();
	m_screen_filter->UnuseFilter();
	m_screen_filter->Draw();

	// TODO : 仮で画像ハンドルを渡す。のちに変更の可能性あり
	UIDrawer ::GetInstance()->Draw(m_screen_filter->GetMainScreen()->GetScreenHandle());
	TabDrawer::GetInstance()->Draw();
}
