#include "window.hpp"

Window::Window():
	m_change_window_count	(0),
	m_is_full_screen		(false)
{
	ChangeWindowMode(m_is_full_screen ? FALSE : TRUE);	// DxLib既存の関数はFALSEがフルスクリーンであるためフラグを反転
	if (m_is_full_screen) { ++m_change_window_count; }
}

Window::~Window()
{

}

void Window::Update()
{
	SetWindowMode();
}

void Window::SetWindowMode()
{
	// ALTを押しながらエンターでウィンドウモード変更

	if (!CheckHitKey(KEY_INPUT_LALT)) { return; }

	if (InputChecker::GetInstance()->GetInputState(KEY_INPUT_RETURN) == InputState::kSingle)
	{
		++m_change_window_count;

		m_is_full_screen = m_change_window_count % 2 ? true : false;
		ChangeWindowMode(m_is_full_screen ? FALSE : TRUE);
		InputChecker::GetInstance()->InitMouseCursor();
	}
}

void Window::SetWindowMode(const bool is_full_screen)
{
	m_is_full_screen	  = is_full_screen;
	m_change_window_count = m_is_full_screen ? 1 : 0;

	ChangeWindowMode(m_is_full_screen ? FALSE : TRUE);
}
