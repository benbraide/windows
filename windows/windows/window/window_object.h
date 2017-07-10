#pragma once

#ifndef WINPP_WINDOW_OBJECT_H
#define WINPP_WINDOW_OBJECT_H

#include "../gui/gui_object_tree.h"

#include "../wrappers/hwnd_wrapper.h"
#include "../wrappers/wnd_class_wrapper.h"

#include "../application/object_manager.h"

#include "../messaging/message_target.h"

#include "window_styles.h"

namespace winpp{
	namespace window{
		class object : public gui::object_tree, public messaging::target{
		public:
			typedef gui::object_tree tree_base_type;

			typedef window::styles styles_type;
			typedef styles_type::info_type styles_info_type;
			typedef std::shared_ptr<styles_type> styles_ptr_type;

			typedef ::UINT uint_type;
			typedef ::WORD word_type;
			typedef ::DWORD dword_type;
			typedef ::WNDPROC procedure_type;

			typedef wrappers::wnd_class wnd_class_type;
			typedef wrappers::hwnd hwnd_type;
			typedef wrappers::hwnd::value_type hwnd_value_type;

			typedef structures::color color_type;

			typedef structures::point point_type;
			typedef structures::size size_type;
			typedef structures::rect rect_type;

			typedef structures::pointf pointf_type;
			typedef structures::sizef sizef_type;
			typedef structures::rectf rectf_type;

			typedef structures::enumerations::position_type position_type;
			typedef structures::enumerations::window_placement_type window_placement_type;

			typedef wnd_class_type::instance_type instance_type;

			typedef hwnd_type::create_info_type create_info_type;

			using tree_base_type::outer_size;
			using tree_base_type::inner_size;
			using tree_base_type::offset;

			class event_tunnel : public tree_base_type::event_tunnel{
			public:
				typedef tree_base_type::event_tunnel base_type;

				using base_type::bind;

				explicit event_tunnel(gui_object_type &object);

				virtual ~event_tunnel();

				virtual unsigned __int64 bind(event_type e, const std::any &callback) override;

				virtual bool unbind(unsigned __int64 id) override;

				events::tunnel<void> pre_create;
				events::tunnel<void> post_destroy;

				events::tunnel<void> close;

				events::tunnel<void> maximize;
				events::tunnel<void> minimize;
				events::tunnel<void> restore;

				events::tunnel<void> show;
				events::tunnel<void> hide;

				events::tunnel<void> erase_background;
				events::tunnel<void> paint;

				events::timer_tunnel<false> timer;
				events::timer_tunnel<true> interval;

			protected:
				virtual unsigned __int64 bind_(const std::wstring &e, const std::any &callback) override;
			};

			virtual ~object();

			virtual operator hwnd_value_type() const;

			virtual void *handle() const override;

			virtual object &outer_rect(const rect_type &value) override;

			virtual rect_type outer_rect() const override;

			virtual rect_type relative_outer_rect() const override;

			virtual object &inner_rect(const rect_type &value) override;

			virtual rect_type inner_rect() const override;

			virtual object &outer_size(const size_type &value) override;

			virtual object &inner_size(const size_type &value) override;

			virtual object &offset(const size_type &value) override;

			virtual point_type relative_offset() const override;

			virtual object &move(const point_type &value) override;

			virtual object &absolute_move(const point_type &value) override;

			virtual point_type convert_to_screen(const point_type &value) const override;

			virtual rect_type convert_to_screen(const rect_type &value) const override;

			virtual point_type convert_from_screen(const point_type &value) const override;

			virtual rect_type convert_from_screen(const rect_type &value) const override;

			virtual object &destroy(force_type force = force_type::dont_force) override;

			virtual event_tunnel &events() override;

			virtual unsigned int group() const override;

			virtual bool is_created() const override;

			virtual bool is_top_level() const override;

			virtual bool is_window() const override;

			virtual procedure_type procedure() const override;

			virtual bool pre_translate(msg_type &msg) override;

			virtual styles_type &styles();

			virtual dword_type filter_styles(dword_type value, bool is_extended) const;

			virtual dword_type white_listed_styles(bool is_extended) const;

			virtual dword_type black_listed_styles(bool is_extended) const;

		protected:
			explicit object(procedure_type procedure = ::DefWindowProcW);

			virtual events_type get_events_() override;

			virtual bool cache_group_(unsigned int value) const override;

			virtual target *target_parent_() const override;

			virtual void *find_event_() override;

			template <typename generic_styles_type, typename... args_types>
			styles_ptr_type create_styles_(args_types &&...args){
				if (styles_ == nullptr)
					styles_ = std::make_shared<generic_styles_type>(std::forward<args_types>(args)...);
				return styles_;
			}

			virtual styles_ptr_type get_styles_();

			virtual void reset_persistent_styles_();

			virtual void create_(const std::wstring &caption, const point_type &offset, const size_type &size, dword_type styles, dword_type extended_styles, const wchar_t *class_name, app_type *app);

			virtual void create_(const create_info_type &info, app_type *app);

			hwnd_type value_;
			procedure_type procedure_;
			styles_info_type persistent_styles_;
			styles_ptr_type styles_;
		};
	}
}

#endif /* !WINPP_WINDOW_OBJECT_H */
