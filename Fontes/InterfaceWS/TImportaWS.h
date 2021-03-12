//---------------------------------------------------------------------------
#ifndef TImportaWSH
#define TImportaWSH

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaWS.h"

//---------------------------------------------------------------------------
class TImportaWS: public VTImportaWS
   {
   public:
                 __fastcall  TImportaWS(VTApl *apl_owner);
                 __fastcall ~TImportaWS(void);
      bool       __fastcall  Executa(AnsiString arq_txt);
      AnsiString __fastcall  MsgErro(void);

   private: //m�todos
//      bool __fastcall ExecutaWebClient(AnsiString arq_exe, AnsiString arq_txt);

   private: //objetos externos
      VTApl    *apl;

   private: //dados locais
      AnsiString msg_erro;
   };

#endif
//-----------------------------------------------------------------------------
// eof
