#pragma once
#include <functional>
#include "../Event/event_system.hpp"

#include "screen_creator.hpp"
#include "../Window/window.hpp"

class ScreenFilter final
{
public:
	enum class BasisFilterKind
	{
		kNormal,	// ノーマルフィルター
		kCinematic,	// シネマティックフィルター
		kRetro,		// レトロフィルター
	};

public:
	ScreenFilter();
	~ScreenFilter();

	void Update();
	void UseFilter();
	void UnuseFilter();

	void Draw();


	#pragma region Event
	void SetNearDeathFilter	(const EnterNearDeathData&			event);
	void SetNormalFilter	(const OnSelectNormalFilterData&	event);
	void SetCinematicFilter	(const OnSelectCinematicFilterData& event);
	void SetRetroFilter		(const OnSelectRetroFilterData&		event);
	#pragma endregion

private:
	void UseNormalFilter();
	void UseCinematicFilter();
	void UseRetroFilter();

	void DrawBasisFilter();
	void DrawNearDeathFilter();

private:
	static constexpr float kBlinkingSpeed = 2.0f;

	std::unordered_map<BasisFilterKind, std::function<void()>> m_basis_filters;
	std::function<void()> m_current_basis_filter;
	int m_basis_alpha_blend_num;

	std::unique_ptr<ScreenCreator> m_main_screen;
	std::unique_ptr<ScreenCreator> m_basis_filter_screen;
	std::unique_ptr<ScreenCreator> m_near_death_filter_screen;

	bool  m_is_using_basis_filter;			// 基礎フィルターを使用しているかを判定
	bool  m_is_using_near_death_filter;		// 瀕死フィルターを使用しているかを判定

	float m_near_death_blinking_sin;
};
