#pragma once
#include "../Support/transform.hpp"
#include "../Support/modeler.hpp"
#include "../Support/animator.hpp"

class Player
{
public:
	Player();
	~Player();

	void Init();
	void Update();
	void Draw()const;

private:
	std::shared_ptr<Transform>	m_transform;
	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Animator>	m_animator;
};
