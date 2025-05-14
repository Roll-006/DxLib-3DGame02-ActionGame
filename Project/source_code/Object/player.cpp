#include "player.hpp"

Player::Player() : 
	m_transform	(std::make_shared<Transform>(v3d::GetZeroVector())),
	m_modeler	(std::make_shared<Modeler>	(m_transform, ModelPath.CHARA_01)),
	m_animator	(std::make_shared<Animator>	(m_modeler, 3.0f))
{

}

Player::~Player()
{

}

void Player::Init()
{

}

void Player::Update()
{
	m_animator->Update();
}

void Player::Draw()const
{
	m_modeler->Draw();
}
