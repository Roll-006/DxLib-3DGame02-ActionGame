#include "effect.hpp"

Effect::Effect(const std::string& obj_name, const float play_speed, const bool is_loop, const std::string& file_path) :
	ObjBase					(obj_name, ObjTag.EFFECT),
	m_origin_effect_handle	(HandleKeeper::GetInstance()->LoadHandle(HandleKind::kEffect, file_path)),
	m_playing_effect_handle	(-1),
	m_play_count			(0),
	m_play_speed			(play_speed),
	m_is_loop				(is_loop)
{

}

Effect::~Effect()
{
	DeleteEffekseerEffect(m_origin_effect_handle);
}

void Effect::Init()
{
	m_play_count = 0;
}

void Effect::Update()
{
	if (!IsActive()) { return; }
}

void Effect::LateUpdate()
{
	if (!IsActive()) { return; }

	ApplyMatrix();
	ApplyPlaySpeed();

	PlayEffect();
}

void Effect::Draw() const
{
	if (!IsActive()) { return; }
}

bool Effect::IsReturnPool()
{
	if (m_playing_effect_handle > -1 && !m_is_loop && m_play_count > 0)
	{
		if (IsEffekseer3DEffectPlaying(m_playing_effect_handle) == -1)
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
		const auto pos		= m_transform->GetPos(CoordinateKind::kWorld);
		const auto rot		= math::ConvertQuaternionToRotMatrix(m_transform->GetQuaternion(CoordinateKind::kWorld));
		const auto angle	= math::ConvertZXYRotMatrixToEulerAngles(rot);
		const auto scale	= m_transform->GetScale(CoordinateKind::kWorld);

		SetPosPlayingEffekseer3DEffect		(m_playing_effect_handle, pos.x,   pos.y,   pos.z);
		SetRotationPlayingEffekseer3DEffect	(m_playing_effect_handle, angle.x, angle.y, angle.z);
		SetScalePlayingEffekseer3DEffect	(m_playing_effect_handle, scale.x, scale.y, scale.z);
	}
}

void Effect::ApplyPlaySpeed() const
{
	if (m_playing_effect_handle > -1)
	{
		SetSpeedPlayingEffekseer3DEffect(m_playing_effect_handle, m_play_speed * FPS::GetDeltaTime());
	}
}

void Effect::PlayEffect()
{
	if (IsEffekseer3DEffectPlaying(m_playing_effect_handle) == -1)
	{
		m_playing_effect_handle = PlayEffekseer3DEffect(m_origin_effect_handle);
		++m_play_count;
	}
}
