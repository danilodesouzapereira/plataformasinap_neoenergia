//---------------------------------------------------------------------------
#ifndef TAnimacaoH
#define TAnimacaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAnimacao.h"

//---------------------------------------------------------------------------
class TAnimacao : public VTAnimacao
   {
   public:		// User declarations
           __fastcall TAnimacao(void);
           __fastcall ~TAnimacao(void);
      void __fastcall Start(VTUnif *unif);
      void __fastcall Stop(void);

   protected: //métodos acessados via property
      bool __fastcall PM_GetEnabled(void);
      void __fastcall PM_SetEnabled(bool enabled);

   private: //métodos
      void __fastcall OnTimerTick(TObject *Sender);

   private: //dados externos
      VTUnif *unif;

   private: //dados locais
      bool drawing;
      struct   {
               bool enabled;
               } PD;
      TTimer *timer;
   };

//---------------------------------------------------------------------------
#endif
//eof
