/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

/* SYNTAX ANALYZER (PARSER)
 * This is the second phase of compilation - checking grammar rules
 * Bison generates a parser that builds an Abstract Syntax Tree (AST)
 * The parser uses tokens from the scanner to verify syntax is correct
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "symtab.h"

/* External declarations for lexer interface */
extern int yylex();      /* Get next token from scanner */
extern int yyparse();    /* Parse the entire input */
extern FILE* yyin;       /* Input file handle */

void yyerror(const char* s);  /* Error handling function */
ASTNode* root = NULL;          /* Root of the Abstract Syntax Tree */

#line 92 "parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_FNUM = 4,                       /* FNUM  */
  YYSYMBOL_ID = 5,                         /* ID  */
  YYSYMBOL_INT = 6,                        /* INT  */
  YYSYMBOL_PRINT = 7,                      /* PRINT  */
  YYSYMBOL_FLOAT = 8,                      /* FLOAT  */
  YYSYMBOL_RETURN = 9,                     /* RETURN  */
  YYSYMBOL_VOID = 10,                      /* VOID  */
  YYSYMBOL_11_ = 11,                       /* '+'  */
  YYSYMBOL_12_ = 12,                       /* '-'  */
  YYSYMBOL_13_ = 13,                       /* '*'  */
  YYSYMBOL_14_ = 14,                       /* '/'  */
  YYSYMBOL_15_ = 15,                       /* ';'  */
  YYSYMBOL_16_ = 16,                       /* '('  */
  YYSYMBOL_17_ = 17,                       /* ')'  */
  YYSYMBOL_18_ = 18,                       /* ','  */
  YYSYMBOL_19_ = 19,                       /* '{'  */
  YYSYMBOL_20_ = 20,                       /* '}'  */
  YYSYMBOL_21_ = 21,                       /* '['  */
  YYSYMBOL_22_ = 22,                       /* ']'  */
  YYSYMBOL_23_ = 23,                       /* '='  */
  YYSYMBOL_YYACCEPT = 24,                  /* $accept  */
  YYSYMBOL_program = 25,                   /* program  */
  YYSYMBOL_stmt_list = 26,                 /* stmt_list  */
  YYSYMBOL_stmt = 27,                      /* stmt  */
  YYSYMBOL_func_decl = 28,                 /* func_decl  */
  YYSYMBOL_param_list = 29,                /* param_list  */
  YYSYMBOL_param = 30,                     /* param  */
  YYSYMBOL_block = 31,                     /* block  */
  YYSYMBOL_return_stmt = 32,               /* return_stmt  */
  YYSYMBOL_func_call = 33,                 /* func_call  */
  YYSYMBOL_arg_list = 34,                  /* arg_list  */
  YYSYMBOL_decl = 35,                      /* decl  */
  YYSYMBOL_assign = 36,                    /* assign  */
  YYSYMBOL_declAssign = 37,                /* declAssign  */
  YYSYMBOL_38_1 = 38,                      /* $@1  */
  YYSYMBOL_expr = 39,                      /* expr  */
  YYSYMBOL_print_stmt = 40,                /* print_stmt  */
  YYSYMBOL_arrayExpr = 41                  /* arrayExpr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  35
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   225

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  24
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  18
/* YYNRULES -- Number of rules.  */
#define YYNRULES  60
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  151

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   265


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      16,    17,    13,    11,    18,    12,     2,    14,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    15,
       2,    23,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    21,     2,    22,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    19,     2,    20,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    58,    58,    66,    71,    75,    80,    87,    88,    89,
      90,    91,    92,    93,    98,   102,   106,   110,   114,   121,
     124,   130,   134,   141,   144,   150,   153,   159,   163,   170,
     173,   179,   186,   193,   202,   209,   216,   227,   232,   237,
     247,   247,   255,   263,   269,   275,   281,   290,   295,   299,
     303,   308,   312,   317,   320,   324,   328,   332,   341,   349,
     352
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "FNUM", "ID",
  "INT", "PRINT", "FLOAT", "RETURN", "VOID", "'+'", "'-'", "'*'", "'/'",
  "';'", "'('", "')'", "','", "'{'", "'}'", "'['", "']'", "'='", "$accept",
  "program", "stmt_list", "stmt", "func_decl", "param_list", "param",
  "block", "return_stmt", "func_call", "arg_list", "decl", "assign",
  "declAssign", "$@1", "expr", "print_stmt", "arrayExpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-67)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      70,   117,     7,   -12,    28,    66,    34,    35,    46,    70,
     -67,   -67,   -67,   -67,    50,   -67,   -67,   -67,   -67,    82,
      33,    33,    51,    33,   109,   -67,   -67,    98,   -67,    33,
     -67,   144,    72,   -67,    54,   -67,   -67,   -67,   -67,   -67,
      39,   174,    79,   149,   -67,     5,     3,    77,   130,   -67,
      15,     4,    33,    33,   137,    33,    33,    33,    33,   -67,
     118,   -67,   -67,    33,     8,   -67,   111,   189,   179,   128,
     -67,   177,   178,    33,   185,   179,   135,   180,   181,   154,
      83,   -67,    90,   183,   191,   -67,   179,   174,    33,    33,
     -67,   -67,   -67,   179,    11,   113,   184,   159,   -67,   -67,
     179,   116,   187,   -67,   186,   -67,    95,   164,   -67,   -67,
     -67,   205,   190,    33,   -67,   -67,   -67,   207,   192,    33,
      33,   193,   -67,   195,    33,   174,   102,   196,    33,   171,
      99,    33,   197,   172,    33,   198,   199,   175,   200,   -67,
     169,   -67,   204,   174,   -67,   -67,   206,   -67,   -67,   -67,
     -67
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     2,
       3,     4,    12,    11,     0,     7,     8,     9,    10,     0,
       0,     0,    40,     0,     0,    48,    49,    50,    26,     0,
      57,     0,     0,    24,     0,     1,     5,     6,    13,    28,
       0,    29,     0,     0,    31,     0,     0,     0,     0,    32,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    25,
       0,    23,    27,     0,     0,    38,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    47,    53,    54,    55,    56,     0,    30,     0,     0,
      21,    22,    15,     0,     0,     0,     0,     0,    58,    17,
       0,     0,     0,    42,    51,    18,     0,     0,    14,    20,
      33,     0,     0,     0,    41,    16,    35,     0,     0,     0,
       0,     0,    37,     0,     0,    59,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    52,
       0,    34,     0,    60,    44,    36,     0,    46,    39,    43,
      45
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -67,   -67,   213,    -6,    -4,   173,   131,   -66,   -67,     1,
     -67,   -67,   -67,   -67,   -67,    -5,   -67,    -1
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     8,     9,    10,    11,    69,    70,    12,    13,    30,
      40,    15,    16,    17,    47,   125,    18,   126
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      31,    14,    92,    36,    23,    37,    71,    77,    14,    99,
      14,    66,    22,    67,    41,    42,    43,    66,    48,    67,
     105,    66,    68,    67,    54,    72,    78,   108,    36,    88,
      37,    89,    75,    24,   115,    14,    25,    26,    27,    32,
       1,     2,     3,     4,     5,     6,    35,    79,    80,    29,
      82,    83,    84,    85,     7,    33,    62,    63,    87,     1,
       2,     3,     4,     5,     6,    38,    44,    45,    97,    25,
      26,    27,    46,     7,    61,     1,     2,     3,     4,     5,
       6,    28,    29,   106,   107,    25,    26,    27,    60,     7,
      55,    56,    57,    58,    55,    56,    57,    58,    29,    39,
      73,    64,    56,    57,    58,   104,    55,    56,    57,    58,
      55,    56,    57,    58,    19,   130,    90,   121,   129,    53,
     134,   139,   135,   133,    49,    50,   140,   137,   110,   143,
      51,   116,    52,    19,   111,    86,   112,   117,    20,   118,
      21,    55,    56,    57,    58,    93,    94,    74,    55,    56,
      57,    58,   100,    94,    81,    55,    56,    57,    58,    59,
      55,    56,    57,    58,    65,    55,    56,    57,    58,   103,
      55,    56,    57,    58,   114,    55,    56,    57,    58,   122,
      55,    56,    57,    58,   148,    55,    56,    57,    58,   134,
     134,   138,   142,   134,    91,   146,    57,    58,     7,    95,
      98,    96,   101,   113,   102,    58,   119,   120,   123,   124,
     127,   128,   141,   144,   145,   147,   131,   132,   136,   149,
      34,   150,     0,    76,     0,   109
};

