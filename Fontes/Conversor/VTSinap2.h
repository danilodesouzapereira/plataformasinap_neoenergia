//---------------------------------------------------------------------------
#ifndef VTSinap2H
#define VTSinap2H

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class VTSinap2 : public TObject
   {
   public:
						  __fastcall  VTSinap2(void) {};
      virtual       __fastcall ~VTSinap2(void) {};
      virtual bool  __fastcall  AbreRedeCompleta(AnsiString dir_base, AnsiString dir_rede) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTSinap2* __fastcall NewObjSinap2(TComponent *Owner, VTApl *apl_owner);

#endif
//---------------------------------------------------------------------------
//eof

