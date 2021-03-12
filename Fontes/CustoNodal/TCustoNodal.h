//---------------------------------------------------------------------------
#ifndef TCustoNodalH
#define TCustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCustoNodal.h"

//---------------------------------------------------------------------------
class TCustoNodal : public VTCustoNodal
   {
   public:  //funções públicas
             __fastcall TCustoNodal(void);
             __fastcall ~TCustoNodal(void);
      bool   __fastcall InsereParcela(double coef, VTCustoEqpto *custo_eqpto);
      TList* __fastcall LisParcela(void);

   protected:  //métodos acessados via property
      double __fastcall PM_GetValor(int ind_pat);

   private:  //dados locais
      TList *lisPARCELA;
   };

//---------------------------------------------------------------------------
#endif
//eof
