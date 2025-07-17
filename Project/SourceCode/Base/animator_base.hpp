#pragma once
#include <unordered_map>

#include "../Manager/handle_keeper.hpp"
#include "../Data/Tag/animation_tag.hpp"
#include "../Data/animator_data.hpp"
#include "../Concept/common_concepts.hpp"
#include "../FPS/fps.hpp"

#include "../Part/modeler.hpp"

class AnimatorBase abstract
{
public:
	AnimatorBase(const std::shared_ptr<Modeler> modeler);
	~AnimatorBase();

	virtual void Init()		abstract;
	virtual void Update()	abstract;

	/// @brief アニメーションをアタッチする
	/// @brief デタッチ処理は自動的に実行
	void AttachAnim(const int next_kind);

	void AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop);
	void AddAnimHandle(const int kind, const int anim_handle,		 const int index, const std::string& tag, const float play_speed, const bool is_loop);

	/// @brief 再生されているアニメーションの種類を取得
	[[nodiscard]] int GetAnimKind(const TimeKind time_kind) const { m_time_kind_data.at(time_kind).kind; }

	/// @brief ブレンド率を取得する
	[[nodiscard]] float GetBlendRate()const { m_blend_rate; }

protected:
	void PlayAnim();
	void BlendAnim();

private:
	virtual void LoadAnim() abstract;

	void DetachAnim(const TimeKind time_kind);

	/// @brief 再生開始地点を設定
	void SetPlayStartTime();

private:
	static constexpr float kBlendSpeed = 3.0f;

	std::unordered_map<int, AnimKindData>			m_anim_data;
	std::unordered_map<TimeKind, AnimTimeKindData>	m_time_kind_data;

	float m_prev_anim_play_rate;			// 前回のアニメーションの再生率
	float m_blend_rate;						// ブレンド率

	std::shared_ptr<Modeler> m_modeler;
};
