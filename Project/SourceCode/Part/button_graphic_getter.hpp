#pragma once
#include "graphicer.hpp"
#include "../Path/ui_graphic_path.hpp"
#include "../Kind/input_kind.hpp"

class ButtonGraphicGetter final
{
public:
	ButtonGraphicGetter();
	~ButtonGraphicGetter();

	[[nodiscard]] std::shared_ptr<Graphicer> GetButtonGraphicer(const InputCode& input_code) const { return m_input_graphics.at(input_code); }

private:
	std::unordered_map<InputCode, std::shared_ptr<Graphicer>> m_input_graphics;
};
