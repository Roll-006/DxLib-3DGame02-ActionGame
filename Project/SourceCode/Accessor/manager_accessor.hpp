//#pragma once
//#include <typeindex>
//#include <unordered_map>
//#include "../Interface/i_singleton.hpp"
//
//class ManagerAccessor final
//{
//public:
//	ManagerAccessor();
//	~ManagerAccessor();
//
//	template<typename T>
//    [[nodiscard]] std::shared_ptr<const T> GetManager() const
//    {
//        const auto type = std::type_index(typeid(T));
//
//        return m_singletons.contains(type) ? std::static_pointer_cast<const T>(m_singletons.at(type)) : nullptr;
//    }
//
//private:
//	std::unordered_map<std::type_index, std::shared_ptr<ISingleton>> m_singletons;
//};
