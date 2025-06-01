#include "player_command.hpp"

void player_cmd::MoveUp::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::MoveDown::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::MoveLeft::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::MoveRight::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::Run::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::Squat::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::Shot::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::Ready::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::Reload::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::SilentKill::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}

void player_cmd::QuickTurn::Execute(ObjBase& obj)
{
	dynamic_cast<Player*>(&obj);
}
