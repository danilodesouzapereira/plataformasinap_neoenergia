//-----------------------------------------------------------------------------
#ifndef VTCria_H
#define VTCria_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMonta.h"

//-----------------------------------------------------------------------------
class VTApl;

//-----------------------------------------------------------------------------
class VTCria : public TObject
   {
   public:
                         __fastcall  VTCria(void) {};
      virtual            __fastcall ~VTCria(void) {};
      virtual void       __fastcall  DefLisEqptoObra(TList *lisEXT)=0;
      virtual VTArranjo* __fastcall  NewObjArranjo(strARRANJO2 &str)=0;
      virtual VTEqpto*   __fastcall  NewObjBarra(strBARRA &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCabo(strCABO &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCanal(strCANAL &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCapacitor(strCAPACITOR &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCapserie(strCAPSERIE &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCarga(strCARGA &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCargaNL(strCARGANL &str)=0;
      virtual VTEqpto*   __fastcall  NewObjChave(strCHAVE &str)=0;
      virtual VTEqpto*   __fastcall  NewObjCluster(strCLUSTER &str)=0;
      virtual VTEqpto*   __fastcall  NewObjFiltro(strFILTRO &str)=0;
      virtual VTEqpto*   __fastcall  NewObjGerador(strGERADOR &str)=0;
      virtual VTEqpto*   __fastcall  NewObjMedidor(strMEDIDOR &str) = 0;
      virtual VTEqpto*   __fastcall  NewObjMutua(strMUTUA &str) = 0;
      virtual VTEqpto*   __fastcall  NewObjReator(strREATOR &str)=0;
      virtual VTEqpto*   __fastcall  NewObjRede(strREDE &str)=0;
      virtual VTEqpto*   __fastcall  NewObjRegulador(strREGULADOR &str)=0;
      virtual VTEqpto*   __fastcall  NewObjSuporte(strSUPORTE &str)=0;
      virtual VTEqpto*   __fastcall  NewObjSuprimento(strSUPRIMENTO &str)=0;
      virtual VTEqpto*   __fastcall  NewObjTrafo(strTRAFO &str)=0;
      virtual VTEqpto*   __fastcall  NewObjTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)=0;
      virtual VTEqpto*   __fastcall  NewObjTrafo3E(strTRAFO3E &str)=0;
      virtual VTEqpto*   __fastcall  NewObjTrafoZZ(strTRAFOZZ &str)=0;
      virtual VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, strARRANJO1 &str_arr)=0;
      virtual VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, strARRANJO2 &str_arr)=0;
      virtual VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, VTArranjo *arranjo)=0;
      virtual VTEqpto*   __fastcall  NewObjTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo)=0;
   };

//-----------------------------------------------------------------------------
// função global para criar objeto VTCria
//-----------------------------------------------------------------------------
VTCria* NewObjCria(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
