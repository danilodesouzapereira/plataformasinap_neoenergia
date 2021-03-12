//---------------------------------------------------------------------------
#ifndef VTRedePTH
#define VTRedePTH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTCarga;
class VTLigacao;
class VTRede;

//---------------------------------------------------------------------------
class VTRedePT : public TObject
   {
   public:  //property
      __property VTRede* Rede = {read=PD.rede,  write=PD.rede};

   public:
                       __fastcall  VTRedePT(void) {};
      virtual          __fastcall ~VTRedePT(void) {};
      virtual void     __fastcall  InsereBarra(VTBarra *barra) = 0;
      virtual void     __fastcall  InsereCargaMedidor(VTCarga *carga) = 0;
      virtual void     __fastcall  InsereLigacaoRamal(VTLigacao *ligacao) = 0;
      virtual TList*   __fastcall  LisBarra(void) = 0;
      virtual TList*   __fastcall  LisCargaMedidor(void) = 0;
      virtual TList*   __fastcall  LisLigacaoRamal(void) = 0;

   protected:  //dados acessados via property
      struct{
            VTRede *rede;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedePT
//---------------------------------------------------------------------------
VTRedePT* __fastcall NewObjRedePT(void);

//---------------------------------------------------------------------------
#endif
//eof



 