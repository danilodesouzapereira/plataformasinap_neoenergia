//---------------------------------------------------------------------------
#ifndef TImportaEstudoH
#define TImportaEstudoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaEstudo.h"

//---------------------------------------------------------------------------
class VTParse;

//---------------------------------------------------------------------------
class TImportaEstudo : public VTImportaEstudo
   {
   public:
           __fastcall  TImportaEstudo(VTApl *apl_owner);
           __fastcall ~TImportaEstudo(void);
      bool __fastcall  Executa(AnsiString arq_txt);

   private: //métodos

   private: //objetos externos
      VTParse  *parse;

   private: //dados locais
      VTApl    *apl;
   };

#endif
//---------------------------------------------------------------------------
//eof

