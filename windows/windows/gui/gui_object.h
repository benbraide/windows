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

			typedef structures::enumerations::gui_sibling_type sibling_type;
			typedef structures::enumerations::placement_type placement_type;
			typedef structures::enumerations::hit_target_type hit_target_type;
			typedef structures::enumerations::event_type event_type;

			typedef std::size_t index_and_size_type;

			typedef std::function<bool(gui_object_type &)> object_traverser_type;
			typedef std::function<bool(gui_sibling_type &)> sibling_traverser_type;

			class event_tunnel{
			public:
				typedef structures::enumerations::event_type event_type;

				virtual ~event_tunnel() = default;

			protected:
				unsigned __int32 id_(){
					return generator_(1, std::numeric_limits<unsigned __int32>::max());
				}

				common::random_uint32 generator_;
			};

			virtual ~object() = default;

			virtual app_type *app() const = 0;

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

			virtual object &destroy(bool no_throw = false) = 0;

			virtual gui_attributes_type &attributes() = 0;

			virtual event_tunnel &events() = 0;

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

			virtual bool is_offspring(const gui_object_type &object) const = 0;

			virtual bool is_child(const gui_object_type &object) const = 0;

			virtual bool is_sibling(const gui_object_type &object) const = 0;

			virtual bool is_parent(const gui_object_type &object) const = 0;

			virtual bool is_ancestor(const gui_object_type &object) const = 0;

			virtual bool is_created() const = 0;

			static const unsigned int state_nil				= (0u << 0x0000u);
			static const unsigned int state_last_pow		= 0x0000u;

			static const index_and_size_type invalid_index	= static_cast<index_and_size_type>(-1);
		};
	}
}

#endif /* !WINPP_GUI_OBJECT_H */
