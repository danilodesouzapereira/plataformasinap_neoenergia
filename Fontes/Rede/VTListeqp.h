//---------------------------------------------------------------------------
#ifndef VTListeqpH
#define VTListeqpH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTEqpto;

//---------------------------------------------------------------------------
class VTListeqp : public TList
   {
   public:  //property
      __property bool BuscaBinariaEnabled = {read=PD.busca_binaria, write=PD.busca_binaria};

   public:
                       __fastcall  VTListeqp(void) {};
      virtual          __fastcall ~VTListeqp(void) {};
      virtual int      __fastcall  Add(VTEqpto *eqpto) = 0;
      virtual VTEqpto* __fastcall  ExisteEqpto(int eqpto_id) = 0;
      virtual bool     __fastcall  Ordena(void) = 0;
      
protected:
   struct{//dados acessados via property
         bool busca_binaria;
         }PD;
   };

//---------------------------------------------------------------------------
//função para criar objeto VTListeqp
VTListeqp *NewObjListeqp(void);

#endif
//---------------------------------------------------------------------------
//eof
