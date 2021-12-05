#pragma once

#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, _10, \
         _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, \
         _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, \
         _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, \
         _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, \
         _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, \
         _61, _62, _63, N, ...) N

#define PP_RSEQ_N()                                        \
         62, 61, 60,                                       \
         59, 58, 57, 56, 55, 54, 53, 52, 51, 50,           \
         49, 48, 47, 46, 45, 44, 43, 42, 41, 40,           \
         39, 38, 37, 36, 35, 34, 33, 32, 31, 30,           \
         29, 28, 27, 26, 25, 24, 23, 22, 21, 20,           \
         19, 18, 17, 16, 15, 14, 13, 12, 11, 10,           \
          9,  8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    PP_ARG_N(__VA_ARGS__)
#define PP_NARG(...)     PP_NARG_(_, ##__VA_ARGS__, PP_RSEQ_N())

#define VARIANT(macro, ...) EVAL(IMPL_NAME, macro, PP_NARG(__VA_ARGS__))
#define EVAL(macro, ...) macro(__VA_ARGS__)
#define IMPL_NAME(x, n) x##_##n

#define FOLDL(f, init, ...) VARIANT(FOLDL, ##__VA_ARGS__)(f, init, ##__VA_ARGS__)
#define FOLDR(f, init, ...) VARIANT(FOLDR, ##__VA_ARGS__)(f, init, ##__VA_ARGS__)

#define FOLDL_0(f, init, ...) init
#define FOLDR_0(f, init, ...) init

#define FOLDL_1(f, init, x, ...) FOLDL_0(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_1(f, init, x, ...) EVAL(f, x, FOLDR_0(f, init, ##__VA_ARGS__))

#define FOLDL_2(f, init, x, ...) FOLDL_1(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_2(f, init, x, ...) EVAL(f, x, FOLDR_1(f, init, ##__VA_ARGS__))

#define FOLDL_3(f, init, x, ...) FOLDL_2(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_3(f, init, x, ...) EVAL(f, x, FOLDR_2(f, init, ##__VA_ARGS__))

#define FOLDL_4(f, init, x, ...) FOLDL_3(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_4(f, init, x, ...) EVAL(f, x, FOLDR_3(f, init, ##__VA_ARGS__))

#define FOLDL_5(f, init, x, ...) FOLDL_4(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_5(f, init, x, ...) EVAL(f, x, FOLDR_4(f, init, ##__VA_ARGS__))

#define FOLDL_6(f, init, x, ...) FOLDL_5(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_6(f, init, x, ...) EVAL(f, x, FOLDR_5(f, init, ##__VA_ARGS__))

#define FOLDL_7(f, init, x, ...) FOLDL_6(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_7(f, init, x, ...) EVAL(f, x, FOLDR_6(f, init, ##__VA_ARGS__))

#define FOLDL_8(f, init, x, ...) FOLDL_7(f, f(init, x), ##__VA_ARGS__)
#define FOLDR_8(f, init, x, ...) EVAL(f, x, FOLDR_7(f, init, ##__VA_ARGS__))
