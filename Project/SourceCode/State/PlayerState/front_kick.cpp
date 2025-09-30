#include "front_kick.hpp"

player_state::FrontKick::FrontKick() :
	ActionStateBase			(static_cast<int>(player_state::ActionStateKind::kFrontKick)),
	m_is_all_stop			(true),
	m_has_trigger_created	(false),
	m_has_trigger_deleted	(false)
{

}

player_state::FrontKick::~FrontKick()
{

}

void player_state::FrontKick::Update(std::shared_ptr<Player> obj)
{
	obj->UpdateMelee();

	// 攻撃判定用トリガーを追加
	if (!m_has_trigger_created)
	{
		if (obj->GetAnimator()->GetPlayRate(AnimatorBase::BodyKind::kUpperBody) > 0.3f)
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

void player_state::FrontKick::LateUpdate(std::shared_ptr<Player> obj)
{

}

void player_state::FrontKick::Enter(std::shared_ptr<Player> obj)
{
	m_has_trigger_created = false;
	m_has_trigger_deleted = false;

	const auto target_obj		= ObjManager::GetInstance()->GetObj<ObjBase>(obj->GetMeleeCandidate().front().target_obj_handle);
	const auto target_transform = target_obj->GetTransform();

	obj->AttackFrontMelee(target_transform->GetPos(CoordinateKind::kWorld), target_transform->GetForward(CoordinateKind::kWorld));
}

void player_state::FrontKick::Exit(std::shared_ptr<Player> obj)
{

}

std::shared_ptr<IState<Player>> player_state::FrontKick::ChangeState(std::shared_ptr<Player> obj)
{
	const auto state_controller = obj->GetStateController();

	// NULL
	if (obj->GetAnimator()->IsPlayEnd(AnimatorBase::BodyKind::kUpperBody))
	{
		return state_controller->GetState<ActionNull, Player>();
	}

	return nullptr;
}
