#include "play_scene.hpp"

PlayScene::PlayScene():
	m_enemy				(std::make_shared<Enemy>()),
	m_house				(std::make_shared<House>()),
	m_ground			(std::make_shared<Ground>()),
	m_skydome			(std::make_shared<Skydome>(std::dynamic_pointer_cast<Camera>(ObjManager::GetInstance()->GetObj(ObjName.CAMERA)))),
	m_rifle_cartridge_object_pool(std::make_shared<RifleCartridgeObjectPool>())
{
	ObjManager		::GetInstance()->AddObj			(m_enemy);
	ObjManager		::GetInstance()->AddObj			(m_house);
	ObjManager		::GetInstance()->AddObj			(m_ground);
	CollisionManager::GetInstance()->AddCollideObj	(m_enemy);
	CollisionManager::GetInstance()->AddCollideObj	(m_house);
	CollisionManager::GetInstance()->AddCollideObj	(m_ground);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_enemy);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_house);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_ground);

	ObjectPoolManager::GetInstance()->AddObjectPool(m_rifle_cartridge_object_pool);
}

PlayScene::~PlayScene()
{

}

void PlayScene::Init()
{

}

void PlayScene::Update()
{
	m_enemy						->Update();
	RifleCartridgeManager::GetInstance()->Update();
	m_house						->Update();
	m_ground					->Update();
	m_skydome					->Update();
}

void PlayScene::LateUpdate()
{
	m_enemy						->LateUpdate();
	RifleCartridgeManager::GetInstance()->LateUpdate();
	m_house						->LateUpdate();
	m_ground					->LateUpdate();
	m_skydome					->LateUpdate();
}

void PlayScene::Draw() const
{
	m_enemy						->Draw();
	RifleCartridgeManager::GetInstance()->Draw();
	m_house						->Draw();
	m_ground					->Draw();
	m_skydome					->Draw();
}
