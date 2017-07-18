#pragma once

#ifndef WINPP_DRAWING_SOLID_COLOR_BRUSH_H
#define WINPP_DRAWING_SOLID_COLOR_BRUSH_H

#include "drawing_brush.h"

namespace winpp{
	namespace drawing{
		class solid_color_brush : public brush{
		public:
			typedef ::ID2D1SolidColorBrush solid_value_type;
			typedef solid_value_type *solid_pointer_type;

			solid_color_brush(drawer_type &drawer, const colorf_type &value);

			solid_color_brush(drawer_type &drawer, const color_type &value);

			virtual ~solid_color_brush();

			virtual pointer_type value() const override;

			virtual void color(const colorf_type &value) override;

			virtual colorf_type color() const override;

		protected:
			solid_pointer_type value_;
		};
	}
}

#endif /* !WINPP_DRAWING_SOLID_COLOR_BRUSH_H */
