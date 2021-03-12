//---------------------------------------------------------------------------
#ifndef VTCampoH
#define VTCampoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCampo : public TObject
   {
   public:  //property
      __property bool       AsBoolean  = {read=PM_GetAsBoolean,  write=PM_SetAsBoolean};
      __property double     AsDouble   = {read=PM_GetAsDouble,   write=PM_SetAsDouble};
      __property int        AsInteger  = {read=PM_GetAsInteger,  write=PM_SetAsInteger};
      __property AnsiString AsString   = {read=PM_GetAsString,   write=PM_SetAsString};
      __property TDateTime  AsDate     = {read=PM_GetAsDate,     write=PM_SetAsDate};
      __property AnsiString Nome       = {read=PM_GetNome};

   public:
              __fastcall VTCampo(void) {};
      virtual __fastcall ~VTCampo(void) {};

   protected:   //métodos acessados via property
      virtual bool       __fastcall PM_GetAsBoolean(void) = 0;
      virtual TDate      __fastcall PM_GetAsDate(void) = 0;
      virtual double     __fastcall PM_GetAsDouble(void) = 0;
      virtual int        __fastcall PM_GetAsInteger(void) = 0;
      virtual AnsiString __fastcall PM_GetAsString(void) = 0;
      virtual AnsiString __fastcall PM_GetNome(void) = 0;
      virtual void       __fastcall PM_SetAsBoolean(bool valor) = 0;
      virtual void       __fastcall PM_SetAsDate(TDate valor) = 0;
      virtual void       __fastcall PM_SetAsDouble(double valor) = 0;
      virtual void       __fastcall PM_SetAsInteger(int valor) = 0;
      virtual void       __fastcall PM_SetAsString(AnsiString valor) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTCampo* __fastcall NewObjCampo(AnsiString nome);

#endif
//---------------------------------------------------------------------------//eof

