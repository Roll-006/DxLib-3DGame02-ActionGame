#include "boss.hpp"
#include "player.hpp"
#include "../Part/boss_state_controller.hpp"

Boss::Boss(const std::string& id) :
	EnemyBase				(ObjName.BOSS),
	m_state					(std::make_shared<BossStateController>()),
	m_is_allow_stealth_kill	(true),
	m_on_stealth_kill		(false)
{
	enemy_id = id;

	JSONLoader json_loader;
	nlohmann::json data;
	if (json_loader.Load("Data/JSON/boss.json", data))
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
		collider_data				= data.at("collider_data");
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
	m_animator = std::make_shared<BossAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_is_calc_look_dir = true;

	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, collider_data.capsule_radius);
	m_collider_creator->CreateLandingTrigger	(this, collider_data.landing_trigger_radius);
	m_collider_creator->CreateVisionTrigger		(this, m_modeler, collider_data.visible_distance, collider_data.visible_fov * math::kDegToRad);
	m_collider_creator->CreateVisibleTrigger	(this, m_modeler);
	m_collider_creator->CreateHeadTrigger		(this, m_modeler, collider_data.head_trigger_radius);
	m_collider_creator->CreateBodyTrigger		(this, m_modeler, collider_data.up_body_trigger_radius,		collider_data.down_body_trigger_radius);
	m_collider_creator->CreateArmTrigger		(this, m_modeler, collider_data.upper_arm_trigger_radius,	collider_data.forearm_trigger_radius, collider_data.hand_trigger_radius);
	m_collider_creator->CreateLegTrigger		(this, m_modeler, collider_data.up_leg_trigger_radius,		collider_data.down_leg_trigger_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(v3d::GetZeroV() + collider_data.collision_area_offset, collider_data.collision_area_radius), this));
}

Boss::~Boss()
{
	m_modeler->InitMaterial();
}

void Boss::Init()
{
	m_state->Init(std::static_pointer_cast<Boss>(shared_from_this()));
}

void Boss::Update()
{
	if (!IsActive()) { return; }

	JudgeAction();
	JudgeInvincible();
	JudgeTargetInSight();

	m_look_dir_offset_speed = look_dir_offset_speed;
	m_is_allow_stealth_kill = true;

	m_state		->Update(std::static_pointer_cast<Boss>(shared_from_this()));
	m_animator	->Update();

	if (m_move_dir.at(TimeKind::kCurrent) != v3d::GetZeroV())
	{
		m_look_dir.at(TimeKind::kNext) = m_move_dir.at(TimeKind::kCurrent);
	}

	CalcMoveDir();
	CalcLookDir();
	CalcMoveVelocity();

	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	m_collider_creator->CalcCapsuleColliderPos	(m_modeler, m_colliders);
	m_collider_creator->CalcVisionTriggerPos	(m_modeler, m_colliders);
	m_collider_creator->CalcVisibleTriggerPos	(m_modeler, m_colliders);
	m_collider_creator->CalcHeadTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcBodyTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcArmTriggerPos		(m_modeler, m_colliders);
	m_collider_creator->CalcLegTriggerPos		(m_modeler, m_colliders);
}

void Boss::LateUpdate()
{
	if (!IsActive()) { return; }

	// TODO : 後に関数化
	auto ray = std::static_pointer_cast<Segment>(GetCollider(ColliderKind::kRayCast)->GetShape());
	const auto target_model_handle = m_state->GetTargetCharacter()->GetModeler()->GetModelHandle();
	auto head_m		= MV1GetFrameLocalWorldMatrix(m_modeler->GetModelHandle(), MV1SearchFrame(m_modeler->GetModelHandle(), BonePath.HEAD));
	auto spine2_m	= MV1GetFrameLocalWorldMatrix(target_model_handle, MV1SearchFrame(target_model_handle, BonePath.SPINE_2));
	const auto head_pos		= MGetTranslateElem(head_m);
	const auto spine2_pos = MGetTranslateElem(spine2_m);
	ray->SetBeginPos(head_pos, true);
	ray->SetEndPos(spine2_pos, true);

	m_state->LateUpdate(std::static_pointer_cast<Boss>(shared_from_this()));

	m_on_collided_vision_trigger	= false;
	m_has_obstacle_between_target	= false;
	m_is_using_projection_velocity	= true;
}

void Boss::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawColliders();
}

