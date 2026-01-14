#pragma once
class Zombie;
class Animator;
namespace zombie_state { class State; }
enum class ZombieStateKind;

class ZombieStateBase abstract
{
public:
	ZombieStateBase(Zombie& zombie, zombie_state::State& state, const std::shared_ptr<Animator>& animator, const ZombieStateKind state_kind);
	~ZombieStateBase();

	virtual void Update()		abstract;
	virtual void LateUpdate()	abstract;
	virtual void Enter()		abstract;
	virtual void Exit()			abstract;

	[[nodiscard]] const ZombieStateKind GetStateKind() { return m_state_kind; }
	[[nodiscard]] virtual const ZombieStateKind GetNextStateKind() abstract;

protected:
	#pragma region 汎用行動
	void Move();
	#pragma endregion

protected:
	Zombie&						m_zombie;
	zombie_state::State&		m_state;
	std::shared_ptr<Animator>	m_animator;
	ZombieStateKind				m_state_kind;

private:
	bool						m_prev_run;
};
