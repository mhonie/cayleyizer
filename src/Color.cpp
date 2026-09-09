//
#include <algorithm>
#include <iterator>
#include <string>

//
#include "Color.h"

//
std::string Colors::ToString(const COLOR color)
{
	switch (color)
	{
		case COLOR::NONE:
			return "NONE";

		case COLOR::RED:
			return "RED";

		case COLOR::BLUE:
			return "BLUE";

		case COLOR::GREEN:
			return "GREEN";

		case COLOR::ORANGE:
			return "ORANGE";

		case COLOR::VIOLET:
			return "VIOLET";

		case COLOR::YELLOW:
			return "YELLOW";

		case COLOR::COLOR_COUNT:
		{
			constexpr int colorCount = 
				
				static_cast<int>(COLOR::COLOR_COUNT);

			return std::to_string(colorCount);
		}
	}

	return {};
}

//
std::set<COLOR> Colors::Distinct
(
	const std::set<COLOR>& firstColors,	
	const std::set<COLOR>& secondColors
)
{
	std::set<COLOR> distinctColors;

	std::set_union
	(
		std::begin(firstColors),
		std::end(firstColors),
		std::begin(secondColors),
		std::end(secondColors),
		std::inserter
		(
			distinctColors,
			std::end(distinctColors)
		)
	);

	return distinctColors;
}