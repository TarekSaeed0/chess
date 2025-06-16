/**
 * @file chess/macros.h
 * @brief Defines macros and constants for the library.
 * @author Tarek Saeed
 * @date 2025-06-14
 */

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

/**
 * @def CHESS_CONSTEXPR
 * @brief Expands to `constexpr` if available, otherwise to `const`.
 */

#ifdef CHESS_HAS_CONSTEXPR
	#define CHESS_CONSTEXPR constexpr
#else
	#define CHESS_CONSTEXPR const
#endif

/**
 * @def CHESS_DEFINE_INTEGRAL_CONSTANT(type, name, value)
 * @brief Defines a compile time integral constant.
 * @param[in] type The type of the constant.
 * @param[in] name The name of the constant.
 * @param[in] value The value of the constant.
 */

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

/**
 * @def CHESS_ENUM(type, name)
 * @brief Defines a typed enum or a typedef enum based on the availability of typed enums in the compiler.
 * @param[in] type The underlying type of the enum.
 * @param[in] name The name of the enum type.
 */

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

/**
 * @def CHESS_NULL
 * @brief Expands to `nullptr` if available, otherwise to `NULL`.
 */

#ifdef CHESS_HAS_NULLPTR
	#define CHESS_NULL nullptr
#else
	#define CHESS_NULL NULL
#endif

/**
 * @def CHESS_ARRAY_LENGTH(array)
 * @brief Computes the length of the given array.
 * @param[in] array The array
 * @return The length of the array
 */

#define CHESS_ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

#endif // CHESS_MACROS_H_INCLUDED
