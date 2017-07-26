#pragma once

#ifndef WINPP_MENU_RADIO_ITEM_H
#define WINPP_MENU_RADIO_ITEM_H

#include "menu_check_item.h"

namespace winpp{
	namespace menu{
		class radio_item : public check_item{
		public:
			radio_item();

			radio_item(gui_object_type &parent, const std::wstring &label, menu_state_type states = menu_state_type::nil);

			virtual ~radio_item();

			virtual radio_item &check();

			virtual radio_item &uncheck();
		};
	}
}

#endif /* !WINPP_MENU_RADIO_ITEM_H */
