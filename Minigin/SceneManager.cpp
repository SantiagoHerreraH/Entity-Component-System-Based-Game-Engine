#include "SceneManager.h"
#include "Scene.h"
#include <iostream>

void dae::SceneManager::Start()
{
	m_Scenes[m_CurrentSceneIndex].Scene->Start();
}

void dae::SceneManager::FixedUpdate()
{
	m_Scenes[m_CurrentSceneIndex].Scene->FixedUpdate();
}

void dae::SceneManager::Update()
{

	m_Scenes[m_CurrentSceneIndex].Scene->Update();
}

void dae::SceneManager::Render()const
{
	m_Scenes[m_CurrentSceneIndex].Scene->Render();
}

dae::Scene& dae::SceneManager::AddScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	SceneData data{};
	data.Scene = scene;
	m_Scenes.push_back(data);
	return *scene;
}

dae::Scene* dae::SceneManager::ChangeCurrentScene(const std::string& name)
{
	for (int i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i].Scene->Name() == name)
		{
			if (m_CurrentSceneIndex != i)
			{
				m_CurrentSceneIndex = i;
				m_Scenes[m_CurrentSceneIndex].OnSceneChanged.Invoke(m_Scenes[m_CurrentSceneIndex].Scene.get());
				m_Scenes[m_CurrentSceneIndex].Scene->Start();
			}

			return m_Scenes[i].Scene.get();
		}
	}
	return nullptr;
}

dae::Scene* dae::SceneManager::GetScene(const std::string& name)
{
	for (int i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i].Scene->Name() == name)
		{

			return m_Scenes[i].Scene.get();
		}
	}
	return nullptr;
}

dae::Event<dae::Scene*>* dae::SceneManager::GetOnSceneChangedEvent(const std::string& name)
{
	for (int i = 0; i < m_Scenes.size(); i++)
	{
		if (m_Scenes[i].Scene->Name() == name)
		{

			return &m_Scenes[i].OnSceneChanged;
		}
	}
	return nullptr;
}

dae::Scene& dae::SceneManager::NextScene(SceneTraversalType sceneTraversalType)
{
	switch (sceneTraversalType)
	{
	case SceneTraversalType::Clamp:

		if (m_CurrentSceneIndex == (m_Scenes.size() - 1))
		{
			return *m_Scenes[m_CurrentSceneIndex].Scene.get();
		}

		++m_CurrentSceneIndex;

		m_Scenes[m_CurrentSceneIndex].Scene->Start();

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();

		break;

	case SceneTraversalType::Loop:

		if (m_CurrentSceneIndex == (m_Scenes.size() - 1))
		{
			m_CurrentSceneIndex = 0;

		}
		else
		{
			++m_CurrentSceneIndex;
		}

		m_Scenes[m_CurrentSceneIndex].Scene->Start();

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();

		break;	
	
	default:

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();
		break;
	}
}

dae::Scene& dae::SceneManager::PreviousScene(SceneTraversalType sceneTraversalType)
{
	switch (sceneTraversalType)
	{
	case SceneTraversalType::Clamp:

		if (m_CurrentSceneIndex == 0)
		{
			return *m_Scenes[m_CurrentSceneIndex].Scene.get();
		}

		--m_CurrentSceneIndex;

		m_Scenes[m_CurrentSceneIndex].Scene->Start();

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();

		break;

	case SceneTraversalType::Loop:

		if (m_CurrentSceneIndex == 0)
		{
			m_CurrentSceneIndex = int(m_Scenes.size() - 1);

		}
		else
		{
			--m_CurrentSceneIndex;
		}

		m_Scenes[m_CurrentSceneIndex].Scene->Start();

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();

		break;

	default:

		return *m_Scenes[m_CurrentSceneIndex].Scene.get();
		break;
	}
}
