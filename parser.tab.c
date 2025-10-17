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
  YYSYMBOL_ID = 4,                         /* ID  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_PRINT = 6,                      /* PRINT  */
  YYSYMBOL_RETURN = 7,                     /* RETURN  */
  YYSYMBOL_VOID = 8,                       /* VOID  */
  YYSYMBOL_9_ = 9,                         /* '+'  */
  YYSYMBOL_10_ = 10,                       /* '-'  */
  YYSYMBOL_11_ = 11,                       /* '*'  */
  YYSYMBOL_12_ = 12,                       /* '/'  */
  YYSYMBOL_13_ = 13,                       /* ';'  */
  YYSYMBOL_14_ = 14,                       /* '('  */
  YYSYMBOL_15_ = 15,                       /* ')'  */
  YYSYMBOL_16_ = 16,                       /* ','  */
  YYSYMBOL_17_ = 17,                       /* '{'  */
  YYSYMBOL_18_ = 18,                       /* '}'  */
  YYSYMBOL_19_ = 19,                       /* '['  */
  YYSYMBOL_20_ = 20,                       /* ']'  */
  YYSYMBOL_21_ = 21,                       /* '='  */
  YYSYMBOL_YYACCEPT = 22,                  /* $accept  */
  YYSYMBOL_program = 23,                   /* program  */
  YYSYMBOL_stmt_list = 24,                 /* stmt_list  */
  YYSYMBOL_stmt = 25,                      /* stmt  */
  YYSYMBOL_func_decl = 26,                 /* func_decl  */
  YYSYMBOL_param_list = 27,                /* param_list  */
  YYSYMBOL_param = 28,                     /* param  */
  YYSYMBOL_block = 29,                     /* block  */
  YYSYMBOL_return_stmt = 30,               /* return_stmt  */
  YYSYMBOL_func_call = 31,                 /* func_call  */
  YYSYMBOL_arg_list = 32,                  /* arg_list  */
  YYSYMBOL_decl = 33,                      /* decl  */
  YYSYMBOL_assign = 34,                    /* assign  */
  YYSYMBOL_declAssign = 35,                /* declAssign  */
  YYSYMBOL_36_1 = 36,                      /* $@1  */
  YYSYMBOL_expr = 37,                      /* expr  */
  YYSYMBOL_print_stmt = 38,                /* print_stmt  */
  YYSYMBOL_arrayExpr = 39,                 /* arrayExpr  */
  YYSYMBOL_arrayExpr2D = 40                /* arrayExpr2D  */
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
#define YYFINAL  31
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   193

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  22
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  54
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   263


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
      14,    15,    11,     9,    16,    10,     2,    12,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    13,
       2,    21,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    19,     2,    20,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    17,     2,    18,     2,     2,     2,     2,
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
       5,     6,     7,     8
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    56,    56,    64,    69,    73,    78,    85,    86,    87,
      88,    89,    90,    91,    96,   100,   104,   111,   114,   120,
     127,   130,   136,   139,   145,   149,   156,   159,   165,   174,
     182,   193,   198,   205,   211,   218,   228,   228,   236,   242,
     249,   258,   262,   267,   271,   276,   279,   283,   287,   291,
     300,   308,   311,   318,   321
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
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "ID", "INT",
  "PRINT", "RETURN", "VOID", "'+'", "'-'", "'*'", "'/'", "';'", "'('",
  "')'", "','", "'{'", "'}'", "'['", "']'", "'='", "$accept", "program",
  "stmt_list", "stmt", "func_decl", "param_list", "param", "block",
  "return_stmt", "func_call", "arg_list", "decl", "assign", "declAssign",
  "$@1", "expr", "print_stmt", "arrayExpr", "arrayExpr2D", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-100)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      68,    63,    23,    80,    27,   115,    30,     8,    68,  -100,
    -100,  -100,  -100,   152,  -100,  -100,  -100,  -100,    54,    64,
      46,   108,    64,  -100,    45,  -100,  -100,   119,    86,  -100,
      48,  -100,  -100,  -100,  -100,  -100,    72,   114,    69,   100,
     124,  -100,     5,     1,   146,   105,    64,    64,    64,    64,
      64,  -100,   153,  -100,  -100,    64,    20,   156,  -100,   166,
     154,    96,  -100,   155,    41,    64,   159,    81,   143,    97,
     161,  -100,   154,   114,     9,    64,  -100,  -100,  -100,   154,
     169,     4,   157,   162,   129,  -100,   163,  -100,   160,    87,
     134,  -100,  -100,  -100,   173,   167,   164,    64,  -100,    64,
      64,   165,  -100,   158,    64,   170,   114,   140,    93,    70,
      64,   175,   141,   172,    64,   177,  -100,  -100,   139,  -100,
     178,    64,   144,   114,  -100,  -100,  -100,   145,   176,   179,
    -100,    64,  -100,   148,  -100
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     2,     3,
       4,    12,    11,     0,     7,     8,     9,    10,     0,     0,
       0,    36,     0,    41,    42,    23,    49,     0,     0,    21,
       0,     1,     5,     6,    13,    25,     0,    26,     0,     0,
       0,    28,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    22,     0,    20,    24,     0,     0,     0,    32,     0,
       0,     0,    17,     0,     0,     0,     0,     0,    45,    46,
      47,    48,     0,    27,     0,     0,    33,    19,    15,     0,
       0,     0,     0,     0,     0,    50,    43,    16,     0,     0,
       0,    14,    18,    29,     0,     0,     0,     0,    37,     0,
       0,     0,    31,     0,     0,     0,    51,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    44,    34,     0,    30,
       0,     0,     0,    52,    39,    35,    38,     0,     0,     0,
      53,     0,    40,     0,    54
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -100,  -100,   174,    -6,    -2,  -100,   103,   -53,  -100,     3,
    -100,  -100,  -100,  -100,  -100,    -4,  -100,   -99,  -100
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     7,     8,     9,    10,    61,    62,    11,    12,    26,
      36,    14,    15,    16,    44,   106,    17,   107,   122
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      27,   109,    32,    13,    63,   112,    33,    78,    31,    13,
      59,    13,    23,    24,    37,    38,    40,    93,    45,    87,
      60,    64,   127,    94,    32,    95,    91,    21,    33,    88,
      23,    24,   133,    13,     1,     2,     3,     4,     5,    74,
      25,    75,    67,    68,    69,    70,    71,     6,    29,    23,
      24,    73,     1,     2,     3,     4,     5,    23,    24,    18,
      82,    84,    83,    39,    46,     6,    53,    23,    24,    35,
      89,    90,     1,     2,     3,     4,     5,    18,    47,    48,
      49,    50,    19,   117,    20,     6,   114,    54,    55,    56,
      47,    48,    49,    50,    22,   108,    47,    48,    49,    50,
      52,    86,    47,    48,    49,    50,   118,   101,    49,    50,
     123,    79,    80,   116,    47,    48,    49,    50,    57,    28,
      66,    41,    42,    47,    48,    49,    50,    43,    47,    48,
      49,    50,    51,    47,    48,    49,    50,    58,    47,    48,
      49,    50,    98,    47,    48,    49,    50,   102,    47,    48,
      49,    50,   125,    48,    49,    50,   114,   114,   115,   120,
     128,   114,   129,   130,   114,    34,   134,    65,    72,    76,
      77,     6,    85,    50,    59,    81,   103,    96,   111,    97,
      30,   100,    99,    92,   104,   105,   110,   113,   119,   121,
     124,   126,   132,   131
};

