#pragma once

#ifndef WINPP_DRAWER_H
#define WINPP_DRAWER_H

#include "drawing_factory.h"

namespace winpp{
	namespace drawing{
		class drawer{
		public:
			typedef factory factory_type;

			typedef ::ID2D1RenderTarget value_type;
			typedef value_type *pointer_type;

			drawer();

			virtual ~drawer();

			drawer(const drawer &) = delete;
			
			drawer &operator =(const drawer &) = delete;

			virtual operator pointer_type() const;

			virtual pointer_type operator ->() const;

			virtual pointer_type value() const = 0;
		};
	}
}

#endif /* !WINPP_DRAWER_H */
