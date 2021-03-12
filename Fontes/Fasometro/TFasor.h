//---------------------------------------------------------------------------
#ifndef TFasorH
#define TFasorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasor.h"

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class TFasor : public VTFasor
   {
   public:
           __fastcall TFasor(VTEqpto *eqpto, TColor color, int atr, int ind_bar, bool visible);
           __fastcall ~TFasor(void);
      void __fastcall DefinePatamar(int ind_pat);

   protected:  //métodos acessados via property
      bool __fastcall PM_GetTipoCurto(void);
      bool __fastcall PM_GetTipoFluxo(void);

   private: //métodos
      void __fastcall IniciaFasorBarra(void);
      void __fastcall IniciaFasorBarraVff_kv(void);
      void __fastcall IniciaFasorBarraVfn_kv(void);
      void __fastcall IniciaFasorLigacao(void);
      void __fastcall IniciaFasorLigacaoI(void);
      void __fastcall IniciaFasorLigacaoS(void);

   private: //objetos externos
      VTEqpto *eqpto;

   private: //dados locais
      bool valor_fase;
      int  ind_bar, ind_pat;
   };

//---------------------------------------------------------------------------
#endif
//eof
