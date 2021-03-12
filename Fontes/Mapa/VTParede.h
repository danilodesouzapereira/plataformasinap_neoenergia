//---------------------------------------------------------------------------
#ifndef VTParedeH
#define VTParedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTRede;

//-----------------------------------------------------------------------------
class VTParede : public TObject
   {
   public:  //property
      __property VTRede* Rede = {read=PD.rede, write=PD.rede};

   public:
                     __fastcall  VTParede(void) {};
      virtual        __fastcall ~VTParede(void) {};
      virtual bool   __fastcall  ExisteBarra(VTBarra *barra) = 0;
      virtual bool   __fastcall  ExisteLigacao(VTLigacao *ligacao) = 0;
      virtual bool   __fastcall  InsereBarra(VTBarra *barra) = 0;
      virtual bool   __fastcall  InsereLigacao(VTLigacao *ligacao) = 0;
      virtual TList* __fastcall  LisBarra(void) = 0;
      virtual int    __fastcall  LisBarra(TList *lisEXT) = 0;
      virtual TList* __fastcall  LisLigacao(void) = 0;
      virtual int    __fastcall  LisEqbar(TList *lisEXT,   int tipo=-1) = 0;
      virtual int    __fastcall  LisEqpto(TList *lisEXT,   int tipo=-1) = 0;
      virtual int    __fastcall  LisLigacao(TList *lisEXT, int tipo=-1) = 0;
      virtual void   __fastcall  ReiniciaLisEqpto(void) = 0;

   protected:  //dados acessados via property
      struct   {
               VTRede* rede;
               }PD;
   };

//---------------------------------------------------------------------------
VTParede* __fastcall NewObjParede(void);

//---------------------------------------------------------------------------
#endif
//eof