static const yytype_int16 yycheck[] =
{
       5,     0,    68,     9,    16,     9,     3,     3,     7,    75,
       9,     6,     5,     8,    19,    20,    21,     6,    23,     8,
      86,     6,    17,     8,    29,    22,    22,    93,    34,    21,
      34,    23,    17,     5,   100,    34,     3,     4,     5,     5,
       5,     6,     7,     8,     9,    10,     0,    52,    53,    16,
      55,    56,    57,    58,    19,    20,    17,    18,    63,     5,
       6,     7,     8,     9,    10,    15,    15,    16,    73,     3,
       4,     5,    21,    19,    20,     5,     6,     7,     8,     9,
      10,    15,    16,    88,    89,     3,     4,     5,    16,    19,
      11,    12,    13,    14,    11,    12,    13,    14,    16,    17,
      23,    22,    12,    13,    14,    22,    11,    12,    13,    14,
      11,    12,    13,    14,    16,   120,     5,    22,   119,    21,
      18,    22,    20,   124,    15,    16,   131,   128,    15,   134,
      21,    15,    23,    16,    21,    17,    23,    21,    21,    23,
      23,    11,    12,    13,    14,    17,    18,    17,    11,    12,
      13,    14,    17,    18,    17,    11,    12,    13,    14,    15,
      11,    12,    13,    14,    15,    11,    12,    13,    14,    15,
      11,    12,    13,    14,    15,    11,    12,    13,    14,    15,
      11,    12,    13,    14,    15,    11,    12,    13,    14,    18,
      18,    20,    20,    18,     5,    20,    13,    14,    19,    22,
      15,    23,    22,    19,    23,    14,    19,    21,     3,    19,
       3,    19,    15,    15,    15,    15,    23,    22,    22,    15,
       7,    15,    -1,    50,    -1,    94
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,     6,     7,     8,     9,    10,    19,    25,    26,
      27,    28,    31,    32,    33,    35,    36,    37,    40,    16,
      21,    23,     5,    16,     5,     3,     4,     5,    15,    16,
      33,    39,     5,    20,    26,     0,    27,    28,    15,    17,
      34,    39,    39,    39,    15,    16,    21,    38,    39,    15,
      16,    21,    23,    21,    39,    11,    12,    13,    14,    15,
      16,    20,    17,    18,    22,    15,     6,     8,    17,    29,
      30,     3,    22,    23,    17,    17,    29,     3,    22,    39,
      39,    17,    39,    39,    39,    39,    17,    39,    21,    23,
       5,     5,    31,    17,    18,    22,    23,    39,    15,    31,
      17,    22,    23,    15,    22,    31,    39,    39,    31,    30,
      15,    21,    23,    19,    15,    31,    15,    21,    23,    19,
      21,    22,    15,     3,    19,    39,    41,     3,    19,    41,
      39,    23,    22,    41,    18,    20,    22,    41,    20,    22,
      39,    15,    20,    39,    15,    15,    20,    15,    15,    15,
      15
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    24,    25,    26,    26,    26,    26,    27,    27,    27,
      27,    27,    27,    27,    28,    28,    28,    28,    28,    29,
      29,    30,    30,    31,    31,    32,    32,    33,    33,    34,
      34,    35,    35,    35,    35,    35,    35,    36,    36,    36,
      38,    37,    37,    37,    37,    37,    37,    39,    39,    39,
      39,    39,    39,    39,    39,    39,    39,    39,    40,    41,
      41
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     6,     5,     6,     5,     5,     1,
       3,     2,     2,     3,     2,     3,     2,     4,     3,     1,
       3,     3,     3,     6,     9,     6,     9,     7,     4,    10,
       0,     6,     5,    10,     9,    10,     9,     3,     1,     1,
       1,     4,     7,     3,     3,     3,     3,     1,     5,     1,
       3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 58 "parser.y"
              { 
        /* Action: Save the statement list as our AST root */
        root = (yyvsp[0].node);  /* $1 refers to the first symbol (stmt_list) */
    }
#line 1223 "parser.tab.c"
    break;

  case 3: /* stmt_list: stmt  */
#line 66 "parser.y"
         { 
        /* Base case: single statement */
        (yyval.node) = (yyvsp[0].node);  /* Pass the statement up as-is */
    }
#line 1232 "parser.tab.c"
    break;

  case 4: /* stmt_list: func_decl  */
#line 71 "parser.y"
              { 
        /* Base case: single function declaration */
        (yyval.node) = (yyvsp[0].node);  /* Pass the function declaration up as-is */
    }
#line 1241 "parser.tab.c"
    break;

  case 5: /* stmt_list: stmt_list stmt  */
#line 75 "parser.y"
                     { 
        /* Recursive case: list followed by another statement */
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node));  /* Build linked list of statements */
    }
