#include "player_command.hpp"

void player_cmd::MoveUp::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->MoveForward();
}

void player_cmd::MoveDown::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->MoveBackward();
}

void player_cmd::MoveLeft::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->MoveLeft();
}

void player_cmd::MoveRight::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->MoveRight();
}

void player_cmd::Run::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->Run();
}

void player_cmd::Squat::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->Squat();
}

void player_cmd::Shot::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->Shot();
}

void player_cmd::Aiming::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->AimingGun();
}

void player_cmd::Reload::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->Reload();
}

void player_cmd::SilentKill::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj);
}

void player_cmd::TurnAround::Execute(ObjBase* obj)
{
	static_cast<Player*>(obj)->TurnAround();
}
