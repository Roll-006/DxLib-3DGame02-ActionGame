#include "houses.hpp"

Houses::Houses()
{
	for (size_t i = 0; i < 1; ++i)
	{
		m_houses.emplace_back(std::make_shared<House>(std::to_string(i)));
	}

	for (const auto& house : m_houses)
	{
		house->AddToObjManager();
	}
}

Houses::~Houses()
{
	for (const auto& house : m_houses)
	{
		house->RemoveToObjManager();
	}
}

void Houses::Init()
{
	for (const auto& house : m_houses)
	{
		house->Init();
	}
}

void Houses::Update()
{
	for (const auto& house : m_houses)
	{
		house->Update();
	}
}

void Houses::LateUpdate()
{
	for (const auto& house : m_houses)
	{
		house->LateUpdate();
	}
}

void Houses::Draw() const
{
	for (const auto& house : m_houses)
	{
		house->Draw();
	}
}
