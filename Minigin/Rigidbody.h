#pragma once
#include <glm.hpp>
#include "PillarLibrary_02/include/InternalIncludes.h"
#include "Transform.h"
#include "SystemInterfaces.h"

namespace dae {

	enum class ForceMode2D {
		Force, // multiply by fixedTimeStep
		Impulse, // leaves the value
	};

	enum class SetVelocityMode {
		NullForces, 
		KeepForces
	};

	class CRigidbody2D final{

	public:

		void SetNetTorque(const glm::vec3& force, ForceMode2D forceMode2D = ForceMode2D::Force);
		void SetNetAngularAcceleration(const glm::vec3& acceleration, ForceMode2D forceMode2D = ForceMode2D::Force);
		void SetNetForce(const glm::vec2& force,  ForceMode2D forceMode2D = ForceMode2D::Force);
		void SetNetAcceleration(const glm::vec2& acceleration, ForceMode2D forceMode2D = ForceMode2D::Force);

		void AddTorque(const glm::vec3& force, ForceMode2D forceMode2D = ForceMode2D::Force);
		void AddForce(const glm::vec2& force, ForceMode2D forceMode2D  = ForceMode2D::Force);
		void AddAcceleration(const glm::vec2& acceleration, ForceMode2D forceMode2D = ForceMode2D::Force);

		void AddVelocity(const glm::vec2& velocity, ForceMode2D forceMode2D = ForceMode2D::Force);
		void AddAngularVelocity(const glm::vec3& velocity, ForceMode2D forceMode2D = ForceMode2D::Force);
		
		const glm::vec3& GetNetTorque()const;
		const glm::vec3& GetNetAngularAcceleration()const;

		const glm::vec2& GetNetAcceleration()const;
		const glm::vec2& GetNetForce()const;

		float GetMass()const;
		void  SetMass(float mass);

		void ClampVelocity(float maxMagnitude, SetVelocityMode setVelocityModeForXAxis = SetVelocityMode::KeepForces);
		void SetVelocityX(float velX, SetVelocityMode setVelocityModeForXAxis = SetVelocityMode::KeepForces);
		void SetVelocityY(float velY, SetVelocityMode setVelocityModeForYAxis = SetVelocityMode::KeepForces);
		void SetVelocity(const glm::vec2& vel, SetVelocityMode setVelocityMode = SetVelocityMode::KeepForces);

		float GetVelocityX()const;
		float GetVelocityY()const;
		const glm::vec2& GetVelocity()const;

		float GetAngularVelocityX()const;
		float GetAngularVelocityY()const;
		float GetAngularVelocityZ()const;
		const glm::vec3& GetAngularVelocity()const;

	private:
		float m_Mass = 1.f;

		glm::vec3 m_AngularVelocity{};
		glm::vec3 m_NetAngularAcceleration{};
		glm::vec3 m_NetTorque{};

		glm::vec2 m_LinearVelocity{};
		glm::vec2 m_NetAcceleration{};
		glm::vec2 m_NetForce{};

	};

	class SApplyRigidbody2D final : public IFixedUpdateSystem {
	public:
		SApplyRigidbody2D(Pillar::URegistry& registry);
		virtual void FixedUpdate() override;

	private:

		Pillar::UComponentIterator<CRigidbody2D> m_Rigidbodies2D;
		Pillar::UComponentIterator<CTransform> m_Transforms;
	};
}