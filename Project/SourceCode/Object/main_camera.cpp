#include "main_camera.hpp"

#include "../VirtualCamera/cinemachine_brain.hpp"
#include "../Command/command_handler.hpp"
#include "../Interface/i_melee_hittable.hpp"
#include "../Interface/i_stealth_killable.hpp"
#include "../Base/enemy_base.hpp"

MainCamera::MainCamera() : 
	PhysicalObjBase				(ObjName.MAIN_CAMERA, ObjTag.CAMERA),
	m_aim_pos					(v3d::GetZeroV()),
	m_collider_radius			(0.0f),
	m_is_active_grab_collider	(false),
	m_grabbed_obj_handle		(-1)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<GrabEvent>	(this, &MainCamera::CreateGrabCollider);
	EventSystem::GetInstance()->Subscribe<OnGrabEvent>	(this, &MainCamera::CreateGrabCollider);
	EventSystem::GetInstance()->Subscribe<ReleaseEvent>	(this, &MainCamera::DeleteGrabCollider);

	mass_kind = MassKind::kHeavy;

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast,				std::make_shared<Segment>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kNearVisionTrigger,	std::make_shared<Cone>(v3d::GetZeroV(), v3d::GetZeroV(), kMeleeCandidateDistance,	kMeleeCandidateFOV * math::kDegToRad), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kFarVisionTrigger,		std::make_shared<Cone>(v3d::GetZeroV(), v3d::GetZeroV(), kMeleeTargetDistance,		kMeleeTargetFOV	   * math::kDegToRad), this));

	// カメラが無視するコライダー
	const auto collision_manager = CollisionManager::GetInstance();
	const ColliderData ray_cast_data{ ObjTag.CAMERA, ColliderKind::kRayCast };
	collision_manager->AddIgnoreCollider(GetObjHandle(), ColliderKind::kCollider);
	collision_manager->AddIgnoreColliderPair(ray_cast_data, { ObjTag.PLAYER,	ColliderKind::kNone });
	collision_manager->AddIgnoreColliderPair(ray_cast_data, { ObjTag.ENEMY,		ColliderKind::kNone });
	collision_manager->AddIgnoreColliderPair(ray_cast_data, { ObjTag.BULLET,	ColliderKind::kNone });
	collision_manager->AddIgnoreColliderPair(ray_cast_data, { "",				ColliderKind::kAttackTrigger });
}

MainCamera::~MainCamera()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<GrabEvent>		(this, &MainCamera::CreateGrabCollider);
	EventSystem::GetInstance()->Unsubscribe<OnGrabEvent>	(this, &MainCamera::CreateGrabCollider);
	EventSystem::GetInstance()->Unsubscribe<ReleaseEvent>	(this, &MainCamera::DeleteGrabCollider);
}

void MainCamera::Init()
{
	m_is_active_grab_collider	= false;
	m_grabbed_obj_handle		= -1;

	RemoveCollider(ColliderKind::kCollider);
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
	CalcGrabColliderPosAndRadius();
}

