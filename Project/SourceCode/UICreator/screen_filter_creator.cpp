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

void ScreenFilterCreator::Init()
{
	m_cinema_scope_frame->Init();
	m_damage_filter		->Init();
}

void ScreenFilterCreator::LateUpdate()
{
	m_cinema_scope_frame->LateUpdate();
	m_damage_filter		->LateUpdate();
}

void ScreenFilterCreator::OnDraw(const int main_screen_handle)
{
	m_cinema_scope_frame->Draw();
	m_damage_filter		->Draw(main_screen_handle);
}
