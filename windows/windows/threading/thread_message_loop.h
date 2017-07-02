#pragma once

#ifndef WINPP_THREAD_MESSAGE_LOOP_H
#define WINPP_THREAD_MESSAGE_LOOP_H

#include "thread_task_stack.h"
#include "thread_message_queue.h"

namespace winpp{
	namespace threading{
		class message_loop{
		public:
			typedef id id_type;
			typedef wrappers::msg msg_type;

			typedef msg_type::uint_type uint_type;
			typedef msg_type::wparam_type wparam_type;
			typedef msg_type::lparam_type lparam_type;

			typedef task_stack::callback_type task_type;

			struct status_info{
				bool is_posted;
				int return_value;
			};

			message_loop();

			virtual ~message_loop();

			virtual int run();

			template <typename arg_wparam_type = wparam_type, typename arg_lparam_type = lparam_type>
			bool post_message(uint_type msg, const arg_wparam_type &wparam, const arg_lparam_type &lparam) const{
				return (::PostThreadMessageW(id_, msg, (wparam_type)wparam, (lparam_type)lparam) != FALSE);
			}

			virtual int result() const;

			virtual const id_type &id() const;

			virtual void add_task(task_type task);

			template <typename return_type>
			return_type execute_task(std::function<return_type()> task){
				return execute_task_(std::integral_constant<bool, std::is_same<return_type, void>::value>(), task);
			}

			void execute_task(std::function<void()> task);

		protected:
			virtual bool is_filtered_message_() const;

			virtual void translate_() const;

			virtual void dispatch_();

			virtual void dispatch_thread_message_();

			virtual bool on_idle_(int index);

			virtual bool is_stopped_() const;

			template <typename return_type>
			return_type execute_task_(std::false_type, std::function<return_type()> task){
				return_type value;
				if (id_type::current() != id_){
					event_object<false> ready(L"");
					task_stack_.add([&]{
						value = task();
						ready.set();
					});

					queue_.wake();//Release message loop
					ready.wait();
				}
				else//Same thread
					value = task();

				return value;
			}

			template <typename return_type>
			return_type execute_task_(std::true_type, std::function<return_type()> task){
				if (id_type::current() != id_){
					event_object<false> ready(L"");
					task_stack_.add([&]{
						task();
						ready.set();
					});

					queue_.wake();//Release message loop
					ready.wait();
				}
				else//Same thread
					task();
			}

			id_type id_;
			message_queue queue_;
			task_stack task_stack_;
			msg_type cache_;
			status_info status_;
		};
	}
}

#endif /* !WINPP_THREAD_MESSAGE_LOOP_H */
