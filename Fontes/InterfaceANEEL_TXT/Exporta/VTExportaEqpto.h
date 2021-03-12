//---------------------------------------------------------------------------
#ifndef VTExportaEqptoH
#define VTExportaEqptoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "TLine.h"

//---------------------------------------------------------------------------
class VTApl;
class VTArranjo;
class VTBarra;
class VTCabo;
class VTCapacitor;
class VTCapserie;
class VTCarga;
class VTChave;
class VTClasse;
class VTCurva;
class VTFases;
class VTFiltro;
class VTGerador;
class VTMutua;
class VTReator;
class VTRede;
class VTRegulador;
class VTSuporte;
class VTSuprimento;
class VTTipoChave;
class VTTipoRede;
class VTTrafo;
class VTTrafo3E;
class VTTrecho;
class VTTrafoZZ;

//---------------------------------------------------------------------------
class VTExportaEqpto : public TObject
   {
   public:
		__property TDateTime  Data      = {read=PD.data,        write=PD.data};
		__property char       Separador = {read=PD.separador,   write=PD.separador};
		__property AnsiString Versao    = {read=PD.versao,      write=PD.versao};

   public:
                         __fastcall  VTExportaEqpto(void) {};
                         __fastcall ~VTExportaEqpto(void) {};
		virtual AnsiString __fastcall  MontaArranjo(VTArranjo *arranjo) = 0;
      virtual AnsiString __fastcall  MontaCoordenada(VTBarra *barra) = 0;
		//virtual AnsiString __fastcall  MontaCapacitor(VTCapacitor *capacitor) = 0;
		virtual AnsiString __fastcall  MontaCargaBT(VTRede *rede, VTCarga *carga) = 0;
		virtual AnsiString __fastcall  MontaCargaMT(VTRede *rede, VTCarga *carga) = 0;
		//virtual AnsiString __fastcall  MontaCargaIP(VTCarga *carga) = 0;
		virtual AnsiString __fastcall  MontaChaveBT(VTChave *chave) = 0;
		virtual AnsiString __fastcall  MontaChaveMT(VTChave *chave) = 0;
		virtual AnsiString __fastcall  MontaCircuitoMT(VTRede *rede_sub, VTRede *rede_pri, double vnom_kv, double vope_pu) = 0;
		virtual AnsiString __fastcall  MontaGeradorBT(VTRede *rede, VTGerador *gerador) = 0;
		virtual AnsiString __fastcall  MontaGeradorMT(VTRede *rede, VTGerador *gerador) = 0;
		virtual AnsiString __fastcall  MontaRamalBT(VTTrecho *trecho) = 0;
		virtual AnsiString __fastcall  MontaReguladorMT(VTRegulador *regulador, int seq, int fases_tag) = 0;
		virtual AnsiString __fastcall  MontaSegmentoBT(VTTrecho *trecho) = 0;
		virtual AnsiString __fastcall  MontaSegmentoMT(VTTrecho *trecho) = 0;
		//virtual AnsiString __fastcall  MontaSuprimento(VTSuprimento *suprimento) = 0;
		virtual AnsiString __fastcall  MontaTrafoAT_MT(VTRede *rede_sub, VTTrafo *trafo, int seq) = 0;
		virtual AnsiString __fastcall  MontaTrafoMT_BT(VTRede *rede_pri, VTTrafo *trafo, int seq) = 0;
		virtual AnsiString __fastcall  MontaVersao(void) = 0;

   protected:
		struct{
				char        separador;
				AnsiString  versao;
				TDateTime   data;
				}PD;
   };

//---------------------------------------------------------------------------
VTExportaEqpto* NewObjExportaEqpto(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

