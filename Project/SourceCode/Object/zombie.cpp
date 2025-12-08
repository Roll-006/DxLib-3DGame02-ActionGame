#include "zombie.hpp"
#include "../MixamoHelper/mixamo_helper.hpp"
#include "player.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie(const std::string& id) :
	EnemyBase				(ObjName.ZOMBIE),
	m_humanoid_foot_ik		(nullptr),
	m_humanoid_frame		(std::make_shared<HumanoidFrameGetter>()),
	m_state					(std::make_shared<ZombieStateController>()),
	m_is_return_pool		(false),
	m_can_grab_target		(false),
	m_is_target_escaped		(false),
	m_is_allow_stealth_kill	(true),
	m_on_stealth_kill		(false)
{
	enemy_id = id;

	nlohmann::json data;
	if (json_loader::Load("Data/JSON/zombie.json", data))
	{
		mass_kind					= data.at("mass_kind");
		invincible_time				= data.at("invincible_time");
		attack_interval_time		= data.at("attack_interval_time");
		detected_notify_distance	= data.at("detected_notify_distance");
		model_path					= data.at("model_path");
		basic_angle					= data.at("basic_angle");
		basic_scale					= data.at("basic_scale");
		walk_speed					= data.at("walk_speed");
		run_speed					= data.at("run_speed");
		run_grab_speed				= data.at("run_grab_speed");
		move_dir_offset_speed		= data.at("move_dir_offset_speed");
		look_dir_offset_speed		= data.at("look_dir_offset_speed");
		collider_data				= data.at("collider_data").get<HumanoidEnemyColliderData>();
		m_leg_ray_data				= data.at("leg_ray_data").get<HumanoidLegRayData>();
		damage_over_time_start_time = data.at("damage_over_time_start_time");
	}

	m_transform->SetPos(CoordinateKind::kWorld, v3d::GetZeroV());
	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = VGet(0.0f, 0.0f, 1.0);
	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	// TODO : JSON指定
	m_health[HealthPartKind::kMain]		= std::make_shared<Gauge>(1684.0f);
	m_health[HealthPartKind::kHead]		= std::make_shared<Gauge>(80.0f);
	m_health[HealthPartKind::kBody]		= std::make_shared<Gauge>(500.0f);
	m_health[HealthPartKind::kLeftArm]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kRightArm]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kLeftLeg]	= std::make_shared<Gauge>(100.0f);
	m_health[HealthPartKind::kRightLeg]	= std::make_shared<Gauge>(100.0f);

	m_modeler  = std::make_shared<Modeler>(m_transform, model_path, basic_angle, basic_scale);
	m_animator = std::make_shared<ZombieAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_humanoid_foot_ik = std::make_shared<HumanoidFootIKSolver>(m_animator, m_modeler, m_colliders, m_leg_ray_data);

	m_is_calc_look_dir = true;

	m_collider_creator->CreateCapsuleCollider		(this, m_modeler, collider_data.capsule_radius);
	m_collider_creator->CreateLandingTrigger		(this, collider_data.landing_trigger_radius);
	m_collider_creator->CreateProjectRay			(this, collider_data.project_ray_length);
	m_collider_creator->CreateCollisionAreaTrigger	(this, collider_data.collision_area_radius);
	m_collider_creator->CreateVisionTrigger			(this, m_modeler, collider_data.visible_distance, collider_data.visible_fov * math::kDegToRad);
	m_collider_creator->CreateVisibleTrigger		(this, m_modeler);
	m_collider_creator->CreateHeadTrigger			(this, m_modeler, collider_data.head_trigger_radius);
	m_collider_creator->CreateBodyTrigger			(this, m_modeler, collider_data.up_body_trigger_radius,		collider_data.down_body_trigger_radius);
	m_collider_creator->CreateArmTrigger			(this, m_modeler, collider_data.upper_arm_trigger_radius,	collider_data.forearm_trigger_radius, collider_data.hand_trigger_radius);
	m_collider_creator->CreateLegTrigger			(this, m_modeler, collider_data.up_leg_trigger_radius,		collider_data.down_leg_trigger_radius);

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

	m_state				->Init(std::static_pointer_cast<Zombie>(shared_from_this()));
	m_humanoid_foot_ik	->Init(std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));
	m_humanoid_foot_ik	->CreateFootRay		(this, std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));
	m_humanoid_foot_ik	->CreateFoeBaseRay	(this, std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));
}

