//---------------------------------------------------------------------------
#ifndef VTBancosH
#define VTBancosH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBanco;
class VTBarra;
class VTCapacitor;

//---------------------------------------------------------------------------
class VTBancos : public TObject
   {
   public:
							  __fastcall  VTBancos(void) {};
      virtual          __fastcall ~VTBancos(void) {};
		virtual VTBanco* __fastcall  ExisteBanco(VTBarra *barra) = 0;
      virtual VTBanco* __fastcall  ExisteBanco(VTCapacitor *capacitor) = 0; //FKM 2015.09.24
		virtual VTBanco* __fastcall  InsereBanco(VTBarra *barra) = 0;
		virtual TList*   __fastcall  LisBanco(void) = 0;
		virtual int      __fastcall  LisCapacitor(TList *lisEXT) = 0;
		virtual int      __fastcall  LisCloneCapacitor(TList *lisEXT) = 0;
		virtual int      __fastcall  NumBanco(void) = 0;
		virtual double   __fastcall  Qtotal_mvar(void) = 0;
		virtual void     __fastcall  Reinicia(void) = 0;
      virtual bool     __fastcall  RetiraCapacitor(VTCapacitor *capacitor) = 0; //FKM 2015.09.24
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTBancos* __fastcall NewObjBancos(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

