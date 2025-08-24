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
	CameraManager			::Generate();
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
	CameraManager			::Delete();
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


		 



		// 仮実装
		const auto velocity						= VGet(50, -200, 100);
		
		const auto current_capsule				= Capsule(VGet(-50, -20, -100) + VGet(0, 400, 100), VGet(-30, 0, -70) + VGet(0, 400, 100), 10);
		auto	   future_capsule				= current_capsule;
		future_capsule.Move(velocity);
		const auto triangle						= Triangle(VGet(100, 0, 200), VGet(0, 500, 0), VGet(-100, 0, 200));
		const auto plane						= Plane(triangle.GetCentroid(), triangle.GetNormalVector());

		const auto  future_begin_pos			= future_capsule.GetSegment().GetBeginPos();
		const auto  future_end_pos				= future_capsule.GetSegment().GetEndPos();
		const auto  plane_to_begin_pos_distance = math::GetDistancePointToPlane(future_begin_pos, plane);
		const auto  plane_to_end_pos_distance	= math::GetDistancePointToPlane(future_end_pos, plane);
		const auto  is_begin_pos_ahead_of_plane = math::IsPointAheadOfPlane(future_begin_pos, plane);
		const auto  is_end_pos_ahead_of_plane	= math::IsPointAheadOfPlane(future_end_pos, plane);
		float       penetration_depth			= 0.0f;
		bool        is_ahead_of_plane			= false;

		// めり込んだ距離を取得
		// 始点・終点がどちらも平面の前方にある場合は近い方を採用
		if (is_begin_pos_ahead_of_plane && is_end_pos_ahead_of_plane)
		{
			penetration_depth = min(plane_to_begin_pos_distance, plane_to_end_pos_distance);
		}
		// 始点・終点がどちらも平面の後方にある場合は遠い方を採用
		else if (!is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
		{
			penetration_depth = max(plane_to_begin_pos_distance, plane_to_end_pos_distance);
		}
		// 始点が平面の前面にあり、終点が後方にある場合は終点からの距離を採用
		else if (is_begin_pos_ahead_of_plane && !is_end_pos_ahead_of_plane)
		{
			penetration_depth = plane_to_end_pos_distance;
		}
		// 終点が平面の前面にあり、始点が後方にある場合は視点からの距離を採用
		else
		{
			penetration_depth = plane_to_begin_pos_distance;
		}

		// velocityと三角形に沿ったベクトルの角度を取得
		const auto  plane_cross_v1 = math::GetNormalVector(plane.GetNormalVector(), axis::GetWorldYAxis());
		auto        plane_cross_v2 = math::GetNormalVector(plane.GetNormalVector(), plane_cross_v1);
		auto        angle = math::GetAngleBetweenTwoVector(plane_cross_v2, v3d::GetNormalizedV(velocity));

		// 角度が90を超えてた場合、今後投影先に使うベクトルを反転
		if (angle > 90.0f * math::kDegToRad)
		{
			plane_cross_v2 *= -1;
			angle = DX_PI_F - angle;
		}

		const auto sub_length			= penetration_depth / sin(angle);
		const auto push_back_length		= sub_length + (sub_length / penetration_depth) * current_capsule.GetRadius();

		const auto  sub_v				= v3d::GetNormalizedV(velocity) * push_back_length;
		const auto  valid_velocity		= velocity - sub_v;
		auto        fix_capsule			= current_capsule;
		fix_capsule.Move(valid_velocity);

		const auto  wall_slide_velocity = math::GetProjectionVector(sub_v, plane_cross_v2);
		auto        result_capsule		= fix_capsule;
		result_capsule.Move(wall_slide_velocity);

		const auto fix_velocity			= result_capsule.GetSegment().GetBeginPos() - future_begin_pos;
		const auto result_velocity		= velocity + fix_velocity;




		// 仮描画
		DrawLine3D(current_capsule.GetSegment().GetBeginPos(), future_capsule.GetSegment().GetBeginPos(), 0xffffff);
		DrawLine3D(current_capsule.GetSegment().GetBeginPos(), current_capsule.GetSegment().GetBeginPos() + plane_cross_v2 * 100, 0xff0000);
		DrawLine3D(current_capsule.GetSegment().GetBeginPos(), current_capsule.GetSegment().GetBeginPos() + result_velocity, 0xff0000);
		
		triangle       .Draw(true, 200, 0xffffff);
		current_capsule.Draw(true,   0, 0xffffff);
		future_capsule .Draw(true,   0, 0xffffff);
		fix_capsule    .Draw(true,   0, 0xffffff);
		result_capsule .Draw(true,   0, 0xffffff);



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
	// TODO : 後にコマンド化
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
