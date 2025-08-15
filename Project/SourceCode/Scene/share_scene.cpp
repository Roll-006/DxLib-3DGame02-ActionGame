#include "share_scene.hpp"

ShareScene::ShareScene() : 
	m_main_camera					(std::make_shared<MainCamera>()),
	m_player						(std::make_shared<Player>()),
	m_share_scene_effect_object_pool(std::make_shared<ShareSceneEffectObjectPool>()),
	m_screen_filter_creator			(std::make_shared<ScreenFilterCreator>())
{
	m_main_camera	->AddToObjManager();
	m_player		->AddToObjManager();

	UIDrawer::GetInstance()->AddUICreator(m_screen_filter_creator);

	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->AddVirtualCameraController(std::make_shared<ControlVirtualCamerasController>(*m_player.get()));

	ObjectPoolHolder::GetInstance()->AddObjectPool(m_share_scene_effect_object_pool);
}

ShareScene::~ShareScene()
{

}

void ShareScene::Init()
{

}

void ShareScene::Update()
{
	m_player					->Update();
	CameraManager::GetInstance()->Update();
	EffectManager::GetInstance()->Update();
}

void ShareScene::LateUpdate()
{
	m_player					->LateUpdate();
	CameraManager::GetInstance()->LateUpdate();
	EffectManager::GetInstance()->LateUpdate();
	m_screen_filter_creator		->LateUpdate();
}

void ShareScene::DrawToShadowMap() const
{
	m_player->DrawToShadowMap();
}

void ShareScene::Draw() const
{
	m_player->Draw();
}
