#pragma once
#include "../Interface/i_ui_creator.hpp"

#include "../UI/game_clear_text.hpp"
#include "../Event/event_system.hpp"

class GameClearUiCreator final : public IUICreator
{
public:
	GameClearUiCreator();
	~GameClearUiCreator()	override;

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
	void Activate(const DeadBossEvent& event);

private:
	std::string m_name;
	int			m_priority;
	bool		m_is_active;

	std::shared_ptr<GameClearText> m_game_clear_text;
};
