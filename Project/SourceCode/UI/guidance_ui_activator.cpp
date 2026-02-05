#include "guidance_ui_activator.hpp"

GuidanceUIActivator::GuidanceUIActivator() :
	m_active_ui	(nullptr),
	m_was_active{ {"move", false}, {"run", false}, {"camera", false}, {"shot", false}, {"reload", false} }
{
	m_stock_ui["move"]		= std::make_shared<GuidanceUI>("move");
	m_stock_ui["run"]		= std::make_shared<GuidanceUI>("run");
	m_stock_ui["camera"]	= std::make_shared<GuidanceUI>("camera");
	m_stock_ui["shot"]		= std::make_shared<GuidanceUI>("shot");
	m_stock_ui["reload"]	= std::make_shared<GuidanceUI>("reload");

	// イベント登録
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>		(this, &GuidanceUIActivator::ActivateOnSceneChange);
	EventSystem::GetInstance()->Subscribe<OnMagazineEmpty>		(this, &GuidanceUIActivator::ActivateOnMagazineEmpty);
}

GuidanceUIActivator::~GuidanceUIActivator()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>	(this, &GuidanceUIActivator::ActivateOnSceneChange);
	EventSystem::GetInstance()->Unsubscribe<OnMagazineEmpty>	(this, &GuidanceUIActivator::ActivateOnMagazineEmpty);
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
void GuidanceUIActivator::ActivateOnSceneChange		(const ChangeSceneEvent&	event)
{
	switch (event.next_scene_kind)
	{
	case SceneKind::kPlay:
		if (!m_was_active.at("move"))
		{
			m_was_active.at("move") = true;
			Activate("move");
		}

		if (!m_was_active.at("run"))
		{
			m_was_active.at("run") = true;
			Activate("run");
		}

		if (!m_was_active.at("camera"))
		{
			m_was_active.at("camera") = true;
			Activate("camera");
		}

		if (!m_was_active.at("shot"))
		{
			m_was_active.at("shot") = true;
			Activate("shot");
		}
		break;

	default:
		break;
	}
}

void GuidanceUIActivator::ActivateOnMagazineEmpty	(const OnMagazineEmpty&		event)
{
	if (!m_was_active.at("reload"))
	{
		m_was_active.at("reload") = true;
		Activate("reload");
	}
}
#pragma endregion
