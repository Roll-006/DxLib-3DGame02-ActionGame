#include "play_scene.hpp"

PlayScene::PlayScene():
	m_skydome(std::make_shared<Skydome>(std::static_pointer_cast<Camera>(ObjManager::GetInstance()->GetObj(ObjName.CAMERA))))
{

}

PlayScene::~PlayScene()
{

}

void PlayScene::Init()
{

}

void PlayScene::Update()
{
	m_skydome->Update();
}

void PlayScene::Draw() const
{
	//m_skydome->Draw();
}
