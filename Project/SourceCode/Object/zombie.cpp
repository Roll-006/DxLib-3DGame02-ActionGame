#include "../Interface/i_grabbable.hpp"
#include "../Base/knife_base.hpp"
#include "../Kind/zombie_state_kind.hpp"
#include "../MixamoHelper/mixamo_helper.hpp"
#include "../InverseKinematics/frame_info.hpp"
#include "zombie.hpp"

Zombie::Zombie(const std::string& id) :
	EnemyBase				(ObjName.ZOMBIE),
	m_humanoid_arm_ik		(nullptr),
	m_humanoid_foot_ik		(nullptr),
	m_humanoid_body_ik		(nullptr),
	m_humanoid_frame		(std::make_shared<HumanoidFrameGetter>()),
	m_state					(nullptr),
	m_ai					(std::make_shared<ZombieAI>()),
	m_is_return_pool		(false),
	m_can_grab_target		(false),
	m_is_target_escaped		(false),
	m_is_allow_stealth_kill	(true),
	m_on_stealth_kill		(false),
	m_is_forward_walk		(false),
	m_is_backward_walk		(false),
	m_push_bone_velocity	{ 
		{RoughBodyKind::kHead,		v3d::GetZeroV()},
		{RoughBodyKind::kBody,		v3d::GetZeroV()},
		{RoughBodyKind::kLeftArm,	v3d::GetZeroV()},
		{RoughBodyKind::kRightArm,	v3d::GetZeroV()},
		{RoughBodyKind::kLeftLeg,	v3d::GetZeroV()},
		{RoughBodyKind::kRightLeg,	v3d::GetZeroV()}}
{
	enemy_id = id;

	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/zombie.json", j_data))
	{
		mass_kind					= j_data.at("mass_kind");
		invincible_time				= j_data.at("invincible_time");
		attack_interval_time		= j_data.at("attack_interval_time");
		detected_notify_distance	= j_data.at("detected_notify_distance");
		data						= j_data.at("data").get<ZombieData>();
		m_arm_ray_data				= j_data.at("arm_ray_data").get<HumanoidArmRayData>();
		m_leg_ray_data				= j_data.at("leg_ray_data").get<HumanoidLegRayData>();

		m_knock_back_gauge = std::make_shared<Gauge>(400.0f);
	}

	m_transform->SetPos(CoordinateKind::kWorld, v3d::GetZeroV());
	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = VGet(0.0f, 0.0f, 1.0);
	ApplyLookDirToRot();

	// TODO : JSON指定
	m_health[HealthPartKind::kMain]		= std::make_shared<Gauge>(1684.0f);
	m_health[HealthPartKind::kHead]		= std::make_shared<Gauge>(80.0f);
	m_health[HealthPartKind::kBody]		= std::make_shared<Gauge>(500.0f);
	m_health[HealthPartKind::kLeftArm]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kRightArm]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kLeftLeg]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kRightLeg]	= std::make_shared<Gauge>(100.0f);

	m_modeler  = std::make_shared<Modeler>(m_transform, data.model_path, data.basic_angle, data.basic_scale);
	m_animator = std::make_shared<Animator>(m_modeler, "zombie");
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_state = std::make_shared<zombie_state::State>(*this, m_animator);

	m_humanoid_arm_ik	= std::make_shared<HumanoidArmIKSolver> (*this, m_animator, m_modeler, m_colliders, m_arm_ray_data);
	m_humanoid_foot_ik	= std::make_shared<HumanoidFootIKSolver>(*this, m_animator, m_modeler, m_colliders, m_leg_ray_data);
	m_humanoid_body_ik	= std::make_shared<HumanoidBodyIKSolver>(*this, m_animator, m_modeler);

	m_is_calc_look_dir = true;

	m_collider_creator->CreateCapsuleCollider		(this, m_modeler, data.collider_data.capsule_radius);
	m_collider_creator->CreateLandingTrigger		(this, data.collider_data.landing_trigger_radius);
	m_collider_creator->CreateProjectRay			(this, data.collider_data.project_ray_length);
	m_collider_creator->CreateCollisionAreaTrigger	(this, data.collider_data.collision_area_radius);
	m_collider_creator->CreateVisionTrigger			(this, m_modeler, data.collider_data.visible_distance, data.collider_data.visible_fov * math::kDegToRad);
	m_collider_creator->CreateVisibleTrigger		(this, m_modeler);
	m_collider_creator->CreateHeadTrigger			(this, m_modeler, data.collider_data.head_trigger_radius);
	m_collider_creator->CreateBodyTrigger			(this, m_modeler, data.collider_data.up_body_trigger_radius,	data.collider_data.down_body_trigger_radius);
	m_collider_creator->CreateArmTrigger			(this, m_modeler, data.collider_data.upper_arm_trigger_radius,	data.collider_data.forearm_trigger_radius, data.collider_data.hand_trigger_radius);
	m_collider_creator->CreateLegTrigger			(this, m_modeler, data.collider_data.up_leg_trigger_radius,		data.collider_data.down_leg_trigger_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kRay, std::make_shared<Segment>(), this));
}

