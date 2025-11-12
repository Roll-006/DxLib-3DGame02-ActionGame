#include "damage_filter.hpp"

DamageFilter::DamageFilter() : 
	m_result_screen					(std::make_unique<ScreenCreator>(Window::kScreenSize, Window::kCenterPos)),
	m_graphicer						(std::make_unique<Graphicer>(UIGraphicPath.DAMAGE_FILTER)),
	m_mask_screen					(std::make_unique<ScreenCreator>(Window::kScreenSize)),
	m_mask_creator					(std::make_unique<MaskCreator>()),
	m_blinking_sin					(DX_PI_F),
	m_result_screen_alpha_blend_num	(UCHAR_MAX),
	m_max_alpha_blend_num			(UCHAR_MAX),
	m_is_loop_blinking				(false),
	m_is_near_death_first_blinking	(false),
	m_is_near_death					(false),
	m_can_blinking					(true)
{
	// イベント登録
	EventSystem::GetInstance()->Subscribe<OnDamageToPlayerEvent>			(this, &DamageFilter::StartDamageBlinking);
	EventSystem::GetInstance()->Subscribe<NearDeathEvent>					(this, &DamageFilter::StartNearDeathBlinking);
	EventSystem::GetInstance()->Subscribe<EnterNearDeathEvent>				(this, &DamageFilter::StartEnterNearDeathBlinking);
	EventSystem::GetInstance()->Subscribe<ChangeSceneEvent>					(this, &DamageFilter::StopNearDeathBlinkind);
	EventSystem::GetInstance()->Subscribe<StartRocketLauncherCutsceneEvent>	(this, &DamageFilter::ActivateCutscene);
	EventSystem::GetInstance()->Subscribe<EndRocketLauncherCutsceneEvent>	(this, &DamageFilter::DeactivateCutscene);
	EventSystem::GetInstance()->Subscribe<DeadAllEnemyEvent>				(this, &DamageFilter::Deactivate);

	m_graphicer->SetCenterPos(Window::kCenterPos);
	
	m_mask_screen->UseScreen();
	m_graphicer->Draw();
	m_mask_screen->UnuseScreen();
}

DamageFilter::~DamageFilter()
{
	// イベントの登録解除
	EventSystem::GetInstance()->Unsubscribe<OnDamageToPlayerEvent>				(this, &DamageFilter::StartDamageBlinking);
	EventSystem::GetInstance()->Unsubscribe<NearDeathEvent>						(this, &DamageFilter::StartNearDeathBlinking);
	EventSystem::GetInstance()->Unsubscribe<EnterNearDeathEvent>				(this, &DamageFilter::StartEnterNearDeathBlinking);
	EventSystem::GetInstance()->Unsubscribe<ChangeSceneEvent>					(this, &DamageFilter::StopNearDeathBlinkind);
	EventSystem::GetInstance()->Unsubscribe<StartRocketLauncherCutsceneEvent>	(this, &DamageFilter::ActivateCutscene);
	EventSystem::GetInstance()->Unsubscribe<EndRocketLauncherCutsceneEvent>		(this, &DamageFilter::DeactivateCutscene);
	EventSystem::GetInstance()->Unsubscribe<DeadAllEnemyEvent>					(this, &DamageFilter::Deactivate);
}

void DamageFilter::Init()
{
	m_result_screen_alpha_blend_num = UCHAR_MAX;
	m_max_alpha_blend_num			= UCHAR_MAX;
	m_is_loop_blinking				= false;
	m_is_near_death_first_blinking	= false;
	m_is_near_death					= false;
	m_can_blinking					= true;
}

void DamageFilter::LateUpdate()
{
	if (m_blinking_sin >= DX_PI_F) { return; }

	// 透過値を取得
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);
	math::Increase(m_blinking_sin, kBlinkingSpeed * delta_time, DX_PI_F, m_is_loop_blinking);
	const auto blend_alpha_num = (sin(m_blinking_sin) * 0.5f + 0.5f) * m_max_alpha_blend_num;
	m_graphicer->SetBlendNum(blend_alpha_num);

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

	CalcResultScreenAlphaBlendNum();
}

void DamageFilter::Draw(const int main_screen_handle) const
{
	if (m_blinking_sin >= DX_PI_F) { return; }

	m_result_screen->UseScreen();

	m_mask_creator->CreateMask();
	m_mask_creator->UseMask(m_mask_screen->GetScreenHandle(), true);
	DrawGraph(0, 0, main_screen_handle, TRUE);
	m_mask_creator->UnuseMask();
	m_mask_creator->DeleteMask();

	m_graphicer->Draw();
	m_result_screen->UnuseScreen();

	m_result_screen->Draw();
}


#pragma region Event
void DamageFilter::ActivateCutscene(const StartRocketLauncherCutsceneEvent& event)
{
	m_can_blinking = false;
}

void DamageFilter::DeactivateCutscene(const EndRocketLauncherCutsceneEvent& event)
{
	m_can_blinking = true;
}

void DamageFilter::Deactivate(const DeadAllEnemyEvent& event)
{
	m_can_blinking = true;
}

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

void DamageFilter::StopNearDeathBlinkind(const ChangeSceneEvent& event)
{
	m_is_near_death_first_blinking = false;
}
#pragma endregion


void DamageFilter::CalcResultScreenAlphaBlendNum()
{
	// TODO : マジックナンバーの削除
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(TimeScaleLayerKind::kUI);

	if (m_can_blinking)
	{
		math::Increase(m_result_screen_alpha_blend_num, static_cast<int>(700.0f * delta_time), UCHAR_MAX, false);
	}
	else
	{
		math::Decrease(m_result_screen_alpha_blend_num, static_cast<int>(700.0f * delta_time), 0);
	}

	m_result_screen->GetGraphicer()->SetBlendNum(m_result_screen_alpha_blend_num);
}