#line 1250 "parser.tab.c"
    break;

  case 6: /* stmt_list: stmt_list func_decl  */
#line 80 "parser.y"
                        {
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node)); /* Append function declaration to statement list */
    }
#line 1258 "parser.tab.c"
    break;

  case 14: /* func_decl: INT ID '(' param_list ')' block  */
#line 98 "parser.y"
                                    {
        (yyval.node) = createFuncDecl("int", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); /* Create function declaration node */
        free((yyvsp[-4].str)); /* Free the function name string */
    }
#line 1267 "parser.tab.c"
    break;

  case 15: /* func_decl: INT ID '(' ')' block  */
#line 102 "parser.y"
                           {
        (yyval.node) = createFuncDecl("int", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1276 "parser.tab.c"
    break;

  case 16: /* func_decl: FLOAT ID '(' param_list ')' block  */
#line 106 "parser.y"
                                        {
        (yyval.node) = createFuncDecl("float", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node));
        free((yyvsp[-4].str));
    }
#line 1285 "parser.tab.c"
    break;

  case 17: /* func_decl: FLOAT ID '(' ')' block  */
#line 110 "parser.y"
                             {
        (yyval.node) = createFuncDecl("float", (yyvsp[-3].str), NULL, (yyvsp[0].node));
        free((yyvsp[-3].str));
    }
#line 1294 "parser.tab.c"
    break;

  case 18: /* func_decl: VOID ID '(' ')' block  */
#line 114 "parser.y"
                            {
        (yyval.node) = createFuncDecl("void", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Void function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1303 "parser.tab.c"
    break;

  case 19: /* param_list: param  */
#line 121 "parser.y"
          {
        (yyval.node) = (yyvsp[0].node); /* Single parameter */
    }
#line 1311 "parser.tab.c"
    break;

  case 20: /* param_list: param_list ',' param  */
#line 124 "parser.y"
                           {
        (yyval.node) = createParamList((yyvsp[-2].node), (yyvsp[0].node)); /* Append parameter to list */
    }
#line 1319 "parser.tab.c"
    break;

  case 21: /* param: INT ID  */
#line 130 "parser.y"
           {
        (yyval.node) = createParam("int", (yyvsp[0].str)); /* Create parameter node */
        free((yyvsp[0].str));
    }
#line 1328 "parser.tab.c"
    break;

  case 22: /* param: FLOAT ID  */
#line 134 "parser.y"
               {  /* ✅ ADD THIS */
        (yyval.node) = createParam("float", (yyvsp[0].str));
        free((yyvsp[0].str));
    }
#line 1337 "parser.tab.c"
    break;

  case 23: /* block: '{' stmt_list '}'  */
#line 141 "parser.y"
                      {
        (yyval.node) = createBlock((yyvsp[-1].node)); /* Create block node with statement list */
    }
#line 1345 "parser.tab.c"
    break;

  case 24: /* block: '{' '}'  */
#line 144 "parser.y"
              {
        (yyval.node) = createBlock(NULL); /* Empty block */
    }
#line 1353 "parser.tab.c"
    break;

  case 25: /* return_stmt: RETURN expr ';'  */
#line 150 "parser.y"
                    {
        (yyval.node) = createReturn((yyvsp[-1].node)); /* Create return statement node */
    }
#line 1361 "parser.tab.c"
    break;

  case 26: /* return_stmt: RETURN ';'  */
#line 153 "parser.y"
                 {
        (yyval.node) = createReturn(NULL); /* Return with no value */
    }
#line 1369 "parser.tab.c"
    break;

  case 27: /* func_call: ID '(' arg_list ')'  */
#line 159 "parser.y"
                        {
        (yyval.node) = createFuncCall((yyvsp[-3].str), (yyvsp[-1].node)); /* Create function call node */
        free((yyvsp[-3].str)); /* Free function name string */
    }
#line 1378 "parser.tab.c"
    break;

  case 28: /* func_call: ID '(' ')'  */
#line 163 "parser.y"
                 {
        (yyval.node) = createFuncCall((yyvsp[-2].str), NULL); /* Function call with no arguments */
        free((yyvsp[-2].str));
    }
#line 1387 "parser.tab.c"
    break;

  case 29: /* arg_list: expr  */
#line 170 "parser.y"
         {
        (yyval.node) = createArgList((yyvsp[0].node), NULL); /* Single argument */
    }
#line 1395 "parser.tab.c"
    break;

  case 30: /* arg_list: arg_list ',' expr  */
#line 173 "parser.y"
                        {
        (yyval.node) = createArgList((yyvsp[0].node), (yyvsp[-2].node)); /* Append argument to list */
    }
#line 1403 "parser.tab.c"
    break;

  case 31: /* decl: INT ID ';'  */
#line 179 "parser.y"
               { 

        /* Create declaration node and free the identifier string */
        (yyval.node) = createDecl("int", (yyvsp[-1].str));  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free((yyvsp[-1].str));             /* Free the string copy from scanner */
    }
#line 1414 "parser.tab.c"
    break;

  case 32: /* decl: FLOAT ID ';'  */
#line 186 "parser.y"
                   {
        (yyval.node) = createDecl("float", (yyvsp[-1].str));
        free((yyvsp[-1].str));
    }
#line 1423 "parser.tab.c"
    break;

  case 33: /* decl: INT ID '[' NUM ']' ';'  */
#line 194 "parser.y"
    {
        (yyval.node) = createArrayDeclOfLength("int", (yyvsp[-4].str), (yyvsp[-2].num));/* done */
        addArrayVar((yyvsp[-4].str), (yyvsp[-2].num), "int");
        free((yyvsp[-4].str));
    }
#line 1433 "parser.tab.c"
    break;

  case 34: /* decl: INT ID '[' NUM ']' '[' NUM ']' ';'  */
#line 203 "parser.y"
    {
        addArray2DVar((yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num), "int");
        (yyval.node) = create2DArrayDeclOfLength("int", (yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-7].str));
    }
#line 1443 "parser.tab.c"
    break;

  case 35: /* decl: FLOAT ID '[' NUM ']' ';'  */
#line 210 "parser.y"
    {
        (yyval.node) = createArrayDeclOfLength("float", (yyvsp[-4].str), (yyvsp[-2].num));/* done */
        addArrayVar((yyvsp[-4].str), (yyvsp[-2].num), "float");
        free((yyvsp[-4].str));
    }
#line 1453 "parser.tab.c"
    break;

  case 36: /* decl: FLOAT ID '[' NUM ']' '[' NUM ']' ';'  */
#line 217 "parser.y"
    {
        addArray2DVar((yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num), "float");
        (yyval.node) = create2DArrayDeclOfLength("float", (yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-7].str));
    }
#line 1463 "parser.tab.c"
    break;

  case 37: /* assign: ID '[' expr ']' '=' expr ';'  */
#line 227 "parser.y"
                                 {
        (yyval.node) = createArrayElemAssign((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); 
        free((yyvsp[-6].str));
    }
#line 1472 "parser.tab.c"
    break;

  case 38: /* assign: ID '=' expr ';'  */
#line 232 "parser.y"
                      { 
        (yyval.node) = createAssign((yyvsp[-3].str), (yyvsp[-1].node));  
        free((yyvsp[-3].str));
    }
#line 1481 "parser.tab.c"
    break;

  case 39: /* assign: ID '[' expr ']' '[' expr ']' '=' expr ';'  */
#line 238 "parser.y"
    {
        (yyval.node) = createArray2DElemAssign((yyvsp[-9].str), (yyvsp[-7].node), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h (done) -> ast.c (done)-> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-9].str));
    }
