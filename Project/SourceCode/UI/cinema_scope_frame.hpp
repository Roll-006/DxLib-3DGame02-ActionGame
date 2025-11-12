#pragma once
#include "../Window/window.hpp"
#include "../Event/event_system.hpp"

class CinemaScopeFrame final
{
public:
	CinemaScopeFrame();
	~CinemaScopeFrame();

	void Init();
	void LateUpdate();
	void Draw() const;

private:
	void EnterFrame	(const DeadAllEnemyEvent&				 event);
	void EnterFrame	(const StartRocketLauncherCutsceneEvent& event);
	void ExitFrame	(const EndRocketLauncherCutsceneEvent&   event);

	void CalcPos();

private:
	static constexpr float kIncreaseSpeed = 200.0f;
	static constexpr Vector2D<int> kFrameSize = { 
		Window::kScreenSize.x, 
		static_cast<int>((Window::kScreenSize.y - Window::kScreenSize.x / Window::kCinemaScopeRatio.x) * 0.5f) };

	Vector2D<int>	m_up_frame_up_left_pos;			// 上部フレームの左上座標
	Vector2D<int>	m_up_frame_down_right_pos;		// 上部フレームの右下座標
	Vector2D<int>	m_down_frame_up_left_pos;		// 下部フレームの左上座標
	Vector2D<int>	m_down_frame_down_right_pos;	// 下部フレームの右下座標
	int				m_thickness;
	float			m_increase;
};
