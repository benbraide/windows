#pragma once

#ifndef WINPP_DRAWING_GRADIENT_STOP_COLLECTION_H
#define WINPP_DRAWING_GRADIENT_STOP_COLLECTION_H

#include <vector>

#include "drawer.h"
#include "../structures/color_structure.h"

namespace winpp{
	namespace drawing{
		class gradient_stop_collection{
		public:
			typedef ::UINT uint_type;
			typedef drawer drawer_type;

			typedef ::ID2D1GradientStopCollection value_type;
			typedef value_type *pointer_type;

			typedef ::D2D1_GRADIENT_STOP gradient_stop_type;
			typedef ::D2D1_GAMMA gamma_type;
			typedef ::D2D1_EXTEND_MODE extend_mode_type;

			typedef std::vector<gradient_stop_type> gradient_stop_list_type;
			typedef const gradient_stop_type *gradient_stop_const_ptr_type;

			typedef ::D2D1_COLOR_F colorf_type;
			typedef structures::color color_type;

			typedef std::vector<colorf_type> colorf_list_type;
			typedef std::vector<color_type> color_list_type;

			gradient_stop_collection(drawer_type &drawer, const gradient_stop_list_type &list, gamma_type gamma = gamma_type::D2D1_GAMMA_1_0,
				extend_mode_type extend_mode = extend_mode_type::D2D1_EXTEND_MODE_CLAMP);

			gradient_stop_collection(drawer_type &drawer, gradient_stop_const_ptr_type list, uint_type list_count, gamma_type gamma = gamma_type::D2D1_GAMMA_1_0,
				extend_mode_type extend_mode = extend_mode_type::D2D1_EXTEND_MODE_CLAMP);

			virtual ~gradient_stop_collection();

			gradient_stop_collection(const gradient_stop_collection &) = delete;

			gradient_stop_collection &operator =(const gradient_stop_collection &) = delete;

			virtual operator pointer_type() const;

			virtual pointer_type operator ->() const;

			virtual pointer_type value() const;

			static void create_uniform_gradient_stop_list(const colorf_list_type &color_list, gradient_stop_list_type &stop_list);

			static void create_uniform_gradient_stop_list(const color_list_type &color_list, gradient_stop_list_type &stop_list);

			static colorf_type to_colorf(const color_type &value);

		protected:
			pointer_type value_;
		};
	}
}

#endif /* !WINPP_DRAWING_GRADIENT_STOP_COLLECTION_H */
