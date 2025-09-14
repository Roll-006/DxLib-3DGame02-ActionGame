#pragma once
#include "graphicer.hpp"

class ScreenCreator final
{
public:
	ScreenCreator(const Vector2D<int> screen_size);
	ScreenCreator(const Vector2D<int> screen_size, const Vector2D<int> center_pos);
	~ScreenCreator();

	/// @brief 生成したスクリーンを使用する
	/// @brief この関数を呼び出した後にスクリーンに反映する画像・図形を描画する
	void UseScreen(const bool is_clear_screen);

	/// @brief 生成したスクリーンの使用を終了する
	/// @brief スクリーンに反映する画像・図形を描画した後にこの関数を呼び出す
	void UnuseScreen();

	/// @brief 描画
	/// @brief UnuseScreenを呼び出した後に呼び出す
	void Draw() const;

	[[nodiscard]] std::shared_ptr<Graphicer> GetGraphicer()		const { return m_graphicer; }
	[[nodiscard]] Vector2D<int>				 GetScreenSize()	const { return m_screen_size; }
	[[nodiscard]] int						 GetScreenHandle()	const { return m_screen_graphic_handle; }

private:
	std::shared_ptr<Graphicer>	m_graphicer;
	Vector2D<int>				m_screen_size;
	int							m_screen_graphic_handle;

	// TODO : 後にデータクラス化
	VECTOR m_prev_pos;
	VECTOR m_prev_target;
	float  m_prev_near;
	float  m_prev_far;
	float  m_prev_fov;

	//bool m_was_enabled;
	//int m_prev_mode;
	//int m_prev_r;
	//int m_prev_g;
	//int m_prev_b;
	//float m_prev_start;
	//float m_prev_end;
	//float m_density;
};
