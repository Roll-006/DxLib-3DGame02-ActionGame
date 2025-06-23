#include "player.hpp"
#include "../Manager/command_handler.hpp"

Player::Player(std::shared_ptr<Camera> camera) :
	CharaBase				(ObjName.PLAYER, ObjTag.PLAYER, ModelPath.CHARA_01, MassKind::kMedium),
	m_camera				(camera),
	m_bone_pos_corrector	(std::make_shared<BonePosCorrector>()),
	m_move_speed			(0.0f),
	m_non_move_time			(0.0f),
	m_is_move				(false),
	m_is_run				(false),
	m_is_squat				(false),
	m_is_ready_gun			(false),
	m_is_turn_around		(false),
	m_is_turn_run			(false),
	m_is_correct_look_dir	(false),
	m_turn_around_count		(0)
{
	// 初期pos・dirを設定
	m_move_dir[TimeKind::kPrev] = m_move_dir[TimeKind::kCurrent] = m_move_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_look_dir[TimeKind::kCurrent] = m_look_dir[TimeKind::kNext] = VGet(0.0f, 0.0f, 1.0f);
	m_transform->SetRot  (CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	m_transform->SetPos  (CoordinateKind::kWorld, VGet(0, 100, 0));

	// コライダー・トリガーを設定
	MakeCapsuleCollider(kCapsuleRadius);
	MakeLandingTrigger (kLandingTriggerRadius);

	// 各アニメーション追加
	LoadAnim();
	m_anim_kind[TimeKind::kPrev] = m_anim_kind[TimeKind::kCurrent] = PlayerAnimKind::kIdle02;
	m_animator->AttachAnim(static_cast<int>(m_anim_kind.at(TimeKind::kCurrent)));

	// 武器設定
	AddGun(std::make_shared<AssaultRifle>());
	AttachGun(GunKind::kAssaultRifle);
}

Player::~Player()
{

}

void Player::Init()
{
	
}

void Player::Update()
{
	// アニメーション情報をシフト
	if (m_anim_kind.at(TimeKind::kPrev) != m_anim_kind.at(TimeKind::kCurrent))
	{
		m_anim_kind.at(TimeKind::kPrev) = m_anim_kind.at(TimeKind::kCurrent);
	}

	InitMove();

	const auto command = CommandHandler::GetInstance();
	command->Execute(CommandKind::kRun,				this);
	command->Execute(CommandKind::kSquat,			this);
	command->Execute(CommandKind::kReadyGun,		this);
	command->Execute(CommandKind::kMoveUpPlayer,	this);
	command->Execute(CommandKind::kMoveDownPlayer,	this);
	command->Execute(CommandKind::kMoveLeftPlayer,	this);
	command->Execute(CommandKind::kMoveRightPlayer, this);

	// MEMO : velocityの適用までに遅延が生じる
	// 

	Move();
	UpdateTransform();
	//ApplyVelocityToCollider();

	// ボーン位置修正
	m_bone_pos_corrector->CorrectGunPoseBone(
		m_modeler->GetModelHandle(), 
		m_look_dir.at(TimeKind::kCurrent),
		m_camera->GetTransform()->GetMatrix(CoordinateKind::kWorld), 
		m_is_ready_gun);

	ChangeAnimState();
	m_animator->Update();

	CalcCapsuleLength();

	m_current_attach_gun->TrackOwner();

	// TODO : 後に位置変更
	m_is_landing = false;
}

void Player::Draw() const
{
	m_modeler->Draw();
	m_current_attach_gun->Draw();

	for (auto& collider : m_collider)
	{
		collider->GetShape()->Draw(true, 0, 0xffffff);
	}

	auto pos  = m_transform->GetPos (CoordinateKind::kWorld);
	auto axes = m_transform->GetAxes(CoordinateKind::kWorld);
	DrawLine3D(pos, pos + axes.x_axis * 100, 0xff0000);
	DrawLine3D(pos, pos + axes.y_axis * 100, 0x00ff22);
	DrawLine3D(pos, pos + axes.z_axis * 100, 0x0077ff);

	DrawFormatString(0, 300, 0xffffff, "%f", m_fall_speed);
}

void Player::OnCollide(const ColliderPairData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kCollider:
		break;

	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	case ColliderKind::kHeadTrigger:
		break;

	default:
		break;
	}
}


