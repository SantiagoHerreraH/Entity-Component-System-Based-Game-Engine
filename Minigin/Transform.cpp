#include "Transform.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp> 
#include <queue>
#include "Renderer.h"

dae::CTransform::CTransform(Pillar::URegistry& registry) :
	m_Registry(&registry)
{
}

bool dae::CTransform::Initialize(const Pillar::UEntityHandle& self)
{
	if (!m_Self.IsValid())
	{
		m_Self = self;
		return true;
	}

	return false;
}

void dae::CTransform::MakeRootNode()
{
	if (m_Self.IsValid() && HasParent())
	{
		m_LocalTransform = GetWorldTransform();

		CTransform* pastParentTransform = m_Registry->GetComponent<CTransform>(m_Parent.GetConstEntity());
		pastParentTransform->m_Children.SwapRemove(m_Self);

		m_Parent = Pillar::UEntityHandle{};
		
	}
}

bool dae::CTransform::SetParent(const Pillar::UEntityHandle& parent, ETransformReparentType transformReparentType)
{
	if (m_Self.IsValid() && parent.IsValid() && m_Self != parent)
	{
		
		CTransform* newParentTransform = m_Registry->GetComponent<CTransform>(parent.GetConstEntity());

		if (!newParentTransform || IsDescendant(parent))
		{
			return false;
		}

		CTransform* pastParentTransform = m_Registry->GetComponent<CTransform>(m_Parent.GetConstEntity());
		if (pastParentTransform)
		{
			pastParentTransform->m_Children.SwapRemove(m_Self);
		}

		newParentTransform->m_Children.Add(m_Self);
		m_Parent = parent;		

		switch (transformReparentType)
		{
		case dae::ETransformReparentType::KeepWorldTransform:

			glm::mat4 parentWorldInverse = glm::inverse(newParentTransform->GetWorldMatrix());
			glm::mat4 newLocalMatrix = parentWorldInverse * GetWorldMatrix();
			m_LocalTransform = TransformDataFromMatrix(newLocalMatrix);

			break;
		case dae::ETransformReparentType::KeepLocalTransform:

			NeedsToRecalculateWorldTransform();
			break;
		default:
			break;
		}

		return true;
	}
	return false;
	
}

const Pillar::UEntityHandleContainer& dae::CTransform::GetChildren() const
{
	return m_Children;
}

bool dae::CTransform::IsDescendant(const Pillar::UEntityHandle& descendant)
{
	if (m_Self.IsValid() && descendant.IsValid())
	{
		CTransform* parentTransform = m_Registry->GetComponent<CTransform>(descendant.GetConstEntity());
		
		if (parentTransform)
		{
			while (parentTransform)
			{
				if (parentTransform->HasParent())
				{
					parentTransform = m_Registry->GetComponent<CTransform>(parentTransform->GetParent().GetConstEntity());

					if (parentTransform == this)
					{
						return true;
					}
				}
				else
				{
					break;
				}
				
			}
		}
		
	}
	return false;
}

bool dae::CTransform::HasParent() const
{
	return m_Parent.IsValid();
}

bool dae::CTransform::HasChildren() const
{
	return m_Children.NumOfEntities() > 0;
}

const Pillar::UEntityHandle& dae::CTransform::GetParent() const
{
	return m_Parent;
}

void dae::CTransform::SetLocalScaleX(float scale)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Scale.x = scale;
}

void dae::CTransform::SetLocalScaleY(float scale)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Scale.y = scale;
}

void dae::CTransform::SetLocalScaleZ(float scale)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Scale.z = scale;
}

void dae::CTransform::SetLocalScale(const glm::vec3& scale)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Scale = scale;
}

void dae::CTransform::SetLocalPositionX(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.x = pos;
}

void dae::CTransform::SetLocalPositionY(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.y = pos;
}

void dae::CTransform::SetLocalPositionZ(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.z = pos;
}

void dae::CTransform::SetLocalPosition(const glm::vec3& pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position = pos;
}

void dae::CTransform::MoveLocalPositionX(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.x += pos;
}

void dae::CTransform::MoveLocalPositionY(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.y += pos;
}

void dae::CTransform::MoveLocalPositionZ(float pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position.z += pos;
}

void dae::CTransform::MoveLocalPosition(const glm::vec3& pos)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Position += pos;
}

void dae::CTransform::SetLocalRotationX(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.x = rotation;
}

void dae::CTransform::SetLocalRotationY(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.y = rotation;
}

void dae::CTransform::SetLocalRotationZ(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.z = rotation;
}

void dae::CTransform::SetLocalRotation(const glm::vec3& rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation = rotation;
}

void dae::CTransform::MoveLocalRotationX(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.x += rotation;
}

void dae::CTransform::MoveLocalRotationY(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.y += rotation;
}

void dae::CTransform::MoveLocalRotationZ(float rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation.z += rotation;
}

