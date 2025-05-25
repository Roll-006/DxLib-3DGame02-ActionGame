#pragma once
#include "../Part/modeler.hpp"
#include "../Part/animator.hpp"

#include "../Object/camera.hpp"

#include "../Data/Kind/player_kind.hpp"

class Player final : public CollideObjBase
{
public:
	Player(std::shared_ptr<Camera> camera);
	~Player();

	void Init()			override;
	void Update()		override;
	void Draw()const	override;

	void OnCollide(const CollideObjBase& check_hit_obj)override;

	void ChangeAnimState();

	[[nodiscard]] std::shared_ptr<Modeler>	GetModeler() const { return m_modeler; }
	[[nodiscard]] std::shared_ptr<Animator>	GetAnimator()const { return m_animator; }

private:
	void Move();

private:
	std::shared_ptr<Modeler>  m_modeler;
	std::shared_ptr<Animator> m_animator;

	std::shared_ptr<Camera>	  m_camera;
};
