//---------------------------------------------------------------------------
#ifndef VTLocalizaH
#define VTLocalizaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRedes;

//---------------------------------------------------------------------------
enum enumOPERADOR    {opINVALIDO=-1, opIGUAL=0, opDIFERENTE, opMAIOR, opMENOR};

//---------------------------------------------------------------------------
class VTLocaliza : public TObject
   {
   public:
                      __fastcall  VTLocaliza(void) {};
      virtual         __fastcall ~VTLocaliza(void) {};
      virtual TList*  __fastcall  LisEqpto(VTRedes *redes, int tipo_eqpto, int operador,
                                           int ind_atrib, AnsiString val_alvo, bool area_visivel=true) = 0;
      virtual TList*  __fastcall  LisEqptoErroGis(VTRedes *redes, int tipo_eqpto, bool area_visivel=true) = 0;

   };

//---------------------------------------------------------------------------
//função para criar objeto VTLocaliza
VTLocaliza* __fastcall NewObjLocaliza(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
