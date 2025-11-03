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
  YYSYMBOL_IF = 11,                        /* IF  */
  YYSYMBOL_ELSE = 12,                      /* ELSE  */
  YYSYMBOL_EQ = 13,                        /* EQ  */
  YYSYMBOL_NEQ = 14,                       /* NEQ  */
  YYSYMBOL_LT = 15,                        /* LT  */
  YYSYMBOL_GT = 16,                        /* GT  */
  YYSYMBOL_LTE = 17,                       /* LTE  */
  YYSYMBOL_GTE = 18,                       /* GTE  */
  YYSYMBOL_19_ = 19,                       /* '+'  */
  YYSYMBOL_20_ = 20,                       /* '-'  */
  YYSYMBOL_21_ = 21,                       /* '*'  */
  YYSYMBOL_22_ = 22,                       /* '/'  */
  YYSYMBOL_23_ = 23,                       /* ';'  */
  YYSYMBOL_24_ = 24,                       /* '('  */
  YYSYMBOL_25_ = 25,                       /* ')'  */
  YYSYMBOL_26_ = 26,                       /* ','  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* '['  */
  YYSYMBOL_30_ = 30,                       /* ']'  */
  YYSYMBOL_31_ = 31,                       /* '='  */
  YYSYMBOL_YYACCEPT = 32,                  /* $accept  */
  YYSYMBOL_program = 33,                   /* program  */
  YYSYMBOL_stmt_list = 34,                 /* stmt_list  */
  YYSYMBOL_stmt = 35,                      /* stmt  */
  YYSYMBOL_func_decl = 36,                 /* func_decl  */
  YYSYMBOL_param_list = 37,                /* param_list  */
  YYSYMBOL_param = 38,                     /* param  */
  YYSYMBOL_block = 39,                     /* block  */
  YYSYMBOL_return_stmt = 40,               /* return_stmt  */
  YYSYMBOL_func_call = 41,                 /* func_call  */
  YYSYMBOL_if_stmt = 42,                   /* if_stmt  */
  YYSYMBOL_arg_list = 43,                  /* arg_list  */
  YYSYMBOL_decl = 44,                      /* decl  */
  YYSYMBOL_assign = 45,                    /* assign  */
  YYSYMBOL_declAssign = 46,                /* declAssign  */
  YYSYMBOL_47_1 = 47,                      /* $@1  */
  YYSYMBOL_expr = 48,                      /* expr  */
  YYSYMBOL_print_stmt = 49,                /* print_stmt  */
  YYSYMBOL_arrayExpr = 50                  /* arrayExpr  */
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
#define YYFINAL  38
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   370

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  32
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  19
/* YYNRULES -- Number of rules.  */
#define YYNRULES  69
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  175

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   273


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
      24,    25,    21,    19,    26,    20,     2,    22,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    23,
       2,    31,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    29,     2,    30,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,     2,    28,     2,     2,     2,     2,
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
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    61,    61,    69,    74,    78,    83,    90,    91,    92,
      93,    94,    95,    96,    97,   102,   106,   110,   114,   118,
     125,   128,   134,   138,   145,   148,   154,   157,   163,   167,
     174,   177,   184,   187,   193,   200,   207,   216,   223,   230,
     241,   246,   251,   261,   261,   269,   277,   283,   289,   295,
     304,   309,   313,   317,   322,   326,   331,   334,   338,   342,
     346,   350,   354,   358,   362,   366,   370,   379,   387,   390
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
  "INT", "PRINT", "FLOAT", "RETURN", "VOID", "IF", "ELSE", "EQ", "NEQ",
  "LT", "GT", "LTE", "GTE", "'+'", "'-'", "'*'", "'/'", "';'", "'('",
  "')'", "','", "'{'", "'}'", "'['", "']'", "'='", "$accept", "program",
  "stmt_list", "stmt", "func_decl", "param_list", "param", "block",
  "return_stmt", "func_call", "if_stmt", "arg_list", "decl", "assign",
  "declAssign", "$@1", "expr", "print_stmt", "arrayExpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-76)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      75,    99,     3,     4,    14,   115,    20,    12,    38,    52,
      75,   -76,   -76,   -76,   -76,    40,   -76,   -76,   -76,   -76,
     -76,    90,    10,    10,   118,    10,    93,   -76,   -76,   -17,
     -76,    10,   -76,   252,    43,    10,   -76,    64,   -76,   -76,
     -76,   -76,   -76,   126,   318,   141,   263,   -76,    15,    -1,
      33,   213,   -76,    16,     2,    10,    10,   226,    10,    10,
      10,    10,    10,    10,    10,    10,    10,    10,   -76,    51,
     239,   -76,   -76,    10,     8,   -76,    83,    94,    76,   160,
     -76,   106,    98,    10,   122,    76,   162,   140,   152,   274,
     159,   -76,   -76,   -76,   -76,   -76,   -76,   -76,   328,   338,
     348,   151,    76,   100,   318,    10,    10,   -76,   -76,   -76,
      76,    71,    67,   173,   285,   -76,   -76,    76,   103,   174,
     -76,   175,   -76,   197,   198,   193,   177,   296,   -76,   -76,
     -76,   203,   191,    10,   -76,   -76,   -76,   216,   194,    10,
      10,   120,   117,   100,   189,   -76,   192,    10,   318,    61,
     206,    10,    84,   195,   -76,    10,   200,   105,    10,   201,
     214,   156,   227,   -76,   307,   -76,   240,   318,   -76,   -76,
     324,   -76,   -76,   -76,   -76
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       2,     3,     4,    12,    11,     0,    14,     7,     8,     9,
      10,     0,     0,     0,    43,     0,     0,    51,    52,    53,
      27,     0,    66,     0,     0,     0,    25,     0,     1,     5,
       6,    13,    29,     0,    32,     0,     0,    34,     0,     0,
       0,     0,    35,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,    24,    28,     0,     0,    41,     0,     0,     0,     0,
      20,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    50,    60,    61,    62,    63,    64,    65,    56,    57,
      58,    59,     0,     0,    33,     0,     0,    22,    23,    16,
       0,     0,     0,     0,     0,    67,    18,     0,     0,     0,
      45,    54,    19,     0,     0,    30,     0,     0,    15,    21,
      36,     0,     0,     0,    44,    17,    38,     0,     0,     0,
       0,    43,     0,     0,     0,    40,     0,     0,    68,     0,
       0,     0,     0,     0,    31,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,    37,     0,    69,    47,    39,
       0,    49,    42,    46,    48
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -76,   -76,   241,    -6,    -4,   209,   246,   -75,   -76,     1,
     -76,   -76,   -76,   -76,   -76,   -76,    -5,   -76,   -26
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     9,    10,    11,    12,    79,    80,    13,    14,    32,
      16,    43,    17,    18,    19,    50,   148,    20,   149
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      33,    15,    81,   109,    39,    87,    40,    21,    24,    15,
     116,    15,    56,    27,    28,    29,    44,    45,    46,    26,
      51,    76,    76,    77,    77,    34,    57,   122,    25,    82,
      70,    39,    88,    40,    31,   128,    35,   105,    15,   106,
      78,    85,   135,     1,     2,     3,     4,     5,     6,     7,
      89,    90,    38,    92,    93,    94,    95,    96,    97,    98,
      99,   100,   101,    41,    83,     8,    36,    69,   104,     1,
       2,     3,     4,     5,     6,     7,   102,    76,   114,    77,
       1,     2,     3,     4,     5,     6,     7,   158,   107,   159,
     130,     8,    71,    27,    28,    29,   131,   125,   132,   108,
     126,   127,     8,     8,    15,     1,   123,     3,   124,     5,
     158,     7,   162,   152,    31,    42,    52,    53,    27,    28,
      29,   157,    54,    21,    55,   161,   136,     8,    22,   113,
      23,   158,   137,   166,   138,   153,   112,   154,    30,    31,
      52,    47,    48,    47,    15,   115,    54,    49,    55,    49,
     164,    72,    73,   167,    58,    59,    60,    61,    62,    63,
      64,    65,    66,    67,    58,    59,    60,    61,    62,    63,
     118,    74,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,   158,   119,   170,   110,   111,   117,   111,   121,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     133,   139,   141,   142,   140,   143,   146,   144,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,   147,   150,
     155,   151,   156,   165,   168,   163,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,   160,   169,    84,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    37,
     171,    91,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    86,   173,   103,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    75,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    67,   120,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,   134,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,   145,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     172,    58,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    58,    59,    60,    61,    62,    63,   174,    65,    66,
      67,    58,    59,    60,    61,    62,    63,   129,     0,    66,
      67,    58,    59,    60,    61,    62,    63,     0,     0,     0,
      67
};

