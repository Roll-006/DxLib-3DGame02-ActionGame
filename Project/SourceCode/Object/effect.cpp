#include "effect.hpp"

Effect::Effect(const EffectData& data) :
	ObjBase						(data.obj_name, ObjTag.EFFECT),
	m_origin_effect_handle		(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kEffect, data.file_path)),
	m_playing_effect_handle		(-1),
	m_time_scale_owner_name		(""),
	m_owner_transform			(nullptr),
	m_return_pool_trigger_handle(-1),
	m_offset_pos				(v3d::GetZeroV()),
	m_offset_angle				(v3d::GetZeroV()),
	m_offset_scale				(VGet(1.0f, 1.0f, 1.0f)),
	m_data						(data),
	m_play_count				(0),
	m_play_wait_timer			(0.0f)
{

}

Effect::~Effect()
{
	DeleteEffekseerEffect(m_origin_effect_handle);
}

void Effect::Init()
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

void Effect::Update()
{
	if (!IsActive()) { return; }

	m_play_wait_timer += GetDeltaTime();
}

void Effect::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMatrix();
	ApplyPlaySpeed();

	PlayEffect();
}

void Effect::DrawToShadowMap() const
{

}

void Effect::Draw() const
{
	if (!IsActive()) { return; }
}

void Effect::AddToObjManager()
{
	ObjManager::GetInstance()->AddObj(shared_from_this());
}

void Effect::RemoveToObjManager()
{
	ObjManager::GetInstance()->RemoveObj(GetObjHandle());
}


#pragma region Attach / Detach
void Effect::AttachOwnerTransform(const std::shared_ptr<Transform> owner_transform)
{
	m_owner_transform = owner_transform;
}

void Effect::DetachOwnerTransform()
{
	m_owner_transform = nullptr;
}
#pragma endregion


#pragma region 登録 / 削除
void Effect::AddTimeScaleOwner(const std::string& owner_name)
{
	m_time_scale_owner_name = owner_name;
}

void Effect::RemoveTimeScaleOwner()
{
	m_time_scale_owner_name = "";
}

void Effect::AddReturnPoolTriggerHandle(const int return_trigger_handle)
{
	m_return_pool_trigger_handle = return_trigger_handle;
}

void Effect::RemoveReturnPoolTriggerHandle()
{
	m_return_pool_trigger_handle = -1;
}
#pragma endregion


bool Effect::IsReturnPool()
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

void Effect::ApplyMatrix() const
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

void Effect::ApplyPlaySpeed() const
{
	if (m_playing_effect_handle > -1)
	{
		SetSpeedPlayingEffekseer3DEffect(m_playing_effect_handle, m_data.play_speed * GetDeltaTime());
	}
}

void Effect::PlayEffect()
{
	if (IsEffekseer3DEffectPlaying(m_playing_effect_handle) == -1 && m_play_wait_timer > m_data.play_wait_time)
	{
		m_playing_effect_handle = PlayEffekseer3DEffect(m_origin_effect_handle);
		++m_play_count;
	}
}

float Effect::GetDeltaTime() const
{
	const auto time_manager = GameTimeManager::GetInstance();

	return m_time_scale_owner_name == ObjName.PLAYER
		? time_manager->GetDeltaTime(TimeScaleController::LayerKind::kPlayer)
		: time_manager->GetDeltaTime(TimeScaleController::LayerKind::kEffect);
}
