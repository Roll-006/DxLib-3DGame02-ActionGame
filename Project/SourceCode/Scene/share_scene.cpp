#include "share_scene.hpp"

ShareScene::ShareScene() : 
	m_main_camera	(std::make_shared<MainCamera>()),
	m_player		(std::make_shared<Player>())
{
	// 各オブジェクトマネージャーへの登録
	ObjManager		::GetInstance()->AddObj				(m_player);
	ObjManager		::GetInstance()->AddObj				(m_main_camera);
	CollisionManager::GetInstance()->AddCollideObj		(m_player);
	CollisionManager::GetInstance()->AddCollideObj		(m_main_camera);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(m_player);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(m_main_camera);
	PhysicsManager  ::GetInstance()->AddIgnoreObjGravity(m_main_camera->GetObjHandle());

	// メインカメラの登録
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->SetMainCamera(m_main_camera);
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
}

void ShareScene::LateUpdate()
{
	m_player					->LateUpdate();
	CameraManager::GetInstance()->LateUpdate();
}

void ShareScene::Draw() const
{
	m_player		->Draw();
	m_main_camera	->Draw();
}
