#pragma once

#ifndef WINPP_MESSAGE_WINDOW_H
#define WINPP_MESSAGE_WINDOW_H

#include "window_object.h"

namespace winpp{
	namespace window{
		class message : public object{
		public:
			message();

			explicit message(const std::wstring &caption);

			message(app_type &app, const std::wstring &caption);

			virtual ~message();

			virtual bool is_top_level() const override;

			virtual dword_type black_listed_styles(bool is_extended) const override;

			virtual void create(const std::wstring &caption);

			virtual void create(app_type &app, const std::wstring &caption);

		protected:
			virtual void create_(const std::wstring &caption, app_type *app);
		};
	}
}

#endif /* !WINPP_MESSAGE_WINDOW_H */
