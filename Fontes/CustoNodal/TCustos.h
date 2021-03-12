//---------------------------------------------------------------------------
#ifndef TCustosH
#define TCustosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCustos.h"

//---------------------------------------------------------------------------
class VTOps;
class VTModifica;

//---------------------------------------------------------------------------
class TCustos : public VTCustos
   {
   public:
                    __fastcall TCustos(VTApl *apl);
                    __fastcall ~TCustos(void);
      VTCustoEqpto* __fastcall ExisteCustoEqpto(VTEqpto *eqpto);
      VTCustoNodal* __fastcall ExisteCustoNodal(VTEqpto *eqpto);
      bool          __fastcall InsereCustoEqpto(VTCustoEqpto *custo_eqpto);
      bool          __fastcall InsereCustoNodal(VTCustoNodal *custo_nodal);
      TList*        __fastcall ListaCustoEqpto(void);
      TList*        __fastcall ListaCustoNodal(void);
      void          __fastcall Reinicia(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisCUSTO_EQP;  //lista de objetos CustoEqpto
      TList *lisCUSTO_NOD;  //lista de objetos CustoNodal
   };

#endif
//---------------------------------------------------------------------------
//eof

