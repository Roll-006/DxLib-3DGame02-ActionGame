#pragma once
#include "../Base/ui_button_base.hpp"
#include "../Calculation/math.hpp"
#include "../Font/font_handler.hpp"
#include "../Part/mask_creator.hpp"
#include "../Part/screen_creator.hpp"

class MainMenuSelectButton final : public UIButtonBase
{
public:
	enum class ButtonKind
	{
		kStartGame,		// ゲームを開始する
		kReturnToGame,	// ゲームに戻る
		kRestart,		// 最初から始める
		kOption,		// オプション
		kQuitGame,		// ゲームを終了する
		kExit,			// ソフトを終了する
	};

public:
	MainMenuSelectButton(
		const ButtonKind			button_kind, 
		const Vector2D<int>&		center_pos, 
		const std::function<void()> exeute_function, 
		const bool					is_init_selected);
	
	~MainMenuSelectButton() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

private:
	void CalcAlphaBlendNum();
	void CalcGraphicScale();

private:
	static constexpr int   kScreenWidth				= 720;
	static constexpr int   kScreenHeightOffset		= 20;
	static constexpr int   kNonSelectAlphaBlendNum	= 100;
	static constexpr float kNonSelectScale			= 0.8f;
	static constexpr float kSelectScale				= 1.0f;
	static constexpr float kScaleChangeSpeed		= 5.0f;

	ButtonKind						m_button_kind;
	int								m_alpha_blend_num;
	float							m_current_graphic_scale;
	float							m_destination_graphic_scale;
	std::shared_ptr<ScreenCreator>	m_button_screen;
};
