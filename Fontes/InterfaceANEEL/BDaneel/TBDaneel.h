//---------------------------------------------------------------------------
#ifndef TBDaneelH
#define TBDaneelH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "VTBDaneel.h"

//---------------------------------------------------------------------------
class VTLog;
class VTPreMonta;

//---------------------------------------------------------------------------
class TBDaneel : public VTBDaneel
   {
   public:
           __fastcall  TBDaneel(VTApl *apl_owner);
			  __fastcall ~TBDaneel(void);
		bool __fastcall  Conecta(AnsiString arq_access);
		void __fastcall  Desconecta(void);
		//leitura
//    bool __fastcall  ReadBase(int cod_base, int cod_dist, int ano, TDateTime dat_bas);
		bool __fastcall  ReadCargaBT(void);
		bool __fastcall  ReadCargaMT(void);
		bool __fastcall  ReadChaveBT(void);
		bool __fastcall  ReadChaveMT(void);
		bool __fastcall  ReadCircMT(void);
		bool __fastcall  ReadCodCondutor(void);
		bool __fastcall  ReadCrvCrgBT(void);
		bool __fastcall  ReadCrvCrgMT(void);
		bool __fastcall  ReadGeradorBT(void);
		bool __fastcall  ReadGeradorMT(void);
		bool __fastcall  ReadRamal(void);
		bool __fastcall  ReadRegulador(void);
		bool __fastcall  ReadSegmentoBT(void);
		bool __fastcall  ReadSegmentoMT(void);
		bool __fastcall  ReadTrafoATATATMT(void);
		bool __fastcall  ReadTrafoMTMTMTBT(void);
		//escrita
		//bool __fastcall  WriteBase(int cod_base, int cod_dist, int ano, TDateTime dat_bas);
		bool __fastcall  WriteCargaBT(sdi_CARGA_BT &sdi);
		bool __fastcall  WriteCargaMT(sdi_CARGA_MT &sdi);
		bool __fastcall  WriteChaveBT(sdi_CHAVE_BT &sdi);
		bool __fastcall  WriteChaveMT(sdi_CHAVE_MT &sdi);
		bool __fastcall  WriteCircMT(sdi_CIRCUITO &sdi);
		bool __fastcall  WriteCodCondutor(sdi_CONDUTOR &sdi);
		bool __fastcall  WriteCrvCrgBT(sdi_CURVA &sdi);
		bool __fastcall  WriteCrvCrgMT(sdi_CURVA &sdi);
		bool __fastcall  WriteGeradorBT(sdi_GERADOR_BT &sdi);
		bool __fastcall  WriteGeradorMT(sdi_GERADOR_MT &sdi);
		bool __fastcall  WriteRamal(sdi_RAMAL &sdi);
		bool __fastcall  WriteRegulador(sdi_REGULADOR &sdi);
		bool __fastcall  WriteSegmentoBT(sdi_SEGMENTO_BT &sdi);
		bool __fastcall  WriteSegmentoMT(sdi_SEGMENTO_MT &sdi);
		bool __fastcall  WriteTrafoATATATMT(sdi_TRAFO_AT_MT &sdi);
		bool __fastcall  WriteTrafoMTMTMTBT(sdi_TRAFO_MT_BT &sdi);

	private:  //métodos
		void __fastcall QueryClose(void);
		bool __fastcall QueryExecSql(AnsiString cmd_sql);
		bool __fastcall QueryOpenReadOnly(AnsiString tablename);

	private:  //objetos externos
		VTApl      *apl;
		VTLog      *plog;
		VTPreMonta *pre_monta;

	private:  //dados locais
		TADOConnection *connection;
		TADOQuery      *query;
	};

#endif
//---------------------------------------------------------------------------
//eof

