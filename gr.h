#ifndef GR_H
#define GR_H

#ifdef GR_INLINES_C
#define GR_INLINE FLINT_DLL
#else
#define GR_INLINE static __inline__
#endif

#include <string.h>
#include <assert.h>
#include "flint/flint.h"
#include "flint/fmpz.h"
#include "flint/fmpq.h"
#include "flint/nmod_vec.h"
#include "flint/ulong_extras.h"
#include "flint/profiler.h"

/* for truth_t */
#include "calcium.h"

#ifdef __cplusplus
 extern "C" {
#endif

GR_INLINE truth_t truth_and(truth_t x, truth_t y)
{
    if (x == T_FALSE || x == T_FALSE)
        return T_FALSE;
    if (x == T_TRUE && y == T_TRUE)
        return T_TRUE;
    return T_UNKNOWN;
}

GR_INLINE truth_t truth_or(truth_t x, truth_t y)
{
    if (x == T_TRUE || x == T_TRUE)
        return T_TRUE;
    if (x == T_FALSE && y == T_FALSE)
        return T_FALSE;
    return T_UNKNOWN;
}

GR_INLINE truth_t truth_not(truth_t x)
{
    if (x == T_TRUE)
        return T_FALSE;
    if (x == T_FALSE)
        return T_TRUE;
    return T_UNKNOWN;
}

typedef int (*gr_funcptr)(void);

/* Copied from Calcium: stream interface allows simple file or string IO. */

typedef struct
{
    FILE * fp;
    char * s;
    slong len;
    slong alloc;
}
gr_stream_struct;

typedef gr_stream_struct gr_stream_t[1];

void gr_stream_init_file(gr_stream_t out, FILE * fp);
void gr_stream_init_str(gr_stream_t out);
void gr_stream_write(gr_stream_t out, const char * s);
void gr_stream_write_si(gr_stream_t out, slong x);
void gr_stream_write_ui(gr_stream_t out, ulong x);
void gr_stream_write_free(gr_stream_t out, char * s);
void gr_stream_write_fmpz(gr_stream_t out, const fmpz_t x);

#define GR_SUCCESS 0
#define GR_DOMAIN 1
#define GR_UNABLE 2
#define GR_WRONG 4

#define MUST_SUCCEED(expr) assert ((expr) == GR_SUCCESS);

typedef void * gr_ptr;
typedef const void * gr_srcptr;
typedef void * gr_ctx_ptr;

#define GR_ENTRY(vec, i, size) ((void *) (((char *) (vec)) + ((i) * (size))))

GR_INLINE int gr_not_implemented(void) { return GR_UNABLE; }

typedef enum
{
    GR_METHOD_CTX_WRITE,
    GR_METHOD_CTX_CLEAR,

    GR_METHOD_INIT,
    GR_METHOD_CLEAR,
    GR_METHOD_SWAP,
    GR_METHOD_RANDTEST,
    GR_METHOD_WRITE,

    GR_METHOD_ZERO,
    GR_METHOD_ONE,
    GR_METHOD_NEG_ONE,

    GR_METHOD_IS_ZERO,
    GR_METHOD_IS_ONE,
    GR_METHOD_IS_NEG_ONE,

    GR_METHOD_EQUAL,

    GR_METHOD_SET,
    GR_METHOD_SET_SI,
    GR_METHOD_SET_UI,
    GR_METHOD_SET_FMPZ,
    GR_METHOD_SET_FMPQ,

    GR_METHOD_GET_SI,
    GR_METHOD_GET_UI,
    GR_METHOD_GET_FMPZ,
    GR_METHOD_GET_FMPQ,

    GR_METHOD_IS_INTEGER,
    GR_METHOD_IS_RATIONAL,

    GR_METHOD_NEG,

    GR_METHOD_ADD,
    GR_METHOD_ADD_SI,
    GR_METHOD_ADD_UI,
    GR_METHOD_ADD_FMPZ,
    GR_METHOD_ADD_FMPQ,

    GR_METHOD_SUB,
    GR_METHOD_SUB_SI,
    GR_METHOD_SUB_UI,
    GR_METHOD_SUB_FMPZ,
    GR_METHOD_SUB_FMPQ,

    GR_METHOD_MUL,
    GR_METHOD_MUL_SI,
    GR_METHOD_MUL_UI,
    GR_METHOD_MUL_FMPZ,
    GR_METHOD_MUL_FMPQ,

    GR_METHOD_ADDMUL,
    GR_METHOD_SUBMUL,

    GR_METHOD_MUL_TWO,
    GR_METHOD_SQR,

    GR_METHOD_IS_INVERTIBLE,
    GR_METHOD_INV,

    GR_METHOD_DIV,
    GR_METHOD_DIV_SI,
    GR_METHOD_DIV_UI,
    GR_METHOD_DIV_FMPZ,
    GR_METHOD_DIV_FMPQ,

    GR_METHOD_DIVEXACT,
    GR_METHOD_DIVEXACT_SI,
    GR_METHOD_DIVEXACT_UI,
    GR_METHOD_DIVEXACT_FMPZ,
    GR_METHOD_DIVEXACT_FMPQ,

    GR_METHOD_POW_UI,
    GR_METHOD_POW_SI,
    GR_METHOD_POW_FMPZ,
    GR_METHOD_POW_FMPQ,
    GR_METHOD_POW,

    GR_METHOD_IS_SQUARE,
    GR_METHOD_SQRT,
    GR_METHOD_RSQRT,

    GR_METHOD_EXP,
    GR_METHOD_LOG,

    /* Vector methods */
    GR_METHOD_VEC_INIT,
    GR_METHOD_VEC_CLEAR,
    GR_METHOD_VEC_SWAP,
    GR_METHOD_VEC_SET,
    GR_METHOD_VEC_ZERO,
    GR_METHOD_VEC_EQUAL,
    GR_METHOD_VEC_IS_ZERO,
    GR_METHOD_VEC_NEG,
    GR_METHOD_VEC_ADD,
    GR_METHOD_VEC_SUB,
    GR_METHOD_VEC_SCALAR_MUL,
    GR_METHOD_VEC_SCALAR_ADDMUL,
    GR_METHOD_VEC_SCALAR_SUBMUL,
    GR_METHOD_VEC_SCALAR_ADDMUL_SI,
    GR_METHOD_VEC_SCALAR_SUBMUL_SI,
    GR_METHOD_VEC_DOT,
    GR_METHOD_VEC_DOT_REV,

    /* Polynomial methods */
    GR_METHOD_POLY_MULLOW,

    GR_METHOD_TAB_SIZE
}
gr_method;

typedef gr_funcptr gr_static_method_table[GR_METHOD_TAB_SIZE];

typedef struct
{
    gr_method index;
    gr_funcptr function;
}
gr_method_tab_input;

void gr_method_tab_init(gr_funcptr * methods, gr_method_tab_input * tab);

/* Ring propery flags. These are not cumulative. (?) */

#define GR_COMMUTATIVE_RING  UWORD(1)
#define GR_INTEGRAL_DOMAIN   UWORD(2)
#define GR_FIELD             UWORD(4)
#define GR_FINITE_RING       UWORD(8)

/* Identify specific rings/fields. */

#define GR_WHICH_RING_CUSTOM             0
#define GR_WHICH_RING_ZZ                 1
#define GR_WHICH_RING_ZZ_MOD             2
#define GR_WHICH_RING_QQ                 3
#define GR_WHICH_RING_RR                 4
#define GR_WHICH_RING_CC                 5
#define GR_WHICH_RING_RR_ALGEBRAIC       6
#define GR_WHICH_RING_CC_ALGEBRAIC       7

typedef struct
{
    ulong flags;
    ulong which_ring;
    ssize_t sizeof_elem;
    void * elem_ctx;
    gr_funcptr * methods;
    ulong size_limit;
}
gr_ctx_struct;

typedef gr_ctx_struct gr_ctx_t[1];


/* Typedefs for method function pointers. */

typedef void ((*gr_method_init_clear_op)(gr_ptr, gr_ctx_ptr));
typedef void ((*gr_method_swap_op)(gr_ptr, gr_ptr, gr_ctx_ptr));
typedef int ((*gr_method_ctx)(gr_ctx_ptr));
typedef int ((*gr_method_ctx_stream)(gr_stream_t, gr_ctx_ptr));
typedef int ((*gr_method_stream_in)(gr_stream_t, gr_srcptr, gr_ctx_ptr));
typedef int ((*gr_method_randtest)(gr_ptr, flint_rand_t state, const void * options, gr_ctx_ptr));
typedef int ((*gr_method_constant_op)(gr_ptr, gr_ctx_ptr));
typedef int ((*gr_method_unary_op)(gr_ptr, gr_srcptr, gr_ctx_ptr));
typedef int ((*gr_method_unary_op_si)(gr_ptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_unary_op_ui)(gr_ptr, ulong, gr_ctx_ptr));
typedef int ((*gr_method_unary_op_fmpz)(gr_ptr, const fmpz_t, gr_ctx_ptr));
typedef int ((*gr_method_unary_op_fmpq)(gr_ptr, const fmpq_t, gr_ctx_ptr));
typedef int ((*gr_method_unary_op_get_si)(slong * gr_ptr, gr_ctx_ptr));
typedef int ((*gr_method_binary_op)(gr_ptr, gr_srcptr, gr_srcptr, gr_ctx_ptr));
typedef int ((*gr_method_binary_op_si)(gr_ptr, gr_srcptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_binary_op_ui)(gr_ptr, gr_srcptr, ulong, gr_ctx_ptr));
typedef int ((*gr_method_binary_op_fmpz)(gr_ptr, gr_srcptr, const fmpz_t, gr_ctx_ptr));
typedef int ((*gr_method_binary_op_fmpq)(gr_ptr, gr_srcptr, const fmpq_t, gr_ctx_ptr));
typedef truth_t ((*gr_method_unary_predicate)(gr_srcptr, gr_ctx_ptr));
typedef truth_t ((*gr_method_binary_predicate)(gr_srcptr, gr_srcptr, gr_ctx_ptr));

typedef void ((*gr_method_vec_init_clear_op)(gr_ptr, slong, gr_ctx_ptr));
typedef void ((*gr_method_vec_swap_op)(gr_ptr, gr_ptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_vec_constant_op)(gr_ptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_vec_op)(gr_ptr, gr_srcptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_vec_vec_op)(gr_ptr, gr_srcptr, gr_srcptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_vec_scalar_op)(gr_ptr, gr_srcptr, slong, gr_srcptr, gr_ctx_ptr));
typedef int ((*gr_method_vec_scalar_op_si)(gr_ptr, gr_srcptr, slong, slong, gr_ctx_ptr));
typedef truth_t ((*gr_method_vec_predicate)(gr_srcptr, slong, gr_ctx_ptr));
typedef truth_t ((*gr_method_vec_vec_predicate)(gr_srcptr, gr_srcptr, slong, gr_ctx_ptr));
typedef int ((*gr_method_vec_dot_op)(gr_ptr, gr_srcptr, int, gr_srcptr, gr_srcptr, slong, gr_ctx_ptr));

typedef int ((*gr_method_poly_binary_trunc_op)(gr_ptr, gr_srcptr, slong, gr_srcptr, slong, slong, gr_ctx_ptr));


/* Macros to retrieve methods (with correct call signature) from context object. */

#define GR_CTX_OP(ctx, NAME) (((gr_method_ctx *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_CTX_STREAM(ctx, NAME) (((gr_method_ctx_stream *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_STREAM_IN(ctx, NAME) (((gr_method_stream_in *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_RANDTEST(ctx, NAME) (((gr_method_randtest *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_INIT_CLEAR_OP(ctx, NAME) (((gr_method_init_clear_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_SWAP_OP(ctx, NAME) (((gr_method_swap_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_CONSTANT_OP(ctx, NAME) (((gr_method_constant_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP(ctx, NAME) (((gr_method_unary_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP_SI(ctx, NAME) (((gr_method_unary_op_si *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP_UI(ctx, NAME) (((gr_method_unary_op_ui *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP_FMPZ(ctx, NAME) (((gr_method_unary_op_fmpz *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP_FMPQ(ctx, NAME) (((gr_method_unary_op_fmpq *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_OP_GET_SI(ctx, NAME) (((gr_method_unary_op_get_si *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_OP(ctx, NAME) (((gr_method_binary_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_OP_SI(ctx, NAME) (((gr_method_binary_op_si *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_OP_UI(ctx, NAME) (((gr_method_binary_op_ui *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_OP_FMPZ(ctx, NAME) (((gr_method_binary_op_fmpz *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_OP_FMPQ(ctx, NAME) (((gr_method_binary_op_fmpq *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_UNARY_PREDICATE(ctx, NAME) (((gr_method_unary_predicate *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_BINARY_PREDICATE(ctx, NAME) (((gr_method_binary_predicate *) ctx->methods)[GR_METHOD_ ## NAME])

#define GR_VEC_INIT_CLEAR_OP(ctx, NAME) (((gr_method_vec_init_clear_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_SWAP_OP(ctx, NAME) (((gr_method_vec_swap_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_CONSTANT_OP(ctx, NAME) (((gr_method_vec_constant_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_OP(ctx, NAME) (((gr_method_vec_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_VEC_OP(ctx, NAME) (((gr_method_vec_vec_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_SCALAR_OP(ctx, NAME) (((gr_method_vec_scalar_op *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_SCALAR_OP_SI(ctx, NAME) (((gr_method_vec_scalar_op_si *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_PREDICATE(ctx, NAME) (((gr_method_vec_predicate *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_VEC_PREDICATE(ctx, NAME) (((gr_method_vec_vec_predicate *) ctx->methods)[GR_METHOD_ ## NAME])
#define GR_VEC_DOT_OP(ctx, NAME) (((gr_method_vec_dot_op *) ctx->methods)[GR_METHOD_ ## NAME])

#define GR_POLY_BINARY_TRUNC_OP(ctx, NAME) (((gr_method_poly_binary_trunc_op *) ctx->methods)[GR_METHOD_ ## NAME])


/* Wrappers to call methods. */

GR_INLINE int gr_ctx_clear(gr_ctx_t ctx) { return GR_CTX_OP(ctx, CTX_CLEAR)(ctx); }
GR_INLINE int gr_ctx_write(gr_stream_t out, gr_ctx_t ctx) { return GR_CTX_STREAM(ctx, CTX_WRITE)(out, ctx); }

GR_INLINE void gr_init(gr_ptr res, gr_ctx_t ctx) { GR_INIT_CLEAR_OP(ctx, INIT)(res, ctx); }
GR_INLINE void gr_clear(gr_ptr res, gr_ctx_t ctx) { GR_INIT_CLEAR_OP(ctx, CLEAR)(res, ctx); }
GR_INLINE void gr_swap(gr_ptr x, gr_ptr y, gr_ctx_t ctx) { GR_SWAP_OP(ctx, SWAP)(x, y, ctx); }

GR_INLINE int gr_randtest(gr_ptr x, flint_rand_t state, const void * options, gr_ctx_t ctx) { return GR_RANDTEST(ctx, RANDTEST)(x, state, options, ctx); }
GR_INLINE int gr_write(gr_stream_t out, gr_srcptr x, gr_ctx_t ctx) { return GR_STREAM_IN(ctx, WRITE)(out, x, ctx); }
GR_INLINE int gr_zero(gr_ptr res, gr_ctx_t ctx) { return GR_CONSTANT_OP(ctx, ZERO)(res, ctx); }
GR_INLINE int gr_one(gr_ptr res, gr_ctx_t ctx) { return GR_CONSTANT_OP(ctx, ONE)(res, ctx); }
GR_INLINE int gr_neg_one(gr_ptr res, gr_ctx_t ctx) { return GR_CONSTANT_OP(ctx, NEG_ONE)(res, ctx); }

GR_INLINE int gr_set_si(gr_ptr res, slong x, gr_ctx_t ctx) { return GR_UNARY_OP_SI(ctx, SET_SI)(res, x, ctx); }
GR_INLINE int gr_set_ui(gr_ptr res, ulong x, gr_ctx_t ctx) { return GR_UNARY_OP_SI(ctx, SET_UI)(res, x, ctx); }
GR_INLINE int gr_set_fmpz(gr_ptr res, const fmpz_t x, gr_ctx_t ctx) { return GR_UNARY_OP_FMPZ(ctx, SET_FMPZ)(res, x, ctx); }
GR_INLINE int gr_set_fmpq(gr_ptr res, const fmpq_t x, gr_ctx_t ctx) { return GR_UNARY_OP_FMPQ(ctx, SET_FMPQ)(res, x, ctx); }

GR_INLINE truth_t gr_is_zero(gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_PREDICATE(ctx, IS_ZERO)(x, ctx); }
GR_INLINE truth_t gr_is_one(gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_PREDICATE(ctx, IS_ONE)(x, ctx); }
GR_INLINE truth_t gr_is_neg_one(gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_PREDICATE(ctx, IS_NEG_ONE)(x, ctx); }

GR_INLINE truth_t gr_equal(gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_PREDICATE(ctx, EQUAL)(x, y, ctx); }

GR_INLINE int gr_set(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, SET)(res, x, ctx); }
GR_INLINE int gr_neg(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, NEG)(res, x, ctx); }

