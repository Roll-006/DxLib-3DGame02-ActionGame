#pragma once
#include <cassert>
#include <DxLib.h>

class FPS
{
public:
	FPS();
	~FPS();

	void Update();
	void Wait()const;

	[[nodiscard]] static float GetDeltaTime() { return m_delta_time; }

private:
	static constexpr int   kFPS				= 60;
	static constexpr int   kMaxWaitTimeMill	= 1000;
	static constexpr float kMillSecond		= 1000.0f;
	static constexpr float kMicroSecond		= 1000000.0f;

	int      m_fps;
	LONGLONG m_first_time;		// 1ƒtƒŒ[ƒ€–Ú‚ÌŒv‘ªŠÔ
	LONGLONG m_current_time;
	LONGLONG m_prev_time;
	int	     m_frame_count;
	float    m_average_fps;

	static float m_delta_time;
	static bool  m_instantiated;
};
