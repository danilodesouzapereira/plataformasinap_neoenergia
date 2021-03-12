//---------------------------------------------------------------------------
#ifndef VTBDhistH
#define VTBDhistH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Oracle.hpp>
#include "..\Rede\BD_Inc.h"

//---------------------------------------------------------------------------
class TPro_etd;
class TPro_pri;
class TPro_sec;
class VTApl;
class VTArsesp;
class VTArvore;
class VTBalancos;
class VTEmpresa;
class VTPerdas;
class VTRadial;
class VTRede;
class VTResumo;
class VTSNPDBConn;


//---------------------------------------------------------------------------
// Estruturas de dados auxiliares

struct strTOPO
   {
   int QtdeRede;
   int QtdeTrafo;
   int QtdeCabo;
   int QtdeCapacitor;
   int QtdeReator;
   int QtdeRegulador;
   int QtdeConsA4;
   int QtdeConsRes;
   int QtdeConsCom;
   int QtdeConsInd;
   int QtdeConsRur;
   int QtdeConsOut;

   // Iniciação dos campos
   strTOPO()
      {
      QtdeRede = QtdeTrafo = QtdeCabo = QtdeCapacitor = QtdeReator = QtdeRegulador = 0;
      QtdeConsA4 = QtdeConsRes = QtdeConsCom = QtdeConsInd = QtdeConsRur = QtdeConsOut = 0;
      }
   };

struct strARSESP
   {
   TDateTime     dttData;
   AnsiString    strRegional;
   AnsiString    strETD;
   AnsiString    strPrimario;
   AnsiString    strTipoCabo;
   AnsiString    strBitolaCabo;
   AnsiString    strTecnologia;
   double        fltTensaoNominal;
   double        fltCorrenteNominal;
   double        fltCorrenteEfetiva;
   double        fltComprimentoTotal;
   double        fltComprimentoTronco;
   double        fltQuedaTensaoMax;
   double        fltQuedaTensaoMin;
   double        fltCargtoMaximo;
   int           intNumConsPrimario;
   int           intNumConsSecundario;
   bool          bolValido;
   AnsiString    strModeloFluxo;
   double        fltImed_faseA;
   double        fltImed_faseB;
   double        fltImed_faseC;
   double        fltIref_faseA;
   double        fltIref_faseB;
   double        fltIref_faseC;
   double        fltIaju_faseA;
   double        fltIaju_faseB;
   double        fltIaju_faseC;
   double        fltTempo_mseg;
   AnsiString    strObservacao;

   // Iniciação dos campos
   strARSESP()
      {
      // OOPS_Perda: dttData fica inválido?
      // dttData
      bolValido = false;
      strRegional = strETD = strPrimario = strTipoCabo = strBitolaCabo = strTecnologia = "";
      strModeloFluxo = strObservacao = "";

      intNumConsPrimario = intNumConsSecundario = 0;

      fltTensaoNominal = fltCorrenteNominal = fltCorrenteEfetiva = fltComprimentoTotal = 0;
      fltComprimentoTronco = fltQuedaTensaoMax = fltQuedaTensaoMin = fltCargtoMaximo = 0;
      fltImed_faseA = fltImed_faseB = fltImed_faseC = fltIref_faseA = fltIref_faseB = fltIref_faseC = 0;
      fltIaju_faseA = fltIaju_faseB = fltIaju_faseC = fltTempo_mseg = 0;
      }
   };

// Estruturas do prodist (originalmente estavam em TPro_sec, TPro_etd e TPro_pri).
struct strPROPRI : public TObject
   {
   // Dados do SISPAI
   bool       stt_flow;         //indica se o flow foi calculado
   int        bd_id;
   AnsiString siglaSupridora;   //formato XXX-NN (onde XXX = SE)
   AnsiString sigla;            //formato XXX-NN (onde XXX = SE)
   AnsiString nome;             //
   double     comp;             //comprimento em km
   AnsiString cabo_tronco;      //código do cabo tronco
   AnsiString cabo_ramal;       //código do cabo ramal
   int        angulo;           //ângulo do alimentador em graus
   double     demanda_max;      //demanda máxima
   int        nbar_carga;       //número de pontos (barras de carga)
   double     fat_pot;          //fator de potência
   int        fat_carga;        //fator de carga (%)
   double     taxa_arbor;       //taxa de arborização (%)
   double     taxa_reforma;     //taxa de reforma
   double     taxa_cresc_vert;  //taxa de crescimento vertical (%)
   double     taxa_cresc_hori;  //taxa de crescimento horizontal (%)
   double     taxa_falha_arbor; //
   double     taxa_falha_eqpto; //
   double     taxa_falha_outro; //
   double     DEC;              //
   double     FEC;              //
   double     alfa;             //alfa
   int        nconsumidores;    //número de consumidores
   double     area_atuacao;     //área (m2) do setor circular definido pelo ângulo do alimentador
   double     dist_baricentro;  //distância (m) do baricentro de carga
   //<JCG> dados novos: 15/02/2007
   AnsiString fase;             //{trifásica/bifásica/monofásica/mista}
   AnsiString cabo_tronco_3f;   //Descrição da bitola do cabo utilizado no tronco trifásico
   AnsiString cabo_tronco_2f;   //Descrição da bitola do cabo utilizado no tronco bifásico
   AnsiString cabo_tronco_1f;   //Descrição da bitola do cabo utilizado no tronco monofásico
   AnsiString cabo_ramal_3f;    //Descrição da bitola do cabo utilizado no ramal trifásico
   AnsiString cabo_ramal_2f;    //Descrição da bitola do cabo utilizado no ramal bifásico
   AnsiString cabo_ramal_1f;    //Descrição da bitola do cabo utilizado no ramal monofásico
   double     comp_tronco_3f;   //comprimento do tronco trifásico, em km
   double     comp_tronco_2f;   //comprimento do tronco bifásico, em km
   double     comp_tronco_1f;   //comprimento do tronco monofásico, em km
   double     comp_ramal_3f;    //comprimento de ramal trifásico, em km
   double     comp_ramal_2f;    //comprimento de ramal bifásico, em km
   double     comp_ramal_1f;    //comprimento de ramal monofásico, em km
   int        num_consumidor_mt;//total de consumidores MT atendidos
   int        num_consumidor_bt;//total de consumidores BT atendidos
   double     fat_perda;        //Fator de perda  (%)
   int        num_reg_tensao;   //Quantidade de reguladores de tensão
   double     dv_maximo;        //tensão na saída da subestação menos a tensão no ponto de tensão mínima (kV)
   double     vnom;            //tensão nominal (kV)

   // Iniciação dos dados
   strPROPRI()
      {
      stt_flow = false;         //indica se o flow foi calculado
      bd_id = 0;

      siglaSupridora = sigla = nome = cabo_tronco = cabo_ramal = fase = cabo_tronco_3f = cabo_tronco_2f = "";
      cabo_tronco_1f = cabo_ramal_3f = cabo_ramal_2f = cabo_ramal_1f = "";

      comp = demanda_max = fat_pot = taxa_arbor = taxa_reforma = taxa_cresc_vert = taxa_cresc_hori = 0;
      taxa_falha_arbor = taxa_falha_eqpto = taxa_falha_outro = DEC = FEC = alfa = area_atuacao = 0;
      dist_baricentro = comp_tronco_3f = comp_tronco_2f = comp_tronco_1f = comp_ramal_3f = comp_ramal_2f = 0;
      comp_ramal_1f = fat_perda = dv_maximo = vnom = 0;

      angulo = nbar_carga = fat_carga = nconsumidores = num_consumidor_mt = num_consumidor_bt = num_reg_tensao = 0;
      }
   };

struct strPROSEC : public TObject
   {
   // Dados do SISPAI
   bool       stt_flow;               //indica se o flow foi calculado
   int        bd_id;
   AnsiString siglaSupridora;        //formato XXX-NN (onde XXX = SE)
   AnsiString sigla;                 //Código do Posto;
   double     snom;                  //Potência nominal do transformador (kVA);
   double     cargto;                //Carregamento do transformador (kVA);
   double     comprimento;           //Comprimento da rede secundária atendida (km);
   AnsiString cabo;                  //Descrição da bitola do cabo utilizado;
   double     fat_carga;             //Fator de carga (pu);
   double     fat_potencia;          //Fator de potência (pu);
   double     taxa_crescimento;      //Taxa de crescimento (% a.a.);
   AnsiString descricao;             //Descrição do Posto;
   AnsiString regional;              //Regional;
   double     vnom_at;               //Tensão (kV);
   double     vnom_bt;               //Tensão (V);
   AnsiString tipologia;             //Tipologia (M - Monofásico / T - Trifásico).
   //<JCG> dados novos: 15/02/2007
   AnsiString fase;                  //{trifásica/bifásica/monofásica/mista}
   AnsiString cod_trafo;             //
   double     fat_utilizacao;        //Fator de utilização (%)
   AnsiString cabo_tronco;           //Descrição da bitola do cabo utilizado no tronco
   AnsiString cabo_ramal;            //Descrição da bitola do cabo utilizado nos ramais
   double     comp_tronco;           //Comprimento do tronco da rede secundária atendida (km)
   double     comp_ramal;            //Comprimento dos ramais da secundária atendida (km)
   double     comp_vao_medio;        //Comprimento do vão médio (m)
   double     fat_carga_leve_pesada; //Relação entre carga leve e pesada (%)

   // Iniciação dos dados
   strPROSEC()
      {
      stt_flow = false;
      bd_id = 0;

      siglaSupridora = sigla = cabo = descricao = regional = "";
      tipologia = fase = cod_trafo = cabo_tronco = cabo_ramal = "";

      snom = cargto = comprimento = fat_carga = fat_potencia = taxa_crescimento = 0;
      vnom_at = vnom_bt = fat_utilizacao = comp_tronco = comp_ramal = comp_vao_medio = 0;
      fat_carga_leve_pesada = 0;
      }

   };

struct strPROETD : public TObject
   {
   bool           stt_flow;       //indica se o flow foi calculado
   int            bd_id;
   AnsiString     sigla;          //3 caracteres
   int            tipo_se;        //tipo da SE
   double         pot_inst;       //potência instalada
   double         demanda_max;    //demanda maxima
   double         demanda_med;    //demanda média
   int            ntrafos;        //numero de trafos da se
   double         taxa_cresc;     //taxa de crescimento (%)
   double         valta ;         //Tensão nominal no lado de alta tensão
   double         vmedia;         //Tensão nominal no lado de baixa tensão
   double         fat_utilizacao; //fator de utilização (pu)
   int            fat_carga;      //fator de carga (%)
   int            ntrafos_min;    //numero minimo de trafos da se
   int            ntrafos_max;    //numero maximo de trafos da se
   int            nalim_min;      //numero minimo de alimentadores da se
   int            nalim_max;      //número máximo de alimentadores
   AnsiString     cod_alim;       //Código dos alimentadores

   strPROETD()
      {
      stt_flow = false;
      bd_id = 0;

      sigla = cod_alim = "";

      tipo_se = ntrafos = fat_carga = ntrafos_min = ntrafos_max = nalim_min = nalim_max = 0;

      pot_inst = demanda_max = demanda_med = taxa_cresc = valta = vmedia = fat_utilizacao = 0;
      }
   };

//---------------------------------------------------------------------------
class VTBDhist : public TObject
   {
   public:
                   __fastcall  VTBDhist(void) {};
		virtual      __fastcall ~VTBDhist(void) {};
		virtual bool __fastcall  ArvoreContemNo(VTArvore* arvore, AnsiString codigo) = 0;
		virtual bool __fastcall  InsereRede(TList* lisPRI) = 0;
		virtual bool __fastcall  InsereArvore(VTArvore* arvore) = 0;
		virtual bool __fastcall  InsereDensidadeCarga(VTArvore* arvore) = 0;
      virtual bool __fastcall  InserePerdaTecnica(VTArvore* arvore) = 0;
      virtual bool __fastcall  LeListaArvore(TList* lstArvore) = 0;
      virtual bool __fastcall  LeArvoreDensidadeCarga(VTArvore* arvore, int intLevel = -1) = 0;
      virtual bool __fastcall  LeArvorePerdaTecnica(VTArvore* arvore, int intLevel = -1) = 0;

   };

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBDhist* NewObjBDhistAccess(VTApl *apl_owner, VTSNPDBConn    *bd_conn);
VTBDhist* NewObjBDhistOracle(VTApl *apl_owner, TOracleSession *ptrOracleSession);

#endif
//---------------------------------------------------------------------------
//eof