#pragma once

#ifndef WINPP_DRAWING_BRUSH_H
#define WINPP_DRAWING_BRUSH_H

#include "drawer.h"

#include "../common/exception.h"
#include "../structures/color_structure.h"

namespace winpp{
	namespace drawing{
		class brush{
		public:
			typedef drawer drawer_type;

			typedef ::ID2D1Brush value_type;
			typedef value_type *pointer_type;

			typedef ::D2D1_COLOR_F colorf_type;
			typedef structures::color color_type;

			brush();

			virtual ~brush();

			brush(const drawer &) = delete;

			brush &operator =(const brush &) = delete;

			virtual operator pointer_type() const;

			virtual pointer_type operator ->() const;

			virtual pointer_type value() const = 0;

			virtual void color(const colorf_type &value);

			virtual void color(const color_type &value);

			virtual colorf_type color() const;
		};
	}
}

#endif /* !WINPP_DRAWING_BRUSH_H */