#line 1490 "parser.tab.c"
    break;

  case 40: /* $@1: %empty  */
#line 247 "parser.y"
           { 
    }
#line 1497 "parser.tab.c"
    break;

  case 41: /* declAssign: INT ID $@1 '=' expr ';'  */
#line 249 "parser.y"
    {
        (yyval.node) = createDeclAssign("int", (yyvsp[-4].str), (yyvsp[-1].node)); /* done */
        free((yyvsp[-4].str));
    }
#line 1506 "parser.tab.c"
    break;

  case 42: /* declAssign: FLOAT ID '=' expr ';'  */
#line 255 "parser.y"
                         { 

        /* Create declaration node and free the identifier string */
        (yyval.node) = createDeclAssign("float", (yyvsp[-3].str), (yyvsp[-1].node));  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free((yyvsp[-3].str));             /* Free the string copy from scanner */
    }
#line 1517 "parser.tab.c"
    break;

  case 43: /* declAssign: INT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'  */
#line 264 "parser.y"
    {
        (yyval.node) = createArrayAssign("int", (yyvsp[-8].str), (yyvsp[-6].num), (yyvsp[-2].node));/* done */
        free((yyvsp[-8].str));
    }
#line 1526 "parser.tab.c"
    break;

  case 44: /* declAssign: INT ID '[' ']' '=' '{' arrayExpr '}' ';'  */
