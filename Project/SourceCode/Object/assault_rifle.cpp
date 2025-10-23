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

void AssaultRifle::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->DrawToShadowMap();
}

void AssaultRifle::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
	std::dynamic_pointer_cast<ObjBase>(m_magazine)->Draw();

	//if(GetMuzzleTransform())	  DrawSphere3D(GetMuzzleTransform()		 ->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, FALSE);
	//if(GetEjectionPortTransform())DrawSphere3D(GetEjectionPortTransform()->GetPos(CoordinateKind::kWorld), 1, 8, 0xffffff, 0xffffff, FALSE);
	//if(GetLoadTransform())		  DrawSphere3D(GetLoadTransform()		 ->GetPos(CoordinateKind::kWorld), 1, 8, 0xffffff, 0xffffff, FALSE);
}

void AssaultRifle::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{

}

void AssaultRifle::CalcCrossHairRange(const VECTOR& owner_velocity)
{
	// 拡散範囲を指定
	const auto circle = std::static_pointer_cast<Circle>(m_cross_hair_shape);

	circle->SetPos			(GetFirstShotPos() + m_aim_dir * kCrossHairDistance);
	circle->SetRadius		(kCrossHairMaxRadius);
	circle->SetNormalVector	(m_aim_dir);


}

void AssaultRifle::CalcTargetPos()
{
	m_target_pos = math::GetRandomPointInCircle(*std::static_pointer_cast<Circle>(m_cross_hair_shape));
}
