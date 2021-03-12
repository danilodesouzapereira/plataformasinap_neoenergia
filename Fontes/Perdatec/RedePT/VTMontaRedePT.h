//---------------------------------------------------------------------------
#ifndef VTMontaRedePTH
#define VTMontaRedePTH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTRede;
class VTTrafo;
class VTRedePT;
class PERDA_MEDIDOR;
class PERDA_RAMAL;
class PERDA_TRAFO;
class PERDA_TRAFO_SED;


//---------------------------------------------------------------------------
class VTMontaRedePT : public TObject
   {
   public:
                              __fastcall  VTMontaRedePT(void) {};
      virtual                 __fastcall ~VTMontaRedePT(void) {};
      virtual void            __fastcall  Clear(void) = 0;
      virtual bool            __fastcall  Executa(void) = 0;
      virtual bool            __fastcall  DeterminaPerdaMedidor(VTCarga *carga, double &p_w) = 0;
      virtual PERDA_MEDIDOR*  __fastcall  ExistePerdaMedidor(VTCarga *carga) = 0;
      virtual PERDA_RAMAL*    __fastcall  ExistePerdaRamal(VTCarga *carga) = 0;
      virtual PERDA_TRAFO*    __fastcall  ExistePerdaTrafo(VTTrafo *trafo) = 0;
      virtual PERDA_TRAFO_SED*__fastcall  ExistePerdaTrafoSED(VTTrafo *trafo) = 0;
      virtual VTRedePT*       __fastcall  ExisteRedePT(VTRede *rede) = 0;
      //virtual bool            __fastcall  RedeAlterada(void) = 0;
      //virtual bool            __fastcall  RetiraEqptosRedePT(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTMontaRedePT
//---------------------------------------------------------------------------
VTMontaRedePT* __fastcall NewObjMontaRedePT(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 