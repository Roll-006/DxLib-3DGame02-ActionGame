#include "screen_filter_creator.hpp"

ScreenFilterCreator::ScreenFilterCreator() :
	m_name				(UICreatorName.SCREEN_FILTER_CREATOR),
	m_priority			(10),
	m_is_active			(true),
	m_cinema_scope_frame(std::make_shared<CinemaScopeFrame>())
{

}

ScreenFilterCreator::~ScreenFilterCreator()
{

}

void ScreenFilterCreator::LateUpdate()
{
	m_cinema_scope_frame->LateUpdate();
}

void ScreenFilterCreator::OnDraw()
{
	m_cinema_scope_frame->Draw();
}

void ScreenFilterCreator::OnNotify(const IEvent& event)
{
	const float delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	// ロケットランチャー専用カットシーンの開始
	if (event.GetType() == std::type_index(typeid(StartRocketLauncherCutsceneData)))
	{
		m_cinema_scope_frame->SetIncreaseThickness(200.0f * delta_time);
	}
	// ロケットランチャー専用カットシーンの終了
	if (event.GetType() == std::type_index(typeid(EndRocketLauncherCutsceneData)))
	{
		m_cinema_scope_frame->SetIncreaseThickness(-200.0f * delta_time);
	}
}
