//---------------------------------------------------------------------------
#ifndef TEstudoH
#define TEstudoH

//----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEstudo.h"

//----------------------------------------------------------------------------
//class

//----------------------------------------------------------------------------
class TEstudo : public VTEstudo
   {
   public:
                    __fastcall   TEstudo(VTApl *apl);
                     __fastcall ~TEstudo(void);
      VTAlternativa* __fastcall  CopiaAlternativa(VTAlternativa *alternativa);
      bool           __fastcall  DefineAlternativaAtiva(VTAlternativa *alternativa, VTObra *obra);
      void           __fastcall  DefineObrasComoFixas(void);
      VTAlternativa* __fastcall  ExisteAlternativa(AnsiString codigo);
      VTAlternativa* __fastcall  ExisteAlternativa(int id);
      VTAlternativa* __fastcall  InsereAlternativa(AnsiString codigo, bool padrao);
      TList*         __fastcall  LisAlternativa(void);
      TList*         __fastcall  LisAlternativaExcluida(void);
      void           __fastcall  LisObraEstudo(TList *lisEXT);
      void           __fastcall  LisAcaoEstudo(TList *lisEXT);
	  void           __fastcall  LisEqptoEstudo(TList *lisEXT, VTRede *rede, int tipo);
	  void           __fastcall  MontaRedesObrasLidasDaBase(void);
	  void           __fastcall  PreparaParaSalvarNaBase(void);
	  void           __fastcall  RetiraAlternativa(VTAlternativa *alternativa);
      void           __fastcall  RetiraAlternativaInvalida(void);
      void           __fastcall  RetiraObraSemAcao(void);
      TStringList*   __fastcall  StrListRede(void);

   protected: //métodos acessados via property
      bool           __fastcall PM_GetAlterado(void);
      int            __fastcall PM_GetAnoInicial(void);
      int            __fastcall PM_GetAnoFinal(void);
      int            __fastcall PM_GetAnoObraInicial(void);
      int            __fastcall PM_GetAnoObraFinal(void);
	  bool           __fastcall PM_GetNovo(void);
      VTObra*        __fastcall PM_GetObraAtiva(void);
      VTAlternativa* __fastcall PM_GetAlternativaAtiva(void);
	  VTAlternativa* __fastcall PM_GetAlternativaPadrao(void);
	  int        	 __fastcall PM_GetModalResult(void);
	  void           __fastcall PM_SetAlterado(bool alterado);
//      void           __fastcall PM_SetAlternativaAtiva(VTAlternativa *alternativa);
	  void           __fastcall PM_SetAnoInicial(int ano_ini);
      void           __fastcall PM_SetAnoFinal(int ano_fim);
	  void           __fastcall PM_SetNovo(bool novo);
      void       	 __fastcall PM_SetModalResult(int modal_result);

   private: //métodos
      void __fastcall ExibeGraficoTopologia(void);
      bool __fastcall FundeAcoesDasObras(void);

   private: //dados externos
      VTApl *apl;

   private: //dados locais
      TList       *lisALT;
      TList       *lisLIXO;
      TStringList *strlistREDE;
   };

//---------------------------------------------------------------------------
#endif
//eof
