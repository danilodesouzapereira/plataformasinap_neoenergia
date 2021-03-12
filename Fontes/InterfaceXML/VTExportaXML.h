//-----------------------------------------------------------------------------
#ifndef VTExportaXMLH
#define VTExportaXMLH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;

//-----------------------------------------------------------------------------
class VTExportaXML : public TObject
   {
   public:  //property
      __property bool ExportaCurvaTipica = {read=exporta.curvatipica,   write=exporta.curvatipica};
      __property bool ExportaArranjo     = {read=exporta.arranjo,     write=exporta.arranjo};
      __property bool ExportaCabo        = {read=exporta.cabo,        write=exporta.cabo};
      __property bool ExportaSuporte     = {read=exporta.suporte,     write=exporta.suporte};
      __property bool ExportaRede        = {read=exporta.rede,        write=exporta.rede};
      __property bool ExportaReducao     = {read=exporta.reducao,     write=exporta.reducao};
      __property bool ExportaTipoChave   = {read=exporta.tipochave,   write=exporta.tipochave};
      __property bool ExportaTipoRede    = {read=exporta.tiporede,    write=exporta.tiporede};

   public:
                   __fastcall  VTExportaXML(void) { };
      virtual      __fastcall ~VTExportaXML(void) { };
      virtual bool __fastcall  ExportaArranjos(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ExportaCurvasTipicas(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ExportaRedes(AnsiString arq_xml) = 0;
      virtual bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_xml) = 0;

   protected:
      struct   {
               bool curvatipica;
               bool arranjo;
               bool cabo;
               bool suporte;
               bool rede;
               bool reducao;
               bool tipochave;
               bool tiporede;
               }exporta;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTExportaXML
//-----------------------------------------------------------------------------
VTExportaXML* NewObjExportaXML(VTApl *apl_owner);

#endif
//-----------------------------------------------------------------------------
// eof
