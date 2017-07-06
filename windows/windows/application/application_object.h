#pragma once

#ifndef WINPP_APPLICATION_OBJECT_H
#define WINPP_APPLICATION_OBJECT_H

#include <mutex>
#include <unordered_map>

#include "../common/exception.h"
#include "../threading/thread_message_loop.h"

#define WINPP_UUID	 "{DABED3E8-D8A5-48FC-B80B-B17C167FA9B0}"
#define WINPP_WUUID	L"{DABED3E8-D8A5-48FC-B80B-B17C167FA9B0}"

namespace winpp{
	namespace application{
		class object_manager;

		class object : public threading::message_loop{
		public:
			typedef object_manager object_manager_type;
			typedef std::shared_ptr<object_manager_type> object_manager_ptr_type;

			typedef threading::message_loop base_type;
			typedef threading::id::value_type dword_type;

			typedef std::unordered_map<dword_type, object *> list_type;

			typedef std::mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				exiting			= (1 << 0x0000),
			};

			object();

			virtual ~object();

			virtual int run() override;

			virtual void exit();

			virtual object_manager_type &object_manager();

			virtual bool is_exiting() const;

			static bool main_is_exiting();

			static object *find(dword_type id);

			static object *main_app;
			static thread_local object *current_app;

		protected:
			virtual bool is_filtered_message_() const override;

			virtual void dispatch_() override;

			virtual bool is_stopped_() const override;

			virtual bool is_dialog_message_();

			state_type states_;
			object_manager_ptr_type object_manager_;

			static dword_type process_id_;
			static uint_type message_id_;

			static list_type list_;
			static lock_type lock_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_APPLICATION_OBJECT_H */
