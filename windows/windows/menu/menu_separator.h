#pragma once

#ifndef WINPP_MENU_SEPARATOR_H
#define WINPP_MENU_SEPARATOR_H

#include "menu_item.h"

namespace winpp{
	namespace menu{
		class separator : public item{
		public:
			separator();

			explicit separator(gui_object_type &parent);

			virtual ~separator();

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual item &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) override;

			virtual void create(gui_object_type &parent, const std::wstring &label, menu_state_type states = menu_state_type::nil) override;

			virtual void create(gui_object_type &parent);

			virtual separator &states(menu_state_type value) override;

			virtual bool is_action() const override;

			virtual bool is_separator() const override;

		protected:
			virtual uint_type get_types_() const override;
		};
	}
}

#endif /* !WINPP_MENU_SEPARATOR_H */