GR_INLINE int gr_add(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, ADD)(res, x, y, ctx); }
GR_INLINE int gr_add_ui(gr_ptr res, gr_srcptr x, ulong y, gr_ctx_t ctx) { return GR_BINARY_OP_UI(ctx, ADD_UI)(res, x, y, ctx); }
GR_INLINE int gr_add_si(gr_ptr res, gr_srcptr x, slong y, gr_ctx_t ctx) { return GR_BINARY_OP_SI(ctx, ADD_SI)(res, x, y, ctx); }
GR_INLINE int gr_add_fmpz(gr_ptr res, gr_srcptr x, const fmpz_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPZ(ctx, ADD_FMPZ)(res, x, y, ctx); }
GR_INLINE int gr_add_fmpq(gr_ptr res, gr_srcptr x, const fmpq_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPQ(ctx, ADD_FMPQ)(res, x, y, ctx); }

GR_INLINE int gr_sub(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, SUB)(res, x, y, ctx); }
GR_INLINE int gr_sub_ui(gr_ptr res, gr_srcptr x, ulong y, gr_ctx_t ctx) { return GR_BINARY_OP_UI(ctx, SUB_UI)(res, x, y, ctx); }
GR_INLINE int gr_sub_si(gr_ptr res, gr_srcptr x, slong y, gr_ctx_t ctx) { return GR_BINARY_OP_SI(ctx, SUB_SI)(res, x, y, ctx); }
GR_INLINE int gr_sub_fmpz(gr_ptr res, gr_srcptr x, const fmpz_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPZ(ctx, SUB_FMPZ)(res, x, y, ctx); }
GR_INLINE int gr_sub_fmpq(gr_ptr res, gr_srcptr x, const fmpq_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPQ(ctx, SUB_FMPQ)(res, x, y, ctx); }

