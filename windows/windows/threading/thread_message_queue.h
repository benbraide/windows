#pragma once

#ifndef WINPP_THREAD_MESSAGE_QUEUE_H
#define WINPP_THREAD_MESSAGE_QUEUE_H

#include "thread_event_object.h"
#include "../wrappers/msg_wrapper.h"

namespace winpp{
	namespace threading{
		class message_queue{
		public:
			typedef structures::enumerations::msg_peek_type peek_type;
			typedef structures::enumerations::msg_sent_type sent_type;

			typedef structures::point point_type;
			typedef wrappers::msg msg_type;

			typedef ::UINT uint_type;
			typedef ::DWORD dword_type;
			typedef ::LRESULT result_type;

			typedef ::HWND hwnd_type;
			typedef event_object<false> event_object_type;

			message_queue();

			bool get(msg_type &msg);

			bool peek(msg_type &msg, peek_type type = peek_type::nil);

			bool wait(dword_type timeout = INFINITE);

			bool wake();

			static point_type last_mouse_position();

			static long last_time();

			static bool is_sent();

			static sent_type sent_info();

			static bool reply(result_type value);

			static void post_quit(int exit_code = 0);

		private:
			event_object<false> event_;
		};
	}
}

#endif /* !WINPP_THREAD_MESSAGE_QUEUE_H */
