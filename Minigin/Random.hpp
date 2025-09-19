#pragma once

#include <random>

namespace dae {

	class Random {

	public:
		static int GetRandomBetweenRange(int min, int max)
		{
			//if you don't add the + 1, the range can never reach max
			return (std::rand() % (max - min + 1)) + min;
		}
	};
}
