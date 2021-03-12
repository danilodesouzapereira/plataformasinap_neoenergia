//---------------------------------------------------------------------------
#ifndef VTCurvaH
#define VTCurvaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//enumerador p/ tipo de curva
enum tipoCURVA {curvaPQVT   = 1,
                curvaPQ     = 2,
                curvaPQ_MDP = 3,
                curvaPFP    = 4,
                curvaPFP_MDP= 5,
                curvaLD     = 6,
                curvaPQVIT  = 7,
                curvaHRM    = 8,
                curvaV      = 9,
                curvaI      =10,
                curvaP      =11,
                curvaQ      =12,
                curvaS      =13
                };

//---------------------------------------------------------------------------
//enumerador p/ unidade dos valores da curva
enum unidadeCURVA {unidKV    =1,
                   unidKVA   =2,
                   unidMVA   =3,
                   unidPU    =4,
                   unidPU_MED=5,
                   unidPU_MAX=6,
                   unidA     =7,
                   unidON_OFF=8,
                   unidKW    =9,
                   unidMW    =10,
                   unidKVAR  =11,
                   unidMVAR  =12
                   };

//---------------------------------------------------------------------------
//enumerador p/ número de valores por tipo de curva típica
enum nvCURVA  {nvCURVA_PQVT=4, nvCURVA_PQ=2,      nvCURVA_PQ_MDP=4,
               nvCURVA_PFP=2,  nvCURVA_PFP_MDP=4, nvCURVA_LD=1,
               nvCURVA_PQVIT=5,
               //
               nvCURVA_IP    = nvCURVA_LD,
               nvCURVA_CAR   = nvCURVA_PQ,
               nvCURVA_RRCAR = 8,
               nvCURVA_GER   = nvCURVA_PQVT,
               nvCURVA_SUP   = nvCURVA_PQVT,
               nvCURVA_RRSUP = 16,
               nvCURVA_CANAL = 3
              };

//---------------------------------------------------------------------------
//enumerador que indica posição das grandezas P,Q,V,F nas curvas
enum indVALOR {indP=0, indQ, indV, indT};

//---------------------------------------------------------------------------
//enumerador que indica dia da semana em que os dados da curva foram obtidos
enum diaMEDICAO {DOMINGO=1, SABADO=7, DIA_UTIL=8, SEMANA_COMPLETA=9};

/*
//---------------------------------------------------------------------------
//enumerador p/ unidade de tempo de curva
enum curvaESCALA  {curvaINDEF=-1, curva1HORA=24, curva15MIN=96,
                   curva10MIN=144, curva5MIN=288};
*/
//---------------------------------------------------------------------------
struct strHM   {
               int hora;
               int minuto;
               };
/*
//---------------------------------------------------------------------------
struct strMDP  {
               double media;
               double desvio; //desvio padrão
               };
*/
//---------------------------------------------------------------------------
class VTEqpto;
class VTClasse;

