#pragma once

#ifndef WINPP_COMMON_METHODS_H
#define WINPP_COMMON_METHODS_H

#include "../structures/enumerations.h"

namespace winpp{
	namespace common{
		class methods{
		public:
			typedef structures::enumerations::placement_type placement_type;

			static bool has_left(placement_type placement);

			static bool has_top(placement_type placement);

			static bool has_right(placement_type placement);

			static bool has_bottom(placement_type placement);

			static bool has_horizontal_center(placement_type placement);

			static bool has_vertical_center(placement_type placement);

			template <typename rect_type, typename size_type>
			static void apply_placement(placement_type placement, rect_type &rect, const size_type &size){
				if (has_right(placement)){
					rect.right = rect.left;
					rect.left -= size.width();
				}
				else if (has_horizontal_center(placement)){
					rect.left -= (size.width() / 2);
					rect.right -= (size.width() / 2);
				}

				if (has_bottom(placement)){
					rect.bottom = rect.top;
					rect.top -= size.height();
				}
				else if (has_vertical_center(placement)){
					rect.top -= (size.height() / 2);
					rect.bottom -= (size.height() / 2);
				}
			}
		};
	}
}

#endif /* !WINPP_COMMON_METHODS_H */
