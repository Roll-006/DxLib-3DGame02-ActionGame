#pragma once
#include <memory>
#include <unordered_map>
#include <EffekseerForDXLib.h>

#include "../Base/one_instance_singleton_base.hpp"

#include "../Window/window.hpp"
#include "../FPS/fps.hpp"
#include "scene_manager.hpp"
#include "obj_manager.hpp"

class GameManager : public OneInstanceSingletonBase<GameManager>
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
	std::unique_ptr<SceneObjManager> m_scene_manager;
};