static const yytype_int16 yycheck[] =
{
       5,     0,     3,    78,    10,     3,    10,    24,     5,     8,
      85,    10,    29,     3,     4,     5,    21,    22,    23,     5,
      25,     6,     6,     8,     8,     5,    31,   102,    24,    30,
      35,    37,    30,    37,    24,   110,    24,    29,    37,    31,
      25,    25,   117,     5,     6,     7,     8,     9,    10,    11,
      55,    56,     0,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    23,    31,    27,    28,    24,    73,     5,
       6,     7,     8,     9,    10,    11,    25,     6,    83,     8,
       5,     6,     7,     8,     9,    10,    11,    26,     5,    28,
      23,    27,    28,     3,     4,     5,    29,   103,    31,     5,
     105,   106,    27,    27,   103,     5,     6,     7,     8,     9,
      26,    11,    28,   139,    24,    25,    23,    24,     3,     4,
       5,   147,    29,    24,    31,   151,    23,    27,    29,    31,
      31,    26,    29,    28,    31,   140,    30,   143,    23,    24,
      23,    23,    24,    23,   143,    23,    29,    29,    31,    29,
     155,    25,    26,   158,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    13,    14,    15,    16,    17,    18,
      30,    30,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    26,    31,    28,    25,    26,    25,    26,    30,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      27,    27,     5,     5,    29,    12,     3,    30,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    27,     3,
      31,    27,    30,    23,    23,    30,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    30,    23,    25,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,     8,
      23,    25,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    53,    23,    25,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    13,    14,    15,    16,    17,    18,    23,    20,    21,
      22,    13,    14,    15,    16,    17,    18,   111,    -1,    21,
      22,    13,    14,    15,    16,    17,    18,    -1,    -1,    -1,
      22
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,     6,     7,     8,     9,    10,    11,    27,    33,
      34,    35,    36,    39,    40,    41,    42,    44,    45,    46,
      49,    24,    29,    31,     5,    24,     5,     3,     4,     5,
      23,    24,    41,    48,     5,    24,    28,    34,     0,    35,
      36,    23,    25,    43,    48,    48,    48,    23,    24,    29,
      47,    48,    23,    24,    29,    31,    29,    48,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      48,    28,    25,    26,    30,    23,     6,     8,    25,    37,
      38,     3,    30,    31,    25,    25,    37,     3,    30,    48,
      48,    25,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    25,    25,    48,    29,    31,     5,     5,    39,
      25,    26,    30,    31,    48,    23,    39,    25,    30,    31,
      23,    30,    39,     6,     8,    35,    48,    48,    39,    38,
      23,    29,    31,    27,    23,    39,    23,    29,    31,    27,
      29,     5,     5,    12,    30,    23,     3,    27,    48,    50,
       3,    27,    50,    48,    35,    31,    30,    50,    26,    28,
      30,    50,    28,    30,    48,    23,    28,    48,    23,    23,
      28,    23,    23,    23,    23
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    32,    33,    34,    34,    34,    34,    35,    35,    35,
      35,    35,    35,    35,    35,    36,    36,    36,    36,    36,
      37,    37,    38,    38,    39,    39,    40,    40,    41,    41,
      42,    42,    43,    43,    44,    44,    44,    44,    44,    44,
      45,    45,    45,    47,    46,    46,    46,    46,    46,    46,
      48,    48,    48,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    48,    48,    49,    50,    50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     2,     2,     1,     1,     1,
       1,     1,     1,     2,     1,     6,     5,     6,     5,     5,
       1,     3,     2,     2,     3,     2,     3,     2,     4,     3,
       5,     7,     1,     3,     3,     3,     6,     9,     6,     9,
       7,     4,    10,     0,     6,     5,    10,     9,    10,     9,
       3,     1,     1,     1,     4,     7,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     5,     1,     3
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
#line 61 "parser.y"
              { 
        /* Action: Save the statement list as our AST root */
        root = (yyvsp[0].node);  /* $1 refers to the first symbol (stmt_list) */
    }
#line 1270 "parser.tab.c"
    break;

  case 3: /* stmt_list: stmt  */
#line 69 "parser.y"
         { 
        /* Base case: single statement */
        (yyval.node) = (yyvsp[0].node);  /* Pass the statement up as-is */
    }
#line 1279 "parser.tab.c"
    break;

  case 4: /* stmt_list: func_decl  */
#line 74 "parser.y"
              { 
        /* Base case: single function declaration */
        (yyval.node) = (yyvsp[0].node);  /* Pass the function declaration up as-is */
    }
#line 1288 "parser.tab.c"
    break;

  case 5: /* stmt_list: stmt_list stmt  */
#line 78 "parser.y"
                     { 
        /* Recursive case: list followed by another statement */
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node));  /* Build linked list of statements */
    }
