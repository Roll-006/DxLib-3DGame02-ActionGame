#pragma once
#include "../Base/one_instance_singleton_base.hpp"

#include "../Scene/title_scene.hpp"
#include "../Scene/play_scene.hpp"
#include "../Scene/game_clear_scene.hpp"
#include "../Scene/game_over_scene.hpp"
#include "../Scene/share_scene.hpp"
#include "../Scene/load_scene.hpp"

/// @brief Only one instance
class SceneObjManager : public OneInstanceSingletonBase
{
public:
	SceneObjManager();
	~SceneObjManager();

	void Init();
	void Update();
	void Draw()const;

	[[nodiscard]] std::shared_ptr<SceneBase> GetScene(SceneKind scene_kind);

private:
	void AttachCurrentScene(SceneKind scene_kind);
	void DetachCurrentScene(SceneKind scene_kind);

private:
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_scene_list;		// すべてのシーン
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_current_scene;	// 実行中のシーン
};
