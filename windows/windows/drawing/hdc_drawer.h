#pragma once

#ifndef WINPP_HDC_DRAWER_H
#define WINPP_HDC_DRAWER_H

#include "drawer.h"
#include "../structures/rect_structure.h"

namespace winpp{
	namespace drawing{
		class hdc_drawer : public drawer{
		public:
			typedef ::HDC hdc_type;

			typedef ::ID2D1DCRenderTarget hdc_value_type;
			typedef hdc_value_type *hdc_pointer_type;

			typedef structures::rect rect_type;

			explicit hdc_drawer(factory_type &factory);

			virtual ~hdc_drawer();

			virtual pointer_type value() const override;

			virtual bool bind(hdc_type value, const rect_type &rect);

		protected:
			hdc_pointer_type value_;
		};
	}
}

#endif /* !WINPP_HDC_DRAWER_H */
