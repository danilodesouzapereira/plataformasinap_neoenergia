//---------------------------------------------------------------------------
#ifndef TDensidadeH
#define TDensidadeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTDensidade.h"

//---------------------------------------------------------------------------
class TPtocarga;
class VTArvore;
class VTClasses;
class VTBarra;
class VTCarga;
class VTRede;
class VTCurva;
class VTDemanda;
class VTPatamares;

//---------------------------------------------------------------------------
class TDensidade : public VTDensidade
   {
   public:
             __fastcall  TDensidade(VTApl *apl_owner);
             __fastcall ~TDensidade(void);
      bool   __fastcall  Executa(TDateTime date);

   protected:  //métodos acessados via property
      //VTArvore* __fastcall PM_GetArvore(void);

   private: //métodos
      void       __fastcall CopiaCargasPtocarga(TList *lisCAR);
      VTCarga*   __fastcall CriaCarga(VTBarra *barra, int classe_tag, AnsiString codigo);
      VTCurva*   __fastcall CriaCurvaPQ(void);
      bool       __fastcall CriaLisPtocarga(VTRede *rede);
      TPtocarga* __fastcall ExistePtocarga(VTRede *rede, VTBarra *barra);
      TPtocarga* __fastcall InserePtocarga(VTRede *rede, VTBarra *barra, AnsiString codigo);
      bool       __fastcall ReiniciaArvoreComResumoCarga(void);
      void       __fastcall SomaDemandaCarga1NaCarga2(VTCarga *carga1, VTCarga *carga2);

   private: //objetos externos
      VTApl       *apl;
      VTClasses   *classes;
      VTDemanda   *demanda;
      VTPatamares *patamares;

   private: //dados locais
      VTArvore *arvore;
      TList    *lisEQP;
      TList    *lisPTC;
      TList    *lisNODE;
      TList    *lisRESUMO;
   };

//---------------------------------------------------------------------------
#endif
//eof

