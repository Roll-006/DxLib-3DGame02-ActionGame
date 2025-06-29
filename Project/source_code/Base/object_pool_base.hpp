#pragma once
#include <queue>
#include "obj_base.hpp"

class ObjectPoolBase abstract
{
public:
	virtual ~ObjectPoolBase() = default;

	/// @brief 使用後のオブジェクトを返却する
	void ReturnObj(const std::shared_ptr<ObjBase> obj);

	/// @brief オブジェクトをプール内から破棄する
	/// @brief オブジェクト名を指定し、一致するオブジェクトをすべて破棄
	void DestroyObj(const std::string& obj_name);

	/// @brief 使用可能なオブジェクトを取得する
	/// @return 初期化・アクティブ化されたオブジェクト(使用可能なオブジェクトがない場合はnullptr)
	[[nodiscard]] std::shared_ptr<ObjBase> GetObj(const std::string& obj_name);

	[[nodiscard]] int GetPoolSize(const std::string& obj_name) const;

protected:
	/// @brief オブジェクトを生成
	/// @brief コンストラクタ時に呼び出される
	void CreateObj(const std::shared_ptr<ObjBase> obj);

protected:
	std::unordered_map<std::string, int> m_pool_size;
	std::unordered_map<std::string, std::queue<std::shared_ptr<ObjBase>>> m_objects;
};
