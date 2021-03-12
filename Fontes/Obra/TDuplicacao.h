//---------------------------------------------------------------------------
#ifndef TDuplicacaoH
#define TDuplicacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTAcao;
class VTApl;
class VTBarra;
class VTEqbar;
class VTEqpto;
class VTLigacao;
class VTRede;
class VTRedes;
class VTObra;

//---------------------------------------------------------------------------
class TDuplicacao
   {
   public:
              __fastcall  TDuplicacao(VTApl *apl);
              __fastcall ~TDuplicacao(void);
      VTAcao* __fastcall  Duplica(VTObra *obra, VTAcao *acao);

   private:  //métodos
      VTAcao* __fastcall  Duplica(VTAcao *acao);
      //AcaoAltera
      VTAcao* __fastcall DuplicaAcaoAltera(VTAcao *acao);
      VTAcao* __fastcall DuplicaAcaoAlteraBarra(VTAcao *acao);
      VTAcao* __fastcall DuplicaAcaoAlteraEqbar(VTAcao *acao);
      VTAcao* __fastcall DuplicaAcaoAlteraLigacao(VTAcao *acao);
      VTAcao* __fastcall DuplicaAcaoAlteraMutua(VTAcao *acao);
      VTAcao* __fastcall DuplicaAcaoAlteraRede(VTAcao *acao);
      //AcaoComposta
      VTAcao* __fastcall  DuplicaAcaoComposta(VTAcao *acao_composta);
      //AcaoInsere
      VTAcao*  __fastcall DuplicaAcaoInsere(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoInsereBarra(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoInsereEqbar(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoInsereLigacao(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoInsereMutua(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoInsereRede(VTAcao *acao);
      //AcaoRetira
      VTAcao*  __fastcall DuplicaAcaoRetira(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoRetiraBarra(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoRetiraEqbar(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoRetiraLigacao(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoRetiraRede(VTAcao *acao);
      VTAcao*  __fastcall DuplicaAcaoRetiraMutua(VTAcao *acao);
      //
      VTBarra*   __fastcall ExisteBarra(VTBarra *barra);
      VTEqbar*   __fastcall ExisteEqbar(VTEqbar *eqbar);
      VTEqpto*   __fastcall ExisteCloneDeEqptoInserido(VTEqpto *eqpto);
      VTLigacao* __fastcall ExisteLigacao(VTLigacao *ligacao);
      VTRede*    __fastcall ExisteRede(VTRede *rede);

   private:  //dados externos
      VTApl   *apl;
      VTObra  *obra;
      VTRedes *redes;

   private:  //dados
      TList *lisCLONE;  //lisCLONE é uma lista que mantém todos os Eqptos clonados
                        //que são criados na duplicação das Acoes de inserção de Eqpto.
                        //Cada Eqpto clonado aponta para seu Eqpto original através do
                        //ponteiro eqpto->Obj
   };

#endif
//---------------------------------------------------------------------------
//eof
