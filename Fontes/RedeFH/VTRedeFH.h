//---------------------------------------------------------------------------
#ifndef VTRedeFH_H
#define VTRedeFH_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
//#include "..\Rede\VTLigacao.h"

//---------------------------------------------------------------------------
class  VTApl;
class  VTBarra;
class  VTEqpto;
class  VTNet;
class  VTNo;
class  VTRedes;
class  VTBarFH;
class  VTLigFH;

//---------------------------------------------------------------------------
class VTRedeFH : public TObject
   {
   public:  //property
      __property int OrdemHarmonica[int ind_freq] = {read=PM_GetOrdemHarmonica,   write=PM_SetOrdemHarmonica};
      __property int NumHarmonicas                = {read=PM_GetNumHarmonicas,    write=PM_SetNumHarmonicas};

   public:
                        __fastcall  VTRedeFH(void) {};
      virtual           __fastcall ~VTRedeFH(void) {};
      virtual VTBarFH*  __fastcall  ExisteBarFH(VTEqpto *eqpto) = 0;
      virtual VTLigFH*  __fastcall  ExisteLigFH(VTEqpto *eqpto) = 0;
      virtual bool      __fastcall  Inicia(VTRedes *redes,
                                           VTNet   *net,
                                           int     num_pat,
                                           int     num_harmonicas,
                                           int     *vet_ordem_harmonicas) = 0;
      virtual TList*    __fastcall  LisBarFH(void) = 0;
      virtual TList*    __fastcall  LisBarFH(VTNo *no) = 0;
      virtual TList*    __fastcall  LisLigFH(void) = 0;
      virtual TList*    __fastcall  LisLigFH_Ficticia(void) = 0;

   protected:  //métodos acessados via property
      virtual int       __fastcall  PM_GetOrdemHarmonica(int ind_freq) = 0;
      virtual int       __fastcall  PM_GetNumHarmonicas(void) = 0;
      //
      virtual void      __fastcall  PM_SetOrdemHarmonica(int ind_freq, int ordem) = 0;
      virtual void      __fastcall  PM_SetNumHarmonicas(int num_harmonicas) = 0;
   };

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTRedeFH
//-----------------------------------------------------------------------------
VTRedeFH* __fastcall NewObjRedeFH(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
