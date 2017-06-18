#pragma once

#ifndef WINPP_COMMON_METHODS_H
#define WINPP_COMMON_METHODS_H

#include "../structures/enumerations.h"

namespace winpp{
	namespace application{
		class object;
	}

	namespace common{
		class methods{
		public:
			typedef application::object application_type;
			typedef structures::enumerations::placement_type placement_type;

			typedef ::D2D1_COLOR_F d2d_color_type;

			typedef ::D2D1_POINT_2F d2d_pointf_type;
			typedef ::D2D1_POINT_2U d2d_pointu_type;

			typedef ::D2D1_SIZE_F d2d_sizef_type;
			typedef ::D2D1_SIZE_U d2d_sizeu_type;

			typedef ::D2D1_RECT_F d2d_rectf_type;
			typedef ::D2D1_RECT_U d2d_rectu_type;

			static application_type *get_app();

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

			template <typename color_type>
			static d2d_color_type to_d2d_color(const color_type &value){
				return d2d_color_type{ value.relative_red(), value.relative_green(), value.relative_blue(), value.relative_alpha() };
			}

			template <typename point_type>
			static d2d_pointf_type to_d2d_pointf(const point_type &value){
				typedef decltype(d2d_pointf_type::x) field_type;
				return d2d_pointf_type{ static_cast<field_type>(value.x()), static_cast<field_type>(value.y()) };
			}

			template <typename point_type>
			static d2d_pointu_type to_d2d_pointu(const point_type &value){
				typedef decltype(d2d_pointu_type::x) field_type;
				return d2d_pointu_type{ static_cast<field_type>(value.x()), static_cast<field_type>(value.y()) };
			}

			template <typename size_type>
			static d2d_sizef_type to_d2d_sizef(const size_type &value){
				typedef decltype(d2d_sizef_type::width) field_type;
				return d2d_sizef_type{ static_cast<field_type>(value.width()), static_cast<field_type>(value.height()) };
			}

			template <typename size_type>
			static d2d_sizeu_type to_d2d_sizeu(const size_type &value){
				typedef decltype(d2d_sizeu_type::width) field_type;
				return d2d_sizeu_type{ static_cast<field_type>(value.width()), static_cast<field_type>(value.height()) };
			}

			template <typename rect_type>
			static d2d_rectf_type to_d2d_rectf(const rect_type &value){
				typedef decltype(d2d_rectf_type::left) field_type;
				return d2d_rectf_type{ static_cast<field_type>(value.left()), static_cast<field_type>(value.top()),
					static_cast<field_type>(value.right()), static_cast<field_type>(value.bottom()) };
			}

			template <typename rect_type>
			static d2d_rectu_type to_d2d_rectu(const rect_type &value){
				typedef decltype(d2d_rectu_type::left) field_type;
				return d2d_rectf_type{ static_cast<field_type>(value.left()), static_cast<field_type>(value.top()),
					static_cast<field_type>(value.right()), static_cast<field_type>(value.bottom()) };
			}

			template <typename value_type = int>
			static float pixel_to_dip(value_type value, float scale){
				return static_cast<float>(value / scale);
			}

			template <typename value_type = int>
			static value_type dip_to_pixel(float value, float scale){
				return static_cast<value_type>(value * scale);
			}

			static void pixel_scale(d2d_pointf_type &dpi);
		};
	}
}

#endif /* !WINPP_COMMON_METHODS_H */
