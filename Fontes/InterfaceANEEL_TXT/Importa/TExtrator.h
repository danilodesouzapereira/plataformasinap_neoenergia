//---------------------------------------------------------------------------
#ifndef TExtratorH
#define TExtratorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"

//---------------------------------------------------------------------------
class VTPreMonta;
class VTLog;

//---------------------------------------------------------------------------
class TExtrator : public VTExtrator
   {
   public:
           __fastcall  TExtrator(VTApl *apl);
           __fastcall ~TExtrator(void);
		bool __fastcall  InsereCondutor(TStrings *campos);
		bool __fastcall  InsereCoordenada(TStrings *campos);
		bool __fastcall  InsereCargaBT(TStrings *campos);
		bool __fastcall  InsereCargaMT(TStrings *campos);
		bool __fastcall  InsereChaveBT(TStrings *campos);
		bool __fastcall  InsereChaveMT(TStrings *campos);
		bool __fastcall  InsereCircuito(TStrings *campos);
		bool __fastcall  InsereGeradorBT(TStrings *campos);
		bool __fastcall  InsereGeradorMT(TStrings *campos);
		bool __fastcall  InsereRamalBT(TStrings *campos);
		bool __fastcall  InsereReguladorMT(TStrings *campos);
		bool __fastcall  InsereSegmentoBT(TStrings *campos);
		bool __fastcall  InsereSegmentoMT(TStrings *campos);
		bool __fastcall  InsereTrafoAT_MT(TStrings *campos);
		bool __fastcall  InsereTrafoMT_BT(TStrings *campos);
		bool __fastcall  InsereVersao(TStrings *campos);
		bool __fastcall  Sucesso(void);
		bool __fastcall  TrataFimDados(void);
		bool __fastcall  TrataIniciaDados(AnsiString cod_dominio);

	protected: //objetos externos
      VTApl       *apl;
      VTPreMonta  *pre_monta;
      VTLog       *plog;

   protected: //dados locais
      bool     sucesso;
      int      versao;
   };

#endif
//-----------------------------------------------------------------------------
// eof
