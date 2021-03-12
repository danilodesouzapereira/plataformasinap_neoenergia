//---------------------------------------------------------------------------
#ifndef VTExportaKmlH
#define VTExportaKmlH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRede;

//---------------------------------------------------------------------------
class VTExportaKml : public TObject
   {
   public:
                   __fastcall  VTExportaKml(void) {};
                   __fastcall ~VTExportaKml(void) {};
      virtual bool __fastcall  Executa(TList *lisREDE, AnsiString arq_kml) = 0;
      virtual bool __fastcall  Executa(VTRede *rede, AnsiString arq_kml) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTExportaKml
//---------------------------------------------------------------------------
VTExportaKml* __fastcall NewObjExportaKml(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 