//---------------------------------------------------------------------------
class VTCurva : public TObject
   {
   //===================================
   public:  //property TEMPORÁRIA ATÉ ALTERAR A BASE DE DADOS
      __property AnsiString  CodigoBD             = {read=PM_GetCodigoBD,              write=PM_SetCodigoBD};
   //===================================

   public:  //property
      __property AnsiString  Codigo                = {read=PD.codigo,                  write=PD.codigo};
      __property AnsiString  Extern_id             = {read=PD.extern_id,               write=PD.extern_id};
      __property int         Id                    = {read=PD.id,                      write=PD.id};
      __property int         Tipo                  = {read=PD.tipo,                    write=PD.tipo};
      __property AnsiString  TipoAsStr             = {read=PM_GetTipoAsStr};
      __property int         Unidade               = {read=PD.unidade,                 write=PD.unidade};
      __property int         Escala                = {read=PD.escala,                  write=PD.escala};
      __property int         NumeroValores         = {read=PD.numero_valores,          write=PD.numero_valores};
      __property bool        Tipica                = {read=PD.tipica,                  write=PD.tipica};
      __property int         DiaMedicao            = {read=PD.dia_medicao,             write=PD.dia_medicao};
      __property AnsiString  DiaMedicaoAsStr       = {read=PM_GetDiaMedicaoAsStr};
      __property double      LimiteInferior        = {read=PD.limite.inf,              write=PD.limite.inf};
      __property double      LimiteSuperior        = {read=PD.limite.sup,              write=PD.limite.sup};
      __property unsigned    StatusAsUnsigned      = {read=PM_GetStatusAsUnsigned,     write=PM_SetStatusAsUnsigned};
      __property bool        Status[unsigned bit]  = {read=PM_GetStatus,               write=PM_SetStatus};
      __property double      FatorCorrecaoEnergia  = {read=PM_GetFatorCorrecaoEnergia, write=PM_SetFatorCorrecaoEnergia};
      __property VTEqpto     *Eqpto                = {read=PD.eqpto,                   write=PD.eqpto};
      __property VTClasse    *Classe               = {read=PD.classe,                  write=PD.classe};
      __property VTCurva     *CurvaSiamesaPUmedia  = {read=PD.curva_siamesa,           write=PD.curva_siamesa};
      __property TObject     *Obj                  = {read=PD.obj,                     write=PD.obj};

   public:  //funções públicas
                        __fastcall  VTCurva(void) {};
      virtual           __fastcall ~VTCurva(void) {};
		virtual VTCurva*  __fastcall  Clone(void) = 0;
		virtual void      __fastcall  CopiaAtributosDe(VTCurva &ref) = 0;
		virtual void      __fastcall  DefineStatus(int bit_status, bool enabled)=0;
      virtual double    __fastcall  DesvioPadrao(int ind_val) = 0;
      virtual bool      __fastcall  GetPeriodo(int index, strHM &hm_ini, strHM &hm_fim) = 0;
      virtual bool      __fastcall  GetPonto(int index, strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor) = 0;
      virtual bool      __fastcall  GetValor(strHM &hm, double valor[], int dim_valor) = 0;
      virtual double    __fastcall  Integral(int ind_val) = 0;
		virtual double    __fastcall  Maximo(int ind_val) = 0;
		virtual double    __fastcall  Media(int ind_val) = 0;
		virtual bool      __fastcall  MediaVarianciaDesvioPadrao(int ind_val, double &media, double &variancia, double &desvio) = 0;
		virtual double    __fastcall  Minimo(int ind_val) = 0;
		virtual int       __fastcall  NumeroPontos(void) = 0;
		virtual void      __fastcall  OrdenaPontos(void) = 0;
      virtual void      __fastcall  Reconfigura(int escala) = 0;
      virtual void      __fastcall  Reinicia(void) = 0;
      virtual bool      __fastcall  RetiraPonto(int index) = 0;
      virtual bool      __fastcall  SetPonto(strHM &hm, double valor[], int dim_valor) = 0;
      virtual bool      __fastcall  SetPonto(strHM &hm_ini, strHM &hm_fim, double valor[], int dim_valor) = 0;
      virtual bool      __fastcall  ValidaPeriodo24Horas(void) = 0;
      virtual double    __fastcall  Variancia(int ind_val) = 0;

   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetDiaMedicaoAsStr(void)=0;
      virtual double     __fastcall PM_GetFatorCorrecaoEnergia(void)=0;
      virtual bool       __fastcall PM_GetStatus(unsigned bit)=0;
      virtual unsigned   __fastcall PM_GetStatusAsUnsigned(void)=0;
      virtual AnsiString __fastcall PM_GetTipoAsStr(void)=0;
      virtual void       __fastcall PM_SetFatorCorrecaoEnergia(double fatcor)=0;
      virtual void       __fastcall PM_SetStatus(unsigned bit, bool enabled)=0;
      virtual void       __fastcall PM_SetStatusAsUnsigned(unsigned status)=0;

   //===================================
   protected: //métodos temporários acessados via property
      virtual AnsiString __fastcall PM_GetCodigoBD(void)=0;
      virtual void       __fastcall PM_SetCodigoBD(AnsiString codigo_bd)=0;
   //===================================

   protected: //dados acessados via property
      struct   {
               int         id;
               AnsiString  codigo;
               AnsiString  extern_id;
               int         tipo;           //ver enum tipoCURVA
               int         unidade;        //ver enum unidadeCURVA
               bool        tipica;
               int         dia_medicao;    //ver enum diaMEDICAO
               int         escala;         //ver enum patESCALA em Util\Constante\Const.h
               int         numero_valores; //número de valores em cada Ponto
               unsigned    status;         //ver enumSTATUS em VTEqpto.h
               double      fatcor_energia;
               VTEqpto     *eqpto;         //Eqpto associado à curva
               VTClasse    *classe;        //Classe de consumidor
               VTCurva     *curva_siamesa; //curva do tipo curvaPQ_MDP/unidPU_MED associada a uma curva definida em pu da máxima
               TObject     *obj;           //ponteiro auxiliar
               struct{//valores limites usados em curvas típicas: energia em kWh/mês
                     double inf;
                     double sup;
                     }limite;
               //IMPORTANTE: uma Curva está associada a um VTEqpto OU a uma VTClase, nunca aos dois ao mesmo tempo
               } PD;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCurva
//---------------------------------------------------------------------------
VTCurva* __fastcall NewObjCurva(void);
VTCurva* __fastcall NewObjCurvaPQ(void);
//VTCurva* __fastcall NewObjCurvaTipica(void);

//---------------------------------------------------------------------------
#endif
//eof
