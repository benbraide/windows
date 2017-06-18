#pragma once

#ifndef WINPP_SIZE_STRUCTURE_H
#define WINPP_SIZE_STRUCTURE_H

#include "../common/common_headers.h"
#include "../wrappers/value_wrapper.h"

namespace winpp{
	namespace structures{
		template <class value_type, class field_type>
		class basic_point;

		template <class value_type>
		struct default_size_native_value{
			value_type cx;
			value_type cy;
		};

		template <class value_type, class field_type = decltype(value_type::cx)>
		class basic_size : public wrappers::value<basic_size<value_type, field_type>, value_type>{
		public:
			typedef wrappers::value<basic_size<value_type, field_type>, value_type> base_type;

			typedef value_type value_type;
			typedef field_type field_type;

			template <typename point_value_type>
			using point_type = basic_point<point_value_type, field_type>;

			basic_size()
				: base_type(value_type{}){}

			basic_size(field_type x, field_type y)
				: base_type(value_type{ x, y }){}

			basic_size(field_type xy)
				: base_type(value_type{ xy, xy }){}

			basic_size(const value_type &value)
				: base_type(value){}

			basic_size(const basic_size &value)
				: base_type(value.value_){}

			basic_size(basic_size &&value)
				: base_type(value.value_){
				value.value_ = {};
			}

			template <typename point_value_type>
			basic_size(const point_type<point_value_type> &value)
				: basic_size(value.x(), value.y()){}

			basic_size &operator =(field_type xy){
				base_type::value_ = { xy, xy };
				return *this;
			}

			basic_size &operator =(const value_type &value){
				base_type::value_ = value;
				return *this;
			}

			basic_size &operator =(const basic_size &value){
				base_type::value_ = value.value_;
				return *this;
			}

			basic_size &operator =(basic_size &&value){
				base_type::value_ = value.value_;
				value.value_ = {};
				return *this;
			}

			template <typename point_value_type>
			basic_size &operator =(const point_type<point_value_type> &value){
				base_type::value_ = { value.x(), value.y() };
				return *this;
			}

			basic_size operator +() const{
				return *this;
			}

			basic_size operator -() const{
				return basic_size{ -base_type::value_.cx, -base_type::value_.cy };
			}

			basic_size operator +(const basic_size &rhs) const{
				return basic_size{ base_type::value_.cx + rhs.value_.cx, base_type::value_.cy + rhs.value_.cy };
			}

			basic_size operator -(const basic_size &rhs) const{
				return basic_size{ base_type::value_.cx - rhs.value_.cx, base_type::value_.cy - rhs.value_.cy };
			}

			template <typename point_value_type>
			basic_size operator +(const point_type<point_value_type> &rhs) const{
				return basic_size{ base_type::value_.cx + rhs.x(), base_type::value_.cy + rhs.y() };
			}

			template <typename point_value_type>
			basic_size operator -(const point_type<point_value_type> &rhs) const{
				return basic_size{ base_type::value_.cx - rhs.x(), base_type::value_.cy - rhs.y() };
			}

			basic_size &operator +=(const basic_size &rhs){
				return (*this = *this + rhs);
			}

			basic_size &operator -=(const basic_size &rhs){
				return (*this = *this - rhs);
			}

			template <typename point_value_type>
			basic_size &operator +=(const point_type<point_value_type> &rhs){
				return (*this = *this + rhs);
			}

			template <typename point_value_type>
			basic_size &operator -=(const point_type<point_value_type> &rhs){
				return (*this = *this - rhs);
			}

			basic_size &operator ()(field_type xy){
				return (*this = xy);
			}

			basic_size &operator ()(field_type x, field_type y){
				return (*this = value_type{ x, y });
			}

			basic_size &operator ()(const value_type &value){
				return (*this = value);
			}

			basic_size &operator ()(const basic_size &value){
				return (*this = value);
			}

			basic_size &operator ()(basic_size &&value){
				return (*this = std::move(value));
			}

			template <typename point_value_type>
			basic_size &operator ()(const point_type<point_value_type> &value){
				return (*this = value);
			}

			basic_size &width(field_type value){
				base_type::value_.cx = value;
				return *this;
			}

			field_type width() const{
				return base_type::value_.cx;
			}

			basic_size &height(field_type value){
				base_type::value_.cy = value;
				return *this;
			}

			field_type height() const{
				return base_type::value_.cy;
			}

			int compare(const_qualified_type value) const{
				if (base_type::value_.cy != value.height)
					return (base_type::value_.cy < value.height) ? -1 : 1;

				if (base_type::value_.cx != value.width)
					return (base_type::value_.cx < value.width) ? -1 : 1;

				return 0;
			}
		};

		typedef basic_size<::SIZE> size;
		typedef basic_size<default_size_native_value<float>, float> sizef;
	}
}

#endif /* !WINPP_SIZE_STRUCTURE_H */
