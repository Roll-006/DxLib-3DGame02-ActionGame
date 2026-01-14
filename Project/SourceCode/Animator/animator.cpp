#include "../JSON/json_loader.hpp"
#include "animator.hpp"

Animator::Animator(const std::shared_ptr<Modeler>& modeler, const std::string& json_name) :
	m_result_modeler		(modeler),
	m_is_on_ground			(false),
	m_prev_left_on_ground	(false),
	m_prev_right_on_ground	(false)
{
	nlohmann::json j_data;
	if (json_loader::Load("Data/JSON/animator.json", j_data))
	{
		const auto handle_keeper = HandleKeeper::GetInstance();
		const auto anim = j_data.at("animator").at(json_name.c_str());
		
		for (const auto& [kind, anim_kind_data] : anim.at("anim_data").items())
		{
			const auto data = anim_kind_data.get<AnimKindData>();
			anim_data[atoi(kind.c_str())] = data;
			anim_data[atoi(kind.c_str())].anim_handle = handle_keeper->LoadHandle(HandleKind::kAnim, data.path);
		}

		blend_speed				= anim.at("blend_speed");
		notify_ground_range		= anim.at("notify_ground_range");
		time_scale_layer_kind	= anim.at("time_scale_layer_kind").get<TimeScaleLayerKind>();
	}

	m_resource_modeler[BodyKind::kUpperBody] = std::make_shared<Modeler>(m_result_modeler->GetModelHandle());
	m_resource_modeler[BodyKind::kLowerBody] = std::make_shared<Modeler>(m_result_modeler->GetModelHandle());

	m_blend_rate[BodyKind::kUpperBody] = 1.0f;
	m_blend_rate[BodyKind::kLowerBody] = 1.0f;

	m_time_kind_data[BodyKind::kUpperBody][TimeKind::kPrev]		= AnimTimeKindData();
	m_time_kind_data[BodyKind::kUpperBody][TimeKind::kCurrent]	= AnimTimeKindData();
	m_time_kind_data[BodyKind::kLowerBody][TimeKind::kPrev]		= AnimTimeKindData();
	m_time_kind_data[BodyKind::kLowerBody][TimeKind::kCurrent]	= AnimTimeKindData();

	SetupStaticFrame();
	DivideFrame();
}

Animator::~Animator()
{
	for (auto& j_data : anim_data)
	{
		if (j_data.second.anim_handle != -1)
		{
			MV1DeleteModel(j_data.second.anim_handle);
			j_data.second.anim_handle = -1;
		}
	}

	m_resource_modeler.erase(BodyKind::kLowerBody);
	m_resource_modeler.erase(BodyKind::kUpperBody);
}

void Animator::Update()
{
	BlendAnim();
	PlayAnim();
	DivideFrame();
}

void Animator::AddAnimHandle(const int kind, const std::string& file_path, const int index, const std::string& tag, const float play_speed, const bool is_loop, const bool is_self_blend, const float landing_paly_rate)
{
	// 上書き不可
	if (anim_data.contains(kind)) { return; }

	int anim_handle = HandleKeeper::GetInstance()->LoadHandle(HandleKind::kAnim, file_path);
	if (anim_handle != -1)
	{
		anim_data[kind] = AnimKindData(anim_handle, file_path, index, tag, play_speed, is_loop, is_self_blend, landing_paly_rate);
	}
}

void Animator::AttachAnimEightDir(
	const int  forward_anim_kind,
	const bool is_move_forward,
	const bool is_move_backward,
	const bool is_move_left,
	const bool is_move_right,
	const bool is_result_attach)
{
	auto offset = 0;

	// forwardのアニメーションを基準とし、enum classの値をずらしてアタッチ
	// WARNING : enum依存なため順番の入れ替えには注意

	// 左前方
	if (is_move_forward && is_move_left)
	{
		offset = 4;
	}
	// 右前方
	else if (is_move_forward && is_move_right)
	{
		offset = 5;
	}
	// 左後方
	else if (is_move_backward && is_move_left)
	{
		offset = 6;
	}
	// 右後方
	else if (is_move_backward && is_move_right)
	{
		offset = 7;
	}
	// 前方
	else if (is_move_forward)
	{
		offset = 0;
	}
	// 後方
	else if (is_move_backward)
	{
		offset = 1;
	}
	// 左
	else if (is_move_left)
	{
		offset = 2;
	}
	// 右
	else if (is_move_right)
	{
		offset = 3;
	}

	if (is_result_attach)
	{
		AttachResultAnim(forward_anim_kind + offset);
	}
	else
	{
		AttachAnim(forward_anim_kind + offset, BodyKind::kLowerBody);
	}
}

