#include "handle_keeper.hpp"

HandleKeeper::HandleKeeper()
{

}

HandleKeeper::~HandleKeeper()
{

}

int HandleKeeper::LoadHandle(const HandleKind handle_kind, const std::string& file_path, const float scale)
{
	// 既にロード済みの場合は早急にハンドルを返す
	for (auto& [kind, path, handle] : m_handles)
	{
		if (kind == handle_kind && path == file_path) { return handle; }
	}

	int handle = -1;

	// 種類別にロード
	switch (handle_kind)
	{
	case HandleKind::kGraphic:
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
	return handle;
}

void HandleKeeper::DeleteHandle(const HandleKind handle_kind, const std::string& file_path)
{
	for(auto itr = m_handles.begin(); itr != m_handles.end(); ++itr)
	{
		// 一致するものが見つかれば削除
		if (std::get<0>(*itr) == handle_kind && std::get<1>(*itr) == file_path)
		{
			m_handles.erase(itr);
		}
	}
}

void HandleKeeper::DeleteHandle(const HandleKind handle_kind, const int handle)
{
	for (auto itr = m_handles.begin(); itr != m_handles.end(); ++itr)
	{
		// 一致するものが見つかれば削除
		if (std::get<0>(*itr) == handle_kind && std::get<2>(*itr) == handle)
		{
			m_handles.erase(itr);
		}
	}
}

int HandleKeeper::GetHandle(const HandleKind handle_kind, const std::string& file_path)
{
	for (auto& [kind, path, handle] : m_handles)
	{
		if (kind == handle_kind && path == file_path) { return handle; }
	}
}
