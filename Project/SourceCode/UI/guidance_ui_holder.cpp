#include "guidance_ui_holder.hpp"

GuidanceUIHolder::GuidanceUIHolder()
{
	m_guidance_ui.emplace_back(std::make_shared<GuidanceUI>("move"));
}

GuidanceUIHolder::~GuidanceUIHolder()
{

}