GR_INLINE int gr_mul(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, MUL)(res, x, y, ctx); }
GR_INLINE int gr_mul_ui(gr_ptr res, gr_srcptr x, ulong y, gr_ctx_t ctx) { return GR_BINARY_OP_UI(ctx, MUL_UI)(res, x, y, ctx); }
GR_INLINE int gr_mul_si(gr_ptr res, gr_srcptr x, slong y, gr_ctx_t ctx) { return GR_BINARY_OP_SI(ctx, MUL_SI)(res, x, y, ctx); }
GR_INLINE int gr_mul_fmpz(gr_ptr res, gr_srcptr x, const fmpz_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPZ(ctx, MUL_FMPZ)(res, x, y, ctx); }
GR_INLINE int gr_mul_fmpq(gr_ptr res, gr_srcptr x, const fmpq_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPQ(ctx, MUL_FMPQ)(res, x, y, ctx); }

GR_INLINE int gr_addmul(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, ADDMUL)(res, x, y, ctx); }
GR_INLINE int gr_submul(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, SUBMUL)(res, x, y, ctx); }

GR_INLINE int gr_mul_two(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, MUL_TWO)(res, x, ctx); }
GR_INLINE int gr_sqr(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, SQR)(res, x, ctx); }

GR_INLINE int gr_div(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, DIV)(res, x, y, ctx); }
GR_INLINE int gr_div_ui(gr_ptr res, gr_srcptr x, ulong y, gr_ctx_t ctx) { return GR_BINARY_OP_UI(ctx, DIV_UI)(res, x, y, ctx); }
GR_INLINE int gr_div_si(gr_ptr res, gr_srcptr x, slong y, gr_ctx_t ctx) { return GR_BINARY_OP_SI(ctx, DIV_SI)(res, x, y, ctx); }
GR_INLINE int gr_div_fmpz(gr_ptr res, gr_srcptr x, const fmpz_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPZ(ctx, DIV_FMPZ)(res, x, y, ctx); }
GR_INLINE int gr_div_fmpq(gr_ptr res, gr_srcptr x, const fmpq_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPQ(ctx, DIV_FMPQ)(res, x, y, ctx); }

