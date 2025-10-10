#include "damage_filter.hpp"

DamageFilter::DamageFilter() : 
	m_graphicer						(std::make_unique<Graphicer>(UIGraphicPath.DAMAGE_FILTER)),
	m_blinking_sin					(DX_PI_F),
	m_max_alpha_blend_num			(255),
	m_is_loop_blinking				(false),
	m_is_near_death_first_blinking	(false),
	m_is_near_death					(false)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<OnDamageToPlayerEvent>		(this, &DamageFilter::StartDamageBlinking);
	EventSystem::GetInstance()->Subscribe<NearDeathEvent>		(this, &DamageFilter::StartNearDeathBlinking);
	EventSystem::GetInstance()->Subscribe<EnterNearDeathEvent>	(this, &DamageFilter::StartEnterNearDeathBlinking);

	m_graphicer->SetCenterPos(Window::kCenterPos);
}

DamageFilter::~DamageFilter()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<OnDamageToPlayerEvent>		(this, &DamageFilter::StartDamageBlinking);
	EventSystem::GetInstance()->Unsubscribe<NearDeathEvent>		(this, &DamageFilter::StartNearDeathBlinking);
	EventSystem::GetInstance()->Unsubscribe<EnterNearDeathEvent>(this, &DamageFilter::StartEnterNearDeathBlinking);
}

void DamageFilter::LateUpdate()
{
	if (m_blinking_sin >= DX_PI_F) { return; }

	// 透過値を取得
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_blinking_sin, kBlinkingSpeed * delta_time, DX_PI_F, m_is_loop_blinking);
	const auto blend_alpha_num = (sin(m_blinking_sin) * 0.5f + 0.5f) * m_max_alpha_blend_num;
	m_graphicer->SetAlphaBlendNum(blend_alpha_num);

	// 瀕死時初期点滅を解除
	if (m_is_near_death_first_blinking && m_blinking_sin == 0.0f)
	{
		m_is_near_death_first_blinking = false;
	}

	// 点滅継続判定
	if (!m_is_near_death)
	{
		m_is_loop_blinking = false;
	}
	m_is_near_death = false;
}

void DamageFilter::Draw() const
{
	if (m_blinking_sin >= DX_PI_F) { return; }

	m_graphicer->Draw();
}


#pragma region Event
void DamageFilter::StartDamageBlinking(const OnDamageToPlayerEvent& event)
{
	if (!m_is_loop_blinking)
	{
		m_blinking_sin = 0.0f;

		// ダメージ量によってフィルターの透過値を変化させる
		if (event.damage_parcent < kDamageMinParcent)
		{
			m_max_alpha_blend_num = kDamageMinAlphaBlendNum;
		}
		else if (event.damage_parcent > kDamageMaxParcent)
		{
			m_max_alpha_blend_num = kDamageMaxAlphaBlendNum;
		}
		else
		{
			const auto unit_value = math::GetUnitValue<float, float>(kDamageMinParcent, kDamageMaxParcent, event.damage_parcent);
			m_max_alpha_blend_num = unit_value * (kDamageMaxAlphaBlendNum - kDamageMinAlphaBlendNum) + kDamageMinAlphaBlendNum;
		}
	}
}

void DamageFilter::StartNearDeathBlinking(const NearDeathEvent& event)
{
	// 最初の点滅が終了した場合のブレンド値
	if (!m_is_near_death_first_blinking)
	{
		m_max_alpha_blend_num = kNearDeathAlphaBlendNum;
	}

	m_is_loop_blinking	= true;
	m_is_near_death		= true;
}

void DamageFilter::StartEnterNearDeathBlinking(const EnterNearDeathEvent& event)
{
	m_blinking_sin					= 0.0f;
	m_max_alpha_blend_num			= kEnterNearDeathAlphaBlendNum;
	m_is_near_death_first_blinking	= true;
}
#pragma endregion