#line 1297 "parser.tab.c"
    break;

  case 6: /* stmt_list: stmt_list func_decl  */
#line 83 "parser.y"
                        {
        (yyval.node) = createStmtList((yyvsp[-1].node), (yyvsp[0].node)); /* Append function declaration to statement list */
    }
#line 1305 "parser.tab.c"
    break;

  case 15: /* func_decl: INT ID '(' param_list ')' block  */
#line 102 "parser.y"
                                    {
        (yyval.node) = createFuncDecl("int", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node)); /* Create function declaration node */
        free((yyvsp[-4].str)); /* Free the function name string */
    }
#line 1314 "parser.tab.c"
    break;

  case 16: /* func_decl: INT ID '(' ')' block  */
#line 106 "parser.y"
                           {
        (yyval.node) = createFuncDecl("int", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1323 "parser.tab.c"
    break;

  case 17: /* func_decl: FLOAT ID '(' param_list ')' block  */
#line 110 "parser.y"
                                        {
        (yyval.node) = createFuncDecl("float", (yyvsp[-4].str), (yyvsp[-2].node), (yyvsp[0].node));
        free((yyvsp[-4].str));
    }
#line 1332 "parser.tab.c"
    break;

  case 18: /* func_decl: FLOAT ID '(' ')' block  */
#line 114 "parser.y"
                             {
        (yyval.node) = createFuncDecl("float", (yyvsp[-3].str), NULL, (yyvsp[0].node));
        free((yyvsp[-3].str));
    }
#line 1341 "parser.tab.c"
    break;

  case 19: /* func_decl: VOID ID '(' ')' block  */
#line 118 "parser.y"
                            {
        (yyval.node) = createFuncDecl("void", (yyvsp[-3].str), NULL, (yyvsp[0].node)); /* Void function with no parameters */
        free((yyvsp[-3].str));
    }
#line 1350 "parser.tab.c"
    break;

  case 20: /* param_list: param  */
#line 125 "parser.y"
          {
        (yyval.node) = (yyvsp[0].node); /* Single parameter */
    }
#line 1358 "parser.tab.c"
    break;

  case 21: /* param_list: param_list ',' param  */
#line 128 "parser.y"
                           {
        (yyval.node) = createParamList((yyvsp[-2].node), (yyvsp[0].node)); /* Append parameter to list */
    }
#line 1366 "parser.tab.c"
    break;

  case 22: /* param: INT ID  */
#line 134 "parser.y"
           {
        (yyval.node) = createParam("int", (yyvsp[0].str)); /* Create parameter node */
        free((yyvsp[0].str));
    }
#line 1375 "parser.tab.c"
    break;

  case 23: /* param: FLOAT ID  */
#line 138 "parser.y"
               {  /* ✅ ADD THIS */
        (yyval.node) = createParam("float", (yyvsp[0].str));
        free((yyvsp[0].str));
    }
#line 1384 "parser.tab.c"
    break;

  case 24: /* block: '{' stmt_list '}'  */
#line 145 "parser.y"
                      {
        (yyval.node) = createBlock((yyvsp[-1].node)); /* Create block node with statement list */
    }
#line 1392 "parser.tab.c"
    break;

  case 25: /* block: '{' '}'  */
#line 148 "parser.y"
              {
        (yyval.node) = createBlock(NULL); /* Empty block */
    }
#line 1400 "parser.tab.c"
    break;

  case 26: /* return_stmt: RETURN expr ';'  */
#line 154 "parser.y"
                    {
        (yyval.node) = createReturn((yyvsp[-1].node)); /* Create return statement node */
    }
#line 1408 "parser.tab.c"
    break;

  case 27: /* return_stmt: RETURN ';'  */
#line 157 "parser.y"
                 {
        (yyval.node) = createReturn(NULL); /* Return with no value */
    }
#line 1416 "parser.tab.c"
    break;

  case 28: /* func_call: ID '(' arg_list ')'  */
#line 163 "parser.y"
                        {
        (yyval.node) = createFuncCall((yyvsp[-3].str), (yyvsp[-1].node)); /* Create function call node */
        free((yyvsp[-3].str)); /* Free function name string */
    }
#line 1425 "parser.tab.c"
    break;

  case 29: /* func_call: ID '(' ')'  */
#line 167 "parser.y"
                 {
        (yyval.node) = createFuncCall((yyvsp[-2].str), NULL); /* Function call with no arguments */
        free((yyvsp[-2].str));
    }
#line 1434 "parser.tab.c"
    break;

  case 30: /* if_stmt: IF '(' expr ')' stmt  */
#line 175 "parser.y"
        { (yyval.node) = createIfNode((yyvsp[-2].node), (yyvsp[0].node), NULL); /* Create if-else statement node */
    }
#line 1441 "parser.tab.c"
    break;

  case 31: /* if_stmt: IF '(' expr ')' stmt ELSE stmt  */
#line 178 "parser.y"
        { (yyval.node) = createIfNode((yyvsp[-4].node), (yyvsp[-2].node), (yyvsp[0].node)); /* Create if statement node */
    }
#line 1448 "parser.tab.c"
    break;

  case 32: /* arg_list: expr  */
#line 184 "parser.y"
         {
        (yyval.node) = createArgList((yyvsp[0].node), NULL); /* Single argument */
    }
#line 1456 "parser.tab.c"
    break;

  case 33: /* arg_list: arg_list ',' expr  */
#line 187 "parser.y"
                        {
        (yyval.node) = createArgList((yyvsp[0].node), (yyvsp[-2].node)); /* Append argument to list */
    }
#line 1464 "parser.tab.c"
    break;

  case 34: /* decl: INT ID ';'  */
#line 193 "parser.y"
               { 

        /* Create declaration node and free the identifier string */
        (yyval.node) = createDecl("int", (yyvsp[-1].str));  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free((yyvsp[-1].str));             /* Free the string copy from scanner */
    }
#line 1475 "parser.tab.c"
    break;

  case 35: /* decl: FLOAT ID ';'  */
#line 200 "parser.y"
                   {
        (yyval.node) = createDecl("float", (yyvsp[-1].str));
        free((yyvsp[-1].str));
    }
#line 1484 "parser.tab.c"
    break;

  case 36: /* decl: INT ID '[' NUM ']' ';'  */
#line 208 "parser.y"
    {
        (yyval.node) = createArrayDeclOfLength("int", (yyvsp[-4].str), (yyvsp[-2].num));/* done */
        addArrayVar((yyvsp[-4].str), (yyvsp[-2].num), "int");
        free((yyvsp[-4].str));
    }
#line 1494 "parser.tab.c"
    break;

  case 37: /* decl: INT ID '[' NUM ']' '[' NUM ']' ';'  */
#line 217 "parser.y"
    {
        addArray2DVar((yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num), "int");
        (yyval.node) = create2DArrayDeclOfLength("int", (yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-7].str));
    }
#line 1504 "parser.tab.c"
    break;

  case 38: /* decl: FLOAT ID '[' NUM ']' ';'  */
#line 224 "parser.y"
    {
        (yyval.node) = createArrayDeclOfLength("float", (yyvsp[-4].str), (yyvsp[-2].num));/* done */
        addArrayVar((yyvsp[-4].str), (yyvsp[-2].num), "float");
        free((yyvsp[-4].str));
    }
#line 1514 "parser.tab.c"
    break;

  case 39: /* decl: FLOAT ID '[' NUM ']' '[' NUM ']' ';'  */
#line 231 "parser.y"
    {
        addArray2DVar((yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num), "float");
        (yyval.node) = create2DArrayDeclOfLength("float", (yyvsp[-7].str), (yyvsp[-5].num), (yyvsp[-2].num)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-7].str));
    }