GR_INLINE int gr_inv(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, INV)(res, x, ctx); }
GR_INLINE truth_t gr_is_invertible(gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_PREDICATE(ctx, IS_INVERTIBLE)(x, ctx); }

GR_INLINE int gr_pow(gr_ptr res, gr_srcptr x, gr_srcptr y, gr_ctx_t ctx) { return GR_BINARY_OP(ctx, POW)(res, x, y, ctx); }
GR_INLINE int gr_pow_ui(gr_ptr res, gr_srcptr x, ulong y, gr_ctx_t ctx) { return GR_BINARY_OP_UI(ctx, POW_UI)(res, x, y, ctx); }
GR_INLINE int gr_pow_si(gr_ptr res, gr_srcptr x, slong y, gr_ctx_t ctx) { return GR_BINARY_OP_SI(ctx, POW_SI)(res, x, y, ctx); }
GR_INLINE int gr_pow_fmpz(gr_ptr res, gr_srcptr x, const fmpz_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPZ(ctx, POW_FMPZ)(res, x, y, ctx); }
GR_INLINE int gr_pow_fmpq(gr_ptr res, gr_srcptr x, const fmpq_t y, gr_ctx_t ctx) { return GR_BINARY_OP_FMPQ(ctx, POW_FMPQ)(res, x, y, ctx); }

