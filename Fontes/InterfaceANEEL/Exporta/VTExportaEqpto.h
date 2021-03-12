//---------------------------------------------------------------------------
#ifndef VTExportaEqptoH
#define VTExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Importa\VTExtrator.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTBarra;
class VTCarga;
class VTChave;
class VTCurva;
class VTGerador;
class VTRede;
class VTRegulador;
class VTTrafo;
class VTTrecho;

//---------------------------------------------------------------------------
class VTExportaEqpto : public TObject
   {
	public:
						 __fastcall  VTExportaEqpto(void) {};
						 __fastcall ~VTExportaEqpto(void) {};
		virtual bool __fastcall  MontaCargaBT(VTRede *rede, VTCarga *carga, sdi_CARGA_BT &sdi) = 0;
		virtual bool __fastcall  MontaCargaMT(VTRede *rede, VTCarga *carga, sdi_CARGA_MT &sdi) = 0;
		virtual bool __fastcall  MontaChaveBT(VTChave *chave, sdi_CHAVE_BT &sdi) = 0;
		virtual bool __fastcall  MontaChaveMT(VTChave *chave, sdi_CHAVE_MT &sdi) = 0;
		virtual bool __fastcall  MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, sdi_CIRCUITO &sdi) = 0;
		virtual bool __fastcall  MontaCondutor(VTArranjo *arranjo, sdi_CONDUTOR &sdi) = 0;
		virtual bool __fastcall  MontaCurvaTipica(VTCurva *curva, sdi_CURVA &sdi) = 0;
		virtual bool __fastcall  MontaGeradorBT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_BT &sdi) = 0;
		virtual bool __fastcall  MontaGeradorMT(VTRede *rede, VTGerador *gerador, sdi_GERADOR_MT &sdi) = 0;
		virtual bool __fastcall  MontaRamalBT(VTTrecho *trecho, sdi_RAMAL &sdi) = 0;
		virtual bool __fastcall  MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag, sdi_REGULADOR &sdi) = 0;
		virtual bool __fastcall  MontaSegmentoBT(VTTrecho *trecho, sdi_SEGMENTO_BT &sdi) = 0;
		virtual bool __fastcall  MontaSegmentoMT(VTTrecho *trecho, sdi_SEGMENTO_MT &sdi) = 0;
		virtual bool __fastcall  MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq, sdi_TRAFO_AT_MT &sdi) = 0;
		virtual bool __fastcall  MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq, sdi_TRAFO_MT_BT &sdi) = 0;
	};

//---------------------------------------------------------------------------
VTExportaEqpto* NewObjExportaEqpto(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

