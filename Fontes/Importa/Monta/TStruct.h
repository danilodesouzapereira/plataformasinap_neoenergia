// ---------------------------------------------------------------------------
#ifndef TStructH
#define TStructH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Rede\Estrutura.h"
#include "..\..\Curvas\VTCurva.h"

// -----------------------------------------------------------------------------
// definição de sentido para Mutua
enum sentidoMUTUA
{
	sentidoB1B2 = 12, sentidoB2B1 = 21
};

// -----------------------------------------------------------------------------
class VTCurva;

// -----------------------------------------------------------------------------
// ID que não deve ser usado na busca de objetos na montagem das redes
// #define ID_NULO   0

// -----------------------------------------------------------------------------
// estrutura auxiliar usada p/ dados de um patamar de uma curva
struct strPATAMAR
{
	__fastcall strPATAMAR(void);
	__fastcall ~strPATAMAR(void);
	// dados
#define SIZEOF_VALOR  32

	strHM hm_ini;
	strHM hm_fim;
	double valor[SIZEOF_VALOR];
};

struct strCURVA
{
	__fastcall strCURVA(void);
	__fastcall ~strCURVA(void);
	// dados
#define SIZEOF_CURVA  144

	int id;
	AnsiString codigo;
	int unidade; // DVK 2013.05.28
	bool tipica;
	unsigned status;
	int num_pat;
	int num_valor;
	strPATAMAR patamar[SIZEOF_CURVA];
};

struct strCURVA_TIPICA : public strCURVA
{
	__fastcall strCURVA_TIPICA(void);
	__fastcall ~strCURVA_TIPICA(void);

	// dados
	int dia;
	int tipo;
	int classe_tag;
	double energia_min_kwhmes;
	double energia_max_kwhmes;
};

// -----------------------------------------------------------------------------
// estrutura auxiliar usada p/ comentários em equipamentos
struct strCOMENTARIO
{
	__fastcall strCOMENTARIO(void);
	__fastcall ~strCOMENTARIO(void);

	// dados
	float dx;
	float dy;
	AnsiString comentario;
};

// -----------------------------------------------------------------------------
// definições de estruturas
struct strARRANJO1
{ // estrutura complementar p/ strTRECHO
	__fastcall strARRANJO1(void);
	__fastcall ~strARRANJO1(void);

	bool tipico;
	// dados
	TDate data;
	AnsiString codigo;
	AnsiString extern_id;
	int tipo_cfg; // ver VTArranjo::enumCFG_ARRANJO
	int fases;
	bool ramal;

	struct
	{
		AnsiString cod_cabo;
	}

	cfg_caboZ0Z1;

	struct
	{
		AnsiString estgeo_id;
		AnsiString cod_caboA;
		AnsiString cod_caboB;
		AnsiString cod_caboC;
		AnsiString cod_caboN;
	}

	cfg_suporte_cabo;

	struct
	{
		double iadm_amp;
		strIMP z0; // ohm/km
		double c0; // nF/km
		strIMP z1; // ohm/km
		double c1; // nF/km
	}

	cfg_arranjoZ0Z1;

	struct
	{
		double iadm_amp;
		double sbase_mva;
		double vbase_kv;
		strIMP z0_pu; // pu
		double yc0_pu; // pu
		strIMP z1_pu; // pu
		double yc1_pu; // pu
	}

	cfg_pu;
};

struct strARRANJO2
{ // arranjos lidos da base de dados do SINAP ou de bases externas
	__fastcall strARRANJO2(void);
	__fastcall ~strARRANJO2(void);

	// dados
	bool tipico;
	TDate data;
	int arranjo_id;
	int tipo_cfg; // ver VTArranjo::enumCFG_ARRANJO
	int fases;
	bool ramal;
	AnsiString codigo;
	AnsiString extern_id;
	double res_terra;
	double vmin_kv;
	double vmax_kv;
	TColor color;
	unsigned status;

