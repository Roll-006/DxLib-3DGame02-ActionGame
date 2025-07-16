#pragma once
#include "../Interface/i_state.hpp"

template<obj_concepts::ObjT ObjT>
class MoveStateBase abstract : public IState<ObjT>
{
public:
	MoveStateBase(const int state_kind) : m_state_kind(state_kind) {}
	virtual ~MoveStateBase() = default;

	[[nodiscard]] int GetStateKind() const { return m_state_kind; }

private:
	int m_state_kind;
};
