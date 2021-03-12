//-----------------------------------------------------------------------------
#ifndef VTExportaANEELH
#define VTExportaANEELH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;
class VTRede;

//-----------------------------------------------------------------------------
class VTExportaANEEL : public TObject
   {
   public:  //property
		__property char Separador = {read=exporta.separador,   write=exporta.separador};

   public:
                   __fastcall  VTExportaANEEL(void) {};
      virtual      __fastcall ~VTExportaANEEL(void) {};
      virtual bool __fastcall  ExportaRedes(AnsiString arq_txt, TDateTime data) = 0;

   protected:
      struct   {
               char separador;
               }exporta;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTExportaANEEL
//-----------------------------------------------------------------------------
VTExportaANEEL* NewObjExportaANEEL(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
