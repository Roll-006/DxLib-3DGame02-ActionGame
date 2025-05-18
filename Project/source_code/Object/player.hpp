#pragma once
#include "../Support/modeler.hpp"
#include "../Support/animator.hpp"

#include "../Object/camera.hpp"

#include "../Data/Kind/player_kind.hpp"

class Player : public ObjBase
{
public:
	Player(std::shared_ptr<Camera> camera);
	~Player();

	void Init();
	void Update();
	void Draw()const;

	void ChangeAnimState();

	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler() const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<Animator>	GetAnimator()const { return m_animator; }

private:
	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Animator>	m_animator;

	std::shared_ptr<Camera>		m_camera;
};
