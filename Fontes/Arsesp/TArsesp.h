//---------------------------------------------------------------------------
#ifndef TArsespH
#define TArsespH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTArsesp.h"

//---------------------------------------------------------------------------
class VTTronco;

//---------------------------------------------------------------------------
class TArsesp : public VTArsesp
   {
   public:
           __fastcall  TArsesp(VTApl *apl_owner);
           __fastcall ~TArsesp(void);
      bool __fastcall  Executa(VTPrimario *primario);
      bool __fastcall  Start(void);
      bool __fastcall  Stop(void);

   private: //métodos
      double __fastcall CalculaDemandaNoPatamar(int ind_pat, TList *lisCARGA);
      void   __fastcall IniciaResumo(void);
      bool   __fastcall SalvaDadosAjusteDemanda(void);
      bool   __fastcall SalvaDadosFluxo(void);
      bool   __fastcall SalvaDadosMedicao(void);
      bool   __fastcall SalvaDadosTopologia(void);
      void   __fastcall SelecionaPatamarMaiorMenorCarregamento(void);
      double __fastcall ValorReal(double valor);

   private: //objetos externos
      VTApl      *apl;
      VTPrimario *primario;

   private: //dados locais
      int         ind_pat_max, ind_pat_min;
      TList       *lisCARGA;
      VTTronco    *tronco;
   };

//---------------------------------------------------------------------------
#endif
//eof