static const yytype_uint8 yycheck[] =
{
       4,   100,     8,     0,     3,   104,     8,    60,     0,     6,
       5,     8,     3,     4,    18,    19,    20,    13,    22,    72,
      15,    20,   121,    19,    30,    21,    79,     4,    30,    20,
       3,     4,   131,    30,     4,     5,     6,     7,     8,    19,
      13,    21,    46,    47,    48,    49,    50,    17,    18,     3,
       4,    55,     4,     5,     6,     7,     8,     3,     4,    14,
      19,    65,    21,    17,    19,    17,    18,     3,     4,    15,
      74,    75,     4,     5,     6,     7,     8,    14,     9,    10,
      11,    12,    19,    13,    21,    17,    16,    15,    16,    20,
       9,    10,    11,    12,    14,    99,     9,    10,    11,    12,
      14,    20,     9,    10,    11,    12,   110,    20,    11,    12,
     114,    15,    16,    20,     9,    10,    11,    12,    18,     4,
      15,    13,    14,     9,    10,    11,    12,    19,     9,    10,
      11,    12,    13,     9,    10,    11,    12,    13,     9,    10,
      11,    12,    13,     9,    10,    11,    12,    13,     9,    10,
      11,    12,    13,    10,    11,    12,    16,    16,    18,    18,
      16,    16,    18,    18,    16,    13,    18,    21,    15,    13,
       4,    17,    13,    12,     5,    20,     3,    20,    20,    17,
       6,    21,    19,    80,    17,    21,    21,    17,    13,    17,
      13,    13,    13,    17
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     6,     7,     8,    17,    23,    24,    25,
      26,    29,    30,    31,    33,    34,    35,    38,    14,    19,
      21,     4,    14,     3,     4,    13,    31,    37,     4,    18,
      24,     0,    25,    26,    13,    15,    32,    37,    37,    17,
      37,    13,    14,    19,    36,    37,    19,     9,    10,    11,
      12,    13,    14,    18,    15,    16,    20,    18,    13,     5,
      15,    27,    28,     3,    20,    21,    15,    37,    37,    37,
      37,    37,    15,    37,    19,    21,    13,     4,    29,    15,
      16,    20,    19,    21,    37,    13,    20,    29,    20,    37,
      37,    29,    28,    13,    19,    21,    20,    17,    13,    19,
      21,    20,    13,     3,    17,    21,    37,    39,    37,    39,
      21,    20,    39,    17,    16,    18,    20,    13,    37,    13,
      18,    17,    40,    37,    13,    13,    13,    39,    16,    18,
      18,    17,    13,    39,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    22,    23,    24,    24,    24,    24,    25,    25,    25,
      25,    25,    25,    25,    26,    26,    26,    27,    27,    28,
      29,    29,    30,    30,    31,    31,    32,    32,    33,    33,
      33,    34,    34,    34,    34,    34,    36,    35,    35,    35,
      35,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      38,    39,    39,    40,    40
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     6,     5,     5,     1,     3,     2,
       3,     2,     3,     2,     4,     3,     1,     3,     3,     6,
       9,     7,     4,     5,     9,    10,     0,     6,    10,     9,
      11,     1,     1,     4,     7,     3,     3,     3,     3,     1,
       5,     1,     3,     3,     5
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
#line 56 "parser.y"
              { 
        /* Action: Save the statement list as our AST root */
        root = (yyvsp[0].node);  /* $1 refers to the first symbol (stmt_list) */
    }
#line 1207 "parser.tab.c"
    break;

  case 3: /* stmt_list: stmt  */
#line 64 "parser.y"
         { 
        /* Base case: single statement */
        (yyval.node) = (yyvsp[0].node);  /* Pass the statement up as-is */
    }
#line 1216 "parser.tab.c"
    break;

  case 4: /* stmt_list: func_decl  */
#line 69 "parser.y"
              { 
        /* Base case: single function declaration */
        (yyval.node) = (yyvsp[0].node);  /* Pass the function declaration up as-is */
    }
#line 1225 "parser.tab.c"
    break;

  case 5: /* stmt_list: stmt_list stmt  */
#line 73 "parser.y"
                     { 
        /* Recursive case: list followed by another statement */
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node));  /* Build linked list of statements */
    }