#line 1524 "parser.tab.c"
    break;

  case 40: /* assign: ID '[' expr ']' '=' expr ';'  */
#line 241 "parser.y"
                                 {
        (yyval.node) = createArrayElemAssign((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); 
        free((yyvsp[-6].str));
    }
#line 1533 "parser.tab.c"
    break;

  case 41: /* assign: ID '=' expr ';'  */
#line 246 "parser.y"
                      { 
        (yyval.node) = createAssign((yyvsp[-3].str), (yyvsp[-1].node));  
        free((yyvsp[-3].str));
    }
#line 1542 "parser.tab.c"
    break;

  case 42: /* assign: ID '[' expr ']' '[' expr ']' '=' expr ';'  */
#line 252 "parser.y"
    {
        (yyval.node) = createArray2DElemAssign((yyvsp[-9].str), (yyvsp[-7].node), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h (done) -> ast.c (done)-> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-9].str));
    }
#line 1551 "parser.tab.c"
    break;

  case 43: /* $@1: %empty  */
#line 261 "parser.y"
           { 
    }
#line 1558 "parser.tab.c"
    break;

  case 44: /* declAssign: INT ID $@1 '=' expr ';'  */
#line 263 "parser.y"
    {
        (yyval.node) = createDeclAssign("int", (yyvsp[-4].str), (yyvsp[-1].node)); /* done */
        free((yyvsp[-4].str));
    }
