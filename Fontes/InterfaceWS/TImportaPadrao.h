//---------------------------------------------------------------------------
#ifndef TImportaPadraoH
#define TImportaPadraoH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaPadrao.h"

//---------------------------------------------------------------------------
class TImportaPadrao : public VTImportaPadrao
   {
   public:
                 __fastcall  TImportaPadrao(VTApl *apl_owner);
                 __fastcall ~TImportaPadrao(void);
      bool       __fastcall  Executa(AnsiString arq_txt);
      AnsiString __fastcall  MsgErro(void);

   private: //métodos
//      bool __fastcall ExecutaWebClient(AnsiString arq_exe, AnsiString arq_txt);

   private: //objetos externos
      VTApl    *apl;

   private: //dados locais
      AnsiString msg_erro;
   };

#endif
//-----------------------------------------------------------------------------
// eof
