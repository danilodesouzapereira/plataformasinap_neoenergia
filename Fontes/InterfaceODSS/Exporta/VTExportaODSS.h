//-----------------------------------------------------------------------------
#ifndef VTExportaODSSH
#define VTExportaODSSH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//opções do usuário
struct strOPCAO_ODSS {
                     bool rede_aneel;
                     int  trafoDY;
					 int  carga_eqv;
                     };

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;

//-----------------------------------------------------------------------------
class VTExportaODSS : public TObject
   {
   public:
                   __fastcall  VTExportaODSS(void) {};
      virtual      __fastcall ~VTExportaODSS(void) {};
	  virtual bool __fastcall  ExportaRedes(AnsiString arq_txt, strOPCAO_ODSS &opcao) = 0;
      virtual bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_txt, strOPCAO_ODSS &opcao) = 0;
   	  virtual bool __fastcall  ExportaCenarioEstabilidade(AnsiString arq_txt) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTExportaODSS
//-----------------------------------------------------------------------------
VTExportaODSS* NewObjExportaODSS(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
