#define NARG(...) __NARG_I_(__VA_ARGS__, __RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14,   \
                _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26,    \
                _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38,    \
                _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50,    \
                _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62,    \
                _63, N, ...)                                                   \
  N
#define __RSEQ_N()                                                             \
  63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,  \
      44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,  \
      26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,   \
      8, 7, 6, 5, 4, 3, 2, 1, 0

#define VFUNC_AUX2(name, n) name##n
#define VFUNC_AUX1(name, n) VFUNC_AUX2(name, n)
#define VFUNC(func, ...) VFUNC_AUX1(func, NARG(__VA_ARGS__))(__VA_ARGS__)

#define VFUNC_AUX2_ALT(name, n) name##n
#define VFUNC_AUX1_ALT(name, n) VFUNC_AUX2_ALT(name, n)
#define VFUNC_ALT(func, ...)                                                   \
  VFUNC_AUX1_ALT(func, NARG(__VA_ARGS__))(__VA_ARGS__)

#define NO_TEMPLATE() VFUNC_ALT(TEMPLATE_, TEMPLATE_PARAMS(_))
#define TEMPLATE(...) VFUNC_ALT(TEMPLATE_, TEMPLATE_PARAMS(__VA_ARGS__, _))

#define TEMPLATE_1(_)

#define TEMPLATE_3(spec1, tv1, _) template <spec1 tv1>

#define TEMPLATE_5(spec1, tv1, spec2, tv2, _) template <spec1 tv1, spec2 tv2>

#define TEMPLATE_7(spec1, tv1, spec2, tv2, spec3, tv3, _)                      \
  template <spec1 tv1, spec2 tv2, spec3 tv3>

#define TEMPLATE_9(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, _)          \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4>

#define TEMPLATE_11(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, _)                                                    \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5>

#define TEMPLATE_13(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, _)                                        \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6>

#define TEMPLATE_15(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, _)                            \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7>

#define TEMPLATE_17(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, _)                \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8>

#define TEMPLATE_19(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, _)    \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9>

#define TEMPLATE_21(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, _)                                           \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10>

#define TEMPLATE_23(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, _)                             \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11>

#define TEMPLATE_25(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, spec12, tv12, _)               \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12>

#define TEMPLATE_27(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13, _) \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13>

#define TEMPLATE_29(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,    \
                    spec14, tv14, _)                                           \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14>

#define TEMPLATE_31(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,    \
                    spec14, tv14, spec15, tv15, _)                             \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14, spec15 tv15>

#define TEMPLATE_33(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,     \
                    tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,       \
                    spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,    \
                    spec14, tv14, spec15, tv15, spec16, tv16, _)               \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14, spec15 tv15, spec16 tv16>

#define NO_SPEC() VFUNC_ALT(SPEC_, TEMPLATE_PARAMS(_))
#define SPEC(...) VFUNC_ALT(SPEC_, TEMPLATE_PARAMS(__VA_ARGS__, _))

#define SPEC_1(_)

#define SPEC_3(spec1, tv1, _) <tv1>

#define SPEC_5(spec1, tv1, spec2, tv2, _) <tv1,tv2>

#define SPEC_7(spec1, tv1, spec2, tv2, spec3, tv3, _) <tv1,tv2,tv3>

#define SPEC_9(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, _)              \
<tv1,tv2,tv3,tv4>

#define SPEC_11(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5, _) \
<tv1,tv2,tv3,tv4,tv5>

#define SPEC_13(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, _)                                                 \
<tv1,tv2,tv3,tv4,tv5,tv6>

#define SPEC_15(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, _)                                     \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7>

#define SPEC_17(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, _)                         \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8>

#define SPEC_19(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, _)             \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9>

#define SPEC_21(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                _)                                                             \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10>

#define SPEC_23(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, _)                                               \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11>

#define SPEC_25(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, spec12, tv12, _)                                 \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11,tv12>

#define SPEC_27(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, spec12, tv12, spec13, tv13, _)                   \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11,tv12,tv13>

#define SPEC_29(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, spec12, tv12, spec13, tv13, spec14, tv14, _)     \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11,tv12,tv13,tv14>

#define SPEC_31(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, spec12, tv12, spec13, tv13, spec14, tv14,        \
                spec15, tv15, _)                                               \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11,tv12,tv13,tv14,tv15>

#define SPEC_33(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5, tv5,    \
                spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, spec10, tv10,  \
                spec11, tv11, spec12, tv12, spec13, tv13, spec14, tv14,        \
                spec15, tv15, spec16, tv16, _)                                 \
<tv1,tv2,tv3,tv4,tv5,tv6,tv7,tv8,tv9,tv10,tv11,tv12,tv13,tv14,tv15,tv16>

#define NULL_SPEC() VFUNC_ALT(NULL_SPEC_, TEMPLATE_PARAMS(_))

#define NULL_SPEC_1(_) template <>

#define NULL_SPEC_3(spec1, tv1, _) template <spec1 tv1>

#define NULL_SPEC_5(spec1, tv1, spec2, tv2, _) template <spec1 tv1, spec2 tv2>

#define NULL_SPEC_7(spec1, tv1, spec2, tv2, spec3, tv3, _)                     \
  template <spec1 tv1, spec2 tv2, spec3 tv3>

#define NULL_SPEC_9(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, _)         \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4>

#define NULL_SPEC_11(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, _)                                                   \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5>

