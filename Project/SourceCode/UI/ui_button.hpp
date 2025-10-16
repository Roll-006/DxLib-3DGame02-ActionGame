#pragma once
#include "../Interface/i_ui_element.hpp"

class UIButton final : public IUIElement
{
public:
	UIButton();
	~UIButton() override;

	void Init()			override;
	void Update()		override;
	void Draw() const	override;

	void Activate() 	override { m_is_active = true;  }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	bool m_is_active;
};
