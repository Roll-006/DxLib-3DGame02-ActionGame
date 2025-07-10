#include "share_scene.hpp"

ShareScene::ShareScene() : 
	m_main_camera	(std::make_shared<MainCamera>()),
	m_player		(std::make_shared<Player>())
{
	ObjManager		::GetInstance()->AddObj			(m_player);
	ObjManager		::GetInstance()->AddObj			(m_main_camera);
	CollisionManager::GetInstance()->AddCollideObj	(m_player);
	CollisionManager::GetInstance()->AddCollideObj	(m_main_camera);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_player);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(m_main_camera);

	//m_camera->AttachTarget(ObjName.PLAYER, m_player->GetModelHandle(), BonePath.SPINE_2, false);
	const auto camera_manager = CameraManager::GetInstance();
	const auto rot_camera = std::make_shared<RotControlVirtualCamera>();
	camera_manager->SetMainCamera(m_main_camera);
	camera_manager->AddVirtualCamera(rot_camera);
	rot_camera->AttachTarget(m_player->GetTransform());
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
	m_main_camera				->Update();
}

void ShareScene::LateUpdate()
{
	m_player					->LateUpdate();
	CameraManager::GetInstance()->LateUpdate();
	m_main_camera				->LateUpdate();
}

void ShareScene::Draw() const
{
	m_player		->Draw();
	m_main_camera	->Draw();
}
