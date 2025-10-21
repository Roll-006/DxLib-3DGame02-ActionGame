#include "share_scene.hpp"
#include "../Manager/scene_manager.hpp"

ShareScene::ShareScene() : 
	m_is_active						(true),
	m_scene_kind					(SceneKind::kShare),
	m_main_camera					(std::make_shared<MainCamera>()),
	m_share_scene_effect_object_pool(std::make_shared<ShareSceneEffectObjectPool>()),
	m_screen_filter_creator			(std::make_shared<ScreenFilterCreator>())
{
	m_main_camera->AddToObjManager();

	UIDrawer::GetInstance()->AddUICreator(m_screen_filter_creator);
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
	CinemachineBrain::GetInstance()	->Update();
	EffectManager	::GetInstance()	->Update();
}

void ShareScene::LateUpdate()
{
	CinemachineBrain::GetInstance()	->LateUpdate();
	EffectManager	::GetInstance()	->LateUpdate();
	m_screen_filter_creator			->LateUpdate();
}

void ShareScene::DrawToShadowMap() const
{
	
}

void ShareScene::Draw() const
{
	EffectManager::GetInstance()->Draw();
	m_main_camera				->Draw();
}

std::shared_ptr<IScene> ShareScene::ChangeScene()
{
	return nullptr;
}

void ShareScene::StartFadeIn()
{

}
