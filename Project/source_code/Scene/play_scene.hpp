#pragma once
#include "../Base/scene_base.hpp"

#include "../Object/enemy.hpp"
#include "../Object/house.hpp"
#include "../Object/ground.hpp"
#include "../Object/skydome.hpp"

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
	std::shared_ptr<Enemy>   m_enemy;
	std::shared_ptr<House>   m_house;
	std::shared_ptr<Ground>  m_ground;
	std::shared_ptr<Skydome> m_skydome;
	std::shared_ptr<BulletObjectPool> m_bullet_object_pool;
};