#include "play_scene.hpp"

PlayScene::PlayScene():
	m_zombie						(std::make_shared<Zombie>()),
	m_house							(std::make_shared<House>()),
	m_ground						(std::make_shared<Ground>()),
	m_skydome						(std::make_shared<Skydome>(ObjManager::GetInstance()->GetObj<MainCamera>(ObjName.MAIN_CAMERA))),
	m_rifle_cartridge_object_pool	(std::make_shared<RifleCartridgeObjectPool>()),
	m_play_scene_effect_object_pool (std::make_shared<PlaySceneEffectObjectPool>()),
	m_player_ui_creator				(std::make_shared<PlayerUICreator>(ObjManager::GetInstance()->GetObj<Player>(ObjName.PLAYER)))
{
	ObjManager		::GetInstance()->AddObj		   (m_zombie);
	ObjManager		::GetInstance()->AddObj		   (m_house);
	ObjManager		::GetInstance()->AddObj		   (m_ground);
	CollisionManager::GetInstance()->AddCollideObj (m_zombie);
	CollisionManager::GetInstance()->AddCollideObj (m_house);
	CollisionManager::GetInstance()->AddCollideObj (m_ground);
	PhysicsManager	::GetInstance()->AddPhysicalObj(m_zombie);
	PhysicsManager	::GetInstance()->AddPhysicalObj(m_house);
	PhysicsManager	::GetInstance()->AddPhysicalObj(m_ground);

	ObjectPoolHolder::GetInstance()->AddObjectPool (m_rifle_cartridge_object_pool);
	ObjectPoolHolder::GetInstance()->AddObjectPool (m_play_scene_effect_object_pool);
	UIDrawer		::GetInstance()->AddUICreator  (m_player_ui_creator);
}

PlayScene::~PlayScene()
{

}

void PlayScene::Init()
{

}

void PlayScene::Update()
{
	m_zombie							->Update();
	RifleCartridgeManager::GetInstance()->Update();
	m_house								->Update();
	m_ground							->Update();
	m_skydome							->Update();
}

void PlayScene::LateUpdate()
{
	m_zombie							->LateUpdate();
	RifleCartridgeManager::GetInstance()->LateUpdate();
	m_house								->LateUpdate();
	m_ground							->LateUpdate();
	m_skydome							->LateUpdate();
	m_player_ui_creator					->LateUpdate();
}

void PlayScene::Draw() const
{
	m_zombie							->Draw();
	RifleCartridgeManager::GetInstance()->Draw();
	m_house								->Draw();
	m_ground							->Draw();
	m_skydome							->Draw();
}
