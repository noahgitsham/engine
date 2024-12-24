#include <cstddef>
#include <cstring>
#include <span>
#include <stdexcept>

namespace colours {

	enum c {
		red = 0,
		green = 1,
		blue = 2,
		alpha = 3,
	};

	typedef struct {
		float colors[3];
	} RGB;

	typedef struct {
		float colors[4];
	} RGBA;

	typedef struct {
		std::byte colours[3] = {};
	} HEX;

	typedef struct {
		std::byte colours[4];
	} HEXA;

	HEX string2HEX(char* str) {
		char* strc; 

		// Remove leading #
		if (strc[0] == '#') strcpy(strc, str+1);
		else strcpy(strc, str);

		if (sizeof(strc) != 6 * sizeof(char)) throw std::invalid_argument("Expected HEX code");

		for(int i = 0; strc[i]; i++) strc[i] = tolower(strc[i]) - 'a';

		std::byte r = 16 * strc[0] + strc[1];
		std::byte g;
		std::byte b;

		return (HEX){ .colours= }
	}

	RGB HEX2RGB(HEX hex) {
		hex.color;
	}
}
