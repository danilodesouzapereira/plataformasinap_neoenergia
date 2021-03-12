//---------------------------------------------------------------------------
#ifndef TImportaANEELH
#define TImportaANEELH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTImportaANEEL.h"

//---------------------------------------------------------------------------
class VTLog;

//---------------------------------------------------------------------------
class TImportaANEEL : public VTImportaANEEL
   {
   public:
           __fastcall  TImportaANEEL(VTApl *apl_owner);
			  __fastcall ~TImportaANEEL(void);
      bool __fastcall  ExecutaImportacao(AnsiString arq_txt);

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


