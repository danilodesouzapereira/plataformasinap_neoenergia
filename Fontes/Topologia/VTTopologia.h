//---------------------------------------------------------------------------
#ifndef VTTopologiaH
#define VTTopologiaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTChave;
class VTApl;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class VTTopologia : public TObject
   {
   public:
                   __fastcall  VTTopologia(void) {};
                   __fastcall ~VTTopologia(void) {};
		virtual void __fastcall  ClassificaRedes(VTRedes *redes=NULL) = 0;
		virtual bool __fastcall  Rebuild(VTRedes *redes, TList *lisREDE, TList *lisBARRA, TList *lisLIGACAO) = 0;
		virtual bool __fastcall  Reconfigura(VTRedes *redes=NULL) = 0;
		virtual bool __fastcall  ReconfiguraRede(VTRedes *redes, TList *lisREDE) = 0;
		virtual bool __fastcall  ReconfiguraRede(VTRedes *redes, VTRede *rede) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTTopologia
//---------------------------------------------------------------------------
VTTopologia* __fastcall NewObjTopologia(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 