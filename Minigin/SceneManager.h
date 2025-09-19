#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "EventSystem.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
		enum class SceneTraversalType {
			Loop,
			Clamp
		};

		struct SceneData {
			std::shared_ptr<Scene> Scene;
			Event<dae::Scene*> OnSceneChanged; //output new scene
		};

	public:
		Scene& AddScene(const std::string& name);
		Scene* ChangeCurrentScene(const std::string& name);
		Scene* GetScene(const std::string& name);
		Event<dae::Scene*>* GetOnSceneChangedEvent(const std::string& name);
		Scene& NextScene(SceneTraversalType sceneTraversalType = SceneTraversalType::Loop);
		Scene& PreviousScene(SceneTraversalType sceneTraversalType = SceneTraversalType::Loop);

		void Start();
		void FixedUpdate();
		void Update();
		void Render()const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<SceneData> m_Scenes;
		int m_CurrentSceneIndex = 0;
	};
}
