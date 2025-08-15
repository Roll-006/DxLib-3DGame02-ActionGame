#pragma once
#include "../Base/scene_base.hpp"

#include "../Object/main_camera.hpp"
#include "../Object/player.hpp"
#include "../Object/selector.hpp"

#include "../Command/command_handler.hpp"

#include "../UI/ui_drawer.hpp"
#include "../UI/screen_filter_creator.hpp"

#include "../Manager/camera_manager.hpp"

class ShareScene final : public SceneBase
{
public:
	ShareScene();
	~ShareScene() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

private:
	std::shared_ptr<MainCamera>					m_main_camera;
	std::shared_ptr<Player>						m_player;
	std::shared_ptr<ShareSceneEffectObjectPool>	m_share_scene_effect_object_pool;
	std::shared_ptr<IUICreator>					m_screen_filter_creator;
};