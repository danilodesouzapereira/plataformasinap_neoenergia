// --------------------------------------------------------------------------------------------- bof
#ifndef TProgressoH
#define TProgressoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>
#include "VTProgresso.h"

class TProg;
// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class
class TProgresso : public VTProgresso
   {
   public:
           __fastcall  TProgresso(void);
           __fastcall ~TProgresso(void);
	  void __fastcall  Add(AnsiString texto, bool clear);
     void __fastcall  Add(AnsiString texto, int pos, int max, bool clear=false);
	  void __fastcall  Start(int tipo_progresso);
	  void __fastcall  Stop(void);
//jss20191003
	  void __fastcall  NoAutoInc(void);
	  bool __fastcall  MaxStep(int max);
	  void __fastcall  Step(void);

   protected:
      int  __fastcall  PM_GetServerPort(void);
      void __fastcall  PM_SetServerPort(int port);

   private: //dados locais
      int    server_port;
      TProg  *prog;
   };

#endif
// --------------------------------------------------------------------------------------------- eof