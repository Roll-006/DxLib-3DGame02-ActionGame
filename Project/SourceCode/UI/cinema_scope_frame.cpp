#include "cinema_scope_frame.hpp"

CinemaScopeFrame::CinemaScopeFrame() : 
	m_up_frame_up_left_pos		(v2d::GetZeroV<Vector2D<int>>()),
	m_up_frame_down_right_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_down_frame_up_left_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_down_frame_down_right_pos	(v2d::GetZeroV<Vector2D<int>>()),
	m_thickness					(0),
	m_increase					(0)
{
	// ƒCƒxƒ“ƒg“o˜^
	EventSystem::GetInstance()->Subscribe<DeadBossEvent>					(this, &CinemaScopeFrame::EnterFrame);
	EventSystem::GetInstance()->Subscribe<StartRocketLauncherCutsceneEvent>	(this, &CinemaScopeFrame::EnterFrame);
	EventSystem::GetInstance()->Subscribe<EndRocketLauncherCutsceneEvent>	(this, &CinemaScopeFrame::ExitFrame);
}

CinemaScopeFrame::~CinemaScopeFrame()
{
	// ƒCƒxƒ“ƒg‚Ì“o˜^‰ðœ
	EventSystem::GetInstance()->Unsubscribe<DeadBossEvent>						(this, &CinemaScopeFrame::EnterFrame);
	EventSystem::GetInstance()->Unsubscribe<StartRocketLauncherCutsceneEvent>	(this, &CinemaScopeFrame::EnterFrame);
	EventSystem::GetInstance()->Unsubscribe<EndRocketLauncherCutsceneEvent>		(this, &CinemaScopeFrame::ExitFrame);
}

void CinemaScopeFrame::Init()
{
	m_thickness = 0;
	m_increase	= 0;
}

void CinemaScopeFrame::LateUpdate()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_increase > 0)
	{
		math::Increase(m_thickness, static_cast<int>( m_increase * delta_time), kFrameSize.y, false);
	}
	else
	{
		math::Decrease(m_thickness, static_cast<int>(-m_increase * delta_time), 0);
	}

	CalcPos();
}

void CinemaScopeFrame::Draw() const
{
	if (m_thickness <= 0) { return; }

	DrawBox(
		m_up_frame_up_left_pos.x, 
		m_up_frame_up_left_pos.y, 
		m_up_frame_down_right_pos.x, 
		m_up_frame_down_right_pos.y,
		0x000000, TRUE);

	DrawBox(
		m_down_frame_up_left_pos.x,
		m_down_frame_up_left_pos.y,
		m_down_frame_down_right_pos.x,
		m_down_frame_down_right_pos.y,
		0x000000, TRUE);
}

void CinemaScopeFrame::CalcPos()
{
	m_up_frame_up_left_pos		= { 0, 0 };
	m_up_frame_down_right_pos	= { Window::kScreenSize.x, m_thickness };
	m_down_frame_up_left_pos	= { 0, Window::kScreenSize.y - m_thickness };
	m_down_frame_down_right_pos = { Window::kScreenSize.x, Window::kScreenSize.y };
}

void CinemaScopeFrame::EnterFrame(const DeadBossEvent& event)
{
	m_increase = kIncreaseSpeed;
}

void CinemaScopeFrame::EnterFrame(const StartRocketLauncherCutsceneEvent& event)
{
	m_increase = kIncreaseSpeed;
}

void CinemaScopeFrame::ExitFrame(const EndRocketLauncherCutsceneEvent& event)
{
	m_increase = -kIncreaseSpeed;
}
