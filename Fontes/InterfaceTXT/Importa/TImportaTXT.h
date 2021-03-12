//---------------------------------------------------------------------------
#ifndef TImportaTXTH
#define TImportaTXTH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaTXT.h"

//---------------------------------------------------------------------------
class VTLog;

//---------------------------------------------------------------------------
class TImportaTXT : public VTImportaTXT
   {
   public:
           __fastcall  TImportaTXT(VTApl *apl_owner);
           __fastcall ~TImportaTXT(void);
      bool __fastcall  ExecutaImportacao(AnsiString arq_txt);
      bool __fastcall  ImportaArranjos(AnsiString arq_txt);
      bool __fastcall  ImportaConfiguracao(AnsiString arq_txt);
      bool __fastcall  ImportaCurvasTipicas(AnsiString arq_txt);
      bool __fastcall  ImportaRedes(AnsiString arq_txt);

   private: //métodos
      void  __fastcall IniciaLog(void);
      void  __fastcall IniciaObjAplLocal(void);

   private: //objetos exgternos
      VTApl   *apl_owner;

   private: //dados locais
      int      dxbar, dybar;
      TList    *lisEQP;
      VTApl   *apl;
      VTLog   *plog;
   };

#endif
//-----------------------------------------------------------------------------
// eof


