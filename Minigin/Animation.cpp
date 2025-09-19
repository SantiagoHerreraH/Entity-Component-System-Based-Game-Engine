#include "Animation.h"
#include "Renderer.h"
#include "Time.h"
#include <algorithm>

namespace dae {

#pragma region Animation

	void CAnimation::Play()
	{
		m_IsPlaying = true;
	}
	void CAnimation::Pause()
	{
		m_IsPlaying = false;
	}
	const CTextureHandle& CAnimation::GetConstTextureRef(int frameNumber)const
	{
		return m_Frames.at(frameNumber).TextureHandle;
	}
	const CTextureHandle& CAnimation::GetCurrentFrame() const
	{
		return m_Frames.at(m_CurrentFrameIndex).TextureHandle;
	}
	void CAnimation::CanLoop(bool canLoop)
	{
		m_CanLoop = canLoop;
	}
	void CAnimation::SetFramesPerSecond(unsigned int numberOfFrames)
	{
		m_SecondsPerFrame = 1.f / float(numberOfFrames);

		float sumedTime{ 0 };
		for (size_t i = 0; i < m_Frames.size(); i++)
		{
			m_Frames.at(i).TimeToGoToTheNextFrame = sumedTime + (m_Frames.at(i).NumberOfFrames * m_SecondsPerFrame);
			sumedTime = m_Frames.at(i).TimeToGoToTheNextFrame;
		}

		Reset();
	}
	void CAnimation::AddFrame(const CTextureHandle& frame, unsigned int numberOfFrames)
	{
		float sumedTime = m_Frames.empty() ? 0 : m_Frames.at(m_Frames.size() - 1).TimeToGoToTheNextFrame;

		m_Frames.emplace_back(AnimationFrameInfo{ frame, numberOfFrames , 0});

		m_Frames.at(m_Frames.size() - 1).TimeToGoToTheNextFrame = sumedTime + (m_Frames.at(m_Frames.size() - 1).NumberOfFrames * m_SecondsPerFrame);
	}

	void CAnimation::ClearFrames()
	{
		m_NumberOfLoops = 0;
		m_IsPlaying = false;
		m_CurrentTime = 0 ;
		m_CurrentFrameIndex = 0;
		m_SecondsPerFrame = 1;
		m_Frames.clear();
	}

	int CAnimation::LoopNumber()
	{
		return m_NumberOfLoops;
	}

	void CAnimation::Reset()
	{
		m_CurrentTime = 0;
		m_NumberOfLoops = 0;
		m_CurrentFrameIndex = 0;
	}

	void CAnimation::Update()
	{
		if (m_IsPlaying)
		{
			m_CurrentTime += Time::GetInstance().GetElapsedSeconds();
			//desired time to change to next frame

			if (m_CurrentTime >= m_Frames.at(m_CurrentFrameIndex).TimeToGoToTheNextFrame)
			{
				

				if (m_CanLoop)
				{
					m_NumberOfLoops = m_CurrentFrameIndex >= m_Frames.size() - 1 ? m_NumberOfLoops + 1 : m_NumberOfLoops;

					m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_Frames.size();
				}
				else if(!m_CanLoop && !m_Frames.empty())
				{

					m_NumberOfLoops = (m_CurrentFrameIndex >= m_Frames.size() - 1) && m_NumberOfLoops < 1 ? m_NumberOfLoops + 1 : m_NumberOfLoops;

					m_CurrentFrameIndex = std::clamp((m_CurrentFrameIndex + 1), 0, (int)m_Frames.size() - 1);
				}

				m_CurrentTime = m_CurrentFrameIndex == 0 ? 0 : m_CurrentTime;

			}
		}
	}

#pragma endregion



	SAnimationPlayer::SAnimationPlayer(Pillar::URegistry& registry) :
		m_Transforms(CTransform{registry})
	{
		Pillar::UComponentFilter filter{};
		filter.SetIncludingFilter(CTransform{ registry }, CAnimation{});
		registry.GetComponents(m_Animations, filter);
		registry.GetComponents(m_Transforms, filter);
	}

	void SAnimationPlayer::Update()
	{
		for (size_t archIdx = 0; archIdx < m_Animations.NumOfArchetypes(); archIdx++)
		{
			for (size_t compIdx = 0; compIdx < m_Animations.NumOfActivatedComponents(archIdx); compIdx++)
			{
				m_Animations.At(archIdx, compIdx).Update();
			}
		}
	}

	void SAnimationPlayer::Render()
	{
		for (size_t archIdx = 0; archIdx < m_Animations.NumOfArchetypes(); archIdx++)
		{
			for (size_t compIdx = 0; compIdx < m_Animations.NumOfActivatedComponents(archIdx); compIdx++)
			{
				TransformData transformData{ m_Transforms.ConstAt(archIdx, compIdx).GetWorldTransform() };
				transformData += m_Animations.ConstAt(archIdx, compIdx).GetCurrentFrame().GetTextureTransform();

				Renderer::GetInstance().RenderTexture(
					*m_Animations.ConstAt(archIdx, compIdx).GetCurrentFrame().Data(),
					transformData.Position,
					transformData.Rotation,
					transformData.Scale,
					m_Animations.ConstAt(archIdx, compIdx).GetCurrentFrame().GetTextureView());
			}
		}
	}

}

