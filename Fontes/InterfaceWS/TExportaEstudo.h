//---------------------------------------------------------------------------
#ifndef TExportaEstudoH
#define TExportaEstudoH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExportaEstudo.h"


//---------------------------------------------------------------------------
class TExportaEstudo : public VTExportaEstudo
   {
   public:
                 __fastcall  TExportaEstudo(VTApl *apl_owner);
                 __fastcall ~TExportaEstudo(void);
      bool       __fastcall  Executa(AnsiString estudo_codigo, AnsiString arq_txt);
      AnsiString __fastcall  MsgErro(void);

   private: //métodos
//      bool        __fastcall ExecutaWebClient(AnsiString arq_exe, AnsiString estudo_codigo, AnsiString arq_txt);
      void        __fastcall LeRespostaWebClient(void);

   private: //objetos externos
      VTApl    *apl;

   private: //objetos externos
      AnsiString msg_erro;
   };

#endif
//-----------------------------------------------------------------------------
// eof
