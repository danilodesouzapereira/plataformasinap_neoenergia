//-----------------------------------------------------------------------------
#ifndef VTPreMontaH
#define VTPreMontaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"

//-----------------------------------------------------------------------------
class VTApl;
class VTCarga;

//-----------------------------------------------------------------------------
class VTPreMonta : public TObject
   {
   public:  //property
      __property bool       ConsA4_Energia      = {read=PD.consA4_energia    , write=PD.consA4_energia    };
/*
      __property AnsiString CaboRedePRI         = {read=PD.caboPRI           , write=PD.caboPRI           };
      __property AnsiString CaboRedeSEC         = {read=PD.caboSEC           , write=PD.caboSEC           };
*/
   public:
                        __fastcall VTPreMonta(void) {};
      virtual           __fastcall~VTPreMonta(void) {};
      virtual bool      __fastcall InsereArranjoCaboPorfase(sdi_ARRANJO &sdi)=0;
      virtual bool      __fastcall InsereArranjoCaboZ0Z1(sdi_ARRANJO &sdi)=0;
      virtual bool      __fastcall InsereArranjoPUSB100(sdi_ARRANJO &sdi)=0;
      virtual bool      __fastcall InsereArranjoZ0Z1(sdi_ARRANJO &sdi)=0;
	  virtual bool      __fastcall InsereBarra(sdi_BARRA &sdi)=0;
      virtual void      __fastcall InsereBarraStart(void)=0;
	  virtual void      __fastcall InsereBarraStop(void)=0;
	  virtual bool 		__fastcall InsereBateria(sdi_BATERIA &sdi)=0;
      virtual bool      __fastcall InsereCaboUnipolar(sdi_CABO &sdi)=0;
      virtual bool      __fastcall InsereCaboZ0Z1(sdi_CABO &sdi)=0;
      virtual bool      __fastcall InsereCapacitor(sdi_CAPACITOR &sdi)=0;
      virtual bool      __fastcall InsereCapacitorSerie(sdi_CAPSERIE &sdi)=0;
      virtual VTCarga*  __fastcall InsereCargaComMedicao(sdi_CARGA &sdi)=0;
      virtual VTCarga*  __fastcall InsereCargaIP(sdi_CARGA_IP &sdi)=0;
      virtual VTCarga*  __fastcall InsereCargaTipica(sdi_CARGA &sdi)=0;
      virtual bool      __fastcall InsereChave(sdi_CHAVE &sdi)=0;
      virtual bool      __fastcall InsereCurvaCapacitor(sdi_CURVA &sdi)=0;
      virtual bool      __fastcall InsereCurvaCarga(sdi_CURVA &sdi)=0;
      virtual bool      __fastcall InsereCurvaCargaIP(sdi_CURVA &sdi)=0;
      virtual bool      __fastcall InsereCurvaGerador(sdi_CURVA &sdi)=0;
      virtual bool      __fastcall InsereCurvaReator(sdi_CURVA &sdi)=0;
      virtual bool      __fastcall InsereFiltro(sdi_FILTRO &sdi)=0;
      virtual bool      __fastcall InsereGerador(sdi_GERADOR &sdi)=0;
      virtual bool      __fastcall InsereMedidor(sdi_MEDIDOR &sdi)=0;
      virtual bool      __fastcall InsereMedidorCanal(sdi_CANAL &sdi)=0;
      virtual bool      __fastcall InsereMutua(sdi_MUTUA &sdi)=0;
      virtual bool      __fastcall InsereReator(sdi_REATOR &sdi)=0;
      virtual bool      __fastcall InsereRede(sdi_REDE &sdi)=0;
      virtual bool      __fastcall InsereReguladorAutomatico(sdi_REGULADOR &sdi)=0;
      virtual bool      __fastcall InsereReguladorTapFixo(sdi_REGULADOR &sdi)=0;
      virtual bool      __fastcall InsereSuporte(sdi_SUPORTE &sdi)=0;
      virtual bool      __fastcall InsereSuprimento(sdi_SUPRIMENTO &sdi)=0;
      virtual bool      __fastcall InsereTipoChave(sdi_TIPO_CHAVE &sdi)=0;
      virtual bool      __fastcall InsereTipoConsumidor(sdi_TIPO_CONS &sdi)=0;
      virtual bool      __fastcall InsereTipoRede(sdi_TIPO_REDE &sdi)=0;
      virtual bool      __fastcall InsereTrafo2E(sdi_TRAFO2E &sdi)=0;
      virtual bool      __fastcall InsereTrafo3E(sdi_TRAFO3E &sdi)=0;
      virtual bool      __fastcall InsereTrafoMono(sdi_TRAFO_MONO &sdi)=0;
      virtual bool      __fastcall InsereTrafoZZ(sdi_TRAFOZZ &sdi)=0;
      virtual bool      __fastcall InsereTrechoComArranjo(sdi_TRECHO &sdi)=0;
      virtual bool      __fastcall InsereTrechoZ0Z1(sdi_TRECHO &sdi)=0;
      virtual bool      __fastcall Sucesso(void) = 0;
      virtual bool      __fastcall TrataFimDados(void)=0;
      virtual bool      __fastcall TrataIniciaDados(AnsiString cod_dominio)=0;

   protected:
      struct   {
               bool       consA4_energia;
               //AnsiString caboPRI;
               //AnsiString caboSEC;
               }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTMonta
//-----------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl);
VTPreMonta* NewObjPreMonta1(VTApl *apl);
VTPreMonta* NewObjPreMonta2(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
