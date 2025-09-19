
# About My Project

I completely reworked Minigin, whose features are described below, and added the following features:

# Game Engine Features


- A row based Entity Component Framework (PillarLibrary_02) that lets you
	- Create id based entities and assign different components to them through a Registry
	- reduce cache misses by maintaining memory contiguity through keeping component matrices (not arrays because of archetype separation)
	- Filter through entity Archetypes and make entity component queries based on components they have or don't have
	- Automatically registers new components
	- Maintain memory contiguity upon entity deactivation through memory swapping
	- Iterators that automatically update upon internal memory movement
	- A registry singleton system
- A Scene Manager system that lets manage scenes and group systems execution
- A scene graph that lets you control spatial object transformations (Transform class)
		- Lets you control rotation, translation, scale
		- Lets you parent and unparent 
		- lets you control which transformation variables you want the Transform to be affected by its parent transformation
			- for instance, have rotation and translation be affected by scene graph, but not scale
- A Game Loop including
	- Update
	- FixedUpdate
	- Render
	- Input tracking
	- Time tracking
- Physics systems
	- 2d triggering systems for nonsolid rectangles
	- 2d Rigidbody systems
	- Movement system for physics interaction
- A State Controller that manages state changes
- A variadic template Event System with instantaneous triggering where you can subscribe std::functions
- A templated Stat Controller with various events that are triggered upon stat modification
- A player controller that receives input from keyboard and gamepad, where you can link Commands to input (through ICommand interface)
- Different rendering systems that include
	- SpriteSheet: for managing grid like textures
	- Animation: animating textures and controlling framerate
	- Simple particle system: where you can control different variables for movable particles
	- Improved ResourceManager for limited loading of resources such as textures
- A ServiceLocator for interacting with and changing different services at runtime depending on needs
- A TimerSystem for controlling execution through various Events (OnStart, OnUpdate, OnEnd, etc...) in the time of your choice
- Simple but effective math classes for 
	- Getting random value between range
	- Controlling sine and cosine movement with x direction and time
- A MovementActionSequence system that lets you
	- chain actions one after another and set their duration to be conditional or time dependent
	- Skip action execution through conditions



# GitHub repo Link

https://github.com/SantiagoHerreraH/Entity-Component-System-Based-Game-Engine.git

# About the Base Minigin

Minigin is a very small project using [SDL2](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/avadae/minigin/actions/workflows/msbuild.yml/badge.svg)](https://github.com/avadae/msbuild/actions)
[![GitHub Release](https://img.shields.io/github/v/release/avadae/minigin?logo=github&sort=semver)](https://github.com/avadae/minigin/releases/latest)

# Goal

Minigin can/may be used as a start project for the exam assignment in the course [Programming 4](https://youtu.be/j96Oh6vzhmg) at DAE. In that assignment students need to recreate a popular 80's arcade game with a game engine they need to program themselves. During the course we discuss several game programming patterns, using the book '[Game Programming Patterns](https://gameprogrammingpatterns.com/)' by [Robert Nystrom](https://github.com/munificent) as reading material. 

# Disclaimer

Minigin is, despite perhaps the suggestion in its name, **not** a game engine. It is just a very simple sdl2 ready project with some of the scaffolding in place to get started. None of the patterns discussed in the course are used yet (except singleton which use we challenge during the course). It is up to the students to implement their own vision for their engine, apply patterns as they see fit, create their game as efficient as possible.

# Use

Either download the latest release of this project and compile/run in visual studio or, since students need to have their work on github too, they can use this repository as a template (see the "Use this template" button at the top right corner). There is no point in forking this project.
