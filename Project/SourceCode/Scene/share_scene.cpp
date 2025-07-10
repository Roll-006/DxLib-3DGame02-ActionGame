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
	camera_manager->SetMainCamera(m_main_camera);
	camera_manager->AddVirtualCamera(std::make_shared<RotControlVirtualCamera>());
	camera_manager->GetVirtualCamera<RotControlVirtualCamera>(VirtualCameraKind::kRotControl)->AttachTarget(m_player->GetTransform());
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
	m_main_camera->Update();
}

void ShareScene::LateUpdate()
{
	m_player->LateUpdate();
	m_main_camera->LateUpdate();
}

void ShareScene::Draw() const
{
	m_player->Draw();
	m_main_camera->Draw();
}
