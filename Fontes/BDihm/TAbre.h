//---------------------------------------------------------------------------
#ifndef TAbreH
#define TAbreH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAbre.h"

//---------------------------------------------------------------------------
class VTBD;
class VTEsquematico;
class VTGrafico;
class VTSelecao;
class VTProgresso;

//---------------------------------------------------------------------------
class TAbre : public VTAbre
   {
   public:
             __fastcall  TAbre(TComponent *Owner, VTApl *apl_owner);
             __fastcall ~TAbre(void);
	  bool   __fastcall  CarregaEstudoExistente(VTEstudo *estudo);
	  bool   __fastcall  CarregaRedeAdicional(TList *lisREDE_PRE);
      bool   __fastcall  CarregaRedeCompleta(void);
      bool   __fastcall  CarregaRedeSelecionada(VTEstudo *estudo_alvo=NULL);
	  bool   __fastcall  CriaEstudoComRedeSelecionada(int tipo_estudo);
	  bool   __fastcall  IniciaConexao(TList *lisREDE_PRE=NULL);
      void   __fastcall  SelecaoClear(void);
      bool   __fastcall  SelecaoExisteRede(VTRede *rede);
      TList* __fastcall  SelecaoInsereRede(VTRede *rede);
      TList* __fastcall  SelecaoLisRede(void);
      int    __fastcall  SelecaoNumeroRede(void);
	  TList* __fastcall  SelecaoRetiraRede(VTRede *rede);
	  bool   __fastcall  ValidaBaseRede(void);

   private:  //métodos acessados via property
      VTApl* __fastcall PM_GetApl(void);
      bool   __fastcall PM_GetProgresso(void);
      void   __fastcall PM_SetProgresso(bool enabled);

   private: //métodos
	  bool __fastcall ConverteBase(void);
	  void __fastcall IniciaRedesBD(void);
	  void __fastcall IniciaObjetosBasicos(void);

   private: //dados locais
      TList         *lisREDE_SEL; //lista de Redes selecionadas
      VTApl         *apl;
      VTBD          *bd;
      VTEsquematico *esquematico;
      VTGrafico     *grafico;
      VTSelecao     *selecao;
      VTProgresso   *progresso;
   };

//---------------------------------------------------------------------------
#endif
//eof

