//---------------------------------------------------------------------------
#ifndef VTEtH
#define VTEtH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTSuprimento;
//class VTPerfilSup;

//---------------------------------------------------------------------------
class VTEt : public TObject
   {
   public:
                   __fastcall  VTEt(void) {};
                   __fastcall ~VTEt(void) {};
      virtual void __fastcall  MontaPerfilDemandaFinal(TList *lisCARGA, VTBarra *barra_ini, VTBarra *barra_car) = 0;
      virtual void __fastcall  MontaPerfilDemandaInicial(TList *lisCARGA) = 0;
      virtual void __fastcall  MontaPerfilTensaoSuprimento(VTSuprimento *suprimento, VTBarra *barra_ini, VTBarra *barra_car) = 0;
   };

//---------------------------------------------------------------------------
VTEt* __fastcall NewObjEt(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof

