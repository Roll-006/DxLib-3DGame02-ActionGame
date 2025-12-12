#include "button_graphic_getter.hpp"

ButtonGraphicGetter::ButtonGraphicGetter()
{
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_F))]					= std::make_shared<Graphicer>(UIGraphicPath.KEY_F);
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_SPACE))]				= std::make_shared<Graphicer>(UIGraphicPath.KEY_SPACE);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kLeft))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_LEFT);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kMiddle))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_MIDDLE);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kRight))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_RIGHT);
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kA))]			= std::make_shared<Graphicer>(UIGraphicPath.PAD_A);
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kB))]			= std::make_shared<Graphicer>(UIGraphicPath.PAD_B);
	m_input_graphics[InputCode(InputKind::kPadTrigger,	static_cast<int>(pad::TriggerKind::kRT))]		= std::make_shared<Graphicer>(UIGraphicPath.PAD_RT);
}

ButtonGraphicGetter::~ButtonGraphicGetter()
{

}
