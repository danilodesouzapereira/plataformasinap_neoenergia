//---------------------------------------------------------------------------
#ifndef TFluxoG3H
#define TFluxoG3H
using namespace std;

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <complex>
#include "..\Fluxo\VTFluxo.h"

//---------------------------------------------------------------------------
class VTData;
class VTGauss3;

//---------------------------------------------------------------------------
class TFluxoG3 : public VTFluxo
   {
   public:
                         __fastcall  TFluxoG3(VTApl *apl, bool g3_as_nr3);
      virtual            __fastcall ~TFluxoG3(void);
      virtual void       __fastcall  CfgAvisoErro(bool enabled);
      virtual bool       __fastcall  Executa(VTNet *net, double Sbase,
                                             AnsiString str1, AnsiString str2);
      virtual AnsiString __fastcall Modelo(void);
      virtual bool       __fastcall UtilizaBarraPV(void);
      virtual bool       __fastcall ValidaAderencia(VTNet *net, VTLog *plog);

   protected:  //métodos acessados via property
              bool __fastcall  PM_GetEquilibrado(void);

   protected: //objetos externos
      VTApl *apl;

   protected: //dados locais
      bool      utiliza_barraPV;
      int       ano;
      double    Sbase;
      VTGauss3  *gauss3;
      VTData    *data;
      TList     *lisEQP;
   };

#endif
//---------------------------------------------------------------------------
//eof
