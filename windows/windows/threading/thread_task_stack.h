#pragma once

#ifndef WINPP_THREAD_TASK_STACK_H
#define WINPP_THREAD_TASK_STACK_H

#include <list>
#include <functional>

#include "thread_id.h"

namespace winpp{
	namespace threading{
		class task_stack{
		public:
			typedef threading::id id_type;

			typedef std::function<void()> callback_type;
			typedef std::list<callback_type> callback_list_type;

			explicit task_stack(id_type id = id_type::current());

			void add(callback_type task);

			void execute();

			id_type id() const;

			bool empty() const;

		private:
			id_type id_;
			callback_list_type callback_list_;
		};
	}
}

#endif /* !WINPP_THREAD_TASK_STACK_H */
