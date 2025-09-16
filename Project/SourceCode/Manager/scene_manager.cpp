#include "scene_manager.hpp"

SceneManager::SceneManager() : 
	m_share_scene	(std::make_shared<ShareScene>()),
	m_current_scene	(std::make_shared<TitleScene>()),
	m_drawer		(std::make_unique<Drawer>(ObjManager::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA)->GetTransform()))
{

}

SceneManager::~SceneManager()
{
	// ˆ—‚È‚µ
	// FIXME : ‚±‚Ì’n“_‚ð”²‚¯‚éÛ‚É—áŠO‚ª”­¶‚·‚é
}

void SceneManager::Update()
{
	InputChecker	::GetInstance()->Update();
	CommandHandler	::GetInstance()->Update();
	CollisionManager::GetInstance()->Update();
	PhysicsManager	::GetInstance()->Update();

	ChangeScene();

	m_current_scene	->Update();
	m_share_scene	->Update();
}

void SceneManager::LateUpdate()
{
	PhysicsManager::GetInstance()->LateUpdate();

	m_current_scene	->LateUpdate();
	m_share_scene	->LateUpdate();

	InputChecker::GetInstance()->LateUpdate();
}

void SceneManager::DrawToShadowMap() const
{
	m_drawer->DrawToShadowMap(m_share_scene, m_current_scene);
}

void SceneManager::Draw() const
{
	m_drawer->Draw(m_share_scene, m_current_scene);
}

void SceneManager::ChangeScene()
{
	const auto next_scene = m_current_scene->ChangeScene();
	if (next_scene)
	{
		m_current_scene = next_scene;
	}
}