void Zombie::Update()
{
	if (!IsActive()) { return; }

	JudgeAction();
	JudgeInvincible();
	JudgeTargetInSight();

	m_look_dir_offset_speed = look_dir_offset_speed;
	m_is_allow_stealth_kill = true;

	m_state				->Update(std::static_pointer_cast<Zombie>(shared_from_this()));
	m_animator			->Update();
	m_humanoid_foot_ik	->Update(std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));

	if (m_move_dir.at(TimeKind::kCurrent) != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = m_move_dir.at(TimeKind::kCurrent);
	}

	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));
}

void Zombie::LateUpdate()
{
	if (!IsActive()) { return; }

	m_is_project = true;

	// TODO : 後に関数化
	auto ray = std::static_pointer_cast<Segment>(GetCollider(ColliderKind::kRay)->GetShape());
	const auto target_model_handle = m_state->GetTargetCharacter()->GetModeler()->GetModelHandle();
	auto head_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), FramePath.HEAD));
	auto spine2_m	= MV1GetFrameLocalWorldMatrix(target_model_handle, MV1SearchFrame(target_model_handle, FramePath.SPINE_2));
	const auto head_pos		= MGetTranslateElem(head_m);
	const auto spine2_pos = MGetTranslateElem(spine2_m);
	ray->SetBeginPos(head_pos, true);
	ray->SetEndPos(spine2_pos, true);

	m_state->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));

	m_humanoid_foot_ik->BlendFrame(std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));

	m_collider_creator->CalcCapsuleColliderPos		(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcProjectRayPos			(m_modeler, m_colliders);
	m_collider_creator->CalcCollisionAreaTriggerPos	(m_modeler, m_colliders, collider_data.collision_area_offset);
	m_collider_creator->CalcVisionTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcVisibleTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcHeadTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcBodyTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcArmTriggerPos			(m_modeler, m_colliders);
	m_collider_creator->CalcLegTriggerPos			(m_modeler, m_colliders);

	m_can_grab_target				= false;
	m_on_collided_vision_trigger	= false;
	m_has_obstacle_between_target	= false;
}

void Zombie::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	//DrawColliders();
}

