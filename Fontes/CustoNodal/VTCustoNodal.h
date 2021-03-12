//---------------------------------------------------------------------------
#ifndef VTCustoNodalH
#define VTCustoNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;
class VTCustoEqpto;

//---------------------------------------------------------------------------
class VTCustoNodal : public TObject
   {
   protected:  //m�todos acessados via property
      virtual double __fastcall PM_GetValor(int ind_pat) = 0;

   public:  //property
      __property double Valor[int ind_pat] = {read=PM_GetValor};

   public:  //m�todos
                     __fastcall VTCustoNodal(void) {};
      virtual        __fastcall ~VTCustoNodal(void) {};
      virtual bool   __fastcall InsereParcela(double coef, VTCustoEqpto *custo_eqpto) = 0;
      virtual TList* __fastcall LisParcela(void) = 0;
      
   public:  //dados
      VTEqpto *eqpto;
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTCustoNodal
//---------------------------------------------------------------------------
VTCustoNodal* __fastcall NewObjCustoNodal(void);

//---------------------------------------------------------------------------
#endif
//eof
