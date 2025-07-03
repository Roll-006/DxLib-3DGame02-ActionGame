#pragma once
#include "../Base/scene_base.hpp"

#include "../Object/enemy.hpp"
#include "../Object/house.hpp"
#include "../Object/ground.hpp"
#include "../Object/skydome.hpp"

#include "../UI/player_ui_creator.hpp"

class PlayScene final : public SceneBase
{
public:
	PlayScene();
	~PlayScene() override;

	void Init()		  override;
	void Update()	  override;
	void LateUpdate() override;
	void Draw() const override;

private:
	std::shared_ptr<Enemy>						m_enemy;
	std::shared_ptr<House>						m_house;
	std::shared_ptr<Ground>						m_ground;
	std::shared_ptr<Skydome>					m_skydome;
	std::shared_ptr<RifleCartridgeObjectPool>	m_rifle_cartridge_object_pool;
	std::shared_ptr<PlayerUICreator>			m_player_ui_creator;
};