#line 270 "parser.y"
    {
        (yyval.node) = createArrayDeclAssign("int", (yyvsp[-7].str), 0, (yyvsp[-2].node));/* done */
        free((yyvsp[-7].str));
    }
#line 1535 "parser.tab.c"
    break;

  case 45: /* declAssign: FLOAT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'  */
#line 276 "parser.y"
    {
        (yyval.node) = createArrayAssign("float", (yyvsp[-8].str), (yyvsp[-6].num), (yyvsp[-2].node));/* done */
        free((yyvsp[-8].str));
    }
#line 1544 "parser.tab.c"
    break;

  case 46: /* declAssign: FLOAT ID '[' ']' '=' '{' arrayExpr '}' ';'  */
#line 282 "parser.y"
    {
        (yyval.node) = createArrayDeclAssign("float", (yyvsp[-7].str), 0, (yyvsp[-2].node));/* done */
        free((yyvsp[-7].str));
    }
#line 1553 "parser.tab.c"
    break;

  case 47: /* expr: '(' expr ')'  */
#line 290 "parser.y"
                 { 
        /* Parenthesized expression - just pass up the inner expression */
        (yyval.node) = (yyvsp[-1].node);  /* $2 is the expr inside the parentheses */
    }
#line 1562 "parser.tab.c"
    break;

  case 48: /* expr: NUM  */
