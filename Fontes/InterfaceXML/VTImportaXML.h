//-----------------------------------------------------------------------------
#ifndef VTImportaXMLH
#define VTImportaXMLH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTImportaXML : public TObject
   {
   public:
                   __fastcall  VTImportaXML(void) { };
      virtual      __fastcall ~VTImportaXML(void) { };
      virtual bool __fastcall  ExecutaAvaliacao(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ExecutaImportacao(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ImportaArranjos(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ImportaConfiguracao(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ImportaCurvasTipicas(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ImportaRedesApenas(AnsiString arq_xml) = 0;
      virtual void __fastcall  RelatorioLog(TStrings *lines) = 0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTImportaXML
//-----------------------------------------------------------------------------
VTImportaXML* NewObjImportaXML(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
