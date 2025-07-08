#pragma once
#include "../Base/singleton_base.hpp"

class CameraManager final : public SingletonBase<CameraManager>
{
public:


private:
	CameraManager();
	~CameraManager() override;
	
private:
	friend SingletonBase<CameraManager>;
};