#line 295 "parser.y"
        { 
        /* Literal number */
        (yyval.node) = createNum((yyvsp[0].num), 0);  /* Create leaf node with number value */
    }
#line 1571 "parser.tab.c"
    break;

  case 49: /* expr: FNUM  */
#line 299 "parser.y"
           { 
        /* Literal float number */
        (yyval.node) = createNum((yyvsp[0].fnum), 1);  /* Create leaf node with float value */
    }
#line 1580 "parser.tab.c"
    break;

  case 50: /* expr: ID  */
#line 303 "parser.y"
         { 
        /* Variable reference */
        (yyval.node) = createVar((yyvsp[0].str));  /* Create leaf node with variable name */
        free((yyvsp[0].str));            /* Free the identifier string */
    }
#line 1590 "parser.tab.c"
    break;

  case 51: /* expr: ID '[' expr ']'  */
#line 308 "parser.y"
                      { /* allows to access whatever value is at index 'expr' */
        (yyval.node) = createArrayAccess((yyvsp[-3].str), (yyvsp[-1].node));
        free((yyvsp[-3].str));
    }
#line 1599 "parser.tab.c"
    break;

  case 52: /* expr: ID '[' expr ']' '[' expr ']'  */
#line 313 "parser.y"
    {
        (yyval.node) = createArray2DAccess((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-6].str));
    }
