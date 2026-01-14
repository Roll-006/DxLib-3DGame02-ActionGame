#pragma once
#include "../GameSystem/game_system_setter.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Window/window.hpp"
#include "../Debugger/debugger.hpp"
#include "../Font/font_handler.hpp"

#include "scene_manager.hpp"
#include "../Accessor/obj_accessor.hpp"

class GameManager final : public SingleInstanceBase<GameManager>
{
public:
	GameManager();
	~GameManager();

	void Run();

	void ExitGame(const ExitGameEvent& event);

private:
	/// @brief ゲームループを継続するかを判定
	[[nodiscard]] bool IsContinueLoop() const;

private:
	std::unique_ptr<GameSystemSetter>	m_game_system_setter;
	std::unique_ptr<Window>				m_window;
	std::unique_ptr<SceneManager>		m_scene_manager;

	bool m_is_exit_game;
};
