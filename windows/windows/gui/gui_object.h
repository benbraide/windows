#pragma once

#ifndef WINPP_GUI_OBJECT_H
#define WINPP_GUI_OBJECT_H

#include <functional>

#include "../common/exception.h"
#include "../common/random_number.h"

#include "../structures/rect_structure.h"
#include "../application/application_object.h"

#include "gui_object_attributes.h"

namespace winpp{
	namespace gui{
		class object_sibling;
		class object_group;
		class object_tree;

		class object{
		public:
			typedef object gui_object_type;
			typedef object_sibling gui_sibling_type;
			typedef object_group gui_group_type;
			typedef object_tree gui_tree_type;
			typedef object_attributes gui_attributes_type;

			typedef application::object app_type;

			typedef structures::point point_type;
			typedef structures::size size_type;
			typedef structures::rect rect_type;

			typedef structures::absolute_point absolute_point_type;
			typedef structures::relative_point relative_point_type;
			typedef structures::outer_rect outer_rect_type;
			typedef structures::inner_rect inner_rect_type;

			typedef structures::enumerations::gui_sibling_type sibling_type;
			typedef structures::enumerations::placement_type placement_type;
			typedef structures::enumerations::hit_target_type hit_target_type;
			typedef structures::enumerations::event_type event_type;
			typedef structures::enumerations::show_mode_type show_mode_type;

			typedef std::size_t index_and_size_type;

			typedef std::function<bool(gui_object_type &)> object_traverser_type;
			typedef std::function<bool(gui_sibling_type &)> sibling_traverser_type;

			typedef ::WNDPROC procedure_type;

			enum class force_type{
				dont_force,
				force,
			};

			class event_tunnel{
			public:
				virtual ~event_tunnel() = default;
			};

			virtual ~object() = default;

			virtual app_type *app() const = 0;

			virtual app_type *scope_app() const = 0;

			virtual void *handle() const = 0;

			virtual object *non_sibling() = 0;

			virtual const object *non_sibling() const = 0;

			virtual object *parent() const = 0;

			virtual object *ancestor(index_and_size_type index) const = 0;

			virtual object *child_at_index(index_and_size_type index) const = 0;

			virtual object *child_at_absolute_index(index_and_size_type index) const = 0;

			virtual object &traverse_children(object_traverser_type traverser) = 0;

			virtual const object &traverse_children(object_traverser_type traverser) const = 0;

			virtual object &traverse_siblings(sibling_traverser_type traverser) = 0;

			virtual const object &traverse_siblings(sibling_traverser_type traverser) const = 0;

			virtual object &traverse_ancestors(object_traverser_type traverser) = 0;

			virtual const object &traverse_ancestors(object_traverser_type traverser) const = 0;

			virtual object &internal_set_parent(gui_object_type *parent) = 0;

			virtual index_and_size_type internal_insert_into_parent(gui_object_type &object) = 0;

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) = 0;

			virtual object &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) = 0;

			virtual object &outer_rect(const rect_type &value) = 0;

			virtual rect_type outer_rect() const = 0;

			virtual rect_type relative_outer_rect() const = 0;

			virtual object &inner_rect(const rect_type &value) = 0;

			virtual rect_type inner_rect() const = 0;

			virtual rect_type content_rect() const = 0;

			virtual object &padding(const rect_type &value) = 0;

			virtual rect_type padding() const = 0;

			virtual object &outer_size(const size_type &value) = 0;

			virtual size_type outer_size() const = 0;

			virtual object &inner_size(const size_type &value) = 0;

			virtual size_type inner_size() const = 0;

			virtual object &offset(const size_type &value) = 0;

			virtual point_type offset() const = 0;

			virtual point_type relative_offset() const = 0;

			virtual object &move(const point_type &value) = 0;

			virtual object &absolute_move(const point_type &value) = 0;

			virtual object &place(placement_type value, const size_type &delta = size_type()) = 0;

