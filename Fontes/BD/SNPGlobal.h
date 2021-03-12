//---------------------------------------------------------------------------
//
// SNPGlobal.h
//
// Fun��es de uso geral para os projetos Sinapsis.
//
//   Data   | Autor | Vers�o | Descri��o
// ==========================================================================
// 20080625 |  MML  |  0.00  | Vers�o inicial.
//
//---------------------------------------------------------------------------

#ifndef SNPGlobalH
#define SNPGlobalH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Dialogs.hpp>

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------
#ifndef SNPLOGFILENAME
#define SNPLOGFILENAME "AppExec.log"
#endif

#define SNPSTRLINEID(x)   (x.sprintf("%s (%s) => %08d", WideString(__FUNC__).c_bstr(), WideString(ExtractFileName(__FILE__)).c_bstr(), __LINE__))
#define SNPSTRLOG(x)      if (plog) { plog->Add(x); }
#define SNPSTRLOGLINE(x)  SNPSTRLINEID(x); SNPSTRLOG(x);

#define SNPSTRLOGINFO     "Verifique mensagem gravada no arquivo: %s"

//-----------------------------------------------------------------------------
// Fun��es
//-----------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
 