#include "main_camera.hpp"
#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Command/command_handler.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Base/enemy_base.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase		(ObjName.MAIN_CAMERA, ObjTag.CAMERA, MassKind::kLight),
	m_aim_pos			(v3d::GetZeroV())
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast,		 std::make_shared<Segment>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kVisionTrigger, std::make_shared<Cone>(v3d::GetZeroV(), v3d::GetZeroV(), kMeleeDistance, kMeleeFOV), this));

	// カメラが無視するコライダー
	const auto collision_manager = CollisionManager::GetInstance();
	const ColliderData camera_data{ ObjTag.CAMERA, ColliderKind::kRayCast };
	collision_manager->AddIgnoreColliderPair(camera_data, { ObjTag.PLAYER,	ColliderKind::kNone });
	collision_manager->AddIgnoreColliderPair(camera_data, { ObjTag.ENEMY,	ColliderKind::kNone });
	collision_manager->AddIgnoreColliderPair(camera_data, { "",				ColliderKind::kAttackTrigger });
}

MainCamera::~MainCamera()
{
	// 処理なし
}

void MainCamera::Init()
{

}

void MainCamera::Update()
{
	if (!IsActive()) { return; }
}

void MainCamera::LateUpdate()
{
	if (!IsActive()) { return; }

	SetAim();

	CalcRayCastPos();
	CalcVisionTriggerPos();
}

void MainCamera::DrawToShadowMap() const
{
	if (!IsActive()) { return; }
}

void MainCamera::Draw() const
{
	if (!IsActive()) { return; }

	for (const auto& collider : m_colliders)
	{
		const auto shape = collider.second->GetShape();
		if (shape != nullptr)
		{
			shape->Draw(true, 0, 0xffffff);
		}
	}
}

void MainCamera::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRayCast:
		if (hit_collider_pair.intersection)
		{
			m_transform->SetPos(CoordinateKind::kWorld, *hit_collider_pair.intersection);
			SetAim();
		}
		break;

	case ColliderKind::kVisionTrigger:
		// メレー可能な状態のキャラクターが視界判定トリガー内に入ったことを通知
		if (target_collider_kind == ColliderKind::kVisibleTrigger)
		{
			const auto melee_hittable = dynamic_cast<IMeleeHittable*>(target_obj);
			if (melee_hittable)
			{
				if (melee_hittable->IsStandStun() || melee_hittable->IsCrouchStun())
				{
					const auto owner_pos		= m_transform->GetPos(CoordinateKind::kWorld);
					const auto target_transform = target_obj->GetTransform();
					const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
					const auto angle			= math::GetAngleBetweenTwoVector(m_transform->GetForward(CoordinateKind::kWorld), v3d::GetNormalizedV(target_pos - owner_pos));

					const OnDownedEnemySpottedEvent event{ target_obj->GetObjHandle(), angle, VSize(target_pos - owner_pos)};
					EventSystem::GetInstance()->Publish(event);
				}
			}
		}
		break;

	default:
		break;
	}
}

void MainCamera::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj				(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj		(physical_obj);
	PhysicsManager	::GetInstance()->AddIgnoreObjGravity(GetObjHandle());

	CinemachineBrain::GetInstance()->SetMainCamera(std::static_pointer_cast<MainCamera>(shared_from_this()));
}

void MainCamera::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	PhysicsManager	::GetInstance()->RemoveIgnoreObjGravity	(obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj		(obj_handle);
	CollisionManager::GetInstance()->RemoveCollideObj		(obj_handle);
	ObjManager		::GetInstance()->RemoveObj				(obj_handle);
}

void MainCamera::ApplyMatrix(const MATRIX& matrix)
{
	m_transform->SetMatrix(CoordinateKind::kWorld, matrix);
}

float MainCamera::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);
}

void MainCamera::SetAim()
{
	const VECTOR pos		= m_transform->GetPos(CoordinateKind::kWorld);
	const VECTOR target_pos	= pos + m_transform->GetForward(CoordinateKind::kWorld);

	SetCameraPositionAndTarget_UpVecY(pos, target_pos);
}

void MainCamera::CalcRayCastPos()
{
	// 光線の座標を計算
	auto ray = std::static_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	ray->SetBeginPos(m_aim_pos, true);
	ray->SetEndPos	(m_transform->GetPos(CoordinateKind::kWorld), true);
}

void MainCamera::CalcVisionTriggerPos()
{
	const auto cone = std::static_pointer_cast<Cone>(GetCollider(ColliderKind::kVisionTrigger)->GetShape());
	cone->SetVertex	(m_transform->GetPos	(CoordinateKind::kWorld));
	cone->SetDir	(m_transform->GetForward(CoordinateKind::kWorld));
}
