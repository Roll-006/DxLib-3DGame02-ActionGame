#include "game_clear_ui_creator.hpp"

GameClearUiCreator::GameClearUiCreator() :
	m_name				(UICreatorName.GAME_CLEAR_UI_CREATOR),
	m_priority			(10),
	m_is_active			(false),
	m_game_clear_text	(std::make_shared<GameClearText>())
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadBossEvent>(this, &GameClearUiCreator::Activate);
}

GameClearUiCreator::~GameClearUiCreator()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<DeadBossEvent>(this, &GameClearUiCreator::Activate);
}

void GameClearUiCreator::Init()
{
	m_game_clear_text->Init();
}

void GameClearUiCreator::LateUpdate()
{
	if (!m_is_active) { return; }

	m_game_clear_text->LateUpdate();
}

void GameClearUiCreator::OnDraw(const int main_screen_handle)
{
	if (!m_is_active) { return; }

	m_game_clear_text->Draw();
}

void GameClearUiCreator::Activate(const DeadBossEvent& event)
{
	m_is_active = true;
}
