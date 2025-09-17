//#pragma once
//#include "../Window/window.hpp"
//
//class CinemaScopeFrame final
//{
//public:
//	CinemaScopeFrame();
//	~CinemaScopeFrame();
//
//	void LateUpdate();
//	void Draw() const;
//
//	/// @brief フレームの太さを増加させる変更する
//	/// @brief シネスコ比率を超えることはない
//	/// @param speed 増加速度(デルタタイム補正後)
//	void SetIncreaseThickness(const float speed);
//
//private:
//	void CalcPos();
//
//private:
//	static constexpr Vector2D<int> kFrameSize = { Window::kScreenSize.x, Window::kScreenSize.y };
//
//	Vector2D<int>	m_up_frame_up_left_pos;			// 上部フレームの左上座標
//	Vector2D<int>	m_up_frame_down_right_pos;		// 上部フレームの右下座標
//	Vector2D<int>	m_down_frame_up_left_pos;		// 下部フレームの左上座標
//	Vector2D<int>	m_down_frame_down_right_pos;	// 下部フレームの右下座標
//	int				m_thickness;
//	float			m_increase;
//};
