#pragma once
#include <memory>
#include <unordered_map>
#include <EffekseerForDXLib.h>

#include "../Window/window.hpp"
#include "../FPS/fps.hpp"
#include "scene_obj_manager.hpp"

/// @brief Only one instance
class GameManager
{
public:
	GameManager();
	~GameManager();

	void Run();

private:
	void SetUpGameSystem();

	/// @brief ƒQ[ƒ€ƒ‹[ƒv‚ğŒp‘±‚·‚é‚©‚ğ”»’è
	[[nodiscard]] bool ContinueLoop();

private:
	std::unique_ptr<Window>			 m_window;
	std::unique_ptr<FPS>			 m_fps;
	std::unique_ptr<SceneObjManager> m_scene_obj_manager;

	static bool m_instantiated;
};