	struct
	{
		int suporte_id; // obrigatório
		struct
		{
			int cabo_id;
			int pto_fixacao_id;
			int multiplicidade;
		} fase[4]; // ordenado por A,B,C,N
	}

	cfg_suporte_cabo;

	struct
	{
		int cabo_id;
		int suporte_id; // opcional
	}

	cfg_caboZOZ1;

	struct
	{
		double iadm_amp;
		strIMP z0;
		double c0;
		strIMP z1;
		double c1;
		int suporte_id; // opcional
	}

	cfg_arranjoZ0Z1;

	struct
	{
		double iadm_amp;
		strIMP mat_z[4][4];
		double mat_c[4][4];
	}

	cfg_matZ;

	struct
	{
		double iadm_amp;
		strIMP z0;
		double c0;
		strIMP z1;
		double c1;
		int suporte_id; // opcional
	}

	cfg_puSB100;
};

struct strBARRA
{
	__fastcall strBARRA(void);
	__fastcall ~strBARRA(void);

	// dados
	bool obra;
	int bar_id;
	AnsiString codigo;
	double vnom;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	strUTM utm;
	strESQ_BARRA esq;
	// int           index;
	strCOMENTARIO comentario;
};

struct strBATERIA
{
	__fastcall strBATERIA(void);
	__fastcall ~strBATERIA(void);

	// dados
	bool obra;
	unsigned show;
	unsigned status;
	int 		   bat_id;
	int			   bar_id;
	AnsiString	   codigo;
	AnsiString	   extern_id;
	int			   fases;
	int			   tipo_operacao;
	int			   modelo_carga;
	int			   modelo_desc;
	double		   p_nom_kw;
	double		   e_nom_kwh;
	double		   cap_inv_kva;
	double		   p_ociosa_pu;
	double		   q_ociosa_pu;
	double		   rend_carga_pu;
	double		   rend_desc_pu;
	double		   gatilho_carga_pu;
	double		   gatilho_desc_pu;

	struct
	{
		double icte_pu;
		double scte_pu;
		double zcte_pu;
	}modelo;

	strCURVA curva;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;

};

struct strCABO
{
	__fastcall strCABO(void);
	__fastcall ~strCABO(void);

	// dados
	int id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned status;
	int tipo_cfg; // ver enumCFG_CABO em VTCabo
	double iadm; // corrente admissível (A)
	double vmin; // tensão mínima em que é utilizado (kV)
	double vmax; // tensão máxima em que é utilizado (kV)
	double r; // resistência (ohm/km)
	double raio; // raio(mm)
	double gmr; // raio médio geométrico (mm)
	strIMP z0; // impedância de seq.0 (ohm/km)
	strIMP z1; // impedância de seq.1 (ohm/km)
	double c0; // capacitânca de seq.0 (nF/km)
	double c1; // capacitânca de seq.1 (nF/km)
};

struct strCANAL
{
	__fastcall strCANAL(void);
	__fastcall ~strCANAL(void);

	// dados
	bool obra;
	int canal_id;
	int medidor_id;
	AnsiString codigo;
	int tipo_canal;
	double tolerancia_perc;

	struct
	{
		int tipo;
		int id;
		int fases;
	}

	eqpto_medido;
	strCURVA curva;
};

struct strCAPACITOR
{
	__fastcall strCAPACITOR(void);
	__fastcall ~strCAPACITOR(void);