#pragma region コマンド
void Player::Run()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	const auto command = CommandHandler::GetInstance();
	const auto input   = InputChecker  ::GetInstance();
	const auto code    = *command->GetCurrentFrameExecuteInputCode(CommandKind::kRun);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kRun))
	{
	case InputModeKind::kTrigger:
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kRun);

			m_is_run = command->GetTriggerCount(CommandHandler::MoveKind::kRun) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		if (input->GetInputState(code) == InputState::kHold)
		{
			m_is_run = true;
		}
		break;
	}

	// ダッシュ状態解除
	if (m_is_run)
	{
		m_is_squat = false;
		command->InitTriggerCount(CommandHandler::MoveKind::kSquat);
	}
}

void Player::Squat()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	const auto command = CommandHandler::GetInstance();
	const auto input = InputChecker::GetInstance();
	const auto code = *command->GetCurrentFrameExecuteInputCode(CommandKind::kSquat);

	switch (command->GetInputModeKind(CommandHandler::MoveKind::kSquat))
	{
	case InputModeKind::kTrigger:
		if (input->GetInputState(code) == InputState::kSingle)
		{
			command->CountUpTrigger(CommandHandler::MoveKind::kSquat);

			m_is_squat = command->GetTriggerCount(CommandHandler::MoveKind::kSquat) % 2 ? true : false;
		}
		break;

	case InputModeKind::kHold:
		if (input->GetInputState(code) == InputState::kHold)
		{
			m_is_squat = true;
		}
		break;
	}

	// しゃがみ状態解除
	if (m_is_squat)
	{
		m_is_run = false;
		command->InitTriggerCount(CommandHandler::MoveKind::kRun);
	}
}

void Player::MoveForward()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	m_is_input_move.at(static_cast<int>(MoveDir::kForward)) = true;

	m_move_dir.at(TimeKind::kNext) += GetMoveForward();
}

void Player::MoveBackward()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) = true;

	m_move_dir.at(TimeKind::kNext) -= GetMoveForward();
}

void Player::MoveLeft()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	m_is_input_move.at(static_cast<int>(MoveDir::kLeft)) = true;

	m_move_dir.at(TimeKind::kNext) -= m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
}

void Player::MoveRight()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	m_is_input_move.at(static_cast<int>(MoveDir::kRight)) = true;

	m_move_dir.at(TimeKind::kNext) += m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
}

void Player::ReadyGun()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }

	m_is_ready_gun			= true;
	m_is_correct_look_dir	= true;

	// 拡大率から実際の距離を取得
	float min_distance = Camera::kNormalDistance / m_current_attach_gun->GetScopeScale();
	m_camera->Approach(min_distance, kAimDownSightsSpeed * FPS::GetDeltaTime());
	m_camera->TrackBoneWobbly();

	// ダッシュ状態を解除
	m_is_run = false;
	CommandHandler::GetInstance()->InitTriggerCount(CommandHandler::MoveKind::kRun);
}

void Player::Shot()
{
	// ターン中は早期return
	if (m_is_turn_around) { return; }
}

void Player::TurnAround()
{
	if (m_is_turn_around)										{ return; }
	if (m_is_run)												{ return; }
	if (m_turn_around_count != 0)								{ return; }
	if (m_move_dir.at(TimeKind::kNext) == v3d::GetZeroV()) { return; }

	// バック中であった場合は振り向く
	const float angle = math::GetAngleBetweenTwoVector(-GetMoveForward(), m_move_dir.at(TimeKind::kNext));
	if (angle < kTurnAroundStickAngle * math::kDegreesToRadian)
	{
		m_look_dir.at(TimeKind::kNext) = -m_look_dir.at(TimeKind::kCurrent);

		m_is_turn_around = true;
		++m_turn_around_count;
	}
}
#pragma endregion


