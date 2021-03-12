//---------------------------------------------------------------------------
#ifndef VTZonasH
#define VTZonasH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTRedes;
class VTZona;

//---------------------------------------------------------------------------
class VTZonas : public TObject
   {
   public:  //property
     __property bool AvisoEnabled       = {read=PD.aviso_enabled,   write=PD.aviso_enabled};
     __property bool ChaveAbertaEnabled = {read=PD.chaveNA_enabled, write=PD.chaveNA_enabled};

   public:
							 __fastcall  VTZonas(void) {};
		virtual         __fastcall ~VTZonas(void) {};
		virtual void    __fastcall  Clear(VTRedes *redes=NULL) = 0;
		virtual bool    __fastcall  Executa(VTRedes *redes=NULL) = 0;
		virtual VTZona* __fastcall  ExisteZona(VTBarra *barra) = 0;
		virtual TList*  __fastcall  LisZona(void) = 0;

   protected:  //dados acessados via property
      struct{
            bool aviso_enabled;
            bool chaveNA_enabled;
            }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTZonas* NewObjZonas(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof
