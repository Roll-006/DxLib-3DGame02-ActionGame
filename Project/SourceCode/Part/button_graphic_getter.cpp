#include "button_graphic_getter.hpp"

ButtonGraphicGetter::ButtonGraphicGetter()
{
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_F))]			= std::make_shared<Graphicer>(UIGraphicPath.KEY_F);
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kA))]	= std::make_shared<Graphicer>(UIGraphicPath.PAD_A);
}

ButtonGraphicGetter::~ButtonGraphicGetter()
{

}