void Player::LoadAnim()
{
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle01),					AnimPath.IDLE_01,						0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdle02),					AnimPath.IDLE_02,						0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleSquat01),				AnimPath.IDLE_SQUAT_01,					0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleSquatShoot01),			AnimPath.IDLE_SQUAT_SHOOT_01,			0, AnimTag.NONE, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kIdleShoot01),				AnimPath.IDLE_SHOOT_01,					0, AnimTag.NONE, 20.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForward01),		AnimPath.WALK_SQUAT_FORWARD_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackward01),		AnimPath.WALK_SQUAT_BACKWARD_01,		0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatLeft01),			AnimPath.WALK_SQUAT_LEFT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatRight01),			AnimPath.WALK_SQUAT_RIGHT_01,			0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForwardLeft01),	AnimPath.WALK_SQUAT_FORWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatForwardRight01),	AnimPath.WALK_SQUAT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackwardLeft01),	AnimPath.WALK_SQUAT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 20.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkSquatBackwardRight01),	AnimPath.WALK_SQUAT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 20.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForward01),		AnimPath.WALK_SHOOT_FORWARD_01,			0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackward01),		AnimPath.WALK_SHOOT_BACKWARD_01,		0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootLeft01),			AnimPath.WALK_SHOOT_LEFT_01,			0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootRight01),			AnimPath.WALK_SHOOT_RIGHT_01,			0, AnimTag.WALK, 40.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardLeft01),	AnimPath.WALK_SHOOT_FORWARD_LEFT_01,	0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootForwardRight01),	AnimPath.WALK_SHOOT_FORWARD_RIGHT_01,	0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardLeft01),	AnimPath.WALK_SHOOT_BACKWARD_LEFT_01,	0, AnimTag.WALK, 30.0f, true);
	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kWalkShootBackwardRight01),	AnimPath.WALK_SHOOT_BACKWARD_RIGHT_01,	0, AnimTag.WALK, 30.0f, true);

	m_animator->AddAnimHandle(static_cast<int>(PlayerAnimKind::kRunForward01),				AnimPath.RUN_FORWARD_01,				0, AnimTag.WALK, 40.0f, true);
}

void Player::ChangeAnimState()
{
	// TODO : のちにイベントでアニメーションを管理

	// アイドル
	if (!m_is_move)
	{
		if (m_non_move_time > kIdelAnimPlayThreshold)
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kIdle02;

			if (m_is_squat)
			{
				m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kIdleSquat01;
			}
		}

		if (m_is_ready_gun)
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kIdleShoot01;
		}
	}

	// ダッシュ
	if (m_is_run)
	{
		m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kRunForward01;
	}

	// 銃を構えながら歩く
	if (m_is_move && !m_is_run)
	{
		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootForward01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootBackward01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootRight01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)) && m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootForwardLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)) && m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootForwardRight01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) && m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootBackwardLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) && m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkShootBackwardRight01;
		}
	}

	// しゃがむ
	if (m_is_squat)
	{
		if (m_is_ready_gun)
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kIdleSquatShoot01;
		}

		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatForward01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatBackward01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatRight01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)) && m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatForwardLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kForward)) && m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatForwardRight01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) && m_is_input_move.at(static_cast<int>(MoveDir::kLeft)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatBackwardLeft01;
		}
		if (m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) && m_is_input_move.at(static_cast<int>(MoveDir::kRight)))
		{
			m_anim_kind.at(TimeKind::kCurrent) = PlayerAnimKind::kWalkSquatBackwardRight01;
		}
	}

	// 状態を反映
	m_animator->AttachAnim(static_cast<int>(m_anim_kind.at(TimeKind::kCurrent)));
}

void Player::Move()
{
	VECTOR velocity = v3d::GetNormalizedV(m_move_dir.at(TimeKind::kNext)) * InputChecker::kStickMaxSlope;
	velocity = GetVelocityFromPad(velocity);

	// 移動判定
	m_is_move = velocity != v3d::GetZeroV() ? true : false;

	if (m_is_move)
	{
		m_non_move_time = 0.0f;
		m_is_correct_look_dir = true;
	}
	else
	{
		m_non_move_time += FPS::GetDeltaTime();
	}

	// 移動速度・方向を計算
	CalcMoveSpeed(VSize(velocity));
	CalcMoveDir(velocity);
	CalcLookDir();

	// 速度を取得
	m_velocity = m_move_dir.at(TimeKind::kCurrent) * m_move_speed;


	// MEMO : 落下ベクトルの適用は、押し戻しと同時に全オブジェクトに一斉に適用
	//m_velocity += m_fall_velocity;
	//
	//// TODO : 仮の押し戻し処理
	//PhysicsManager::GetInstance()->PushBack(this);
	//
	//
	//
	//// 振り向き判定
	//// TODO : 後に振り向きダッシュに使用
	//if (m_is_run)
	//{
	//	const auto angle = math::GetAngleBetweenTwoVector(m_move_dir.at(TimeKind::kNext), m_move_dir.at(TimeKind::kCurrent));
	//	if (angle >= 120.0f * math::kDegreesToRadian)
	//	{
	//		m_is_turn_run = true;
	//		DrawFormatString(0, 60, 0xffffff, "振り向き");
	//	}
	//}
}

