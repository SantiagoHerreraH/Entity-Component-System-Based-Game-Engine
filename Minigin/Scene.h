#pragma once
#include "SceneManager.h"
#include "SystemInterfaces.h"

namespace dae
{

	class Scene final
	{
		friend Scene& SceneManager::AddScene(const std::string& name);
	public:
		
		Pillar::URegistry& Registry();
		
		/*
			ORDER OF ADDITION IS ORDER OF EXECUTION
		*/
		void AddStartSystem(const std::shared_ptr<IStartSystem>& system);
		void AddUpdateSystem(const std::shared_ptr<IUpdateSystem>& system);
		void AddFixedUpdateSystem(const std::shared_ptr<IFixedUpdateSystem>& system);
		void AddRenderingSystem(const std::shared_ptr<IRenderSystem>& system);


		bool RemoveStartSystem(const std::shared_ptr<IStartSystem>& system);
		bool RemoveUpdateSystem(const std::shared_ptr<IUpdateSystem>& system);
		bool RemoveFixedUpdateSystem(const std::shared_ptr<IFixedUpdateSystem>& system);
		bool RemoveRenderingSystem(const std::shared_ptr<IRenderSystem>& system);
		void RemoveAll();

		void Start();
		void FixedUpdate();
		void Update();
		void Render() const;

		const std::string& Name() const { return m_Name; }

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		Pillar::URegistry m_Registry{};

		std::string m_Name;
		std::vector < std::shared_ptr<IStartSystem>> m_StartSystems{};
		std::vector < std::shared_ptr<IUpdateSystem>> m_UpdateSystems{};
		std::vector < std::shared_ptr<IFixedUpdateSystem>> m_FixedUpdateSystems{};
		std::vector < std::shared_ptr<IRenderSystem>> m_RenderSystems{};

		static unsigned int m_idCounter; 
	};

}
