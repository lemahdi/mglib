/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2010 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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


/* First part of user declarations.  */

/* Line 310 of lalr1.cc  */
#line 9 "src/mginfra/my_parser.ypp"

#include <iostream>
using namespace std;
#include "mginfra/nodes.h"
#include "mginfra/arg.h"

#pragma warning(push)
#pragma warning(disable:4065)
#pragma warning(disable:4267)
#pragma warning(disable:4512)
#pragma warning(disable:4127)
#pragma warning(disable:4702)



/* Line 310 of lalr1.cc  */
#line 54 "my_parser.tab.cpp"


#include "my_parser.tab.hpp"

/* User implementation prologue.  */

/* Line 316 of lalr1.cc  */
#line 54 "src/mginfra/my_parser.ypp"

extern int yylex(yy::my_parser::semantic_type *yylval,
	yy::my_parser::location_type* yyloc,
	MG::MG_TableWalker &walker, MG::MG_NodeManager &manager);



/* Line 316 of lalr1.cc  */
#line 71 "my_parser.tab.cpp"

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_REDUCE_PRINT(Rule)
# define YY_STACK_PRINT()

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {

/* Line 379 of lalr1.cc  */
#line 137 "my_parser.tab.cpp"
#if YYERROR_VERBOSE

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  my_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              /* Fall through.  */
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

#endif

  /// Build a parser object.
  my_parser::my_parser (MG::MG_TableWalker &walker_yyarg, MG::MG_NodeManager &manager_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      walker (walker_yyarg),
      manager (manager_yyarg)
  {
  }

  my_parser::~my_parser ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  my_parser::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  my_parser::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  my_parser::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  my_parser::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  my_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  my_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  my_parser::debug_level_type
  my_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  my_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  int
  my_parser::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    /* State.  */
    int yyn;
    int yylen = 0;
    int yystate = 0;

    /* Error handling.  */
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    semantic_type yylval;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[2];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    YYCDEBUG << "Starting parse" << std::endl;


    /* User initialization code.  */
    
/* Line 552 of lalr1.cc  */
#line 61 "src/mginfra/my_parser.ypp"
{
	// Filename for locations here
	yylloc.begin.filename = yylloc.end.filename = new std::string("stdin");
}

/* Line 552 of lalr1.cc  */
#line 322 "my_parser.tab.cpp"

    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yyn == yypact_ninf_)
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
	YYCDEBUG << "Reading a token: ";
	yychar = yylex (&yylval, &yylloc, walker, manager);
      }


    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yyn == 0 || yyn == yytable_ninf_)
	goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
	  case 2:

/* Line 677 of lalr1.cc  */
#line 68 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNode(walker, ADD_NODE, (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 3:

/* Line 677 of lalr1.cc  */
#line 69 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNode(walker, SUB_NODE, (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 4:

/* Line 677 of lalr1.cc  */
#line 70 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNode(walker, MUL_NODE, (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 5:

/* Line 677 of lalr1.cc  */
#line 71 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNode(walker, DIV_NODE, (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 6:

/* Line 677 of lalr1.cc  */
#line 72 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = (yysemantic_stack_[(3) - (2)].nval); }
    break;

  case 7:

/* Line 677 of lalr1.cc  */
#line 73 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNode(walker, NEG_NODE, NULL, (yysemantic_stack_[(2) - (2)].nval)); }
    break;

  case 8:

/* Line 677 of lalr1.cc  */
#line 74 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildCmpNode(walker, CMP_NODE, (yysemantic_stack_[(3) - (2)].uval), (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 9:

/* Line 677 of lalr1.cc  */
#line 75 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildNum(walker, (yysemantic_stack_[(1) - (1)].dval)); }
    break;

  case 10:

/* Line 677 of lalr1.cc  */
#line 76 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildDate(walker, (yysemantic_stack_[(1) - (1)].lval)); }
    break;

  case 11:

/* Line 677 of lalr1.cc  */
#line 77 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildRef(walker, (yysemantic_stack_[(4) - (1)].cval), (yysemantic_stack_[(4) - (3)].ival)); free((yysemantic_stack_[(4) - (1)].cval)); }
    break;

  case 12:

/* Line 677 of lalr1.cc  */
#line 78 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildFunc(walker, (yysemantic_stack_[(4) - (1)].cval), (yysemantic_stack_[(4) - (3)].nval)); free((yysemantic_stack_[(4) - (1)].cval)); }
    break;

  case 13:

/* Line 677 of lalr1.cc  */
#line 81 "src/mginfra/my_parser.ypp"
    { if (manager.CheckIndex((yysemantic_stack_[(1) - (1)].cval))) (yyval.ival) = 0; }
    break;

  case 14:

/* Line 677 of lalr1.cc  */
#line 82 "src/mginfra/my_parser.ypp"
    { (yyval.ival) = (int)(yysemantic_stack_[(3) - (3)].dval); }
    break;

  case 15:

