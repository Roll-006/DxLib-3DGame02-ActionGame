#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Event/event_system.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_shot_rocket_launcher.hpp"

player_state::ShotRocketLauncher::ShotRocketLauncher(Player& player, State& state, const std::shared_ptr<Animator>& animator) :
    PlayerStateBase                     (player, state, animator, PlayerStateKind::kShotRocketLauncher),
    m_rocket_launcher_camera_controller (nullptr),
    m_wait_timer                        (0.0f),
    m_was_shot                          (false)
{
    m_basic_anim_kind               .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_walk_forward_anim_kind        .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_run_forward_anim_kind         .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_crouch_walk_forward_anim_kind .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_crouch_anim_kind              .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
}

player_state::ShotRocketLauncher::~ShotRocketLauncher()
{

}

void player_state::ShotRocketLauncher::Update()
{
    m_animator->DivideFrame(FramePath.HIPS);
    BasicMove();

    m_wait_timer += GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kCamera);

    m_player.StopSearchStealthKillTarget();
    m_player.StopSearchMeleeTarget();
    m_player.SetLookDirOffsetValueForAim();

    if (const auto& gun = std::dynamic_pointer_cast<RocketLauncher>(m_player.GetCurrentHeldWeapon()))
    {
        gun->CalcShotTimer();
    }
}

void player_state::ShotRocketLauncher::LateUpdate()
{
    if (const auto& rocket_launcher = std::dynamic_pointer_cast<RocketLauncher>(m_player.GetCurrentHeldWeapon()))
    {
        const auto camera = ObjAccessor::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);

        // ボーン位置補正
        m_player.GetFramePosCorrector()->CorrectAimPoseFramePos(m_player.GetModeler()->GetModelHandle(), rocket_launcher->GetAimDir());

        rocket_launcher->CalcCrossHairPos();
        rocket_launcher->CalcCrossHairRangeShot();
        rocket_launcher->CalcTargetPos();
        rocket_launcher->SetAimDir(rocket_launcher->GetAimDir());
        rocket_launcher->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));

        // 発射
        if (m_wait_timer > kShotWaitTime && !m_was_shot)
        {
            rocket_launcher->OnShot();

            const RocketLauncherShotEvent event{ rocket_launcher->GetOwnerName(), rocket_launcher->GetExhaustVentTransform() };
            EventSystem::GetInstance()->Publish(event);

            m_was_shot = true;
        }
    }
}

void player_state::ShotRocketLauncher::Enter()
{
    m_wait_timer = 0.0f;
    m_was_shot = false;

    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    m_rocket_launcher_camera_controller = std::make_shared<RocketLauncherVirtualCamerasController>(m_player);
    cinemachine_brain->AddVirtualCameraController(m_rocket_launcher_camera_controller);

    m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
    m_player.HoldWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

void player_state::ShotRocketLauncher::Exit()
{
    // 演出用カメラを削除
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    cinemachine_brain->RemoveVirtualCameraController(m_rocket_launcher_camera_controller);
    m_rocket_launcher_camera_controller = nullptr;

    m_player.ReleaseWeapon();
    m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

const PlayerStateKind player_state::ShotRocketLauncher::GetNextStateKind()
{
    if (m_player.GetDeltaTime() <= 0.0f)
    {
        return PlayerStateKind::kNone;
    }
    else if (m_rocket_launcher_camera_controller->IsEndExitRot())
    {
        return PlayerStateKind::kAimGun;
    }

    return PlayerStateKind::kNone;
}
