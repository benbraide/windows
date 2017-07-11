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

			typedef messaging::ncactivate ncactivate_message_type;
			typedef messaging::activate activate_message_type;

			virtual ~handler();

		protected:
			virtual bool on_nccreate(const message_object_type &e);

			virtual bool on_create(const message_object_type &e);

			virtual void on_destroy(const message_object_type &e);

			virtual void on_ncdestroy(const message_object_type &e);

			virtual bool on_close(const message_object_type &e);

			virtual bool on_ncactivate(const ncactivate_message_type &e);

			virtual void on_activate(const activate_message_type &e);

			friend class map;
		};
	}
}

#endif /* !WINPP_MESSAGE_HANDLER_H */
