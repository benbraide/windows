#pragma once

#ifndef WINPP_EVENT_OBJECT_H
#define WINPP_EVENT_OBJECT_H

#include "../gui/gui_object.h"

namespace winpp{
	namespace events{
		class object{
		public:
			typedef gui::object gui_object_type;

			enum class state_type : unsigned int{
				nil				= (0 << 0x0000),
				prevented		= (1 << 0x0000),
				stopped			= (1 << 0x0001),
			};

			explicit object(gui_object_type &target);

			virtual ~object();

			object &prevent();

			object &stop_propagation();

			bool is_prevented() const;

			bool is_propagating() const;

			gui_object_type &target() const;

		protected:
			gui_object_type *target_;
			state_type states_;
		};

		template <class value_type>
		class object_with_data : public object{
		public:
			typedef value_type value_type;
			typedef std::conditional_t<std::is_object_v<value_type>, const value_type &, value_type> const_qualified_type;

			object_with_data(gui_object_type &target, const_qualified_type data)
				: object(target), data_(data){}

			virtual ~object_with_data() = default;

			const_qualified_type data() const{
				return data_;
			}

		private:
			value_type data_;
		};

		WINPP_MAKE_OPERATORS(object::state_type);
	}
}

#endif /* !WINPP_EVENT_OBJECT_H */