void Animator::AttachAnim(const int next_kind, const BodyKind body_kind)
{
	if (!CanAttachAnim(next_kind, body_kind)) { return; }

	auto& body_map = m_time_kind_data.at(body_kind);

	auto& prev_time_data	= body_map.at(TimeKind::kPrev);
	auto& current_time_data	= body_map.at(TimeKind::kCurrent);

	// Current ➡ Prev
	prev_time_data = current_time_data;

	// Next ➡ Current
	current_time_data.kind = next_kind;
	current_time_data.attach_index = MV1AttachAnim(
			m_resource_modeler.at(body_kind)->GetModelHandle(),
			anim_data.at(next_kind).index,
			anim_data.at(next_kind).anim_handle,
			TRUE);
	current_time_data.total_time = MV1GetAttachAnimTotalTime(m_resource_modeler.at(body_kind)->GetModelHandle(),current_time_data.attach_index);
	SetPlayStartTime(&current_time_data, prev_time_data, body_kind);

	// 前回のアニメーションが存在しない場合は、ブレンド済み(ブレンド率100%)とする
	m_blend_rate.at(body_kind) = prev_time_data.attach_index > -1 ? 0.0f : 1.0f;
}

void Animator::AttachResultAnim(const int next_kind)
{
	if (!CanResultAttachAnim()) { return; }

	AttachAnim(next_kind, BodyKind::kUpperBody);
	AttachAnim(next_kind, BodyKind::kLowerBody);

	// 下半身の再生率を引き継ぐ
	const auto lower_play_timer = m_time_kind_data.at(BodyKind::kLowerBody).at(TimeKind::kCurrent).play_timer;
	m_time_kind_data.at(BodyKind::kUpperBody).at(TimeKind::kCurrent).play_timer = lower_play_timer;
}

void Animator::DetachAnim(const TimeKind time_kind, const BodyKind body_kind)
{
	auto& data = m_time_kind_data.at(body_kind).at(time_kind);

	if (data.attach_index > -1)
	{
		MV1DetachAnim(m_resource_modeler.at(body_kind)->GetModelHandle(), data.attach_index);
		data.attach_index = -1;
	}
}


#pragma region Getter
int Animator::GetAnimKind(const BodyKind body_kind, const TimeKind time_kind) const
{
	if (!m_time_kind_data.contains(body_kind))					{ return -1; }
	if (!m_time_kind_data.at(body_kind).contains(time_kind))	{ return -1; }
	
	return m_time_kind_data.at(body_kind).at(time_kind).kind;
}

std::string Animator::GetAnimTag(const BodyKind body_kind, const TimeKind time_kind) const
{
	const auto& data = m_time_kind_data.at(body_kind).at(time_kind);

	if (time_kind == TimeKind::kPrev)
	{
		return anim_data.contains(data.kind) ? anim_data.at(data.kind).tag : GetAnimTag(body_kind, TimeKind::kCurrent);
	}

	return anim_data.contains(data.kind) ? anim_data.at(data.kind).tag : "";
}

float Animator::GetGroundPlayRate(const BodyKind body_kind) const
{
	return anim_data.at(m_time_kind_data.at(body_kind).at(TimeKind::kCurrent).kind).ground_play_rate;
}

float Animator::GetPlayRate(const BodyKind body_kind) const
{
	const auto& data = m_time_kind_data.at(body_kind).at(TimeKind::kCurrent);

	return data.play_timer / data.total_time;
}

bool Animator::IsPlayEnd(const BodyKind body_kind) const
{
	const auto& data = m_time_kind_data.at(body_kind).at(TimeKind::kCurrent);

	return data.play_timer == data.total_time;
}
#pragma endregion