void Player::InitMove()
{
	const auto command = CommandHandler::GetInstance();

	for (auto& is_input : m_is_input_move) { is_input = false; }

	// 移動方向
	m_move_dir.at(TimeKind::kPrev) = m_move_dir.at(TimeKind::kCurrent);
	m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();

	// 向き補正
	m_is_correct_look_dir = false;

	// 照準
	if (!m_is_ready_gun)
	{
		m_camera->Depart(Camera::kNormalDistance, kAimDownSightsSpeed * FPS::GetDeltaTime());
		m_camera->TrackBoneHeightOnly();
	}

	m_is_ready_gun = false;

	// ダッシュ判定
	if (command->GetInputModeKind(CommandHandler::MoveKind::kRun) == InputModeKind::kHold) { m_is_run = false; }
	if (!m_is_move)
	{
		m_is_run = false;
		command->InitTriggerCount(CommandHandler::MoveKind::kRun);
	}

	// しゃがみ判定
	if (command->GetInputModeKind(CommandHandler::MoveKind::kSquat) == InputModeKind::kHold) { m_is_squat = false; }
}

void Player::CalcMoveSpeed(const float input_slope)
{
	// 移動していない場合は速度を0に設定
	if (!m_is_move)
	{
		m_move_speed = 0.0f;
		return;
	}

	// しゃがみ処理
	if (m_is_squat)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

		math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kSquatWalkSpeed);
		return;
	}

	// ゆっくり歩く処理
	// スティック入力でしか実行されない
	if (input_slope <= kWalkStickSlopeLimit - InputChecker::kStickDeadZone)
	{
		// 速い状態から歩き状態に移行した場合、急速に減速させる
		if (m_move_speed > kWalkSpeed) { m_move_speed = kWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kSlowWalkSpeed);
		math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kSlowWalkSpeed);
		return;
	}

	// 歩き処理
	if (!m_is_run)
	{
		// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
		if (m_move_speed < kSlowWalkSpeed) { m_move_speed = kSlowWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kWalkSpeed);
		math::Decrease(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kWalkSpeed);
 		return;
	}

	// ダッシュ処理
	if (m_is_run)
	{
		// 遅い状態からダッシュ状態に移行した場合、急速に加速させる
		if (m_move_speed < kWalkSpeed) { m_move_speed = kWalkSpeed; }

		math::Increase(m_move_speed, kAcceleration * FPS::GetDeltaTime(), kRunSpeed);
	}
}

void Player::CalcMoveDir(const VECTOR& velocity)
{
	if (!m_is_move) { return; }

	//if (m_is_run && m_camera->IsLookSameDirTarget())
	//{
	//	int a = 0;
	//}

	// 目的とする向きと距離を取得
	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(velocity);
	const VECTOR distance_v = m_move_dir.at(TimeKind::kNext) - m_move_dir.at(TimeKind::kCurrent);

	// 現在のdirを目的とするdirに近づけていく
	m_move_dir.at(TimeKind::kCurrent) += v3d::GetNormalizedV(distance_v) * kMoveDirCorrectionSpeed;
	const float distance = VSize(m_move_dir.at(TimeKind::kNext) - m_move_dir.at(TimeKind::kCurrent));
	if (distance < kConfirmMoveDirThreshold)
	{
		m_move_dir.at(TimeKind::kCurrent) = m_move_dir.at(TimeKind::kNext);
	}
}

void Player::CalcLookDir()
{
	// 振り向き処理
	const auto command = CommandHandler::GetInstance();
	command->Execute(CommandKind::kTurnAround, this);

	// 連続振り向きを阻止
	if (!command->GetCurrentFrameExecuteInputCode(CommandKind::kTurnAround))
	{
		m_turn_around_count = 0;
	}

	if (m_is_turn_around)
	{
		m_camera->InitYaw();
		m_is_correct_look_dir = true;
	}
	else
	{
		if (m_is_move)
		{
			// ダッシュ状態であれば進行方向を向く
			if (m_is_run)
			{
				m_look_dir.at(TimeKind::kNext) = m_move_dir.at(TimeKind::kCurrent);
			}
			// 歩き状態である場合は常にカメラと同じ向きを向く
			else
			{
				m_look_dir.at(TimeKind::kNext) = GetMoveForward();
			}
		}
	}

	// 銃を構えている場合は常にカメラと同じ向きを向く
	if (m_is_ready_gun)
	{
		m_look_dir.at(TimeKind::kNext) = GetMoveForward();
	}

	// 滑らかにdirを補正
	CorrectLookDir();
}

