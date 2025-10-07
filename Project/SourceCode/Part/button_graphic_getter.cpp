#include "button_graphic_getter.hpp"

ButtonGraphicGetter::ButtonGraphicGetter()
{
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_F))]			= std::make_shared<Graphicer>(UIGraphicPath.ASSAULT_RIFLE);
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kA))]	= std::make_shared<Graphicer>(UIGraphicPath.ROCKET_LAUNCHER);
}

ButtonGraphicGetter::~ButtonGraphicGetter()
{

}
