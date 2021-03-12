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
struct strIMP;

//---------------------------------------------------------------------------
class TPreMonta : public VTPreMonta
   {
   public:
                       __fastcall  TPreMonta(VTApl *apl);
                       __fastcall ~TPreMonta(void);
      virtual bool     __fastcall  InsereArranjoCaboPorfase(sdi_ARRANJO &sdi);
      virtual bool     __fastcall  InsereArranjoCaboZ0Z1(sdi_ARRANJO &sdi);
      virtual bool     __fastcall  InsereArranjoPUSB100(sdi_ARRANJO &sdi);
      virtual bool     __fastcall  InsereArranjoZ0Z1(sdi_ARRANJO &sdi);
      virtual bool     __fastcall  InsereBarra(sdi_BARRA &sdi);
      virtual void     __fastcall  InsereBarraStart(void);
	  virtual void     __fastcall  InsereBarraStop(void);
	  virtual bool     __fastcall  InsereBateria(sdi_BATERIA &sdi);
      virtual bool     __fastcall  InsereCaboUnipolar(sdi_CABO &sdi);
      virtual bool     __fastcall  InsereCaboZ0Z1(sdi_CABO &sdi);
      virtual bool     __fastcall  InsereCapacitor(sdi_CAPACITOR &sdi);
      virtual bool     __fastcall  InsereCapacitorSerie(sdi_CAPSERIE &sdi);
      virtual VTCarga* __fastcall  InsereCargaComMedicao(sdi_CARGA &sdi);
      virtual VTCarga* __fastcall  InsereCargaIP(sdi_CARGA_IP &sdi);
      virtual VTCarga* __fastcall  InsereCargaTipica(sdi_CARGA &sdi);
      virtual bool     __fastcall  InsereChave(sdi_CHAVE &sdi);
      virtual bool     __fastcall  InsereCurvaCapacitor(sdi_CURVA &sdi);
      virtual bool     __fastcall  InsereCurvaCarga(sdi_CURVA &sdi);
      virtual bool     __fastcall  InsereCurvaCargaIP(sdi_CURVA &sdi);
      virtual bool     __fastcall  InsereCurvaGerador(sdi_CURVA &sdi);
      virtual bool     __fastcall  InsereCurvaReator(sdi_CURVA &sdi);
      virtual bool     __fastcall  InsereFiltro(sdi_FILTRO &sdi);
      virtual bool     __fastcall  InsereGerador(sdi_GERADOR &sdi);
      virtual bool      __fastcall InsereMedidor(sdi_MEDIDOR &sdi);
      virtual bool     __fastcall  InsereMedidorCanal(sdi_CANAL &sdi);
      virtual bool     __fastcall  InsereMutua(sdi_MUTUA &sdi);
      virtual bool     __fastcall  InsereReator(sdi_REATOR &sdi);
      virtual bool     __fastcall  InsereRede(sdi_REDE &sdi);
      virtual bool     __fastcall  InsereReguladorAutomatico(sdi_REGULADOR &sdi);
      virtual bool     __fastcall  InsereReguladorTapFixo(sdi_REGULADOR &sdi);
      virtual bool     __fastcall  InsereSuporte(sdi_SUPORTE &sdi);
      virtual bool     __fastcall  InsereSuprimento(sdi_SUPRIMENTO &sdi);
      virtual bool     __fastcall  InsereTipoChave(sdi_TIPO_CHAVE &sdi);
      virtual bool     __fastcall  InsereTipoConsumidor(sdi_TIPO_CONS &sdi);
      virtual bool     __fastcall  InsereTipoRede(sdi_TIPO_REDE &sdi);
      virtual bool     __fastcall  InsereTrafo2E(sdi_TRAFO2E &sdi);
      virtual bool     __fastcall  InsereTrafo3E(sdi_TRAFO3E &sdi);
      virtual bool     __fastcall  InsereTrafoMono(sdi_TRAFO_MONO &sdi);
      virtual bool     __fastcall  InsereTrafoZZ(sdi_TRAFOZZ &sdi);
      virtual bool     __fastcall  InsereTrechoComArranjo(sdi_TRECHO &sdi);
      virtual bool     __fastcall  InsereTrechoZ0Z1(sdi_TRECHO &sdi);
      //
      virtual bool __fastcall  Sucesso(void);
      virtual bool __fastcall  TrataFimDados(void);
      virtual bool __fastcall  TrataIniciaDados(AnsiString cod_dominio);

   protected: //métodos
      void       __fastcall AtualizaCargaEquivalente(VTCarga *carga_eqv, VTCarga *carga);
      VTCarga*   __fastcall CriaCargaEquivalente(int barra_id, AnsiString codigo, int fases_tag);
      AnsiString __fastcall DefineCodigoUnicoArranjo(AnsiString cod_orig);
      double     __fastcall DefineDefasagemTrafo(int ligacao_pri, int ligacao_sec);
      VTDominio* __fastcall ExisteDominio(void);
      bool       __fastcall IniciaClasseConsumidor(void);
      VTCluster* __fastcall InsereCluster(AnsiString codigo);
      bool       __fastcall InsereDominio(AnsiString codigo);
      bool       __fastcall MontaCurvaTipica(sdi_CURVA &sdi, strCURVA_TIPICA &str_curva);
      int        __fastcall TraduzClasseConsumidor(AnsiString codigo, AnsiString grupo="BT");
      int        __fastcall TraduzTipoChave(AnsiString codigo);
      int        __fastcall TraduzTipoLigacao(AnsiString ligacao);
      int        __fastcall TraduzTipoLTC(AnsiString ajuste);
      int        __fastcall TraduzTipoRede(AnsiString tipo);
      int        __fastcall TraduzTipoSuprimento(int tipo_sup);
      bool       __fastcall ValidaImpedancia(strIMP &z);

   private:
      int __fastcall ExtraiFases(AnsiString extern_id);

   protected: //objetos externos
      VTApl       *apl;
      VTArranjos  *arranjos;
      VTClasses   *classes;
      VTTipos     *tipos;
      VTFases     *fases;
      VTLog       *plog;
      VTMonta     *monta;
      VTCabo      *cabo_rede_pri;
      VTCabo      *cabo_rede_sec;
      VTDominio   *dominio;
      VTRedes     *redes;
      //VTRede      *rede_etd;
      //VTRede      *rede_pri;
      //VTRede      *rede_sec;

   protected: //dados locais
      #define        ERRO_DE_SISTEMA "Erro de sistema."
      bool           sucesso;
	  AnsiString     codigo_dominio;
      //VTPatamares    *patamares;
      VTRede         *rede;
   };

#endif
//-----------------------------------------------------------------------------
// eof



