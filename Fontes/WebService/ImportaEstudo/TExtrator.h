//---------------------------------------------------------------------------
#ifndef TExtratorH
#define TExtratorH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTExtrator.h"

//---------------------------------------------------------------------------
class VTMonta;

//---------------------------------------------------------------------------
class TExtrator : public VTExtrator
   {
   public:
           __fastcall  TExtrator(VTApl *apl_owner);
           __fastcall ~TExtrator(void);
      bool __fastcall  InsereAlternativa(TStrings *campos);
	  bool __fastcall  InsereDemandaPotencia(TStrings *campos);
	  bool __fastcall  InsereElementoPotencia(TStrings *campos); //DVK 2015.04.28
      bool __fastcall  InsereEquipamento(TStrings *campos);
      bool __fastcall  InsereEstudo(TStrings *campos);
	  bool __fastcall  InsereInformacaoTecnica(TStrings *campos);
	  bool __fastcall  InsereMunicipio(TStrings *campos);
      bool __fastcall  InsereObra(TStrings *campos);
	  bool __fastcall  InserePotenciaInstalada(TStrings *campos);
	  bool __fastcall  InsereRede(TStrings *campos);
	  bool __fastcall  InsereRedeAlternativa(TStrings *campos);
	  bool __fastcall  InsereSolicitacao(TStrings *campos);
      bool __fastcall  InsereValorInformacaoTecnica(TStrings *campos);
      bool __fastcall  TrataInicio(void);
      bool __fastcall  TrataTermino(void);

   private:
	  void __fastcall CriaCroqui(AnsiString endereco_croqui);

   protected: //objetos externos
      VTApl    *apl;
      VTMonta  *monta;

   protected: //dados locais
	  bool     sucesso;
	  TByteDynArray *croqui;
	  TMemoryStream *mem, *out;
   };

#endif
//-----------------------------------------------------------------------------
// eof
