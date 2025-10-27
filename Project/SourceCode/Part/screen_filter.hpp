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

	void Init();
	void Update();
	void UseFilter();
	void UnuseFilter();

	void CreateScreen();
	void Draw();


	#pragma region Getter
	[[nodiscard]] int GetScreenHandle() const { return m_result_screen->GetScreenHandle(); }
	#pragma endregion
	
private:
	#pragma region Event
	void Init				(const ChangeSceneEvent&				event);
	void SetNearDeathFilter	(const EnterNearDeathEvent&				event);
	void SetDeathFilter		(const DeadPlayerEvent&					event);
	void SetNormalFilter	(const OnSelectNormalFilterEvent&		event);
	void SetCinematicFilter	(const OnSelectCinematicFilterEvent&	event);
	void SetRetroFilter		(const OnSelectRetroFilterEvent&		event);
	#pragma endregion


	void UseNormalFilter();
	void UseCinematicFilter();
	void UseRetroFilter();

	void CreateFilter();
	void CreateBasisFilter();
	void CreateNearDeathFilter();
	void CreateDeathFilter();

private:
	static constexpr float kBlinkingSpeed = 2.0f;

	std::unordered_map<BasisFilterKind, std::function<void()>> m_basis_filters;
	std::function<void()> m_current_basis_filter;
	int m_basis_alpha_blend_num;

	std::shared_ptr<ScreenCreator> m_resource_screen;
	std::shared_ptr<ScreenCreator> m_result_screen;
	std::shared_ptr<ScreenCreator> m_basis_filter_screen;
	std::shared_ptr<ScreenCreator> m_near_death_filter_screen;
	std::shared_ptr<ScreenCreator> m_death_filter_screen;

	bool  m_is_using_basis_filter;			// 基礎フィルターを使用しているかを判定
	bool  m_is_using_near_death_filter;		// 瀕死フィルターを使用しているかを判定
	bool  m_is_using_death_filter;			// 死亡フィルターを使用しているかを判定

	float m_near_death_blinking_sin;
	int	  m_death_filter_alpha_blend_num;
};