void dae::CTransform::MoveLocalRotation(const glm::vec3& rotation)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform.Rotation += rotation;
}

void dae::CTransform::SetLocalTransform(const TransformData& transformData)
{
	NeedsToRecalculateWorldTransform();
	m_LocalTransform = transformData;
}

const dae::TransformData& dae::CTransform::GetWorldTransform() const
{
	CalculateWorldTransform();

	return m_WorldTransform;
}

const dae::TransformData& dae::CTransform::GetLocalTransform() const
{
	return m_LocalTransform;
}

const dae::TransformData& dae::CTransform::GetViewTransform(const glm::mat4& inverseCameraMatrix) const
{
	m_ViewTransform = TransformDataFromMatrix(inverseCameraMatrix * GetWorldMatrix());

	return m_ViewTransform;
}

const glm::mat4& dae::CTransform::GetWorldMatrix() const
{
	CalculateWorldTransform();

	return m_WorldMatrix;
}

glm::vec3 dae::CTransform::GetForwardVector() const
{
	CalculateWorldTransform(); 
	return -glm::normalize(glm::vec3(m_WorldMatrix[2]));
}

glm::vec3 dae::CTransform::GetUpVector() const
{
	CalculateWorldTransform();
	return glm::normalize(glm::vec3(m_WorldMatrix[1]));  // Y-axis
}

glm::vec3 dae::CTransform::GetRightVector() const
{
	CalculateWorldTransform();
	return glm::normalize(glm::vec3(m_WorldMatrix[0]));  // X-axis
}

void dae::CTransform::DeleteSelfAndDescendence()
{
	if (m_Self.IsValid())
	{
		std::queue<Pillar::FEntity> entities{};
		entities.push(m_Self.GetConstEntity());

		while (!entities.empty())
		{
			CTransform* currentTransform{ m_Registry->GetComponent<CTransform>(entities.front()) };

			if (currentTransform->HasChildren())
			{
				const Pillar::UEntityHandleContainer& children =
					currentTransform->GetChildren();

				for (size_t i = 0; i < children.NumOfEntities(); i++)
				{
					entities.push(children.ConstEntityAt(i).GetConstEntity());
				}
			}

			m_Registry->DestroyEntity(entities.front());
			entities.pop();
		}
	}
	
}

glm::vec3 dae::CTransform::LocalToWorldVec(const glm::vec3& localVec) const
{
	const glm::mat4& worldMatrix = GetWorldMatrix();
	return glm::vec3(worldMatrix * glm::vec4(localVec, 1.0f));
}

glm::vec2 dae::CTransform::LocalToWorldVec(const glm::vec2& localVec) const
{
	const glm::mat4& worldMatrix = GetWorldMatrix();
	glm::vec4 result = worldMatrix * glm::vec4(localVec, 0.0f, 1.0f);
	return glm::vec2(result);
}

glm::vec3 dae::CTransform::WorldToLocalVec(const glm::vec3& worldVec) const
{
	glm::mat4 inverseWorld = glm::inverse(GetWorldMatrix());
	return glm::vec3(inverseWorld * glm::vec4(worldVec, 1.0f));
}

glm::vec2 dae::CTransform::WorldToLocalVec(const glm::vec2& worldVec) const
{
	glm::mat4 inverseWorld = glm::inverse(GetWorldMatrix());
	glm::vec4 result = inverseWorld * glm::vec4(worldVec, 0.0f, 1.0f);
	return glm::vec2(result);
}

void dae::CTransform::OverrideWorldScaleWithLocalScale(bool override)
{
	NeedsToRecalculateWorldTransform();
	m_OverrideWorldScaleWithLocalScale = override;
}

void dae::CTransform::OverrideWorldPositionWithLocalPosition(bool override)
{
	NeedsToRecalculateWorldTransform();
	m_OverrideWorldPositionWithLocalPosition = override;
}

void dae::CTransform::OverrideWorldRotationWithLocalRotation(bool override)
{
	NeedsToRecalculateWorldTransform();
	m_OverrideWorldRotationWithLocalRotation = override;
}

void dae::CTransform::NeedsToRecalculateWorldTransform()
{
	if (!m_NeedToRecalculateWorldTransform)
	{
		m_NeedToRecalculateWorldTransform = true;

		if (HasChildren())
		{
			CTransform* currentParentTransform;
			CTransform* currentChildTransform;

			std::queue<CTransform*> transforms;
			transforms.push(this);

			while (!transforms.empty())
			{
				currentParentTransform = transforms.front();
				transforms.pop();

				const Pillar::UEntityHandleContainer& children = currentParentTransform->GetChildren();

				for (size_t i = 0; i < children.NumOfEntities(); i++)
				{
					currentChildTransform = m_Registry->GetComponent<CTransform>(children.ConstEntityAt(i).GetConstEntity());

					if (currentChildTransform && !currentChildTransform->m_NeedToRecalculateWorldTransform)
					{
						currentChildTransform->m_NeedToRecalculateWorldTransform = true;
						transforms.push(currentChildTransform);
					}
				}
			}
		}
		
	}
	
}

