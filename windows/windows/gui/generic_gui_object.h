#pragma once

#ifndef WINPP_GENERIC_GUI_OBJECT_H
#define WINPP_GENERIC_GUI_OBJECT_H

#include <any>
#include <memory>
#include <string>

#include "gui_object.h"
#include "../events/event_tunnel.h"

namespace winpp{
	namespace gui{
		class generic_object : public object{
		public:
			class event_tunnel : public object::event_tunnel{
			public:
				typedef structures::point point_type;
				typedef structures::size size_type;

				event_tunnel();

				virtual ~event_tunnel();

				virtual unsigned __int64 bind(std::wstring e, const std::any &callback);

				virtual unsigned __int64 bind(const std::string &e, const std::any &callback);

				virtual unsigned __int64 bind(event_type e, const std::any &callback);

				virtual bool unbind(unsigned __int64 id);

				events::tunnel<bool> create;
				events::tunnel<void> destroy;

				events::tunnel<void, events::object_with_data<point_type>> move;
				events::tunnel<void, events::object_with_data<size_type>> size;

			protected:
				virtual unsigned __int64 bind_(const std::wstring &e, const std::any &callback);
			};

			typedef std::shared_ptr<gui_attributes_type> attributes_type;
			typedef std::shared_ptr<event_tunnel> events_type;

			virtual ~generic_object();

			virtual app_type *app() const override;

			virtual void *handle() const override;

			virtual object *non_sibling() override;

			virtual const object *non_sibling() const override;

			virtual object *parent() const override;

			virtual object *ancestor(index_and_size_type index) const override;

			virtual object *child_at_index(index_and_size_type index) const override;

			virtual object *child_at_absolute_index(index_and_size_type index) const override;

			virtual object &traverse_children(object_traverser_type traverser) override;

			virtual const object &traverse_children(object_traverser_type traverser) const override;

			virtual object &traverse_siblings(sibling_traverser_type traverser) override;

			virtual const object &traverse_siblings(sibling_traverser_type traverser) const override;

			virtual object &traverse_ancestors(object_traverser_type traverser) override;

			virtual const object &traverse_ancestors(object_traverser_type traverser) const override;

			virtual object &internal_set_parent(gui_object_type *parent) override;

			virtual index_and_size_type internal_insert_into_parent(gui_object_type &object) override;

			virtual index_and_size_type internal_insert_child(gui_object_type &child, index_and_size_type before = invalid_index) override;

			virtual object &internal_remove_child(gui_object_type &child, force_type force = force_type::dont_force) override;

			virtual object &outer_rect(const rect_type &value) override;

			virtual rect_type outer_rect() const override;

			virtual rect_type relative_outer_rect() const override;

			virtual object &inner_rect(const rect_type &value) override;

			virtual rect_type inner_rect() const override;

			virtual rect_type content_rect() const override;

			virtual object &padding(const rect_type &value) override;

			virtual rect_type padding() const override;

			virtual object &outer_size(const size_type &value) override;

			virtual size_type outer_size() const override;

			virtual object &inner_size(const size_type &value) override;

			virtual size_type inner_size() const override;

			virtual object &offset(const size_type &value) override;

			virtual point_type offset() const override;

			virtual point_type relative_offset() const override;

			virtual object &move(const point_type &value) override;

			virtual object &absolute_move(const point_type &value) override;

			virtual object &place(placement_type value, const size_type &delta = size_type()) override;

			virtual point_type compute_placement(placement_type value, const size_type &delta = size_type()) const override;

			virtual hit_target_type hit_test(const point_type &value) const override;

			virtual gui_object_type *hit_target(const point_type &value) const override;

			virtual gui_object_type *deepest_hit_target(const point_type &value) const override;

			virtual point_type convert_to_screen(const point_type &value) const override;

			virtual rect_type convert_to_screen(const rect_type &value) const override;

			virtual point_type convert_from_screen(const point_type &value) const override;

			virtual rect_type convert_from_screen(const rect_type &value) const override;

			virtual object &destroy(force_type force = force_type::dont_force) override;

			virtual gui_attributes_type &attributes() override;

			virtual event_tunnel &events() override;

			virtual unsigned int group() const override;

			virtual index_and_size_type proposed_index() const override;

			virtual index_and_size_type index_in_parent() const override;

			virtual index_and_size_type absolute_index_in_parent() const override;

			virtual index_and_size_type child_index(const gui_object_type &object) const override;

			virtual index_and_size_type child_absolute_index(const gui_object_type &object) const override;

			virtual index_and_size_type absolute_index(index_and_size_type index) const override;

			virtual index_and_size_type children_count() const override;

			virtual index_and_size_type sibling_count() const override;

			virtual index_and_size_type ancestor_count() const override;

			virtual bool is_sibling() const override;

			virtual bool is_group() const override;

			virtual bool is_tree() const override;

			virtual bool is_inside_group() const override;

			virtual bool has_children() const override;

			virtual bool has_siblings() const override;

			virtual bool has_parent() const override;

			virtual bool is_group(unsigned int value) const override;

			virtual bool is_offspring(const gui_object_type &object) const override;

			virtual bool is_child(const gui_object_type &object) const override;

			virtual bool is_parent(const gui_object_type &object) const override;

			virtual bool is_ancestor(const gui_object_type &object) const override;

			virtual bool is_sibling(const gui_object_type &object) const override;

			virtual bool is_created() const override;

			virtual bool is_top_level() const override;

			virtual bool is_menu() const override;

			virtual bool is_menu_item() const override;

			virtual bool is_non_window() const override;

			virtual bool is_window() const override;

			virtual bool is_dialog() const override;

			virtual bool is_modal() const override;

			virtual bool is_control() const override;

			virtual procedure_type procedure() const override;

		protected:
			virtual void created_();

			virtual void destroyed_();

			virtual void sized_();

			virtual void moved_();

			virtual attributes_type get_attributes_();

			virtual events_type get_events_();

			template <typename generic_attributes_type>
			attributes_type create_attributes_(){
				if (attributes_ == nullptr)
					attributes_ = std::make_shared<generic_attributes_type>(*this);
				return attributes_;
			}

			template <typename generic_events_type, typename... args_types>
			events_type create_events_(args_types &&...args){
				if (events_ == nullptr)
					events_ = std::make_shared<generic_events_type>(std::forward<args_types>(args)...);
				return events_;
			}

			app_type *app_ = nullptr;
			gui_object_type *parent_ = nullptr;
			attributes_type attributes_;
			events_type events_;
		};
	}
}

#endif /* !WINPP_GENERIC_GUI_OBJECT_H */
