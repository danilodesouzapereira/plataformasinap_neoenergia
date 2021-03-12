//---------------------------------------------------------------------------
#ifndef TValPerdafeH
#define TValPerdafeH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTValPerdafe.h"

//---------------------------------------------------------------------------
class VTRadial;
class VTPrimario;
class VTSecundario;
class VTBarra;
class VTCarga;
class VTLigacao;
class VTTrafo;
class VTTrecho;
class VTFases;

//---------------------------------------------------------------------------
class TValPerdafe : public VTValPerdafe
   {
   public:
              __fastcall  TValPerdafe(VTApl *apl);
              __fastcall ~TValPerdafe(void);
      bool    __fastcall  Executa(void);
      int     __fastcall  ExisteTrafoComPerdaFeInvalida(void);
      TList*  __fastcall  LisTrafoComPerdaFeInvalida(void);

   private: //métodos

   private: //objetos externos
      VTApl   *apl;

   private: //dados locais
      TList  *lisEQP;
      TList  *lisTRF;
   };

#endif
//-----------------------------------------------------------------------------
// eof