Zombie::~Zombie()
{
	m_colliders.clear();
}

void Zombie::Init()
{
	m_is_return_pool = false;

	m_modeler->InitMaterial();

	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(shared_from_this());
	m_humanoid_arm_ik	->Init();
	m_humanoid_foot_ik	->Init();
	m_humanoid_body_ik	->Init();
	m_humanoid_foot_ik	->CreateFootRay	  (this);
	m_humanoid_foot_ik	->CreateFoeBaseRay(this);
}

void Zombie::Update()
{
	if (!IsActive()) { return; }

	JudgeAction();
	JudgeInvincible();
	JudgeTargetInSight();

	m_look_dir_offset_speed			= data.look_dir_offset_speed;
	m_move_dir_offset_speed			= data.move_dir_offset_speed;
	m_is_allow_stealth_kill			= true;
	m_can_decrease_knock_back_gauge = true;

	m_state				->Update();
	m_animator			->Update();
	m_humanoid_foot_ik	->Update();
	m_humanoid_arm_ik	->Update();
	m_humanoid_body_ik	->Update();
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_is_project = true;

	// TODO : 後に関数化
	auto ray = std::static_pointer_cast<Segment>(GetCollider(ColliderKind::kRay)->GetShape());
	const auto target_model_handle = m_ai->GetTarget()->GetModeler()->GetModelHandle();
	auto head_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), FramePath.HEAD));
	auto spine2_m	= MV1GetFrameLocalWorldMatrix(target_model_handle, MV1SearchFrame(target_model_handle, FramePath.SPINE_2));
	const auto head_pos		= matrix::GetPos(head_m);
	const auto spine2_pos = matrix::GetPos(spine2_m);
	ray->SetBeginPos(head_pos, true);
	ray->SetEndPos(spine2_pos, true);

	m_state->LateUpdate();

	// IK処理
	OnHitBoneReaction();
	m_humanoid_foot_ik->BlendFrame();
	m_humanoid_arm_ik ->BlendFrame();
	m_humanoid_body_ik->BlendFrame();

	// コライダー更新処理
	m_collider_creator->CalcCapsuleColliderPos		(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcProjectRayPos			(m_modeler, m_colliders);
	m_collider_creator->CalcCollisionAreaTriggerPos	(m_modeler, m_colliders, data.collider_data.collision_area_offset);
	m_collider_creator->CalcVisionTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcVisibleTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcHeadTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcBodyTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcArmTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcLegTriggerPos			(m_modeler, m_colliders);

	// フラグ更新
	m_can_grab_target				= false;
	m_on_collided_vision_trigger	= false;
	m_has_obstacle_between_target	= false;
}

void Zombie::Draw() const
{
	if (!IsActive()) { return; }

	//m_modeler->Draw();

	mixamo_helper::DrawFrames(m_modeler->GetModelHandle(), true, true, true, false);

	//DrawColliders();
}

