#pragma once

class ZombieAI final
{
public:
	ZombieAI();
	~ZombieAI();

	void AttachTarget(const std::shared_ptr<CharacterBase>& target);
	void DetachTarget();

	[[nodiscard]] std::shared_ptr<CharacterBase> GetTarget() const { return m_target; }

private:
	std::shared_ptr<CharacterBase> m_target;
};
