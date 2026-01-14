#pragma once
#include <unordered_map>

#include "../Handle/handle_keeper.hpp"
#include "../Part/modeler.hpp"
#include "../GameTime/game_time_manager.hpp"

#include "../Data/animator_data.hpp"
#include "../Tag/animation_tag.hpp"
#include "../Name/obj_name.hpp"
#include "../Event/event_system.hpp"

class Animator final
{
public:
	enum class BodyKind
	{
		kUpperBody,		// 上半身
		kLowerBody,		// 下半身
	};

public:
	Animator(const std::shared_ptr<Modeler>& modeler, const std::string& json_name);
	~Animator();

	void Update();

	void AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop, const bool is_self_blend = false, const float landing_paly_rate = 1.0f);

	/// @brief アニメーションをアタッチする
	/// @brief デタッチ処理は自動的に実行
	/// @param next_kind アタッチするアニメーションの種類
	/// @param body_kind アタッチする部位
	void AttachAnim(const int next_kind, const BodyKind body_kind);

	/// @brief リザルト(全身)アニメーションに直接アタッチする
	/// @param アタッチするアニメーションの種類
	void AttachResultAnim(const int next_kind);

	/// @brief 8方向移動を行うアニメーションのアタッチを行う
	/// @param forward_anim_kind 前方に移動するアニメーション
	void AttachAnimEightDir(
		const int  forward_anim_kind, 
		const bool is_move_forward, 
		const bool is_move_backward, 
		const bool is_move_left, 
		const bool is_move_right, 
		const bool is_result_attach);

	/// @brief 上半身と下半身の境目のボーンを設定する
	/// @brief 指定可能ボーン : HIPS, SPINE, SPINE_1, SPINE_2, NECK, HEAD, HEAD_TOP_END
	/// @param upper_body_end_frame 上半身の最下層とするボーン
	void DivideFrame(const TCHAR* upper_body_end_frame = FramePath.SPINE_2);


	#pragma region Getter
	/// @brief アニメーションのタグを取得
	/// @return タグ(該当のタグがない場合は[""]が返る)
	[[nodiscard]] int			GetAnimKind			(const BodyKind body_kind, const TimeKind time_kind) const;
	[[nodiscard]] std::string	GetAnimTag			(const BodyKind body_kind, const TimeKind time_kind) const;
	[[nodiscard]] float			GetGroundPlayRate	(const BodyKind body_kind) const;

	[[nodiscard]] float			GetBlendRate		(const BodyKind body_kind) const { return m_blend_rate.contains(body_kind) ? m_blend_rate.at(body_kind) : 0.0f; }
	[[nodiscard]] float			GetPlayRate			(const BodyKind body_kind) const;

	/// @brief 再生が終了したかを判定
	/// @brief ループ再生されないアニメーションでのみ有効
	[[nodiscard]] bool			IsPlayEnd			(const BodyKind body_kind) const;
	#pragma endregion

protected:
	void PlayAnim();
	void BlendAnim();

private:
	void DetachAnim(const TimeKind time_kind, const BodyKind body_kind);

	/// @brief 不変(上半身・下半身の行き来がない)ボーンを設定する
	void SetupStaticFrame();

	/// @brief 再生開始地点を設定
	void SetPlayStartTime(AnimTimeKindData* current_time_kind_data, const AnimTimeKindData& prev_time_kind_data, const BodyKind body_kind);

	/// @brief 上半身と下半身のアニメーションを合成する
	void CombineAnim();

	/// @brief 足が地面にあることを通知する
	void NotifyOnGround();
	void NotifyFoot(bool is_left, const std::string& tag);

	/// @brief アニメーションのアタッチが可能であるかを判定
	[[nodiscard]] bool CanResultAttachAnim();
	[[nodiscard]] bool CanAttachAnim(const int next_kind, const BodyKind body_kind);

private:
	std::unordered_map<int, AnimKindData>	anim_data;
	float									blend_speed;
	float									notify_ground_range;
	TimeScaleLayerKind						time_scale_layer_kind;

	std::unordered_map<BodyKind, std::unordered_map<TimeKind, AnimTimeKindData>>	m_time_kind_data;
	std::unordered_map<BodyKind, std::shared_ptr<Modeler>>							m_resource_modeler;
	std::shared_ptr<Modeler>														m_result_modeler;
	std::unordered_map<BodyKind, std::unordered_map<std::string, int>>				m_frame_numbers;
	std::unordered_map<BodyKind, float>												m_prev_anim_play_rate;		// 前回のアニメーションの再生率
	std::unordered_map<BodyKind, float>												m_blend_rate;				// ブレンド率
	bool																			m_is_on_ground;
	bool																			m_prev_left_on_ground;
	bool																			m_prev_right_on_ground;

	friend void from_json(const nlohmann::json& j_data, Animator& animator);
	friend void to_json  (nlohmann::json& j_data, const Animator& animator);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Animator& animator)
{
	j_data.at("anim_data")				.get_to(animator.anim_data);
	j_data.at("blend_speed")			.get_to(animator.blend_speed);
	j_data.at("notify_ground_range")	.get_to(animator.notify_ground_range);
	j_data.at("time_scale_layer_kind")	.get_to(animator.time_scale_layer_kind);
}

inline void to_json(nlohmann::json& j_data, const Animator& animator)
{
	j_data = nlohmann::json
	{
		{ "anim_data",				animator.anim_data },
		{ "blend_speed",			animator.blend_speed },
		{ "notify_ground_range",	animator.notify_ground_range },
		{ "time_scale_layer_kind",	animator.time_scale_layer_kind },
	};
}
#pragma endregion