GR_INLINE int gr_sqrt(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, SQRT)(res, x, ctx); }
GR_INLINE int gr_rsqrt(gr_ptr res, gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_OP(ctx, RSQRT)(res, x, ctx); }
GR_INLINE truth_t gr_is_square(gr_srcptr x, gr_ctx_t ctx) { return GR_UNARY_PREDICATE(ctx, IS_SQUARE)(x, ctx); }

GR_INLINE void _gr_vec_init(gr_ptr vec, slong len, gr_ctx_t ctx) { GR_VEC_INIT_CLEAR_OP(ctx, VEC_INIT)(vec, len, ctx); }
GR_INLINE void _gr_vec_clear(gr_ptr vec, slong len, gr_ctx_t ctx) { GR_VEC_INIT_CLEAR_OP(ctx, VEC_CLEAR)(vec, len, ctx); }
GR_INLINE void _gr_vec_swap(gr_ptr vec1, gr_ptr vec2, slong len, gr_ctx_t ctx) { GR_VEC_SWAP_OP(ctx, VEC_SWAP)(vec1, vec2, len, ctx); }

GR_INLINE int _gr_vec_zero(gr_ptr vec, slong len, gr_ctx_t ctx) { return GR_VEC_CONSTANT_OP(ctx, VEC_ZERO)(vec, len, ctx); }
GR_INLINE int _gr_vec_set(gr_ptr res, gr_srcptr src, slong len, gr_ctx_t ctx) { return GR_VEC_OP(ctx, VEC_SET)(res, src, len, ctx); }
GR_INLINE int _gr_vec_neg(gr_ptr res, gr_srcptr src, slong len, gr_ctx_t ctx) { return GR_VEC_OP(ctx, VEC_NEG)(res, src, len, ctx); }
GR_INLINE int _gr_vec_add(gr_ptr res, gr_srcptr src1, gr_srcptr src2, slong len, gr_ctx_t ctx) { return GR_VEC_VEC_OP(ctx, VEC_ADD)(res, src1, src2, len, ctx); }
GR_INLINE int _gr_vec_sub(gr_ptr res, gr_srcptr src1, gr_srcptr src2, slong len, gr_ctx_t ctx) { return GR_VEC_VEC_OP(ctx, VEC_SUB)(res, src1, src2, len, ctx); }
GR_INLINE int _gr_vec_scalar_mul(gr_ptr vec1, gr_srcptr vec2, slong len, gr_srcptr c, gr_ctx_t ctx) { return GR_VEC_SCALAR_OP(ctx, VEC_SCALAR_MUL)(vec1, vec2, len, c, ctx); }
GR_INLINE int _gr_vec_scalar_addmul(gr_ptr vec1, gr_srcptr vec2, slong len, gr_srcptr c, gr_ctx_t ctx) { return GR_VEC_SCALAR_OP(ctx, VEC_SCALAR_ADDMUL)(vec1, vec2, len, c, ctx); }
GR_INLINE int _gr_vec_scalar_submul(gr_ptr vec1, gr_srcptr vec2, slong len, gr_srcptr c, gr_ctx_t ctx) { return GR_VEC_SCALAR_OP(ctx, VEC_SCALAR_SUBMUL)(vec1, vec2, len, c, ctx); }
GR_INLINE int _gr_vec_scalar_addmul_si(gr_ptr vec1, gr_srcptr vec2, slong len, slong c, gr_ctx_t ctx) { return GR_VEC_SCALAR_OP_SI(ctx, VEC_SCALAR_ADDMUL_SI)(vec1, vec2, len, c, ctx); }
GR_INLINE int _gr_vec_scalar_submul_si(gr_ptr vec1, gr_srcptr vec2, slong len, slong c, gr_ctx_t ctx) { return GR_VEC_SCALAR_OP_SI(ctx, VEC_SCALAR_SUBMUL_SI)(vec1, vec2, len, c, ctx); }
GR_INLINE truth_t _gr_vec_equal(gr_srcptr vec1, gr_srcptr vec2, slong len, gr_ctx_t ctx) { return GR_VEC_VEC_PREDICATE(ctx, VEC_EQUAL)(vec1, vec2, len, ctx); }
GR_INLINE truth_t _gr_vec_is_zero(gr_srcptr vec, slong len, gr_ctx_t ctx) { return GR_VEC_PREDICATE(ctx, VEC_IS_ZERO)(vec, len, ctx); }
GR_INLINE int _gr_vec_dot(gr_ptr res, gr_srcptr initial, int subtract, gr_srcptr vec1, gr_srcptr vec2, slong len, gr_ctx_t ctx) { return GR_VEC_DOT_OP(ctx, VEC_DOT)(res, initial, subtract, vec1, vec2, len, ctx); }
GR_INLINE int _gr_vec_dot_rev(gr_ptr res, gr_srcptr initial, int subtract, gr_srcptr vec1, gr_srcptr vec2, slong len, gr_ctx_t ctx) { return GR_VEC_DOT_OP(ctx, VEC_DOT_REV)(res, initial, subtract, vec1, vec2, len, ctx); }

