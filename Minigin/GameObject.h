#pragma once

#include "Scene.h"
#include "Transform.h"

namespace dae {


	class GameObject final{

	public:
		template<typename ... ComponentTypes>
		GameObject(Scene& scene, const TransformData& transformData, ComponentTypes... components) :
			m_Registry(&scene.Registry())
		{
			CTransform transform{ *m_Registry };
			m_Registry->CreateEntity(m_Entity, transform, components...);

			CTransform& transformRef = GetTransform(); 
			transformRef.Initialize(m_Entity);
			transformRef.SetLocalTransform(transformData);
		}
		~GameObject() {

		}

		bool SetParent(const GameObject& other, ETransformReparentType transformReparentType = ETransformReparentType::KeepWorldTransform);
		void MakeRoot();

		const CTransform& GetTransform()const;
		CTransform& GetTransform();

		bool IsActivated()const;
		void Deactivate();
		void Activate();
		const Pillar::UEntityHandle& GetEntityHandle()const;
		template<typename ComponentType>
		ComponentType* const GetComponent();


	private:
		Pillar::UEntityHandle m_Entity; 
		Pillar::URegistry* m_Registry;
	};
	template<typename ComponentType>
	inline ComponentType* const GameObject::GetComponent()
	{
		return m_Registry->GetComponent<ComponentType>(m_Entity.GetConstEntity());
	}
}