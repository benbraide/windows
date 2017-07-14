#pragma once

#ifndef WINPP_MESSAGE_TARGET_H
#define WINPP_MESSAGE_TARGET_H

#include <any>

#include "../wrappers/msg_wrapper.h"
#include "../wrappers/hwnd_wrapper.h"

#include "../events/event_tunnel.h"
#include "../gui/gui_object_tree.h"

#include "message_handler.h"

namespace winpp{
	namespace messaging{
		class target : public handler{
		public:
			typedef ::WNDPROC procedure_type;
			typedef ::LRESULT lresult_type;

			typedef wrappers::msg msg_type;
			typedef wrappers::hwnd hwnd_type;

			typedef events::object event_object_type;

			class event_tunnel : public gui::object_tree::event_tunnel{
			public:
				typedef gui::object_tree::event_tunnel base_type;

				typedef structures::enumerations::event_type event_type;
				typedef structures::enumerations::hit_target_type hit_target_type;

				using base_type::bind;

				virtual ~event_tunnel();

				virtual unsigned __int64 bind(event_type e, const std::any &callback) override;

				events::tunnel<lresult_type, events::dispatch_message> dispatch_message;
				events::tunnel<lresult_type, events::unrecognized_message> unrecognized_message;

				events::tunnel<void> pre_create;
				events::tunnel<void> post_destroy;

				events::tunnel<void, events::mouse_activate> mouse_activate;
				events::tunnel<void, events::pre_activate> pre_activate;
				events::tunnel<void, events::activate> activate;

				events::tunnel<void> child_activate;
				events::tunnel<void, events::pre_activate> activate_app;

				events::tunnel<void> cancel_mode;
				events::tunnel<void, events::focus_change> focus_change;
				events::tunnel<void, events::enable> enable;

				events::tunnel<::HCURSOR, events::set_cursor> set_cursor;
				events::tunnel<hit_target_type, events::hit_test> hit_test;

				events::tunnel<void, events::position> position;
				events::tunnel<void, events::size> size;
				events::tunnel<void, events::move> move;

				events::tunnel<void> close;

				events::tunnel<void> maximize;
				events::tunnel<void> minimize;
				events::tunnel<void> restore;

				events::tunnel<void> show;
				events::tunnel<void> hide;

				events::tunnel<void> erase_background;
				events::tunnel<void> paint;

			protected:
				virtual unsigned __int64 bind_(const std::wstring &e, const std::any &callback) override;
			};

			virtual ~target();

			virtual bool pre_translate(msg_type &msg);

			virtual lresult_type dispatch(const msg_type &msg, bool is_sent);

			virtual lresult_type unrecognized_message(message_object_type &e);

			virtual procedure_type procedure() const = 0;

			virtual event_tunnel &events() = 0;

		protected:
			virtual target *target_parent_() const = 0;

			virtual lresult_type call_default_(const msg_type &info);

			template <typename return_type>
			return_type default_(const msg_type &info){
				return (return_type)call_default_(info);
			}
		};
	}
}

#endif /* !WINPP_MESSAGE_TARGET_H */
