#include "guidance_ui_activator.hpp"

GuidanceUIActivator::GuidanceUIActivator() :
	m_active_ui(nullptr)
{
	m_stock_ui["move"] = std::make_shared<GuidanceUI>("move");

	// イベント登録
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>(this, &GuidanceUIActivator::ActivateOnSceneChange);
}

GuidanceUIActivator::~GuidanceUIActivator()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>(this, &GuidanceUIActivator::ActivateOnSceneChange);
}

void GuidanceUIActivator::LateUpdate()
{
	if (m_active_ui)
	{
		// 非アクティブ状態になった場合、除外、もしくはアクティブ状態を待つUIと入れ替える
		if (!m_active_ui->IsActive())
		{
			if (!m_wait_ui.empty())
			{
				m_active_ui = m_wait_ui.front();
				m_wait_ui.pop();
				m_active_ui->Activate();
			}
			else
			{
				m_active_ui = nullptr;
			}
		}
	}

	if (m_active_ui) { m_active_ui->LateUpdate(); }
}

void GuidanceUIActivator::Activate(const std::string& ui_name)
{
	const auto push_ui = m_stock_ui.at(ui_name);

	// 稼働中のUIがある場合、稼働を待つ
	if (m_active_ui)
	{
		m_wait_ui.push(push_ui);
	}
	else
	{
		m_active_ui = push_ui;
		m_active_ui->Activate();
	}
}


#pragma region Event
void GuidanceUIActivator::ActivateOnSceneChange(const ChangeSceneEvent& event)
{
	switch (event.next_scene_kind)
	{
	case SceneKind::kPlay:
		Activate("move");
		break;

	default:
		break;
	}
}
#pragma endregion