#line 1567 "parser.tab.c"
    break;

  case 45: /* declAssign: FLOAT ID '=' expr ';'  */
#line 269 "parser.y"
                         { 

        /* Create declaration node and free the identifier string */
        (yyval.node) = createDeclAssign("float", (yyvsp[-3].str), (yyvsp[-1].node));  /* $2 is the ID token's string value; stored in the symbol table; returns into $$, which is a pointer to a sub tree. */
        free((yyvsp[-3].str));             /* Free the string copy from scanner */
    }
#line 1578 "parser.tab.c"
    break;

  case 46: /* declAssign: INT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'  */
#line 278 "parser.y"
    {
        (yyval.node) = createArrayAssign("int", (yyvsp[-8].str), (yyvsp[-6].num), (yyvsp[-2].node));/* done */
        free((yyvsp[-8].str));
    }
#line 1587 "parser.tab.c"
    break;

  case 47: /* declAssign: INT ID '[' ']' '=' '{' arrayExpr '}' ';'  */
#line 284 "parser.y"
    {
        (yyval.node) = createArrayDeclAssign("int", (yyvsp[-7].str), 0, (yyvsp[-2].node));/* done */
        free((yyvsp[-7].str));
    }
#line 1596 "parser.tab.c"
    break;

  case 48: /* declAssign: FLOAT ID '[' NUM ']' '=' '{' arrayExpr '}' ';'  */
