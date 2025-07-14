#pragma once
#include "../Concept/obj_concepts.hpp"

template<obj_concepts::ObjT ObjT>
class IState abstract
{
public:
	virtual ~IState() = default;

	virtual void Update		(const ObjT* obj) abstract;
	virtual void ChangeState(const ObjT* obj) abstract;
};
