#include "animator.hpp"

Animator::Animator(std::shared_ptr<Modeler> modeler, const float blend_speed) :
	m_prev_anim_play_rate		(0.0f),
	m_blend_rate				(0.0f),
	m_blend_speed				(blend_speed),
	m_is_first_frame_change_anim(false),
	m_modeler					(modeler)

{
	m_time_state_data[TimeState::kPrev] = m_time_state_data[TimeState::kCurrent] = AnimTimeStateData(-1, -1, 0.0f);
}

Animator::~Animator()
{
	for (auto& data : m_kind_data)
	{
		MV1DeleteModel(data.second.anim_handle);
	}
}

void Animator::Init()
{
	DetachAnim(TimeState::kPrev);
	DetachAnim(TimeState::kCurrent);
}

void Animator::Update()
{
	m_is_first_frame_change_anim = false;

	BlendAnim();
	PlayAnim();
}

void Animator::AddAnimHandle(const int kind, const std::string& file_path, const std::string& tag, const float play_speed, const bool is_loop)
{
	// 上書き不可
	if (m_kind_data.count(kind)) { return; }

	m_kind_data[kind] = AnimKindData(MV1LoadModel(file_path.c_str()), tag, play_speed, is_loop);
}

void Animator::AttachAnim(const int next_kind)
{
	// 現在のアニメーションと同じであった場合は変更しない
	if (m_time_state_data.at(TimeState::kCurrent).kind == next_kind) { return; }

	m_is_first_frame_change_anim = true;

	DetachAnim(TimeState::kPrev);

	// データを移行(Current ➡ Prev, Next ➡ Current)
	m_time_state_data.at(TimeState::kPrev)				   = m_time_state_data.at(TimeState::kCurrent);
	m_time_state_data.at(TimeState::kCurrent).attach_index = MV1AttachAnim(m_modeler->GetModelHandle(), 1, m_kind_data.at(next_kind).anim_handle, TRUE);
	m_time_state_data.at(TimeState::kCurrent).kind		   = next_kind;
	SetPlayStartTime();

	// 前回のアニメーションが存在しない場合は、ブレンド済み(ブレンド率100%)とする
	m_blend_rate = m_time_state_data.at(TimeState::kPrev).attach_index > -1 ? 0.0f : 1.0f;
}

void Animator::DetachAnim(const TimeState time_state)
{
	if (m_time_state_data.at(time_state).attach_index > -1)
	{
		MV1DetachAnim(m_modeler->GetModelHandle(), m_time_state_data.at(time_state).attach_index);
		m_time_state_data.at(time_state).attach_index = -1;
	}
}

void Animator::SetPlayStartTime()
{
	if (m_kind_data.count(m_time_state_data.at(TimeState::kPrev).kind))
	{
		std::string prev_tag	= m_kind_data.at(m_time_state_data.at(TimeState::kPrev).kind).tag;
		std::string current_tag = m_kind_data.at(m_time_state_data.at(TimeState::kCurrent).kind).tag;

		// 同類アニメーションであった場合は再生率を引き継ぐ
		if (prev_tag == current_tag)
		{
			float current_total_t = MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), m_time_state_data.at(TimeState::kCurrent).attach_index);
			float prev_total_t	  = MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), m_time_state_data.at(TimeState::kPrev).attach_index);

			m_prev_anim_play_rate = m_time_state_data.at(TimeState::kPrev).play_timer / prev_total_t;
			m_time_state_data.at(TimeState::kCurrent).play_timer = current_total_t * m_prev_anim_play_rate;
			return;
		}
	}
	m_time_state_data.at(TimeState::kCurrent).play_timer = 0.0f;
}

void Animator::PlayAnim()
{
	for (auto& [state_t, data] : m_time_state_data)
	{
		if (data.attach_index > -1)
		{
			float total_t = MV1GetAttachAnimTotalTime(m_modeler->GetModelHandle(), data.attach_index);
			float blend_r = state_t == TimeState::kCurrent ? m_blend_rate : 1.0f - m_blend_rate;

			data.play_timer += m_kind_data.at(data.kind).play_speed * FPS::GetDeltaTime();
			if (data.play_timer > total_t)
			{
				data.play_timer = m_kind_data.at(data.kind).is_loop ? 0.0f : total_t;
			}

			MV1SetAttachAnimTime(m_modeler->GetModelHandle(), data.attach_index, data.play_timer);
			MV1SetAttachAnimBlendRate(m_modeler->GetModelHandle(), data.attach_index, blend_r);
		}
	}
}

void Animator::BlendAnim()
{
	if (m_blend_rate >= 1.0f) { return; }

	m_blend_rate += m_blend_speed * FPS::GetDeltaTime();
	if (m_blend_rate > 1.0f)
	{
		m_blend_rate = 1.0f;
	}
}
