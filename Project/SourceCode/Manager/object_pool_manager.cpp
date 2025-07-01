#include "object_pool_manager.hpp"

ObjectPoolManager::ObjectPoolManager()
{
	// ˆ—‚È‚µ
}

ObjectPoolManager::~ObjectPoolManager()
{
	// ˆ—‚È‚µ
}

void ObjectPoolManager::RemoveObjectPool(const std::string& object_pool_name)
{
	m_object_pools.erase(object_pool_name);
}

std::shared_ptr<ObjectPoolBase> ObjectPoolManager::GetObjectPool(const std::string& object_pool_name)
{
	return m_object_pools.count(object_pool_name) ? m_object_pools.at(object_pool_name) : nullptr;
}
