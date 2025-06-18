#pragma once
#include "../Base/scene_base.hpp"

#include "../Object/skydome.hpp"
#include "../Object/enemy.hpp"

class PlayScene final : public SceneBase
{
public:
	PlayScene();
	~PlayScene() override;

	void Init()		 override;
	void Update()	 override;
	void Draw() const override;

private:
	std::shared_ptr<Skydome> m_skydome;
	std::shared_ptr<Enemy>   m_enemy;
};