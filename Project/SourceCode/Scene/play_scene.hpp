#pragma once
#include "../Interface/i_scene.hpp"

#include "../Object/player.hpp"
#include "../Manager/enemy_manager.hpp"
#include "../Object/houses.hpp"
#include "../Object/ground.hpp"
#include "../Object/skydome.hpp"
#include "../Part/stealth_kill_target_searcher.hpp"
#include "../Part/melee_target_searcher.hpp"
#include "../Part/item_creator.hpp"

#include "../UI/ui_drawer.hpp"
#include "../UICreator/player_ui_creator.hpp"
#include "../Tab/tab_drawer.hpp"
#include "../Tab/game_clear_tab.hpp"
#include "../Tab/game_over_tab.hpp"
#include "../Tab/pause_tab.hpp"
#include "../Light/light_holder.hpp"

#include "../ObjectPool/play_scene_effect_object_pool.hpp"

class PlayScene final : public IScene
{
public:
	PlayScene();
	~PlayScene() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void Activate()		override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] bool						IsActive()		const	override { return m_is_active; }
	[[nodiscard]] std::shared_ptr<IScene>	ChangeScene()			override;
	[[nodiscard]] virtual SceneKind			GetSceneKind()	const	override { return m_scene_kind; }

private:
	void StartFadeIn() override;

private:
	bool		m_is_active;
	SceneKind	m_scene_kind;
	float		m_elapsed_time;
	bool		m_can_fade_in;

	std::shared_ptr<Player>						m_player;
	std::shared_ptr<EnemyManager>				m_enemy_manager;
	std::shared_ptr<Houses>						m_houses;
	std::shared_ptr<Ground>						m_ground;
	std::shared_ptr<Skydome>					m_skydome;
	std::shared_ptr<StealthKillTargetSearcher>	m_stealth_kill_target_searcher;
	std::shared_ptr<MeleeTargetSearcher>		m_melee_target_searcher;
	std::shared_ptr<ItemCreator>				m_item_creator;
	std::shared_ptr<RifleCartridgeObjectPool>	m_rifle_cartridge_object_pool;
	std::shared_ptr<PlaySceneEffectObjectPool>  m_play_scene_effect_object_pool;
	std::shared_ptr<IUICreator>					m_player_ui_creator;
	//std::shared_ptr<IUICreator>					m_game_clear_ui_creator;
	std::shared_ptr<GameClearTab>				m_game_clear_tab;
	std::shared_ptr<GameOverTab>				m_game_over_tab;
	std::shared_ptr<PauseTab>					m_pause_tab;
};
