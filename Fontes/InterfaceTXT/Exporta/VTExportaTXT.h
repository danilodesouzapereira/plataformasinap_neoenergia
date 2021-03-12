//-----------------------------------------------------------------------------
#ifndef VTExportaTXTH
#define VTExportaTXTH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;

//-----------------------------------------------------------------------------
class VTExportaTXT : public TObject
   {
   public:  //property
      __property bool ExportaCurvaTipica   = {read=exporta.curvatipica,  write=exporta.curvatipica};
      __property bool ExportaArranjo       = {read=exporta.arranjo,      write=exporta.arranjo};
      __property bool ExportaCabo           = {read=exporta.cabo,        write=exporta.cabo};
      __property bool ExportaSuporte        = {read=exporta.suporte,     write=exporta.suporte};
      __property bool ExportaRede           = {read=exporta.rede,        write=exporta.rede};
      __property bool ExportaReducao        = {read=exporta.reducao,     write=exporta.reducao};
      __property bool ExportaTipoChave      = {read=exporta.tipochave,   write=exporta.tipochave};
      __property bool ExportaTipoConsumidor = {read=exporta.tipocons,    write=exporta.tipocons};
      __property bool ExportaTipoRede       = {read=exporta.tiporede,    write=exporta.tiporede};
      __property bool Criptografar          = {read=exporta.criptografar,write=exporta.criptografar};
      __property char Separador             = {read=exporta.separador,   write=exporta.separador};

   public:
                   __fastcall  VTExportaTXT(void) {};
      virtual      __fastcall ~VTExportaTXT(void) {};
      virtual bool __fastcall  ExportaArranjos(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ExportaCurvasTipicas(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ExportaEqptos(TList *lisEXT, AnsiString arq_txt) = 0;
      virtual bool __fastcall  ExportaRedes(AnsiString arq_txt) = 0;
      virtual bool __fastcall  ExportaUmaRede(VTRede *rede, AnsiString arq_txt) = 0;

   protected:
      struct   {
               bool curvatipica;
               bool arranjo;
               bool cabo;
               bool suporte;
               bool rede;
               bool reducao;
               bool tipochave;
               bool tipocons;
               bool tiporede;
               bool criptografar;
               char separador;
               }exporta;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTExportaTXT
//-----------------------------------------------------------------------------
VTExportaTXT* NewObjExportaTXT(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
