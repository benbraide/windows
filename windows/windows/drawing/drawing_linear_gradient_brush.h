#pragma once

#ifndef WINPP_DRAWING_LINEAR_GRADIENT_BRUSH_H
#define WINPP_DRAWING_LINEAR_GRADIENT_BRUSH_H

#include "drawing_brush.h"
#include "drawing_gradient_stop_collection.h"

namespace winpp{
	namespace drawing{
		class linear_gradient_brush : public brush{
		public:
			typedef ::ID2D1LinearGradientBrush linear_gradient_value_type;
			typedef linear_gradient_value_type *linear_gradient_pointer_type;

			typedef ::D2D1_LINEAR_GRADIENT_BRUSH_PROPERTIES properties_type;

			linear_gradient_brush(drawer_type &drawer, const properties_type &properties, gradient_stop_collection &stop_collection);

			virtual ~linear_gradient_brush();

			virtual pointer_type value() const override;

		protected:
			linear_gradient_pointer_type value_;
		};
	}
}

#endif /* !WINPP_DRAWING_LINEAR_GRADIENT_BRUSH_H */
