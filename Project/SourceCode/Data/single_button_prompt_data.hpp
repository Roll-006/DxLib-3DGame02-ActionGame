#pragma once
#include "../Data/text_data.hpp"
#include "../Kind/command_kind.hpp"

struct SingleButtonPromptData
{
	TextData		text_data			= {};
	CommandKind		command_kind		= CommandKind::kDecide;
	CommandSlotKind command_slot_kind	= CommandSlotKind::kMain;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, SingleButtonPromptData& single_button_prompt_data)
{
	j_data.at("text_data")			.get_to(single_button_prompt_data.text_data);
	j_data.at("command_kind")		.get_to(single_button_prompt_data.command_kind);
	j_data.at("command_slot_kind")	.get_to(single_button_prompt_data.command_slot_kind);
}

inline void to_json(nlohmann::json& j_data, const SingleButtonPromptData& single_button_prompt_data)
{
	j_data = nlohmann::json
	{
		{ "text_data",			single_button_prompt_data.text_data },
		{ "command_kind",		single_button_prompt_data.command_kind },
		{ "command_slot_kind",	single_button_prompt_data.command_slot_kind },
	};
}
#pragma endregion
