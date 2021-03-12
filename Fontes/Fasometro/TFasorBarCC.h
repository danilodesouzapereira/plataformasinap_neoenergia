//---------------------------------------------------------------------------
#ifndef TFasorBarCCH
#define TFasorBarCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorBarCC : public VTFasor
   {
   public:
           __fastcall TFasorBarCC(VTBarCC *barCC, TColor color, int atr, int ind_bar, bool visible);
           __fastcall ~TFasorBarCC(void);
      void __fastcall DefinePatamar(int ind_pat);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetTipoCurto(void);
      bool __fastcall PM_GetTipoFluxo(void);

   private: //métodos
      void       __fastcall IniciaFasorBarCC_Vff_pu(void);
      void       __fastcall IniciaFasorBarCC_Vfn_pu(void);
      AnsiString __fastcall NomeDefeito(int ind_def);

   private: //objetos externos
      VTBarCC *barCC;

   private: //dados locais
      int  ind_bar, ind_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
