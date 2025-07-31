#pragma once
#include <unordered_map>

#include "../Handle/handle_keeper.hpp"
#include "../Data/Tag/animation_tag.hpp"
#include "../Data/animator_data.hpp"
#include "../Concept/common_concepts.hpp"
#include "../FPS/fps.hpp"

#include "../Part/modeler.hpp"

class AnimatorBase abstract
{
public:
	enum class BodyKind
	{
		kUpperBody,		// 上半身
		kLowerBody,		// 下半身
	};

public:
	AnimatorBase(const std::shared_ptr<Modeler> modeler);
	~AnimatorBase();

	virtual void Init()		abstract;
	virtual void Update()	abstract;

	/// @brief アニメーションをアタッチする
	/// @brief デタッチ処理は自動的に実行
	/// @param next_kind アタッチするアニメーションの種類
	/// @param body_kind アタッチする部位
	void AttachAnim(const int next_kind, const BodyKind body_kind);
	/// @brief リザルト(全身)アニメーションに直接アタッチする
	/// @param アタッチするアニメーションの種類
	void AttachResultAnim(const int next_kind);

	void AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop);
	void AddAnimHandle(const int kind, const int anim_handle,		 const int index, const std::string& tag, const float play_speed, const bool is_loop);

	/// @brief ブレンド率を取得する
	[[nodiscard]] float GetBlendRate()const { m_blend_rate; }

	/// @brief 再生が終了したかを判定
	/// @brief ループ再生されないアニメーションでのみ有効
	[[nodiscard]] bool IsPlayEnd(const BodyKind body_kind);

protected:
	void PlayAnim();
	void BlendAnim();

private:
	virtual void LoadAnim() abstract;

	void SetBoneNum();

	void DetachAnim(const TimeKind time_kind, const BodyKind body_kind);

	/// @brief 再生開始地点を設定
	void SetPlayStartTime(AnimTimeKindData* current_time_kind_data, const AnimTimeKindData& prev_time_kind_data, const BodyKind body_kind);

	/// @brief 上半身と下半身のアニメーションを合成する
	void CombineAnim();

	/// @brief アニメーションのアタッチが可能であるかを判定
	[[nodiscard]] bool CanAttachAnim(const int next_kind, const BodyKind body_kind);

private:
	static constexpr float kBlendSpeed = 3.0f;

	std::vector<std::tuple<BodyKind, TimeKind, AnimTimeKindData>>		m_time_kind_data;
	std::unordered_map<int, AnimKindData>								m_anim_data;
	std::unordered_map<BodyKind, std::shared_ptr<Modeler>>				m_resource_modeler;
	std::shared_ptr<Modeler>											m_result_modeler;
	std::unordered_map<BodyKind, std::unordered_map<std::string, int>>	m_bone_num;
	std::unordered_map<BodyKind, float>									m_prev_anim_play_rate;		// 前回のアニメーションの再生率
	std::unordered_map<BodyKind, float>									m_blend_rate;				// ブレンド率
};
