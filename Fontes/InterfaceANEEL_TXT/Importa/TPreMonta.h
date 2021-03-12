//---------------------------------------------------------------------------
#ifndef TPreMontaH
#define TPreMontaH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTPreMonta.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTArranjos;
class VTBarra;
class VTCabo;
class VTClasses;
class VTCurvas;
class VTDominio;
class VTFases;
class VTLog;
class VTMonta;
class VTRede;
class VTRedes;
class VTSuporte;
class VTSuprimento;
class VTTipos;
class VTTrafo;
class TSe;
struct strIMP;

//---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
	{
   public:
						 __fastcall  TPreMonta(VTApl *apl);
						 __fastcall ~TPreMonta(void);
		virtual void __fastcall  DefineDataReferencia(TDateTime data);
		virtual void __fastcall  DefineFormatoCampoAcoplamento(int opcao);
		virtual void __fastcall  DefineSubestacaoComPrimarios(bool enabled);
		virtual bool __fastcall  InsereCargaBT(sdi_CARGA_BT &sdi);
		virtual bool __fastcall  InsereCargaMT(sdi_CARGA_MT &sdi);
		virtual bool __fastcall  InsereChaveBT(sdi_CHAVE_BT &sdi);
		virtual bool __fastcall  InsereChaveMT(sdi_CHAVE_MT &sdi);
		virtual bool __fastcall  InsereCircuito(sdi_CIRCUITO &sdi);
		virtual bool __fastcall  InsereCondutor(sdi_CONDUTOR &sdi);
		virtual bool __fastcall  InsereCoordenada(sdi_COORDENADA &sdi);
		virtual bool __fastcall  InsereGeradorBT(sdi_GERADOR_BT &sdi);
		virtual bool __fastcall  InsereGeradorMT(sdi_GERADOR_MT &sdi);
		virtual bool __fastcall  InsereRamalBT(sdi_RAMAL_BT &sdi);
		virtual bool __fastcall  InsereReguladorMT(sdi_REGULADOR &sdi);
		virtual bool __fastcall  InsereSegmentoBT(sdi_SEGMENTO_BT &sdi);
		virtual bool __fastcall  InsereSegmentoMT(sdi_SEGMENTO_MT &sdi);
		virtual bool __fastcall  InsereTrafoAT_MT(sdi_TRAFO_AT_MT &sdi);
		virtual bool __fastcall  InsereTrafoMT_BT(sdi_TRAFO_MT_BT &sdi);
		virtual bool __fastcall  Sucesso(void);
      virtual bool __fastcall  TrataFimDados(void);
      virtual bool __fastcall  TrataIniciaDados(AnsiString cod_dominio);

   protected: //métodos
		void          __fastcall AtualizaCargaEquivalente(VTCarga *carga_eqv, VTCarga *carga);
		double        __fastcall CalculaPotenciaLampada(double energia_mwhmes, VTCurva *curva);
		VTCarga*      __fastcall CriaCargaEquivalente(int barra_id, AnsiString codigo, int fases_tag);
		AnsiString    __fastcall DefineCodigoUnicoArranjo(AnsiString cod_orig);
		double        __fastcall DefineDefasagemTrafo(int ligacao_pri, int ligacao_sec);
		VTDominio*    __fastcall ExisteDominio(void);
		VTBarra*      __fastcall ExisteInsereBarra(AnsiString codigo);
		TSe*          __fastcall ExisteInsereSe(sdi_TRAFO_AT_MT &sdi);
		VTRede*       __fastcall ExisteInsereRedeBT(AnsiString codigo, VTBarra *barra_in);
		VTRede*       __fastcall ExisteRede(AnsiString codigo);
		TSe*          __fastcall ExisteSe(AnsiString codigo);
		VTBarra*      __fastcall InsereBarra(AnsiString codigo);
		VTCluster*    __fastcall InsereCluster(AnsiString codigo);
		bool          __fastcall InsereDominio(AnsiString codigo);
		VTRede*       __fastcall InsereRede(AnsiString codigo, int segmento, VTBarra *barra_ini);
		VTSuprimento* __fastcall InsereSuprimento(AnsiString codigo, VTBarra *barra, double v_pu);
		bool          __fastcall IntegraSubestacaoComPrimarios(void);
		int           __fastcall TraduzClasseConsumidor(AnsiString codigo, AnsiString grupo="BT");
		int           __fastcall TraduzTipoChave(AnsiString codigo);
		int           __fastcall TraduzTipoLigacao(AnsiString ligacao);
		int           __fastcall TraduzTipoLTC(AnsiString ajuste);
		int           __fastcall TraduzTipoRede(AnsiString tipo);
		int           __fastcall TraduzTipoSuprimento(int tipo_sup);
		bool          __fastcall ValidaImpedancia(strIMP &z);

	protected: //objetos externos
      VTApl       *apl;
      VTArranjos  *arranjos;
		VTClasses   *classes;
		VTCurvas    *curvas;
      VTTipos     *tipos;
		VTFases     *fases;
      VTLog       *plog;
      VTMonta     *monta;
		VTCabo      *cabo_rede_pri;
      VTCabo      *cabo_rede_sec;
      VTDominio   *dominio;
      VTRedes     *redes;

   protected: //dados locais
		#define        DXBAR  1000
		#define        DYBAR -1000
		bool           sucesso;
		bool           subestacao_integrada;
		int            formato_acoplamento;
		TDateTime      data;
		AnsiString     codigo_dominio;
		VTRede         *ult_rede;
		TList          *lisSE;
	};

#endif
//-----------------------------------------------------------------------------
// eof



