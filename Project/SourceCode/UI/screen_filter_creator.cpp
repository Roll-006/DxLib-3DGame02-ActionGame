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
