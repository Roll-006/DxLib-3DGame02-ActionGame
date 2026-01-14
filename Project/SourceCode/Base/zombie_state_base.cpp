#include "../Object/zombie.hpp"
#include "../Animator/animator.hpp"
#include "../State/ZombieState/zombie_state.hpp"
#include "../Kind/zombie_state_kind.hpp"
#include "../Kind/zombie_anim_kind.hpp"
#include "zombie_state_base.hpp"

ZombieStateBase::ZombieStateBase(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator, const ZombieStateKind state_kind) :
	m_zombie	(zombie),
	m_state		(state),
	m_animator	(animator),
	m_state_kind(state_kind),
	m_prev_run	(false)
{

}

ZombieStateBase::~ZombieStateBase()
{

}

void ZombieStateBase::Move()
{
	const auto target_pos		= m_zombie.GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto look_dir			= m_zombie.GetLookDir(TimeKind::kCurrent);
	const auto move_dir			= v3d::GetNormalizedV(m_zombie.GetMoveVelocity());

	// 8方向で45°刻み
	const auto angle_diff		= math::GetSignedAngleBetweenTwoVectorXZ(look_dir, move_dir);
	const auto is_move_forward	= angle_diff <  67.5f * math::kDegToRad && angle_diff >  -67.5f * math::kDegToRad;
	const auto is_move_backward = angle_diff > 112.5f * math::kDegToRad || angle_diff < -112.5f * math::kDegToRad;
	const auto is_move_left		= angle_diff >  22.5f * math::kDegToRad && angle_diff <  157.5f * math::kDegToRad;
	const auto is_move_right	= angle_diff < -22.5f * math::kDegToRad && angle_diff > -157.5f * math::kDegToRad;

	m_animator->DivideFrame(FramePath.HIPS);

	// ダッシュ
	if (m_state.TryRun())
	{
		//printfDx("RUN\n");
		m_zombie.CalcMoveSpeedRun();
		m_zombie.SyncMoveDirWithLookDir();
		m_zombie.UpdateLocomotion();
		m_prev_run = true;

		// アニメーションアタッチ
		m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun));
	}
	// 歩く
	else if (m_state.TryWalk())
	{
		//printfDx("WALK\n");
		m_zombie.CalcMoveSpeed();
		m_zombie.LookAtTarget(target_pos);
		m_zombie.UpdateLocomotion();
		m_prev_run = false;

		// アニメーションアタッチ
		m_animator->AttachAnimEightDir(static_cast<int>(ZombieAnimKind::kMoveForward), is_move_forward, is_move_backward, is_move_left, is_move_right, false);
		m_animator->AttachAnim		  (static_cast<int>(ZombieAnimKind::kMoveForwardWalk), Animator::BodyKind::kUpperBody);
	}
	// どちらでもない場合は以前の状態を引き継ぐ
	else if(m_prev_run)
	{
		//printfDx("RUN\n");
		m_zombie.CalcMoveSpeedRun();
		m_zombie.SyncMoveDirWithLookDir();
		m_zombie.UpdateLocomotion();

		// アニメーションアタッチ
		m_animator->AttachResultAnim(static_cast<int>(ZombieAnimKind::kMoveForwardRun));
	}
	else
	{
		//printfDx("WALK\n");
		m_zombie.CalcMoveSpeed();
		m_zombie.LookAtTarget(target_pos);
		m_zombie.UpdateLocomotion();

		// アニメーションアタッチ
		m_animator->AttachAnimEightDir(static_cast<int>(ZombieAnimKind::kMoveForward), is_move_forward, is_move_backward, is_move_left, is_move_right, false);
		m_animator->AttachAnim		  (static_cast<int>(ZombieAnimKind::kMoveForwardWalk), Animator::BodyKind::kUpperBody);
	}
}