glm::mat4 dae::CTransform::MatrixFromTransformData(const TransformData& transformData)const
{
	glm::mat4 currentMatrix{ glm::identity<glm::mat4>() };

	// Correct order: T * R * S
	currentMatrix = glm::translate(currentMatrix, transformData.Position);
	currentMatrix = glm::rotate(currentMatrix, glm::radians(transformData.Rotation.x), glm::vec3{ 1.f, 0.f, 0.f });
	currentMatrix = glm::rotate(currentMatrix, glm::radians(transformData.Rotation.y), glm::vec3{ 0.f, 1.f, 0.f });
	currentMatrix = glm::rotate(currentMatrix, glm::radians(transformData.Rotation.z), glm::vec3{ 0.f, 0.f, 1.f });
	currentMatrix = glm::scale(currentMatrix, transformData.Scale);

	return currentMatrix;
}

dae::TransformData dae::CTransform::TransformDataFromMatrix(const glm::mat4& matrix)const
{
	TransformData transformData{};
	transformData.Position = glm::vec3(matrix[3]);

	transformData.Scale.x = glm::length(glm::vec3(matrix[0])); // X-axis scale
	transformData.Scale.y = glm::length(glm::vec3(matrix[1])); // Y-axis scale
	transformData.Scale.z = glm::length(glm::vec3(matrix[2])); // Z-axis scale

	glm::mat3 rotationMatrix = glm::mat3(matrix); // Extract 3x3 rotation matrix
	rotationMatrix[0] /= transformData.Scale.x; // Normalize X axis
	rotationMatrix[1] /= transformData.Scale.y; // Normalize Y axis
	rotationMatrix[2] /= transformData.Scale.z; // Normalize Z axis

	glm::quat rotation = glm::quat_cast(rotationMatrix); // Convert to quaternion
	transformData.Rotation = glm::eulerAngles(rotation); // Extract Euler angles (in radians)
	transformData.Rotation = glm::degrees(transformData.Rotation); // Convert to degrees if needed

	return transformData;
}

void dae::CTransform::CalculateWorldTransform()const
{
	if (m_NeedToRecalculateWorldTransform)
	{
		if (HasParent())
		{
			m_WorldMatrix = MatrixFromTransformData(m_LocalTransform);
			CTransform* currentParentTransform{ m_Registry->GetComponent<CTransform>(m_Parent.GetConstEntity()) };

			while (currentParentTransform)
			{
				//parent should always have transform
				if (currentParentTransform->m_NeedToRecalculateWorldTransform)
				{
					m_WorldMatrix = MatrixFromTransformData(currentParentTransform->m_LocalTransform) * m_WorldMatrix;
				
					if (currentParentTransform->HasParent())
					{
						currentParentTransform = m_Registry->GetComponent<CTransform>(currentParentTransform->m_Parent.GetConstEntity());
					}
					else
					{
						currentParentTransform = nullptr;
					}
				}
				else
				{
					m_WorldMatrix = currentParentTransform->m_WorldMatrix * m_WorldMatrix;
					currentParentTransform = nullptr;
				}

			}

			m_WorldTransform = TransformDataFromMatrix(m_WorldMatrix);

			if (m_OverrideWorldScaleWithLocalScale)
			{
				m_WorldTransform.Scale = m_LocalTransform.Scale;
			}
			if (m_OverrideWorldPositionWithLocalPosition)
			{
				m_WorldTransform.Position = m_LocalTransform.Position;
			}
			if (m_OverrideWorldRotationWithLocalRotation)
			{
				m_WorldTransform.Rotation = m_LocalTransform.Rotation;
			}
		}
		else
		{
			m_WorldMatrix = MatrixFromTransformData(m_LocalTransform);
			m_WorldTransform = m_LocalTransform;
		}

		
	}

	m_NeedToRecalculateWorldTransform = false;
	
}

dae::SDebugDrawTransform::SDebugDrawTransform(Pillar::URegistry& registry) :
	m_Transforms(CTransform{registry})
{
	registry.GetComponents(m_Transforms);
}

void dae::SDebugDrawTransform::Render() 
{
	glm::ivec2 center{};
	for (size_t archIdx = 0; archIdx < m_Transforms.NumOfArchetypes(); archIdx++)
	{
		for (size_t compIdx = 0; compIdx < m_Transforms.NumOfActivatedComponents(archIdx); compIdx++)
		{

			center.x = (int)m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform().Position.x;
			center.y = (int)m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform().Position.y;

			Renderer::GetInstance().DrawCross(center, 10, SDL_Color{0, 255, 0});
		}
	}
}
