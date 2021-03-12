//---------------------------------------------------------------------------
#ifndef VTSalvaH
#define VTSalvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTEstudo;
class VTBalancos;
class VTPerdas;
class VTRadial;
class VTArvore;

//---------------------------------------------------------------------------
class VTSalva : public TObject
   {
   public:  //property
      __property bool  Progresso = {read=PM_GetProgresso,   write=PM_SetProgresso  };

   public:
                     __fastcall  VTSalva(void) {};
                     __fastcall ~VTSalva(void) {};
      virtual bool   __fastcall  SalvaEstudo(VTEstudo *estudo) = 0;
      virtual bool   __fastcall  SalvaEstudoComo(VTEstudo *estudo) = 0;
      virtual bool   __fastcall  SalvaHistoricoDensidadeCarga(VTArvore *arvore) = 0;
      virtual bool   __fastcall  SalvaHistoricoPerdaTecnica(VTArvore *arvore) = 0;
      virtual bool   __fastcall  SalvaRede(void) = 0;
      virtual bool   __fastcall  SalvaRedeComo(void) = 0;

   protected:  //métodos acessados via property
      virtual bool   __fastcall PM_GetProgresso(void) = 0;
      virtual void   __fastcall PM_SetProgresso(bool enabled) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTSalva
//---------------------------------------------------------------------------
VTSalva* __fastcall NewObjSalva(TComponent *Owner, VTApl *apl_owner);

//---------------------------------------------------------------------------
#endif
//eof



 