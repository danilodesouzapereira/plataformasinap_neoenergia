//---------------------------------------------------------------------------
#ifndef TFasorLigCCH
#define TFasorLigCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class TFasorLigCC : public VTFasor
   {
   public:
           __fastcall TFasorLigCC(VTLigCC *ligCC, TColor color, int atr, int ind_bar, bool visible);
           __fastcall ~TFasorLigCC(void);
      void __fastcall DefinePatamar(int ind_pat);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetTipoCurto(void);
      bool __fastcall PM_GetTipoFluxo(void);

   private: //métodos
      void       __fastcall IniciaFasorLigCC_I(void);
      AnsiString __fastcall NomeDefeito(int ind_def);

   private: //objetos externos
      VTLigCC *ligCC;

   private: //dados locais
      int  ind_bar, ind_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
