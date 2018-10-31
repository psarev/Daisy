#ifndef DAISY_COLOR_H
#define DAISY_COLOR_H

namespace daisy {
	class color final
	{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;

		bool operator==(color const& c) const;

		static const color BLACK;
		static const color WHITE;
		static const color RED;
		static const color GREEN;
		static const color BLUE;
	};
}

#endif // !DAISY_COLOR_H
