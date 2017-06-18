#pragma once

#ifndef WINPP_PREPROCESSOR_H
#define WINPP_PREPROCESSOR_H

#define WINPP_MAKE_OPERATORS_(x, t)\
	inline constexpr x operator |(x l, x r){\
		return (x)((t)(l) | (t)(r));\
	}\
\
	inline constexpr x operator &(x l, x r){\
		return (x)((t)(l) & (t)(r));\
	}\
\
	inline constexpr x operator ~(x r){\
		return (x)(~(t)(r));\
	}\
\
	inline x operator |=(x &l, x r){\
		return (l = (x)((t)(l) | (t)(r)));\
	}\
\
	inline x operator &=(x &l, x r){\
		return (l = (x)((t)(l) & (t)(r)));\
	}\
\
	inline x operator <<(x &l, t r){\
		return (x)((t)(l) << (r));\
	}\
\
	inline x operator >>(x &l, t r){\
		return (x)((t)(l) >> (r));\
	}

#define WINPP_MAKE_OPERATORS(x) WINPP_MAKE_OPERATORS_(x, unsigned int)

#define WINPP_MAKE_OPERATORS_EX(x) WINPP_MAKE_OPERATORS_(x, unsigned long long)

#ifndef MAKEDWORD
#define MAKEDWORD(l, h) ((::DWORD)MAKELONG(l, h))
#endif // !MAKEDWORD

#ifndef RGBA
#define RGBA(r, g, b, a) ((::COLORREF)( (((::DWORD)(::BYTE)(a)) << 24) | RGB(r, g, b) ))
#endif // !RGBA

#define GetAValue(rgba) (LOBYTE((rgba) >> 24))

#define WINPP_MAKE_MOUSE_POSITION(value) { GET_X_LPARAM(value), GET_Y_LPARAM(value) }
#define WINPP_SPLIT_WORD(value) { LOWORD(value), HIWORD(value) }

#define WINPP_SET(source, target) ((source) |= (target))
#define WINPP_REMOVE(source, target) ((source) &= ~(target))

#define WINPP_SET_V(source, target) ((source) | (target))
#define WINPP_REMOVE_V(source, target) ((source) & ~(target))

#define WINPP_IS(source, target) (((source) & (target)) == (target))
#define WINPP_IS_ANY(source, target) (((source) & (target)) != static_cast<decltype(target)>(0))

#endif /* !WINPP_PREPROCESSOR_H */
