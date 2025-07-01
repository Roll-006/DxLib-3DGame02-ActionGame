#pragma once

#include "../Window/window.hpp"
#include "../FPS/fps.hpp"

#include "scene_manager.hpp"
#include "obj_manager.hpp"
#include "object_pool_manager.hpp"

class GameManager final : public OneInstanceSingletonBase<GameManager>
{
public:
	GameManager();
	~GameManager();

	void Run();

private:
	void SetUpGameSystem();

	/// @brief ƒQ[ƒ€ƒ‹[ƒv‚ğŒp‘±‚·‚é‚©‚ğ”»’è
	[[nodiscard]] bool IsContinueLoop();

private:
	std::unique_ptr<Window>			 m_window;
	std::unique_ptr<FPS>			 m_fps;
	std::unique_ptr<SceneObjManager> m_scene_manager;
};
