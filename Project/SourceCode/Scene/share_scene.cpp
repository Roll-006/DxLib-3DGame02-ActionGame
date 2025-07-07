#include "share_scene.hpp"

ShareScene::ShareScene() : 
	m_camera(std::make_shared<MainCamera>()),
	m_player(std::make_shared<Player>(m_camera))
{
	ObjManager		::GetInstance()->AddObj			(m_player);
	ObjManager		::GetInstance()->AddObj			(m_camera);
	CollisionManager::GetInstance()->AddCollideObj	(m_player);
	CollisionManager::GetInstance()->AddCollideObj	(m_camera);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_player);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_camera);

	m_camera->AttachTarget(ObjName.PLAYER, m_player->GetModelHandle(), BonePath.SPINE_2, false);
	m_camera->Init();
}

ShareScene::~ShareScene()
{

}

void ShareScene::Init()
{

}

void ShareScene::Update()
{
	m_player->Update();
	m_camera->Update();
}

void ShareScene::LateUpdate()
{
	m_player->LateUpdate();
	m_camera->LateUpdate();
}

void ShareScene::Draw() const
{
	m_player->Draw();
	m_camera->Draw();
}
