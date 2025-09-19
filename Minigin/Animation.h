#pragma once

#include "SystemInterfaces.h"
#include "Texture.h"
#include "Rect.h"
#include "PillarLibrary_02/include/InternalIncludes.h"

namespace dae {

	/*
		Each animation for now has:
		- a character
			- 
		- a state
		- 
		- The animation manager is just the interface 
		to change the animation state -> the actual change in state will be managed by the event system
		- There will be an event listener that will change the current animation corresponding its state
	*/

	

	class CAnimation final{

	public:

		void Update();

		void CanLoop(bool canLoop); //default is true
		void SetFramesPerSecond(unsigned int numberOfFrames);
		void AddFrame(const CTextureHandle& frame, unsigned int numberOfFrames);

		void ClearFrames();

		int LoopNumber();

		void Reset();
		void Play();
		void Pause();

		const CTextureHandle& GetConstTextureRef(int frameNumber)const;
		const CTextureHandle& GetCurrentFrame()const;

	private:
		int m_NumberOfLoops{ 0 };
		bool m_CanLoop{ true };
		bool m_IsPlaying{ true };
		double m_CurrentTime{0};
		int m_CurrentFrameIndex{ 0 };
		float m_SecondsPerFrame{ 1 };

		struct AnimationFrameInfo {
			CTextureHandle TextureHandle;
			unsigned int NumberOfFrames;
			float TimeToGoToTheNextFrame;
		};

		std::vector<AnimationFrameInfo> m_Frames;
	};


	class SAnimationPlayer final : public IUpdateSystem, public IRenderSystem
	{

	public:
		SAnimationPlayer(Pillar::URegistry& registry);
		virtual void Update() override;
		virtual void Render() override;

	private:

		Pillar::UComponentIterator<CAnimation> m_Animations;
		Pillar::UComponentIterator<CTransform> m_Transforms;

	};
}