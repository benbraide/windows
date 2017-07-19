#pragma once

#ifndef WINPP_APPLICATION_OBJECT_H
#define WINPP_APPLICATION_OBJECT_H

#include <mutex>
#include <memory>
#include <unordered_map>

#include "../common/exception.h"
#include "../wrappers/hwnd_wrapper.h"
#include "../threading/thread_message_loop.h"

#include "../drawing/drawing_factory.h"
#include "../drawing/hdc_drawer.h"

#define WINPP_UUID	 "{DABED3E8-D8A5-48FC-B80B-B17C167FA9B0}"
#define WINPP_WUUID	L"{DABED3E8-D8A5-48FC-B80B-B17C167FA9B0}"

namespace winpp{
	namespace gui{
		class object;
	}

	namespace application{
		class object_manager;

		class object : public threading::message_loop{
		public:
			typedef ::HRESULT hresult_type;

			typedef object_manager object_manager_type;
			typedef std::shared_ptr<object_manager_type> object_manager_ptr_type;

			typedef wrappers::hwnd hwnd_type;
			typedef gui::object gui_object_type;

			typedef threading::message_loop base_type;
			typedef threading::id::value_type dword_type;

			typedef drawing::factory factory_type;
			typedef drawing::hdc_drawer hdc_drawer_type;
			typedef std::shared_ptr<hdc_drawer_type> hdc_drawer_ptr_type;

			typedef std::shared_ptr<object> object_ptr_type;
			typedef std::unordered_map<dword_type, object_ptr_type> list_type;

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

			virtual factory_type &drawing_factory();

			virtual hdc_drawer_type &drawer();

			virtual void drawing_result(hresult_type result);

			virtual bool is_exiting() const;

			static bool main_is_exiting();

			static object &get();

			static object &get(dword_type id);

			static object *find(dword_type id);

			static int run_app();

			static dword_type main_thread;
			static object *main_app;
			static thread_local object *current_app;

		protected:
			virtual bool is_filtered_message_() const override;

			virtual void dispatch_() override;

			virtual bool is_stopped_() const override;

			virtual bool is_pre_processed_();

			state_type states_;
			object_manager_ptr_type object_manager_;
			factory_type factory_;
			hdc_drawer_ptr_type drawer_;

			static list_type list_;
			static lock_type lock_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_APPLICATION_OBJECT_H */
