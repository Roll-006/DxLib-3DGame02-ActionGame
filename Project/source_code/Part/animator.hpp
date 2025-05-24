#pragma once
#include <unordered_map>

#include "../Data/Path/animation_path.hpp"
#include "../Data/Tag/animation_tag.hpp"
#include "../Data/animator_data.hpp"
#include "../Concept/common_concepts.hpp"
#include "../FPS/fps.hpp"

#include "modeler.hpp"

class Animator final
{
public:
	enum class TimeState
	{
		kPrev,
		kCurrent,
	};

public:
	Animator(const std::shared_ptr<Modeler> modeler, const float blend_speed);
	~Animator();

	void Init();
	void Update();

	/// @brief アニメーションをアタッチする
	/// @brief デタッチ処理は自動的に実行
	void AttachAnim(const int next_kind);

	void AddAnimHandle(const int kind, const std::string& file_path, const std::string& tag, const float play_speed, const bool is_loop);

private:
	void DetachAnim(const TimeState time_state);

	/// @brief 再生開始地点を設定
	void SetPlayStartTime();

	void PlayAnim();
	void BlendAnim();

private:
	// TODO : 命名がややこしいため変更予定
	std::unordered_map<int, AnimKindData>				m_kind_data;
	std::unordered_map<TimeState, AnimTimeStateData>	m_time_state_data;

	float m_prev_anim_play_rate;			// 前回のアニメーションの再生率
	float m_blend_rate;
	float m_blend_speed;
	bool  m_is_first_frame_change_anim;		// アニメーションが変更されて最初のフレームであるかどうか

	std::shared_ptr<Modeler> m_modeler;
};
