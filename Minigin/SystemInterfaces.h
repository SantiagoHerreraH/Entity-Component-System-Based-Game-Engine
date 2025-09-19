#pragma once
#include <memory>
#include "PillarLibrary_02/include/InternalIncludes.h"

namespace dae
{

	/*
		All of the system interfaces have constructors that accept URegistries, and do nothing,
		except enforce all of the children systems to have registries in their constructors because of how 
		UComponentIterators work. 
		Also, they are filled in with registry; because of compiler warnings that are treated as errors
	*/

	class IStartSystem {

	public:
		virtual ~IStartSystem() {};
		virtual void Start() = 0;
	};

	class IFixedUpdateSystem {

	public:
		virtual ~IFixedUpdateSystem() {};
		virtual void FixedUpdate() = 0;
	};

	class IUpdateSystem {

	public:
		virtual ~IUpdateSystem() {};
		virtual void Update() = 0;
	};

	class IRenderSystem{

	public:

		virtual ~IRenderSystem() {};
		virtual void Render() = 0;
	};
}
