#include "game_clear_tab.hpp"

GameClearTab::GameClearTab() :
	m_tab_handle			(HandleCreator::GetInstance()->CreateHandle()),
	m_priority				(0),
	m_is_active				(false),
	m_can_select			(true),
	m_can_calc_active_time	(false),
	m_active_timer			(0.0f),
	m_alpha_blend_num		(0),
	m_ui_selector			(std::make_shared<UISelector>(0, true, true)),
	m_result_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos))
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadBossEvent>(this, &GameClearTab::StartActivateTimer);

	//std::vector<Vector2D<int>> center_pos;
	//for (int i = 0; i < 2; ++i)
	//{
	//	center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	//}

	//m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kContinue, center_pos.at(0), [this]() { ExecuteContinue(); }, true));
	//m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kQuitGame, center_pos.at(1), [this]() { ExecuteQuitGame(); }, false));

	//m_filter_graphic->SetCenterPos(Window::kCenterPos);
	//m_filter_graphic->SetBlendNum(220);

	CalcAlphaBlendNum();
}

GameClearTab::~GameClearTab()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<DeadBossEvent>(this, &GameClearTab::StartActivateTimer);
}

void GameClearTab::Init()
{
	m_ui_selector->Init();
}

void GameClearTab::Update()
{
	JudgeActive();

	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }

	CreateResultScreen();
	CalcAlphaBlendNum();
}

void GameClearTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_result_screen->GetGraphicer()->GetBlendNum());
	//DrawGraph(0, 0, main_screen_handle, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_result_screen->Draw();
}

void GameClearTab::StartActivateTimer(const DeadBossEvent& event)
{
	m_can_calc_active_time = true;
}

void GameClearTab::JudgeActive()
{
	if (m_is_active) { return; }
	if (!m_can_calc_active_time) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	m_active_timer += delta_time;
	if (m_active_timer > kActiveWaitTime)
	{
		m_is_active = true;
	}
}

void GameClearTab::CalcAlphaBlendNum()
{
	if (m_alpha_blend_num >= UCHAR_MAX) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), UCHAR_MAX, false);
	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void GameClearTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	//DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//m_filter_graphic->Draw();

	//for (const auto& button : m_ui_selector->GetUIButtons())
	//{
	//	button->Draw();
	//}

	m_result_screen->UnuseScreen();
}
