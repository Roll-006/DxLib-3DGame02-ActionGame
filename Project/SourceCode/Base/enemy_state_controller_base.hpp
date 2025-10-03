#pragma once
#include <memory>
#include <typeindex>
#include "../Interface/i_state_controller.hpp"

#include "../Base/character_base.hpp"

template<obj_concepts::ObjT ObjT>
class EnemyStateControllerBase abstract : public IStateController<ObjT>
{
public:
	EnemyStateControllerBase() : m_target_character(nullptr){}

	virtual ~EnemyStateControllerBase() = default;

	void AttachTarget(const std::shared_ptr<CharacterBase>& target_character) { m_target_character = target_character; }
	void DetachTarget() { m_target_character = nullptr; }

	[[nodiscard]] std::shared_ptr<CharacterBase> GetTargetCharacter() const { return m_target_character; }

protected:
	std::shared_ptr<CharacterBase> m_target_character;
};
