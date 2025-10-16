#include "ui_selector.hpp"

UISelector::UISelector(const int init_button_index, const bool is_first_stop_select, const bool is_loop_select) :
	m_init_button_index		(init_button_index),
	m_prev_button_index		(init_button_index),
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
	
}

void UISelector::Update()
{
	CalcButtonIndex();

	for (size_t i = 0; i < m_ui_buttons.size(); ++i)
	{
		if (i == m_current_button_index)
		{
			m_ui_buttons.at(i)->Activate();

			printfDx("%d\n", i);
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
	if (command->IsExecute(CommandKind::kSelectDown, TimeKind::kCurrent))
	{
		// タイマーを減らす
		m_select_timer -= delta_time;

		// タイマーが0以下になったら次のボタンへ
		if (m_select_timer <= 0.0f)
		{
			m_current_button_index += 1;

			// ループ処理
			if (m_is_loop_select && m_current_button_index >= static_cast<int>(m_ui_buttons.size()))
			{
				m_current_button_index = 0;
			}

			// タイマーをリセット
			if (m_selected_stop)
			{
				// 2回目以降は一定間隔
				m_select_timer = kSelectIntervalTime;
				printfDx("interval\n");
			}
			else
			{
				// 最初だけ長めの待ち時間
				m_selected_stop = true;
				m_select_timer = kFirstSelectStopTime;
				printfDx("first wait\n");
			}
		}
	}
	else
	{
		// 入力を離したら状態リセット
		m_select_timer = 0.0f;       // 次の押下で即反応させたいので0に
		m_selected_stop = false;
	}
}