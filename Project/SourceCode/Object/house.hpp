#pragma once
#include "../Base/physical_obj_base.hpp"

#include "../Part/modeler.hpp"
#include "../Json/json_loader.hpp"

#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"
#include "../Manager/physics_manager.hpp"

class House final : public PhysicalObjBase
{
public:
	House(const std::string& house_id);
	~House() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Modeler> GetModeler() const { return m_modeler; }

private:
	std::shared_ptr<Modeler>	m_modeler;
	std::shared_ptr<Modeler>	m_collision_modeler;

	std::string					m_model_path;
	std::string					m_collision_model_path;
	VECTOR						m_pos;
	VECTOR						m_angle;
	float						m_basic_scale;
	VECTOR						m_collision_area_offset;
	float						m_collision_area_radius;

	friend void from_json(const nlohmann::json& data, House& house);
	friend void to_json  (nlohmann::json& data, const House& house);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, House& house)
{
	from_json(data, static_cast<PhysicalObjBase&>(house));

	data.at("model_path")			.get_to(house.m_model_path);
	data.at("collision_model_path")	.get_to(house.m_collision_model_path);
	data.at("position")				.get_to(house.m_pos);
	data.at("angle")				.get_to(house.m_angle);
	data.at("basic_scale")			.get_to(house.m_basic_scale);
	data.at("collision_area_offset").get_to(house.m_collision_area_offset);
	data.at("collision_area_radius").get_to(house.m_collision_area_radius);
}

inline void to_json(nlohmann::json& data, const House& house)
{
	nlohmann::json base_json;
	to_json(base_json, static_cast<const PhysicalObjBase&>(house));

	nlohmann::json derived_json =
	{
		{ "model_path",				house.m_model_path },
		{ "collision_model_path",	house.m_collision_model_path },
		{ "position",				house.m_pos },
		{ "angle",					house.m_angle },
		{ "basic_scale",			house.m_basic_scale },
		{ "collision_area_offset",	house.m_collision_area_offset },
		{ "collision_area_radius",	house.m_collision_area_radius }
	};

	data = base_json;
	data.update(derived_json);
}
#pragma endregion
