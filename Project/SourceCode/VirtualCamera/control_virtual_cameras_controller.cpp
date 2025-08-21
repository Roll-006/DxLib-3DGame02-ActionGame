#include "control_virtual_cameras_controller.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Object/player.hpp"
#include "../Part/player_state_controller.hpp"

ControlVirtualCamerasController::ControlVirtualCamerasController(Player& player) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kControl),
	m_controller_handle				(HandleCreator::GetInstance()->CreateHandle()),
	m_is_active						(true),
	m_player						(player),
	m_rot_control_camera			(std::make_shared<VirtualCamera>(ObjName.ROT_CONTROL_VIRTUAL_CAMERA, BlendActivationPolicyKind::kKeepOriginCamera)),
	m_aim_control_camera			(std::make_shared<VirtualCamera>(ObjName.AIM_CONTROL_VIRTUAL_CAMERA, BlendActivationPolicyKind::kKeepOriginCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_current_aim_pos				(v3d::GetZeroV()),
	m_move_dir						(v3d::GetZeroV()),
	m_velocity						(v3d::GetZeroV()),
	m_recoil_data					(RecoilData()),
	m_is_recoiling					(false),
	m_is_reached_recoil_peak		(true)
{
	// パラメータ設定
	SetupForRotCamera();
	SetupForAimCamera();
	
	const auto camera_brain = CinemachineBrain::GetInstance();
	camera_brain->SetBlendTime(1.0f);
	camera_brain->AddVirtualCamera(m_rot_control_camera, true);
	camera_brain->AddVirtualCamera(m_aim_control_camera, false);
}

ControlVirtualCamerasController::~ControlVirtualCamerasController()
{
	const auto camera_brain = CinemachineBrain::GetInstance();
	camera_brain->RemoveVirtualCamera(m_rot_control_camera->GetObjHandle());
	camera_brain->RemoveVirtualCamera(m_aim_control_camera->GetObjHandle());
}

void ControlVirtualCamerasController::Init()
{

}

void ControlVirtualCamerasController::Update()
{
	if (!IsActive()) { return; }

	m_move_dir = v3d::GetZeroV();
	m_velocity = v3d::GetZeroV();

	Move();
}

void ControlVirtualCamerasController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimPos();
	CalcRecoilAngle();
	CalcResultAngle();

	MATRIX result_m = MGetIdent();
	CreateRotationXYZMatrix(&result_m, m_result_angle.x, m_result_angle.y, m_result_angle.z);
	m_aim_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(result_m));
}

void ControlVirtualCamerasController::OnRecoil(const GunBase& gun)
{
	const auto rand_gen			= RandomGenerator::GetInstance();
	m_recoil_data				= gun.GetRecoilData();
	m_is_recoiling				= true;
	m_is_reached_recoil_peak	= false;
	m_recoil_timer				= 0.0f;

	m_input_angle[TimeKind::kCurrent] = m_result_angle;

	// リコイル値の取得
	float recoil_pitch			= 1.0f;
	float recoil_yaw			= rand_gen->GetRandClosedOpen(m_recoil_data.min_yaw, m_recoil_data.max_yaw);
	recoil_pitch				*= m_recoil_data.pitch;
	recoil_yaw					*= m_recoil_data.yaw;

	m_recoil_angle[TimeKind::kNext] = VGet(-recoil_pitch, recoil_yaw, 0.0f);
}

VirtualCameraControllerKind ControlVirtualCamerasController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

std::shared_ptr<VirtualCameraBase> ControlVirtualCamerasController::GetHaveVirtualCamera(const std::string& name) const
{
	const auto camera_brain = CinemachineBrain::GetInstance();
	const auto camera = camera_brain->GetVirtualCamera(name);

	if (   camera == m_rot_control_camera
		|| camera == m_aim_control_camera)
	{
		return camera;
	}

	return nullptr;
}

std::vector<std::shared_ptr<VirtualCameraBase>> ControlVirtualCamerasController::GetHaveAllVirtualCamera() const
{
	return std::vector<std::shared_ptr<VirtualCameraBase>>{m_rot_control_camera, m_aim_control_camera};
}
		
void ControlVirtualCamerasController::SetupForRotCamera()
{
	m_rot_control_camera->SetPriority(5);
	m_rot_control_camera->AttachTarget(m_aim_transform);

	const auto body = m_rot_control_camera->GetBody();
	body->SetFollowOffset(kFollowOffsetForRotCamera);
	body->SetDamping(kBodyDampingForRotCamera);

	const auto aim  = m_rot_control_camera->GetAim();
	aim->SetTrackedObjOffset(kTrackedObjOffsetForRotCamera);
	aim->SetHorizontalDamping(kHorizontalDampingForRotCamera);
	aim->SetVerticalDamping	(kVerticalDampingForRotCamera);
}

