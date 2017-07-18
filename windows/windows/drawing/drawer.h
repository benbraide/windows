#pragma once

#ifndef WINPP_DRAWER_H
#define WINPP_DRAWER_H

#include <memory>

#include "drawing_factory.h"
#include "../structures/color_structure.h"

namespace winpp{
	namespace drawing{
		class brush;

		class drawer{
		public:
			typedef factory factory_type;

			typedef ::ID2D1RenderTarget value_type;
			typedef value_type *pointer_type;

			typedef brush brush_type;
			typedef std::shared_ptr<brush_type> brush_ptr_type;

			typedef ::D2D1_COLOR_F colorf_type;
			typedef structures::color color_type;

			drawer();

			virtual ~drawer();

			drawer(const drawer &) = delete;
			
			drawer &operator =(const drawer &) = delete;

			virtual operator pointer_type() const;

			virtual pointer_type operator ->() const;

			virtual pointer_type value() const = 0;

			virtual brush_type &default_brush();

			virtual brush_type &default_brush(const colorf_type &color);

			virtual brush_type &default_brush(const color_type &color);

		protected:
			brush_ptr_type brush_;
		};
	}
}

#endif /* !WINPP_DRAWER_H */
