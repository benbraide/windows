#pragma once

#ifndef WINPP_CHILD_WINDOW_H
#define WINPP_CHILD_WINDOW_H

#include "dialog_window.h"

namespace winpp{
	namespace window{
		template <class base_type>
		class basic_child{
		public:
			typedef base_type base_type;

			typedef gui::object gui_object_type;
			typedef gui::object_sibling gui_sibling_type;

			typedef gui_object_type::point_type point_type;
			typedef gui_object_type::size_type size_type;

			basic_child(){
				base_type::reset_persistent_styles_();
			}

			basic_child(gui_object_type &parent, const std::wstring &caption, const point_type &offset, const size_type &size){
				base_type::reset_persistent_styles_();
				create(parent, caption, offset, size);
			}

			virtual ~basic_child(){
				base_type::destroy(true);
			}

			virtual void create(gui_object_type &parent, const std::wstring &caption, const point_type &offset, const size_type &size){
				base_type::internal_insert_into_parent(parent);
				base_type::create_(caption, offset, size, 0u, 0u, nullptr, parent.app());
			}
		};

		typedef basic_child<object> child;
		typedef basic_child<dialog> child_dialog;
	}
}

#endif /* !WINPP_CHILD_WINDOW_H */
