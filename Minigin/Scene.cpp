#include "Scene.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

Pillar::URegistry& dae::Scene::Registry()
{
	return m_Registry;
}

void dae::Scene::AddStartSystem(const std::shared_ptr<IStartSystem>& system)
{
	m_StartSystems.emplace_back(system);
}

void dae::Scene::AddUpdateSystem(const std::shared_ptr<IUpdateSystem>& system)
{
	m_UpdateSystems.emplace_back(system);
}

void dae::Scene::AddFixedUpdateSystem(const std::shared_ptr<IFixedUpdateSystem>& system)
{
	m_FixedUpdateSystems.emplace_back(system);
}

void dae::Scene::AddRenderingSystem(const std::shared_ptr<IRenderSystem>& system)
{
	m_RenderSystems.emplace_back(system);
}

bool dae::Scene::RemoveStartSystem(const std::shared_ptr<IStartSystem>& system)
{
	bool removed{ false };

	for (int i = 0; i < m_StartSystems.size(); )
	{
		if (system == m_StartSystems[i])
		{
			m_StartSystems.erase(m_StartSystems.begin() + i);

			removed = true;
		}
		else
		{
			++i;
		}
	}

	return removed;
}

//not that common to be called, and not an overwhelmnig amount of systems, so for now, linearity doesn't matter
bool dae::Scene::RemoveUpdateSystem(const std::shared_ptr<IUpdateSystem>& system)
{
	bool removed{false};

	for (int i = 0; i < m_UpdateSystems.size(); )
	{
		if (system == m_UpdateSystems[i])
		{
			m_UpdateSystems.erase(m_UpdateSystems.begin() + i);

			removed = true;
		}
		else
		{
			++i;
		}
	}

	return removed;
}

bool dae::Scene::RemoveFixedUpdateSystem(const std::shared_ptr<IFixedUpdateSystem>& system)
{
	bool removed{ false };

	for (int i = 0; i < m_FixedUpdateSystems.size(); )
	{
		if (system == m_FixedUpdateSystems[i])
		{
			m_FixedUpdateSystems.erase(m_FixedUpdateSystems.begin() + i);

			removed = true;
		}
		else
		{
			++i;
		}
	}

	return removed;
}

bool dae::Scene::RemoveRenderingSystem(const std::shared_ptr<IRenderSystem>& system)
{
	bool removed{ false };

	for (int i = 0; i < m_RenderSystems.size(); )
	{
		if (system == m_RenderSystems[i])
		{
			m_RenderSystems.erase(m_RenderSystems.begin() + i);

			removed = true;
		}
		else
		{
			++i;
		}
	}

	return removed;
}

void dae::Scene::RemoveAll()
{
	m_StartSystems.clear();
	m_RenderSystems.clear();
	m_FixedUpdateSystems.clear();
	m_UpdateSystems.clear();
}

void dae::Scene::Start()
{
	for (auto& system : m_StartSystems)
	{
		system->Start();
	}
}

void dae::Scene::FixedUpdate()
{
	for (auto& system : m_FixedUpdateSystems)
	{
		system->FixedUpdate();
	}
}

void Scene::Update()
{
	for(auto& system: m_UpdateSystems)
	{
		system->Update();
	}
}

void Scene::Render() const
{
	for (const auto& system : m_RenderSystems)
	{
		system->Render();
	}
}

