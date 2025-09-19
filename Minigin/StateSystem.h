#pragma once

#include <string>
#include "PillarLibrary_02/include/InternalIncludes.h"

namespace dae {

#pragma region State

	class IState {
	public:
		virtual ~IState() {};
		virtual bool AssignState(const std::string& stateName) = 0;
		virtual void RemoveState(const std::string& stateName) = 0;
	};

	template <typename ComponentType>
	class State final : public IState {

	public:
		State(const Pillar::UEntityHandle& entityHandle, Pillar::URegistry& registry);
		virtual bool AssignState(const std::string& stateName) override;
		void AddState(const std::string& stateName, const ComponentType& state, bool* outChangedPrevious = nullptr);
		virtual void RemoveState(const std::string& stateName)override;

	private:
		std::unordered_map<std::string, ComponentType> m_States;
		Pillar::UEntityHandle m_EntityHandle;
		Pillar::URegistry* m_Registry;
	};

	template<typename ComponentType>
	inline State<ComponentType>::State(const Pillar::UEntityHandle& entityHandle, Pillar::URegistry& registry) :
		m_EntityHandle(entityHandle),
		m_Registry(&registry){}

	template<typename ComponentType>
	inline bool State<ComponentType>::AssignState(const std::string& stateName)
	{
		if (m_States.contains(stateName))
		{
			return m_Registry->SetComponent<ComponentType>(m_States.at(stateName), m_EntityHandle);
		}

		return false;
	}

	template<typename ComponentType>
	inline void State<ComponentType>::AddState(const std::string& stateName, const ComponentType& state, bool* outChangedPrevious)
	{
		if (m_States.contains(stateName))
		{
			m_States[stateName] = state;

			if (outChangedPrevious)
			{
				*outChangedPrevious = true;
			}
		}
		else
		{
			m_States.emplace(std::pair(stateName, state));
		}
	}

	template<typename ComponentType>
	inline void State<ComponentType>::RemoveState(const std::string& stateName)
	{
		m_States.erase(stateName);
	}

#pragma endregion

#pragma region StateController

	class StateController final{

	public:
		StateController(const Pillar::UEntityHandle& entityHandle, Pillar::URegistry& registry);
		void AssignState(const std::string& stateName);

		template <typename ComponentType>
		void AddState(const std::string& stateName, const ComponentType& state, bool* outChangedPrevious = nullptr);

		void RemoveState(const std::string& stateName);

	private:
		std::unordered_set<std::type_index> m_StateTypes;
		std::vector<std::shared_ptr<IState>> m_States;
		Pillar::UEntityHandle m_EntityHandle;
		Pillar::URegistry* m_Registry;
		std::string m_CurrentStateName;

	};
	
	template<typename ComponentType>
	inline void StateController::AddState(const std::string& stateName, const ComponentType& state, bool* outChangedPrevious)
	{
		std::type_index currentType = typeid(ComponentType);

		if (!m_StateTypes.contains(currentType))
		{
			m_StateTypes.emplace(currentType);
			m_States.push_back(std::make_shared<State<ComponentType>>(State<ComponentType>{m_EntityHandle, * m_Registry}));
		}

		State<ComponentType>* currentState = (State<ComponentType>*)m_States.back().get();
		currentState->AddState(stateName, state, outChangedPrevious);
	}

#pragma endregion

}