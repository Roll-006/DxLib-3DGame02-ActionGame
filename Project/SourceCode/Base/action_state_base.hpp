#pragma once
#include "../Interface/i_state.hpp"

template<obj_concepts::ObjT ObjT>
class ActionStateBase abstract : public IState<ObjT>
{
public:
	ActionStateBase(const int state_kind) : m_state_kind(state_kind), m_is_active(true) {}
	virtual ~ActionStateBase() = default;

	[[nodiscard]] int GetStateKind() const { return m_state_kind; }

	/// @brief アクティブ化する
	void Activate()   override { m_is_active = true; }
	/// @brief 非アクティブ化する
	void Deactivate() override { m_is_active = false; }

	[[nodiscard]] virtual std::shared_ptr<IState<ObjT>> ChangeState(const ObjT* obj) abstract;
	[[nodiscard]] bool IsActive() const override { return m_is_active; }

private:
	int	 m_state_kind;
	bool m_is_active;
};
