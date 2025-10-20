#include "drawer.hpp"

Drawer::Drawer() :
	m_shadow_map	(std::make_unique<ShadowMap>()),
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

	GraphFilter(m_screen_filter->GetMainScreen()->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 0, 0);

	// TODO : 仮で画像ハンドルを渡す。のちに変更の可能性あり
	UIDrawer  ::GetInstance()->Draw(m_screen_filter->GetMainScreen()->GetScreenHandle());
	TabDrawer ::GetInstance()->Draw(m_screen_filter->GetMainScreen()->GetScreenHandle());
	SceneFader::GetInstance()->Draw();
}