void Animator::PlayAnim()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(time_scale_layer_kind);

	for (auto& [body_kind, time_map] : m_time_kind_data)
	{
		for (auto& [time_kind, j_data] : time_map)
		{
			if (j_data.attach_index < 0) continue;

			const auto  blend_rate	= time_kind == TimeKind::kCurrent ? m_blend_rate.at(body_kind) : 1.0f - m_blend_rate.at(body_kind);
			const auto& anim		= anim_data.at(j_data.kind);
			const auto  is_loop		= anim.is_loop && !anim.is_self_blend;
			const auto  play_speed	= anim.play_speed * delta_time;

			math::Increase(j_data.play_timer, play_speed,j_data.total_time, is_loop);

			MV1SetAttachAnimTime(m_resource_modeler.at(body_kind)->GetModelHandle(), j_data.attach_index, j_data.play_timer);
			MV1SetAttachAnimBlendRate(m_resource_modeler.at(body_kind)->GetModelHandle(), j_data.attach_index, blend_rate);
		}
	}

	CombineAnim();
	NotifyOnGround();
}

void Animator::BlendAnim()
{
	const auto delta_time = GameTimeManager::GetInstance()->GetDeltaTime(time_scale_layer_kind);

	// ブレンド率100%まで増加させる
	math::Increase(m_blend_rate.at(BodyKind::kUpperBody), blend_speed * delta_time, 1.0f, false);
	math::Increase(m_blend_rate.at(BodyKind::kLowerBody), blend_speed * delta_time, 1.0f, false);

	// ブレンドが完了した場合、PravAnimは不要なためデタッチする
	if (m_blend_rate.at(BodyKind::kUpperBody) == 1.0f) { DetachAnim(TimeKind::kPrev, BodyKind::kUpperBody); }
	if (m_blend_rate.at(BodyKind::kLowerBody) == 1.0f) { DetachAnim(TimeKind::kPrev, BodyKind::kLowerBody); }
}

void Animator::SetupStaticFrame()
{
	const int model_handle = m_result_modeler->GetModelHandle();
	std::unordered_map<std::string, int> lower_body_frame_num;
	std::unordered_map<std::string, int> upper_body_frame_num;

	// 下半身のボーンを設定
	const auto begin_lower_body_frame_num = MV1SearchFrame(model_handle, FramePath.LEFT_UP_LEG);
	const auto   end_lower_body_frame_num = MV1SearchFrame(model_handle, FramePath.RIGHT_TOE_END);
	for (int i = begin_lower_body_frame_num; i <= end_lower_body_frame_num; ++i)
	{
		lower_body_frame_num[MV1GetFrameName(model_handle, i)] = i;
	}
	m_frame_numbers[BodyKind::kLowerBody] = lower_body_frame_num;

	// 上半身のボーンを設定
	const auto begin_upper_body_frame_num	= MV1SearchFrame(model_handle, FramePath.LEFT_SHOULDER);
	const auto end_upper_body_frame_num		= MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_PINKY_4);
	for (int i = begin_upper_body_frame_num; i <= end_upper_body_frame_num; ++i)
	{
		upper_body_frame_num[MV1GetFrameName(model_handle, i)] = i;
	}
	m_frame_numbers[BodyKind::kUpperBody] = upper_body_frame_num;
}

void Animator::DivideFrame(const TCHAR* upper_body_end_frame)
{
	const int model_handle = m_result_modeler->GetModelHandle();

	// 下半身のボーンを設定
	const auto begin_lower_body_frame_index = MV1SearchFrame(model_handle, FramePath.HIPS);
	const auto end_lower_body_frame_index	= MV1SearchFrame(model_handle, upper_body_end_frame) - 1;
	for (int i = begin_lower_body_frame_index; i <= end_lower_body_frame_index; ++i)
	{
		m_frame_numbers[BodyKind::kLowerBody][MV1GetFrameName(model_handle, i)] = i;
		m_frame_numbers[BodyKind::kUpperBody].erase(MV1GetFrameName(model_handle, i));
	}

	// 上半身のボーンを設定
	const auto begin_upper_body_frame_index = MV1SearchFrame(model_handle, upper_body_end_frame);
	const auto end_upper_body_frame_index	= MV1SearchFrame(model_handle, FramePath.HEAD_TOP_END_END);
	for (int i = begin_upper_body_frame_index; i <= end_upper_body_frame_index; ++i)
	{
		m_frame_numbers[BodyKind::kUpperBody][MV1GetFrameName(model_handle, i)] = i;
	}
}