GR_INLINE int
_gr_poly_mullow(gr_ptr res, gr_srcptr poly1, slong len1, gr_srcptr poly2, slong len2, slong len, gr_ctx_t ctx)
{
    return GR_POLY_BINARY_TRUNC_OP(ctx, POLY_MULLOW)(res, poly1, len1, poly2, len2, len, ctx);
}

/* todo: could allow overloading this as well */
GR_INLINE int
_gr_vec_randtest(gr_ptr res, flint_rand_t state, slong len, void * options, gr_ctx_t ctx)
{
    int status;
    slong i, sz;

    sz = ctx->sizeof_elem;

    status = GR_SUCCESS;
    for (i = 0; i < len; i++)
    {
        if (n_randint(state, 2))
            status |= gr_zero(GR_ENTRY(res, i, sz), ctx);
        else
            status |= gr_randtest(GR_ENTRY(res, i, sz), state, options, ctx);
    }

    return status;
}

int gr_ctx_print(gr_ctx_t ctx);
int gr_ctx_println(gr_ctx_t ctx);
int gr_print(gr_srcptr x, gr_ctx_t ctx);
int gr_println(gr_srcptr x, gr_ctx_t ctx);
int gr_ctx_get_str(char ** s, gr_ctx_t ctx);
int gr_get_str(char ** s, gr_srcptr x, gr_ctx_t ctx);

