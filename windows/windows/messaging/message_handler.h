#pragma once

#ifndef WINPP_MESSAGE_HANDLER_H
#define WINPP_MESSAGE_HANDLER_H

#include "message_object.h"

namespace winpp{
	namespace messaging{
		class handler{
		public:
			typedef messaging::object message_object_type;

			virtual ~handler();

			virtual bool on_nccreate(const message_object_type &e);

			virtual bool on_create(const message_object_type &e);

			virtual void on_destroy(const message_object_type &e);

			virtual void on_ncdestroy(const message_object_type &e);

			virtual bool on_close(const message_object_type &e);
		};
	}
}

#endif /* !WINPP_MESSAGE_HANDLER_H */
