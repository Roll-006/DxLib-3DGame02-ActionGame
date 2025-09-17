#include "drawer.hpp"

Drawer::Drawer(const std::shared_ptr<Transform> camera_transform) :
	m_shadow_map	(std::make_unique<ShadowMap>(camera_transform)),
	m_screen_creator(std::make_unique<ScreenCreator>(Window::kScreenSize, Vector2D<int>(Window::kScreenSize.x * 0.5f, Window::kScreenSize.y * 0.5f)))
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
	m_screen_creator->UseScreen();
	m_shadow_map->UseShadowMap();

	current_scene	->Draw();
	share_scene		->Draw();

	m_shadow_map->UnuseShadowMap();
	DrawEffekseer3D();
	m_screen_creator->UnuseScreen();

	// TODO : フィルター処理を分離
	GraphFilter(m_screen_creator->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 60, 10);
	m_screen_creator->Draw();

	UIDrawer::GetInstance()->Draw();
}
