#pragma once

#ifndef WINPP_TOP_LEVEL_WINDOW_H
#define WINPP_TOP_LEVEL_WINDOW_H

#include "dialog_window.h"

namespace winpp{
	namespace window{
		template <class this_base_type>
		class basic_top_level : public this_base_type{
		public:
			typedef this_base_type base_type;

			typedef gui::object gui_object_type;
			typedef gui::object_sibling gui_sibling_type;

			typedef gui_object_type::point_type point_type;
			typedef gui_object_type::size_type size_type;

			typedef application::object app_type;

			basic_top_level(){
				base_type::reset_persistent_styles_();
			}

			basic_top_level(const std::wstring &caption, const point_type &offset, const size_type &size){
				base_type::reset_persistent_styles_();
				create(caption, offset, size);
			}

			basic_top_level(app_type &app, const std::wstring &caption, const point_type &offset, const size_type &size){
				base_type::reset_persistent_styles_();
				create(app, caption, offset, size);
			}

			virtual ~basic_top_level(){
				base_type::destroy(gui::object::force_type::force);
			}

			virtual void create(const std::wstring &caption, const point_type &offset, const size_type &size){
				if (is_created())
					throw common::unsupported_exception();
				base_type::create_(caption, offset, size, 0u, 0u, nullptr, nullptr);
			}

			virtual void create(app_type &app, const std::wstring &caption, const point_type &offset, const size_type &size){
				if (is_created())
					throw common::unsupported_exception();
				base_type::create_(caption, offset, size, 0u, 0u, nullptr, &app);
			}
		};

		typedef basic_top_level<object> top_level;
		typedef basic_top_level<dialog> top_level_dialog;
	}
}

#endif /* !WINPP_TOP_LEVEL_WINDOW_H */
