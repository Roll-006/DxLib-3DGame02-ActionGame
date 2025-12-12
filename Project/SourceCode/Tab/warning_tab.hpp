#pragma once
#include "../Interface/i_tab.hpp"

#include "../Event/event_system.hpp"
#include "../UI/ui_selector.hpp"
#include "../UI/button_prompt.hpp"
#include "../UIButton/sub_menu_select_button.hpp"
#include "../Part/scene_fader.hpp"
#include "../Part/button_graphic_getter.hpp"
#include "../Data/warning_tab_data.hpp"
#include "../JSON/json_loader.hpp"

class WarningTab final : public ITab
{
public:
	enum class WarningKind
	{
		kRestart,
		kQuitGame,
		kExit,
	};

public:
	WarningTab(const WarningKind kind);
	~WarningTab() override;

	void Init()			override;
	void Update()		override;
	void OnDraw(const int main_screen_handle) const override;

	void Activate() 	override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	void AllowSelect()		override { m_can_select = true; }
	void DisallowSelect()	override { m_can_select = false; }

	[[nodiscard]] int  GetTabHandle()	const override	{ return m_tab_handle; }
	[[nodiscard]] int  GetPriority()	const override	{ return data.priority; }
	[[nodiscard]] bool IsActive()		const override	{ return m_is_active; }
	[[nodiscard]] bool CanSelect()		const override	{ return m_can_select; }
	[[nodiscard]] bool IsDecide()		const			{ return m_is_decide; }
	[[nodiscard]] bool IsBack()			const			{ return m_is_execute_back && m_result_screen->GetGraphicer()->GetBlendNum() <= 0; }

private:
	void ExecuteDecide();
	void ExecuteBack();

	void JudgeSelect();

	void CalcAlphaBlendNum();
	void CreateResultScreen();

	/// @brief タブを閉じる
	/// @brief UIボタンからではなくタブを開くコマンドから閉じる
	void BackTab();

private:
	WarningTabData							data;

	int										m_tab_handle;
	bool									m_is_active;
	bool									m_can_select;
	bool									m_is_decide;
	bool									m_is_execute_back;
	int										m_alpha_blend_num;
	std::shared_ptr<UISelector>				m_ui_selector;
	std::shared_ptr<Graphicer>				m_warning_icon_graphic;
	std::shared_ptr<ScreenCreator>			m_result_screen;

	std::shared_ptr<ButtonPrompt>			m_button_prompt;
};
