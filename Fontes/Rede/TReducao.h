//---------------------------------------------------------------------------
#ifndef TReducaoH
#define TReducaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "VTReducao.h"

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class TReducao : public VTReducao
   {
   public:
                 __fastcall  TReducao(int num_pat);
                 __fastcall ~TReducao(void);
      VTBarra*   __fastcall  BarraFicticia(VTLigacao *ligacao);
      VTReducao* __fastcall  Clone(void);
      void       __fastcall  CopiaAtributosDe(VTEqpto &ref);
      bool       __fastcall  ExisteEqbarLigacaoConectaBarra(VTBarra *barra);
      bool       __fastcall  ExisteGeracao(void);
      void       __fastcall  InsereEqbar(VTEqbar *eqbar);
      void       __fastcall  InsereLigacao(VTLigacao *ligacao);
      void       __fastcall  IniciaEsquematico(int num_pat);
      void       __fastcall  IniciaInterconexao(int num_pat);
      TList*     __fastcall  LisBarra(void);
      void       __fastcall  LisBarra(TList *lisEQP);
      TList*     __fastcall  LisBarraEsquematico(void);
      TList*     __fastcall  LisEqbar(void);
      void       __fastcall  LisEqbar(TList *lisEQP);
      void       __fastcall  LisEqbar(TList *lisEQP, int tipo);
      void       __fastcall  LisEqpto(TList *lisEQP);
      void       __fastcall  LisEqpto(TList *lisEQP, int tipo);
      TList*     __fastcall  LisLigacao(void);
      void       __fastcall  LisLigacao(TList *lisEQP);
      void       __fastcall  LisLigacao(TList *lisEQP, int tipo);
      TList*     __fastcall  LisLigacaoEsquematico(void);
      int        __fastcall  NumeroDeEqbar(int tipo);
      void       __fastcall  Reinicia(void);
	  void       __fastcall  RemoveLigDaBarra(TList *lisLIG);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);
      bool       __fastcall  Visible(void);

   protected: //redefinição de método virtual de VTEpqto
      void  __fastcall PM_SetShow(unsigned bit, bool enabled);

   protected: //métodos acessados via property
      bool           __fastcall PM_GetCarregada(void);
      VTCorte*       __fastcall PM_GetCorte(void);
      VTCrescimento* __fastcall PM_GetCrescimento(void);
      VTMRede*       __fastcall PM_GetMRede(void);
      int            __fastcall PM_GetNumeroEqpto(void);
      bool           __fastcall PM_GetValida(void);
      void           __fastcall PM_SetCorte(VTCorte *corte);
      void           __fastcall PM_SetCrescimento(VTCrescimento *crescimento);
      void           __fastcall PM_SetMRede(VTMRede *mrede);
      void           __fastcall PM_SetValida(bool valida);

	private: //métodos
		VTRede* __fastcall FirstRede(void);

	private: //externos
		VTMRede *mrede;  //ponteiro p/ objeto VMRede
      VTCorte *corte;  //ponteiro p/ objeto VTCorte ao qual a Redução se aplica

   private: //dados acessados via property
      VTCrescimento *crescimento;

   private: //locais
      //bool  nula;     //não pode ser montada (Rede não conecta o Corte)
      bool  valida;     //indica se a redução da MRede pode ser feita no Corte
      TList *lisBAR;    //lista de objetos VTBarra da Rede
      TList *lisEQB;    //lista de objetos VTEqbar da Reducao
      TList *lisLIG;    //lista de objetos VTLigacao da Reducao
      //estrutura p/ representação esquemática da rede reduzida
      strESQ_REDE  esq;
   };

//---------------------------------------------------------------------------
#endif
//eof

