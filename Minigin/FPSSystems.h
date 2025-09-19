#pragma once

#include "SystemInterfaces.h"
#include "Transform.h"
#include "Text.h"

namespace dae {

	enum class EFPSTimeFormat {
		Frames,
		//TODO -> Time
	};

	enum class EFPSInfoType {
		UpdateElapsedSeconds,
		// TODO -> FixedUpdateDuration
	};

	class CFPSCounter {

	public:
		void SetInfoType(EFPSInfoType infoType);
		EFPSInfoType GetInfoType()const;

		void SetTimeFormat(EFPSTimeFormat timeFormat);
		EFPSTimeFormat GetTimeFormat()const;

		void SetTimeDelay(float timeDelay);
		void IncreaseCounter(float value);
		void ResetCounter();
		bool CounterReachedTimeDelay()const;

		void SetValue(int value);
		int GetValue()const;

	private:
		EFPSInfoType m_InfoType;
		EFPSTimeFormat m_TimeFormat;
		float m_TimeDelay{1.f};
		float m_CurrentTimeDelay;
		int m_Value;
	};

	class SFPSCountingSystem final : public IUpdateSystem {

	public:
		SFPSCountingSystem(Pillar::URegistry& registry);

		virtual void Update() override;

	private:
		Pillar::UComponentIterator<CFPSCounter> m_FPSCounters;
		Pillar::URegistry& m_Registry;
	};

	class SFPSRenderingSystem final : public IUpdateSystem, public IRenderSystem {

	public:
		SFPSRenderingSystem(Pillar::URegistry& registry);

		virtual void Update() override;
		virtual void Render() override;

	private:
		Pillar::UComponentIterator<CFPSCounter> m_FPSCounters;
		mutable Pillar::UComponentIterator<CText> m_Text;
		Pillar::UComponentIterator<CTransform> m_Transforms;
	};
}