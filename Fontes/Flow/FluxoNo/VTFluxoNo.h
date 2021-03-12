//---------------------------------------------------------------------------
#ifndef VTFluxoNoH
#define VTFluxoNoH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTNet;

//---------------------------------------------------------------------------
class VTFluxoNo : public TObject
   {
   public:  //property
      __property int  ModeloCarga = {read=PD.modelo_carga,  write=PD.modelo_carga};

   public:
                   __fastcall VTFluxoNo(void) {};
                   __fastcall ~VTFluxoNo(void) {};
      virtual bool __fastcall Executa(VTNet *net, double Sbase) = 0;

   protected:  //dados acessados via property
      struct   {
               int modelo_carga;
               }PD;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFluxoNo
//---------------------------------------------------------------------------
VTFluxoNo* __fastcall NewObjFluxoNo(VTApl *apl);

#endif
//---------------------------------------------------------------------------
//eof

 