void Zombie::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();
	const auto			state_kind				= m_state->GetCurrentStateKind();

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_on_ground = true;
		break;

	case ColliderKind::kRay:
		if (dynamic_cast<PhysicalObjBase*>(m_ai->GetTarget().get()) != target_obj) { m_has_obstacle_between_target = true; }
		break;

	case ColliderKind::kProjectRay:
		if (hit_collider_pair.intersection) { m_current_project_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kLeftLegRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.left_leg_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kRightLegRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.right_leg_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kLeftFootRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.left_foot_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kRightFootRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.right_foot_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kLeftToeBaseRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.left_toe_base_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kRightToeBaseRay:
		if (hit_collider_pair.intersection) { m_leg_ray_data.right_toe_base_cast_pos = hit_collider_pair.intersection; }
		break;

	case ColliderKind::kMiddleVisionTrigger:
		if (target_collider_kind == ColliderKind::kVisibleTrigger && target_tag == ObjTag.PLAYER)
		{
			m_on_collided_vision_trigger = true;
		}
		break;

	case ColliderKind::kCollider:
		if (target_name == ObjName.KNIFE)
		{
			const auto damage = dynamic_cast<KnifeBase*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kMain, damage);

			//ナイフ攻撃では必ずノックバックさせる
			m_knock_back_gauge->DecreaseZero();

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(target_obj->GetTransform()->GetPos(CoordinateKind::kWorld), damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kHeadTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto bullet = dynamic_cast<Bullet*>(target_obj);
			const auto damage = bullet->GetPower();

			// ダウン中は部位HPは減少させない
			if (   state_kind != ZombieStateKind::kStandStun
				&& state_kind != ZombieStateKind::kCrouchLeftStun
				&& state_kind != ZombieStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kHead, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			// ボーンリアクションvelocityを設定
			const auto push_speed		= VSize(bullet->GetVelocity());
			const auto head_push_dir	= v3d::GetNormalizedV(bullet->GetVelocity());
			const auto arm_push_dir		= v3d::GetNormalizedV(-bullet->GetVelocity()) + axis::GetWorldYAxis();
			m_push_bone_velocity[RoughBodyKind::kHead]		= head_push_dir * push_speed * 0.005f;
			m_push_bone_velocity[RoughBodyKind::kLeftArm]	= arm_push_dir  * push_speed * 0.003f;
			m_push_bone_velocity[RoughBodyKind::kRightArm]	= arm_push_dir  * push_speed * 0.003f;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kUpBodyTrigger:
	case ColliderKind::kDownBodyTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kBody,	damage);
			OnDamage(HealthPartKind::kMain,	damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kLeftUpperArmTrigger:
	case ColliderKind::kLeftForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto bullet = dynamic_cast<Bullet*>(target_obj);
			const auto damage = bullet->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			const auto push_speed	= VSize(bullet->GetVelocity());
			const auto push_dir		= v3d::GetNormalizedV(bullet->GetVelocity()) + axis::GetWorldYAxis();
			m_push_bone_velocity[RoughBodyKind::kLeftArm] = push_dir * push_speed * 0.006f;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kLeftHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto bullet = dynamic_cast<Bullet*>(target_obj);
			const auto damage = bullet->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			const auto push_speed	= VSize(bullet->GetVelocity());
			const auto push_dir		= v3d::GetNormalizedV(bullet->GetVelocity()) + axis::GetWorldYAxis();
			m_push_bone_velocity[RoughBodyKind::kLeftArm] = push_dir * push_speed * 0.006f;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}

		if (target_obj == m_ai->GetTarget().get() && target_collider_kind == ColliderKind::kCollider)
		{
			// 無敵状態でない場合のみ掴み判定を許可する
			if (!m_ai->GetTarget()->IsInvincible())
			{
				m_can_grab_target = true;
			}
		}
		break;

	case ColliderKind::kRightUpperArmTrigger:
	case ColliderKind::kRightForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto bullet = dynamic_cast<Bullet*>(target_obj);
			const auto damage = bullet->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			// ボーンリアクションvelocityを設定
			const auto push_speed	= VSize(bullet->GetVelocity());
			const auto push_dir		= v3d::GetNormalizedV(bullet->GetVelocity()) + axis::GetWorldYAxis();
			m_push_bone_velocity[RoughBodyKind::kRightArm] = push_dir * push_speed * 0.006f;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kRightHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto bullet = dynamic_cast<Bullet*>(target_obj);
			const auto damage = bullet->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			// ボーンリアクションvelocityを設定
			const auto push_speed	= VSize(bullet->GetVelocity());
			const auto push_dir		= v3d::GetNormalizedV(bullet->GetVelocity()) + axis::GetWorldYAxis();
			m_push_bone_velocity[RoughBodyKind::kRightArm] = push_dir * push_speed * 0.006f;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}

		if (target_obj == m_ai->GetTarget().get() && target_collider_kind == ColliderKind::kCollider)
		{
			// 無敵状態でない場合のみ掴み判定を許可する
			if (!m_ai->GetTarget()->IsInvincible())
			{
				m_can_grab_target = true;
			}
		}
		break;

	case ColliderKind::kLeftUpLegTrigger:
	case ColliderKind::kLeftDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			// ダウン中は部位HPは減少させない
			if (   state_kind != ZombieStateKind::kCrouchLeftStun
				&& state_kind != ZombieStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kLeftLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kRightUpLegTrigger:
	case ColliderKind::kRightDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			// ダウン中は部位HPは減少させない
			if (   state_kind != ZombieStateKind::kCrouchLeftStun
				&& state_kind != ZombieStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kRightLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);
			if (m_can_decrease_knock_back_gauge) { m_knock_back_gauge->Decrease(damage); }

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	default:
		break;
	}
}

