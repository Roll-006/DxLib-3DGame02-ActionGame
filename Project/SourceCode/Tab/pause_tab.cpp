#include "pause_tab.hpp"

PauseTab::PauseTab() :
	m_tab_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_priority					(0),
	m_is_active					(false),
	m_is_deactivate_forcibly	(false),
	m_can_select				(true),
	m_is_execute_return_to_game	(false),
	m_is_restart				(false),
	m_is_option					(false),
	m_is_quit_game				(false),
	m_alpha_blend_num			(0),
	m_ui_selector				(std::make_shared<UISelector>(0, true, true)),
	m_result_screen				(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_warning_restart_tab		(std::make_shared<WarningTab>(WarningTab::WarningKind::kRestart)),
	m_warning_quit_game_tab		(std::make_shared<WarningTab>(WarningTab::WarningKind::kQuitGame))
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>(this, &PauseTab::Deactivate);

	TabDrawer::GetInstance()->AddTab(m_warning_restart_tab);
	TabDrawer::GetInstance()->AddTab(m_warning_quit_game_tab);

	std::vector<Vector2D<int>> center_pos;
	for (int i = 0; i < 4; ++i)
	{
		center_pos.emplace_back(kFirstButtonCenterPos + Vector2D<int>(0, kButtonPosInterval * i));
	}

	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kReturnToGame,	center_pos.at(0), [this]() { ExecuteReturnToGame(); },	true));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kRestart,		center_pos.at(1), [this]() { ExecuteRestart(); },		false));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kOption,		center_pos.at(2), [this]() { ExecuteOption(); },		false));
	m_ui_selector->AddUIButton(std::make_shared<MainMenuSelectButton>(MainMenuSelectButton::ButtonKind::kQuitGame,		center_pos.at(3), [this]() { ExecuteQuitGame();	},		false));

	CalcAlphaBlendNum();
}

PauseTab::~PauseTab()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &PauseTab::Deactivate);

	TabDrawer::GetInstance()->RemoveTab(m_warning_restart_tab	->GetTabHandle());
	TabDrawer::GetInstance()->RemoveTab(m_warning_quit_game_tab	->GetTabHandle());
}

void PauseTab::Init()
{
	m_ui_selector->Init();

	m_is_active					= false;
	m_is_deactivate_forcibly	= false;
	m_is_option					= false;
	m_is_execute_return_to_game = false;
	m_alpha_blend_num			= 0;

	const auto game_time_manager = GameTimeManager::GetInstance();
	game_time_manager->InitTimeScale();

	game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  m_prev_time_scale.at(TimeScaleLayerKind::kWorld));
	game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, m_prev_time_scale.at(TimeScaleLayerKind::kPlayer));
	game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, m_prev_time_scale.at(TimeScaleLayerKind::kEffect));
	game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, m_prev_time_scale.at(TimeScaleLayerKind::kCamera));
}

void PauseTab::Update()
{
	JudgeActive();

	if (!m_is_active) { return; }

	JudgeDeactivate();
	JudgeSelect();
	BackTab();

	if (m_can_select) { m_ui_selector->Update(); }

	CreateResultScreen();
	CalcAlphaBlendNum();

	m_warning_restart_tab	->Update();
	m_warning_quit_game_tab	->Update();

	if (m_warning_restart_tab->IsDecide())
	{
		const auto fader = SceneFader::GetInstance();
		fader->StartFade(255, 150.0f);

		m_is_restart = true;
	}
	else if (m_warning_restart_tab->IsBack())
	{
		m_warning_restart_tab->Init();
	}

	if (m_warning_quit_game_tab->IsDecide())
	{
		const auto fader = SceneFader::GetInstance();
		fader->StartFade(255, 150.0f);

		m_is_quit_game = true;
	}
	else if (m_warning_quit_game_tab->IsBack())
	{
		m_warning_quit_game_tab->Init();
	}
}

void PauseTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_result_screen->GetGraphicer()->GetBlendNum());
	DrawGraph(0, 0, main_screen_handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_result_screen->Draw();
}

void PauseTab::Deactivate(const DeadPlayerEvent& event)
{
	m_is_deactivate_forcibly = true;
}

void PauseTab::ExecuteReturnToGame()
{
	m_is_execute_return_to_game = true;
}

void PauseTab::ExecuteRestart()
{
	m_warning_restart_tab->Activate();
}

void PauseTab::ExecuteOption()
{

}

void PauseTab::ExecuteQuitGame()
{
	m_warning_quit_game_tab->Activate();
}

void PauseTab::JudgeActive()
{
	if (m_is_active) { return; }
	
	if(!m_is_deactivate_forcibly && CommandHandler::GetInstance()->IsExecute(CommandKind::kPause, TimeKind::kCurrent))
	{
		m_is_active = true;

		const auto game_time_manager = GameTimeManager::GetInstance();

		m_prev_time_scale[TimeScaleLayerKind::kWorld]  = game_time_manager->GetTimeScale(TimeScaleLayerKind::kWorld);
		m_prev_time_scale[TimeScaleLayerKind::kPlayer] = game_time_manager->GetTimeScale(TimeScaleLayerKind::kPlayer);
		m_prev_time_scale[TimeScaleLayerKind::kEffect] = game_time_manager->GetTimeScale(TimeScaleLayerKind::kEffect);
		m_prev_time_scale[TimeScaleLayerKind::kCamera] = game_time_manager->GetTimeScale(TimeScaleLayerKind::kCamera);

		game_time_manager->SetTimeScale(TimeScaleLayerKind::kWorld,  0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kPlayer, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kEffect, 0.0f);
		game_time_manager->SetTimeScale(TimeScaleLayerKind::kCamera, 0.0f);
	}
}

void PauseTab::JudgeDeactivate()
{
	if (m_is_deactivate_forcibly || IsReturnToGame()) { Init(); }
}

void PauseTab::JudgeSelect()
{
	const auto is_active_warning_tag	= m_warning_quit_game_tab->IsActive() || m_warning_restart_tab->IsActive();
	const auto is_max_blend_num			= m_result_screen->GetGraphicer()->GetBlendNum() >= 255;

	m_can_select = is_max_blend_num && !is_active_warning_tag ? true : false;
}

void PauseTab::CalcAlphaBlendNum()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	if (m_is_execute_return_to_game)
	{
		math::Decrease(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), 0);
	}
	else
	{
		math::Increase(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), 255, false);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void PauseTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	m_result_screen->UnuseScreen();
}

void PauseTab::BackTab()
{
	if (m_result_screen->GetGraphicer()->GetBlendNum() < 255
		|| m_warning_quit_game_tab->IsActive()
		|| m_warning_restart_tab->IsActive())
	{
		return;
	}

	if (CommandHandler::GetInstance()->IsExecute(CommandKind::kPause, TimeKind::kCurrent))
	{
		m_is_execute_return_to_game = true;
	}
}
