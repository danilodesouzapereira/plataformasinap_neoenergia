//---------------------------------------------------------------------------
#ifndef VTConversorH
#define VTConversorH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
#include <Fontes\ImportaRedeGIS\VTImportaRedeGIS.h>

//---------------------------------------------------------------------------
class VTConversor : public TObject
   {
   public:
                            __fastcall  VTConversor(void){};
      virtual               __fastcall ~VTConversor(void){};
      virtual bool          __fastcall  ImportaLisArqPrimario(TStrings *files, strOPImporta *opcao) = 0;
      virtual bool          __fastcall  ImportaLisArqSubestacao(TStrings *files, strOPImporta *opcao) = 0;
      virtual strOPImporta* __fastcall  OpcoesImportacao(void) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criarobjeto VTConversor
//-----------------------------------------------------------------------------
VTConversor* NewObjConversor(VTApl *apl);

//---------------------------------------------------------------------------
#endif
