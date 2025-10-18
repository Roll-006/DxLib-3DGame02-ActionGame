#include "ui_button_base.hpp"

UIButtonBase::UIButtonBase(const std::function<void()> exeute_function) :
	m_is_active			(false),
	m_exeute_function	(exeute_function)
{

}

UIButtonBase::~UIButtonBase()
{

}

void UIButtonBase::Exeute()
{
	if (!m_is_active) { return; }

	// 決定ボタンを入力した際の実行内容はstd::functionから受け取る
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecute(CommandKind::kDecide, TimeKind::kCurrent))
	{
		m_exeute_function();
	}
}
