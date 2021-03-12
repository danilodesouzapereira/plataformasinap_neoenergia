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

   // Inicia��o dos campos
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

   // Inicia��o dos campos
   strARSESP()
      {
      // OOPS_Perda: dttData fica inv�lido?
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
   AnsiString cabo_tronco;      //c�digo do cabo tronco
   AnsiString cabo_ramal;       //c�digo do cabo ramal
   int        angulo;           //�ngulo do alimentador em graus
   double     demanda_max;      //demanda m�xima
   int        nbar_carga;       //n�mero de pontos (barras de carga)
   double     fat_pot;          //fator de pot�ncia
   int        fat_carga;        //fator de carga (%)
   double     taxa_arbor;       //taxa de arboriza��o (%)
   double     taxa_reforma;     //taxa de reforma
   double     taxa_cresc_vert;  //taxa de crescimento vertical (%)
   double     taxa_cresc_hori;  //taxa de crescimento horizontal (%)
   double     taxa_falha_arbor; //
   double     taxa_falha_eqpto; //
   double     taxa_falha_outro; //
   double     DEC;              //
   double     FEC;              //
   double     alfa;             //alfa
   int        nconsumidores;    //n�mero de consumidores
   double     area_atuacao;     //�rea (m2) do setor circular definido pelo �ngulo do alimentador
   double     dist_baricentro;  //dist�ncia (m) do baricentro de carga
   //<JCG> dados novos: 15/02/2007
   AnsiString fase;             //{trif�sica/bif�sica/monof�sica/mista}
   AnsiString cabo_tronco_3f;   //Descri��o da bitola do cabo utilizado no tronco trif�sico
   AnsiString cabo_tronco_2f;   //Descri��o da bitola do cabo utilizado no tronco bif�sico
   AnsiString cabo_tronco_1f;   //Descri��o da bitola do cabo utilizado no tronco monof�sico
   AnsiString cabo_ramal_3f;    //Descri��o da bitola do cabo utilizado no ramal trif�sico
   AnsiString cabo_ramal_2f;    //Descri��o da bitola do cabo utilizado no ramal bif�sico
   AnsiString cabo_ramal_1f;    //Descri��o da bitola do cabo utilizado no ramal monof�sico
   double     comp_tronco_3f;   //comprimento do tronco trif�sico, em km
   double     comp_tronco_2f;   //comprimento do tronco bif�sico, em km
   double     comp_tronco_1f;   //comprimento do tronco monof�sico, em km
   double     comp_ramal_3f;    //comprimento de ramal trif�sico, em km
   double     comp_ramal_2f;    //comprimento de ramal bif�sico, em km
   double     comp_ramal_1f;    //comprimento de ramal monof�sico, em km
   int        num_consumidor_mt;//total de consumidores MT atendidos
   int        num_consumidor_bt;//total de consumidores BT atendidos
   double     fat_perda;        //Fator de perda  (%)
   int        num_reg_tensao;   //Quantidade de reguladores de tens�o
   double     dv_maximo;        //tens�o na sa�da da subesta��o menos a tens�o no ponto de tens�o m�nima (kV)
   double     vnom;            //tens�o nominal (kV)

   // Inicia��o dos dados
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
   AnsiString sigla;                 //C�digo do Posto;
   double     snom;                  //Pot�ncia nominal do transformador (kVA);
   double     cargto;                //Carregamento do transformador (kVA);
   double     comprimento;           //Comprimento da rede secund�ria atendida (km);
   AnsiString cabo;                  //Descri��o da bitola do cabo utilizado;
   double     fat_carga;             //Fator de carga (pu);
   double     fat_potencia;          //Fator de pot�ncia (pu);
   double     taxa_crescimento;      //Taxa de crescimento (% a.a.);
   AnsiString descricao;             //Descri��o do Posto;
   AnsiString regional;              //Regional;
   double     vnom_at;               //Tens�o (kV);
   double     vnom_bt;               //Tens�o (V);
   AnsiString tipologia;             //Tipologia (M - Monof�sico / T - Trif�sico).
   //<JCG> dados novos: 15/02/2007
   AnsiString fase;                  //{trif�sica/bif�sica/monof�sica/mista}
   AnsiString cod_trafo;             //
   double     fat_utilizacao;        //Fator de utiliza��o (%)
   AnsiString cabo_tronco;           //Descri��o da bitola do cabo utilizado no tronco
   AnsiString cabo_ramal;            //Descri��o da bitola do cabo utilizado nos ramais
   double     comp_tronco;           //Comprimento do tronco da rede secund�ria atendida (km)
   double     comp_ramal;            //Comprimento dos ramais da secund�ria atendida (km)
   double     comp_vao_medio;        //Comprimento do v�o m�dio (m)
   double     fat_carga_leve_pesada; //Rela��o entre carga leve e pesada (%)

   // Inicia��o dos dados
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
   double         pot_inst;       //pot�ncia instalada
   double         demanda_max;    //demanda maxima
   double         demanda_med;    //demanda m�dia
   int            ntrafos;        //numero de trafos da se
   double         taxa_cresc;     //taxa de crescimento (%)
   double         valta ;         //Tens�o nominal no lado de alta tens�o
   double         vmedia;         //Tens�o nominal no lado de baixa tens�o
   double         fat_utilizacao; //fator de utiliza��o (pu)
   int            fat_carga;      //fator de carga (%)
   int            ntrafos_min;    //numero minimo de trafos da se
   int            ntrafos_max;    //numero maximo de trafos da se
   int            nalim_min;      //numero minimo de alimentadores da se
   int            nalim_max;      //n�mero m�ximo de alimentadores
   AnsiString     cod_alim;       //C�digo dos alimentadores

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
//fun��o global
//---------------------------------------------------------------------------
VTBDhist* NewObjBDhistAccess(VTApl *apl_owner, VTSNPDBConn    *bd_conn);
VTBDhist* NewObjBDhistOracle(VTApl *apl_owner, TOracleSession *ptrOracleSession);

#endif
//---------------------------------------------------------------------------
//eof