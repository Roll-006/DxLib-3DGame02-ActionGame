#include "roundhouse_kick.hpp"

player_state::RoundhouseKick::RoundhouseKick() :
	ActionStateBase			(static_cast<int>(player_state::ActionStateKind::kRoundhouseKick)),
	m_is_stop_all_state		(true),
	m_has_trigger_created	(false),
	m_has_trigger_deleted	(false)
{

}

player_state::RoundhouseKick::~RoundhouseKick()
{

}

void player_state::RoundhouseKick::Update(std::shared_ptr<Player>& obj)
{
	obj->UpdateMelee();

	// 攻撃判定用トリガーを追加
	if (!m_has_trigger_created)
	{
		if (obj->GetAnimator()->GetPlayRate(AnimatorBase::BodyKind::kUpperBody) > 0.35f)
		{
			obj->AddCollider(std::make_shared<Collider>(ColliderKind::kAttackTrigger, std::make_shared<Capsule>(v3d::GetZeroV(), v3d::GetZeroV(), kAttackTriggerRadius), obj.get()));
			m_has_trigger_created = true;
		}
	}

	// 攻撃判定用トリガーを削除
	if (!m_has_trigger_deleted)
	{
		if (obj->GetAnimator()->GetPlayRate(AnimatorBase::BodyKind::kUpperBody) > 0.8f)
		{
			obj->RemoveCollider(ColliderKind::kAttackTrigger);
			m_has_trigger_deleted = true;
		}
	}

	// 座標計算
	if (m_has_trigger_created && !m_has_trigger_deleted)
	{
		obj->GetModeler()->ApplyMatrix();
		const auto model_handle = obj->GetModeler()->GetModelHandle();

		// 行列情報を取得
		auto right_leg_m	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_LEG));
		auto right_foot_m	= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, BonePath.RIGHT_FOOT));

		// 位置を取得
		const auto right_leg_pos	= MGetTranslateElem(right_leg_m);
		const auto right_foot_pos	= MGetTranslateElem(right_foot_m);

		// 位置を適用
		const auto capsule = std::static_pointer_cast<Capsule>(obj->GetCollider(ColliderKind::kAttackTrigger)->GetShape());

		capsule->SetSegmentBeginPos	(right_leg_pos,  true);
		capsule->SetSegmentEndPos	(right_foot_pos, true);
	}
}

void player_state::RoundhouseKick::LateUpdate(std::shared_ptr<Player>& obj)
{

}

void player_state::RoundhouseKick::Enter(std::shared_ptr<Player>& obj)
{
	m_has_trigger_created = false;
	m_has_trigger_deleted = false;

	obj->SetupVersatilityMelee();
}

void player_state::RoundhouseKick::Exit(std::shared_ptr<Player>& obj)
{

}

std::shared_ptr<IState<Player>> player_state::RoundhouseKick::ChangeState(std::shared_ptr<Player>& obj)
{
	if (obj->GetDeltaTime() <= 0.0f) { return nullptr; }

	const auto state_controller = obj->GetStateController();
	
	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
