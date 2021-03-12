//---------------------------------------------------------------------------
#ifndef VTBDaneelH
#define VTBDaneelH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Importa\VTExtrator.h"

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTBDaneel : public TObject
   {

   public:
                   __fastcall  VTBDaneel(void) {};
      virtual      __fastcall ~VTBDaneel(void) {};
		virtual bool __fastcall  Conecta(AnsiString filename) = 0;
		virtual void __fastcall  Desconecta(void) = 0;
		//leitura
		//virtual bool __fastcall  ReadBase(int cod_base, int cod_dist, int ano, TDateTime dat_bas) = 0;
		virtual bool __fastcall  ReadCargaBT(void) = 0;
		virtual bool __fastcall  ReadCargaMT(void) = 0;
		virtual bool __fastcall  ReadChaveBT(void) = 0;
		virtual bool __fastcall  ReadChaveMT(void) = 0;
		virtual bool __fastcall  ReadCircMT(void) = 0;
		virtual bool __fastcall  ReadCodCondutor(void) = 0;
		virtual bool __fastcall  ReadCrvCrgBT(void) = 0;
		virtual bool __fastcall  ReadCrvCrgMT(void) = 0;
		virtual bool __fastcall  ReadGeradorBT(void) = 0;
		virtual bool __fastcall  ReadGeradorMT(void) = 0;
		virtual bool __fastcall  ReadRamal(void) = 0;
		virtual bool __fastcall  ReadRegulador(void) = 0;
		virtual bool __fastcall  ReadSegmentoBT(void) = 0;
		virtual bool __fastcall  ReadSegmentoMT(void) = 0;
		virtual bool __fastcall  ReadTrafoATATATMT(void) = 0;
		virtual bool __fastcall  ReadTrafoMTMTMTBT(void) = 0;
		//escrita
		//virtual bool __fastcall  WriteBase(int cod_base, int cod_dist, int ano, TDateTime dat_bas) = 0;
		virtual bool __fastcall  WriteCargaBT(sdi_CARGA_BT &sdi) = 0;
		virtual bool __fastcall  WriteCargaMT(sdi_CARGA_MT &sdi) = 0;
		virtual bool __fastcall  WriteChaveBT(sdi_CHAVE_BT &sdi) = 0;
		virtual bool __fastcall  WriteChaveMT(sdi_CHAVE_MT &sdi) = 0;
		virtual bool __fastcall  WriteCircMT(sdi_CIRCUITO &sdi) = 0;
		virtual bool __fastcall  WriteCodCondutor(sdi_CONDUTOR &sdi) = 0;
		virtual bool __fastcall  WriteCrvCrgBT(sdi_CURVA &sdi) = 0;
		virtual bool __fastcall  WriteCrvCrgMT(sdi_CURVA &sdi) = 0;
		virtual bool __fastcall  WriteGeradorBT(sdi_GERADOR_BT &sdi) = 0;
		virtual bool __fastcall  WriteGeradorMT(sdi_GERADOR_MT &sdi) = 0;
		virtual bool __fastcall  WriteRamal(sdi_RAMAL &sdi) = 0;
		virtual bool __fastcall  WriteRegulador(sdi_REGULADOR &sdi) = 0;
		virtual bool __fastcall  WriteSegmentoBT(sdi_SEGMENTO_BT &sdi) = 0;
		virtual bool __fastcall  WriteSegmentoMT(sdi_SEGMENTO_MT &sdi) = 0;
		virtual bool __fastcall  WriteTrafoATATATMT(sdi_TRAFO_AT_MT &sdi) = 0;
		virtual bool __fastcall  WriteTrafoMTMTMTBT(sdi_TRAFO_MT_BT &sdi) = 0;
	};

//---------------------------------------------------------------------------
//função global
//---------------------------------------------------------------------------
VTBDaneel* NewObjBDaneel(VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof