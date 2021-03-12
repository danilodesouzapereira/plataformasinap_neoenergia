#ifndef VTGeralH
#define VTGeralH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTPadraoRede;

//---------------------------------------------------------------------------
#define CONFIAB_MAX_EQPTO 4
enum FORMATO {fmtCIRCULO, fmtQUADRADO, fmtSIGLA, fmtICONE, fmtTIPCHV};

//---------------------------------------------------------------------------
//constantes p/ AG
enum eAG_VISU_GRAF  {agGRAF_NONE=0, agGRAF_TOPO=1, agGRAF_FLOW=2};
enum eAG_VISU_INDIV {agINDIV_ALL=0, agINDIV_ALFA=1};
enum eAG_SELECAO    {agROLETA=0, agTORNEIO=1, agRANKING=2};

//---------------------------------------------------------------------------
struct strAG   {//estrutura de dados para o AG
               int      nger;//número de gerações
               int      nindiv;//número de indivíduos
               double   probMut;//probabilidade de mutação AG básico
               double   probCruz;//probabilidade de cruzamento AG básico
               bool     cruzUniforme;//informa se o cruzamento é uniforme
               double   probCruzUnif;//probabilidade de cruzamento uniforme
               int      tiposelecao;//tipo de selecao 0: proporcional, 1: torneio, 2: ranking-hk0704
               double   perc_ind;//% dos indiv.da  geracao que participam da subpopulacao(torneio ou ranking)-hk0704
               double   etaRank;//eta que define tipo de seleção Ranking ( > 1 é uniforme)
               bool     critParada;//informa se há critério de parada
               int      ngerParada;//número de gerações para a parada
               double   kcustoTotal;//fator k para custos de investimento e perdas
               double   kinvest;//fator k para custos de investimento e perdas
               double   ktensao;//fator k para nota de tensão
               double   kcarreg;//fator k para nota de carregamento
               int      tipoComb;//tipo de combinação entre os Ks, =0 produto, =1 e-fuzzy
               double   gama;//soma ponderada, =0: op.media =1: op.min
               double   kdiagBar;//ponderação entre o numero de barras com tensao regular e ruim
                                 //knbar = 0.0: barras tensao ruim
                                 //      = 1.0: barras tensão regular
               bool     recondAuto;//decisão para recondutoramento automático
               struct   {int tipoGrafico; int npatExibe; int exibeIndiv;} visu[MAX_PAT];
               //ver enum eAG_VISU_GRAF e eAG_VISU_INDIV
               };

struct strDIAG  {
                int NOTA[2], NOTA_V[3], NOTA_I[3];
                int PESO_V, PESO_I;
                };
struct strPAT  {
               int    NUM_PAT;
               int    HORA_INI[MAX_PAT], HORA_FIM[MAX_PAT];
               int    HORA_DEM[MAX_PAT], DURA_PAT[MAX_PAT];
               bool   PONTA[MAX_PAT];
               AnsiString NOME[MAX_PAT];
               };
struct strDEMAN {
                 int NUM_GRUPO, NUM_ENSAIO;
                 };
struct strCURTO {
                double R_DEF,   X_DEF;
				double P_TRIF,  Q_TRIF;
                double P_FASET, Q_FASET;
				};
struct strFLOW  {
				int    ITERMAX;
                double TOLER, POTCON, CORCON, IMPCON;
				double I[2], S[2];
//				double V[redeCOUNT][4]; // FKM depende to enum redeSEGMENTO (VTTipoRede)
				double V[4];
				};
//FKM estrutura para guardar os valores limites das tensoes
struct strLIMTENSAO
				{
				//LIM[nivel][ponto], nivel:0.BT,1.MT,2.AT;0.inicio,1.fim
				double LIM_KV[3][2];
				//booleanos para saber se o limites sao abertos ou fechados
				bool   LIM_ISABERTO[3][2];
				};
struct strLOCAP {
                int    NATFPOT;
                double VMIN, VMAX, FPOT, IMAX;
                };
struct strCONFIAB{
                 AnsiString eqpto;
                 double     taxa_falha, tempo_rep, taxa_man, tempo_man;
                 int        direcao;
                 };
struct strREGU {
               int    NUM_MAX;
               double SNOM, VNOM, FAIXA, PASSO, VMIN, VREG[MAX_PAT];
               };
struct strCOR   {
                TColor Faixa[MAX_FAIXA+1];
                TColor Circ[MAX_COR];
                };
//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTGrid;
class VTInfoset;
class VTCfgFluxoH3;
class VTCfgPerdaReg;
class VTCfgPerdaTec;

//---------------------------------------------------------------------------
class VTGeral : public TObject
   {
   public:  //property
      __property VTInfoset*     Infoset     = {read=PD.infoset};
      __property bool           Iniciado    = {read=PD.iniciado,  write=PD.iniciado};
      __property int            BaseDado    = {read=PD.base_dado, write=PD.base_dado};
      __property VTCfgFluxoH3*  CfgFluxoH3  = {read=PD.cfg_fluxoH3};
      __property VTCfgPerdaTec* CfgPerdaTec = {read=PD.cfg_perda_tec};

   public:
                            __fastcall  VTGeral(void) {};
      virtual               __fastcall ~VTGeral(void) {};
      virtual VTPadraoRede* __fastcall  ExistePadraoRede(int padraorede_id) = 0;
      virtual void          __fastcall  CopiaAtributosDe(VTGeral &geral, bool somenteCfgFlow = false) = 0;
	  virtual bool          __fastcall  InserePadraoRede(VTPadraoRede* padrao) = 0;
	  virtual bool          __fastcall  LeGeral(bool redeEmpresa) = 0;
	  virtual bool			__fastcall 	LeLimitesTensao(void)=0;
	  virtual TList*        __fastcall  LisPadraoRede(void) = 0;
	  virtual bool          __fastcall  PadroesRedesDiferentes(VTGeral &geral)=0;
      virtual void          __fastcall  ReiniciaValoresDefault(void) = 0;
	  virtual bool          __fastcall  RetiraPadraoRede(VTPadraoRede* padrao=NULL) = 0;
	  virtual bool			__fastcall	SalvaLimitesTensao(void)=0;
	  virtual bool          __fastcall  SalvaGeral(bool redeEmpresa = false) = 0;
	  virtual int			__fastcall  TipoSegmentoNivelTensao(VTBarra *barra) = 0;


   public: //dados
      strAG      AG;
      strDIAG    DIAG;
      strPAT     PAT;
      strDEMAN   DEMAN;
      strCURTO   CURTO;
      strFLOW    FLOW;
      strLOCAP   LOCAP;
      strREGU    REGU;
      strCONFIAB CONFIAB[CONFIAB_MAX_EQPTO];
	  strCOR     COR;
	  strLIMTENSAO LIM_V;
      //
      VTGrid *grid;
      //dados acessados via property
      struct   {
               bool          iniciado;
               int           base_dado;
               VTInfoset     *infoset;
               VTCfgFluxoH3  *cfg_fluxoH3;
               VTCfgPerdaTec *cfg_perda_tec;
			   } PD;
	  #define ARQ_CFG_LIMITE_TENSAO           "CfgLimV"
   };

//---------------------------------------------------------------------------
//protótipo de função para criar objetos VTGeral
//---------------------------------------------------------------------------
VTGeral* __fastcall NewObjGeral(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
