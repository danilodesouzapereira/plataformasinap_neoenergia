//-----------------------------------------------------------------------------
#ifndef VTPreMonta_H
#define VTPreMonta_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTPreMonta : public TObject
   {
   public:
      __property int AreaFiltrada = {read=PD.area_filtrada, write=PD.area_filtrada};

   public:
                   __fastcall VTPreMonta(void) {};
      virtual      __fastcall ~VTPreMonta(void) {};
      virtual bool __fastcall InsereDBAR(TStringList *stList) = 0;
      virtual bool __fastcall InsereDBSH(TStringList *stList) = 0;
      virtual bool __fastcall InsereDGBA(TStringList *stList) = 0;
      virtual bool __fastcall InsereDGBT(TStringList *stList) = 0;
      virtual bool __fastcall InsereDINJ(TStringList *stList) = 0;
      virtual bool __fastcall InsereDLIN(TStringList *stList) = 0;
      virtual void __fastcall Reinicia(void) = 0;
      virtual bool __fastcall Sucesso(void) = 0;
      virtual bool __fastcall TrataFimDados(void) = 0;

   protected:
      struct{
            int area_filtrada;
            }PD;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTPreMonta
//-----------------------------------------------------------------------------
VTPreMonta* NewObjPreMonta(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
