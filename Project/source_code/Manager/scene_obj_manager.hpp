#pragma once
#include <cassert>
#include <string>
#include <unordered_map>

#include "../Object/Scene/title_scene.hpp"
#include "../Object/Scene/play_scene.hpp"
#include "../Object/Scene/game_clear_scene.hpp"
#include "../Object/Scene/game_over_scene.hpp"
#include "../Object/Scene/share_scene.hpp"
#include "../Object/Scene/load_scene.hpp"

/// @brief Only one instance
class SceneObjManager
{
public:
	SceneObjManager();
	~SceneObjManager();

	void Init();
	void Update();
	void Draw()const;

	[[nodiscard]] std::shared_ptr<SceneObj> GetScene(const std::string& scene_name);

private:
	void AttachScene(const std::string& scene_name);
	void DetachScene(const std::string& scene_name);

private:
	std::unordered_map<std::string, std::shared_ptr<SceneObj>> m_scene_list;
	std::unordered_map<std::string, std::shared_ptr<SceneObj>> m_current_scene;

	static bool m_instantiated;
};
