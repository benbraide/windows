#pragma once

#ifndef WINPP_APPLICATION_OBJECT_H
#define WINPP_APPLICATION_OBJECT_H

#include "../common/exception.h"
#include "../threading/thread_message_loop.h"

namespace winpp{
	namespace application{
		class object : public threading::message_loop{
		public:
			typedef threading::message_loop base_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				exiting			= (1 << 0x0000),
			};

			object();

			virtual ~object();

			virtual int run() override;

			virtual void exit();

			virtual bool is_exiting() const;

			static bool main_is_exiting();

			static object *main_app;
			static thread_local object *current_app;

		protected:
			virtual bool is_filtered_message_() const override;

			virtual void dispatch_() override;

			virtual bool is_stopped_() const override;

			virtual bool is_dialog_message_();

			state_type states_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_APPLICATION_OBJECT_H */
