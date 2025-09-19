#include "GameObject.h"
#include <queue>

bool dae::GameObject::SetParent(const GameObject& other, ETransformReparentType transformReparentType)
{
	return GetTransform().SetParent(other.GetEntityHandle(), transformReparentType);
}

void dae::GameObject::MakeRoot()
{
	GetTransform().MakeRootNode();
}

const dae::CTransform& dae::GameObject::GetTransform() const
{
	return *m_Registry->GetComponent<CTransform>(m_Entity.GetConstEntity());
}

dae::CTransform& dae::GameObject::GetTransform()
{
	return *m_Registry->GetComponent<CTransform>(m_Entity.GetConstEntity());
}

bool dae::GameObject::IsActivated() const
{
	return m_Registry->IsActivated(m_Entity.GetConstEntity());
}

void dae::GameObject::Deactivate()
{
	m_Registry->DeactivateEntity(m_Entity.GetConstEntity());
}

void dae::GameObject::Activate()
{
	m_Registry->ActivateEntity(m_Entity.GetConstEntity());
}

const Pillar::UEntityHandle& dae::GameObject::GetEntityHandle() const
{
	return m_Entity;
}