void ControlVirtualCamerasController::SetupForAimCamera()
{
	m_aim_control_camera->SetPriority(4);
	m_aim_control_camera->AttachTarget(m_aim_transform);

	const auto body = m_aim_control_camera->GetBody();
	body->SetFollowOffset(kFollowOffsetForAimCamera);
	body->SetDamping(kBodyDampingForAimCamera);

	const auto aim = m_aim_control_camera->GetAim();
	aim->SetTrackedObjOffset(kTrackedObjOffsetForAimCamera);
	aim->SetHorizontalDamping(kHorizontalDampingForAimCamera);
	aim->SetVerticalDamping(kVerticalDampingForAimCamera);
}

void ControlVirtualCamerasController::Move()
{
	CalcMoveDirFromPad();
	CalcMoveDirFromMouse();
	CalcMoveDirFromCommand();

	CalcInputAngle();
}

void ControlVirtualCamerasController::CalcMoveDirFromPad()
{
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return; }

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const auto up_param		= input->GetInputParameter(pad::StickKind::kRSUp);
	const auto down_param	= input->GetInputParameter(pad::StickKind::kRSDown);
	const auto left_param	= input->GetInputParameter(pad::StickKind::kRSLeft);
	const auto right_param	= input->GetInputParameter(pad::StickKind::kRSRight);

	// 速度ベクトル・入力判定を取得
	if (up_param)	 { m_velocity.x = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  up_param);	  }
	if (down_param)  { m_velocity.x =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -down_param);  }
	if (left_param)  { m_velocity.y = -math::GetUnitValue<int, float>(InputChecker::kStickDeadZone, -InputChecker::kStickMinSlope, -left_param);  }
	if (right_param) { m_velocity.y =  math::GetUnitValue<int, float>(InputChecker::kStickDeadZone,  InputChecker::kStickMaxSlope,  right_param); }
	m_velocity *= kMoveSpeedWithStick;

	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCamerasController::CalcMoveDirFromMouse()
{
	//if (m_is_init_aiming) { return; }
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * kMoveSpeedWithMouse;

	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCamerasController::CalcMoveDirFromCommand()
{
	//if (m_is_init_aiming) { return; }

	const auto input	= InputChecker	::GetInstance();
	const auto command	= CommandHandler::GetInstance();

	// パッド・マウスの入力は例外的にコマンド入力の動きを適用しない
	for (int i = 0; i < 4; ++i) { if (input->IsInput(static_cast<mouse::SlideDirKind>(i)))	{ return; } }
	for (int i = 0; i < 8; ++i) { if (input->IsInput(static_cast<pad::StickKind>(i)))		{ return; } }

	// コマンドパターンで入力された場合の速度・方向を取得
	if (command->IsExecute(CommandKind::kMoveUpCamera,		TimeKind::kCurrent)) { m_move_dir.x = -1; }
	if (command->IsExecute(CommandKind::kMoveDownCamera,	TimeKind::kCurrent)) { m_move_dir.x =  1; }
	if (command->IsExecute(CommandKind::kMoveLeftCamera,	TimeKind::kCurrent)) { m_move_dir.y = -1; }
	if (command->IsExecute(CommandKind::kMoveRightCamera,	TimeKind::kCurrent)) { m_move_dir.y =  1; }

	if (m_move_dir != v3d::GetZeroV())
	{
		m_move_dir = v3d::GetNormalizedV(m_move_dir);
		m_velocity = m_move_dir * kMoveSpeedWithButton;
	}
}

void ControlVirtualCamerasController::CalcAimPos()
{
	const auto modeler = m_player.GetModeler();
	modeler->ApplyMatrix();

	// 追跡するボーンから行列を取得
	const auto	model_handle	= modeler->GetModelHandle();
	const auto	fream_index		= MV1SearchFrame(model_handle, BonePath.SPINE_2);
	auto		fream_world_m	= MV1GetFrameLocalWorldMatrix(model_handle, fream_index);
	auto		aim_pos			= MGetTranslateElem(fream_world_m);

	if (!IsTrackCameraOriginBone())
	{
		// ボーンと同じ高さの位置を追跡
		const auto player_transform = m_player.GetTransform();
		const auto begin_pos		= player_transform->GetPos(CoordinateKind::kWorld);
		const auto distance			= begin_pos - aim_pos;
		aim_pos = begin_pos + player_transform->GetUp(CoordinateKind::kWorld) * VSize(distance);
	}

	m_current_aim_pos = math::GetApproachedVector(m_current_aim_pos, aim_pos, kCameraAimOffsetBasicSpeed * VSize(aim_pos - m_current_aim_pos));

	m_aim_transform->SetPos(CoordinateKind::kWorld, m_current_aim_pos);
}

void ControlVirtualCamerasController::CalcInputAngle()
{
	const auto time_manager = GameTimeManager::GetInstance();
	m_velocity *= time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);

	// 角度を取得
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecute(CommandKind::kMoveUpCamera,		TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveDownCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecute(CommandKind::kMoveLeftCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }
	if (command->IsExecute(CommandKind::kMoveRightCamera,	TimeKind::kCurrent)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }

	m_input_angle[TimeKind::kCurrent].y = math::ConnectMinusPiToPi(m_input_angle[TimeKind::kCurrent].y);

	// 角度制限
	if (m_input_angle[TimeKind::kCurrent].x < kMinVerticalInputAngle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = kMinVerticalInputAngle * math::kDegToRad; }
	if (m_input_angle[TimeKind::kCurrent].x > kMaxVerticalInputAngle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = kMaxVerticalInputAngle * math::kDegToRad; }
}

void ControlVirtualCamerasController::CalcRecoilAngle()
{
	if (!m_is_recoiling) { return; }

	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleController::LayerKind::kCamera);

	// リコイル処理
	if (!m_is_reached_recoil_peak)
	{
		m_recoil_timer += delta_time;
		const auto t = math::GetUnitValue<float, float>(0.0f, m_recoil_data.recoil_time, m_recoil_timer);
		m_recoil_angle[TimeKind::kCurrent] = math::GetLerpVector(v3d::GetZeroV(), m_recoil_angle[TimeKind::kNext], t);

		// 復帰へ移行
		if (t >= 1.0f)
		{
			m_is_reached_recoil_peak = true;
			m_recoil_angle[TimeKind::kNext] = v3d::GetZeroV();
		}
	}
	// 最高地点に到達した場合、元の位置へ復帰させる
	else
	{
		const auto weapon_action_state = m_player.GetStateController()->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind();
		
		// 銃エイミング状態でのみ復帰を許可する
		if (weapon_action_state == static_cast<int>(player_state::WeaponActionStateKind::kAimGun))
		{
			m_recoil_angle[TimeKind::kCurrent] = math::GetDampedValue(
				m_recoil_angle[TimeKind::kCurrent], m_recoil_angle[TimeKind::kNext], m_recoil_data.return_damping, delta_time);
			
			// 終了判定
			if (VSize(m_recoil_angle[TimeKind::kCurrent]) < kRecoilEndThreshold)
			{
				m_recoil_angle[TimeKind::kCurrent] = m_recoil_angle[TimeKind::kNext];

				m_is_recoiling = false;
			}
		}
		else
		{
			m_is_recoiling = false;
		}
	}
}

