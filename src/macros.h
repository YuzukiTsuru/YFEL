#ifndef MACROS_H
#define MACROS_H

#undef  YFEL_BEGIN_C_DECLS
#undef  YFEL_END_C_DECLS

#ifdef __cplusplus
#  define YFEL_BEGIN_C_DECLS extern "C" {
#  define YFEL_END_C_DECLS }
#else  // !__cplusplus
#  define YFEL_BEGIN_C_DECLS
#  define YFEL_END_C_DECLS
#endif  // __cplusplus

#endif // MACROS_H
