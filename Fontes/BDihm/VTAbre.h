//---------------------------------------------------------------------------
#ifndef VTAbreH
#define VTAbreH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTRede;

//---------------------------------------------------------------------------
class VTAbre : public TObject
   {
   public:  //property
      __property VTApl* apl          = {read=PM_GetApl};
      __property int    AvisoNumRede = {read=PD.aviso_num_rede, write=PD.aviso_num_rede};
      __property bool   Progresso    = {read=PM_GetProgresso,   write=PM_SetProgresso  };

   public:
                     __fastcall  VTAbre(void) {};
                     __fastcall ~VTAbre(void) {};
	  virtual bool   __fastcall  CarregaEstudoExistente(VTEstudo *estudo) = 0;
	  virtual bool   __fastcall  CarregaRedeAdicional(TList *lisREDE_PRE) = 0;
      virtual bool   __fastcall  CarregaRedeCompleta(void) = 0;
      virtual bool   __fastcall  CarregaRedeSelecionada(VTEstudo *estudo_alvo=NULL) = 0;
      virtual bool   __fastcall  CriaEstudoComRedeSelecionada(int tipo_estudo) = 0;
      virtual bool   __fastcall  IniciaConexao(TList *lisREDE_PRE=NULL) = 0;
      virtual void   __fastcall  SelecaoClear(void) = 0;
      virtual bool   __fastcall  SelecaoExisteRede(VTRede *rede) = 0;
      virtual TList* __fastcall  SelecaoInsereRede(VTRede *rede) = 0;
      virtual TList* __fastcall  SelecaoLisRede(void) = 0;
      virtual int    __fastcall  SelecaoNumeroRede(void) = 0;
	  virtual TList* __fastcall  SelecaoRetiraRede(VTRede *rede) = 0;
	  virtual bool   __fastcall  ValidaBaseRede(void) = 0;

   protected:  //métodos acessados via property
      virtual VTApl* __fastcall PM_GetApl(void) = 0;
      virtual bool   __fastcall PM_GetProgresso(void) = 0;
      virtual void   __fastcall PM_SetProgresso(bool enabled) = 0;

   private:  //dados acessados via property
      struct{
            int  aviso_num_rede;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTAbre
//---------------------------------------------------------------------------
VTAbre* __fastcall NewObjAbre(TComponent *Owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 