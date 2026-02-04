#include "button_graphic_getter.hpp"

ButtonGraphicGetter::ButtonGraphicGetter()
{
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_F))]					= std::make_shared<Graphicer>(UIGraphicPath.KEY_F);
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_SPACE))]				= std::make_shared<Graphicer>(UIGraphicPath.KEY_SPACE);
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_W))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/W.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_A))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/A.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_S))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/S.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_D))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/D.png");
	
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kLeft))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_LEFT);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kMiddle))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_MIDDLE);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kRight))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_RIGHT);
	
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kUp))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kLeft))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kDown))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kRight))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kA))]			= std::make_shared<Graphicer>(UIGraphicPath.PAD_A);
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kB))]			= std::make_shared<Graphicer>(UIGraphicPath.PAD_B);
	
	m_input_graphics[InputCode(InputKind::kPadTrigger,	static_cast<int>(pad::TriggerKind::kRT))]		= std::make_shared<Graphicer>(UIGraphicPath.PAD_RT);
	
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kLSUp))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/LS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kLSLeft))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/LS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kLSDown))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/LS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kLSRight))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/LS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kRSUp))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/RS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kRSLeft))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/RS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kRSDown))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/RS.png");
	m_input_graphics[InputCode(InputKind::kPadStick,	static_cast<int>(pad::StickKind::kRSRight))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/RS.png");
}

ButtonGraphicGetter::~ButtonGraphicGetter()
{

}
