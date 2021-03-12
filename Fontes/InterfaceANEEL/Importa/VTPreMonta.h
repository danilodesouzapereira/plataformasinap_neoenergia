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
	public:
						 __fastcall  VTPreMonta(void) {};
		virtual      __fastcall ~VTPreMonta(void) {};
		virtual void __fastcall  DefineDataReferencia(TDateTime data) = 0;
		virtual void __fastcall  DefineFormatoCampoAcoplamento(int opcao) = 0;
		virtual void __fastcall  DefineSubestacaoComPrimarios(bool enabled) = 0;
		virtual bool __fastcall  InsereCargaBT(sdi_CARGA_BT &sdi) = 0;
		virtual bool __fastcall  InsereCargaMT(sdi_CARGA_MT &sdi) = 0;
		virtual bool __fastcall  InsereChaveBT(sdi_CHAVE_BT &sdi) = 0;
		virtual bool __fastcall  InsereChaveMT(sdi_CHAVE_MT &sdi) = 0;
		virtual bool __fastcall  InsereCircuito(sdi_CIRCUITO &sdi) = 0;
		virtual bool __fastcall  InsereCondutor(sdi_CONDUTOR &sdi) = 0;
		virtual bool __fastcall  InsereCoordenada(sdi_COORDENADA &sdi) = 0;
		virtual bool __fastcall  InsereCurvaTipicaBT(sdi_CURVA &sdi) = 0;
		virtual bool __fastcall  InsereCurvaTipicaMT(sdi_CURVA &sdi) = 0;
		virtual bool __fastcall  InsereGeradorBT(sdi_GERADOR_BT &sdi) = 0;
		virtual bool __fastcall  InsereGeradorMT(sdi_GERADOR_MT &sdi) = 0;
		virtual bool __fastcall  InsereRamalBT(sdi_RAMAL &sdi) = 0;
		virtual bool __fastcall  InsereReguladorMT(sdi_REGULADOR &sdi) = 0;
		virtual bool __fastcall  InsereSegmentoBT(sdi_SEGMENTO_BT &sdi) = 0;
		virtual bool __fastcall  InsereSegmentoMT(sdi_SEGMENTO_MT &sdi) = 0;
		virtual bool __fastcall  InsereTrafoAT_MT(sdi_TRAFO_AT_MT &sdi) = 0;
		virtual bool __fastcall  InsereTrafoMT_BT(sdi_TRAFO_MT_BT &sdi) = 0;
		virtual bool __fastcall  Sucesso(void) = 0;
		virtual bool __fastcall  TrataFimDados(void)=0;
		virtual bool __fastcall  TrataIniciaDados(AnsiString cod_dominio="")=0;
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
