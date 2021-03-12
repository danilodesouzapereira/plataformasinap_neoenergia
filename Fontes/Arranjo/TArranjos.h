//---------------------------------------------------------------------------
#ifndef TArranjosH
#define TArranjosH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTArranjos.h"

//---------------------------------------------------------------------------
class TArranjos : public VTArranjos
   {
   public:
                 __fastcall  TArranjos(void);
                 __fastcall ~TArranjos(void);
      void       __fastcall  Clear(void);
      bool       __fastcall  ExisteAlteracao(void);
      VTArranjo* __fastcall  ExisteArranjo(int arranjo_id);
      VTArranjo* __fastcall  ExisteArranjo(AnsiString codigo, int tipo_cfg);
      VTArranjo* __fastcall  ExisteArranjoCABOxFASE(VTSuporte *suporte, VTCabo *caboA,
                                                    VTCabo *caboB, VTCabo *caboC, VTCabo *caboN);
      VTArranjo* __fastcall  ExisteArranjoCABO_Z0Z1(VTCabo *cabo, int fases);
      VTArranjo* __fastcall  ExisteArranjoRamal(AnsiString codigo, int fases);
      VTArranjo* __fastcall  ExisteArranjoZ0Z1(double r0, double x0, double c0,
                                               double r1, double x1, double c1, int fases=faseINV);
      VTArranjo* __fastcall  ExisteArranjoExternId(AnsiString extern_id);
      VTCabo*    __fastcall  ExisteCabo(int cabo_id);
      VTCabo*    __fastcall  ExisteCabo(AnsiString codigo, int tipo_cfg);
      VTCabo*    __fastcall  ExisteCaboExternId(AnsiString extern_id);
      VTSuporte* __fastcall  ExisteSuporte(int suporte_id);
      VTSuporte* __fastcall  ExisteSuporte(AnsiString codigo);
      VTSuporte* __fastcall  ExisteSuporteExternId(AnsiString extern_id);
      bool       __fastcall  InsereArranjo(VTArranjo *arranjo);
      bool       __fastcall  InsereCabo(VTCabo *cabo);
      bool       __fastcall  InsereSuporte(VTSuporte *suporte);
      TList*     __fastcall  LisArranjo(void);
      TList*     __fastcall  LisCabo(void);
      TList*     __fastcall  LisSuporte(void);
      void       __fastcall  ReiniciaControleExternId(void);
      int        __fastcall  Tipo(void);
      AnsiString __fastcall  TipoAsString(void);

   private: //dados locais
      TList  *lisARR;
      TList  *lisCAB;
      TList  *lisSUP;
      //lista de objetos duplicados (código igual mas ID diferente)
      struct{
            TList  *lisARR;
            TList  *lisCAB;
            TList  *lisSUP;
            }dup;
   };

//---------------------------------------------------------------------------
#endif
//eof

