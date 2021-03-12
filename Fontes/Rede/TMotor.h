//---------------------------------------------------------------------------
#ifndef TMotorH
#define TMotorH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMotor.h"

//---------------------------------------------------------------------------
class TMotor : public VTMotor
   {
   public:
                   __fastcall  TMotor(void);
                   __fastcall ~TMotor(void);
      VTMotor*     __fastcall  Clone(void);
		void         __fastcall  CopiaAtributosDe(VTEqpto &ref);
		void         __fastcall  DefineCurva(VTCurva *curva);
		VTCurva*     __fastcall  ExisteCurva(void);
		int          __fastcall  Tipo(void);
      AnsiString   __fastcall  TipoAsString(void);

   protected:  //m�todos acessados via property
      AnsiString __fastcall PM_GetTipoMotorAsString(void);

   private: //m�todos acessados via property
      AnsiString  __fastcall PM_GetPadrao(void);
      void        __fastcall PM_SetPadrao(AnsiString padrao);

   private:
      AnsiString padrao;
   };

//---------------------------------------------------------------------------
#endif
//eof

 