#line 290 "parser.y"
    {
        (yyval.node) = createArrayAssign("float", (yyvsp[-8].str), (yyvsp[-6].num), (yyvsp[-2].node));/* done */
        free((yyvsp[-8].str));
    }
#line 1605 "parser.tab.c"
    break;

  case 49: /* declAssign: FLOAT ID '[' ']' '=' '{' arrayExpr '}' ';'  */
#line 296 "parser.y"
    {
        (yyval.node) = createArrayDeclAssign("float", (yyvsp[-7].str), 0, (yyvsp[-2].node));/* done */
        free((yyvsp[-7].str));
    }
#line 1614 "parser.tab.c"
    break;

  case 50: /* expr: '(' expr ')'  */
#line 304 "parser.y"
                 { 
        /* Parenthesized expression - just pass up the inner expression */
        (yyval.node) = (yyvsp[-1].node);  /* $2 is the expr inside the parentheses */
    }
#line 1623 "parser.tab.c"
    break;

  case 51: /* expr: NUM  */
#line 309 "parser.y"
        { 
        /* Literal number */
        (yyval.node) = createNum((yyvsp[0].num), 0);  /* Create leaf node with number value */
    }
#line 1632 "parser.tab.c"
    break;

  case 52: /* expr: FNUM  */
#line 313 "parser.y"
           { 
        /* Literal float number */
        (yyval.node) = createNum((yyvsp[0].fnum), 1);  /* Create leaf node with float value */
    }
#line 1641 "parser.tab.c"
    break;

  case 53: /* expr: ID  */
#line 317 "parser.y"
         { 
        /* Variable reference */
        (yyval.node) = createVar((yyvsp[0].str));  /* Create leaf node with variable name */
        free((yyvsp[0].str));            /* Free the identifier string */
    }
#line 1651 "parser.tab.c"
    break;

  case 54: /* expr: ID '[' expr ']'  */
