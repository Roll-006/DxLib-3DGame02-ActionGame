#include "zombie.hpp"
#include "player.hpp"
#include "../Part/zombie_state_controller.hpp"

Zombie::Zombie() :
	EnemyBase				(ObjName.ZOMBIE, MassKind::kMedium),
	m_state					(std::make_shared<ZombieStateController>()),
	m_can_grab_target		(false),
	m_is_target_escaped		(false),
	m_is_allow_stealth_kill	(true),
	m_on_stealth_kill		(false)
{
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

	m_modeler  = std::make_shared<Modeler>(m_transform, ModelPath.ZOMBIE_01, m_basic_angle, m_basic_scale);
	m_animator = std::make_shared<ZombieAnimator>(m_modeler, m_state);
	SetColliderModelHandle(m_modeler->GetModelHandle());

	m_invincible_time		= kInvincibleTime;
	m_attack_interval_time	= kAttackIntervalTime;
	m_is_calc_look_dir		= true;

	m_collider_creator->CreateCapsuleCollider	(this, m_modeler, m_capsule_radius);
	m_collider_creator->CreateLandingTrigger	(this, m_landing_trigger_radius);
	m_collider_creator->CreateVisionTrigger		(this, m_modeler, m_visible_distance, m_fov * math::kDegToRad);
	m_collider_creator->CreateVisibleTrigger	(this, m_modeler);
	m_collider_creator->CreateHeadTrigger		(this, m_modeler, m_hand_trigger_radius);
	m_collider_creator->CreateBodyTrigger		(this, m_modeler, m_up_body_trigger_radius,		m_down_body_trigger_radius);
	m_collider_creator->CreateArmTrigger		(this, m_modeler, m_upper_arm_trigger_radius,	m_forearm_trigger_radius, m_hand_trigger_radius);
	m_collider_creator->CreateLegTrigger		(this, m_modeler, m_up_leg_trigger_radius,		m_down_leg_trigger_radius);

	AddCollider(std::make_shared<Collider>(ColliderKind::kRayCast, std::make_shared<Segment>(), this));
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollisionAreaTrigger, std::make_shared<Sphere>(v3d::GetZeroV() + m_collision_area_offset, m_collision_area_radius), this));
}

Zombie::~Zombie()
{
	m_modeler->InitMaterial();
}

void Zombie::Init()
{

}

void Zombie::Update()
{
	if (!IsActive()) { return; }

	JudgeAction();
	JudgeInvincible();
	JudgeLostTarget();

	m_look_dir_offset_speed = kLookDirOffsetSpeed;
	m_is_allow_stealth_kill = true;

	m_state		->Update(std::static_pointer_cast<Zombie>(shared_from_this()));
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

void Zombie::LateUpdate()
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

	m_state->LateUpdate(std::static_pointer_cast<Zombie>(shared_from_this()));

	m_can_grab_target				= false;
	m_on_collided_vision_trigger	= false;
	m_has_obstacle_between_target	= false;
	m_is_using_projection_velocity	= true;
}

void Zombie::Draw() const
{
	if (!IsActive()) { return; }

	m_modeler->Draw();

	DrawColliders();
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
			if (action_state_kind != zombie_state::ActionStateKind::kStandStun)
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

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
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

			EventSystem::GetInstance()->Publish(OnDamageEvent(*hit_collider_pair.intersection, damage / m_health.at(HealthPartKind::kMain)->GetMaxValue()));
		}
		break;

	default:
		break;
	}
}

void Zombie::OnDamage(const HealthPartKind part_kind, const float damage)
{
	if (!m_health.count(part_kind)) { return; }

	m_health.at(part_kind)->Decrease(damage);
	m_invincible_timer	= m_invincible_time;
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

	// コライダーの位置を修正
	m_collider_creator->CalcCapsuleColliderPos(m_modeler, m_colliders);
	m_collider_creator->CalcLandingTriggerPos (m_modeler, m_colliders);

	const auto sphere = std::static_pointer_cast<Sphere>(GetCollider(ColliderKind::kCollisionAreaTrigger)->GetShape());
	sphere->SetPos(pos + m_collision_area_offset);
}

void Zombie::Grab()
{
	m_is_target_escaped = false;

	// 掴んだことを演出カメラに通知
	const GrabEvent event{ GetEnemyHandle(), m_modeler };
	EventSystem::GetInstance()->Publish(event);

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
	const ReleaseEvent event{ GetEnemyHandle() };
	EventSystem::GetInstance()->Publish(event);

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
	m_move_dir_offset_speed = kMoveDirOffsetSpeed;
}

void Zombie::TrackMove(const VECTOR& target_pos)
{
	const auto pos				= m_transform->GetPos(CoordinateKind::kWorld);
	const auto pos_y0			= VGet(pos.x, 0.0f, pos.z);
	const auto target_pos_y0	= VGet(target_pos.x, 0.0f, target_pos.z);

	m_move_dir.at(TimeKind::kNext) = v3d::GetNormalizedV(target_pos_y0 - pos_y0);
}

void Zombie::UpdateGrabRun()
{
	m_move_dir_offset_speed = 0.5f;
	m_move_speed = m_run_grab_speed;
}

void Zombie::CalcMoveSpeed()
{
	m_move_speed = m_walk_speed;
}

void Zombie::CalcMoveSpeedStop()
{
	m_move_speed = 0.0f;
}

void Zombie::CalcMoveSpeedRun()
{
	m_move_speed = m_run_speed;
}

void Zombie::JudgeAction()
{
	const auto is_alive_target = m_state->GetTargetCharacter()->GetHealth(HealthPartKind::kMain)->IsAlive();
	m_can_action = is_alive_target && !m_is_disallow_action_forcibly;
}
