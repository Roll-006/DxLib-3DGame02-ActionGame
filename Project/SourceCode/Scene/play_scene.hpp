#pragma once
#include "../Interface/i_scene.hpp"

#include "../Object/player.hpp"
#include "../Manager/enemy_manager.hpp"
#include "../Object/house.hpp"
#include "../Object/ground.hpp"
#include "../Object/skydome.hpp"

#include "../UI/ui_drawer.hpp"
#include "../UI/player_ui_creator.hpp"

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

	[[nodiscard]] bool IsActive() const override { return m_is_active; }
	[[nodiscard]] std::shared_ptr<IScene> ChangeScene() override;

private:
	bool m_is_active;

	std::shared_ptr<Player>						m_player;
	std::shared_ptr<EnemyManager>				m_enemy_manager;
	std::shared_ptr<House>						m_house;
	std::shared_ptr<Ground>						m_ground;
	std::shared_ptr<Skydome>					m_skydome;
	std::shared_ptr<RifleCartridgeObjectPool>	m_rifle_cartridge_object_pool;
	std::shared_ptr<PlaySceneEffectObjectPool>  m_play_scene_effect_object_pool;
	std::shared_ptr<IUICreator>					m_player_ui_creator;
};
