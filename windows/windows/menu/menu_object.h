#pragma once

#ifndef WINPP_MENU_OBJECT_H
#define WINPP_MENU_OBJECT_H

#include "menu_newline.h"
#include "menu_radio_item.h"
#include "menu_separator.h"

#include "../gui/gui_object_tree.h"

namespace winpp{
	namespace application{
		class object_manager;
	}

	namespace menu{
		class object : public gui::object_tree{
		public:
			typedef gui::object_tree base_type;

			typedef ::HMENU hmenu_type;

			enum class action_type{
				dont_create,
				create,
			};

			explicit object(action_type action = action_type::dont_create);

			explicit object(app_type &app);

			explicit object(gui_object_type &parent);

			virtual ~object();

			virtual operator hmenu_type() const;

			virtual void *handle() const override;

			virtual hit_target_type hit_test(const point_type &value) const override;

			virtual rect_type outer_rect() const override;

			virtual rect_type relative_outer_rect() const override;

			virtual rect_type inner_rect() const override;

			virtual object &destroy(force_type force = force_type::dont_force) override;

			virtual unsigned int object_group() const override;

			virtual bool is_created() const override;

			virtual bool is_menu() const override;

			virtual bool is_popup() const;

			virtual void create();

			virtual void create(app_type &app);

			virtual void create(gui_object_type &parent);

		protected:
			friend class application::object_manager;

			virtual index_and_size_type insert_into_parent_(gui_object_type &parent, index_and_size_type index) override;

			hmenu_type value_;
		};
	}
}

#endif /* !WINPP_MENU_OBJECT_H */
