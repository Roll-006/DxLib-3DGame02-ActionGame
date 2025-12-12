#include "game_clear_tab.hpp"

GameClearTab::GameClearTab() :
	m_tab_handle					(HandleCreator::GetInstance()->CreateHandle()),
	m_priority						(0),
	m_is_active						(false),
	m_can_select					(false),
	m_can_calc_active_time			(false),
	m_is_retry						(false),
	m_is_quit_game					(false),
	m_active_timer					(0.0f),
	m_alpha_blend_num				(0),
	m_change_time_scale_wait_time	(0.0f),
	m_is_change_time_scale			(false),
	m_game_clear_text				(std::make_shared<GameClearText>()),
	m_ui_selector					(std::make_shared<UISelector>(0, true, true)),
	m_result_screen					(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_button_prompt					(std::make_shared<ButtonPrompt>("game_clear"))
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<DeadAllEnemyEvent>(this, &GameClearTab::StartActivateTimer);

	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 2; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}
	
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kRetry,	 center_pos.at(0), [this]() { ExecuteRetry();	 }, true));
	m_ui_selector->AddUIButton(std::make_shared<SubMenuSelectButton>(SubMenuSelectButton::ButtonKind::kQuitGame, center_pos.at(1), [this]() { ExecuteQuitGame(); }, false));

	m_button_prompt->AddExplanatoryText(0, "ゲームを再度プレイします");
	m_button_prompt->AddExplanatoryText(1, "タイトルに戻ります");

	CalcAlphaBlendNum();
}

GameClearTab::~GameClearTab()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<DeadAllEnemyEvent>(this, &GameClearTab::StartActivateTimer);
}

void GameClearTab::Init()
{
	m_ui_selector->Init();
	m_game_clear_text->Init();

	m_is_retry		= false;
	m_is_quit_game	= false;
}

void GameClearTab::Update()
{
	JudgeActive();

	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }
	m_button_prompt->Update(m_ui_selector->GetCurrentButtonIndex());

	m_can_select = !SceneFader::GetInstance()->IsFading() && m_is_change_time_scale;

	m_game_clear_text->Update();

	ChangeTimeScale();
	CreateResultScreen();
	CalcAlphaBlendNum();
}

void GameClearTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_result_screen->GetGraphicer()->GetBlendNum());
	DrawGraph(0, 0, main_screen_handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_result_screen		->Draw();
	m_game_clear_text	->Draw();
}

void GameClearTab::StartActivateTimer(const DeadAllEnemyEvent& event)
{
	m_can_calc_active_time = true;
}

void GameClearTab::ExecuteRetry()
{
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, 200.0f);

	m_is_retry		= true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void GameClearTab::ExecuteQuitGame()
{
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, 200.0f);

	m_is_quit_game	= true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
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

void GameClearTab::ChangeTimeScale()
{
	if (!m_is_active)			{ return; }
	if (m_is_change_time_scale) { return; }

	const auto game_time_manager	= GameTimeManager::GetInstance();
	const auto delta_time			= game_time_manager->GetDeltaTime(TimeScaleLayerKind::kUI);

	m_change_time_scale_wait_time += delta_time;
	if (m_change_time_scale_wait_time > kDrawResultWaitTime)
	{
		m_is_change_time_scale = true;
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, 0.0f);
	}
}

void GameClearTab::CalcAlphaBlendNum()
{
	if (m_alpha_blend_num >= UCHAR_MAX) { return; }

	if (m_change_time_scale_wait_time > kDrawResultWaitTime)
	{
		const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
		math::Increase(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), UCHAR_MAX, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void GameClearTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 170);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	m_button_prompt->Draw();

	m_result_screen->UnuseScreen();
}