void Player::CorrectLookDir()
{
	// ヨー角回転を取得し、-π～πで値を管理する
	/*VGet(0.0f, atan2f(m_look_dir.at(TimeKind::kCurrent).x, m_look_dir.at(TimeKind::kCurrent).z), 0.0f);*/
	const VECTOR current_yaw	= math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const VECTOR next_yaw		= math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	VECTOR distance = next_yaw - current_yaw;
	distance.y = math::ConnectMinusPiToPi(distance.y);

	// スコープを覗く場合は速度を上昇させる
	float add_angle			= m_is_ready_gun ? -kLookDirCorrectionAngleForADS		: -kLookDirCorrectionAngle;
	float threshold_angle	= m_is_ready_gun ? kConfirmLookDirThresholdAngleForADS	: kConfirmLookDirThresholdAngle;

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { add_angle *= -1; }

	// 回転を適用
	const Quaternion rot_q = quat::MakeQuaternion(axis::GetWorldYAxis(), add_angle);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	const float angle = math::GetAngleBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	if (angle < threshold_angle * math::kDegreesToRadian)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);

		// 振り向き判定がfalseになることにより適用処理が通らなくなることを避けるため先行して適用
		// TODO : 通常時は二度適用処理が行われているため処理の変更を検討
		UpdateTransform();
		ApplyVelocityToCollider();

		// 目的のdirに達した場合は振り向き処理は終了とする
		m_is_turn_around = false;
	}
}

VECTOR Player::GetVelocityFromPad(VECTOR& velocity)
{
	if (velocity != v3d::GetZeroV()) { return velocity; }
	if (InputChecker::GetInstance()->GetCurrentInputDevice() != DeviceKind::kPad) { return velocity; }

	// 移動方向を取得
	const VECTOR right = m_camera->GetTransform()->GetRight(CoordinateKind::kWorld);
	VECTOR forward = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;
	forward = v3d::GetNormalizedV(forward);

	// 各方向のパラメーターを取得
	const auto input = InputChecker::GetInstance();
	const int forward_param	 = input->GetInputParameter(pad::StickKind::kLSUp   );
	const int backward_param = input->GetInputParameter(pad::StickKind::kLSDown );
	const int left_param	 = input->GetInputParameter(pad::StickKind::kLSLeft );
	const int right_param	 = input->GetInputParameter(pad::StickKind::kLSRight);

	// 速度ベクトルを取得
	if (forward_param)
	{
		velocity += forward * (forward_param - InputChecker::kStickDeadZone);
		m_is_input_move.at(static_cast<int>(MoveDir::kForward)) = true;
	}
	if (backward_param)
	{
		velocity += forward * (backward_param + InputChecker::kStickDeadZone);
		m_is_input_move.at(static_cast<int>(MoveDir::kBackward)) = true;
	}
	if (left_param)
	{
		velocity += right * (left_param + InputChecker::kStickDeadZone);
		m_is_input_move.at(static_cast<int>(MoveDir::kLeft)) = true;
	}
	if (right_param)
	{
		velocity += right * (right_param - InputChecker::kStickDeadZone);
		m_is_input_move.at(static_cast<int>(MoveDir::kRight)) = true;
	}

	return velocity;
}

void Player::UpdateTransform()
{
	m_transform->SetRot  (CoordinateKind::kWorld, m_look_dir.at(TimeKind::kCurrent));
	m_transform->SetScale(CoordinateKind::kWorld, kModelScale);
	//m_transform->SetPos  (CoordinateKind::kWorld, m_transform->GetPos(CoordinateKind::kWorld) + m_velocity);
}

VECTOR Player::GetMoveForward()
{
	VECTOR forward = m_camera->GetTransform()->GetForward(CoordinateKind::kWorld);
	forward.y = 0.0f;

	return v3d::GetNormalizedV(forward);
}
