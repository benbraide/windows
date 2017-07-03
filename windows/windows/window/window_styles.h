#pragma once

#ifndef WINPP_WINDOW_STYLES_H
#define WINPP_WINDOW_STYLES_H

#include <mutex>

#include "../wrappers/hwnd_wrapper.h"
#include "../threading/thread_id.h"

namespace winpp{
	namespace window{
		class object;

		class styles{
		public:
			typedef ::DWORD dword_type;

			typedef wrappers::hwnd hwnd_type;
			typedef wrappers::hwnd::data_index_type data_index_type;

			typedef threading::id thread_id_type;

			typedef std::recursive_mutex lock_type;
			typedef std::lock_guard<lock_type> guard_type;

			struct info_type{
				dword_type basic;
				dword_type extended;
			};

			struct update_info{
				bool active;
				thread_id_type locking_thread;
				info_type enabled;
				info_type disabled;
			};

			styles(object &object, info_type &info);

			virtual ~styles();

			styles &begin();

			styles &end();

			styles &cancel();

			bool has_begun() const;

			styles &apply(dword_type value, bool enabled, bool extended);

			styles &apply_modal();

			bool has(dword_type value, bool extended) const;

			styles &dbl_click(bool enabled);

			bool dbl_click() const;

			styles &drop_shadow(bool enabled);

			bool drop_shadow() const;

			styles &horizontal_redraw(bool enabled);

			bool horizontal_redraw() const;

			styles &vertical_redraw(bool enabled);

			bool vertical_redraw() const;

			styles &close(bool enabled);

			bool close() const;

			styles &maximize(bool enabled);

			bool maximize() const;

			styles &minimize(bool enabled);

			bool minimize() const;

			styles &border(bool enabled);

			bool border() const;

			styles &dialog_frame(bool enabled);

			bool dialog_frame() const;

			styles &caption(bool enabled);

			bool caption() const;

			styles &system_menu(bool enabled);

			bool system_menu() const;

			styles &thick_frame(bool enabled);

			bool thick_frame() const;

			styles &clip_children(bool enabled);

			bool clip_children() const;

			styles &clip_siblings(bool enabled);

			bool clip_siblings() const;

			styles &horizontal_scroll_bar(bool enabled);

			bool horizontal_scroll_bar() const;

			styles &vertical_scroll_bar(bool enabled);

			bool vertical_scroll_bar() const;

			styles &tab_stop(bool enabled);

			bool tab_stop() const;

			styles &enabled(bool enabled);

			bool enabled() const;

			styles &visible(bool enabled);

			bool visible() const;

			styles &file_drop(bool enabled);

			bool file_drop() const;

			styles &taskbar(bool enabled);

			bool taskbar() const;

			styles &sunken_edge(bool enabled);

			bool sunken_edge() const;

			styles &context_help(bool enabled);

			bool context_help() const;

			styles &control_parent(bool enabled);

			bool control_parent() const;

			styles &dialog_modal_frame(bool enabled);

			bool dialog_modal_frame() const;

			styles &layered(bool enabled);

			bool layered() const;

			styles &right_alignment(bool enabled);

			bool right_alignment() const;

			styles &rtl_reading(bool enabled);

			bool rtl_reading() const;

			styles &rtl_layout(bool enabled);

			bool rtl_layout() const;

			styles &left_scroll_bar(bool enabled);

			bool left_scroll_bar() const;

			styles &mouse_activation(bool enabled);

			bool mouse_activation() const;

			styles &inheritable_layout(bool enabled);

			bool inheritable_layout() const;

			styles &parent_notify(bool enabled);

			bool parent_notify() const;

			styles &static_edge(bool enabled);

			bool static_edge() const;

			styles &window_edge(bool enabled);

			bool window_edge() const;

			styles &top_most(bool enabled);

			bool top_most() const;

			styles &transparent(bool enabled);

			bool transparent() const;

			dword_type retrieve(bool extended) const;

			const info_type &enabled_changes() const;

			const info_type &disabled_changes() const;

		protected:
			hwnd_type object_value_() const;

			dword_type merge_changes_(bool extended) const;

			void write_changes_();

			void write_(dword_type value, bool extended, bool add);

			virtual void final_write_(dword_type value, bool extended);

			styles &update_(dword_type value, bool enabled, bool extended);

			styles &enable_(dword_type value, bool extended);

			styles &disable_(dword_type value, bool extended);

			virtual dword_type retrieve_(bool extended) const;

			bool has_(dword_type value, bool extended) const;

			object *object_;
			info_type *info_;
			update_info update_info_;
			lock_type lock_;
		};

		class styles_batch{
		public:
			explicit styles_batch(styles &target);

			~styles_batch();

			styles_batch(const styles_batch &) = delete;
			
			styles_batch &operator =(const styles_batch &) = delete;

			void end();

			void cancel();

		private:
			styles *target_;
		};
	}
}

#endif /* !WINPP_WINDOW_STYLES_H */
