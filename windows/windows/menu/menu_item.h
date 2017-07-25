#pragma once

#ifndef WINPP_MENU_ITEM_H
#define WINPP_MENU_ITEM_H

#include "../gui/generic_gui_object.h"

namespace winpp{
	namespace menu{
		class object;

		class item : public gui::generic_object{
		public:
			typedef gui::generic_object base_type;
			typedef menu::object menu_object_type;

			typedef structures::enumerations::menu_state_type menu_state_type;

			typedef ::WORD word_type;
			typedef ::UINT uint_type;
			typedef ::ULONG_PTR ulong_ptr_type;

			typedef ::HMENU hmenu_type;
			typedef ::MENUITEMINFOW info_type;

			item();

			item(gui_object_type &parent, const std::wstring &label, menu_state_type states = menu_state_type::nil);

			virtual ~item();

			virtual void *handle() const override;

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual item &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) override;

			virtual hit_target_type hit_test(const point_type &value) const override;

			virtual rect_type outer_rect() const override;

			virtual rect_type relative_outer_rect() const override;

			virtual rect_type inner_rect() const override;

			virtual item &update_state() override;

			virtual item &destroy(force_type force = force_type::dont_force) override;

			virtual unsigned int object_group() const override;

			virtual bool is_created() const override;

			virtual bool is_menu_item() const override;

			virtual void create(gui_object_type &parent, const std::wstring &label, menu_state_type states = menu_state_type::nil);

			menu_object_type *sub() const;

			virtual item &states(menu_state_type value);

			virtual menu_state_type states() const;

			virtual bool is_owner_drawn() const;

			virtual item &enable();

			virtual item &disable();

			virtual bool is_disabled() const;

			virtual item &set_as_default(bool is_default = true);

			virtual bool is_default() const;

			virtual item &check();

			virtual item &uncheck();

			virtual item &toggle_check();

			virtual bool is_checked() const;

			virtual bool is_action() const;

			virtual bool is_separator() const;

			static const word_type invalid_id = 0;

		protected:
			virtual attributes_type get_attributes_() override;

			virtual void create_(gui_object_type &parent, const std::wstring &label, menu_state_type states);

			virtual uint_type get_types_() const;

			virtual void set_info_(const info_type &info);

			virtual void get_info_(info_type &info) const;

			word_type id_;
			menu_object_type *sub_;
		};
	}
}

#endif /* !WINPP_MENU_ITEM_H */