void Animator::SetPlayStartTime(AnimTimeKindData* current_time_kind_data, const AnimTimeKindData& prev_time_kind_data, const BodyKind body_kind)
{
	if (anim_data.contains(prev_time_kind_data.kind))
	{
		const std::string prev_tag		= anim_data.at(prev_time_kind_data.kind).tag;
		const std::string current_tag	= anim_data.at(current_time_kind_data->kind).tag;

		if (anim_data.at(current_time_kind_data->kind).is_self_blend)
		{
			current_time_kind_data->play_timer = 0.0f;
			return;
		}

		// 同類アニメーションであった場合は再生率を引き継ぐ
		if (prev_tag == current_tag && prev_tag != AnimTag.NONE && current_tag != AnimTag.NONE)
		{
			m_prev_anim_play_rate[body_kind] = prev_time_kind_data.play_timer / prev_time_kind_data.total_time;
			current_time_kind_data->play_timer = current_time_kind_data->total_time * m_prev_anim_play_rate[body_kind];
			return;
		}
	}

	current_time_kind_data->play_timer = 0.0f;
}

void Animator::CombineAnim()
{
	std::vector<std::tuple<BodyKind, int, MATRIX>> matrix;

	// 各ボーンの行列を取得
	for (const auto& [body_kind, frame_numbers] : m_frame_numbers)
	{
		for (const auto& [name, frame_num] : frame_numbers)
		{
			const auto m = MV1GetFrameLocalMatrix(m_resource_modeler.at(body_kind)->GetModelHandle(), frame_num);
			matrix.emplace_back(body_kind, frame_num, m);
		}
	}

	// リザルトモデルに適用
	for (const auto& [body_kind, frame_num, mat] : matrix)
	{
		MV1SetFrameUserLocalMatrix(m_result_modeler->GetModelHandle(), frame_num, mat);
	}
}

void Animator::NotifyOnGround()
{
	const auto tag = GetAnimTag(BodyKind::kLowerBody, TimeKind::kCurrent);
	if (tag != AnimTag.WALK && tag != AnimTag.RUN) { return; }

	const auto play_rate	= GetPlayRate(BodyKind::kLowerBody);
	const auto is_left		= play_rate >= 0.5f - notify_ground_range * 0.5f && play_rate < 0.5f + notify_ground_range * 0.5f;
	const auto is_right		= play_rate <= notify_ground_range * 0.5f		 || play_rate > 1.0f - notify_ground_range * 0.5f;

	if (is_left  && !m_prev_left_on_ground)	 { NotifyFoot(true,  tag); }
	if (is_right && !m_prev_right_on_ground) { NotifyFoot(false, tag); }

	m_prev_left_on_ground	= is_left;
	m_prev_right_on_ground	= is_right;
}

void Animator::NotifyFoot(bool is_left, const std::string& tag)
{
	const auto is_run		= tag == AnimTag.RUN;
	const auto model_handle = m_result_modeler->GetModelHandle();
	const auto frame		= is_left ? FramePath.LEFT_FOOT : FramePath.RIGHT_FOOT;
	const auto mat			= MV1GetFrameLocalWorldMatrix(model_handle, MV1SearchFrame(model_handle, frame));

	EventSystem::GetInstance()->Publish(OnGroundFootEvent(is_run, is_left, matrix::GetPos(mat), time_scale_layer_kind));
}

bool Animator::CanResultAttachAnim()
{
	// どちらかのブレンド率が1.0に到達していない場合はアタッチを許可しない
	if (m_blend_rate.at(BodyKind::kUpperBody) != 1.0f || m_blend_rate.at(BodyKind::kLowerBody) != 1.0f)
	{
		return false;
	}

	return true;
}

bool Animator::CanAttachAnim(const int next_kind, const BodyKind body_kind)
{
	if (!anim_data.contains(next_kind))		{ return false; }
	if (m_blend_rate.at(body_kind) != 1.0f) { return false; }

	const auto& current = m_time_kind_data.at(body_kind).at(TimeKind::kCurrent);

	if (current.kind == next_kind)
	{
		if (!(anim_data.at(next_kind).is_self_blend && IsPlayEnd(body_kind)))
		{
			return false;
		}
	}

	return true;
}