	// dados
	bool obra;
	int cap_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	int ligacao;
	double vnom;
	double q_mvar;
	strCURVA curva;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strCAPSERIE
{
	__fastcall strCAPSERIE(void);
	__fastcall ~strCAPSERIE(void);

	// dados
	bool obra;
	int id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double vnom; // kV
	double snom; // MVA
	strIMP z; // ohm
	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strCARGA
{
	__fastcall strCARGA(void);
	__fastcall ~strCARGA(void);

	// dados
	bool obra;
	int car_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	bool vip;
	bool grupoA;
	int fases;
	int classe_cons;
	VTCurva *curva_tipica;
    int categoria;

	struct
	{
		double icte_pu;
		double scte_pu;
		double zcte_pu;
	}

	modelo;

	// int                      numero_cons;
	struct
	{
		int residencial;
		int comercial;
		int industrial;
		int rural;
		int outros;
		int a4;
		int a2;
	}

	num_cons;
	double energia_mwhmes;
	strCURVA curva;

	struct
	{ // dados de IP
		int num_lamp;
		double pot_kw;
		double fat_pot;
	}

	ip;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;

	struct
	{
		VTBarra *barra;
	}

	obj;
};

struct strCARGANL
{
	__fastcall strCARGANL(void);
	__fastcall ~strCARGANL(void);

	// dados
	bool obra;
	int car_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	int ordem;
	double fat_pot;
	bool flag_indutivo;
	bool flag_kVA;
	double snomA;
	double snomB;
	double snomC;
	double angA;
	double angB;
	double angC;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strCHAVE
{
	__fastcall strCHAVE(void);
	__fastcall ~strCHAVE(void);

	// dados
	bool obra;
	int chv_id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	AnsiString codigo;
	AnsiString extern_id;
	AnsiString elo_fusivel;
	AnsiString tipo_gis;
	unsigned show;
	unsigned status;
	double corn;
	int estado;
	bool protecao_ativa;
	bool telecomandada;
	int tecno_comunicacao;

	struct
	{
		int id;
		int tipo;
		AnsiString codigo;
	}

	tipochave;
	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strCLUSTER
{
	__fastcall strCLUSTER(void);
	__fastcall ~strCLUSTER(void);

	// dados
	bool obra;
	int cluster_id;
	AnsiString codigo;
};

struct strDOMINIO
{
	__fastcall strDOMINIO(void);
	__fastcall ~strDOMINIO(void);

	// dados
	bool obra;
	int dominio_id;
	TDateTime data_criacao;
	AnsiString codigo;
};

struct strFILTRO
{
	__fastcall strFILTRO(void);
	__fastcall ~strFILTRO(void);

	// dados
	bool obra;
	int id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double vnom; // kV
	double snom; // MVAr
	strIMP z0; // pu
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strGERADOR
{
	__fastcall strGERADOR(void);
	__fastcall ~strGERADOR(void);

	// dados
	bool obra;
	int ger_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	int tiposup;
	int tipo_geracao;
	int ligacao;
   int fases;
	double vnom;
	double snom_mva;
	double smax_mva;
	double fplim;
	strIMP z0;
	strIMP z1;
	strCURVA curva;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strMEDIDOR
{
	__fastcall strMEDIDOR(void);
	__fastcall ~strMEDIDOR(void);

	// dados
	bool obra;
	int medidor_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strMUTUA
{
	__fastcall strMUTUA(void);
	__fastcall ~strMUTUA(void);

	// dados
	bool obra;
	int id;
	int rede_id;
	int tre1_id;
	int tre2_id;
	int tre1_bar_id;
	int tre2_bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	strIMP z0; // ohm/km
	strIMP z1; // ohm/km
	double postre1;
	double postre2;
	strCOMENTARIO comentario;
};

struct strPLAN_ACAO
{
	__fastcall strPLAN_ACAO(void);
	__fastcall ~strPLAN_ACAO(void);

	// dados
	int acao_id;
	int obra_id;
	int alternativa_id;
	int estudo_id;
	int tipo_acao;
	int tipo_eqpto;
	int eqpto_id;
	int eqpto_antes_id;
	int eqpto_depos_id;
    int item_id;
};

struct strPLAN_ALTERNATIVA
{
	__fastcall strPLAN_ALTERNATIVA(void);
	__fastcall ~strPLAN_ALTERNATIVA(void);

	// dados
	int alternativa_id;
	int estudo_id;
	AnsiString codigo;
    AnsiString justificativa;
};

struct strPLAN_ESTUDO
{
	__fastcall strPLAN_ESTUDO(void);
	__fastcall ~strPLAN_ESTUDO(void);

	// dados
	int estudo_id;
	int tipo;
	AnsiString codigo;
	AnsiString autor;
	TDateTime data;
	AnsiString justificativa;
};

struct strPLAN_OBRA
{
	__fastcall strPLAN_OBRA(void);
	__fastcall ~strPLAN_OBRA(void);

	// dados
	int obra_id;
	int alternativa_id;
	int estudo_id;
	int item_id;
	int tipo;
	int ipe_atributo_id;
	AnsiString codigo;
	TDateTime data_inicio;
	TDateTime data_ativacao;
	AnsiString responsavel;
	double ob_especial;
	double custo;
	bool altera_taxa_falha;
	double taxa_falha_ins;
	double taxa_falha_sub;
	bool finalizada;
    bool bloqueada;
};

struct strPLAN_IPE
{
	__fastcall strPLAN_IPE(void);
	__fastcall ~strPLAN_IPE(void);

	// dados
	int estudo_id;
	int alternativa_id;
	int obra_id;
	int atributo_tipo;
	int atributo_id;
    int item_id;
	AnsiString descricao;
};

struct strREATOR
{
	__fastcall strREATOR(void);
	__fastcall ~strREATOR(void);

	// dados
	bool obra;
	int id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	int ligacao;
	double vnom; // kV
	double q_mvar; // MVA
	strCURVA curva;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strREDE
{
	__fastcall strREDE(void);
	__fastcall ~strREDE(void);

	// dados
	bool obra;
	int rede_id;
	int cluster_id;
	int dominio_id;
	int barini_id;
	int mrede_id;
	bool mrede_redutivel;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	bool estatica;
	bool radial;
	bool equivalente;
	TColor color;

    struct
    {
        int id;
        int segmento;
        AnsiString codigo;
    }

	tiporede;

	struct
	{
		int x1, y1, x2, y2;
	}

	area_esq;

    struct
    {
		int id_alim_pai;
        bool mae_filha_conectada;
        AnsiString cod_chave;
        strCURVA curva_demanda;
        AnsiString sigla;
        AnsiString regional;
    }

	dados_ext;

	strCOMENTARIO comentario;
};

struct strREGULADOR
{
	__fastcall strREGULADOR(void);
	__fastcall ~strREGULADOR(void);

	// dados
	bool obra;
	int id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	int modelo;
	int rel_tap;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	bool by_pass;
	int ligacao;
	int fases;
	double snom_mva;
	double var_tensao; // % para baixo ou para cima
	int num_passo; // passos para baixo ou para cima
	strIMP z0;
	strIMP z1;
	bool ajuste_auto;

	struct
	{
		bool reversivel;
		int bar_ref_id;
	}sentido;

	struct
	{
		int passo; // passo fixo selecionado
	}cfg_fixo;

	struct
	{
		int bar_ref_id;
		int bar_ref_inv_id;
		double v_pu; // tensão alvo da barra controlada
		double vinv_pu; // tensão alvo da barra controlada no sentido inverso
		double larg_bandamorta_pu; //largura de banda morta em pu
		double larg_bandamorta_inv_pu; //largura de banda morta em pu
	}cfg_auto;

	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strSUPORTE
{
	__fastcall strSUPORTE(void);
	__fastcall ~strSUPORTE(void);

	// dados
	int id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned status;
	int tipo; // ver enum enumTIPOSUPORTE
	double vmin; // tensão mínima em que é utilizado
	double vmax; // tensão máxima rede  em que é utilizado
	int num_pto; // número de pontos de fixação

	struct
	{
		int id; // ID do ponto de fixação
		AnsiString codigo; // código do pto fixação
		double coord_x; // distância horizontal
		double coord_y; // distância vertical
	}

	pto_fixacao[20];
};

struct strSUPRIMENTO
{
	__fastcall strSUPRIMENTO(void);
	__fastcall ~strSUPRIMENTO(void);

	// dados
	bool obra;
	int sup_id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	int tiposup;
	double smax;
	double vnom_kv;
	strIMP zeq0;
	strIMP zeq1;
	strPOT pcc_3f;
	strPOT pcc_ft;
	strCURVA curva;
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strTIPO_CHAVE
{
	__fastcall strTIPO_CHAVE(void);
	__fastcall ~strTIPO_CHAVE(void);

	// dados
	int id;
	int tipo;
	AnsiString codigo;
	int modo_operacao;
	unsigned status;
};

struct strTIPO_REDE
{
	__fastcall strTIPO_REDE(void);
	__fastcall ~strTIPO_REDE(void);

	// dados
	int id;
	int segmento;
	AnsiString codigo;
	AnsiString descricao;
	double vmin_kv;
	double vmax_kv;
	int qtde_rede; // número de redes deste tipo existentes na base
	TColor color;
	unsigned status;
};

struct strTRAFO
{
	__fastcall strTRAFO(void);
	__fastcall ~strTRAFO(void);

	// dados
	bool obra;
	int trf_id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double snom_mva;
	strIMP z0; // em pu de Snom ou pu de Sb=100 quando Snom é nulo
	strIMP z1; // idem z0
	double perda_ferro; // em % de Snom ou % de Sb=100 quando Snom é nulo
	bool auto_trafo;
	int modelo_eqv; // modelo de équivalente {1: monofásico, 3: trifásico

	struct
	{
		int fases;
		int ligacao;
		double vnom;
		double tap;
		double defasagem;
		strIMP zterra; // ohm
	}

	pri, sec;

	struct
	{
		int tipo; // ver enumLTC em VTTrafo
		int bar_id;
		double v_pu; // tensão em pu
	}

	ltc;
	strTRAFO_EQV eqv;
	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strTRAFO3E
{
	__fastcall strTRAFO3E(void);
	__fastcall ~strTRAFO3E(void);

	// dados
	bool obra;
	int trf_id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	int bar3_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double perda_ferro;
	strIMP zps0;
	strIMP zps1;
	strIMP zpt0;
	strIMP zpt1;
	strIMP zst0;
	strIMP zst1;

	struct
	{
		int fases;
		int ligacao;
		double snom;
		double vnom;
		double tap;
		double defasagem;
		strIMP zterra;
	}
	pri, sec, ter;
   struct
   {
      int    tipo;
      float  dv_perc;
      int    num_passo;
      int    passo;
   } ltc;

	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strTRAFOZZ
{
	__fastcall strTRAFOZZ(void);
	__fastcall ~strTRAFOZZ(void);

	// dados
	bool obra;
	int id;
	int bar_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double vnom; // kV
	double snom; // MVAr
	strIMP z0; // pu
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

struct strTRECHO
{
	__fastcall strTRECHO(void);
	__fastcall ~strTRECHO(void);

	// dados
	bool obra;
	int tre_id;
	int rede_id;
	int bar1_id;
	int bar2_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	unsigned status;
	double comp_km;
	int fases;
	strESQ_LIGA esq;
	strCOMENTARIO comentario;
};

struct strYREF
{
	__fastcall strYREF(void);
	__fastcall ~strYREF(void);

	// dados
	bool obra;
	int yref_id;
	int barra_id;
	AnsiString codigo;
	AnsiString extern_id;
	unsigned show;
	strADM y0_pu;
	strADM y1_pu;
	strADM maty_pu[4][4];
	strESQ_EQBAR esq;
	strCOMENTARIO comentario;
};

// ---------------------------------------------------------------------------
#endif
// eof
