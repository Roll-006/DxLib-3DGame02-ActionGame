#pragma once
#include "../Drawer/drawer.hpp"

#include "../Scene/share_scene.hpp"
#include "../Scene/title_scene.hpp"
#include "../Scene/play_scene.hpp"
//#include "../Scene/game_clear_scene.hpp"
#include "../Scene/load_scene.hpp"

class SceneManager final : public SingleInstanceBase<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void LateUpdate();
	void DrawToShadowMap()	const;
	void Draw()				const;

private:
	void ChangeScene();

private:
	std::shared_ptr<IScene> m_share_scene;
	std::shared_ptr<IScene> m_current_scene;

	std::unique_ptr<Drawer> m_drawer;
};