#line 1234 "parser.tab.c"
    break;

  case 6: /* stmt_list: stmt_list func_decl  */
#line 78 "parser.y"
                        {
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node)); /* Append function declaration to statement list */
    }
#line 1242 "parser.tab.c"
    break;

  case 14: /* func_decl: INT ID '(' param_list ')' block  */
#line 96 "parser.y"
                                    {
        (yyval.node) = createFuncDecl("int", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); /* Create function declaration node */
        free((yyvsp[-4].str)); /* Free the function name string */
    }
#line 1251 "parser.tab.c"
    break;

  case 15: /* func_decl: INT ID '(' ')' block  */
#line 100 "parser.y"
                           {
        (yyval.node) = createFuncDecl("int", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1260 "parser.tab.c"
    break;

  case 16: /* func_decl: VOID ID '(' ')' block  */
#line 104 "parser.y"
                            {
        (yyval.node) = createFuncDecl("void", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Void function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1269 "parser.tab.c"
    break;

  case 17: /* param_list: param  */
#line 111 "parser.y"
          {
        (yyval.node) = (yyvsp[0].node); /* Single parameter */
    }
#line 1277 "parser.tab.c"
    break;

  case 18: /* param_list: param_list ',' param  */
#line 114 "parser.y"
                           {
        (yyval.node) = createParamList((yyvsp[-2].node), (yyvsp[0].node)); /* Append parameter to list */
    }
#line 1285 "parser.tab.c"
    break;

  case 19: /* param: INT ID  */
#line 120 "parser.y"
           {
        (yyval.node) = createParam("int", (yyvsp[0].str)); /* Create parameter node */
        free((yyvsp[0].str));
    }
#line 1294 "parser.tab.c"
    break;

  case 20: /* block: '{' stmt_list '}'  */
#line 127 "parser.y"
                      {
        (yyval.node) = createBlock((yyvsp[-1].node)); /* Create block node with statement list */
    }
#line 1302 "parser.tab.c"
    break;

  case 21: /* block: '{' '}'  */
#line 130 "parser.y"
              {
        (yyval.node) = createBlock(NULL); /* Empty block */
    }
#line 1310 "parser.tab.c"
    break;

  case 22: /* return_stmt: RETURN expr ';'  */
#line 136 "parser.y"
                    {
        (yyval.node) = createReturn((yyvsp[-1].node)); /* Create return statement node */
    }
#line 1318 "parser.tab.c"
    break;

  case 23: /* return_stmt: RETURN ';'  */
#line 139 "parser.y"
                 {
        (yyval.node) = createReturn(NULL); /* Return with no value */
    }
#line 1326 "parser.tab.c"
    break;

  case 24: /* func_call: ID '(' arg_list ')'  */
#line 145 "parser.y"
                        {
        (yyval.node) = createFuncCall((yyvsp[-3].str), (yyvsp[-1].node)); /* Create function call node */
        free((yyvsp[-3].str)); /* Free function name string */
    }
#line 1335 "parser.tab.c"
    break;

  case 25: /* func_call: ID '(' ')'  */
#line 149 "parser.y"
                 {
        (yyval.node) = createFuncCall((yyvsp[-2].str), NULL); /* Function call with no arguments */
        free((yyvsp[-2].str));
    }
#line 1344 "parser.tab.c"
    break;

  case 26: /* arg_list: expr  */
#line 156 "parser.y"
         {
        (yyval.node) = createArgList((yyvsp[0].node), NULL); /* Single argument */
    }
#line 1352 "parser.tab.c"
    break;

  case 27: /* arg_list: arg_list ',' expr  */
#line 159 "parser.y"
                        {
        (yyval.node) = createArgList((yyvsp[0].node), (yyvsp[-2].node)); /* Append argument to list */
    }
#line 1360 "parser.tab.c"
    break;

  case 28: /* decl: INT ID ';'  */
#line 165 "parser.y"
               { 

        /* Create declaration node and free the identifier string */
        (yyval.node) = createDecl((yyvsp[-1].str));  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free((yyvsp[-1].str));             /* Free the string copy from scanner */
    }
#line 1371 "parser.tab.c"
    break;

  case 29: /* decl: INT ID '[' NUM ']' ';'  */
#line 175 "parser.y"
    {
        (yyval.node) = createArrayDeclOfLength("int", (yyvsp[-4].str), (yyvsp[-2].num));/* done */
        free((yyvsp[-4].str));
    }
#line 1380 "parser.tab.c"
    break;

  case 30: /* decl: INT ID '[' NUM ']' '[' NUM ']' ';'  */
#line 183 "parser.y"
    {
        addArray2DVar((yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num));
        (yyval.node) = create2DArrayDeclOfLength("int", (yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-7].str));
    }
#line 1390 "parser.tab.c"
    break;

  case 31: /* assign: ID '[' expr ']' '=' expr ';'  */
#line 193 "parser.y"
                                 {
        (yyval.node) = createArrayElemAssign((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); 
        free((yyvsp[-6].str));
    }
#line 1399 "parser.tab.c"
    break;

  case 32: /* assign: ID '=' expr ';'  */
#line 198 "parser.y"
                      { 
        (yyval.node) = createAssign((yyvsp[-3].str), (yyvsp[-1].node));  
        free((yyvsp[-3].str));
    }
#line 1408 "parser.tab.c"
    break;

  case 33: /* assign: ID '=' '{' '}' ';'  */
#line 206 "parser.y"
    {
        /* not implemented, might not.*/
        free((yyvsp[-4].str));
    }
#line 1417 "parser.tab.c"
    break;

  case 34: /* assign: ID '[' expr ']' '[' ']' '=' arrayExpr ';'  */
#line 212 "parser.y"
    {
        /* not implemented, might not. */
        /* $$ createArray2DRowAssign($1, $3, $8); */
        free ((yyvsp[-8].str));
    }
#line 1427 "parser.tab.c"
    break;

  case 35: /* assign: ID '[' expr ']' '[' expr ']' '=' expr ';'  */
#line 219 "parser.y"
    {
        (yyval.node) = createArray2DElemAssign((yyvsp[-9].str), (yyvsp[-7].node), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h (done) -> ast.c (done)-> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-9].str));
    }
#line 1436 "parser.tab.c"
    break;

  case 36: /* $@1: %empty  */
#line 228 "parser.y"
           { 
    }
#line 1443 "parser.tab.c"
    break;

  case 37: /* declAssign: INT ID $@1 '=' expr ';'  */
#line 230 "parser.y"
    {
        (yyval.node) = createDeclAssign("int", (yyvsp[-4].str), (yyvsp[-1].node)); /* done */
        free((yyvsp[-4].str));
    }
#line 1452 "parser.tab.c"
    break;

  case 38: /* declAssign: INT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'  */
#line 237 "parser.y"
    {
        (yyval.node) = createArrayAssign("int", (yyvsp[-8].str), (yyvsp[-6].num), (yyvsp[-2].node));/* done */
        free((yyvsp[-8].str));
    }
#line 1461 "parser.tab.c"
    break;

  case 39: /* declAssign: INT ID '[' ']' '=' '{' arrayExpr '}' ';'  */
#line 243 "parser.y"
    {
        (yyval.node) = createArrayDeclAssign("int", (yyvsp[-7].str), 0, (yyvsp[-2].node));/* done */
        free((yyvsp[-7].str));
    }
#line 1470 "parser.tab.c"
    break;

  case 40: /* declAssign: INT ID '[' ']' '[' ']' '=' '{' arrayExpr2D '}' ';'  */
#line 250 "parser.y"
    {
        /* $$ = create2DArrayDeclAssign("int", $2, 0, 0, $9); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-9].str));
    }
#line 1479 "parser.tab.c"
    break;

  case 41: /* expr: NUM  */
#line 258 "parser.y"
        { 
        /* Literal number */
        (yyval.node) = createNum((yyvsp[0].num));  /* Create leaf node with number value */
    }
#line 1488 "parser.tab.c"
    break;

  case 42: /* expr: ID  */
#line 262 "parser.y"
         { 
        /* Variable reference */
        (yyval.node) = createVar((yyvsp[0].str));  /* Create leaf node with variable name */
        free((yyvsp[0].str));            /* Free the identifier string */
    }
#line 1498 "parser.tab.c"
    break;

  case 43: /* expr: ID '[' expr ']'  */
#line 267 "parser.y"
                      { /* allows to access whatever value is at index 'expr' */
        (yyval.node) = createArrayAccess((yyvsp[-3].str), (yyvsp[-1].node));
        free((yyvsp[-3].str));
    }
#line 1507 "parser.tab.c"
    break;

  case 44: /* expr: ID '[' expr ']' '[' expr ']'  */
#line 272 "parser.y"
    {
        (yyval.node) = createArray2DAccess((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-6].str));
    }
#line 1516 "parser.tab.c"
    break;

  case 45: /* expr: expr '+' expr  */
#line 276 "parser.y"
                    { 
        /* Addition operation - builds binary tree */
        (yyval.node) = createBinOp('+', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1525 "parser.tab.c"
    break;

  case 46: /* expr: expr '-' expr  */
#line 279 "parser.y"
                      { 
        /* Subtraction operation - builds binary tree */
        (yyval.node) = createBinOp('-', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1534 "parser.tab.c"
    break;

  case 47: /* expr: expr '*' expr  */
#line 283 "parser.y"
                    { 
        /* Multiplication operation - builds binary tree */
        (yyval.node) = createBinOp('*', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1543 "parser.tab.c"
    break;

  case 48: /* expr: expr '/' expr  */
#line 287 "parser.y"
                    { 
        /* Division operation - builds binary tree */
        (yyval.node) = createBinOp('/', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1552 "parser.tab.c"
    break;

  case 49: /* expr: func_call  */
#line 291 "parser.y"
                { 
        /* ADDED */
        /* Function call as an expression */
        (yyval.node) = (yyvsp[0].node);  /* Just pass up the function call node */
    }
#line 1562 "parser.tab.c"
    break;

  case 50: /* print_stmt: PRINT '(' expr ')' ';'  */
#line 300 "parser.y"
                           { 
        /* Create print node with expression to print */
        (yyval.node) = createPrint((yyvsp[-2].node));  /* $3 is the expression inside parens */
    }
#line 1571 "parser.tab.c"
    break;

  case 51: /* arrayExpr: expr  */
#line 308 "parser.y"
         {
        (yyval.node) = createExprList((yyvsp[0].node), NULL); /* done */
    }
#line 1579 "parser.tab.c"
    break;

  case 52: /* arrayExpr: arrayExpr ',' expr  */
#line 311 "parser.y"
                         {
        (yyval.node) = createExprList((yyvsp[0].node), (yyvsp[-2].node)); /* done */
    }
#line 1587 "parser.tab.c"
    break;

  case 53: /* arrayExpr2D: '{' arrayExpr '}'  */
#line 318 "parser.y"
                      {
        /* $$ = create2DExprList($2, NULL); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
    }
#line 1595 "parser.tab.c"
    break;

  case 54: /* arrayExpr2D: arrayExpr2D ',' '{' arrayExpr '}'  */
#line 321 "parser.y"
                                        {
        /* $$ = create2DExprList($4, $1); */ /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
    }
#line 1603 "parser.tab.c"
    break;


#line 1607 "parser.tab.c"

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

#line 326 "parser.y"


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
