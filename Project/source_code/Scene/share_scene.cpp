#include "share_scene.hpp"

ShareScene::ShareScene() : 
	m_camera(std::make_shared<Camera>()),
	m_player(std::make_shared<Player>(m_camera))
{
	ObjManager::GetInstance()->AddObj(m_camera);
	ObjManager::GetInstance()->AddObj(m_player);

	m_camera->AttachTarget(ObjName.PLAYER);
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

void ShareScene::Draw() const
{
	m_player->Draw();
	m_camera->Draw();
}