/* Line 677 of lalr1.cc  */
#line 83 "src/mginfra/my_parser.ypp"
    { (yyval.ival) = -(int)(yysemantic_stack_[(3) - (3)].dval); }
    break;

  case 16:

/* Line 677 of lalr1.cc  */
#line 86 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = NULL; }
    break;

  case 17:

/* Line 677 of lalr1.cc  */
#line 87 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildArg(walker, (yysemantic_stack_[(1) - (1)].nval), NULL); }
    break;

  case 18:

/* Line 677 of lalr1.cc  */
#line 88 "src/mginfra/my_parser.ypp"
    { (yyval.nval) = manager.BuildArg(walker, (yysemantic_stack_[(3) - (1)].nval), (yysemantic_stack_[(3) - (3)].nval)); }
    break;

  case 20:

/* Line 677 of lalr1.cc  */
#line 93 "src/mginfra/my_parser.ypp"
    {
		cout << "= ";
		cout << manager.Eval((yysemantic_stack_[(3) - (2)].nval)).Double();
		walker.IncCurrentCol();
		cout << endl << "> ";
	}
    break;

  case 21:

/* Line 677 of lalr1.cc  */
#line 99 "src/mginfra/my_parser.ypp"
    { /* blank line or a comment */
		cout << "> ";
	}
    break;



