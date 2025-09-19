#pragma once
#include "PillarLibrary_02/include/InternalIncludes.h"
#include "Scene.h"

#include <glm.hpp>
namespace dae
{
	struct TransformData final
	{
		glm::vec3 Position{ 0.f, 0.f, 0.f};
		glm::vec3 Scale{ 1.f, 1.f, 1.f };
		glm::vec3 Rotation{ 0.f,0.f, 0.f };

		TransformData& operator+=(const TransformData& other) {
			Position += other.Position;
			Scale *= other.Scale;
			Rotation += other.Rotation;
			return *this;
		}

		
	};

	inline TransformData operator+(const TransformData& one, const TransformData& two) {
		
		return TransformData{one.Position + two.Position, one.Scale * two.Scale, one.Rotation + two.Rotation};
	}

	enum class ETransformReparentType {
		KeepWorldTransform,
		KeepLocalTransform
	};

	/*
		retrieve the first transform in the hierarchy that is dirty and calculate all of the matrices in a specific hierarchy
		set all of the children that were not calculated as
		calculate the 
	
	*/

	/*
	
	Fixed Update
	- Change Physics Values
	- Change transform values with physics

	Update
	- Systems That Set Transforms Outside physics (respawn)
	- Calculate the transform hierarchy based on dirtied values
	- Render on screen
	
	*/

	/*
	
	 --- APPROACH 1 ---

	- Only calculate the transform hierarchy once


	Problems with this approach
	- if you Get the transform value, it will be the one from the last tick
	- the transform values will only be modified at the end of the tick
	
	Perks
	- You only calculate the transform once
	- The Set Dirty is way faster because you don't have to set dirty of all of the children nodes

	--- APPROACH 2 ---

	- Calculate the transform each time you get it


	Problems with this approach
	- calculate the transform value more than once
	- the set dirty is expensive because you have to set all of the children transform dirty

	Perks
	- each time you get the transform, you get a fresh transform

	//
	For now I will choose APPROACH 2
	
	*/

	/*
	
	update loop
	check if any is dirtied and if it is update all of the below
	render all of them based on the view transform
	*/

	//If you change 
	class CTransform final
	{
	public:
		CTransform(Pillar::URegistry& registry);
		bool Initialize(const Pillar::UEntityHandle& self);

		bool HasParent()const;
		bool HasChildren()const;
		const Pillar::UEntityHandle& GetParent()const;
		void MakeRootNode();
		bool SetParent(const Pillar::UEntityHandle& parent, ETransformReparentType transformReparentType);
		const Pillar::UEntityHandleContainer& GetChildren()const;
		bool IsDescendant(const Pillar::UEntityHandle& descendant);

		void SetLocalScaleX(float scale);
		void SetLocalScaleY(float scale);
		void SetLocalScaleZ(float scale);
		void SetLocalScale(const glm::vec3& scale);

		void SetLocalPositionX(float pos);
		void SetLocalPositionY(float pos);
		void SetLocalPositionZ(float pos);
		void SetLocalPosition(const glm::vec3& pos);

		void MoveLocalPositionX(float pos);
		void MoveLocalPositionY(float pos);
		void MoveLocalPositionZ(float pos);
		void MoveLocalPosition(const glm::vec3& pos);

		void SetLocalRotationX(float rotation);
		void SetLocalRotationY(float rotation);
		void SetLocalRotationZ(float rotation);
		void SetLocalRotation(const glm::vec3& rotation);

		void MoveLocalRotationX(float rotation);
		void MoveLocalRotationY(float rotation);
		void MoveLocalRotationZ(float rotation);
		void MoveLocalRotation(const glm::vec3& rotation);

		void SetLocalTransform(const TransformData& transformData);

		const TransformData& GetWorldTransform()const;
		const TransformData& GetLocalTransform()const;
		const TransformData& GetViewTransform(const glm::mat4& inverseCameraMatrix)const;
		const glm::mat4& GetWorldMatrix()const ;

		glm::vec3 GetForwardVector()const; 
		glm::vec3 GetUpVector()const;
		glm::vec3 GetRightVector()const;

		void DeleteSelfAndDescendence();

		glm::vec3 LocalToWorldVec(const glm::vec3& localVec) const;
		glm::vec2 LocalToWorldVec(const glm::vec2& localVec) const;
		glm::vec3 WorldToLocalVec(const glm::vec3& worldVec) const;
		glm::vec2 WorldToLocalVec(const glm::vec2& worldVec) const;

		//Does the transformation, keeps the world matrix, but overrides the world transform.
		//Of course, this effect can only be noticed if the transform is child since all of the 
		//root local transforms are by default world transforms
		void OverrideWorldScaleWithLocalScale(bool override);
		void OverrideWorldPositionWithLocalPosition(bool override);
		void OverrideWorldRotationWithLocalRotation(bool override);

	private:

		//because of dirty pattern, some values have to be mutable and const

		void NeedsToRecalculateWorldTransform();
		glm::mat4 MatrixFromTransformData(const TransformData& transformData)const;
		TransformData TransformDataFromMatrix(const glm::mat4& matrix)const;

		void CalculateWorldTransform()const;

		mutable TransformData m_ViewTransform;
		mutable TransformData m_WorldTransform;
		TransformData m_LocalTransform;
		mutable glm::mat4 m_WorldMatrix;

		Pillar::UEntityHandle m_Self;
		Pillar::UEntityHandle m_Parent;
		Pillar::UEntityHandleContainer m_Children;

		Pillar::URegistry* m_Registry;
		mutable bool m_NeedToRecalculateWorldTransform;

		bool m_OverrideWorldScaleWithLocalScale			{false};
		bool m_OverrideWorldPositionWithLocalPosition	{false};
		bool m_OverrideWorldRotationWithLocalRotation	{false};

	};


	class SDebugDrawTransform final : public IRenderSystem {

	public:
		SDebugDrawTransform(Pillar::URegistry& registry);
		void Render() override;

	private:

		Pillar::UComponentIterator<CTransform> m_Transforms;
	};
}
