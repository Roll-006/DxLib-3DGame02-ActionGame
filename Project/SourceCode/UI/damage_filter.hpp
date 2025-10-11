#pragma once
#include "../Part/graphicer.hpp"
#include "../Path/ui_graphic_path.hpp"
#include "../Event/event_system.hpp"

class DamageFilter final
{
public:
	DamageFilter();
	~DamageFilter();

	void LateUpdate();
	void Draw() const;


	#pragma region Event
	void StartDamageBlinking		(const OnDamageToPlayerEvent&  event);
	void StartNearDeathBlinking		(const NearDeathEvent& event);
	void StartEnterNearDeathBlinking(const EnterNearDeathEvent& event);
	#pragma endregion

private:
	static constexpr int	kEnterNearDeathAlphaBlendNum	= 190;
	static constexpr int	kNearDeathAlphaBlendNum			= 90;
	static constexpr int	kDamageMaxAlphaBlendNum			= 170;
	static constexpr int	kDamageMinAlphaBlendNum			= 70;
	static constexpr float	kDamageMaxParcent				= 0.25;
	static constexpr float	kDamageMinParcent				= 0.0625;
	static constexpr float	kBlinkingSpeed					= 2.0f;

	std::unique_ptr<Graphicer> m_graphicer;
	float m_blinking_sin;
	int   m_max_alpha_blend_num;
	bool  m_is_loop_blinking;
	bool  m_is_near_death_first_blinking;
	bool  m_is_near_death;
};
