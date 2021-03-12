//---------------------------------------------------------------------------
#ifndef TAnaTecH
#define TAnaTecH

//----------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAnaTec.h"

//----------------------------------------------------------------------------
class VTAlternativa;
class VTArvore;
class VTCorte;
class VTItemObra;
class VTObra;
class VTConsulta;
class VTNode;
class VTResumoTec;
class VTRede;
class VTTrafo;
class VTTrafo3E;
class VTProgresso;
class VTCalcIndCont;

//----------------------------------------------------------------------------
class TAnaTec : public VTAnaTec
   {
   public:
             __fastcall  TAnaTec(VTApl *apl_owner);
             __fastcall ~TAnaTec(void);
      bool   __fastcall  Executa(void);
      bool   __fastcall  Executa(VTAlternativa *alternativa, int ano);

   private:
      struct strLKC  {
                     double Esup_mwh;
                     double Eext_mwh;
                     double Eint_mwh;
                     };

   private: //métodos
      bool 		   __fastcall AlimentadorPai(VTRede *rede_sub, VTRede *rede_filha);
	  void 	       __fastcall Cabecalho(void);
	  double       __fastcall CalculaCompensacao(void);
      double       __fastcall CalculaEnergia_mwh(double p_mw, int num_pat);
	  double       __fastcall CalculaEnergiaRecebida(VTRede *rede);
      AnsiString   __fastcall CodigoGIS(VTRede *rede);
	  VTResumoTec* __fastcall DeterminaResumo(VTRede *rede, VTResumoTec *resumo_pai);
	  VTRede*   __fastcall DeterminaSubestacao(VTNode *node_pai, VTRede *rede_filha);
	  bool      __fastcall Executa(VTAlternativa *alternativa);
	  bool      __fastcall ExecutaContinuidade(VTAlternativa *alternativa, int ano);
	  bool      __fastcall ExecutaFluxoPotencia(int ano);
	  bool      __fastcall ExisteGD(void);
      bool      __fastcall FluxoEnergiaDaRedeNoCorte_mwh(VTRede *rede, VTCorte *corte, strLKC &lkc);
	  bool 		__fastcall ImprimeDEC_FEC_END(VTAlternativa *alternativa, int ano);
	  void      __fastcall IniciaAlternativas(void);
//	  void      __fastcall IniciaLisEqptoInseridoSubstituido(TList *lisACAO);
	  void      __fastcall IniciaLisEqptoInseridoSubstituido(VTItemObra *itemObra, TList *lisACAO);
	  void      __fastcall IniciaLisRedeInserida(TList *lisACAO);
	  void      __fastcall IniciaResumoTec(VTAlternativa *alternativa, int ano, bool fluxo_ok);
	  void      __fastcall InsereGD(void);
      void      __fastcall LisLigacao(TList *lisLIG, VTBarra *barra, TList *lisEQP);
	  bool      __fastcall PossuiuReticulado(void);
	  void      __fastcall RemoveGD(void);
	  void      __fastcall SalvaContinuidade(VTResumoTec *resumo, int ano);
	  void      __fastcall SalvaContinuidade(VTResumoTec *resumo, int ano, VTRede *rede);

	  void      __fastcall SalvaDiagTecnico(VTResumoTec *resumo, int ano);
	  void 		__fastcall SalvaDiagTecnicoBarra(VTResumoTec *resumo, int ano, TList *lisBarra);
	  void 		__fastcall SalvaDiagTecnicoCarga(VTResumoTec *resumo, int ano, TList *lisCarga);
	  void 		__fastcall SalvaDiagTecnicoTrafo(VTResumoTec *resumo, int ano, TList *lisTrafo);
	  void		__fastcall SalvaDiagTecnicoTrecho(VTResumoTec *resumo, int ano, TList *lisTrecho);

	  void      __fastcall SalvaEnergiaEntrada(VTResumoTec *resumo, int ano);
	  void      __fastcall SalvaEnergiaEntrada(VTResumoTec *resumo, int ano, TList *lisSuprimentos);

	  void      __fastcall SalvaEnergiaEntregue(VTResumoTec *resumo, int ano);
	  void      __fastcall SalvaEnergiaEntregue(VTResumoTec *resumo, int ano, TList *lisCargas);

	  void      __fastcall SalvaEnergiaGerada(VTResumoTec *resumo, int ano);
	  void      __fastcall SalvaEnergiaGerada(VTResumoTec *resumo, int ano, TList *lisGeradores);

	  void      __fastcall SalvaPerdaTecnica(VTResumoTec *resumo, int ano);
      void      __fastcall SalvaPerdaTecnica(VTResumoTec *resumo, int ano, TList *lisLigacao);

   private: //dados externos
      VTApl         *apl;

   private: //dados locais
      #define   DIAS_MES 30

      int           ano_ini, ano_fim;
	  TList         *lisEQP, *lisREDE;
	  TList         *lisEQP_INS, *lisEQP_SUB;
	  TList         *lisOBRA;
	  VTConsulta    *consulta;
	  VTCalcIndCont *calc_ind_cont;
	  VTProgresso   *progresso;
	  //adicionados para impressão de índices de continuidade
	  TStringList	*slContinuidade, *campos;
	  AnsiString	arq_rede;
      //arvore com hierarquia de redes
	  VTArvore      *arvore;
   };

//---------------------------------------------------------------------------
#endif
//eof
