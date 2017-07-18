#pragma once

#ifndef WINPP_DRAWING_FACTORY_H
#define WINPP_DRAWING_FACTORY_H

#include "../common/common_headers.h"

namespace winpp{
	namespace drawing{
		template <typename interface_type>
		using com_ptr = ::Microsoft::WRL::ComPtr<interface_type>;

		class factory{
		public:
			typedef ::D2D1_FACTORY_TYPE factory_type;
			typedef ::ID2D1Factory value_type;
			typedef value_type *pointer_type;

			explicit factory(factory_type type = factory_type::D2D1_FACTORY_TYPE_SINGLE_THREADED);

			~factory();

			factory(const factory &) = delete;
			
			factory &operator =(const factory &) = delete;

			operator pointer_type() const;

			pointer_type operator ->() const;

			pointer_type value() const;

		protected:
			pointer_type value_;
		};
	}
}

#endif /* !WINPP_DRAWING_FACTORY_H */
