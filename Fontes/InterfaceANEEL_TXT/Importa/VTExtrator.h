//-----------------------------------------------------------------------------
#ifndef VTExtratorH
#define VTExtratorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Rede\Estrutura.h"

struct sdi_COORDENADA   {
								AnsiString   codbase;
								AnsiString   codigo;
								struct       {
												 double x;
                                     double y;
                                     }utm_m;
								};
/*
struct sdi_CAPACITOR    {
                        int          id;
                        int          id_barra;
                        AnsiString   cod;
                        AnsiString   extern_id;
                        double       vnom_kv;
                        double       snom_kva;
                        AnsiString   ligacao;
								strESQ_EQBAR esq;
								};
*/
struct sdi_CARGA_BT     {
								AnsiString   codbase;
								AnsiString   ramal;
								AnsiString   codigo;
								AnsiString   fases;
								AnsiString   codigo_pto;
								int          rede_associada;
								int          tipo_medicao;
								AnsiString   tipologia_curva;
								double       energia_mwhmes[12];
								};
struct sdi_CARGA_MT     {
								AnsiString   codbase;
								AnsiString   circuito;
								AnsiString   codigo;
								AnsiString   fases;
								AnsiString   codigo_pto;
								int          rede_associada;
								AnsiString   tipologia_curva;
								double       energia_mwhmes[12];
								};
struct sdi_CHAVE_BT     {
								AnsiString   codbase;
								AnsiString   codigo_rede;
								AnsiString   codigo_segmento;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								AnsiString   fases;
								int          estado;
								};
struct sdi_CHAVE_MT     {
								AnsiString   codbase;
								AnsiString   circuito;
								AnsiString   codigo_segmento;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								AnsiString   fases;
								int          estado;
								};
struct sdi_CIRCUITO     {
								AnsiString   codbase;
								AnsiString   subestacao;
								AnsiString   codigo;
								double       vnom_kv;
								double       vope_pu;
								AnsiString   codigo_pto;
								int          tipico;
								double       energia_mwhmes[12];
								double       perda_mwhmes[12];
								};
struct sdi_CONDUTOR     {
								AnsiString   codbase;
								AnsiString   codigo;
								double       r_ohm_km;
								double       x_ohm_km;
								double       i_max_amp;
								AnsiString   descricao;
								};
struct sdi_GERADOR_BT   {
								AnsiString   codbase;
								AnsiString   ramal;
								AnsiString   codigo;
								AnsiString   codigo_pto;
								AnsiString   fases;
								double       energia_mwhmes[12];
								};
struct sdi_GERADOR_MT   {
								AnsiString   codbase;
								AnsiString   circuito;
								AnsiString   codigo;
								AnsiString   codigo_pto;
								AnsiString   fases;
								double       energia_mwhmes[12];
								};
struct sdi_RAMAL_BT     {
								AnsiString   codbase;
								AnsiString   rede;
								AnsiString   codigo;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								AnsiString   fases;
								AnsiString   codigo_condutor;
								double       comprimento_km;
								};
struct sdi_REGULADOR    {
								AnsiString   circuito;
								AnsiString   codigo;
								int          sequencial;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								double       snom_kva;
								int          tipo;
								double       vreg_pu;
								AnsiString   fases_pri;
								AnsiString   fases_sec;
								double       r_perc;
								double       xhl_perc;
								double       perda_total_w;
								double       perda_vazio_w;
								double       energia_mwhano;
								};
struct sdi_SEGMENTO_BT  {
								AnsiString   codbase;
								AnsiString   rede;
								AnsiString   codigo;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								AnsiString   fases;
								AnsiString   codigo_condutor;
								double       comprimento_km;
								};
struct sdi_SEGMENTO_MT  {
								AnsiString   codbase;
								AnsiString   circuito;
								AnsiString   codigo;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								AnsiString   fases;
								AnsiString   codigo_condutor;
								double       comprimento_km;
								AnsiString   classificacao;
								};
struct sdi_TRAFO_AT_MT  {
								AnsiString   codbase;
								AnsiString   subestacao;
								AnsiString   codigo;
								int          sequencial;
								int          tipo;
								double       vnom_pri_kv;
								double       vnom_sec_kv;
								double       vnom_ter_kv;
								double       snom_mva;
								double       perda_vazio_perc;
								double       perda_total_perc;
								double       energia_mwhmes[12];
								AnsiString   nivel_tensao;
								AnsiString   propriedade;
								};
struct sdi_TRAFO_MT_BT  {
								AnsiString   codbase;
								AnsiString   circuito;
								AnsiString   codigo_trafo;  //código da rede secundária
								int          sequencial;
								AnsiString   codigo_pto1;
								AnsiString   codigo_pto2;
								double       snom_kva;
								int          mrt;
								int          tipo;
								AnsiString   fases_pri;
								AnsiString   fases_sec;
								AnsiString   fases_ter;
								double       vff_sec_kv;
								double       vtap_pu;
								double       r_perc;
								double       xhl_perc;
								double       xht_perc;
								double       xlt_perc;
								double       perda_total_perc;
								double       perda_vazio_perc;
								AnsiString   classificacao;
								AnsiString   propriedade;
								};

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTExtrator : public TObject
   {                    
	public:
						 __fastcall  VTExtrator(void) {};
      virtual      __fastcall ~VTExtrator(void) {};
		virtual bool __fastcall  InsereCondutor(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCoordenada(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCargaBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCargaMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereChaveBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereChaveMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCircuito(TStrings *campos) = 0;
		virtual bool __fastcall  InsereGeradorBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereGeradorMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereRamalBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereReguladorMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereSegmentoBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereSegmentoMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereTrafoAT_MT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereTrafoMT_BT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereVersao(TStrings *campos) = 0;
		virtual bool __fastcall  Sucesso(void) = 0;
      virtual bool __fastcall  TrataFimDados(void) = 0;
      virtual bool __fastcall  TrataIniciaDados(AnsiString cod_dominio) = 0;
	};

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTExtrator* NewObjExtrator(VTApl *apl);
VTExtrator* NewObjExtratorCaboArranjo(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
