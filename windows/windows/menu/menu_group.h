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

		protected:
			virtual index_and_size_type insert_into_parent_(gui_object_type &parent, index_and_size_type index) override;

			virtual bool pre_insert_(gui_object_type &object, index_and_size_type &index) override;

			virtual index_and_size_type post_insert_(gui_object_type &object, index_and_size_type index) override;

			virtual void post_remove_(gui_object_type &object) override;
		};
	}
}

#endif /* !WINPP_MENU_GROUP_H */
