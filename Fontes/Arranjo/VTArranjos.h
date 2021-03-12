//---------------------------------------------------------------------------
#ifndef VTArranjosH
#define VTArranjosH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Rede\VTEqpto.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTArranjo;
class VTCabo;
class VTSuporte;

//---------------------------------------------------------------------------
class VTArranjos : public VTEqpto
   {
   public:
                         __fastcall  VTArranjos(void) {};
      virtual            __fastcall ~VTArranjos(void) {};
      virtual void       __fastcall  Clear(void) = 0;
      virtual bool       __fastcall  ExisteAlteracao(void) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjo(int arranjo_id) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjo(AnsiString codigo, int tipo_cfg=-1) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjoCABOxFASE(VTSuporte *suporte, VTCabo *caboA,
                                                            VTCabo *caboB, VTCabo *caboC, VTCabo *caboN) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjoCABO_Z0Z1(VTCabo *cabo, int fases=faseINV) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjoRamal(AnsiString codigo, int fases) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjoZ0Z1(double r0, double x0, double c0,
                                                       double r1, double x1, double c1,
                                                       int fases=faseINV) = 0;
      virtual VTArranjo* __fastcall  ExisteArranjoExternId(AnsiString extern_id) = 0;
      virtual VTCabo*    __fastcall  ExisteCabo(int cabo_id) = 0;
      virtual VTCabo*    __fastcall  ExisteCabo(AnsiString codigo, int tipo_cfg=-1) = 0;
      virtual VTCabo*    __fastcall  ExisteCaboExternId(AnsiString extern_id) = 0;
      virtual VTSuporte* __fastcall  ExisteSuporte(int suporte_id) = 0;
      virtual VTSuporte* __fastcall  ExisteSuporte(AnsiString codigo) = 0;
      virtual VTSuporte* __fastcall  ExisteSuporteExternId(AnsiString extern_id) = 0;
      virtual bool       __fastcall  InsereArranjo(VTArranjo *arranjo) = 0;
      virtual bool       __fastcall  InsereCabo(VTCabo *cabo) = 0;
      virtual bool       __fastcall  InsereSuporte(VTSuporte *suporte) = 0;
      virtual TList*     __fastcall  LisArranjo(void) = 0;
      virtual TList*     __fastcall  LisCabo(void) = 0;
      virtual TList*     __fastcall  LisSuporte(void) = 0;
      virtual void       __fastcall  ReiniciaControleExternId(void) = 0;
   };                                
                                     
//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTArranjos
//---------------------------------------------------------------------------
VTArranjos* __fastcall NewObjArranjos(void);

//---------------------------------------------------------------------------
#endif
//eof


