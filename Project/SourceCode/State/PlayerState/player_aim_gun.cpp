#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "../../Command/command_handler.hpp"
#include "player_state.hpp"
#include "player_aim_gun.hpp"

player_state::AimGun::AimGun(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator) :
	PlayerStateBase	(player, state, animator, PlayerStateKind::kAimGun),
	m_elapsed_time	(0.0f)
{
	m_basic_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
	m_walk_forward_anim_kind		.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
	m_run_forward_anim_kind			.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
	m_crouch_walk_forward_anim_kind	.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
	m_crouch_anim_kind				.at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
}

player_state::AimGun::~AimGun()
{
}

void player_state::AimGun::Update()
{
	m_animator->DivideFrame(FramePath.HIPS);
	Move();

	m_elapsed_time += m_player.GetDeltaTime();

	m_player.StopSearchStealthKillTarget();
	m_player.StopSearchMeleeTarget();
	m_player.AllowCalcLookDir();
	m_player.SetLookDirOffsetValueForAim();
	m_player.DirOfCameraForward();

	const auto gun = std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
	gun->CalcShotTimer();
}

void player_state::AimGun::LateUpdate()
{
	const auto gun		= std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
	const auto camera	= ObjAccessor::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
	const auto aim_dir	= camera->GetTransform()->GetForward(CoordinateKind::kWorld);

	m_player.GetFramePosCorrector()->CorrectAimPoseFramePos(m_player.GetModeler()->GetModelHandle(), aim_dir);

	if (m_state.GetPrevStateKind() == PlayerStateKind::kShot)
	{
		if (m_elapsed_time > kWaitTime)
		{
			gun->CalcCrossHairRange(m_player.GetMoveVelocity());
		}
	}
	else
	{
		gun->CalcCrossHairRange(m_player.GetMoveVelocity());
	}

	gun->CalcCrossHairPos();
	gun->CalcTargetPos();
	gun->SetAimDir(aim_dir);
	gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));
}

void player_state::AimGun::Enter()
{
	m_elapsed_time = 0.0f;

	// カメラ設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	cinemachine_brain->SetBlendTime(0.3f);
	const auto rot_camera = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
	const auto aim_camera = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

	if (rot_camera) { rot_camera->Deactivate(); }
	if (aim_camera) { aim_camera->Activate(); }

	m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
	m_player.HoldWeapon  (m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));

	const auto state_kind = m_state.GetCurrentStateKind();
	if (state_kind != PlayerStateKind::kShot)
	{
		const auto gun = std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());
		gun->InitCrossHairRange();

		// エイミング状態通知
		EventSystem::GetInstance()->Publish(AimGunEvent(gun->GetTransform()->GetPos(CoordinateKind::kWorld), TimeScaleLayerKind::kPlayer));
	}
}

void player_state::AimGun::Exit()
{
	// カメラ設定
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	cinemachine_brain->SetBlendTime(0.3f);
	const auto rot_camera = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
	const auto aim_camera = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

	if (rot_camera) { rot_camera->Activate(); }
	if (aim_camera) { aim_camera->Deactivate(); }

	m_player.ReleaseWeapon();
	m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

const PlayerStateKind player_state::AimGun::GetNextStateKind()
{
	const auto cinemachine_brain = CinemachineBrain::GetInstance();
	const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	const auto command			 = CommandHandler::GetInstance();
	const auto gun				 = std::static_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon());

	if (m_player.GetDeltaTime() <= 0.0f)
	{
		return PlayerStateKind::kNone;
	}

	// 勝利ポーズ
	else if (m_player.IsVictoryPose())
	{
		return PlayerStateKind::kVictoryPose;
	}
	// 死亡
	else if (m_state.TryDead())
	{
		return PlayerStateKind::kDead;
	}
	// 捕まれる
	else if (m_state.TryGrabbed())
	{
		return PlayerStateKind::kGrabbed;
	}

	// 銃装備状態
	if (!command->IsExecute(CommandKind::kAimGun, TimeKind::kCurrent) && camera_controller->IsReachedRecoilPeak())
	{
		return PlayerStateKind::kEquipGun;
	}
	// リロード
	if (m_player.CanControl() && m_state.TryReload() && !camera_controller->IsRecoiling())
	{
		return PlayerStateKind::kReload;
	}
	// リロード
	if (m_state.TryPullTriggerReload())
	{
		return PlayerStateKind::kReload;
	}
	// ショット
	else if (m_state.TryPullTrigger())
	{
		if (gun->IsShot() && m_animator->GetBlendRate(Animator::BodyKind::kUpperBody) >= 1.0f)
		{
			// ロケットランチャーショット(必殺技)
			if (gun->GetGunKind() == GunKind::kRocketLauncher)
			{
				return PlayerStateKind::kShotRocketLauncher;
			}
			// 通常ショット
			else
			{
				return PlayerStateKind::kShot;
			}
		}
	}

	return PlayerStateKind::kNone;
}