/* Line 677 of lalr1.cc  */
#line 581 "my_parser.tab.cpp"
	default:
          break;
      }
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	error (yylloc, yysyntax_error_ (yystate));
      }

    yyerror_range[0] = yylloc;
    if (yyerrstatus_ == 3)
      {
	/* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

	if (yychar <= yyeof_)
	  {
	  /* Return failure if at end of input.  */
	  if (yychar == yyeof_)
	    YYABORT;
	  }
	else
	  {
	    yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
	    yychar = yyempty_;
	  }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;

    yyerror_range[0] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
	if (yyn != yypact_ninf_)
	{
	  yyn += yyterror_;
	  if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
	    {
	      yyn = yytable_[yyn];
	      if (0 < yyn)
		break;
	    }
	}

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	YYABORT;

	yyerror_range[0] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[1] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval, &yylloc);

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (yystate_stack_.height () != 1)
      {
	yydestruct_ ("Cleanup: popping",
		   yystos_[yystate_stack_[0]],
		   &yysemantic_stack_[0],
		   &yylocation_stack_[0]);
	yypop_ ();
      }

    return yyresult;
  }

  // Generate an error message.
  std::string
  my_parser::yysyntax_error_ (int yystate)
  {
    std::string res;
    YYUSE (yystate);
#if YYERROR_VERBOSE
    int yyn = yypact_[yystate];
    if (yypact_ninf_ < yyn && yyn <= yylast_)
      {
	/* Start YYX at -YYN if negative to avoid negative indexes in
	   YYCHECK.  */
	int yyxbegin = yyn < 0 ? -yyn : 0;

	/* Stay within bounds of both yycheck and yytname.  */
	int yychecklim = yylast_ - yyn + 1;
	int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
	int count = 0;
	for (int x = yyxbegin; x < yyxend; ++x)
	  if (yycheck_[x + yyn] == x && x != yyterror_)
	    ++count;

	// FIXME: This method of building the message is not compatible
	// with internationalization.  It should work like yacc.c does it.
	// That is, first build a string that looks like this:
	// "syntax error, unexpected %s or %s or %s"
	// Then, invoke YY_ on this string.
	// Finally, use the string as a format to output
	// yytname_[tok], etc.
	// Until this gets fixed, this message appears in English only.
	res = "syntax error, unexpected ";
	res += yytnamerr_ (yytname_[tok]);
	if (count < 5)
	  {
	    count = 0;
	    for (int x = yyxbegin; x < yyxend; ++x)
	      if (yycheck_[x + yyn] == x && x != yyterror_)
		{
		  res += (!count++) ? ", expecting " : " or ";
		  res += yytnamerr_ (yytname_[x]);
		}
	  }
      }
    else
#endif
      res = YY_("syntax error");
    return res;
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char my_parser::yypact_ninf_ = -7;
  const signed char
  my_parser::yypact_[] =
  {
        -7,    19,    -7,    -7,    -5,    -7,    -7,    26,    26,    49,
      26,     4,    -7,    39,    -7,    26,    26,    26,    26,    26,
      -6,    -2,    -7,    28,    -7,    54,    10,    10,    -7,    -7,
      26,    -7,     7,    23,    -7,    -7,    -7,    -7
  };

  /* YYDEFACT[S] -- default rule to reduce with in state S when YYTABLE
     doesn't specify something else to do.  Zero means the default is an
     error.  */
  const unsigned char
  my_parser::yydefact_[] =
  {
        19,     0,     1,     9,     0,    10,    21,     0,     0,     0,
      16,     0,     7,     0,    20,     0,     0,     0,     0,     0,
      17,     0,    13,     0,     6,     8,     2,     3,     4,     5,
      16,    12,     0,     0,    11,    18,    14,    15
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  my_parser::yypgoto_[] =
  {
        -7,    -1,    -7,    -3,    -7
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  my_parser::yydefgoto_[] =
  {
        -1,    20,    23,    21,     1
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If zero, do what YYDEFACT says.  */
  const signed char my_parser::yytable_ninf_ = -1;
  const signed char
  my_parser::yytable_[] =
  {
         9,    15,    16,    17,    18,    19,    12,    13,    22,    10,
      36,    11,    30,    31,    25,    26,    27,    28,    29,     2,
      18,    19,     3,     4,     5,     6,    37,    35,     7,     3,
       4,     5,     0,     8,     0,     7,    32,    33,     0,     0,
       8,     0,     0,     0,     0,    34,    15,    16,    17,    18,
      19,     0,     0,     0,    24,    14,    15,    16,    17,    18,
      19,    -1,    16,    17,    18,    19
  };

  /* YYCHECK.  */
  const signed char
  my_parser::yycheck_[] =
  {
         1,     7,     8,     9,    10,    11,     7,     8,     4,    14,
       3,    16,    18,    15,    15,    16,    17,    18,    19,     0,
      10,    11,     3,     4,     5,     6,     3,    30,     9,     3,
       4,     5,    -1,    14,    -1,     9,     8,     9,    -1,    -1,
      14,    -1,    -1,    -1,    -1,    17,     7,     8,     9,    10,
      11,    -1,    -1,    -1,    15,     6,     7,     8,     9,    10,
      11,     7,     8,     9,    10,    11
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  my_parser::yystos_[] =
  {
         0,    23,     0,     3,     4,     5,     6,     9,    14,    20,
      14,    16,    20,    20,     6,     7,     8,     9,    10,    11,
      20,    22,     4,    21,    15,    20,    20,    20,    20,    20,
      18,    15,     8,     9,    17,    22,     3,     3
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  my_parser::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,    43,    45,
      42,    47,   124,   263,    40,    41,    91,    93,    44
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  my_parser::yyr1_[] =
  {
         0,    19,    20,    20,    20,    20,    20,    20,    20,    20,
      20,    20,    20,    21,    21,    21,    22,    22,    22,    23,
      23,    23
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  my_parser::yyr2_[] =
  {
         0,     2,     3,     3,     3,     3,     3,     2,     3,     1,
       1,     4,     4,     1,     3,     3,     0,     1,     3,     0,
       3,     2
  };

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const my_parser::yytname_[] =
  {
    "$end", "error", "$undefined", "NUMBER", "NAME", "DATE", "EOL", "CMP",
  "'+'", "'-'", "'*'", "'/'", "'|'", "UMINUS", "'('", "')'", "'['", "']'",
  "','", "$accept", "exp", "index", "arglist", "calclist", 0
  };
#endif

#if YYDEBUG
  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const my_parser::rhs_number_type
  my_parser::yyrhs_[] =
  {
        23,     0,    -1,    20,     8,    20,    -1,    20,     9,    20,
      -1,    20,    10,    20,    -1,    20,    11,    20,    -1,    14,
      20,    15,    -1,     9,    20,    -1,    20,     7,    20,    -1,
       3,    -1,     5,    -1,     4,    16,    21,    17,    -1,     4,
      14,    22,    15,    -1,     4,    -1,    21,     8,     3,    -1,
      21,     9,     3,    -1,    -1,    20,    -1,    20,    18,    22,
      -1,    -1,    23,    20,     6,    -1,    23,     6,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  my_parser::yyprhs_[] =
  {
         0,     0,     3,     7,    11,    15,    19,    23,    26,    30,
      32,    34,    39,    44,    46,    50,    54,    55,    57,    61,
      62,    66
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned char
  my_parser::yyrline_[] =
  {
         0,    68,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    81,    82,    83,    86,    87,    88,    92,
      93,    99
  };

  // Print the state stack on the debug stream.
  void
  my_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  my_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  my_parser::token_number_type
  my_parser::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      14,    15,    10,     8,    18,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    16,     2,    17,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    12,     2,     2,     2,     2,     2,
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
       5,     6,     7,    13
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int my_parser::yyeof_ = 0;
  const int my_parser::yylast_ = 65;
  const int my_parser::yynnts_ = 5;
  const int my_parser::yyempty_ = -2;
  const int my_parser::yyfinal_ = 2;
  const int my_parser::yyterror_ = 1;
  const int my_parser::yyerrcode_ = 256;
  const int my_parser::yyntokens_ = 19;

  const unsigned int my_parser::yyuser_token_number_max_ = 263;
  const my_parser::token_number_type my_parser::yyundef_token_ = 2;


} // yy

/* Line 1053 of lalr1.cc  */
#line 1023 "my_parser.tab.cpp"


/* Line 1055 of lalr1.cc  */
#line 104 "src/mginfra/my_parser.ypp"


// C++ code section of parser
namespace yy {
	void
	my_parser::error(location const &loc, const std::string& s) {
		std::cerr << "error at " << loc << ": " << s << std::endl;
	}
}

#ifdef WIN32
#pragma warning(pop)
#endif
