//---------------------------------------------------------------------------
#ifndef TPlanejamentoH
#define TPlanejamentoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTPlanejamento.h"

//---------------------------------------------------------------------------
class TPlanejamento : public VTPlanejamento
   {
   public:
                __fastcall  TPlanejamento(VTApl *apl);
                __fastcall ~TPlanejamento(void);
      VTEstudo* __fastcall  ExisteEstudo(AnsiString codigo);
      VTEstudo* __fastcall  ExisteEstudo(int id);
      bool      __fastcall  ExisteEstudoAlterado(void);
      bool      __fastcall  ExisteEstudoNovo(void);
      void      __fastcall  InsereActionAnalise(TAction *ActionExterna);
      void      __fastcall  InsereActionRelatorio(TAction *ActionExterna);
      VTEstudo* __fastcall  InsereEstudo(AnsiString codigo, int tipo, int estudo_id);
      VTEstudo* __fastcall  InsereEstudoRedeOriginal(void);
      TList*    __fastcall  LisActionAnalise(void);
      TList*    __fastcall  LisActionRelatorio(void);
      TList*    __fastcall  LisEstudo(void);
      void      __fastcall  Reinicia(VTEstudo *estudo);
      void      __fastcall  RetiraActionAnalise(TAction *ActionExterna);
      void      __fastcall  RetiraActionRelatorio(TAction *ActionExterna);
      bool      __fastcall  RetiraEstudo(VTEstudo *estudo);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TList *lisACTION_ANALISE;
      TList *lisACTION_RELATORIO;
      TList *lisESTUDO;
   };

#endif
//---------------------------------------------------------------------------
//eof

