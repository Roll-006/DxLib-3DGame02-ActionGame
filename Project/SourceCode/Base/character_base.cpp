#include "character_base.hpp"

#pragma region 武器
CharacterBase::CharacterBase(const std::string& name, const std::string& tag) : 
	PhysicalObjBase			(name, tag),
	invincible_time			(0.0f),
	m_modeler				(nullptr),
	m_animator				(nullptr),
	m_collider_creator		(std::make_shared<CharacterColliderCreator>()),
	m_move_dir				{ {TimeKind::kCurrent, v3d::GetZeroV()},{TimeKind::kNext, v3d::GetZeroV()} },
	m_look_dir				{ {TimeKind::kCurrent, v3d::GetZeroV()},{TimeKind::kNext, v3d::GetZeroV()} },
	m_destination_pos		(v3d::GetZeroV()),
	m_move_speed			(0.0f),
	m_move_dir_offset_speed	(0.0f),
	m_look_dir_offset_speed	(0.0f),
	m_is_calc_look_dir		(false),
	m_current_held_weapon	(nullptr),
	m_invincible_timer		(0.0f),
	m_is_invincible			(false)
{
		
}

void CharacterBase::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void CharacterBase::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

void CharacterBase::CalcCorrectMoveDir()
{
	const auto current_pos			= m_transform->GetPos(CoordinateKind::kWorld);
	const auto current_pos_y0		= VGet(current_pos.x, 0.0f, current_pos.z);
	const auto destination_pos_y0	= VGet(m_destination_pos.x, 0.0f, m_destination_pos.z);

	if (VSize(destination_pos_y0 - current_pos_y0) < 1.0f)
	{
		m_move_dir.at(TimeKind::kNext) = v3d::GetZeroV();
	}
	else
	{
		m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(destination_pos_y0 - current_pos_y0);
	}
}

bool CharacterBase::IsGoUpHill(const Triangle& hit_triangle) const
{
	// 法線のyを消し飛ばす
	auto horizontal_v = hit_triangle.GetNormalVector();
	horizontal_v.y = 0.0f;
	horizontal_v = v3d::GetNormalizedV(horizontal_v);

	// 移動方向との角度の差を取得
	const auto angle = math::GetAngleBetweenTwoVector(m_move_dir.at(TimeKind::kCurrent), horizontal_v);

	// 角度の差が90度より大きい場合は上り坂に向かっている
	return angle * math::kRadToDeg > 90.0f;
}

void CharacterBase::ApplyLookDirToRot(const VECTOR& look_dir)
{
	m_transform->SetRot  (CoordinateKind::kWorld, look_dir);
}

void CharacterBase::CalcMoveDir()
{
	// 現在のdirを目的とするdirに近づけていく
	m_move_dir.at(TimeKind::kCurrent) = math::GetApproachedVector(
		m_move_dir.at(TimeKind::kCurrent),
		m_move_dir.at(TimeKind::kNext),
		m_move_dir_offset_speed);
}

void CharacterBase::CalcLookDir()
{
	if (!m_is_calc_look_dir) { return; }

	// ヨー角回転を取得し、-π～πで値を管理する
	const auto current_yaw	= math::GetYawRotVector(m_look_dir.at(TimeKind::kCurrent));
	const auto next_yaw		= math::GetYawRotVector(m_look_dir.at(TimeKind::kNext));
	auto	   distance		= next_yaw - current_yaw;
	distance.y = math::ConnectMinusValueToValue(distance.y, DX_PI_F);

	// カメラを基準にして右側であった場合は反転
	if (distance.y > 0) { m_look_dir_offset_speed *= -1; }

	// 回転を適用
	const auto look_dir_offset_speed = -m_look_dir_offset_speed * GetDeltaTime();
	const auto rot_q = quat::CreateQuaternion(axis::GetWorldYAxis(), look_dir_offset_speed);
	m_look_dir.at(TimeKind::kCurrent) = math::GetRotatedPos(m_look_dir.at(TimeKind::kCurrent), rot_q);

	// 終了判定
	const auto angle = math::GetYawBetweenTwoVector(m_look_dir.at(TimeKind::kNext), m_look_dir.at(TimeKind::kCurrent));
	const auto dynamic_threshold = std::abs(look_dir_offset_speed * math::kStopThreshold);
	if (angle < dynamic_threshold)
	{
		m_look_dir.at(TimeKind::kCurrent) = m_look_dir.at(TimeKind::kNext);
	}
}

void CharacterBase::CalcMoveVelocity()
{
	m_move_velocity = m_move_dir.at(TimeKind::kCurrent) * m_move_speed;
	m_velocity += m_move_velocity;
}

void CharacterBase::JudgeInvincible()
{
	math::Decrease(m_invincible_timer, GetDeltaTime(), 0.0f);
	if (m_invincible_timer <= 0.0f)
	{
		m_is_invincible = false;
	}
}
