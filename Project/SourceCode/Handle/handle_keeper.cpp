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
	int handle = ReloadHandle(handle_kind, file_path);
	if (handle > -1) { return handle; };

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

int HandleKeeper::ReloadHnadle(const HandleKind handle_kind, const int handle)
{
	int result_handle = -1;

	for (const auto& [kind, path, hdl] : m_handles)
	{
		switch (handle_kind)
		{
		case HandleKind::kGraphic:
			if (hdl == handle) { result_handle = handle; }
			break;

		case HandleKind::kModel:
		case HandleKind::kAnim:
			if (hdl == handle) { result_handle = MV1DuplicateModel(handle); }
			break;

		case HandleKind::kEffect:
			if (hdl == handle) { result_handle = handle; }
			break;
		}
	}

	return result_handle;
}

int HandleKeeper::ReloadHandle(const HandleKind handle_kind, const std::string& file_path)
{
	// FIXME : おそらくエラーの原因。モデルおよび画像の消失

	for (auto& [kind, path, handle] : m_handles)
	{
		switch (handle_kind)
		{
		case HandleKind::kGraphic:
			if (path == file_path) { return handle; }
			break;

		case HandleKind::kModel:
		case HandleKind::kAnim:
			if (path == file_path) { return MV1DuplicateModel(handle); }
			break;

		case HandleKind::kEffect:
			if (path == file_path) { return handle; }
			break;
		}
	}

	return -1;
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
