#ifndef DAISY_VECTOR4_H
#define DAISY_VECTOR4_H

#include <cmath>

namespace daisy {
	class vector4 final
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vector4();
		vector4(float const x, float const y, float const z, float const w);
	};
}

#endif // !DAISY_VECTOR4_H
