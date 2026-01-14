#include "../../Object/player.hpp"
#include "../../Kind/player_anim_kind.hpp"
#include "../../Animator/animator.hpp"
#include "../../Kind/player_state_kind.hpp"
#include "player_state.hpp"
#include "player_shot.hpp"

player_state::Shot::Shot(Player& player, State& state, const std::shared_ptr<Animator>& animator) :
    PlayerStateBase(player, state, animator, PlayerStateKind::kShot)
{
    m_basic_anim_kind               .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_walk_forward_anim_kind        .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_run_forward_anim_kind         .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_crouch_walk_forward_anim_kind .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
    m_crouch_anim_kind              .at(Animator::BodyKind::kUpperBody) = PlayerAnimKind::kAimGun;
}

player_state::Shot::~Shot()
{

}

void player_state::Shot::Update()
{
    m_animator->DivideFrame(FramePath.HIPS);
    BasicMove();

    m_player.StopSearchStealthKillTarget();
    m_player.StopSearchMeleeTarget();
    m_player.SetLookDirOffsetValueForAim();

    if (const auto& gun = std::dynamic_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon()))
    {
        gun->CalcShotTimer();
    }
}

void player_state::Shot::LateUpdate()
{
    if (const auto& gun = std::dynamic_pointer_cast<GunBase>(m_player.GetCurrentHeldWeapon()))
    {
        const auto camera = ObjAccessor::GetInstance()->GetObj<ObjBase>(ObjName.MAIN_CAMERA);
        const auto aim_dir = camera->GetTransform()->GetForward(CoordinateKind::kWorld);

        // ボーン位置補正
        m_player.GetFramePosCorrector()->CorrectAimPoseFramePos(m_player.GetModeler()->GetModelHandle(), aim_dir);

        gun->CalcCrossHairPos();
        gun->CalcCrossHairRangeShot();
        gun->CalcTargetPos();
        gun->SetAimDir(aim_dir);
        gun->SetPosOnRay(camera->GetTransform()->GetPos(CoordinateKind::kWorld));

        if (m_player.GetDeltaTime() > 0.0f) { gun->OnShot(); }

        // リコイル処理
        const auto camera_controller = CinemachineBrain::GetInstance()->GetVirtualCameraController(VirtualCameraControllerKind::kControl);
        std::static_pointer_cast<ControlVirtualCamerasController>(camera_controller)->OnRecoil(*gun.get());
    }
}

void player_state::Shot::Enter()
{
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
    const auto rot_camera        = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
    const auto aim_camera        = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

    if (rot_camera) rot_camera->Deactivate();
    if (aim_camera) aim_camera->Activate();

    m_player.DetachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
    m_player.HoldWeapon  (m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

void player_state::Shot::Exit()
{
    const auto cinemachine_brain = CinemachineBrain::GetInstance();
    const auto camera_controller = std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
    const auto rot_camera        = camera_controller->GetHaveVirtualCamera(ObjName.ROT_CONTROL_VIRTUAL_CAMERA);
    const auto aim_camera        = camera_controller->GetHaveVirtualCamera(ObjName.AIM_CONTROL_VIRTUAL_CAMERA);

    if (rot_camera) rot_camera->Activate();
    if (aim_camera) aim_camera->Deactivate();

    m_player.ReleaseWeapon();
    m_player.AttachWeapon(m_player.GetCurrentEquipWeapon(WeaponSlotKind::kMain));
}

const PlayerStateKind player_state::Shot::GetNextStateKind()
{
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

    // 銃エイミング状態に遷移
    return PlayerStateKind::kAimGun;
}
