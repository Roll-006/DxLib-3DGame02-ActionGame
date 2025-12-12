#include "game_over_tab.hpp"

GameOverTab::GameOverTab() : 
	m_tab_handle		(HandleCreator::GetInstance()->CreateHandle()),
	m_priority			(0),
	m_is_active			(false),
	m_can_select		(true),
	m_can_calc_wait_time(false),
	m_is_continue		(false),
	m_is_quit_game		(false),
	m_active_wait_timer	(0.0f),
	m_alpha_blend_num	(0),
	m_game_over_text	(std::make_shared<GameOverText>()),
	m_ui_selector		(std::make_shared<UISelector>(0, true, true)),
	m_filter_graphic	(std::make_shared<Graphicer>(UIGraphicPath.GAME_OVER_TAB_FILTER)),
	m_result_screen		(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_button_prompt		(std::make_shared<ButtonPrompt>("game_over"))
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

	m_button_prompt->AddExplanatoryText(0, "ゲームを続けます");
	m_button_prompt->AddExplanatoryText(1, "タイトルに戻ります");

	m_filter_graphic->SetCenterPos(Window::kCenterPos);
	m_filter_graphic->SetBlendNum(220);

	CalcAlphaBlendNum();
}

GameOverTab::~GameOverTab()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>(this, &GameOverTab::Activate);
}

void GameOverTab::Init()
{
	m_ui_selector->Init();
	m_game_over_text->Init();
}

void GameOverTab::Update()
{
	JudgeActive();

	if (!m_is_active) { return; }

	if (m_can_select) { m_ui_selector->Update(); }
	m_button_prompt->Update(m_ui_selector->GetCurrentButtonIndex());

	m_game_over_text->Update();

	CreateResultScreen();
	CalcAlphaBlendNum();
}

void GameOverTab::OnDraw(const int main_screen_handle) const
{
	if (!m_is_active) { return; }

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_result_screen->GetGraphicer()->GetBlendNum());
	DrawGraph(0, 0, main_screen_handle, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_result_screen	->Draw();
	m_game_over_text->Draw();
}

void GameOverTab::Activate(const DeadPlayerEvent& event)
{
	m_can_calc_wait_time = true;
}

void GameOverTab::ExecuteContinue()
{
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, 200.0f);

	m_is_continue	= true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
}

void GameOverTab::ExecuteQuitGame()
{
	SceneFader::GetInstance()->StartFade(UCHAR_MAX, 200.0f);

	m_is_quit_game	= true;
	m_can_select	= false;

	EventSystem::GetInstance()->Publish(DecisionEvent());
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

void GameOverTab::CalcAlphaBlendNum()
{
	if (m_alpha_blend_num >= UCHAR_MAX) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_alpha_blend_num, static_cast<int>(kFadeSpeed * delta_time), UCHAR_MAX, false);
	m_result_screen->GetGraphicer()->SetBlendNum(m_alpha_blend_num);
}

void GameOverTab::CreateResultScreen()
{
	m_result_screen->UseScreen();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(0, 0, Window::kScreenSize.x, Window::kScreenSize.y, 0x000000, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	m_filter_graphic->Draw();

	for (const auto& button : m_ui_selector->GetUIButtons())
	{
		button->Draw();
	}

	m_button_prompt->Draw();

	m_result_screen->UnuseScreen();
}
