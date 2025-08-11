#include "control_virtual_camera_controller.hpp"
#include "../Manager/camera_manager.hpp"
#include "../Object/player.hpp"
#include "../Part/player_state_controller.hpp"

ControlVirtualCameraController::ControlVirtualCameraController(Player& player) :
	m_virtual_camera_controller_kind(VirtualCameraControllerKind::kControl),
	m_is_active						(true),
	m_player						(player),
	m_rot_control_camera			(std::make_shared<VirtualCamera>(BlendActivationPolicyKind::kKeepOriginCamera)),
	m_aim_control_camera			(std::make_shared<VirtualCamera>(BlendActivationPolicyKind::kKeepOriginCamera)),
	m_aim_transform					(std::make_shared<Transform>()),
	m_current_aim_pos				(v3d::GetZeroV()),
	m_move_dir						(v3d::GetZeroV()),
	m_velocity						(v3d::GetZeroV()),
	m_input_angle					{ {TimeKind::kNext, v3d::GetZeroV()}, {TimeKind::kCurrent, v3d::GetZeroV()} }
{
	// パラメータ設定
	SetupForRotCamera();
	SetupForAimCamera();

	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->SetBlendTime(1.0f);
	camera_manager->AddVirtualCamera(m_rot_control_camera, true);
	camera_manager->AddVirtualCamera(m_aim_control_camera, false);
}

ControlVirtualCameraController::~ControlVirtualCameraController()
{
	const auto camera_manager = CameraManager::GetInstance();
	camera_manager->RemoveVirtualCamera(m_rot_control_camera->GetObjHandle());
	camera_manager->RemoveVirtualCamera(m_aim_control_camera->GetObjHandle());
}

void ControlVirtualCameraController::Init()
{

}

void ControlVirtualCameraController::Update()
{
	if (!IsActive()) { return; }

	m_move_dir = v3d::GetZeroV();
	m_velocity = v3d::GetZeroV();

	Move();
}

void ControlVirtualCameraController::LateUpdate()
{
	if (!IsActive()) { return; }

	CalcAimPos();

	MATRIX input_m = MGetIdent();
	CreateRotationXYZMatrix(&input_m, m_input_angle[TimeKind::kCurrent].x, m_input_angle[TimeKind::kCurrent].y, m_input_angle[TimeKind::kCurrent].z);
	m_aim_transform->SetRot(CoordinateKind::kWorld, MGetRotElem(input_m));
}

VirtualCameraControllerKind ControlVirtualCameraController::GetVirtualCameraControllerKind() const
{
	return m_virtual_camera_controller_kind;
}

void ControlVirtualCameraController::SetupForRotCamera()
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

void ControlVirtualCameraController::SetupForAimCamera()
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

void ControlVirtualCameraController::Move()
{
	CalcMoveDirFromPad();
	CalcMoveDirFromMouse();
	CalcMoveDirFromCommand();

	CalcInputAngle();
}

void ControlVirtualCameraController::CalcMoveDirFromPad()
{
	//if (m_data.is_init_aiming) { return; }
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

	// 入力方向も合わせて取得
	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCameraController::CalcMoveDirFromMouse()
{
	//if (m_is_init_aiming) { return; }
	if (m_move_dir != v3d::GetZeroV()) { return; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kKeyboard) { return; }

	// 移動速度を取得
	Vector2D<float> velocity_2d = InputChecker::GetInstance()->GetMouseVelocity(TimeKind::kCurrent);
	m_velocity = VGet(velocity_2d.y, velocity_2d.x, 0.0f) * kMoveSpeedWithMouse;

	// 入力方向も合わせて取得
	m_move_dir = v3d::GetNormalizedV(m_velocity);
}

void ControlVirtualCameraController::CalcMoveDirFromCommand()
{
	//if (m_is_init_aiming) { return; }

	const auto input	= InputChecker::GetInstance();
	const auto command	= CommandHandler::GetInstance();

	// パッド・マウスの入力は例外的にコマンド入力の動きを適用しない
	for (int i = 0; i < 4; ++i) { if (input->IsInput(static_cast<mouse::SlideDirKind>(i)))	{ return; } }
	for (int i = 0; i < 8; ++i) { if (input->IsInput(static_cast<pad::StickKind>(i)))		{ return; } }

	// コマンドパターンで入力された場合の速度・方向を取得
	if (command->IsExecuting(CommandKind::kMoveUpCamera)) { m_move_dir.x = -1; }
	if (command->IsExecuting(CommandKind::kMoveDownCamera)) { m_move_dir.x = 1; }
	if (command->IsExecuting(CommandKind::kMoveLeftCamera)) { m_move_dir.y = -1; }
	if (command->IsExecuting(CommandKind::kMoveRightCamera)) { m_move_dir.y = 1; }

	if (m_move_dir != v3d::GetZeroV())
	{
		m_move_dir = v3d::GetNormalizedV(m_move_dir);
		m_velocity = m_move_dir * kMoveSpeedWithButton;
	}
}

void ControlVirtualCameraController::CalcInputAngle()
{
	// 視点リセット
	//CalcInitAim();

	const auto time_manager = GameTimeManager::GetInstance();
	m_velocity *= time_manager->GetDeltaTime(TimeScaleController::LayerKind::kCamera);

	// 角度を取得
	const auto command = CommandHandler::GetInstance();
	if (command->IsExecuting(CommandKind::kMoveUpCamera)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecuting(CommandKind::kMoveDownCamera)) { m_input_angle[TimeKind::kCurrent].x += m_velocity.x; }
	if (command->IsExecuting(CommandKind::kMoveLeftCamera)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }
	if (command->IsExecuting(CommandKind::kMoveRightCamera)) { m_input_angle[TimeKind::kCurrent].y += m_velocity.y; }

	m_input_angle[TimeKind::kCurrent].y = math::ConnectMinusPiToPi(m_input_angle[TimeKind::kCurrent].y);

	// 角度制限
	if (m_input_angle[TimeKind::kCurrent].x < kMinVerticalInputAngle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = kMinVerticalInputAngle * math::kDegToRad; }
	if (m_input_angle[TimeKind::kCurrent].x > kMaxVerticalInputAngle * math::kDegToRad) { m_input_angle[TimeKind::kCurrent].x = kMaxVerticalInputAngle * math::kDegToRad; }
}

void ControlVirtualCameraController::CalcAimPos()
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

bool ControlVirtualCameraController::IsTrackCameraOriginBone() const
{
	const auto state = m_player.GetStateController();
	const auto weapon_state_kind = static_cast<player_state::WeaponActionStateKind>(state->GetWeaponActionState(TimeKind::kCurrent)->GetStateKind());

	return(weapon_state_kind == player_state::WeaponActionStateKind::kFirstSideSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kSecondSideSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kSpinningSlashKnife
		|| weapon_state_kind == player_state::WeaponActionStateKind::kStabKnife);
}
