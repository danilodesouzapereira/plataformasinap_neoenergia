//-----------------------------------------------------------------------------
#ifndef VTTema_H
#define VTTema_H

//arquivos incluídos ----------------------------------------------------------
#include <Classes.hpp>

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTTema : public TObject
   {
  public:  //property
      __property AnsiString Codigo = {read=PD.codigo, write=PD.codigo};
      __property TColor     Color  = {read=PD.color , write=PD.color };

   public:
                   __fastcall  VTTema(void) {};
                   __fastcall ~VTTema(void){};
      virtual void __fastcall  DefineCorCelula(void) = 0;
      virtual void __fastcall  Reinicia(int ind_pat) = 0;


   protected:  //dados acessados via property
      struct   {
               AnsiString codigo;
               TColor     color;
               }PD;
    };

//-----------------------------------------------------------------------------
//função global
VTTema* __fastcall NewObjTema(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