void Boss::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	PhysicalObjBase*	target_obj				= hit_collider_pair.target_collider->GetOwnerObj();
	const auto			target_name				= target_obj->GetName();
	const auto			target_tag				= target_obj->GetTag();
	const auto			target_collider_kind	= hit_collider_pair.target_collider->GetColliderKind();
	const auto			action_state_kind		= static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	case ColliderKind::kRayCast:
		if (dynamic_cast<PhysicalObjBase*>(m_state->GetTargetCharacter().get()) != target_obj)
		{
			m_has_obstacle_between_target = true;
		}
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
			if (action_state_kind != boss_state::ActionStateKind::kStandStun)
			{
				OnDamage(HealthPartKind::kHead, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kUpBodyTrigger:
	case ColliderKind::kDownBodyTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kBody,	damage);
			OnDamage(HealthPartKind::kMain,	damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kLeftUpperArmTrigger:
	case ColliderKind::kLeftForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kLeftHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kLeftArm,	damage);
			OnDamage(HealthPartKind::kMain,		damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kRightUpperArmTrigger:
	case ColliderKind::kRightForearmTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kRightHandTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			OnDamage(HealthPartKind::kRightArm, damage);
			OnDamage(HealthPartKind::kMain,		damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kLeftUpLegTrigger:
	case ColliderKind::kLeftDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			// ダウン中は部位HPは減少させない
			if (   action_state_kind != boss_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != boss_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kLeftLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	case ColliderKind::kRightUpLegTrigger:
	case ColliderKind::kRightDownLegTrigger:
		if (target_name == ObjName.BULLET)
		{
			const auto damage = dynamic_cast<Bullet*>(target_obj)->GetPower();

			// ダウン中は部位HPは減少させない
			if (   action_state_kind != boss_state::ActionStateKind::kCrouchLeftStun
				&& action_state_kind != boss_state::ActionStateKind::kCrouchRightStun)
			{
				OnDamage(HealthPartKind::kRightLeg, damage);
			}

			OnDamage(HealthPartKind::kMain, damage);

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	default:
		break;
	}
}

void Boss::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.count(part_kind)) { return; }

	m_health.at(part_kind)->Decrease(damage);
	m_invincible_timer	= invincible_time;
	m_is_invincible		= true;
}

void Boss::AttachTarget(const std::shared_ptr<CharacterBase>& target_character)
{
	m_state->AttachTarget(target_character);
}

void Boss::DetachTarget()
{
	m_state->DetachTarget();
}

void Boss::OnRespawn(const VECTOR& pos, const VECTOR& look_dir)
{
	m_transform->SetPos(CoordinateKind::kWorld, pos);

	m_look_dir.at(TimeKind::kNext) = m_look_dir.at(TimeKind::kCurrent) = v3d::GetNormalizedV(look_dir);
	ApplyLookDirToRot(m_look_dir.at(TimeKind::kCurrent));

	// コライダーの位置を修正
	m_collider_creator->CalcCapsuleColliderPos(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos (m_modeler, m_colliders);

	const auto sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollisionAreaTrigger)->GetShape());
	sphere->SetPos(pos + collider_data.collision_area_offset);
}

void Boss::Detected()
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

void Boss::OnStealthKill()
{
	m_on_stealth_kill = true;
}

void Boss::ExitStealthKilled()
{
	m_on_stealth_kill = false;
}


#pragma region Getter
float Boss::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();
	return time_manager->GetDeltaTime(TimeScaleLayerKind::kWorld);
}

bool Boss::IsStandStun() const
{
	const auto action_state = static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());
	
	return action_state == boss_state::ActionStateKind::kStandStun;
}

bool Boss::IsCrouchStun() const
{
	const auto action_state = static_cast<boss_state::ActionStateKind>(m_state->GetActionState(TimeKind::kCurrent)->GetStateKind());

	return(action_state == boss_state::ActionStateKind::kCrouchLeftStun
		|| action_state == boss_state::ActionStateKind::kCrouchRightStun);
}
#pragma endregion


void Boss::Move()
{
	m_move_dir_offset_speed = move_dir_offset_speed;
}

void Boss::TrackMove(const VECTOR& target_pos)
{
	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(target_pos_y0 - pos_y0);
}

void Boss::UpdateGrabRun()
{
	m_move_dir_offset_speed = 0.5f;	
	m_move_speed = run_grab_speed;
}

void Boss::CalcMoveSpeed()
{
	m_move_speed = walk_speed;
}

void Boss::CalcMoveSpeedStop()
{
	m_move_speed = 0.0f;
}

void Boss::CalcMoveSpeedRun()
{
	m_move_speed = run_speed;
}

void Boss::JudgeAction()
{
	const auto is_alive_target = m_state->GetTargetCharacter()->GetHealth(HealthPartKind::kMain)->IsAlive();
	m_can_action = is_alive_target && !m_is_disallow_action_forcibly;
}
