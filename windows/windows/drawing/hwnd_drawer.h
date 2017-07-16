#pragma once

#ifndef WINPP_HWND_DRAWER_H
#define WINPP_HWND_DRAWER_H

#include "drawer.h"
#include "../structures/rect_structure.h"

namespace winpp{
	namespace drawing{
		class hwnd_drawer : public drawer{
		public:
			typedef ::HWND hwnd_type;
			typedef ::UINT32 uint32_type;

			typedef ::ID2D1HwndRenderTarget hwnd_value_type;
			typedef hwnd_value_type *hwnd_pointer_type;

			typedef structures::rect rect_type;

			hwnd_drawer(factory_type &factory, hwnd_type value);

			virtual ~hwnd_drawer();

			virtual pointer_type value() const override;

			virtual bool is_occluded() const;

		protected:
			hwnd_pointer_type value_;
		};
	}
}

#endif /* !WINPP_HWND_DRAWER_H */