#define NULL_SPEC_13(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, _)                                       \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6>

#define NULL_SPEC_15(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, _)                           \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7>

#define NULL_SPEC_17(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, _)               \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8>

#define NULL_SPEC_19(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9, _)   \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9>

#define NULL_SPEC_21(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, _)                                          \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10>

#define NULL_SPEC_23(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, _)                            \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11>

#define NULL_SPEC_25(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, spec12, tv12, _)              \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12>

#define NULL_SPEC_27(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,   \
                     _)                                                        \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13>

#define NULL_SPEC_29(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,   \
                     spec14, tv14, _)                                          \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14>

#define NULL_SPEC_31(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,   \
                     spec14, tv14, spec15, tv15, _)                            \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14, spec15 tv15>

#define NULL_SPEC_33(spec1, tv1, spec2, tv2, spec3, tv3, spec4, tv4, spec5,    \
                     tv5, spec6, tv6, spec7, tv7, spec8, tv8, spec9, tv9,      \
                     spec10, tv10, spec11, tv11, spec12, tv12, spec13, tv13,   \
                     spec14, tv14, spec15, tv15, spec16, tv16, _)              \
  template <spec1 tv1, spec2 tv2, spec3 tv3, spec4 tv4, spec5 tv5, spec6 tv6,  \
            spec7 tv7, spec8 tv8, spec9 tv9, spec10 tv10, spec11 tv11,         \
            spec12 tv12, spec13 tv13, spec14 tv14, spec15 tv15, spec16 tv16>

#define ENTER_NS() VFUNC_ALT(ENTER_NS_, NAMESPACE(_))

#define ENTER_NS_1(_)

#define ENTER_NS_2(ns1, _) namespace ns1 {

#define ENTER_NS_3(ns1, ns2, _)                                                \
  namespace ns1 {                                                              \
  namespace ns2 {

#define ENTER_NS_4(ns1, ns2, ns3, _)                                           \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {

#define ENTER_NS_5(ns1, ns2, ns3, ns4, _)                                      \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {                                                              \
  namespace ns4 {

#define ENTER_NS_6(ns1, ns2, ns3, ns4, ns5, _)                                 \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {                                                              \
  namespace ns4 {                                                              \
  namespace ns5 {

#define ENTER_NS_7(ns1, ns2, ns3, ns4, ns5, ns6, _)                            \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {                                                              \
  namespace ns4 {                                                              \
  namespace ns5 {                                                              \
  namespace ns6 {

#define ENTER_NS_8(ns1, ns2, ns3, ns4, ns5, ns6, ns7, _)                       \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {                                                              \
  namespace ns4 {                                                              \
  namespace ns5 {                                                              \
  namespace ns6 {                                                              \
  namespace ns7 {

#define ENTER_NS_9(ns1, ns2, ns3, ns4, ns5, ns6, ns7, ns8, _)                  \
  namespace ns1 {                                                              \
  namespace ns2 {                                                              \
  namespace ns3 {                                                              \
  namespace ns4 {                                                              \
  namespace ns5 {                                                              \
  namespace ns6 {                                                              \
  namespace ns7 {                                                              \
  namespace ns8 {

#define LEAVE_NS() VFUNC_ALT(LEAVE_NS_, NAMESPACE(_))

#define LEAVE_NS_1(_)

#define LEAVE_NS_2(ns1, _) }

#define LEAVE_NS_3(ns1, ns2, _)                                                \
  }                                                                            \
  }

#define LEAVE_NS_4(ns1, ns2, ns3, _)                                           \
  }                                                                            \
  }                                                                            \
  }

#define LEAVE_NS_5(ns1, ns2, ns3, ns4, _)                                      \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#define LEAVE_NS_6(ns1, ns2, ns3, ns4, ns5, _)                                 \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#define LEAVE_NS_7(ns1, ns2, ns3, ns4, ns5, ns6, _)                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#define LEAVE_NS_8(ns1, ns2, ns3, ns4, ns5, ns6, ns7, _)                       \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#define LEAVE_NS_9(ns1, ns2, ns3, ns4, ns5, ns6, ns7, ns8, _)                  \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }                                                                            \
  }

#define NS_PREFIX() VFUNC_ALT(NS_PREFIX_, NAMESPACE(_))

#define NS_PREFIX_1(_)

#define NS_PREFIX_2(ns1, _) ns1::

#define NS_PREFIX_3(ns1, ns2, _) ns1::ns2::

#define NS_PREFIX_4(ns1, ns2, ns3, _) ns1::ns2::ns3::

#define NS_PREFIX_5(ns1, ns2, ns3, ns4, _) ns1::ns2::ns3::ns4::

#define NS_PREFIX_6(ns1, ns2, ns3, ns4, ns5, _) ns1::ns2::ns3::ns4::ns5::

#define NS_PREFIX_7(ns1, ns2, ns3, ns4, ns5, ns6, _)                           \
  ns1::ns2::ns3::ns4::ns5::ns6::

#define NS_PREFIX_8(ns1, ns2, ns3, ns4, ns5, ns6, ns7, _)                      \
  ns1::ns2::ns3::ns4::ns5::ns6::ns7::

#define NS_PREFIX_9(ns1, ns2, ns3, ns4, ns5, ns6, ns7, ns8, _)                 \
  ns1::ns2::ns3::ns4::ns5::ns6::ns7::ns8::