void Zombie::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();
	const auto			action_state_kind		= static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_on_ground = true;
		break;

	case ColliderKind::kRay:
		if (dynamic_cast<PhysicalObjBase*>(m_state->GetTargetCharacter().get()) != target_obj) { m_has_obstacle_between_target = true; }
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
		break;

	case ColliderKind::kHeadTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			// ダウン中は部位HPは減少させない
			if (action_state_kind != zombie_state::ActionStateKind::kStandStun)
			{
				OnDamage(HealthPartKind::kHead, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

			m_is_detection_shared = true;

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

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kLeftUpperArmTrigger:
	case ColliderKind::kLeftForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kLeftHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}

		if (target_obj == GetStateController()->GetTargetCharacter().get() && target_collider_kind == ColliderKind::kCollider)
		{
			// 無敵状態でない場合のみ掴み判定を許可する
			if (!GetStateController()->GetTargetCharacter()->IsInvincible())
			{
				m_can_grab_target = true;
			}
		}
		break;

	case ColliderKind::kRightUpperArmTrigger:
	case ColliderKind::kRightForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}
		break;

	case ColliderKind::kRightHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);

			m_is_detection_shared = true;

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue(), TimeScaleLayerKind::kWorld));
		}

		if (target_obj == GetStateController()->GetTargetCharacter().get() && target_collider_kind == ColliderKind::kCollider)
		{
			// 無敵状態でない場合のみ掴み判定を許可する
			if (!GetStateController()->GetTargetCharacter()->IsInvincible())
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
			if (   action_state_kind != zombie_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != zombie_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kLeftLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

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
			if (   action_state_kind != zombie_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != zombie_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kRightLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

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

	// 着地していた場合、以前の投影座標より下にあれば投影を許可する
	if (IsOnGround())
	{
		const auto prev_project_pos = GetPrevProjectPos();
		if (prev_project_pos)
		{
			if ((project_pos->y - prev_project_pos->y) >= math::kEpsilonLow) { return; }
		}
	}

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
	m_state->AttachTarget(target_character);
}

void Zombie::DetachTarget()
{
	m_state->DetachTarget();
}

void Zombie::OnRespawn(const VECTOR& pos, const VECTOR& look_dir)
{
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = v3d::GetNormalizedV(look_dir);
	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	m_collider_creator->CalcCapsuleColliderPos	(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos	(m_modeler, m_colliders);
	m_collider_creator->CalcProjectRayPos		(m_modeler, m_colliders);
}

void Zombie::Detected()
{
	m_look_dir_offset_speed = 8.0f;
	AllowCalcLookDir();

	const auto target_pos			= m_state->GetTargetCharacter()->GetTransform()->GetPos(CoordinateKind::kWorld);
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
	auto	   head_m		= MV1GetFrameLocalWorldMatrix(model_handle, GetHumanoidFrame()->GetHeadIndex(model_handle));
	const auto head_pos		= MGetTranslateElem(head_m);

	// 掴んだことを演出カメラに通知
	EventSystem::GetInstance()->Publish(GrabEvent(GetEnemyID(), GetObjHandle(), m_modeler, head_pos));

	// プレイヤーの掴まれた関数を呼び出す
	const auto player	= std::static_pointer_cast<Player>(m_state->GetTargetCharacter());
	const auto grabber	= std::dynamic_pointer_cast<IGrabber>(shared_from_this());

	player->OnGrabbed(grabber, m_transform->GetPos(CoordinateKind::kWorld), m_look_dir.at(TimeKind::kCurrent));
	player->OnDamage (HealthPartKind::kMain, 80.0f);

	SetAttackIntervalTime();
}

void Zombie::Release()
{
	// 離したことを演出カメラに通知
	EventSystem::GetInstance()->Publish(ReleaseEvent(GetEnemyID(), GetObjHandle()));

	std::static_pointer_cast<Player>(m_state->GetTargetCharacter())->OnRelease();
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
float Zombie::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

bool Zombie::IsStandStun() const
{
	const auto action_state = static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());
	
	return action_state == zombie_state::ActionStateKind::kStandStun;
}

bool Zombie::IsCrouchStun() const
{
	const auto action_state = static_cast<zombie_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	return(action_state == zombie_state::ActionStateKind::kCrouchLeftStun
		|| action_state == zombie_state::ActionStateKind::kCrouchRightStun);
}
#pragma endregion


void Zombie::Move()
{
	m_move_dir_offset_speed = move_dir_offset_speed;
}

void Zombie::TrackMove(const VECTOR& target_pos)
{
	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(target_pos_y0 - pos_y0);
}

void Zombie::OnFootIK()
{
	m_humanoid_foot_ik->OnFootIK(std::dynamic_pointer_cast<IHumanoid>(shared_from_this()));
}

void Zombie::OnCrouchIK()
{
	const auto humanoid = std::dynamic_pointer_cast<IHumanoid>(shared_from_this());

	m_humanoid_foot_ik->CalcRightLegRayPos	(humanoid);
	m_humanoid_foot_ik->OnRightKneelCrouchIK(humanoid);
}

void Zombie::UpdateGrabRun()
{
	m_move_dir_offset_speed = 0.5f;	
	m_move_speed = run_grab_speed;
}

void Zombie::CalcMoveSpeed()
{
	m_move_speed = walk_speed;
}

void Zombie::CalcMoveSpeedStop()
{
	m_move_speed = 0.0f;
}

void Zombie::CalcMoveSpeedRun()
{
	m_move_speed = run_speed;
}

void Zombie::JudgeAction()
{
	const auto is_alive_target = m_state->GetTargetCharacter()->GetHealth(HealthPartKind::kMain)->IsAlive();
	m_can_action = is_alive_target && !m_is_disallow_action_forcibly;
}
