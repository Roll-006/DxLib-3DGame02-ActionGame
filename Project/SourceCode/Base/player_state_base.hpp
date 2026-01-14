#pragma once
class Player;
class Animator;
namespace player_state { class State; }
enum class PlayerStateKind;
enum class PlayerAnimKind;

class PlayerStateBase abstract
{
public:
	PlayerStateBase(Player& player, player_state::State& state, const std::shared_ptr<Animator>& animator, const PlayerStateKind state_kind);
	~PlayerStateBase();

	virtual void Update()		abstract;
	virtual void LateUpdate()	abstract;
	virtual void Enter()		abstract;
	virtual void Exit()			abstract;

	[[nodiscard]] const PlayerStateKind GetStateKind() const { return m_state_kind; }
	[[nodiscard]] virtual const PlayerStateKind GetNextStateKind() abstract;
	[[nodiscard]] const bool IsCrouch() const { return m_is_crouch; }
	[[nodiscard]] const bool IsRun()	const { return m_is_run; }

protected:
	void Move();
	void BasicMove();

private:
	#pragma region 汎用Try判定
	[[nodiscard]] const bool TryMove()		const;
	[[nodiscard]] const bool TryCrouch()	const;
	[[nodiscard]] const bool TryRun()		const;
	#pragma endregion


	#pragma region 汎用行動
	void UpdateMove();

	void UpdateCrouch();

	void UpdateRun();
	#pragma endregion


	void AttachAnim(const std::function<void()>& lower_body_anim_function, const std::unordered_map<Animator::BodyKind, PlayerAnimKind>& anim_kind);

protected:
	Player&						m_player;
	player_state::State&		m_state;
	std::shared_ptr<Animator>	m_animator;
	PlayerStateKind				m_state_kind;
	std::unordered_map<Animator::BodyKind, PlayerAnimKind> m_basic_anim_kind;
	std::unordered_map<Animator::BodyKind, PlayerAnimKind> m_walk_forward_anim_kind;
	std::unordered_map<Animator::BodyKind, PlayerAnimKind> m_run_forward_anim_kind;
	std::unordered_map<Animator::BodyKind, PlayerAnimKind> m_crouch_walk_forward_anim_kind;
	std::unordered_map<Animator::BodyKind, PlayerAnimKind> m_crouch_anim_kind;

private:
	bool m_is_crouch;
	bool m_is_run;
	bool m_prev_crouch;
	bool m_prev_run;
};
