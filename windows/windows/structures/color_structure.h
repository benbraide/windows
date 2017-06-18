#pragma once

#ifndef WINPP_COLOR_STRUCTURE_H
#define WINPP_COLOR_STRUCTURE_H

#include "enumerations.h"
#include "../wrappers/value_wrapper.h"

namespace winpp{
	namespace structures{
		template <class value_type>
		class basic_color : public wrappers::value<basic_color<value_type>, value_type>{
		public:
			typedef wrappers::value<basic_color<value_type>, value_type> base_type;

			typedef enumerations::named_color_type named_type;
			typedef enumerations::color_alpha_type alpha_type;

			typedef ::BYTE byte_type;

			basic_color()
				: byte_type(RGBA(0, 0, 0, 255)){}

			basic_color(value_type value)
				: byte_type(value){}

			basic_color(value_type rgb, byte_type alpha)
				: base_type(RGBA(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), alpha)){}

			basic_color(value_type rgb, alpha_type alpha)
				: base_type(RGBA(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb), static_cast<byte_type>(alpha))){}

			basic_color(byte_type red, byte_type green, byte_type blue, alpha_type alpha = alpha_type::opaque)
				: base_type(RGBA(red, green, blue, static_cast<byte_type>(alpha))){}

			basic_color(byte_type red, byte_type green, byte_type blue, byte_type alpha)
				: base_type(RGBA(red, green, blue, alpha)){}

			basic_color(float red, float green, float blue, float alpha)
				: base_type(RGBA(from_relative(red), from_relative(green), from_relative(blue), from_relative(alpha))){}

			basic_color(float red, float green, float blue, alpha_type alpha)
				: base_type(RGBA(from_relative(red), from_relative(green), from_relative(blue), static_cast<byte_type>(alpha))){}

			basic_color(named_type name, alpha_type alpha = alpha_type::opaque)
				: basic_color(static_cast<value_type>(name), alpha){}

			basic_color(named_type name, byte_type alpha)
				: basic_color(static_cast<value_type>(name), alpha){}

			bool operator ==(named_type rhs) const{
				return (RGB(GetRValue(base_type::value_), GetGValue(base_type::value_), GetBValue(base_type::value_)) == static_cast<value_type>(rhs));
			}

			bool operator !=(named_type rhs) const{
				return !(*this == rhs);
			}

			friend bool operator ==(named_type lhs, const basic_color &rhs){
				return (rhs == lhs);
			}

			friend bool operator !=(named_type lhs, const basic_color &rhs){
				return (rhs != lhs);
			}

			basic_color &red(byte_type value){
				base_type::value_ = RGBA(value, GetGValue(base_type::value_), GetBValue(base_type::value_), GetAValue(base_type::value_));
				return *this;
			}

			basic_color &red(float value){
				base_type::value_ = RGBA(from_relative(value), GetGValue(base_type::value_), GetBValue(base_type::value_), GetAValue(base_type::value_));
				return *this;
			}

			byte_type red() const{
				return GetRValue(base_type::value_);
			}

			float relative_red() const{
				return to_relative(GetRValue(base_type::value_));
			}

			basic_color &green(byte_type value){
				base_type::value_ = RGBA(GetRValue(base_type::value_), value, GetBValue(base_type::value_), GetAValue(base_type::value_));
				return *this;
			}

			basic_color &green(float value){
				base_type::value_ = RGBA(GetRValue(base_type::value_), from_relative(value), GetBValue(base_type::value_), GetAValue(base_type::value_));
				return *this;
			}

			byte_type green() const{
				return GetGValue(base_type::value_);
			}

			float relative_green() const{
				return to_relative(GetGValue(base_type::value_));
			}

			basic_color &blue(byte_type value){
				base_type::value_ = RGBA(GetRValue(base_type::value_), GetGValue(base_type::value_), value, GetAValue(base_type::value_));
				return *this;
			}

			basic_color &blue(float value){
				base_type::value_ = RGBA(GetRValue(base_type::value_), GetGValue(base_type::value_), from_relative(value), GetAValue(base_type::value_));
				return *this;
			}

