#include "animator_component.hpp"

AnimatorComponent::AnimatorComponent(GameObj* owner_obj, float blend_speed) :
	ComponentBase				(owner_obj),
	m_model_handle				(-1),
	m_prev_anim_play_rate		(0.0f),
	m_blend_rate				(0.0f),
	m_blend_speed				(blend_speed),
	m_is_first_frame_change_anim(false)
{
	m_time_state_data[TimeState::kPrev] = m_time_state_data[TimeState::kCurrent] = AnimTimeStateData(-1, -1, 0.0f);
	m_model_handle = m_owner_obj->GetComponent<ModelerComponent>()->GetModelHandle();
}

AnimatorComponent::~AnimatorComponent()
{
	for (auto& data : m_kind_data)
	{
		MV1DeleteModel(data.second.anim_handle);
	}
}

void AnimatorComponent::Init()
{
	DetachAnim(TimeState::kPrev);
	DetachAnim(TimeState::kCurrent);
}

void AnimatorComponent::Update()
{
	m_is_first_frame_change_anim = false;

	SetAnimState();
	Blend();
	Play();
}

void AnimatorComponent::LateUpdate()
{

}

void AnimatorComponent::Draw()const
{
	// 処理なし
}

void AnimatorComponent::AddAnimHandle(int kind, std::string file_path, std::string tag, float play_speed, bool is_loop)
{
	// 上書き不可
	if (m_kind_data.count(kind)) { return; }

	m_kind_data[kind] = AnimKindData(MV1LoadModel(file_path.c_str()), tag, play_speed, is_loop);
}

void AnimatorComponent::AttachAnim(int next_kind)
{
	if (m_time_state_data.at(TimeState::kCurrent).kind == next_kind) { return; }

	m_is_first_frame_change_anim = true;

	DetachAnim(TimeState::kPrev);

	// MEMO : mixamoから追加したアニメーションしか想定していない
	m_time_state_data.at(TimeState::kPrev)					= m_time_state_data.at(TimeState::kCurrent);
	m_time_state_data.at(TimeState::kCurrent).attach_index	= MV1AttachAnim(m_model_handle, 1, m_kind_data.at(next_kind).anim_handle, TRUE);
	m_time_state_data.at(TimeState::kCurrent).kind			= next_kind;
	SetPlayStartTime();

	m_blend_rate = m_time_state_data.at(TimeState::kPrev).attach_index > -1 ? 0.0f : 1.0f;
}

void AnimatorComponent::DetachAnim(TimeState time_state)
{
	if (m_time_state_data.at(time_state).attach_index > -1)
	{
		MV1DetachAnim(m_model_handle, m_time_state_data.at(time_state).attach_index);
		m_time_state_data.at(time_state).attach_index = -1;
	}
}

void AnimatorComponent::SetAnimState()
{
	// TODO : 仮実装のため後に変更
	if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_1) == InputState::kHold)
	{
		AttachAnim(1);
	}
	else if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_2) == InputState::kHold)
	{
		AttachAnim(2);
	}
	else if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_3) == InputState::kHold)
	{
		AttachAnim(3);
	}
	else
	{
		AttachAnim(0);
	}
}

void AnimatorComponent::SetPlayStartTime()
{
	if (m_kind_data.count(m_time_state_data.at(TimeState::kPrev).kind))
	{
		std::string prev_tag    = m_kind_data.at(m_time_state_data.at(TimeState::kPrev).kind).tag;
		std::string current_tag = m_kind_data.at(m_time_state_data.at(TimeState::kCurrent).kind).tag;

		// 同類アニメーションであった場合は再生率を引き継ぐ
		if (prev_tag == current_tag)
		{
			float current_total_t = MV1GetAttachAnimTotalTime(m_model_handle, m_time_state_data.at(TimeState::kCurrent).attach_index);
			float prev_total_t	  = MV1GetAttachAnimTotalTime(m_model_handle, m_time_state_data.at(TimeState::kPrev).attach_index);

			m_prev_anim_play_rate = m_time_state_data.at(TimeState::kPrev).play_timer / prev_total_t;
			m_time_state_data.at(TimeState::kCurrent).play_timer = current_total_t * m_prev_anim_play_rate;
			return;
		}
	}

	m_time_state_data.at(TimeState::kCurrent).play_timer = 0.0f;
}

void AnimatorComponent::Play()
{
	for (auto& [state_t, data] : m_time_state_data)
	{
		if (data.attach_index > -1)
		{
			float total_t = MV1GetAttachAnimTotalTime(m_model_handle, data.attach_index);
			float blend_r = state_t == TimeState::kCurrent ? m_blend_rate : 1.0f - m_blend_rate;

			data.play_timer += m_kind_data.at(data.kind).play_speed * FPS::GetDeltaTime();
			if (data.play_timer > total_t)
			{
				data.play_timer = m_kind_data.at(data.kind).is_loop ? 0.0f : total_t;
			}

			MV1SetAttachAnimTime(m_model_handle, data.attach_index, data.play_timer);
			MV1SetAttachAnimBlendRate(m_model_handle, data.attach_index, blend_r);
		}
	}
}

void AnimatorComponent::Blend()
{
	if (m_blend_rate >= 1.0f) { return; }

	m_blend_rate += m_blend_speed * FPS::GetDeltaTime();
	if (m_blend_rate > 1.0f)
	{
		m_blend_rate = 1.0f;
	}
}
