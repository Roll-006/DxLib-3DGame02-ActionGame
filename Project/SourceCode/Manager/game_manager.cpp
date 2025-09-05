#include "game_manager.hpp"

GameManager::GameManager():
	m_game_system_setter(std::make_unique<GameSystemSetter>()),
	m_window			(std::make_unique<Window>()),
	m_scene_manager		(nullptr)
{
	HandleKeeper			::Generate();
	HandleCreator			::Generate();
	UIDrawer				::Generate();
	CommandHandler			::Generate();
	ObjectPoolHolder		::Generate();

	GameTimeManager			::Generate();
	ObjManager				::Generate();
	CollisionManager		::Generate();
	PhysicsManager			::Generate();
	CinemachineBrain		::Generate();
	EffectManager			::Generate();
	RifleCartridgeManager	::Generate();

	m_scene_manager = std::make_unique<SceneManager>();
}

GameManager::~GameManager()
{	
	HandleKeeper			::Delete();
	HandleCreator			::Delete();
	UIDrawer				::Delete();
	CommandHandler			::Delete();
	ObjectPoolHolder		::Delete();

	GameTimeManager			::Delete();
	ObjManager				::Delete();
	CollisionManager		::Delete();
	PhysicsManager			::Delete();
	CinemachineBrain		::Delete();
	EffectManager			::Delete();
	RifleCartridgeManager	::Delete();
}

void GameManager::Run()
{
	while (IsContinueLoop())
	{
		GameTimeManager::GetInstance()->Update();
		m_window->Update();

		m_scene_manager->Update();
		m_scene_manager->LateUpdate();
		m_scene_manager->DrawToShadowMap();
		m_scene_manager->Draw();



		Capsule c = Capsule(VGet(0, 0,   0), VGet(0, 80,  0), 20);
		Segment s = Segment(VGet(0, 0, -40), VGet(0, 40, 40));
		VECTOR h1, h2;
		float t1, t2;

		float dist = math::GetDistanceSegmentToSegment(c.GetSegment(), s, h1, h2, t1, t2);

		c.Draw(true, 0, 0xffffff);
		s.Draw(true, 0, 0xff0000);
		DrawSphere3D(h1, 3, 8, 0xffffff, 0xffffff, TRUE);
		//DrawSphere3D(h2, 3, 8, 0xff0000, 0xff0000, TRUE);
		DrawLine3D(h1, h2, 0xffffff);



		GameTimeManager::GetInstance()->Draw();
		GameTimeManager::GetInstance()->WaitTime();
		ScreenFlip();
	}
}

bool GameManager::IsContinueLoop()
{
	if (ProcessMessage()  != 0) { return false; }
	if (ClearDrawScreen() != 0) { return false; }

	#ifdef _DEBUG
	// TODO : Œã‚ÉƒRƒ}ƒ“ƒh‰»
	if (InputChecker::GetInstance()->IsInput(KEY_INPUT_ESCAPE)) { return false; }

	if (   InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRB)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kLSPush)
		&& InputChecker::GetInstance()->IsInput(pad::ButtonKind ::kRSPush)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT)
		&& InputChecker::GetInstance()->IsInput(pad::TriggerKind::kRT))
	{
		return false;
	}
	#endif

	return true;
}
