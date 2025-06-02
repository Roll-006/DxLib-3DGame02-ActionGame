#pragma once
#include "../Base/scene_base.hpp"
#include "../Object/camera.hpp"
#include "../Object/player.hpp"
#include "../Object/selector.hpp"
#include "../Manager/command_handler.hpp"

class ShareScene final : public SceneBase
{
public:
	ShareScene();
	~ShareScene() override;

	void Init()		 override;
	void Update()	 override;
	void Draw() const override;

private:
	std::shared_ptr<Camera> m_camera;
	std::shared_ptr<Player> m_player;
};