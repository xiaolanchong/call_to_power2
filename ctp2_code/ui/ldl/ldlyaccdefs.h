//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C/C++ header
// Description  : Slic cmd definitions, to make ldl parser different from
//                the slic parsers that is generated by the same tool.
//                and contains the same global symbols.
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2 
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Added more yy* symbols to be replaced by ldl* symbols so that there are 
//   less problems with the other parser charing the same global symbols that
//   caused usage of invalid memory. Actual this is done for sc.tab.c but what
//   a defect is there is a defect here as well, the game just didn't fail.
//   - May 28th 2005 Martin G�hmann
//
//----------------------------------------------------------------------------

#ifndef __LDLYACCDEFS_H__
#define __LDLYACCDEFS_H__

#define yyact ldlact
#define yychar ldlchar
#define yychk ldlchk
#define yydebug ldldebug
#define yydef ldldef
#define yyerrflag ldlerrflag
#define yyerror ldlerror
#define yyexca ldlexca
#define yylex ldllex
#define yylval ldllval
#define yynerrs ldlnerrs
#define yypact ldlpact
#define yyparse ldlparse
#define yypgo ldlpgo
#define yyps ldlps
#define yypv ldlpv
#define yyr1 ldlr1
#define yyr2 ldlr2
#define yyreds ldlreds
#define yys ldls
#define yystate ldlstate
#define yytmp ldltmp
#define yytoks ldltoks
#define yyv ldlv
#define yyval ldlval
// Added by Martin G�hmann to fix external linkage 
// issures in the sliccmd parser
#define yyssp ldlssp
#define yyvsp ldlvsp
#define yyss ldlss
#define yysslim ldlsslim
#define yyvs ldlvs
// Removed since redefined later
//#define yystacksize ldlstacksize

#define yygrowstack ldlgrowstack
#define yyrule ldlrule
#define yyname ldlname
//
#define yycheck ldlcheck
#define yytable ldltable
#define yygindex ldlgindex
#define yyrindex ldlrindex
#define yysindex ldlsindex
#define yydgoto ldldgoto
#define yydefred ldldefred
#define yylen ldllen
#define yylhs ldllhs

#endif
