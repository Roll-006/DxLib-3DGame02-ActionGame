#pragma once
#include "graphicer.hpp"
#include "../Data/Path/ui_graphic_path.hpp"
#include "../Shape/2DShape/square.hpp"

class WeaponShortcutIcon final
{
public:
	WeaponShortcutIcon(const Vector2D<int> center_pos, const int width, const int height);
	~WeaponShortcutIcon();

	void Draw() const;

	void AttachGraphic(std::shared_ptr<Graphicer> graphicer);
	void DetachGraphic()
	{
		m_graphicer = nullptr;
	}

private:
	Vector2D<int>				m_center_pos;
	int							m_icon_width;
	int							m_icon_height;
	std::shared_ptr<Graphicer>	m_graphicer;
};
