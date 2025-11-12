#pragma once
#include "../Part/screen_creator.hpp"
#include "../Path/ui_graphic_path.hpp"
#include "../Part/mask_creator.hpp"
#include "../Event/event_system.hpp"

class DamageFilter final
{
public:
	DamageFilter();
	~DamageFilter();

	void Init();
	void LateUpdate();
	void Draw(const int main_screen_handle) const;

private:
	#pragma region Event
	void ActivateCutscene			(const StartRocketLauncherCutsceneEvent&	event);
	void DeactivateCutscene			(const EndRocketLauncherCutsceneEvent&		event);
	void Deactivate					(const DeadAllEnemyEvent&					event);
	void StartDamageBlinking		(const OnDamageToPlayerEvent&				event);
	void StartNearDeathBlinking		(const NearDeathEvent&						event);
	void StartEnterNearDeathBlinking(const EnterNearDeathEvent&					event);
	void StopNearDeathBlinkind		(const ChangeSceneEvent&					event);
	#pragma endregion

	
	void CalcResultScreenAlphaBlendNum();

private:
	static constexpr int	kEnterNearDeathAlphaBlendNum	= 190;
	static constexpr int	kNearDeathAlphaBlendNum			= 90;
	static constexpr int	kDamageMaxAlphaBlendNum			= 170;
	static constexpr int	kDamageMinAlphaBlendNum			= 70;
	static constexpr float	kDamageMaxParcent				= 0.25;
	static constexpr float	kDamageMinParcent				= 0.0625;
	static constexpr float	kBlinkingSpeed					= 2.0f;

	std::unique_ptr<ScreenCreator>	m_result_screen;
	std::unique_ptr<Graphicer>		m_graphicer;
	std::unique_ptr<ScreenCreator>	m_mask_screen;
	std::unique_ptr<MaskCreator>	m_mask_creator;

	float m_blinking_sin;
	int   m_result_screen_alpha_blend_num;
	int   m_max_alpha_blend_num;
	bool  m_is_loop_blinking;
	bool  m_is_near_death_first_blinking;
	bool  m_is_near_death;
	bool  m_can_blinking;
};
