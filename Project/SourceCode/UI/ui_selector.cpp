#include "ui_selector.hpp"
#include "../Event/event_system.hpp"

UISelector::UISelector(const int init_button_index, const bool is_first_stop_select, const bool is_loop_select) :
	m_init_button_index		(init_button_index),
	m_current_button_index	(init_button_index),
	m_select_timer			(0.0f),
	m_is_first_stop_select	(is_first_stop_select),
	m_selected_stop			(false),
	m_is_loop_select		(is_loop_select)
{

}

UISelector::~UISelector()
{

}

void UISelector::Init()
{
	m_current_button_index = m_init_button_index;
}

void UISelector::Update()
{
	CalcButtonIndex();

	for (size_t i = 0; i < m_ui_buttons.size(); ++i)
	{
		if (i == m_current_button_index)
		{
			m_ui_buttons.at(i)->Activate();
		}
		else
		{
			m_ui_buttons.at(i)->Deactivate();
		}

		m_ui_buttons.at(i)->Update();
	}
}

void UISelector::AddUIButton(const std::shared_ptr<IUIElement> ui_button)
{
	m_ui_buttons.emplace_back(ui_button);
}

void UISelector::CalcButtonIndex()
{
	const auto delta_time	= GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	const auto command		= CommandHandler ::GetInstance();
	int increase = 0;

	const auto is_prev_input = 
		   command->IsExecute(CommandKind::kSelectDown, TimeKind::kPrev)
		|| command->IsExecute(CommandKind::kSelectUp,   TimeKind::kPrev);

	if (command->IsExecute(CommandKind::kSelectDown, TimeKind::kCurrent))
	{
		increase =  1;
	}
	else if (command->IsExecute(CommandKind::kSelectUp, TimeKind::kCurrent))
	{
		increase = -1;
	}

	// データを一時的に保存
	const auto prev_button_index = m_current_button_index;

	if (increase != 0)
	{
		m_select_timer -= delta_time;

		if (m_select_timer <= 0.0f)
		{
			m_current_button_index += increase;

			// ループ処理
			if (m_is_loop_select || !is_prev_input)
			{
				if (m_current_button_index >= static_cast<int>(m_ui_buttons.size()))
				{
					m_current_button_index = 0;
				}
				else if (m_current_button_index < 0)
				{
					m_current_button_index = static_cast<int>(m_ui_buttons.size() - 1);
				}
			}
			else
			{
				m_current_button_index = std::clamp(m_current_button_index, 0, static_cast<int>(m_ui_buttons.size() - 1));
			}

			// タイマーをリセット
			if (!m_selected_stop)
			{
				// 最初だけ特別扱い (フラグが有効なら長め、無効なら通常)
				m_select_timer	= m_is_first_stop_select ? kFirstSelectStopTime : kSelectIntervalTime;
				m_selected_stop = true;
			}
			else
			{
				// 2回目以降は一定間隔
				m_select_timer = kSelectIntervalTime;
			}
		}
	}
	else
	{
		m_select_timer	= 0.0f;
		m_selected_stop = false;
	}

	// 入力されたら通知
	if (prev_button_index != m_current_button_index) { EventSystem::GetInstance()->Publish(SelectMenuEvent()); }
}
