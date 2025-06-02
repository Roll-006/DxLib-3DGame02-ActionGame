#include "handle_keeper.hpp"

HandleKeeper::HandleKeeper()
{

}

HandleKeeper::~HandleKeeper()
{

}

void HandleKeeper::AddHandle(const HandleKind handle_kind, const std::string& file_path, const float scale)
{
	// 既にロード済みの場合は早期return
	for (auto& [kind, path, handle] : m_handles)
	{
		if (kind == handle_kind && path == file_path) { return; }
	}

	int handle = -1;

	// 種類別にロード
	switch (handle_kind)
	{
	case HandleKind::kImage:
		handle = LoadGraph(file_path.c_str(), TRUE);
		break;

	case HandleKind::kModel:
	case HandleKind::kAnim:
		handle = MV1LoadModel(file_path.c_str());
		break;

	case HandleKind::kEffect:
		handle = LoadEffekseerEffect(file_path.c_str(), scale);
		break;
	}

	m_handles.emplace_back(std::make_tuple(handle_kind, file_path, handle));
}

void HandleKeeper::RemoveHandle(const HandleKind handle_kind, const std::string& file_path)
{
	// 削除する入力コードを検索
	const auto remove = std::find_if(m_handles.begin(), m_handles.end(), [=](const std::tuple<HandleKind, std::string, int> h)
	{
		return std::get<0>(h) == handle_kind && h.second.kind == code.kind && p.second.code == code.code;
	});

	// 一致する入力コードを削除
	if (remove != codes->end())
	{
		codes->erase(remove);
	}
}

void HandleKeeper::RemoveHandle(const HandleKind handle_kind, const int handle)
{

}

int HandleKeeper::GetHandle(const HandleKind handle_kind, const std::string& file_path)
{
	for (auto& [kind, path, handle] : m_handles)
	{
		if (kind == handle_kind && path == file_path) { return handle; }
	}
}
