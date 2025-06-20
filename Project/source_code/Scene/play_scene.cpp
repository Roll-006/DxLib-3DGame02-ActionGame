#include "play_scene.hpp"

PlayScene::PlayScene():
	m_enemy		(std::make_shared<Enemy>()),
	m_house		(std::make_shared<House>()),
	m_ground	(std::make_shared<Ground>()),
	m_skydome	(std::make_shared<Skydome>(std::static_pointer_cast<Camera>(ObjManager::GetInstance()->GetObj(ObjName.CAMERA))))
{
	ObjManager    ::GetInstance()->AddObj		 (m_enemy);
	ObjManager    ::GetInstance()->AddObj		 (m_house);
	ObjManager    ::GetInstance()->AddObj		 (m_ground);
	PhysicsManager::GetInstance()->AddPhysicalObj(m_enemy);
	PhysicsManager::GetInstance()->AddPhysicalObj(m_house);
	PhysicsManager::GetInstance()->AddPhysicalObj(m_ground);
}

PlayScene::~PlayScene()
{

}

void PlayScene::Init()
{

}

void PlayScene::Update()
{
	m_enemy->Update();
	m_house->Update();
	m_ground->Update();
	m_skydome->Update();
}

void PlayScene::Draw() const
{
	m_enemy->Draw();
	m_house->Draw();
	m_ground->Draw();
	m_skydome->Draw();
}
