#pragma once

#ifndef WINPP_MESSAGE_HANDLER_H
#define WINPP_MESSAGE_HANDLER_H

#include "message_object.h"

namespace winpp{
	namespace messaging{
		class map;

		class handler{
		public:
			typedef messaging::object message_object_type;

			typedef messaging::mouse_activate mouse_activate_message_type;
			typedef messaging::ncactivate ncactivate_message_type;
			typedef messaging::activate activate_message_type;

			typedef structures::enumerations::mouse_activate_type mouse_activate_type;

			virtual ~handler();

		protected:
			virtual bool on_nccreate(message_object_type &e);

			virtual bool on_create(message_object_type &e);

			virtual void on_destroy(message_object_type &e);

			virtual void on_ncdestroy(message_object_type &e);

			virtual bool on_close(message_object_type &e);

			virtual mouse_activate_type on_mouse_activate(mouse_activate_message_type &e);

			virtual bool on_ncactivate(ncactivate_message_type &e);

			virtual void on_activate(activate_message_type &e);

			friend class map;
		};
	}
}

#endif /* !WINPP_MESSAGE_HANDLER_H */
