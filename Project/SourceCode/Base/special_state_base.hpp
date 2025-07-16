#pragma once
#include "../Interface/i_state.hpp"

template<obj_concepts::ObjT ObjT>
class SpecialStateBase abstract : public IState<ObjT>
{
public:
	SpecialStateBase(const int state_kind) : m_state_kind(state_kind){}
	virtual ~SpecialStateBase() = default;

	[[nodiscard]] int GetStateKind() const { return m_state_kind; }

private:
	int m_state_kind;
};
