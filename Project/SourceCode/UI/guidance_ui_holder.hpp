#pragma once
#include "guidance_ui.hpp"

class GuidanceUIHolder final
{
public:
	GuidanceUIHolder();
	~GuidanceUIHolder();

private:
	std::vector<std::shared_ptr<GuidanceUI>> m_guidance_ui;
};