void ControlVirtualCamerasController::CalcResultAngle()
{
	m_result_angle = m_input_angle[TimeKind::kCurrent] + m_recoil_angle[TimeKind::kCurrent];
}

//void ControlVirtualCameraController::CalcInitAim()
//{
//	//if (!m_data.is_init_aiming) { return; }
//
//	VECTOR distance_v	= m_input_angle[TimeKind::kCurrent] - m_input_angle[TimeKind::kCurrent];
//	distance_v.y		= math::ConnectMinusPiToPi(distance_v.y);
//	VECTOR dir			= v3d::GetNormalizedV(distance_v);
//
//	// 右・左回りから最短経路を取得し、回転方向に反映
//	float distance = VSize(distance_v);
//	const float shortest = min(distance, DX_TWO_PI_F - distance);
//	if (distance != shortest)
//	{
//		dir.y *= -1;
//	}
//
//	// 目的地に遠いほど速く移動させる
//	const auto time_manager = GameTimeManager::GetInstance();
//	m_input_angle[TimeKind::kCurrent] += dir * distance * m_init_angle_speed * time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);
//
//	// 終了判定
//	distance = VSize(m_data.input_angle[TimeKind::kNext] - m_data.input_angle[TimeKind::kCurrent]);
//	if (distance < m_data.init_end_threshold)
//	{
//		m_data.input_angle[TimeKind::kCurrent] = m_data.input_angle.at(TimeKind::kNext);
//		m_data.is_init_aiming = false;
//	}
//}

bool ControlVirtualCamerasController::IsTrackCameraOriginBone() const
{
	const auto state = m_player.GetStateController();
	const auto weapon_state_kind = static_cast<player_state::WeaponActionStateKind>(state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());

	return(weapon_state_kind == player_state::WeaponActionStateKind::kFirstSideSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kSecondSideSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kSpinningSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kStabKnife);
}
