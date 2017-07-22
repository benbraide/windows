#pragma once

#ifndef WINPP_MENU_ITEM_H
#define WINPP_MENU_ITEM_H

#include "../gui/generic_gui_object.h"

namespace winpp{
	namespace menu{
		class item : public gui::generic_object{
		public:
			typedef gui::generic_object base_type;
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

			virtual item &states(menu_state_type value);

			virtual menu_state_type states() const;

			virtual bool is_owner_drawn() const;

			static const word_type invalid_id = 0;

		protected:
			virtual attributes_type get_attributes_() override;

			word_type id_;
		};
	}
}

#endif /* !WINPP_MENU_ITEM_H */
