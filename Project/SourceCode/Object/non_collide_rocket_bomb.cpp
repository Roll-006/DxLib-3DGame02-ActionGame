#include "non_collide_rocket_bomb.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

NonCollildeRocketBomb::NonCollildeRocketBomb() :
	PhysicalObjBase			(ObjName.ROCKET_BOMB, ObjTag.BULLET, MassKind::kLight),
	m_modeler				(std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_BOMB, kBasicAngle, kBasicScale)),
	m_time_scale_owner_name	(""),
	m_move_dir				(v3d::GetZeroV()),
	m_first_pos				(v3d::GetZeroV()),
	m_move_speed			(0.0f),
	m_deceleration			(0.0f)
{
	SetColliderModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
}

NonCollildeRocketBomb::~NonCollildeRocketBomb()
{

}

void NonCollildeRocketBomb::Init()
{
	m_time_scale_owner_name = "";
	m_velocity				= v3d::GetZeroV();
	m_fall_velocity			= v3d::GetZeroV();
}

void NonCollildeRocketBomb::Update()
{
	if (!IsActive()) { return; }
}

void NonCollildeRocketBomb::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMoveDirToRot();
	Move();
	CalcRayPos();
}

void NonCollildeRocketBomb::DrawToShadowMap() const
{
	if (!IsActive()) { return; }

	m_modeler->DrawToShadowMap();
}

void NonCollildeRocketBomb::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawSphere3D(m_transform->GetPos(CoordinateKind::kWorld), 2, 8, 0xffffff, 0xffffff, TRUE);
	//std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape())->Draw(false, 0, 0xffffff);
}

void NonCollildeRocketBomb::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			RifleCartridgeManager::GetInstance()->DeleteBullet(shared_from_this());
			RifleCartridgeManager::GetInstance()->AddHitPos(*hit_collider_pair.intersection);

			EffectManager::GetInstance()->ForciblyReturnPoolEffect(GetObjHandle());
		}
		break;

	default:
		break;
	}
}

void NonCollildeRocketBomb::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj						 (shared_from_this());
	PhysicsManager	::GetInstance()->AddPhysicalObj				 (physical_obj);
	PhysicsManager	::GetInstance()->AddIgnoreObjPhysicalBehavior(GetObjHandle());
}

void NonCollildeRocketBomb::OnShot(GunBase& gun)
{
	m_time_scale_owner_name = gun.GetOwnerName();

	m_transform->SetRot(CoordinateKind::kWorld, MGetIdent());
	m_first_pos		= gun.GetFirstShotPos();
	m_transform->SetPos(CoordinateKind::kWorld, m_first_pos);
	m_move_dir		= gun.GetTransform()->GetForward(CoordinateKind::kWorld);
	m_move_speed	= gun.GetInitialVelocity();
	m_deceleration	= gun.GetDeceleration();
}

void NonCollildeRocketBomb::OnReload(const std::shared_ptr<Modeler> owner_modeler)
{
	m_owner_modeler = owner_modeler;
}

void NonCollildeRocketBomb::CompletedReload()
{

}

float NonCollildeRocketBomb::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleController::LayerKind::kWorld);
}

bool NonCollildeRocketBomb::IsReturnPool()
{
	return false;
}

void NonCollildeRocketBomb::ApplyMoveDirToRot()
{
	if (m_velocity != v3d::GetZeroV())
	{
		const auto forward = v3d::GetNormalizedV(m_velocity);

		if (forward != axis::GetWorldYAxis() && forward != -axis::GetWorldYAxis())
		{
			m_transform->SetRot(CoordinateKind::kWorld, forward);
		}
	}
	else
	{
		m_transform->SetRot(CoordinateKind::kWorld, m_move_dir);
	}
}

void NonCollildeRocketBomb::Move()
{
	math::Decrease(m_move_speed, m_deceleration, 0.0f);
	m_velocity = m_move_dir * m_move_speed;
}

void NonCollildeRocketBomb::CalcRayPos()
{

}

void NonCollildeRocketBomb::TrackOwnerHand()
{
	if (!m_owner_modeler) { return; }
	m_owner_modeler->ApplyMatrix();

	// アタッチする部位の行列情報を取り出す
	const auto owner_attach_frame_num = MV1SearchFrame(m_owner_modeler->GetModelHandle(), BonePath.LEFT_HAND);
	const auto owner_attach_frame_mat = MV1GetFrameLocalWorldMatrix(m_owner_modeler->GetModelHandle(), owner_attach_frame_num);

	// 武器をアタッチする部位に合わせて回転し、行列を取得
	const auto offset_angle_mat = math::ConvertEulerAnglesToXYZRotMatrix(kHoldOffsetAngle);
	const auto result_mat = offset_angle_mat * owner_attach_frame_mat;

	// 情報を適用
	m_transform->SetMatrix	(CoordinateKind::kWorld, result_mat);
	m_transform->SetPos		(CoordinateKind::kLocal, m_transform->GetPos(CoordinateKind::kLocal) + VTransformSR(kHoldOffsetPos, result_mat));
	m_transform->SetScale	(CoordinateKind::kWorld, kHoldOffsetScale);
}

void NonCollildeRocketBomb::TrackMuzzle()
{

}
