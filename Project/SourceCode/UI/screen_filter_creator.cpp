#include "screen_filter_creator.hpp"

ScreenFilterCreator::ScreenFilterCreator() :
	m_name				(UICreatorName.SCREEN_FILTER_CREATOR),
	m_priority			(1),
	m_is_active			(true),
	m_cinema_scope_frame(std::make_shared<CinemaScopeFrame>()),
	m_damage_filter		(std::make_shared<DamageFilter>())
{

}

ScreenFilterCreator::~ScreenFilterCreator()
{

}

void ScreenFilterCreator::LateUpdate()
{
	m_cinema_scope_frame->LateUpdate();
	m_damage_filter		->LateUpdate();
}

void ScreenFilterCreator::OnDraw()
{
	m_cinema_scope_frame->Draw();
	m_damage_filter		->Draw();
}
