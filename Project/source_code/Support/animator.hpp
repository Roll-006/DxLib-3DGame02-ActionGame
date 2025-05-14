#pragma once
#include <unordered_map>

#include "../Data/Path/animation_path.hpp"
#include "../Data/Tag/animation_tag.hpp"
#include "../Data/animator_data.hpp"
#include "../Concept/common_concepts.hpp"
#include "../FPS/fps.hpp"

#include "modeler.hpp"

class Animator
{
public:
	enum class TimeState
	{
		kPrev,
		kCurrent,
	};

public:
	Animator(std::shared_ptr<Modeler> modelr, float blend_speed);
	~Animator();

	void Init();
	void Update();

	void AddAnimHandle(int kind, std::string file_path, std::string tag, float play_speed, bool is_loop);

private:
	void AttachAnim(int next_kind);
	void DetachAnim(TimeState time_state);

	void SetAnimState();
	void SetPlayStartTime();

	void Play();
	void Blend();

private:
	// TODO : 命名がややこしいため変更予定
	std::unordered_map<int, AnimKindData>				m_kind_data;
	std::unordered_map<TimeState, AnimTimeStateData>	m_time_state_data;

	float m_prev_anim_play_rate;			// 前回のアニメーションの再生率
	float m_blend_rate;
	float m_blend_speed;
	bool  m_is_first_frame_change_anim;		// アニメーションが変更されて最初のフレームであるかどうか

	std::shared_ptr<Modeler> m_modelr;
};
