//-----------------------------------------------------------------------------
#ifndef VTExtratorH
#define VTExtratorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\..\Curvas\VTCurva.h"
#include "..\..\Rede\Estrutura.h"

struct sdi_COORDENADA   {
								int          CodBase;
								AnsiString   codigo;
								struct       {
												 double x;
                                     double y;
                                     }utm_m;
								};
struct sdi_CARGA_BT     {
								int          CodBase;
								AnsiString   CodTrafo;
								AnsiString   CodRamal;
								AnsiString   CodConsumidorBT;
								AnsiString   CodFases;
								AnsiString   CodPontoAcoplamento;
								int          SemRedeAssociada;
								int          TipoMedicao;
								AnsiString   TipologiaCurvaCarga;
								double       Energia_mwhmes[12];
								AnsiString   Descricao;
								};
struct sdi_CARGA_MT     {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodConsumidorMT;
								AnsiString   CodFases;
								AnsiString   CodPontoAcoplamento;
								int          SemRedeAssociada;
								AnsiString   TipologiaCurvaCarga;
								double       Energia_mwhmes[12];
								AnsiString   Descricao;
								};
struct sdi_CHAVE_BT     {
								int          CodBase;
								AnsiString   CodTrafo;
								AnsiString   CodChave;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								AnsiString   CodFases;
								int          Estado;
								AnsiString   Descricao;
								};
struct sdi_CHAVE_MT     {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodChave;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								AnsiString   CodFases;
								int          Estado;
								AnsiString   Descricao;
								};
struct sdi_CIRCUITO     {
								int          CodBase;
								AnsiString   CodSubestacao;
								AnsiString   CodAlimentador;
								double       Vnom_kv;
								double       Vope_pu;
								AnsiString   CodPontoAcoplamento;
								int          CircAtipico;
								double       Energia_mwhmes[12];
								double       Perda_mwhmes[12];
								AnsiString   Descricao;
								};
struct sdi_CONDUTOR     {
								int          CodBase;
								AnsiString   CodCondutor;
								double       Rohm_km;
								double       Xohm_km;
								double       Imax_amp;
								AnsiString   Descricao;
								};
struct sdi_CURVA        {
								int          CodBase;
								AnsiString   CodCurva;
								AnsiString   TipoDia;
								double       P_kw[96];
								};
struct sdi_GERADOR_BT   {
								int          CodBase;
								AnsiString   CodRamal;
								AnsiString   CodGerador;
								AnsiString   CodPontoAcoplamento;
								AnsiString   CodFases;
								double       Energia_mwhmes[12];
								AnsiString   Descricao;
								};
struct sdi_GERADOR_MT   {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodGerador;
								AnsiString   CodPontoAcoplamento;
								AnsiString   CodFases;
								double       Energia_mwhmes[12];
								AnsiString   Descricao;
								};
struct sdi_RAMAL        {
								int          CodBase;
								AnsiString   CodTrafo;
								AnsiString   CodRamal;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								AnsiString   CodFases;
								AnsiString   CodCondutor;
								double       Comprimento_km;
								AnsiString   Descricao;
								};
struct sdi_REGULADOR    {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodRegulador;
								int          SeqBanco;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								double       Snom_kva;
								int          TipoRegulador;
								double       Vreg_pu;
								AnsiString   CodFasesPri;
								AnsiString   CodFasesSec;
								double       R_perc;
								double       Xhl_perc;
								double       PerdaTotal_w;
								double       PerdaVazio_w;
								double       Energia_mwhano;
								AnsiString   Descricao;
								};
struct sdi_SEGMENTO_BT  {
								int          CodBase;
								AnsiString   CodTrafo;
								AnsiString   CodSegmento;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								AnsiString   CodFases;
								AnsiString   CodCondutor;
								double       Comprimento_km;
								AnsiString   Descricao;
								};
struct sdi_SEGMENTO_MT  {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodSegmento;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								AnsiString   CodFases;
								AnsiString   CodCondutor;
								double       Comprimento_km;
								AnsiString   ClasseSegmento;
								AnsiString   Descricao;
								};
struct sdi_TRAFO_AT_MT  {
								int          CodBase;
								AnsiString   CodSubestacao;
								AnsiString   CodTrafo;
								int          SeqBanco;
								int          TipoTrafo;
								double       Vnom_pri_kv;
								double       Vnom_sec_kv;
								double       Vnom_ter_kv;
								double       Snom_mva;
								double       PerdaVazio_perc;
								double       PerdaTotal_perc;
								double       Energia_mwhmes[12];
								AnsiString   SegAlocacaoPerda;
								AnsiString   Propriedade;
								AnsiString   Descricao;
								};
struct sdi_TRAFO_MT_BT  {
								int          CodBase;
								AnsiString   CodAlimentador;
								AnsiString   CodTrafo;
								int          SeqBanco;
								AnsiString   CodPontoAcoplamento1;
								AnsiString   CodPontoAcoplamento2;
								double       Snom_kva;
								int          MRT;
								int          TipoTrafo;
								AnsiString   CodFasesPri;
								AnsiString   CodFasesSec;
								AnsiString   CodFasesTer;
								double       Vnom_sec_kv;
								double       Vtap_pu;
								double       R_perc;
								double       Xhl_perc;
								double       Xht_perc;
								double       Xlt_perc;
								double       PerdaTotal_perc;
								double       PerdaVazio_perc;
								AnsiString   ClasseTrafo;
								AnsiString   Propriedade;
								AnsiString   Descricao;
								};

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTExtrator : public TObject
   {                    
	public:
						 __fastcall  VTExtrator(void) {};
      virtual      __fastcall ~VTExtrator(void) {};
		virtual bool __fastcall  InsereCargaBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCargaMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereChaveBT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereChaveMT(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCircuito(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCondutor(TStrings *campos) = 0;
		virtual bool __fastcall  InsereCoordenada(TStrings *campos) = 0;
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

#endif
//-----------------------------------------------------------------------------
// eof
