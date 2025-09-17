#pragma once
#include <functional>

#include "../Interface/i_observer.hpp"
#include "screen_creator.hpp"
#include "../Window/window.hpp"

class ScreenFilter final : public IObserver
{
public:
	enum class Kind
	{
		kVillain,
		kHeao,
		kNearDeath,
	};

public:
	ScreenFilter();
	~ScreenFilter();

	void OnNotify(const IEvent& event) override;

	void Update();
	void UseFilter();
	void UnuseFilter();

	void Draw() const;

private:
	void SetNearDeathFilter();

private:
	std::unordered_map<Kind, std::function<void()>> m_filters;
	std::shared_ptr<ScreenCreator> m_screen_creator;

	bool m_is_using;				
	bool m_is_using_basis_filter;	// 基礎フィルターを使用しているかを判定
};
