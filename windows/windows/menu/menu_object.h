#pragma once

#ifndef WINPP_MENU_OBJECT_H
#define WINPP_MENU_OBJECT_H

#include "menu_newline.h"
#include "../gui/gui_object_tree.h"

namespace winpp{
	namespace menu{
		class object : public gui::object_tree{
		public:
			typedef gui::object_tree base_type;

			typedef ::HMENU hmenu_type;

			virtual operator hmenu_type() const;

			virtual bool is_popup() const;
		};
	}
}

#endif /* !WINPP_MENU_OBJECT_H */