#line 322 "parser.y"
                      { /* allows to access whatever value is at index 'expr' */
        (yyval.node) = createArrayAccess((yyvsp[-3].str), (yyvsp[-1].node));
        free((yyvsp[-3].str));
    }
#line 1660 "parser.tab.c"
    break;

  case 55: /* expr: ID '[' expr ']' '[' expr ']'  */
#line 327 "parser.y"
    {
        (yyval.node) = createArray2DAccess((yyvsp[-6].str), (yyvsp[-4].node), (yyvsp[-1].node)); /* scanner.l -> parser.y -> ast.h -> ast.c -> symtab.h -> symtab.c -> codegen.c -> tac.h -> tac.c */
        free((yyvsp[-6].str));
    }
#line 1669 "parser.tab.c"
    break;

  case 56: /* expr: expr '+' expr  */
#line 331 "parser.y"
                    { 
        /* Addition operation - builds binary tree */
        (yyval.node) = createBinOp('+', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1678 "parser.tab.c"
    break;

  case 57: /* expr: expr '-' expr  */
#line 334 "parser.y"
                      { 
        /* Subtraction operation - builds binary tree */
        (yyval.node) = createBinOp('-', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1687 "parser.tab.c"
    break;

  case 58: /* expr: expr '*' expr  */
#line 338 "parser.y"
                    { 
        /* Multiplication operation - builds binary tree */
        (yyval.node) = createBinOp('*', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1696 "parser.tab.c"
    break;

  case 59: /* expr: expr '/' expr  */
#line 342 "parser.y"
                    { 
        /* Division operation - builds binary tree */
        (yyval.node) = createBinOp('/', (yyvsp[-2].node), (yyvsp[0].node));  /* Left child, op, right child */
    }
#line 1705 "parser.tab.c"
    break;

  case 60: /* expr: expr EQ expr  */
#line 346 "parser.y"
                   {
        /* Equality comparison */
        (yyval.node) = createBinOp(EQ, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1714 "parser.tab.c"
    break;

  case 61: /* expr: expr NEQ expr  */
#line 350 "parser.y"
                    {
        /* Not equal comparison */
        (yyval.node) = createBinOp(NEQ, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1723 "parser.tab.c"
    break;

  case 62: /* expr: expr LT expr  */
#line 354 "parser.y"
                   {
        /* Less than comparison */
        (yyval.node) = createBinOp(LT, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1732 "parser.tab.c"
    break;

  case 63: /* expr: expr GT expr  */
#line 358 "parser.y"
                   {
        /* Greater than comparison */
        (yyval.node) = createBinOp(GT, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1741 "parser.tab.c"
    break;

  case 64: /* expr: expr LTE expr  */
#line 362 "parser.y"
                    {
        /* Less than or equal comparison */
        (yyval.node) = createBinOp(LTE, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1750 "parser.tab.c"
    break;

  case 65: /* expr: expr GTE expr  */
#line 366 "parser.y"
                    {
        /* Greater than or equal comparison */
        (yyval.node) = createBinOp(GTE, (yyvsp[-2].node), (yyvsp[0].node));
    }
#line 1759 "parser.tab.c"
    break;

  case 66: /* expr: func_call  */
#line 370 "parser.y"
                { 
        /* ADDED */
        /* Function call as an expression */
        (yyval.node) = (yyvsp[0].node);  /* Just pass up the function call node */
    }
#line 1769 "parser.tab.c"
    break;

  case 67: /* print_stmt: PRINT '(' expr ')' ';'  */
#line 379 "parser.y"
                           { 
        /* Create print node with expression to print */
        (yyval.node) = createPrint((yyvsp[-2].node));  /* $3 is the expression inside parens */
    }
#line 1778 "parser.tab.c"
    break;

  case 68: /* arrayExpr: expr  */
#line 387 "parser.y"
         {
        (yyval.node) = createExprList((yyvsp[0].node), NULL); /* done */
    }
#line 1786 "parser.tab.c"
    break;

  case 69: /* arrayExpr: arrayExpr ',' expr  */
#line 390 "parser.y"
                         {
        (yyval.node) = createExprList((yyvsp[0].node), (yyvsp[-2].node)); /* done */
    }
#line 1794 "parser.tab.c"
    break;


#line 1798 "parser.tab.c"

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

#line 405 "parser.y"


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
