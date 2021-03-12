//---------------------------------------------------------------------------
#ifndef TSelecaoH
#define TSelecaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSelecao.h"

//---------------------------------------------------------------------------
class VTMRede;
class VTCorte;

//---------------------------------------------------------------------------
class TSelecao : public VTSelecao
   {
   public:
            __fastcall  TSelecao(VTApl *apl);
            __fastcall ~TSelecao(void);
	  bool   __fastcall  CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL, int aviso_num_rede);
	  bool   __fastcall  CarregaRedeAdicional(VTBD *bd, VTEstudo *estudo, TList *lisREDE_PRE, TList *lisREDE_SEL, int aviso_num_rede);
	  bool   __fastcall  CarregaEstudo(VTBD *bd, VTEstudo *estudo);
	  bool   __fastcall  CarregaRedeCompleta(VTBD *bd, VTEstudo *estudo, int aviso_num_rede);
     bool   __fastcall  CarregaRedeSelecionada(VTBD *bd, VTEstudo *estudo, TList *lisREDE_SEL, int aviso_num_rede);

   private: //métodos
   	void     __fastcall AlteraMRedeValida(TList *lisREDE);
      void     __fastcall DestroiMRedeNaoSelNaoViz(void);
	   VTCorte* __fastcall ExisteCorteComum(VTMRede *mrede, TList *lisMREDE);
	   void     __fastcall IniciaLisMRedePreCarregadas(TList *lisREDE_PRE=NULL);
      void     __fastcall IniciaLisMRedeSelecionada(TList *lisREDE_SEL);
	   void     __fastcall IniciaLisMRedeVizinha(void);
      void     __fastcall LeListaMRedeVizinha(VTMRede *mrede, TList *lisMR);
      void     __fastcall LeListaRedesExistentes(VTBD *bd, VTEstudo *estudo);
      void     __fastcall PreparaMRedeVizinha(void);
      void     __fastcall RetiraCorte(VTCorte *corte, TList *lisMREDE);

   private: //dados externos
      VTApl    *apl;
      VTBD     *bd;
      VTEstudo *estudo;

   private: //dados locais
      TList *lisEQP;
      //TList *lisCORTE;
      TList *lisMREDE_SEL;
      TList *lisMREDE_VIZ;
      TList *lisREDE_VIZ;
	  TList *lisMREDE_PRE;
	  TList *lisMREDE_REDUZIDAS;
   };

//---------------------------------------------------------------------------
#endif
//eof

