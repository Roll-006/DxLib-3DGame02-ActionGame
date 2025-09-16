#include "drawer.hpp"

Drawer::Drawer(const std::shared_ptr<Transform> camera_transform) :
	m_shadow_map(std::make_unique<ShadowMap>(camera_transform))
{

}

Drawer::~Drawer()
{

}

void Drawer::DrawToShadowMap(const std::shared_ptr<IScene> share_scene, const std::shared_ptr<IScene> current_scene) const
{
	m_shadow_map->SetupDrawShadowMap();

	share_scene		->DrawToShadowMap();
	current_scene	->DrawToShadowMap();

	m_shadow_map->EndDrawShadowMap();
}

void Drawer::Draw(const std::shared_ptr<IScene> share_scene, const std::shared_ptr<IScene> current_scene) const
{
	m_shadow_map->UseShadowMap();

	share_scene		->Draw();
	current_scene	->Draw();

	m_shadow_map->UnuseShadowMap();

	DrawEffekseer3D();
	UIDrawer::GetInstance()->Draw();
}
