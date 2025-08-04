#pragma once
#include "../Base/scene_base.hpp"

#include "../Object/zombie.hpp"
#include "../Object/house.hpp"
#include "../Object/ground.hpp"
#include "../Object/skydome.hpp"

#include "../Manager/ui_drawer.hpp"
#include "../UI/player_ui_creator.hpp"

#include "../ObjectPool/play_scene_effect_object_pool.hpp"

class PlayScene final : public SceneBase
{
public:
	PlayScene();
	~PlayScene() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

private:
	std::shared_ptr<Zombie>						m_zombie;
	std::shared_ptr<House>						m_house;
	std::shared_ptr<Ground>						m_ground;
	std::shared_ptr<Skydome>					m_skydome;
	std::shared_ptr<RifleCartridgeObjectPool>	m_rifle_cartridge_object_pool;
	std::shared_ptr<PlaySceneEffectObjectPool>  m_play_scene_effect_object_pool;
	std::shared_ptr<PlayerUICreator>			m_player_ui_creator;
};
