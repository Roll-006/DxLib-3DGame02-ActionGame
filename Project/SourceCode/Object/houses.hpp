#pragma once
#include "house.hpp"

class Houses final
{
public:
	Houses();
	~Houses();

	void Init();
	void Update();
	void LateUpdate();
	void Draw() const;

private:
	std::vector<std::shared_ptr<House>> m_houses;
};
