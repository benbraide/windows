#pragma once

#ifndef WINPP_DRAWING_RADIAL_GRADIENT_BRUSH_H
#define WINPP_DRAWING_RADIAL_GRADIENT_BRUSH_H

#include "drawing_brush.h"
#include "drawing_gradient_stop_collection.h"

namespace winpp{
	namespace drawing{
		class radial_gradient_brush : public brush{
		public:
			typedef ::ID2D1RadialGradientBrush radial_gradient_value_type;
			typedef radial_gradient_value_type *radial_gradient_pointer_type;

			typedef ::D2D1_RADIAL_GRADIENT_BRUSH_PROPERTIES properties_type;

			radial_gradient_brush(drawer_type &drawer, const properties_type &properties, gradient_stop_collection &stop_collection);

			virtual ~radial_gradient_brush();

			virtual pointer_type value() const override;

		protected:
			radial_gradient_pointer_type value_;
		};
	}
}

#endif /* !WINPP_DRAWING_RADIAL_GRADIENT_BRUSH_H */
