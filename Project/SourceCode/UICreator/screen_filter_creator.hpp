#pragma once
#include "../Interface/i_ui_creator.hpp"

#include "../UI/cinema_scope_frame.hpp"
#include "../UI/damage_filter.hpp"

// FIXME : UIとは別のため分離の必要あり

class ScreenFilterCreator final : public IUICreator
{
public:
	ScreenFilterCreator();
	~ScreenFilterCreator()	override;

	void Init() override;
	void LateUpdate() override;
	void OnDraw(const int main_screen_handle) override;

	/// @brief アクティブ化する
	void Activate() 	override { m_is_active = true;  }
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] std::string GetName()		const override { return m_name; }
	[[nodiscard]] int		  GetPriority() const override { return m_priority; }
	[[nodiscard]] bool		  IsActive()	const override { return m_is_active; }

private:
	std::string m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<CinemaScopeFrame>	m_cinema_scope_frame;
	std::shared_ptr<DamageFilter>		m_damage_filter;
};
