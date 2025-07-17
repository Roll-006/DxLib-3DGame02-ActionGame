#include "animator_base.hpp"

AnimatorBase::AnimatorBase(const std::shared_ptr<Modeler> modeler) :
	m_prev_anim_play_rate	(0.0f),
	m_blend_rate			(1.0f),
	m_modeler				(modeler)

{
	m_time_kind_data[TimeKind::kPrev] = m_time_kind_data[TimeKind::kCurrent] = AnimTimeKindData();
}

AnimatorBase::~AnimatorBase()
{
	for (auto& data : m_anim_data)
	{
		MV1DeleteModel(data.second.anim_handle);
	}
}

void AnimatorBase::AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop)
{
	// 上書き不可
	if (m_anim_data.count(kind)) { return; }

	int handle = HandleKeeper::GetInstance()->LoadHandle(HandleKind::kAnim, file_path);
	if (handle != -1)
	{
		m_anim_data[kind] = AnimKindData(handle, index, tag, play_speed, is_loop);
	}
}

void AnimatorBase::AddAnimHandle(const int kind, const int anim_handle, const int index, const std::string& tag, const float play_speed, const bool is_loop)
{
	// 上書き不可
	if (m_anim_data.count(kind)) { return; }

	if (anim_handle != -1)
	{
		m_anim_data[kind] = AnimKindData(anim_handle, index, tag, play_speed, is_loop);
	}
}

void AnimatorBase::AttachAnim(const int next_kind)
{	
	if (!m_anim_data.count(next_kind))								{ return; }	// アニメーションが存在しない場合は早期return
	if (m_time_kind_data.at(TimeKind::kCurrent).kind == next_kind)	{ return; }	// 現在のアニメーションと同じであった場合はアタッチを許可しない	
	if (m_blend_rate != 1.0f)										{ return; }	// ブレンドが完了していない場合はアタッチを許可しない

	DetachAnim(TimeKind::kPrev);

	// データをシフト(Current ➡ Prev, Next ➡ Current)
	m_time_kind_data.at(TimeKind::kPrev)					= m_time_kind_data.at(TimeKind::kCurrent);
	m_time_kind_data.at(TimeKind::kCurrent).attach_index	= MV1AttachAnim(m_modeler->GetModelHandle(), m_anim_data.at(next_kind).index, m_anim_data.at(next_kind).anim_handle, TRUE);
	m_time_kind_data.at(TimeKind::kCurrent).kind			= next_kind;
	SetPlayStartTime();

	// 前回のアニメーションが存在しない場合は、ブレンド済み(ブレンド率100%)とする
	m_blend_rate = m_time_kind_data.at(TimeKind::kPrev).attach_index > -1 ? 0.0f : 1.0f;
}

void AnimatorBase::DetachAnim(const TimeKind time_kind)
{
	if (m_time_kind_data.at(time_kind).attach_index > -1)
	{
		MV1DetachAnim(m_modeler->GetModelHandle(), m_time_kind_data.at(time_kind).attach_index);
		m_time_kind_data.at(time_kind).attach_index = -1;
	}
}

void AnimatorBase::SetPlayStartTime()
{
	if (m_anim_data.count(m_time_kind_data.at(TimeKind::kPrev).kind))
	{
		const std::string prev_tag	  = m_anim_data.at(m_time_kind_data.at(TimeKind::kPrev).kind).tag;
		const std::string current_tag = m_anim_data.at(m_time_kind_data.at(TimeKind::kCurrent).kind).tag;

		// 同類アニメーションであった場合は再生率を引き継ぐ
		if (prev_tag == current_tag)
		{
			const float current_total_t = MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), m_time_kind_data.at(TimeKind::kCurrent).attach_index);
			const float prev_total_t	= MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), m_time_kind_data.at(TimeKind::kPrev).attach_index);

			m_prev_anim_play_rate = m_time_kind_data.at(TimeKind::kPrev).play_timer / prev_total_t;
			m_time_kind_data.at(TimeKind::kCurrent).play_timer = current_total_t * m_prev_anim_play_rate;

			return;
		}
	}

	m_time_kind_data.at(TimeKind::kCurrent).play_timer = 0.0f;
}

void AnimatorBase::PlayAnim()
{
	for (auto& [state_t, data] : m_time_kind_data)
	{
		// アニメーションが有効であった場合のみ再生
		if (data.attach_index > -1)
		{
			const float total_t = MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), data.attach_index);
			const float blend_r = state_t == TimeKind::kCurrent ? m_blend_rate : 1.0f - m_blend_rate;

			float play_speed = m_anim_data.at(data.kind).play_speed * FPS::GetDeltaTime();
			math::IncreaseLoop(data.play_timer, play_speed, total_t, m_anim_data.at(data.kind).is_loop);

			// 再生位置・ブレンド率を適用
			MV1SetAttachAnimTime	 (m_modeler->GetModelHandle(), data.attach_index, data.play_timer);
			MV1SetAttachAnimBlendRate(m_modeler->GetModelHandle(), data.attach_index, blend_r);
		}
	}
}

void AnimatorBase::BlendAnim()
{
	// ブレンド率100%まで増加させる
	math::Increase(m_blend_rate, kBlendSpeed * FPS::GetDeltaTime(), 1.0f);
  
	// ブレンドが完了した場合、PravAnimは不要なためデタッチする
	if (m_blend_rate == 1.0f)
	{
		DetachAnim(TimeKind::kPrev);
	}
}
