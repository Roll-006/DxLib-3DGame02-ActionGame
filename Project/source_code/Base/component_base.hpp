#pragma once
#include <memory>

#include "../Concept/component_concepts.hpp"
#include "../Base/game_obj.hpp"

#include "../FPS/fps.hpp"

class GameObj;

class ComponentBase
{
public:
	ComponentBase(GameObj* owner_obj) : m_owner_obj(owner_obj){}
	virtual ~ComponentBase()	= default;

	virtual void Init()			= 0;
	virtual void Update()		= 0;
	virtual void LateUpdate()	= 0;
	virtual void Draw()const	= 0;

protected:
	GameObj* m_owner_obj;
};
