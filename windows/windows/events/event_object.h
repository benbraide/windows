#pragma once

#ifndef WINPP_EVENT_OBJECT_H
#define WINPP_EVENT_OBJECT_H

#include "../gui/gui_object.h"

namespace winpp{
	namespace events{
		class object{
		public:
			typedef gui::object gui_object_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				prevented		= (1 << 0x0000),
				stopped			= (1 << 0x0001),
			};

			explicit object(gui_object_type &target);

			virtual object &prevent();

			virtual object &stop_propagation();

			virtual bool is_prevented() const;

			virtual bool is_propagating() const;

			virtual gui_object_type &target() const;

		protected:
			gui_object_type *target_;
			state_type states_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_EVENT_OBJECT_H */
