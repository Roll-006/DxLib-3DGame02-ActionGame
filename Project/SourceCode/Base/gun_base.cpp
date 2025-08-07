#include "gun_base.hpp"

GunBase::GunBase(const std::string& name, const GunKind gun_kind, const HolsterKind holster_kind) :
	WeaponBase						(name, WeaponKind::kGun, holster_kind),
	m_subject						(std::make_shared<Subject<GunBase>>()),
	m_diffusion_shape				(nullptr),
	m_aim_dir						(v3d::GetZeroV()),
	m_target_pos					(v3d::GetZeroV()),
	m_muzzle_offset_pos				(v3d::GetZeroV()),
	m_ejection_port_offset_pos		(v3d::GetZeroV()),
	m_point_on_ray					(v3d::GetZeroV()),
	m_current_remaining_bullet_num	(0),
	m_max_remaining_bullet_num		(0),
	m_scope_scale					(0.0f),
	m_range							(0.0f),
	m_initial_velocity				(0.0f),
	m_deceleration					(0.0f),
	m_shot_interval_time			(0.0f),
	m_shot_timer					(0.0f),
	m_on_pull_trigger				(false),
	m_gun_kind						(gun_kind)
{
	//m_subject->AddObserver(EffectManager);
}

void GunBase::OnShot()
{
	RifleCartridgeManager::GetInstance()->Shot(*this);
	--m_current_remaining_bullet_num;

	m_subject->Notify(*this, EventKind::kShot);
}

int GunBase::OnReload(const int have_bullets)
{
	int remaining_bullets_num = have_bullets;

	// 既に最大値の場合はリロードさせない
	if (m_current_remaining_bullet_num >= m_max_remaining_bullet_num) { return remaining_bullets_num; }

	// 不足分の計算
	const int shortage_num = m_max_remaining_bullet_num - m_current_remaining_bullet_num;

	// 可能な数だけリロードし、所持している弾丸を減少させる
	if (remaining_bullets_num < shortage_num)
	{
		remaining_bullets_num = 0;
		m_current_remaining_bullet_num += have_bullets;
	}
	else
	{
		remaining_bullets_num -= shortage_num;
		m_current_remaining_bullet_num += shortage_num;
	}

	return remaining_bullets_num;
}


#pragma region Getter
VECTOR GunBase::GetShotDir() const
{
	return v3d::GetNormalizedV(m_target_pos - GetFirstShotPos());
}

VECTOR GunBase::GetMuzzlePos() const
{
	const auto world_m		= m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos	= m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_muzzle_offset_pos, world_m);
}

VECTOR GunBase::GetEjectionPortPos() const
{
	const auto world_m   = m_transform->GetMatrix(CoordinateKind::kWorld);
	const auto local_pos = m_transform->GetPos   (CoordinateKind::kLocal);

	return local_pos + VTransformSR(m_ejection_port_offset_pos, world_m);
}

VECTOR GunBase::GetFirstShotPos() const
{
	// 操作キャラの場合は銃口からカメラforwardに投影
	// 非操作キャラの場合はそのまま銃口
	Segment s1 = Segment(m_point_on_ray, m_point_on_ray + m_aim_dir);
	Segment s2 = Segment(m_point_on_ray, GetMuzzlePos());
	VECTOR  v1 = s1.GetEndPos() - s1.GetBeginPos();
	VECTOR  v2 = s2.GetEndPos() - s2.GetBeginPos();
	VECTOR  h  = math::GetProjectionVector(v2, v1);

	return s1.GetBeginPos() + h;
}

bool GunBase::IsShot() const
{
	if (m_current_remaining_bullet_num > 0 && m_shot_timer == 0.0f && m_on_pull_trigger)
	{
		return true;
	}
	return false;
}

bool GunBase::CanReload() const
{
	return m_current_remaining_bullet_num < m_max_remaining_bullet_num;
}
#pragma endregion


void GunBase::CalcShotTimer()
{
	// TODO : 連射が可能なため後に仕様変更
	if (m_on_pull_trigger)
	{
		math::Increase(m_shot_timer, FPS::GetDeltaTime(), m_shot_interval_time, true);
	}
	else
	{
		m_shot_timer = 0.0f;
	}
}