void Zombie::OnProjectPos()
{
	if (!IsActive())	{ return; }
	if (!IsProject())	{ return; }

	const auto project_pos = GetCurrentProjectPos();
	if (!project_pos)	{ return; }

	const auto project_ray = GetCollider(ColliderKind::kProjectRay);
	if (!project_ray)	{ return; }

	const auto collider = GetCollider(ColliderKind::kCollider);
	if (!collider)		{ return; }

	const auto capsule = std::dynamic_pointer_cast<Capsule>(collider->GetShape());
	if (!capsule)		{ return; }

	const auto hit_triangle = project_ray->GetHitTriangles();
	if (hit_triangle.size() <= 0) { return; }

	// 坂に向かっている場合は投影を許可しない
	if (IsGoUpHill(hit_triangle.front())) { return; }

	// 光線の始点からの距離
	const auto future_begin_pos = *project_pos + axis::GetWorldYAxis() * capsule->GetRadius();
	const auto distance = math::GetDistancePointToTriangle(future_begin_pos, hit_triangle.front());

	// 固定位置を決定
	const auto penetration		= capsule->GetRadius() - distance;
	const auto push_back_length = math::GetHypotenuseLengthIsoscelesRightTriangle(penetration);
	const auto result_pos		= *project_pos + axis::GetWorldYAxis() * push_back_length;
	m_transform->SetPos(CoordinateKind::kWorld, result_pos);
}

void Zombie::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.contains(part_kind)) { return; }

	m_health.at(part_kind)->Decrease(damage);
	m_invincible_timer	= invincible_time;
	m_is_invincible		= true;
}

void Zombie::AttachTarget(const std::shared_ptr<CharacterBase>& target_character)
{
	m_ai->AttachTarget(target_character);
}

void Zombie::DetachTarget()
{
	m_ai->DetachTarget();
}

