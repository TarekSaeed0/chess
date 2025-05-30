#ifndef CHESS_MACROS_H_INCLUDED
#define CHESS_MACROS_H_INCLUDED

#if defined(__cplusplus) && defined(__cpp_constexpr)
	#define CHESS_HAS_CONSTEXPR
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#if defined(__clang__)
		#if (__clang_major__ > 19) || (__clang_major__ == 19 && __clang_minor__ >= 1)
			#define CHESS_HAS_CONSTEXPR
		#endif
	#else
		#define CHESS_HAS_CONSTEXPR
	#endif
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
	#define CHESS_HAS_TYPED_ENUM
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define CHESS_HAS_TYPED_ENUM
#endif

#if defined(__cplusplus) && __cplusplus >= 201103L
	#define CHESS_HAS_NULLPTR
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
	#define CHESS_HAS_NULLPTR
#endif

#ifdef CHESS_HAS_CONSTEXPR
	#define CHESS_CONSTEXPR constexpr
#else
	#define CHESS_CONSTEXPR const
#endif

#ifdef CHESS_HAS_CONSTEXPR
	#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
		static CHESS_CONSTEXPR type name = (value)
#else
	#ifdef CHESS_HAS_TYPED_ENUM
		#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
			enum : type {                                           \
				name = (value)                                        \
			}
	#else
		#define CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value) \
			enum {                                                  \
				name = (value)                                        \
			}
	#endif
#endif

#ifdef CHESS_HAS_TYPED_ENUM
	#define CHESS_ENUM(type, name) \
		enum name : type;            \
		typedef enum name name;      \
		enum name : type
#else
	#define CHESS_ENUM(type, name) \
		typedef type name;           \
		enum name
#endif

#ifdef CHESS_HAS_NULLPTR
	#define CHESS_NULL nullptr
#else
	#define CHESS_NULL NULL
#endif

#define CHESS_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#endif // CHESS_MACROS_H_INCLUDED
