//-----------------------------------------------------------------------------
#ifndef VTExportaODSSH
#define VTExportaODSSH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
//op��es do usu�rio
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
// fun��o global para criar objeto VTExportaODSS
//-----------------------------------------------------------------------------
VTExportaODSS* NewObjExportaODSS(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
