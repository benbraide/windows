#pragma once

#ifndef WINPP_MENU_GROUP_H
#define WINPP_MENU_GROUP_H

#include "menu_item.h"
#include "../gui/gui_object_group.h"

namespace winpp{
	namespace menu{
		class group : public gui::object_group{
		public:
			typedef gui::object_group base_type;

			group();

			group(gui_object_type &parent);

			virtual ~group();

			virtual void *handle() const override;

			virtual group &destroy(force_type force = force_type::dont_force) override;

			virtual unsigned int object_group() const override;

			virtual bool is_created() const override;

			virtual bool is_menu_group() const override;

			virtual void create(gui_object_type &parent);
		};
	}
}

#endif /* !WINPP_MENU_GROUP_H */