			byte_type blue() const{
				return GetBValue(base_type::value_);
			}

			float relative_blue() const{
				return to_relative(GetBValue(base_type::value_));
			}

			basic_color &alpha(byte_type value, bool multiply = false){
				if (multiply){
					auto rgba = reinterpret_cast<byte_type *>(&base_type::value_);

					rgba[0] = ((rgba[0] * value) / static_cast<byte_type>(0xFF));
					rgba[1] = ((rgba[1] * value) / static_cast<byte_type>(0xFF));
					rgba[2] = ((rgba[2] * value) / static_cast<byte_type>(0xFF));
					rgba[3] = value;
				}
				else//Set alpha channel
					base_type::value_ = RGBA(GetRValue(base_type::value_), GetGValue(base_type::value_), GetBValue(base_type::value_), value);

				return *this;
			}

			basic_color &alpha(float value, bool multiply = false){
				return alpha(from_relative(value), multiply);
			}

			basic_color &alpha(alpha_type value, bool multiply = false){
				return alpha(static_cast<byte_type>(value), multiply);
			}

			byte_type alpha() const{
				return GetAValue(base_type::value_);
			}

			float relative_alpha() const{
				return to_relative(GetAValue(base_type::value_));
			}

			basic_color &name(named_type value){
				return rgb(static_cast<value_type>(value));
			}

			named_type name() const{
				return static_cast<named_type>(rgb());
			}

			basic_color &rgb(value_type value){
				base_type::value_ = RGBA(GetRValue(value), GetGValue(value), GetBValue(value), alpha());
				return *this;
			}

			basic_color &rgb(byte_type red, byte_type green, byte_type blue){
				base_type::value_ = RGBA(red, green, blue, alpha());
				return *this;
			}

			value_type rgb() const{
				return  RGB(GetRValue(base_type::value_), GetGValue(base_type::value_), GetBValue(base_type::value_));
			}

			value_type bgr() const{
				return  RGB(GetBValue(base_type::value_), GetGValue(base_type::value_), GetRValue(base_type::value_));
			}

			basic_color &rgba(value_type value){
				base_type::value_ = value;
				return *this;
			}

			basic_color &rgba(byte_type red, byte_type green, byte_type blue, byte_type alpha){
				base_type::value_ = RGBA(red, green, blue, alpha);
				return *this;
			}

			basic_color &rgba(byte_type red, byte_type green, byte_type blue, alpha_type alpha){
				base_type::value_ = RGBA(red, green, blue, static_cast<byte_type>(alpha));
				return *this;
			}

			basic_color &rgba(float red, float green, float blue, float alpha){
				base_type::value_ = RGBA(from_relative(red), from_relative(green), from_relative(blue), from_relative(alpha));
				return *this;
			}

			basic_color &rgba(float red, float green, float blue, alpha_type alpha){
				base_type::value_ = RGBA(from_relative(red), from_relative(green), from_relative(blue), static_cast<byte_type>(alpha));
				return *this;
			}

			value_type rgba() const{
				return base_type::value_;
			}

			bool is_transparent() const{
				return (GetAValue(base_type::value_) == static_cast<byte_type>(0));
			}

			bool is_opaque() const{
				return (GetAValue(base_type::value_) == static_cast<byte_type>(255));
			}

			bool is_named_alpha() const{
				return (is_opaque() || is_transparent());
			}

			static basic_color system(enumerations::system_color_index_type index, alpha_type alpha = alpha_type::opaque){
				return basic_color(static_cast<value_type>(::GetSysColor(static_cast<int>(index))), alpha);
			}

			static basic_color system(enumerations::system_color_index_type index, byte_type alpha){
				return basic_color(static_cast<value_type>(::GetSysColor(static_cast<int>(index))), alpha);
			}

			static byte_type from_relative(float value){
				return static_cast<byte_type>(value * 255);
			}

			static float to_relative(byte_type value){
				return (value / 255.0f);
			}
		};

		typedef basic_color<::COLORREF> color;
	}
}

#endif /* !WINPP_COLOR_STRUCTURE_H */
