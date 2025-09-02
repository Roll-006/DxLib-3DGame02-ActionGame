//#pragma once
//#include <memory>
//
//#include "../Base/enemy_state_controller_base.hpp"
//
//template<typename EnemyT>
//class IEnemyAI abstract
//{
//public:
//	virtual ~IEnemyAI() = default;
//
//	virtual void Update		(const std::shared_ptr<EnemyT> enemy) abstract;
//	virtual void LateUpdate	(const std::shared_ptr<EnemyT> enemy) abstract;
//
//	[[nodiscard]] virtual std::shared_ptr<EnemyStateControllerBase<EnemyT>> GetStateController() const abstract;
//
//protected:
//	virtual void CalcDistanceToTarget(const std::shared_ptr<EnemyT> enemy) abstract;
//};