void Zombie::OnRespawn(const VECTOR& pos, const VECTOR& look_dir)
{
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = v3d::GetNormalizedV(look_dir);
	ApplyLookDirToRot();

	m_collider_creator->CalcCapsuleColliderPos	(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos	(m_modeler, m_colliders);
	m_collider_creator->CalcProjectRayPos		(m_modeler, m_colliders);
}

void Zombie::Detected()
{
	m_look_dir_offset_speed = 8.0f;
	AllowCalcLookDir();
	CalcMoveSpeedStop();

	const auto target_pos			= m_ai->GetTarget()->GetTransform()->GetPos(CoordinateKind::kWorld);
	const auto target_pos_y0		= VGet(target_pos.x, 0.0f, target_pos.z);
	const auto pos					= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0				= VGet(pos.x, 0.0f, pos.z);
	m_look_dir.at(TimeKind::kNext)	= v3d::GetNormalizedV(target_pos_y0 - pos_y0);

	m_move_dir.at(TimeKind::kCurrent) = v3d::GetZeroV();
}

void Zombie::Grab()
{
	m_is_target_escaped = false;

	const auto model_handle = m_modeler->GetModelHandle();
	const auto head_m		= MV1GetFrameLocalWorldMatrix(model_handle, GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= matrix::GetPos(head_m);

	// 掴んだことを演出カメラに通知
	EventSystem::GetInstance()->Publish(GrabEvent(GetEnemyID(), GetObjHandle(), m_modeler, head_pos));

	// プレイヤーの掴まれた関数を呼び出す
	const auto grabbable	= std::dynamic_pointer_cast<IGrabbable>(m_ai->GetTarget());
	const auto grabber		= std::dynamic_pointer_cast<IGrabber>(shared_from_this());

	if (grabbable)
	{
		grabbable->OnGrabbed(grabber, m_transform->GetPos(CoordinateKind::kWorld), m_look_dir.at(TimeKind::kCurrent));
	}

	m_ai->GetTarget()->OnDamage (HealthPartKind::kMain, 80.0f);

	SetUpAttackIntervalTime();
}

void Zombie::Release()
{
	// 離したことを演出カメラに通知
	EventSystem::GetInstance()->Publish(ReleaseEvent(GetEnemyID(), GetObjHandle()));

	const auto grabbable = std::dynamic_pointer_cast<IGrabbable>(m_ai->GetTarget());
	if (grabbable)
	{
		grabbable->OnRelease();
	}
}

void Zombie::OnEscape()
{
	m_is_target_escaped = true;
}

void Zombie::OnStealthKill()
{
	m_on_stealth_kill = true;
}

void Zombie::ExitStealthKilled()
{
	m_on_stealth_kill = false;
}


#pragma region Getter
const float Zombie::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

const bool Zombie::IsStandStun() const
{
	const auto action_state = m_state->GetCurrentStateKind();
	
	return action_state == ZombieStateKind::kStandStun;
}

const bool Zombie::IsCrouchStun() const
{
	const auto action_state = m_state->GetCurrentStateKind();

	return(action_state == ZombieStateKind::kCrouchLeftStun
		|| action_state == ZombieStateKind::kCrouchRightStun);
}
#pragma endregion


void Zombie::TrackMove(const VECTOR& target_pos, const bool is_distance_limit)
{
	constexpr float kBackwardEnterDist	= 50.0f;
	constexpr float kBackwardExitDist	= 60.0f;

	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);
	const auto distance			= VSize(target_pos - pos);
	auto	   next_move_dir	= v3d::GetNormalizedV(target_pos_y0 - pos_y0);

	if (is_distance_limit)
	{
		if (m_is_backward_walk)
		{
			if (distance >= kBackwardExitDist)
			{
				m_is_backward_walk = false;
			}
		}
		else
		{
			if (distance < kBackwardEnterDist)
			{
				m_is_backward_walk = true;
			}
		}

		if (m_is_backward_walk)
		{
			next_move_dir *= -1.0f;
		}
		else if (distance < kBackwardExitDist)
		{
			next_move_dir = v3d::GetZeroV();
		}
	}

	m_move_dir.at(TimeKind::kNext) = next_move_dir;
}

void Zombie::OnFootIK()
{
	m_humanoid_foot_ik->ApplyFootIK();
}

void Zombie::OnLeftCrouchIK()
{
	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(shared_from_this());

	m_humanoid_foot_ik->CalcLeftLegRayPos();
	m_humanoid_arm_ik ->CalcLeftHandRayPos();
	m_humanoid_foot_ik->ApplyLeftKneelCrouchIK();
	m_humanoid_arm_ik ->ApplyLeftKneelCrouchIK();
}

void Zombie::OnRightCrouchIK()
{
	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(shared_from_this());
	
	m_humanoid_foot_ik->CalcRightLegRayPos();
	m_humanoid_arm_ik ->CalcRightHandRayPos();
	m_humanoid_foot_ik->ApplyRightKneelCrouchIK();
	m_humanoid_arm_ik ->ApplyRightKneelCrouchIK();
}

void Zombie::UpdateGrabRun()
{
	m_move_dir_offset_speed = 0.5f;	
	m_move_speed = data.run_grab_speed;
}

void Zombie::CalcMoveSpeed()
{
	m_move_speed = data.walk_speed;
}

void Zombie::CalcMoveSpeedStop()
{
	m_move_speed = 0.0f;
}

void Zombie::CalcMoveSpeedRun()
{
	m_move_speed = data.run_speed;
}

void Zombie::JudgeAction()
{
	const auto is_alive_target = m_ai->GetTarget()->GetHealth(HealthPartKind::kMain)->IsAlive();
	m_can_action = is_alive_target && !m_is_disallow_action_forcibly;
}

void Zombie::OnHitBoneReaction()
{
	auto model_handle	= -1;
	auto frame_data		= FrameData();

	for (auto& velocity : m_push_bone_velocity)
	{
		// IK適用処理
		if (velocity.second != v3d::GetZeroV())
		{
			velocity.second = math::GetApproachedVector(velocity.second, v3d::GetZeroV(), 30.0f * GetDeltaTime());

			switch (velocity.first)
			{
			case RoughBodyKind::kHead:
				model_handle	= m_modeler->GetModelHandle();
				frame_data		= frame_info::GetFrameInfo(model_handle, m_humanoid_frame->GetNeckIndex(model_handle));
				m_humanoid_body_ik->ApplyBodyIK(frame_data.world_pos + velocity.second, m_humanoid_frame->GetNeckIndex(model_handle), HumanoidBodyIKSolver::IKKind::kHitReaction);
				if (velocity.second == v3d::GetZeroV()) { m_humanoid_body_ik->ChangeOriginMatrix(true); }
				break;

			case RoughBodyKind::kBody:
				break;

			case RoughBodyKind::kLeftArm:
				model_handle	= m_modeler->GetModelHandle();
				frame_data		= frame_info::GetFrameInfo(model_handle, m_humanoid_frame->GetLeftHandIndex(model_handle));
				m_humanoid_arm_ik->ApplyLeftArmIK(frame_data.world_pos + velocity.second, HumanoidArmIKSolver::IKKind::kHitReaction);
				if (velocity.second == v3d::GetZeroV()) { m_humanoid_arm_ik->ChangeLeftArmOriginMatrix(true); }
				break;

			case RoughBodyKind::kRightArm:
				model_handle	= m_modeler->GetModelHandle();
				frame_data		= frame_info::GetFrameInfo(model_handle, m_humanoid_frame->GetRightHandIndex(model_handle));
				m_humanoid_arm_ik->ApplyRightArmIK(frame_data.world_pos + velocity.second, HumanoidArmIKSolver::IKKind::kHitReaction);
				if (velocity.second == v3d::GetZeroV()) { m_humanoid_arm_ik->ChangeRightArmOriginMatrix(true); }
				break;

			case RoughBodyKind::kLeftLeg:
				break;

			case RoughBodyKind::kRightLeg:
				break;
			}
		}
	}
}
