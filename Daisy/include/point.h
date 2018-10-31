#ifndef DAISY_POINT_H
#define DAISY_POINT_H

namespace daisy {
	class point2d final
	{
	public:
		unsigned int x;
		unsigned int y;

		bool operator==(point2d const& p) const;
	};
}

#endif // !DAISY_POINT_H