void MainCamera::Draw() const
{
	if (!IsActive()) { return; }

	DrawColliders();
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

	case ColliderKind::kNearVisionTrigger:
		if (target_collider_kind == ColliderKind::kVisibleTrigger)
		{
			// メレー可能な状態のキャラクターが視界判定トリガー内に入ったことを通知
			const auto melee_hittable = dynamic_cast<IMeleeHittable*>(target_obj);
			if (melee_hittable)
			{
				if (melee_hittable->IsStandStun() || melee_hittable->IsCrouchStun())
				{
					const auto owner_pos		= m_transform->GetPos(CoordinateKind::kWorld);
					const auto target_transform = target_obj->GetTransform();
					const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
					const auto angle			= math::GetAngleBetweenTwoVector(m_transform->GetForward(CoordinateKind::kWorld), v3d::GetNormalizedV(target_pos - owner_pos));

					const OnDownedNearEnemySpottedEvent event{ target_obj->GetObjHandle(), angle, VSize(target_pos - owner_pos)};
					EventSystem::GetInstance()->Publish(event);
				}
			}

			// ステルスキル可能なキャラクターが視界判定トリガー内に入ったことを通知
			const auto stealth_killable = dynamic_cast<IStealthKillable*>(target_obj);
			if (stealth_killable)
			{
				const auto owner_pos		= m_transform->GetPos(CoordinateKind::kWorld);
				const auto target_transform = target_obj->GetTransform();
				const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
				const auto angle			= math::GetAngleBetweenTwoVector(m_transform->GetForward(CoordinateKind::kWorld), v3d::GetNormalizedV(target_pos - owner_pos));

				const OnNearEnemySpottedEvent event{ target_obj->GetObjHandle(), angle, VSize(target_pos - owner_pos) };
				EventSystem::GetInstance()->Publish(event);
			}
		}
		break;

	case ColliderKind::kFarVisionTrigger:
		if (target_collider_kind == ColliderKind::kVisibleTrigger)
		{
			// メレー可能な状態のキャラクターが視界判定トリガー内に入ったことを通知
			const auto melee_hittable = dynamic_cast<IMeleeHittable*>(target_obj);
			if (melee_hittable)
			{
				if (melee_hittable->IsStandStun() || melee_hittable->IsCrouchStun())
				{
					const auto owner_pos		= m_transform->GetPos(CoordinateKind::kWorld);
					const auto target_transform = target_obj->GetTransform();
					const auto target_pos		= target_transform->GetPos(CoordinateKind::kWorld);
					const auto angle			= math::GetAngleBetweenTwoVector(m_transform->GetForward(CoordinateKind::kWorld), v3d::GetNormalizedV(target_pos - owner_pos));

					const OnDownedFarEnemySpottedEvent event{ target_obj->GetObjHandle(), angle, VSize(target_pos - owner_pos) };
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


#pragma region Event
void MainCamera::CreateGrabCollider(const GrabEvent& event)
{
	m_collider_radius			= 0.0f;
	m_is_active_grab_collider	= true;

	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, std::make_shared<Sphere>(m_aim_pos, m_collider_radius), this));

	PhysicsManager::GetInstance()->AddIgnorePushBackPair(GetObjHandle(), event.obj_handle);
}

void MainCamera::CreateGrabCollider(const OnGrabEvent& event)
{
	m_grabbed_obj_handle = event.obj_handle;

	PhysicsManager::GetInstance()->AddIgnorePushBackPair(GetObjHandle(), event.obj_handle);
}

void MainCamera::DeleteGrabCollider(const ReleaseEvent& event)
{
	m_is_active_grab_collider = false;

	RemoveCollider(ColliderKind::kCollider);

	PhysicsManager::GetInstance()->RemoveIgnorePushBackPair(GetObjHandle(), event.obj_handle);
	PhysicsManager::GetInstance()->RemoveIgnorePushBackPair(GetObjHandle(), m_grabbed_obj_handle);
}
#pragma endregion


#pragma region Getter
float MainCamera::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kCamera);
}
#pragma endregion


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
	const auto near_cone = std::static_pointer_cast<Cone>(GetCollider(ColliderKind::kNearVisionTrigger)->GetShape());
	near_cone->SetVertex(m_transform->GetPos	(CoordinateKind::kWorld));
	near_cone->SetDir	(m_transform->GetForward(CoordinateKind::kWorld));

	const auto far_cone = std::static_pointer_cast<Cone>(GetCollider(ColliderKind::kFarVisionTrigger)->GetShape());
	far_cone->SetVertex	(m_transform->GetPos	(CoordinateKind::kWorld));
	far_cone->SetDir	(m_transform->GetForward(CoordinateKind::kWorld));
}

void MainCamera::CalcGrabColliderPosAndRadius()
{
	if (!m_is_active_grab_collider) { return; }

	math::Increase(m_collider_radius, 80.0f * GetDeltaTime(), 300.0f, false);

	const auto sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollider)->GetShape());
	sphere->SetPos	 (m_aim_pos);
	sphere->SetRadius(m_collider_radius);
}
