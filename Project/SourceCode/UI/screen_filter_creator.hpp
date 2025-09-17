#pragma once
#include "../Interface/i_ui_creator.hpp"
#include "../Interface/i_observer.hpp"

#include "cinema_scope_frame.hpp"

// FIXME : UIとは別のため分離の必要あり

class ScreenFilterCreator final : public IUICreator, public IObserver
{
public:
	ScreenFilterCreator();
	~ScreenFilterCreator()	override;

	void LateUpdate()	override;
	void OnDraw()		override;

	/// @brief アクティブ化する
	void Activate() 	override { m_is_active = true; }
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	override { m_is_active = false; }

	void OnNotify(const IEvent& event) override;

	[[nodiscard]] std::string GetName()		const override { return m_name; }
	[[nodiscard]] int		  GetPriority() const override { return m_priority; }
	[[nodiscard]] bool		  IsActive()	const override { return m_is_active; }

private:
	std::string m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<CinemaScopeFrame> m_cinema_scope_frame;
};
