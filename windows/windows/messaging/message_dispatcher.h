#pragma once

#ifndef WINPP_MESSAGE_DISPATCHER_H
#define WINPP_MESSAGE_DISPATCHER_H

#include "message_target.h"

namespace winpp{
	namespace application{
		class object_manager;
	}

	namespace window{
		class object;
	}

	namespace messaging{
		class dispatcher{
		public:
			typedef ::LRESULT lresult_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::hwnd hwnd_type;

			typedef target target_type;

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) = 0;
		};

		template <class return_type, class message_object_type = object>
		class typed_dispatcher : public dispatcher{
		public:
			typedef return_type return_type;
			typedef message_object_type message_object_type;

			typedef return_type(target_type::*callback_type)(message_object_type &);

			explicit typed_dispatcher(callback_type callback)
				: callback_(callback){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override{
				return dispatch_<return_type>(info, is_sent, target);
			}

		protected:
			template <typename unused_type = return_type>
			std::enable_if_t<std::is_same_v<unused_type, void>, lresult_type> dispatch_(const msg_type &info, bool is_sent, target_type &target){
				call_(info, is_sent, target);
				return 0;
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<unused_type, void>, lresult_type> dispatch_(const msg_type &info, bool is_sent, target_type &target){
				return (lresult_type)call_(info, is_sent, target);
			}

			template <typename unused_type = return_type>
			std::enable_if_t<std::is_same_v<unused_type, void>, return_type> call_(const msg_type &info, bool is_sent, target_type &target, bool skip = false){
				message_object_type e(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
				call_(target, e);
				if (skip)//Skip default
					e.skip();
			}

			template <typename unused_type = return_type>
			std::enable_if_t<!std::is_same_v<unused_type, void>, return_type> call_(const msg_type &info, bool is_sent, target_type &target, bool skip = false){
				message_object_type e(info, is_sent, target.procedure(), reinterpret_cast<gui::object *>(&target));
				auto value = call_(target, e);
				if (skip)//Skip default
					e.skip();

				return value;
			}

			virtual return_type call_(target_type &target, message_object_type &e){
				return (target.*callback_)(e);
			}

			callback_type callback_;
		};

		class object_manager_handling_dispatcher : public dispatcher{
		public:
			typedef dispatcher base_type;

			typedef application::object_manager object_manager_type;
			typedef window::object window_type;

			typedef lresult_type(object_manager_type::*handler_type)(window_type &, const msg_type &);

			explicit object_manager_handling_dispatcher(handler_type handler);

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;

		protected:
			handler_type handler_;
		};

		class unrecognized_dispatcher : public typed_dispatcher<::LRESULT>{
		public:
			typedef typed_dispatcher<::LRESULT> base_type;

			template <typename... args_types>
			explicit unrecognized_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class nccreate_dispatcher : public typed_dispatcher<bool>{
		public:
			typedef typed_dispatcher<bool> base_type;

			template <typename... args_types>
			explicit nccreate_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class create_dispatcher : public typed_dispatcher<bool>{
		public:
			typedef typed_dispatcher<bool> base_type;

			template <typename... args_types>
			explicit create_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class destroy_dispatcher : public typed_dispatcher<void>{
		public:
			typedef typed_dispatcher<void> base_type;

			template <typename... args_types>
			explicit destroy_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class ncdestroy_dispatcher : public typed_dispatcher<void>{
		public:
			typedef typed_dispatcher<void> base_type;

			template <typename... args_types>
			explicit ncdestroy_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class close_dispatcher : public typed_dispatcher<bool>{
		public:
			typedef typed_dispatcher<bool> base_type;

			template <typename... args_types>
			explicit close_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class mouse_activate_dispatcher : public typed_dispatcher<structures::enumerations::mouse_activate_type, mouse_activate>{
		public:
			typedef typed_dispatcher<structures::enumerations::mouse_activate_type, mouse_activate> base_type;
			typedef structures::enumerations::mouse_activate_type mouse_activate_type;

			template <typename... args_types>
			explicit mouse_activate_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class ncactivate_dispatcher : public typed_dispatcher<bool, ncactivate>{
		public:
			typedef typed_dispatcher<bool, ncactivate> base_type;

			template <typename... args_types>
			explicit ncactivate_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class activate_dispatcher : public typed_dispatcher<void, activate>{
		public:
			typedef typed_dispatcher<void, activate> base_type;

			template <typename... args_types>
			explicit activate_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class child_activate_dispatcher : public typed_dispatcher<void>{
		public:
			typedef typed_dispatcher<void> base_type;

			template <typename... args_types>
			explicit child_activate_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class activate_app_dispatcher : public typed_dispatcher<void, ncactivate>{
		public:
			typedef typed_dispatcher<void, ncactivate> base_type;

			template <typename... args_types>
			explicit activate_app_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class cancel_mode_dispatcher : public typed_dispatcher<void>{
		public:
			typedef typed_dispatcher<void> base_type;

			template <typename... args_types>
			explicit cancel_mode_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class focus_change_dispatcher : public typed_dispatcher<void, focus_change>{
		public:
			typedef typed_dispatcher<void, focus_change> base_type;

			template <typename... args_types>
			explicit focus_change_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class enable_dispatcher : public typed_dispatcher<void, enable>{
		public:
			typedef typed_dispatcher<void, enable> base_type;

			template <typename... args_types>
			explicit enable_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class set_cursor_dispatcher : public typed_dispatcher<::HCURSOR, set_cursor>{
		public:
			typedef typed_dispatcher<::HCURSOR, set_cursor> base_type;
			typedef structures::enumerations::hit_target_type hit_target_type;

			template <typename... args_types>
			explicit set_cursor_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;

		protected:
			virtual ::HCURSOR retrieve_cursor_(const msg_type &info);
		};

		class hit_test_dispatcher : public typed_dispatcher<structures::enumerations::hit_target_type, hit_test>{
		public:
			typedef typed_dispatcher<structures::enumerations::hit_target_type, hit_test> base_type;
			typedef structures::enumerations::hit_target_type hit_target_type;

			template <typename... args_types>
			explicit hit_test_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class position_dispatcher : public typed_dispatcher<void, position>{
		public:
			typedef typed_dispatcher<void, position> base_type;

			template <typename... args_types>
			explicit position_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class size_dispatcher : public typed_dispatcher<bool, size>{
		public:
			typedef typed_dispatcher<bool, size> base_type;
			typedef structures::rect::value_type rect_value_type;

			template <typename... args_types>
			explicit size_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class move_dispatcher : public typed_dispatcher<bool, move>{
		public:
			typedef typed_dispatcher<bool, move> base_type;
			typedef structures::rect::value_type rect_value_type;

			template <typename... args_types>
			explicit move_dispatcher(args_types &&... args)
				: base_type(std::forward<args_types>(args)...){}

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;
		};

		class mouse_dispatcher : public dispatcher{
		public:
			typedef ::UINT uint_type;

			typedef dispatcher base_type;
			typedef structures::enumerations::hit_target_type hit_target_type;

			typedef events::tunnel<void, events::mouse> event_type;
			typedef void(target_type::*handler_type)(mouse &);

			struct info_type{
				uint_type code;
				event_type *event_object;
				handler_type handler;
			};

			virtual lresult_type dispatch(const msg_type &info, bool is_sent, target_type &target) override;

			static uint_type non_client_to_client(uint_type code);

			static uint_type client_to_non_client(uint_type code);

			static void retrieve_info(const msg_type &info, target_type &target, info_type &out);

			static void retrieve_event_and_handler(target_type &target, info_type &in_out);
		};
	}
}

#endif /* !WINPP_MESSAGE_DISPATCHER_H */
