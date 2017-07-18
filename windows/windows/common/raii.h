#pragma once

#ifndef WINPP_RAII_H
#define WINPP_RAII_H

#include <functional>

namespace winpp{
	namespace common{
		class raii{
		public:
			typedef std::function<void()> callback_type;

			raii(callback_type init, callback_type uninit);

			~raii();

		private:
			callback_type uninit_;
		};
	}
}

#endif /* !WINPP_RAII_H */
