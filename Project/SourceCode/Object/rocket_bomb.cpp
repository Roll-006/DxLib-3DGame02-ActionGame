#include "rocket_bomb.hpp"
#include "../Manager/rifle_cartridge_manager.hpp"
#include "../Base/gun_base.hpp"

RocketBomb::RocketBomb() :
	PhysicalObjBase			(ObjName.ROCKET_BOMB, ObjTag.BULLET),
	m_modeler				(std::make_shared<Modeler>(m_transform, ModelPath.ROCKET_BOMB, kBasicAngle, kBasicScale)),
	m_shot_owner_name		(""),
	m_move_dir				(v3d::GetZeroV()),
	m_prev_pos				(v3d::GetZeroV()),
	m_first_pos				(v3d::GetZeroV()),
	m_move_speed			(0.0f),
	m_deceleration			(0.0f),
	m_range					(0.0f),
	m_power					(0.0f)
{
	mass_kind = MassKind::kLight;

	SetColliderModelHandle(m_modeler->GetModelHandle());

	AddCollider(std::make_shared<Collider>(ColliderKind::kRay, std::make_shared<Segment>(), this));
}

RocketBomb::~RocketBomb()
{

}

void RocketBomb::Init()
{
	m_shot_owner_name		= "";
	m_move_velocity			= v3d::GetZeroV();
	m_fall_velocity			= v3d::GetZeroV();
	m_blend_timer			= 0.0f;
	m_go_straight_timer		= 0.0f;
}

void RocketBomb::Update()
{
	if (!IsActive()) { return; }

	// 位置情報をシフト
	m_prev_pos = m_transform->GetPos(CoordinateKind::kWorld);
}

void RocketBomb::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMoveDirToRot();
	Move();
	CalcRayCastPos();
}

void RocketBomb::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();
}

void RocketBomb::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kRay:
		if (hit_collider_pair.intersection)
		{
			RifleCartridgeManager::GetInstance()->DeleteBullet(shared_from_this());

			const OnHitBulletEvent event{ GetName(), *hit_collider_pair.intersection, m_move_dir };
			EventSystem::GetInstance()->Publish(event);

			// TODO : ロケランが着弾した際にエフェクトを強制的に削除する処理を行っているが、
			//		　射程距離を超えた際に削除する処理は書かなくていいのか？
			EffectManager::GetInstance()->ForciblyReturnPoolEffect(GetObjHandle(), ObjectPoolName.PLAY_SCENE_EFFECT_POOL);
		}
		break;

	default:
		break;
	}
}

void RocketBomb::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj			(shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj	(physical_obj);
	PhysicsManager	::GetInstance()->AddPhysicalObj	(physical_obj);
}

void RocketBomb::RemoveToObjManager()
{
	const auto obj_handle = GetObjHandle();

	CollisionManager::GetInstance()->RemoveCollideObj (obj_handle);
	PhysicsManager	::GetInstance()->RemovePhysicalObj(obj_handle);
	ObjManager		::GetInstance()->RemoveObj		  (obj_handle);
}

void RocketBomb::OnShot(GunBase& gun)
{
	m_shot_owner_name	= gun.GetOwnerName();

	m_transform->SetMatrix(CoordinateKind::kWorld, gun.GetMuzzleTransform()->GetMatrix(CoordinateKind::kWorld));
	m_first_pos			= m_transform->GetPos(CoordinateKind::kWorld) + gun.GetTransform()->GetForward(CoordinateKind::kWorld);
	m_transform->SetPos(CoordinateKind::kWorld, m_first_pos);
	m_prev_pos			= m_first_pos;
	m_move_dir			= gun.GetTransform()->GetForward(CoordinateKind::kWorld);
	m_destination_dir	= gun.GetShotDir();
	m_move_speed		= gun.GetInitialVelocity();
	m_deceleration		= gun.GetDeceleration();
	m_range				= gun.GetRange();
	m_power				= gun.GetPower();

	CalcRayCastPos();

	const OnShotBulletEvent event{ GetName(), gun.GetOwnerName(), GetObjHandle(), m_transform };
	EventSystem::GetInstance()->Publish(event);

	// 必殺技専用カメラにトランスフォーム情報を設定
	// プレイヤーが撃った場合のみ設定
	// TODO : 操作キャラクター用インターフェイスを継承したものにのみ反応しすシステムのほうが良い可能性あり
	// MEMO : プレイヤーが一人の場合にしか対応していない
	if (m_shot_owner_name == ObjName.PLAYER)
	{
		const auto cinemachine_brain = CinemachineBrain::GetInstance();
		const auto camera_controller = std::dynamic_pointer_cast<RocketLauncherVirtualCamerasController>(cinemachine_brain->GetVirtualCameraController(VirtualCameraControllerKind::kRocketLauncherCutscene));
		if (camera_controller)
		{
			camera_controller->SetRocketBomb(shared_from_this());
		}
	}
}

float RocketBomb::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_shot_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleLayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

bool RocketBomb::IsReturnPool()
{
	float distance = VSize(m_transform->GetPos(CoordinateKind::kWorld) - m_first_pos);

	// 射程範囲を超えた場合は弾丸をプールに返却
	return distance > m_range ? true : false;
}

void RocketBomb::ApplyMoveDirToRot()
{
	if (m_move_velocity != v3d::GetZeroV())
	{
		const auto forward = v3d::GetNormalizedV(m_move_velocity);

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

void RocketBomb::Move()
{
	math::Decrease(m_move_speed, m_deceleration, 0.0f);

	// ロケット弾がロケットランチャーから排出されるまでは直進
	if (m_go_straight_timer < kGoStraightTime)
	{
		m_go_straight_timer += GetDeltaTime();

		m_move_velocity = m_move_dir * m_move_speed;
	}
	// 排出されたら画面中央から撃たれた際の軌道に近づける
	else if (m_blend_timer < kBlendTime)
	{
		m_blend_timer += GetDeltaTime();

		const auto t			= math::GetUnitValue<float, float>(0.0f, kBlendTime, m_blend_timer);
		const auto current_dir	= math::GetLerp(m_move_dir, m_destination_dir, t);
		m_move_velocity = current_dir * m_move_speed;
	}
	// 画面中央から撃たれた際の軌道で移動
	else
	{
		m_move_velocity = m_destination_dir * m_move_speed;
	}

	m_velocity += m_move_velocity;
}

void RocketBomb::CalcRayCastPos()
{
	// 光線の位置を計算
	auto ray = std::dynamic_pointer_cast<Segment>(GetCollider(ColliderKind::kRay)->GetShape());
	ray->SetBeginPos(m_prev_pos, true);
	ray->SetEndPos(m_transform->GetPos(CoordinateKind::kWorld), true);
}
