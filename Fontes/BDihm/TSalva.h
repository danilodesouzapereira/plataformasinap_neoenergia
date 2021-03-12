//---------------------------------------------------------------------------
#ifndef TSalvaH
#define TSalvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSalva.h"

//---------------------------------------------------------------------------
class VTBD;
class VTDominio;
class VTRede;
class VTProgresso;

//---------------------------------------------------------------------------
class TSalva : public VTSalva
   {
   public:
             __fastcall  TSalva(TComponent *Owner, VTApl *apl_oner);
             __fastcall ~TSalva(void);
      bool   __fastcall  SalvaEstudo(VTEstudo *estudo);
      bool   __fastcall  SalvaEstudoComo(VTEstudo *estudo);
      bool   __fastcall  SalvaHistoricoDensidadeCarga(VTArvore *arvore);
      bool   __fastcall  SalvaHistoricoPerdaTecnica(VTArvore *arvore);
      bool   __fastcall  SalvaRede(void);
      bool   __fastcall  SalvaRedeComo(void);

   private:  //métodos acessados via property
      bool   __fastcall PM_GetProgresso(void);
      void   __fastcall PM_SetProgresso(bool enabled);

   private: //métodos
      void       __fastcall AvisoBaseVersaoInvalida(void);
      void       __fastcall BackupBaseDado(bool salva);
      VTDominio* __fastcall ExisteDominioRede(VTRede *rede);
      VTDominio* __fastcall ExisteDominioRedeVizinha(VTRede *rede);
      bool       __fastcall IniciaConexao(void);
      bool       __fastcall InsereDominio(void);
      bool       __fastcall LicencaSinapViewer(void);
      bool       __fastcall RetiraDominio(void);

   private: //dados locais
      TList       *lisEQP;
      VTApl       *apl;
      VTBD        *bd;
      VTProgresso *progresso;
      bool        reservado;
   };

//---------------------------------------------------------------------------
#endif
//eof