			virtual point_type compute_placement(placement_type value, const size_type &delta = size_type()) const = 0;

			virtual hit_target_type hit_test(const point_type &value) const = 0;

			virtual gui_object_type *hit_target(const point_type &value) const = 0;

			virtual gui_object_type *deepest_hit_target(const point_type &value) const = 0;

			virtual point_type convert_to_screen(const point_type &value) const = 0;

			virtual rect_type convert_to_screen(const rect_type &value) const = 0;

			virtual point_type convert_from_screen(const point_type &value) const = 0;

			virtual rect_type convert_from_screen(const rect_type &value) const = 0;

			virtual object &destroy(force_type force = force_type::dont_force) = 0;

			virtual object &show(show_mode_type mode = show_mode_type::show) = 0;

			virtual object &hide() = 0;

			virtual bool is_hidden() const = 0;

			virtual gui_attributes_type &attributes() = 0;

			virtual event_tunnel &events() = 0;

			virtual unsigned int group() const = 0;

			virtual index_and_size_type proposed_index() const = 0;

			virtual index_and_size_type index_in_parent() const = 0;

			virtual index_and_size_type absolute_index_in_parent() const = 0;

			virtual index_and_size_type child_index(const gui_object_type &object) const = 0;

			virtual index_and_size_type child_absolute_index(const gui_object_type &object) const = 0;

			virtual index_and_size_type absolute_index(index_and_size_type index) const = 0;

			virtual index_and_size_type children_count() const = 0;

			virtual index_and_size_type sibling_count() const = 0;

			virtual index_and_size_type ancestor_count() const = 0;

			virtual bool is_sibling() const = 0;

			virtual bool is_group() const = 0;

			virtual bool is_tree() const = 0;

			virtual bool is_inside_group() const = 0;

			virtual bool has_children() const = 0;

			virtual bool has_siblings() const = 0;

			virtual bool has_parent() const = 0;

			virtual bool is_group(unsigned int value) const = 0;

			virtual bool is_offspring(const gui_object_type &object) const = 0;

			virtual bool is_child(const gui_object_type &object) const = 0;

			virtual bool is_sibling(const gui_object_type &object) const = 0;

			virtual bool is_parent(const gui_object_type &object) const = 0;

			virtual bool is_ancestor(const gui_object_type &object) const = 0;

			virtual bool is_created() const = 0;

			virtual bool is_top_level() const = 0;

			virtual bool is_menu() const = 0;

			virtual bool is_menu_item() const = 0;

			virtual bool is_non_window() const = 0;

			virtual bool is_window() const = 0;

			virtual bool is_dialog() const = 0;

			virtual bool is_modal() const = 0;

			virtual bool is_control() const = 0;

			virtual procedure_type procedure() const = 0;

			template <typename target_type>
			target_type &query(){
				auto value = try_query<target_type>();
				if (value == nullptr)
					throw common::cast_exception();
				return *value;
			}

			template <typename target_type>
			const target_type &query() const{
				auto value = try_query<target_type>();
				if (value == nullptr)
					throw common::cast_exception();
				return *value;
			}

			template <typename target_type>
			target_type *try_query(){
				return dynamic_cast<target_type *>(non_sibling());
			}

			template <typename target_type>
			const target_type *try_query() const{
				return dynamic_cast<const target_type *>(non_sibling());
			}

			static const unsigned int default_group			= 0x0000u;
			static const unsigned int menu_group			= 0x0001u;
			static const unsigned int menu_item_group		= 0x0002u;
			static const unsigned int window_group			= 0x0003u;
			static const unsigned int non_window_group		= 0x0004u;

			static const unsigned int state_nil				= (0u << 0x0000u);
			static const unsigned int state_last_pow		= 0x0000u;

			static const index_and_size_type invalid_index	= static_cast<index_and_size_type>(-1);
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_H */