/* Macros for allocating temporary variables on the stack. */
/* todo: use vector init functions when provided */

#define GR_TMP_START TMP_INIT; TMP_START;
#define GR_TMP_ALLOC TMP_ALLOC
#define GR_TMP_END TMP_END

#define GR_TMP_INIT_VEC(vec, len, ctx) \
    do { \
        gr_method_vec_init_clear_op vec_init = GR_VEC_INIT_CLEAR_OP(ctx, VEC_INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        (vec) = (gr_ptr) GR_TMP_ALLOC((len) * _gr_elem_size); \
        vec_init((vec), (len), (ctx)); \
    } while (0)

#define GR_TMP_CLEAR_VEC(vec, len, ctx) \
    do { \
        gr_method_vec_init_clear_op vec_clear = GR_VEC_INIT_CLEAR_OP(ctx, VEC_CLEAR); \
        vec_clear((vec), (len), (ctx)); \
    } while (0)

#define GR_TMP_INIT1(x1, ctx) \
    do { \
        gr_method_init_clear_op init = GR_INIT_CLEAR_OP(ctx, INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        x1 = (gr_ptr) GR_TMP_ALLOC(1 * _gr_elem_size); \
        init(x1, (ctx)); \
    } while (0)

#define GR_TMP_INIT2(x1, x2, ctx) \
    do { \
        gr_method_init_clear_op init = GR_INIT_CLEAR_OP(ctx, INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        x1 = (gr_ptr) GR_TMP_ALLOC(2 * _gr_elem_size); \
        x2 = (gr_ptr) ((char *) x1 + _gr_elem_size); \
        init(x1, (ctx)); \
        init(x2, (ctx)); \
    } while (0)

#define GR_TMP_INIT3(x1, x2, x3, ctx) \
    do { \
        gr_method_init_clear_op init = GR_INIT_CLEAR_OP(ctx, INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        x1 = (gr_ptr) GR_TMP_ALLOC(3 * _gr_elem_size); \
        x2 = (gr_ptr) ((char *) x1 + _gr_elem_size); \
        x3 = (gr_ptr) ((char *) x2 + _gr_elem_size); \
        init(x1, (ctx)); \
        init(x2, (ctx)); \
        init(x3, (ctx)); \
    } while (0)

#define GR_TMP_INIT4(x1, x2, x3, x4, ctx) \
    do { \
        gr_method_init_clear_op init = GR_INIT_CLEAR_OP(ctx, INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        x1 = (gr_ptr) GR_TMP_ALLOC(4 * _gr_elem_size); \
        x2 = (gr_ptr) ((char *) x1 + _gr_elem_size); \
        x3 = (gr_ptr) ((char *) x2 + _gr_elem_size); \
        x4 = (gr_ptr) ((char *) x3 + _gr_elem_size); \
        init(x1, (ctx)); \
        init(x2, (ctx)); \
        init(x3, (ctx)); \
        init(x4, (ctx)); \
    } while (0)

#define GR_TMP_INIT5(x1, x2, x3, x4, x5, ctx) \
    do { \
        gr_method_init_clear_op init = GR_INIT_CLEAR_OP(ctx, INIT); \
        ssize_t _gr_elem_size = (ctx)->sizeof_elem; \
        x1 = (gr_ptr) GR_TMP_ALLOC(5 * _gr_elem_size); \
        x2 = (gr_ptr) ((char *) x1 + _gr_elem_size); \
        x3 = (gr_ptr) ((char *) x2 + _gr_elem_size); \
        x4 = (gr_ptr) ((char *) x3 + _gr_elem_size); \
        x5 = (gr_ptr) ((char *) x4 + _gr_elem_size); \
        init(x1, (ctx)); \
        init(x2, (ctx)); \
        init(x3, (ctx)); \
        init(x4, (ctx)); \
        init(x5, (ctx)); \
    } while (0)

#define GR_TMP_CLEAR1(x1, ctx) \
    do { \
        gr_method_init_clear_op clear = GR_INIT_CLEAR_OP(ctx, CLEAR); \
        clear(x1, (ctx)); \
    } while (0)

#define GR_TMP_CLEAR2(x1, x2, ctx) \
    do { \
        gr_method_init_clear_op clear = GR_INIT_CLEAR_OP(ctx, CLEAR); \
        clear(x1, (ctx)); \
        clear(x2, (ctx)); \
    } while (0)

#define GR_TMP_CLEAR3(x1, x2, x3, ctx) \
    do { \
        gr_method_init_clear_op clear = GR_INIT_CLEAR_OP(ctx, CLEAR); \
        clear(x1, (ctx)); \
        clear(x2, (ctx)); \
        clear(x3, (ctx)); \
    } while (0)

#define GR_TMP_CLEAR4(x1, x2, x3, x4, ctx) \
    do { \
        gr_method_init_clear_op clear = GR_INIT_CLEAR_OP(ctx, CLEAR); \
        clear(x1, (ctx)); \
        clear(x2, (ctx)); \
        clear(x3, (ctx)); \
        clear(x4, (ctx)); \
    } while (0)

#define GR_TMP_CLEAR5(x1, x2, x3, x4, x5, ctx) \
    do { \
        gr_method_init_clear_op clear = GR_INIT_CLEAR_OP(ctx, CLEAR); \
        clear(x1, (ctx)); \
        clear(x2, (ctx)); \
        clear(x3, (ctx)); \
        clear(x4, (ctx)); \
        clear(x5, (ctx)); \
    } while (0)

/* Some base rings */

void gr_ctx_init_random(gr_ctx_t ctx, flint_rand_t state);

void gr_ctx_init_fmpz(gr_ctx_t ctx);
void gr_ctx_init_fmpq(gr_ctx_t ctx);
void gr_ctx_init_nmod8(gr_ctx_t ctx, unsigned char n);

void gr_ctx_init_real_qqbar(gr_ctx_t ctx);
void gr_ctx_init_complex_qqbar(gr_ctx_t ctx);

void gr_ctx_init_real_arb(gr_ctx_t ctx, slong prec);
void gr_ctx_init_complex_acb(gr_ctx_t ctx, slong prec);

void gr_ctx_init_real_ca(gr_ctx_t ctx);
void gr_ctx_init_complex_ca(gr_ctx_t ctx);
void gr_ctx_init_real_algebraic_ca(gr_ctx_t ctx);
void gr_ctx_init_complex_algebraic_ca(gr_ctx_t ctx);

/* Generic polynomial ring */

typedef struct
{
    gr_ctx_struct * base_ring;
    slong degree_limit;
}
polynomial_ctx_t;

#define POLYNOMIAL_CTX(ring_ctx) ((polynomial_ctx_t *)((ring_ctx)->elem_ctx))
#define POLYNOMIAL_ELEM_CTX(ring_ctx) (POLYNOMIAL_CTX(ring_ctx)->base_ring)

void gr_ctx_init_polynomial(gr_ctx_t ctx, gr_ctx_t base_ring);

/* Generic matrix ring */

typedef struct
{
    gr_ctx_struct * base_ring;
    slong n;
}
matrix_ctx_t;

#define MATRIX_CTX(ring_ctx) ((matrix_ctx_t *)((ring_ctx)->elem_ctx))

void gr_ctx_init_matrix(gr_ctx_t ctx, gr_ctx_t base_ring, slong n);

#ifdef __cplusplus
}
#endif

#endif
