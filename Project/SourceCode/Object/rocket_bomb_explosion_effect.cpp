#include "rocket_bomb_explosion_effect.hpp"

RocketBombExplosionEffect::RocketBombExplosionEffect() :
	PhysicalObjBase				(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT, ObjTag.EFFECT, MassKind::kLight),
	m_origin_effect_handle		(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kEffect, EffectPath.ROCKET_BOMB_HIT_EXPLOSION)),
	m_playing_effect_handle		(-1),
	m_time_scale_owner_name		(""),
	m_owner_transform			(nullptr),
	m_return_pool_trigger_handle(-1),
	m_offset_pos				(v3d::GetZeroV()),
	m_offset_angle				(v3d::GetZeroV()),
	m_offset_scale				(VGet(1.025f, 1.025f, 1.025f)),
	m_data						(EffectData(ObjName.ROCKET_BOMB_HIT_EXPLOSION_EFFECT, EffectPath.ROCKET_BOMB_HIT_EXPLOSION, 50.0f, 0.0f, false)),
	m_play_count				(0),
	m_play_wait_timer			(0.0f)
{
	AddCollider(std::make_shared<Collider>(ColliderKind::kCollider, std::make_shared<Sphere>(v3d::GetZeroV(), kHitRadius), this));
}

RocketBombExplosionEffect::~RocketBombExplosionEffect()
{
	DeleteEffekseerEffect(m_origin_effect_handle);
}

void RocketBombExplosionEffect::Init()
{
	m_time_scale_owner_name			= "";
	m_return_pool_trigger_handle	= -1;
	m_owner_transform				= nullptr;

	m_offset_pos					= v3d::GetZeroV();
	m_offset_angle					= v3d::GetZeroV();
	m_offset_scale					= VGet(1.0f, 1.0f, 1.0f);

	m_play_count					= 0;
	m_play_wait_timer				= 0.0f;

	// Effekseer上で無限生成がオンかつループ再生がtrueの場合は
	// プールから取り出された段階で再生する
	if (m_playing_effect_handle == -1 && m_data.is_loop)
	{
		m_playing_effect_handle = PlayEffekseer3DEffect(m_origin_effect_handle);
	}
}

void RocketBombExplosionEffect::Update()
{
	if (!IsActive()) { return; }

	m_play_wait_timer += GetDeltaTime();
}

void RocketBombExplosionEffect::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMatrix();
	ApplyPlaySpeed();

	PlayEffect();
}

void RocketBombExplosionEffect::DrawToShadowMap() const
{

}

void RocketBombExplosionEffect::Draw() const
{
	if (!IsActive()) { return; }
}

void RocketBombExplosionEffect::OnCollide(const ColliderPairOneToOneData& hit_collider_pair)
{
	switch (hit_collider_pair.owner_collider->GetColliderKind())
	{
	case ColliderKind::kLandingTrigger:
		m_is_landing = true;
		break;

	default:
		break;
	}
}

void RocketBombExplosionEffect::AddToObjManager()
{
	const auto physical_obj = std::dynamic_pointer_cast<PhysicalObjBase>(shared_from_this());

	ObjManager		::GetInstance()->AddObj		  (shared_from_this());
	CollisionManager::GetInstance()->AddCollideObj(physical_obj);
}


#pragma region Attach / Detach
void RocketBombExplosionEffect::AttachOwnerTransform(const std::shared_ptr<Transform> owner_transform)
{
	m_owner_transform = owner_transform;
}

void RocketBombExplosionEffect::DetachOwnerTransform()
{
	m_owner_transform = nullptr;
}
#pragma endregion


#pragma region 登録 / 削除
void RocketBombExplosionEffect::AddTimeScaleOwner(const std::string& owner_name)
{
	m_time_scale_owner_name = owner_name;
}

void RocketBombExplosionEffect::RemoveTimeScaleOwner()
{
	m_time_scale_owner_name = "";
}

void RocketBombExplosionEffect::AddReturnPoolTriggerHandle(const int return_trigger_handle)
{
	m_return_pool_trigger_handle = return_trigger_handle;
}

void RocketBombExplosionEffect::RemoveReturnPoolTriggerHandle()
{
	m_return_pool_trigger_handle = -1;
}
#pragma endregion


bool RocketBombExplosionEffect::IsReturnPool()
{
	if (m_playing_effect_handle > -1)
	{
		// ループ再生なしで再生が終了した場合はプールに返却
		if (!m_data.is_loop && m_play_count > 0 && IsEffekseer3DEffectPlaying(m_playing_effect_handle) == -1)
		{
			return true;
		}
	}

	return false;
}

void RocketBombExplosionEffect::ApplyMatrix() const
{
	if (m_playing_effect_handle > -1)
	{
		if (m_owner_transform)
		{
			m_transform->SetMatrix(CoordinateKind::kWorld, m_owner_transform->GetMatrix(CoordinateKind::kWorld));
		}

		// オフセット値の適用
		const auto scale_m		= MGetScale(m_offset_scale);
		const auto rot_m		= math::ConvertEulerAnglesToXYZRotMatrix(m_offset_angle);
		const auto pos_m		= MGetTranslate(m_offset_pos);
		const auto offset_m		= scale_m * rot_m * pos_m;
		m_transform->SetMatrix	(CoordinateKind::kWorld, offset_m * m_transform->GetMatrix(CoordinateKind::kWorld));

		// 情報の取得
		const auto pos			= m_transform->GetPos(CoordinateKind::kWorld);
		const auto angle		= math::ConvertZXYRotMatrixToEulerAngles(m_transform->GetRotMatrix(CoordinateKind::kWorld));
		const auto scale		= m_transform->GetScale(CoordinateKind::kWorld);

		SetPosPlayingEffekseer3DEffect		(m_playing_effect_handle, pos.x,   pos.y,   pos.z);
		SetRotationPlayingEffekseer3DEffect	(m_playing_effect_handle, angle.x, angle.y, angle.z);
		SetScalePlayingEffekseer3DEffect	(m_playing_effect_handle, scale.x, scale.y, scale.z);
	}
}

void RocketBombExplosionEffect::ApplyPlaySpeed() const
{
	if (m_playing_effect_handle > -1)
	{
		SetSpeedPlayingEffekseer3DEffect(m_playing_effect_handle, m_data.play_speed * GetDeltaTime());
	}
}

void RocketBombExplosionEffect::PlayEffect()
{
	if (IsEffekseer3DEffectPlaying(m_playing_effect_handle) == -1 && m_play_wait_timer > m_data.play_wait_time)
	{
		m_playing_effect_handle = PlayEffekseer3DEffect(m_origin_effect_handle);
		++m_play_count;
	}
}

float RocketBombExplosionEffect::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleController::LayerKind::kEffect);
}
