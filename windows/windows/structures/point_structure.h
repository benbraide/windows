#pragma once

#ifndef WINPP_POINT_STRUCTURE_H
#define WINPP_POINT_STRUCTURE_H

#include "../common/common_headers.h"
#include "../wrappers/value_wrapper.h"

namespace winpp{
	namespace structures{
		template <class value_type, class field_type>
		class basic_size;

		template <class value_type>
		struct default_point_native_value{
			value_type x;
			value_type y;
		};

		template <class value_type, class field_type = decltype(value_type::x)>
		class basic_point : public wrappers::value<basic_point<value_type, field_type>, value_type>{
		public:
			typedef wrappers::value<basic_point<value_type, field_type>, value_type> base_type;

			typedef value_type value_type;
			typedef field_type field_type;

			template <typename size_value_type>
			using size_type = basic_size<size_value_type, field_type>;

			basic_point()
				: base_type(value_type{}){}

			basic_point(field_type x, field_type y)
				: base_type(value_type{ x, y }){}

			basic_point(field_type xy)
				: base_type(value_type{ xy, xy }){}

			basic_point(const value_type &value)
				: base_type(value){}

			basic_point(const basic_point &value)
				: base_type(value.value_){}

			basic_point(basic_point &&value)
				: base_type(value.value_){
				value.value_ = {};
			}

			template <typename size_value_type>
			basic_point(const size_type<size_value_type> &value)
				: basic_point(value.width(), value.height()){}

			basic_point &operator =(field_type xy){
				base_type::value_ = { xy, xy };
				return *this;
			}

			basic_point &operator =(const value_type &value){
				base_type::value_ = value;
				return *this;
			}

			basic_point &operator =(const basic_point &value){
				base_type::value_ = value.value_;
				return *this;
			}

			basic_point &operator =(basic_point &&value){
				base_type::value_ = value.value_;
				value.value_ = {};
				return *this;
			}

			template <typename size_value_type>
			basic_point &operator =(const size_type<size_value_type> &value){
				base_type::value_ = { value.width(), value.height() };
				return *this;
			}

			basic_point operator +() const{
				return *this;
			}

			basic_point operator -() const{
				return basic_point{ -base_type::value_.x, -base_type::value_.y };
			}

			basic_point operator +(const basic_point &rhs) const{
				return basic_point{ base_type::value_.x + rhs.value_.x, base_type::value_.y + rhs.value_.y };
			}

			basic_point operator -(const basic_point &rhs) const{
				return basic_point{ base_type::value_.x - rhs.value_.x, base_type::value_.y - rhs.value_.y };
			}

			template <typename size_value_type>
			basic_point operator +(const size_type<size_value_type> &rhs) const{
				return basic_point{ base_type::value_.x + rhs.width(), base_type::value_.y + rhs.height() };
			}

			template <typename size_value_type>
			basic_point operator -(const size_type<size_value_type> &rhs) const{
				return basic_point{ base_type::value_.x - rhs.width(), base_type::value_.y - rhs.height() };
			}

			basic_point &operator +=(const basic_point &rhs){
				return (*this = *this + rhs);
			}

			basic_point &operator -=(const basic_point &rhs){
				return (*this = *this - rhs);
			}

			template <typename size_value_type>
			basic_point &operator +=(const size_type<size_value_type> &rhs){
				return (*this = *this + rhs);
			}

			template <typename size_value_type>
			basic_point &operator -=(const size_type<size_value_type> &rhs){
				return (*this = *this - rhs);
			}

			basic_point &operator ()(field_type xy){
				return (*this = xy);
			}

			basic_point &operator ()(field_type x, field_type y){
				return (*this = value_type{ x, y });
			}

			basic_point &operator ()(const value_type &value){
				return (*this = value);
			}

			basic_point &operator ()(const basic_point &value){
				return (*this = value);
			}

			basic_point &operator ()(basic_point &&value){
				return (*this = std::move(value));
			}

			template <typename size_value_type>
			basic_point &operator ()(const size_type<size_value_type> &value){
				return (*this = value);
			}

			basic_point &x(field_type value){
				base_type::value_.x = value;
				return *this;
			}

			field_type x() const{
				return base_type::value_.x;
			}

			basic_point &y(field_type value){
				base_type::value_.y = value;
				return *this;
			}

			field_type y() const{
				return base_type::value_.y;
			}

			int compare(const_qualified_type value) const{
				if (base_type::value_.y != value.y)
					return (base_type::value_.y < value.y) ? -1 : 1;

				if (base_type::value_.x != value.x)
					return (base_type::value_.x < value.x) ? -1 : 1;

				return 0;
			}
		};

		typedef basic_point<::POINT> point;
		typedef basic_point<default_point_native_value<float>, float> pointf;
	}
}

#endif /* !WINPP_POINT_STRUCTURE_H */
