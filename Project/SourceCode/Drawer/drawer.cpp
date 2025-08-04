#include "drawer.hpp"

Drawer::Drawer(const std::shared_ptr<Transform> camera_transform) :
	m_shadow_map(std::make_unique<ShadowMap>(camera_transform))
{

}

Drawer::~Drawer()
{

}

void Drawer::DrawToShadowMap(std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> current_scene) const
{
	m_shadow_map->SetupDrawShadowMap();

	for (const auto& scene : current_scene)
	{
		scene.second->DrawToShadowMap();
	}

	m_shadow_map->EndDrawShadowMap();
}

void Drawer::Draw(std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> current_scene) const
{
	m_shadow_map->UseShadowMap();

	for (const auto& scene : current_scene)
	{
		scene.second->Draw();
	}

	m_shadow_map->UnuseShadowMap();

	DrawEffekseer3D();
	UIDrawer::GetInstance()->Draw();
}
