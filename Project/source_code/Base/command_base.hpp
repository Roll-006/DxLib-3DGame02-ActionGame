#pragma once
#include "obj_base.hpp"
#include "../Data/Kind/command_kind.hpp"

class CommandBase abstract
{
public:
	CommandBase(const CommandKind kind) : m_command_kind(kind){}

	virtual void Execute(ObjBase& obj) abstract;

	[[nodiscard]] CommandKind GetCommandKind() const { return m_command_kind; }
	
private:
	CommandKind m_command_kind;
};