#line 1608 "parser.tab.c"
    break;

  case 53: /* expr: expr '+' expr  */
#line 317 "parser.y"
                    { 
        /* Addition operation - builds binary tree */
        (yyval.node) = createBinOp('+', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1617 "parser.tab.c"
    break;

  case 54: /* expr: expr '-' expr  */
#line 320 "parser.y"
                      { 
        /* Subtraction operation - builds binary tree */
        (yyval.node) = createBinOp('-', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1626 "parser.tab.c"
    break;

  case 55: /* expr: expr '*' expr  */
#line 324 "parser.y"
                    { 
        /* Multiplication operation - builds binary tree */
        (yyval.node) = createBinOp('*', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1635 "parser.tab.c"
    break;

  case 56: /* expr: expr '/' expr  */
#line 328 "parser.y"
                    { 
        /* Division operation - builds binary tree */
        (yyval.node) = createBinOp('/', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1644 "parser.tab.c"
    break;

  case 57: /* expr: func_call  */
#line 332 "parser.y"
                { 
        /* ADDED */
        /* Function call as an expression */
        (yyval.node) = (yyvsp[0].node);  /* Just pass up the function call node */
    }
#line 1654 "parser.tab.c"
    break;

  case 58: /* print_stmt: PRINT '(' expr ')' ';'  */
#line 341 "parser.y"
                           { 
        /* Create print node with expression to print */
        (yyval.node) = createPrint((yyvsp[-2].node));  /* $3 is the expression inside parens */
    }
#line 1663 "parser.tab.c"
    break;

  case 59: /* arrayExpr: expr  */
#line 349 "parser.y"
         {
        (yyval.node) = createExprList((yyvsp[0].node), NULL); /* done */
    }
#line 1671 "parser.tab.c"
    break;

  case 60: /* arrayExpr: arrayExpr ',' expr  */
#line 352 "parser.y"
                         {
        (yyval.node) = createExprList((yyvsp[0].node), (yyvsp[-2].node)); /* done */
    }
#line 1679 "parser.tab.c"
    break;


#line 1683 "parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 367 "parser.y"


/* ERROR HANDLING - Called by Bison when syntax error detected */
void yyerror(const char* s) {
    fprintf(stderr, "Syntax Error: %s\n", s);
}


// Our compilers supports:
// 1. Binary operations: +, -, *, /
// 2. Stand alone int variable declarations
// 3. int variable assignments
// 4. Simultaneous int declaration and assignment
// 5. stand alone 1d and 2d array variable declarations
// 6. by-index element assignments for 1d and 2d arrays
// 7. by-index element access for 1d and 2d arrays
// 8. simultaneous declaring and assigning values to a 1d array.
// 9. print statements for values
