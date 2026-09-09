#ifndef COLOR_H
#define COLOR_H

//
#include <set>
#include <string>

//
enum class COLOR
{
	//
	NONE,

	//
	RED,
	ORANGE,
	YELLOW,
	GREEN,
	BLUE,
	VIOLET,

	//
	COLOR_COUNT
};

//
class Colors
{
public:
	//
	static std::string ToString(COLOR color);

	//
	static std::set<COLOR> Distinct
	(
		const std::set<COLOR>& firstColors, 
		const std::set<COLOR>& secondColors
	);
};

#endif //COLOR_H