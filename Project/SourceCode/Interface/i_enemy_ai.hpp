#pragma once
#include <memory>

template<typename EnemyT>
class IEnemyAI abstract
{
public:
	virtual ~IEnemyAI() = default;

	virtual void Update		(const std::shared_ptr<EnemyT> enemy) abstract;
	virtual void LateUpdate	(const std::shared_ptr<EnemyT> enemy) abstract;
};
