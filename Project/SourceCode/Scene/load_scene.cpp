#include "load_scene.hpp"
#include "../Manager/scene_manager.hpp"

LoadScene::LoadScene(const SceneKind next_scene_kind) :
	m_is_active			(true),
	m_scene_kind		(SceneKind::kLoad),
	m_next_scene_kind	(next_scene_kind),
	m_font_handle		(FontHandler::GetInstance()->GetFontHandle(FontName.GAME_CLEAR_TEXT)),
	m_text				("Now Loading"),
	m_font_size			(GetDrawStringWidthToHandle(m_text.c_str(), -1, m_font_handle), GetFontSizeToHandle(m_font_handle))
{

}

LoadScene::~LoadScene()
{

}

void LoadScene::Init()
{

}

void LoadScene::Update()
{

}

void LoadScene::LateUpdate()
{

}

void LoadScene::DrawToShadowMap() const
{

}

void LoadScene::Draw() const
{
	DrawStringToHandle(
		static_cast<int>(Window::kScreenSize.x - m_font_size.x),
		static_cast<int>(Window::kScreenSize.y - m_font_size.y),
		m_text.c_str(), 0xffffff, m_font_handle);
}

std::shared_ptr<IScene> LoadScene::ChangeScene()
{
	// ƒvƒŒƒC
	if (m_next_scene_kind == SceneKind::kPlay)
	{
		return std::make_shared<PlayScene>();
	}

	return nullptr;
}

void LoadScene::StartFadeIn()
{

}
