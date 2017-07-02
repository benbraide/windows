#pragma once

#ifndef WINPP_COMMON_METHODS_H
#define WINPP_COMMON_METHODS_H

#include <string>
#include <functional>

#include "../structures/enumerations.h"

namespace winpp{
	namespace application{
		class object;
	}

	namespace window{
		class object;
	}

	namespace common{
		template <class type>
		struct type_tree{};

		template <>
		struct type_tree<unsigned __int8>{
			typedef unsigned __int16 bigger;
			static const unsigned __int64 mask = 0;
		};

		template <>
		struct type_tree<unsigned __int16>{
			typedef unsigned __int8 smaller;
			typedef unsigned __int32 bigger;
			static const unsigned __int64 mask = 0;
		};

		template <>
		struct type_tree<unsigned __int32>{
			typedef unsigned __int16 smaller;
			typedef unsigned __int64 bigger;
			static const unsigned __int64 mask = 0xffff0000u;
		};

		template <>
		struct type_tree<unsigned __int64>{
			typedef unsigned __int32 smaller;
			static const unsigned __int64 mask = 0xffffffff00000000ull;
		};

		class methods{
		public:
			typedef ::WNDPROC procedure_type;

			typedef application::object application_type;
			typedef window::object window_type;

			typedef structures::enumerations::placement_type placement_type;

			typedef ::D2D1_COLOR_F d2d_color_type;

			typedef ::D2D1_POINT_2F d2d_pointf_type;
			typedef ::D2D1_POINT_2U d2d_pointu_type;

			typedef ::D2D1_SIZE_F d2d_sizef_type;
			typedef ::D2D1_SIZE_U d2d_sizeu_type;

			typedef ::D2D1_RECT_F d2d_rectf_type;
			typedef ::D2D1_RECT_U d2d_rectu_type;

			typedef ::HWND hwnd_type;
			typedef ::CREATESTRUCTW create_info_type;

			typedef ::DWORD dword_type;
			typedef ::UUID uuid_type;

			static application_type *get_app();

			static hwnd_type create_window(const create_info_type &info, procedure_type *previous_procedure, application_type *&app);

			static std::string uuid();

			static std::wstring wuuid();

			static std::wstring convert_string(const std::string &value);

			template <typename string_type>
			static void format_string(string_type &value, std::function<bool(typename string_type::traits_type::char_type &)> formatter){
				for (auto &c : value){
					if (!formatter(c))
						break;
				}
			}

			static void to_upper(std::wstring &value);

			static void to_upper(std::string &value);

			static void to_lower(std::wstring &value);

			static void to_lower(std::string &value);

			static bool has_left(placement_type placement);

			static bool has_top(placement_type placement);

			static bool has_right(placement_type placement);

			static bool has_bottom(placement_type placement);

			static bool has_horizontal_center(placement_type placement);

			static bool has_vertical_center(placement_type placement);

			template <typename size_type>
			static size_type compute_placement_delta(const size_type &parent_size, const size_type &target_size, placement_type placement){
				size_type delta;
				if (has_right(placement))
					delta.width(parent_size.width() - target_size.width());
				else if (has_horizontal_center(placement))
					delta.width((parent_size.width() - target_size.width()) / 2);

				if (has_bottom(placement))
					delta.height(parent_size.height() - target_size.height());
				else if (has_vertical_center(placement))
					delta.height((parent_size.height() - target_size.height()) / 2);

				return delta;
			}

			template <typename rect_type, typename size_type>
			static void apply_placement(placement_type placement, rect_type &rect, const size_type &size){
				if (has_right(placement)){
					rect.right = rect.left;
					rect.left -= size.width();
				}
				else if (has_horizontal_center(placement)){
					rect.left += (size.width() / 2);
					rect.right -= (size.width() / 2);
				}

				if (has_bottom(placement)){
					rect.bottom = rect.top;
					rect.top -= size.height();
				}
				else if (has_vertical_center(placement)){
					rect.top += (size.height() / 2);
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

			template <typename object_type>
			static auto hwnd_owner(hwnd_type value, object_type *object){
				return ::SetWindowLongPtrW(value, static_cast<int>(structures::enumerations::data_index_type::user_data), reinterpret_cast<::LONG_PTR>((typename object_type::gui_object_type *)object));
			}

			template <typename object_type>
			static object_type *hwnd_owner(hwnd_type value){
				return (object_type *)reinterpret_cast<typename object_type::gui_object_type *>(::GetWindowLongPtrW(value, static_cast<int>(structures::enumerations::data_index_type::user_data)));
			}

			template <typename value_type>
			static auto combine(value_type low, value_type high){
				typedef typename type_tree<value_type>::bigger bigger_type;
				return ((static_cast<bigger_type>(low) << static_cast<bigger_type>(sizeof value_type)) | static_cast<bigger_type>(high));
			}

			template <typename value_type>
			static auto low(value_type value){
				typedef typename type_tree<value_type>::smaller smaller_type;
				return static_cast<smaller_type>(value);
			}

			template <typename value_type>
			static auto high(value_type value){
				typedef typename type_tree<value_type>::smaller smaller_type;
				return static_cast<smaller_type>((value & value_type::mask) >> static_cast<value_type>(sizeof value_type));
			}
		};
	}
}

#endif /* !WINPP_COMMON_METHODS_H */
