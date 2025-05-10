#pragma once
#include "../../Base/component_base.hpp"

#include "../../Data/Path/animation_path.hpp"
#include "../../Data/Tag/animation_tag.hpp"
#include "../../Data/animator_data.hpp"
#include "../../Concept/common_concepts.hpp"

#include "modeler_component.hpp"


// 仮
#include "../../Manager/input_checker.hpp"


class AnimatorComponent : public ComponentBase
{
public:
	enum class TimeState
	{
		kPrev,
		kCurrent,
	};

public:
	AnimatorComponent(GameObj* owner_obj, float blend_speed);
	~AnimatorComponent()override;

	void Init()		 override;
	void Update()	 override;
	void LateUpdate()override;
	void Draw()const override;

	void AddAnimHandle(int kind, std::string file_path, std::string tag, float play_speed, bool is_loop);

private:
	void AttachAnim(int next_kind);
	void DetachAnim(TimeState time_state);

	void SetAnimState();
	void SetPlayStartTime();

	void Play();
	void Blend();

private:
	int m_model_handle;

	// TODO : 命名がややこしいため変更予定
	std::unordered_map<int, AnimKindData>				m_kind_data;		// 各アニメーションのデータ
	std::unordered_map<TimeState, AnimTimeStateData>	m_time_state_data;	// 

	float m_prev_anim_play_rate;			// 前回のアニメーションの再生率
	float m_blend_rate;						// ブレンド率
	float m_blend_speed;					// ブレンド速度
	bool  m_is_first_frame_change_anim;		// アニメーションが変更されて最初のフレームであるかどうか
};
