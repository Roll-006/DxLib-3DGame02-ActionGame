//#include "zombie_ai.hpp"
//
//ZombieAI::ZombieAI(const std::shared_ptr<Player> player) : 
//	m_state				(std::make_shared<ZombieStateController>()),
//	m_player			(player),
//	m_distance_to_target(0.0f)
//{
//
//}
//
//ZombieAI::~ZombieAI()
//{
//
//}
//
//void ZombieAI::Update(const std::shared_ptr<Zombie> enemy)
//{
//	CalcDistanceToTarget(enemy);
//	m_state->Update(enemy);
//}
//
//void ZombieAI::LateUpdate(const std::shared_ptr<Zombie> enemy)
//{
//	m_state->LateUpdate(enemy);
//}
//
//void ZombieAI::CalcDistanceToTarget(const std::shared_ptr<Zombie> enemy)
//{
//	const auto target_pos	= m_player->GetTransform()->GetPos(CoordinateKind::kWorld);
//	const auto pos			= enemy   ->GetTransform()->GetPos(CoordinateKind::kWorld);
//
//	m_distance_to_target = VSize(target_pos - pos);
//}
