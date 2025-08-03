#pragma once
#include <unordered_map>

#include "effect_manager.hpp"
#include "ui_drawer.hpp"

#include "../Scene/title_scene.hpp"
#include "../Scene/play_scene.hpp"
#include "../Scene/game_clear_scene.hpp"
#include "../Scene/game_over_scene.hpp"
#include "../Scene/share_scene.hpp"
#include "../Scene/load_scene.hpp"

class SceneManager final : public OneInstanceSingletonBase<SceneManager>
{
public:
	SceneManager();
	~SceneManager();

	void Update();
	void LateUpdate();
	void Draw() const;

	/// @brief 現在のシーンの種類を取得
	[[nodiscard]] std::vector<SceneKind> GetCurrentSceneKind() const;

private:
	/// @brief 実行中のシーンに追加
	void AttachCurrentScene(const SceneKind scene_kind);
	/// @brief 実行中のシーンから除外
	void DetachCurrentScene(const SceneKind scene_kind);

private:
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_scene_list;		// すべてのシーン
	std::unordered_map<SceneKind, std::shared_ptr<SceneBase>> m_current_scene;	// 現在(実行中)のシーン
};
