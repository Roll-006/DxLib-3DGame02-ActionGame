#include "guidance_ui_holder.hpp"

GuidanceUIHolder::GuidanceUIHolder() : 
	m_active_ui(nullptr)
{
	m_stock_ui["move"] = std::make_shared<GuidanceUI>("move");
}

GuidanceUIHolder::~GuidanceUIHolder()
{

}

void GuidanceUIHolder::LateUpdate()
{
	if (m_active_ui) { m_active_ui->LateUpdate(); }
}

void GuidanceUIHolder::Draw(const int main_screen_handle) const
{
	if (m_active_ui) { m_active_ui->Draw(main_screen_handle); }
}

void GuidanceUIHolder::Activate(const std::string& ui_name)
{
	const auto push_ui = m_stock_ui.at(ui_name);

	// 稼働中のUIがある場合、稼働を待つ
	if (m_active_ui)
	{
		m_wait_ui.push(push_ui);
	}
	else
	{
		m_active_ui = push_ui;
	}
}
