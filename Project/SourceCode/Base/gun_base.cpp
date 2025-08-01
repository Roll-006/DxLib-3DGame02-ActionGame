#include "gun_base.hpp"

GunBase::GunBase(const std::string& name, const GunKind gun_kind, const HolsterKind holster_kind, const std::string& file_path) :
	WeaponBase						(name, WeaponKind::kGun, holster_kind, file_path),
	m_diffusion_shape				(nullptr),
	m_aim_dir						(v3d::GetZeroV()),
	m_target_pos					(v3d::GetZeroV()),
	m_muzzle_correct_pos			(v3d::GetZeroV()),
	m_ejection_port_correct_pos		(v3d::GetZeroV()),
	m_point_on_ray_line				(v3d::GetZeroV()),
	m_current_remaining_bullet_num	(0),
	m_max_remaining_bullet_num		(0),
	m_scope_scale					(0.0f),
	m_range							(0.0f),
	m_initial_velocity				(0.0f),
	m_deceleration					(0.0f),
	m_shot_interval_time			(0.0f),
	m_shot_timer					(0.0f),
	m_is_shot						(false),
	m_on_pull_trigger				(false),
	m_on_aiming						(false),
	m_gun_kind						(gun_kind)
{
	// 処理なし
}

void GunBase::OnReload(int& have_bullets)
{
	// 既に最大値の場合はリロードさせない
	if (m_current_remaining_bullet_num >= m_max_remaining_bullet_num) { return; }

	// 不足分の計算
	const int shortage_num = m_max_remaining_bullet_num - m_current_remaining_bullet_num;

	// 可能な数だけリロードし、所持している弾丸を減少させる
	if (have_bullets < shortage_num)
	{
		have_bullets = 0;
		m_current_remaining_bullet_num += have_bullets;
	}
	else
	{
		have_bullets -= shortage_num;
		m_current_remaining_bullet_num += shortage_num;
	}
}

VECTOR GunBase::GetShotDir() const
{
	return v3d::GetNormalizedV(m_target_pos - GetFirstShotPos());
}

VECTOR GunBase::GetMuzzlePos() const
{
	const auto world_m		= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos	= m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_muzzle_correct_pos, world_m);
}

VECTOR GunBase::GetEjectionPortPos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_ejection_port_correct_pos, world_m);
}

VECTOR GunBase::GetFirstShotPos() const
{
	// 操作キャラの場合は銃口からカメラforwardに投影
	// 非操作キャラの場合はそのまま銃口
	Segment s1 = Segment(m_point_on_ray_line, m_point_on_ray_line + m_aim_dir);
	Segment s2 = Segment(m_point_on_ray_line, GetMuzzlePos());
	VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
	VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
	VECTOR  h  = math::GetProjectionVector(v2, v1);

	return s1.GetBeginPos() + h;
}

void GunBase::Shot()
{
	// 弾丸がない場合は早期return
	if (m_current_remaining_bullet_num <= 0)
	{
		return;
	}

	// トリガーが引かれていない場合は早期return
	if (!m_on_pull_trigger)
	{
		m_shot_timer = m_shot_interval_time;
		m_is_shot    = false;
		return;
	}

	// エイミング中のみ発射タイマーを増加
	math::IncreaseLoop(m_shot_timer, FPS::GetDeltaTime(), m_shot_interval_time, true);
	m_is_shot = m_shot_timer == 0.0f ? true : false;

	// 発射呼び出し
	if (m_is_shot)
	{
		RifleCartridgeManager::GetInstance()->Shot(*this);
		--m_current_remaining_bullet_num;
	}
}
