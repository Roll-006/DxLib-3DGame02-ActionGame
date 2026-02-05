#include "button_graphic_getter.hpp"

ButtonGraphicGetter::ButtonGraphicGetter()
{
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_R))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/R.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_F))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/F.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_W))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/W.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_A))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/A.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_S))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/S.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_D))]					= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/D.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_SPACE))]				= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/SPACE.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_ESCAPE))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/ESCAPE.png");
	m_input_graphics[InputCode(InputKind::kKey,			static_cast<int>(KEY_INPUT_LSHIFT))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Key/SHIFT_Left.png");
	
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kLeft))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_LEFT);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kMiddle))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_MIDDLE);
	m_input_graphics[InputCode(InputKind::kMouseButton,	static_cast<int>(mouse::ButtonKind::kRight))]	= std::make_shared<Graphicer>(UIGraphicPath.MOUSE_RIGHT);
	
	m_input_graphics[InputCode(InputKind::kMouseWheel,	static_cast<int>(mouse::WheelKind::kUp))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/wheel_up.png");
	m_input_graphics[InputCode(InputKind::kMouseWheel,	static_cast<int>(mouse::WheelKind::kDown))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/wheel_down.png");
	
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kUp))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kLeft))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kDown))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	m_input_graphics[InputCode(InputKind::kMouseSlide,	static_cast<int>(mouse::SlideDirKind::kRight))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Mouse/none.png");
	
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kA))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/A.png");
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kB))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/B.png");
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kX))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/X.png");
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kY))]			= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/Y.png");
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kLSPush))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/push_LS.png");
	m_input_graphics[InputCode(InputKind::kPadButton,	static_cast<int>(pad::ButtonKind::kRSPush))]	= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/push_RS.png");

	m_input_graphics[InputCode(InputKind::kPadTrigger,	static_cast<int>(pad::TriggerKind::kRT))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/RT.png");
	m_input_graphics[InputCode(InputKind::kPadTrigger,	static_cast<int>(pad::TriggerKind::kLT))]		= std::make_shared<Graphicer>("Data/Graphic/UI/Button/Pad/LT.png");
	
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
