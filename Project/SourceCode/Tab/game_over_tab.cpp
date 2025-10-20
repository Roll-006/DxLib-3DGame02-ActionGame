#include "game_over_tab.hpp"

GameOverTab::GameOverTab() : 
	m_priority			(0),
	m_is_active			(false),
	m_can_select		(true),
	m_can_calc_wait_time(false),
	m_active_wait_timer	(0.0f),
	m_ui_selector		(std::make_shared<UISelector>(0, true, true))
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>(this, &GameOverTab::Activate);

	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 2; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kContinue, center_pos.at(0), [this]() { ExecuteContinue(); }, true));
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kQuitGame, center_pos.at(1), [this]() { ExecuteQuitGame(); }, false));
}

GameOverTab::~GameOverTab()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &GameOverTab::Activate);
}

void GameOverTab::Init()
{
	m_ui_selector->Init();
}

void GameOverTab::Update()
{
	JudgeActive();

	if (!m_is_active) { return; }

	m_ui_selector->Update();
}

void GameOverTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}
}

void GameOverTab::Activate(const DeadPlayerEvent& event)
{
	m_can_calc_wait_time = true;
}

void GameOverTab::ExecuteContinue()
{

}

void GameOverTab::ExecuteQuitGame()
{

}

void GameOverTab::JudgeActive()
{
	if (m_is_active)			{ return; }
	if (!m_can_calc_wait_time)	{ return; }

	// プレイヤーの死亡通知を受け取ってから一定時間後にアクティブ化
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	m_active_wait_timer += delta_time;
	if (m_active_wait_timer > kActiveWaitTime)
	{
		m_is_active = true;
	}
}
