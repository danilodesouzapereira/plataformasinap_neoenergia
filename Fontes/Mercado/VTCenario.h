//---------------------------------------------------------------------------
#ifndef VTCenarioH
#define VTCenarioH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTCenario : public TObject
   {
   public:  //property
		__property AnsiString Codigo   = {read=PD.codigo,   write=PD.codigo};
      __property int        Id       = {read=PD.id,       write=PD.id};
      __property float      Taxa     = {read=PD.taxa,     write=PD.taxa};

   public:  //métodos
               __fastcall  VTCenario(void) {};
      virtual  __fastcall ~VTCenario(void) {};

   protected:  //dados acessados via properpy
      struct   {
               AnsiString codigo;
               int        id;
               float      taxa;
               }PD;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCenario
//---------------------------------------------------------------------------
//VTCenario* __fastcall NewObjCenario(void);

//---------------------------------------------------------------------------
#endif
//eof
