#pragma once
#include <unordered_map>

#include "../Manager/handle_keeper.hpp"
#include "../Data/Tag/animation_tag.hpp"
#include "../Data/animator_data.hpp"
#include "../Concept/common_concepts.hpp"
#include "../FPS/fps.hpp"

#include "modeler.hpp"

class Animator final
{
public:
	Animator(const std::shared_ptr<Modeler> modeler);
	~Animator();

	void Init();
	void Update();

	/// @brief アニメーションをアタッチする
	/// @brief デタッチ処理は自動的に実行
	void AttachAnim(const int next_kind);

	void AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop);
	void AddAnimHandle(const int kind, const int anim_handle,		 const int index, const std::string& tag, const float play_speed, const bool is_loop);

private:
	void DetachAnim(const TimeKind time_kind);

	/// @brief 再生開始地点を設定
	void SetPlayStartTime();

	void PlayAnim();
	void BlendAnim();

private:
	static constexpr float kBlendSpeed = 3.0f;

	std::unordered_map<int, AnimKindData>			m_anim_data;
	std::unordered_map<TimeKind, AnimTimeKindData>	m_time_kind_data;

	float m_prev_anim_play_rate;			// 前回のアニメーションの再生率
	float m_blend_rate;
	float m_blend_speed;
	bool  m_is_first_frame_change_anim;		// アニメーションが変更されて最初のフレームであるかどうか

	std::shared_ptr<Modeler> m_modeler;
};
