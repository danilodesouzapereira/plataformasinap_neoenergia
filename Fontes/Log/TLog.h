//-----------------------------------------------------------------------------
#ifndef TLogH
#define TLogH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTLog.h"

//---------------------------------------------------------------------------
class TLog : public VTLog
   {
   public:
                 __fastcall TLog(VTApl *apl_owner);
                 __fastcall ~TLog(void);
      bool       __fastcall  Add(AnsiString msg);
      AnsiString __fastcall  ArqLog(void);
      TStrings*  __fastcall  Campos(void);
      void       __fastcall  Close(void);
      void       __fastcall  DefineValor(AnsiString campo, AnsiString valor);
      void       __fastcall  InsereCampo(AnsiString campo, AnsiString valor=" ");
      bool       __fastcall  Open(AnsiString arq_log, bool append=false);
      void       __fastcall  ShowModal(AnsiString Caption);

   private: //métodos
      bool  __fastcall WriteToFile(AnsiString txt);

   private: //objetos externos
      VTApl *apl;

   private: //dados
      AnsiString  arq;
      FILE        *fp;
      TStrings    *StrListLog;
      struct   {
               TStrings *StrListNome;
               TStrings *StrListValor;
               }campos;
   };

#endif
//---------------------------------------------------------------------------
//eof
