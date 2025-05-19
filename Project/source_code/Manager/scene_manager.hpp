#pragma once
#include <unordered_map>

#include "../Base/one_instance_singleton_base.hpp"

#include "../Scene/title_scene.hpp"
#include "../Scene/play_scene.hpp"
#include "../Scene/game_clear_scene.hpp"
#include "../Scene/game_over_scene.hpp"
#include "../Scene/share_scene.hpp"
#include "../Scene/load_scene.hpp"

class SceneObjManager final : public OneInstanceSingletonBase<SceneObjManager>
{
public:
	SceneObjManager();
	~SceneObjManager();

	void Update();
	void Draw()const;

private:
	void AttachCurrentScene(const SceneKind scene_kind);
	void DetachCurrentScene(const SceneKind scene_kind);

private:
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_scene_list;		// すべてのシーン
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_current_scene;	// 現在(実行中)のシーン
};
