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

			object();

			virtual ~object();

			static object *main_app;
			static thread_local object *current_app;

		protected:
			virtual bool is_filtered_message_() const override;

			virtual void dispatch_() override;

			virtual bool is_stopped_() const override;

			virtual bool is_dialog_message_();
		};
	}
}

#endif /* !WINPP_APPLICATION_OBJECT_H */
