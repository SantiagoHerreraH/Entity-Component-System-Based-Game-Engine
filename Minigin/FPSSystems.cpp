#include "FPSSystems.h"
#include "Time.h"
#include "Renderer.h"

namespace dae {


	void CFPSCounter::SetInfoType(EFPSInfoType infoType)
	{
		m_InfoType = infoType;
	}

	EFPSInfoType CFPSCounter::GetInfoType() const
	{
		return m_InfoType;
	}

	void CFPSCounter::SetTimeFormat(EFPSTimeFormat timeFormat)
	{
		m_TimeFormat = timeFormat;
	}

	EFPSTimeFormat CFPSCounter::GetTimeFormat() const
	{
		return m_TimeFormat;
	}

	void CFPSCounter::SetTimeDelay(float timeDelay)
	{
		m_TimeDelay = timeDelay;
	}

	void CFPSCounter::IncreaseCounter(float value)
	{
		m_CurrentTimeDelay += value;
	}

	void CFPSCounter::ResetCounter()
	{
		m_CurrentTimeDelay = m_TimeDelay <= m_CurrentTimeDelay ? m_CurrentTimeDelay - m_TimeDelay : 0;
	}

	bool CFPSCounter::CounterReachedTimeDelay() const
	{
		return m_CurrentTimeDelay >= m_TimeDelay;
	}

	void CFPSCounter::SetValue(int value)
	{
		m_Value = value;
	}

	int CFPSCounter::GetValue() const
	{
		return m_Value;
	}

	SFPSCountingSystem::SFPSCountingSystem(Pillar::URegistry& registry) : 
		m_Registry(registry)
	{
		registry.GetComponents(m_FPSCounters);
	}
	void SFPSCountingSystem::Update()
	{
		std::vector<CFPSCounter> comps{};

		for (size_t archIdx = 0; archIdx < m_FPSCounters.NumOfArchetypes(); archIdx++)
		{
			comps = m_FPSCounters.Move(archIdx);

			for (size_t compIdx = 0; compIdx < comps.size(); compIdx++)
			{
				comps[compIdx].IncreaseCounter(Time::GetInstance().GetElapsedSeconds());

				if (comps[compIdx].CounterReachedTimeDelay())
				{
					if (comps[compIdx].GetInfoType() == EFPSInfoType::UpdateElapsedSeconds)
					{
						if (comps[compIdx].GetTimeFormat() == EFPSTimeFormat::Frames)
						{
							comps[compIdx].SetValue(Time::GetInstance().GetFramesPerSecond());
						}
					}

					comps[compIdx].ResetCounter();
				}

			}

			m_FPSCounters.MoveBack(archIdx, std::move(comps));
		}

	}
	SFPSRenderingSystem::SFPSRenderingSystem(Pillar::URegistry& registry) :
		m_Transforms(CTransform{ registry })
	{
		Pillar::UComponentFilter filter;
		filter.SetIncludingFilter(CFPSCounter{}, CText{}, CTransform{registry});

		registry.GetComponents(m_FPSCounters, filter);
		registry.GetComponents(m_Text, filter);
		registry.GetComponents(m_Transforms, filter);
	}
	
	void SFPSRenderingSystem::Update()
	{
		int value{};

		std::vector<CFPSCounter>	fpsComps{};
		std::vector<CText>			textComps{};
		size_t currentActiveComponents = 0;

		for (size_t archIdx = 0; archIdx < m_FPSCounters.NumOfArchetypes(); archIdx++)
		{
			currentActiveComponents = m_FPSCounters.NumOfActivatedComponents(archIdx);
			fpsComps = m_FPSCounters.Move(archIdx);
			textComps = m_Text.Move(archIdx);

			for (size_t compIdx = 0; compIdx < currentActiveComponents; compIdx++)
			{
				fpsComps[compIdx].IncreaseCounter(Time::GetInstance().GetElapsedSeconds());

				if (fpsComps[compIdx].CounterReachedTimeDelay())
				{
					if (fpsComps[compIdx].GetInfoType() == EFPSInfoType::UpdateElapsedSeconds)
					{
						if (fpsComps[compIdx].GetTimeFormat() == EFPSTimeFormat::Frames)
						{
							fpsComps[compIdx].SetValue(Time::GetInstance().GetFramesPerSecond());

							value = fpsComps[compIdx].GetValue();
							textComps[compIdx].SetText(std::to_string(value));
						}
					}

					fpsComps[compIdx].ResetCounter();
				}

			}

			m_FPSCounters.MoveBack(archIdx, std::move(fpsComps));
			m_Text.MoveBack(archIdx, std::move(textComps));
		}
	}

	void SFPSRenderingSystem::Render() 
	{
		for (size_t archIdx = 0; archIdx < m_FPSCounters.NumOfArchetypes(); archIdx++)
		{
			for (size_t compIdx = 0; compIdx < m_FPSCounters.NumOfActivatedComponents(archIdx); compIdx++)
			{
				Renderer::GetInstance().RenderTexture(
					*m_Text.At(archIdx, compIdx).Data(),
					m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform().Position.x,
					m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform().Position.y);
			}
		}
	}
	
}