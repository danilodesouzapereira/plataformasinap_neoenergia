//---------------------------------------------------------------------------
#ifndef TMedicoesH
#define TMedicoesH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMedicoes.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArvore;
class VTMedicao;
class VTMedidor;
class VTCurva;
class TPontosMed;

//---------------------------------------------------------------------------
class TMedicoes : public VTMedicoes
   {
   public:
                 __fastcall  TMedicoes(VTApl *apl_owner);
                 __fastcall ~TMedicoes(void);
      void       __fastcall  AcertaBarraMedidores(void);
      void       __fastcall  AtualizaMedidores(void);
      void       __fastcall  Clear(void);
      void       __fastcall  ConverteMedidorPtrifasicoToPQfase(void);
      void       __fastcall  CorrenteToPQfase(void);
      void       __fastcall  CorrenteToPQtrifasico(void);
      bool       __fastcall  CriaMedicoesMedidores(void);
      VTMedicao* __fastcall  CriaMedicaoMedidor(VTMedidor *medidor);
      VTMedicao* __fastcall  ExisteMedicao(AnsiString cod_chave);
	  VTMedidor* __fastcall  ExisteMedidor(VTChave *chave);
	  bool       __fastcall  FluxoZerado(VTRede *rede);
      void       __fastcall  InsereDisjuntoresFicticios(bool clear_lisRedeVal = false);
      VTMedidor* __fastcall  InsereMedidor(VTChave *chave);
      TList*     __fastcall  LisMedicao(void);
      void       __fastcall  LisMedicaoComErroNoAjuste(TList *lisMED_ERRO);
      TList*     __fastcall  LisMedidor(void);
      TList*     __fastcall  LisMedidorAjustado(void);
	  TList*     __fastcall  LisMedicao(VTRede *rede);
	  TList*     __fastcall  LisMedicaoDJ(void);
	  TList*     __fastcall  LisMedicaoValida(bool med_neg_enabled);
      double     __fastcall  MaiorErro(VTCanal *canal);
      //void       __fastcall  PreencheLisMedidor(void);
      void       __fastcall  PreencheLisMedidor(bool clear_lisRedeVal = false);
      void       __fastcall  PreencheLisRedeVal(TList *lisREDE);
      void       __fastcall  RemoveChavesFic(void);
      bool       __fastcall  Valida(bool limite_ajuste, double ajuste_max);
      void       __fastcall  ValidaMedidores(bool limite_ajuste, double ajuste_max);

   private: //métodos
      void       __fastcall AlteraEqptosMedidor(VTMedidor *medidor);
      void       __fastcall ConverteMedI(TList *lisI,   VTMedicao *medicao);
      void       __fastcall ConverteMedPQ(TList *lisPQ, VTMedicao *medicao);
      void       __fastcall ConverteMedP(TList *lisPQ,  VTMedicao *medicao);
      void       __fastcall ConverteMedQ(TList *lisPQ,  VTMedicao *medicao);
      void       __fastcall ConverteMedS(TList *lisS,   VTMedicao *medicao);
      VTChave*   __fastcall CriaChaveFicticia(VTCanal *canal);
      VTCurva*   __fastcall CriaCurva(int num_ponto, int num_valores);
      void       __fastcall CriaMedicaoValida(bool limite_ajuste, double ajuste_max);
      void       __fastcall DefineCurvaEstimada(VTCanal *canal);
      VTChave*   __fastcall ExisteChave(VTCanal *canal);
      VTChave*   __fastcall ExisteChave(VTMedidor *medidor);
      VTChave*   __fastcall ExisteDisjuntor(VTPrimario *primario);
      VTNode*    __fastcall ExisteNode(VTRede *rede);
      VTNode*    __fastcall ExisteNode(VTArvore *arvore_bloco, VTBloco *bloco);
      bool       __fastcall ExisteVizinhaEquivalentada(VTPrimario *primario);
      bool       __fastcall ExisteVizinhaEquivalentada(VTRede *rede);
	  int        __fastcall ExtraiCampos(AnsiString linha);
      int        __fastcall IndBarraMontante(VTElo *elo, VTBloco *bloco_jus, VTBloco *bloco_mon);
      void       __fastcall PreencheLisPriVal(void);
      void       __fastcall PreencheLisRedeVal(void);
      bool       __fastcall ValidaMedicoes(double &i_faseA, double &i_faseB, double &i_faseC);
      void       __fastcall ValidaMedicoes(double ajuste_max);
      void       __fastcall ValidaMedidores(void);
      bool       __fastcall VerificaComentario(AnsiString linha);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      TPontosMed  *pontos_med;
	  TList       *lisMED_ALL, *lisMED_SEL, *lisMEDIDOR, *lisPRI_VAL, *lisREDE_VAL;
      TList       *lisMED_DJ; //medidores na saida dos primários
      TStringList *stringListArq; //Armazena o conteúdo do arquivo txt lido
      TStringList *campos;        //Armazena uma linha com os campos de uma linha
      VTArvore    *arvore, *arvore_bloco;
    };

//---------------------------------------------------------------------------
#endif
//eof

