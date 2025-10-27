#include "assault_rifle.hpp"

// MEMO : 見た目上はアサルトライフルとして実装するが、挙動はバイオRE:4のサブマシンガンとする
//		  そのため、GunKindはサブマシンガンとして扱う

AssaultRifle::AssaultRifle() :
	GunBase(ObjName.ASSAULT_RIFLE, GunKind::kSubmachineGun, HolsterKind::kRifle)
{
	m_magazine = std::make_shared<AssaultRifleMagazine>(m_load_transform);
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->AddToObjManager();

	m_modeler = std::make_shared<Modeler>(m_transform, ModelPath.ASSAULT_RIFLE, kBasicAngle, kBasicScale);	
	SetColliderModelHandle(m_modeler->GetModelHandle());

	SetOffset(kHoldOffsetPos,   kHoldOffsetAngle,   kHoldOffsetScale, 
			  kAttachOffsetPos, kAttachOffsetAngle, kAttachOffsetScale);

	m_cross_hair_shape			= std::make_shared<Circle>();
	m_scope_scale				= kScopeScale;
	m_range						= kRange;
	m_power						= kPower;
	m_initial_velocity			= kInitialVelocity;
	m_deceleration				= kDeceleration;
	m_shot_interval_time		= kShotIntervalTime;
	m_recoil_data				= RecoilData(1.0f, -0.3f, 0.5f * math::kDegToRad, 0.7f * math::kDegToRad, 0.2f, VGet(0.5f, 0.5f, 0.5f));

	// TODO : 仮で弾数を設定
	m_max_remaining_bullet_num		= 30;
	m_current_remaining_bullet_num	= m_max_remaining_bullet_num;

	//AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
}

AssaultRifle::~AssaultRifle()
{

}

void AssaultRifle::Init()
{

}

void AssaultRifle::Update()
{
	if (!IsActive()) { return; }

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Update();
}

void AssaultRifle::LateUpdate()
{
	if (!IsActive()) { return; }

	//TrackOwnerHand();
	CalcTransform(m_muzzle_transform,		 kMuzzleOffsetPos);
	CalcTransform(m_ejection_port_transform, kEjectionPortOffsetPos);
	CalcTransform(m_load_transform,			 kLoadPortOffsetPos);

	std::dynamic_pointer_cast<ObjBase>(m_magazine)->LateUpdate();
}

void AssaultRifle::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw();
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void AssaultRifle::InitCrossHairRange()
{
	m_cross_hair_radius = kCrossHairMaxRadius;
}

void AssaultRifle::CalcCrossHairRange(const VECTOR& owner_move_velocity)
{
	// 拡散範囲を指定
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	// カメラのvelocityを取得
	const auto cinemachine_brain	= CinemachineBrain::GetInstance();
	const auto camera_controller	= std::static_pointer_cast<ControlVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kControl));
	const auto camera_velocity		= camera_controller->GetVelocity();
		
	if (owner_move_velocity != v3d::GetZeroV() || camera_velocity != v3d::GetZeroV())
	{
		if (m_cross_hair_radius < kCrossHairMiddleRadius)
		{
			math::Increase(m_cross_hair_radius, 600.0f * GetDeltaTime(), kCrossHairMiddleRadius, false);
		}
		else
		{
			math::Increase(m_cross_hair_radius, 90.0f * GetDeltaTime(), kCrossHairMaxRadius,	false);
		}
	}
	else
	{
		if (m_cross_hair_radius > kCrossHairMiddleRadius)
		{
			math::Decrease(m_cross_hair_radius, 20.0f * GetDeltaTime(), kCrossHairMiddleRadius);
		}
		else
		{
			math::Decrease(m_cross_hair_radius, 300.0f * GetDeltaTime(), kCrossHairMinRadius);
		}
	}

	circle->SetRadius(m_cross_hair_radius);
}

void AssaultRifle::CalcCrossHairRangeShot()
{
	// 拡散範囲を指定
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	if (m_cross_hair_radius < kCrossHairMiddleRadius)
	{
		m_cross_hair_radius = kCrossHairMiddleRadius;
	}
	else
	{
		math::Increase(m_cross_hair_radius, 100.0f * GetDeltaTime(), kCrossHairMaxRadius, false);
	}

	circle->SetRadius(m_cross_hair_radius);
}

void AssaultRifle::CalcCrossHairPos()
{
	// 拡散範囲を指定
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetPos(GetFirstShotPos() + m_aim_dir * kCrossHairDistance);
	circle->SetNormalVector(m_aim_dir);
}

void AssaultRifle::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::static_pointer_cast<Circle>(m_cross_hair_shape));
}
