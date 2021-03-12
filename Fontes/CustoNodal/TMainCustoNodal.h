//---------------------------------------------------------------------------
#ifndef TMainCustoNodalH
#define TMainCustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMainCustoNodal.h"

//---------------------------------------------------------------------------
class VTOps;
class VTModifica;

//---------------------------------------------------------------------------
class TMainCustoNodal : public VTMainCustoNodal
   {
   public:
            __fastcall TMainCustoNodal(VTApl *apl);
            __fastcall ~TMainCustoNodal(void);
      bool __fastcall Executa(void);

   private: //métodos
      bool  __fastcall CalculaCoeficienteCustoNodal(void);
      bool  __fastcall CalculaCustoNodal(void);
      bool  __fastcall CalculaFluxoPotencia(void);
      bool  __fastcall IniciaLisCustoEqpto(void);
      bool  __fastcall IniciaLisCustoNodal(void);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisEQP;
      TList *lisLIG;
   };

#endif
//---------------------------------------------------------------------------
//eof

