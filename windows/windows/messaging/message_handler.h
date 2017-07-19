#pragma once

#ifndef WINPP_MESSAGE_HANDLER_H
#define WINPP_MESSAGE_HANDLER_H

#include "message_object.h"

namespace winpp{
	namespace messaging{
		class map;
		class mouse_dispatcher;
		class key_dispatcher;

		class handler{
		public:
			typedef ::UINT uint_type;
			typedef ::HCURSOR hcursor_type;

			typedef messaging::object message_object_type;

			typedef messaging::mouse_activate mouse_activate_message_type;
			typedef messaging::ncactivate ncactivate_message_type;
			typedef messaging::activate activate_message_type;

			typedef messaging::focus_change focus_change_message_type;
			typedef messaging::enable enable_message_type;

			typedef messaging::set_cursor set_cursor_message_type;
			typedef messaging::hit_test hit_test_message_type;

			typedef messaging::position position_message_type;
			typedef messaging::size size_message_type;
			typedef messaging::move move_message_type;

			typedef messaging::draw draw_message_type;

			typedef messaging::mouse mouse_message_type;
			typedef messaging::key key_message_type;

			typedef structures::enumerations::mouse_activate_type mouse_activate_type;
			typedef structures::enumerations::hit_target_type hit_target_type;

			virtual ~handler();

			virtual handler *parent_handler() const;

		protected:
			virtual bool on_nccreate(message_object_type &e);

			virtual bool on_create(message_object_type &e);

			virtual void on_destroy(message_object_type &e);

			virtual void on_ncdestroy(message_object_type &e);

			virtual bool on_close(message_object_type &e);

			virtual mouse_activate_type on_mouse_activate(mouse_activate_message_type &e);

			virtual bool on_ncactivate(ncactivate_message_type &e);

			virtual void on_activate(activate_message_type &e);

			virtual void on_child_activate(message_object_type &e);

			virtual void on_activate_app(ncactivate_message_type &e);

			virtual void on_cancel_mode(message_object_type &e);

			virtual void on_focus_change(focus_change_message_type &e);

			virtual void on_enable(enable_message_type &e);

			virtual hcursor_type on_set_cursor(set_cursor_message_type &e);

			virtual hit_target_type on_hit_test(hit_test_message_type &e);

			virtual void on_position(position_message_type &e);

			virtual bool on_size(size_message_type &e);

			virtual bool on_move(move_message_type &e);

			virtual bool on_erase_background(draw_message_type &e);

			virtual void on_paint(draw_message_type &e);

			virtual void on_mouse_move(mouse_message_type &e);

			virtual void on_mouse_hover(mouse_message_type &e);

			virtual void on_mouse_enter(mouse_message_type &e);

			virtual void on_mouse_leave(mouse_message_type &e);

			virtual void on_mouse_down(mouse_message_type &e);

			virtual void on_mouse_up(mouse_message_type &e);

			virtual void on_mouse_dbl_click(mouse_message_type &e);

			virtual void on_mouse_drag(mouse_message_type &e);

			virtual void on_mouse_drag_end(mouse_message_type &e);

			virtual void on_mouse_wheel(mouse_message_type &e);

			virtual void on_key_down(key_message_type &e);

			virtual void on_key_up(key_message_type &e);

			virtual void on_key_pressed(key_message_type &e);

			virtual void on_dead_key(key_message_type &e);

			virtual void bubble_mouse(mouse_message_type &e);

			virtual void bubble_key(key_message_type &e);

			friend class map;
			friend class mouse_dispatcher;
			friend class key_dispatcher;
		};
	}
}

#endif /* !WINPP_MESSAGE_HANDLER_H */
