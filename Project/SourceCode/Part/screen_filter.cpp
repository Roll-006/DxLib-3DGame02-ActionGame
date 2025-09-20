#include "screen_filter.hpp"

ScreenFilter::ScreenFilter() : 
	m_current_basis_filter		(nullptr),
	m_basis_alpha_blend_num		(255),
	m_main_screen				(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_basis_filter_screen		(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_near_death_filter_screen	(std::make_shared<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_is_using_basis_filter		(false),
	m_is_using_near_death_filter(false),
	m_near_death_blinking_sin	(0.0f)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<EnterNearDeathEvent>			([this](const EnterNearDeathEvent&			event)
	{
		SetNearDeathFilter(event);
	});
	EventSystem::GetInstance()->Subscribe<OnSelectNormalFilterEvent>	([this](const OnSelectNormalFilterEvent&		event)
	{
		SetNormalFilter(event);
	});
	EventSystem::GetInstance()->Subscribe<OnSelectCinematicFilterEvent>	([this](const OnSelectCinematicFilterEvent&	event)
	{
		SetCinematicFilter(event);
	});
	EventSystem::GetInstance()->Subscribe<OnSelectRetroFilterEvent>		([this](const OnSelectRetroFilterEvent&		event)
	{
		SetRetroFilter(event);
	});

	// 基礎フィルター登録
	m_basis_filters[BasisFilterKind::kNormal]	 = [this]() { UseNormalFilter();	};
	m_basis_filters[BasisFilterKind::kCinematic] = [this]() { UseCinematicFilter();	};
	m_basis_filters[BasisFilterKind::kRetro]	 = [this]() { UseRetroFilter();		};
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kNormal);
}

ScreenFilter::~ScreenFilter()
{

}

void ScreenFilter::Update()
{

}

void ScreenFilter::UseFilter()
{
	m_main_screen->UseScreen();
}

void ScreenFilter::UnuseFilter()
{
	m_main_screen->UnuseScreen();
}

void ScreenFilter::Draw()
{
	m_main_screen->Draw();

	DrawBasisFilter();
	DrawNearDeathFilter();
}


#pragma region Event
void ScreenFilter::SetNearDeathFilter(const EnterNearDeathEvent& event)
{
	m_is_using_near_death_filter	= true;
	m_near_death_blinking_sin		= 0.0f;
}

void ScreenFilter::SetNormalFilter(const OnSelectNormalFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kNormal);
	m_basis_alpha_blend_num = 255;
}

void ScreenFilter::SetCinematicFilter(const OnSelectCinematicFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kCinematic);
	m_basis_alpha_blend_num = 255;
}

void ScreenFilter::SetRetroFilter(const OnSelectRetroFilterEvent& event)
{
	m_current_basis_filter = m_basis_filters.at(BasisFilterKind::kRetro);
	m_basis_alpha_blend_num = 255;
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

void ScreenFilter::DrawBasisFilter()
{
	if (!m_current_basis_filter) { return; }

	m_basis_filter_screen->UseScreen();
	m_main_screen->Draw();
	m_basis_filter_screen->UnuseScreen();
	m_basis_filter_screen->GetGraphicer()->SetAlphaBlendNum(m_basis_alpha_blend_num);

	m_current_basis_filter();
	m_basis_filter_screen->Draw();
}

void ScreenFilter::DrawNearDeathFilter()
{
	if (!m_is_using_near_death_filter) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_near_death_blinking_sin, kBlinkingSpeed * delta_time, DX_PI_F, false);
	const auto blend_alpha_num = (sin(m_near_death_blinking_sin) * 0.5f + 0.5f) * 255;

	// 瀕死フィルター解除判定
	if (m_near_death_blinking_sin >= DX_PI_F)
	{
		m_is_using_near_death_filter = false;
	}

	m_near_death_filter_screen->UseScreen();
	m_main_screen->Draw();
	m_near_death_filter_screen->UnuseScreen();
	m_near_death_filter_screen->GetGraphicer()->SetAlphaBlendNum(blend_alpha_num);

	GraphFilter(m_near_death_filter_screen->GetScreenHandle(), DX_GRAPH_FILTER_MONO, 0, 0);
	m_near_death_filter_screen->Draw();
}
