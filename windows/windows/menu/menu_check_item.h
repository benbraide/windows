#pragma once

#ifndef WINPP_MENU_CHECK_ITEM_H
#define WINPP_MENU_CHECK_ITEM_H

#include "menu_item.h"

namespace winpp{
	namespace menu{
		class check_item : public item{
		public:
			check_item();

			check_item(gui_object_type &parent, const std::wstring &label, menu_state_type states = menu_state_type::nil);

			virtual ~check_item();

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual item &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) override;
		};
	}
}

#endif /* !WINPP_MENU_CHECK_ITEM_H */
