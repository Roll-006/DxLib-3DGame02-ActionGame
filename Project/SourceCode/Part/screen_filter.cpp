#include "screen_filter.hpp"

ScreenFilter::ScreenFilter() : 
	m_current_basis_filter			(nullptr),
	m_basis_alpha_blend_num			(UCHAR_MAX),
	m_resource_screen				(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos, false)),
	m_result_screen					(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos, false)),
	m_basis_filter_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_near_death_filter_screen		(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_death_filter_screen			(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_is_using_basis_filter			(false),
	m_is_using_near_death_filter	(false),
	m_is_using_death_filter			(false),
	m_near_death_blinking_sin		(0.0f),
	m_death_filter_alpha_blend_num	(0)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>				(this, &ScreenFilter::Init);
	EventSystem::GetInstance()->Subscribe<EnterNearDeathEvent>			(this, &ScreenFilter::SetNearDeathFilter);
	EventSystem::GetInstance()->Subscribe<DeadPlayerEvent>				(this, &ScreenFilter::SetDeathFilter);
	EventSystem::GetInstance()->Subscribe<OnSelectNormalFilterEvent>	(this, &ScreenFilter::SetNormalFilter);
	EventSystem::GetInstance()->Subscribe<OnSelectCinematicFilterEvent>	(this, &ScreenFilter::SetCinematicFilter);
	EventSystem::GetInstance()->Subscribe<OnSelectRetroFilterEvent>		(this, &ScreenFilter::SetRetroFilter);

	// 基礎フィルター登録
	m_basis_filters[BasisFilterKind::kNormal]	 = [this]() { UseNormalFilter();	};
	m_basis_filters[BasisFilterKind::kCinematic] = [this]() { UseCinematicFilter();	};
	m_basis_filters[BasisFilterKind::kRetro]	 = [this]() { UseRetroFilter();		};
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kNormal);
}

ScreenFilter::~ScreenFilter()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>				(this, &ScreenFilter::Init);
	EventSystem::GetInstance()->Unsubscribe<EnterNearDeathEvent>			(this, &ScreenFilter::SetNearDeathFilter);
	EventSystem::GetInstance()->Unsubscribe<DeadPlayerEvent>				(this, &ScreenFilter::SetDeathFilter);
	EventSystem::GetInstance()->Unsubscribe<OnSelectNormalFilterEvent>		(this, &ScreenFilter::SetNormalFilter);
	EventSystem::GetInstance()->Unsubscribe<OnSelectCinematicFilterEvent>	(this, &ScreenFilter::SetCinematicFilter);
	EventSystem::GetInstance()->Unsubscribe<OnSelectRetroFilterEvent>		(this, &ScreenFilter::SetRetroFilter);
}

void ScreenFilter::Init()
{
	m_is_using_near_death_filter	= false;
	m_is_using_death_filter			= false;
}

void ScreenFilter::Update()
{

}

void ScreenFilter::UseFilter()
{
	m_resource_screen->UseScreen();
}

void ScreenFilter::UnuseFilter()
{
	m_resource_screen->UnuseScreen();
}

void ScreenFilter::CreateScreen()
{
	CreateFilter();

	m_result_screen->UseScreen();

	m_resource_screen->Draw();
	if (m_current_basis_filter)			{ m_basis_filter_screen		->Draw(); }
	if (m_is_using_near_death_filter)	{ m_near_death_filter_screen->Draw(); }
	if (m_is_using_death_filter)		{ m_death_filter_screen		->Draw(); }

	m_result_screen->UnuseScreen();
}

void ScreenFilter::Draw()
{
	m_result_screen->Draw();
}

void ScreenFilter::CreateFilter()
{
	CreateBasisFilter();
	CreateNearDeathFilter();
	CreateDeathFilter();
}


#pragma region Event
void ScreenFilter::Init(const ChangeSceneEvent& event)
{
	Init();
}

void ScreenFilter::SetNearDeathFilter(const EnterNearDeathEvent& event)
{
	m_is_using_near_death_filter	= true;
	m_near_death_blinking_sin		= 0.0f;
}

void ScreenFilter::SetDeathFilter(const DeadPlayerEvent& event)
{
	m_is_using_death_filter			= true;
	m_death_filter_alpha_blend_num	= 0;
}

void ScreenFilter::SetNormalFilter(const OnSelectNormalFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kNormal);
	m_basis_alpha_blend_num = UCHAR_MAX;
}

void ScreenFilter::SetCinematicFilter(const OnSelectCinematicFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kCinematic);
	m_basis_alpha_blend_num = UCHAR_MAX;
}

void ScreenFilter::SetRetroFilter(const OnSelectRetroFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kRetro);
	m_basis_alpha_blend_num = UCHAR_MAX;
}
#pragma endregion


void ScreenFilter::UseNormalFilter()
{
	// TODO : 仮。ライト処理終了後に調整でフィルターを決める
	m_current_basis_filter = nullptr;
}

void ScreenFilter::UseCinematicFilter()
{

}

void ScreenFilter::UseRetroFilter()
{

}

void ScreenFilter::CreateBasisFilter()
{
	if (!m_current_basis_filter) { return; }

	m_basis_filter_screen->UseScreen();
	m_resource_screen->Draw();
	m_basis_filter_screen->UnuseScreen();
	m_basis_filter_screen->GetGraphicer()->SetBlendNum(m_basis_alpha_blend_num);

	m_current_basis_filter();
}

void ScreenFilter::CreateNearDeathFilter()
{
	if (!m_is_using_near_death_filter) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_near_death_blinking_sin, kBlinkingSpeed * delta_time, DX_PI_F, false);
	const auto blend_alpha_num = static_cast<int>((sin(m_near_death_blinking_sin) * 0.5f + 0.5f) * UCHAR_MAX);

	// 瀕死フィルター解除判定
	if (m_near_death_blinking_sin >= DX_PI_F)
	{
		m_is_using_near_death_filter = false;
	}

	m_near_death_filter_screen->UseScreen();
	m_resource_screen->Draw();
	m_near_death_filter_screen->UnuseScreen();
	m_near_death_filter_screen->GetGraphicer()->SetBlendNum(blend_alpha_num);

	GraphFilter(m_near_death_filter_screen->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 0, 0);
}

void ScreenFilter::CreateDeathFilter()
{
	if (!m_is_using_death_filter) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_death_filter_alpha_blend_num, static_cast<int>(100.0f * delta_time), UCHAR_MAX, false);

	m_death_filter_screen->UseScreen();
	m_resource_screen->Draw();
	m_death_filter_screen->UnuseScreen();
	m_death_filter_screen->GetGraphicer()->SetBlendNum(m_death_filter_alpha_blend_num);

	GraphFilter(m_death_filter_screen->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 0, 0);
}
