// --------------------------------------------------------------------------------------------- bof
#ifndef VTProgressoH
#define VTProgressoH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------ define
#define MAX_STEP_CARREGA_ESTUDO       54
#define MAX_STEP_CARREGA_MREDE_ESTUDO 34
#define MAX_STEP_SALVA_ESTUDO         11
#define MAX_STEP_SALVA_REDE_ESTUDO    41

//-----------------------------------------------------------------------------
#define  PROG_MAX_MSG_LEN   1024
#define  PROG_PORT          9666

enum tipoPROGRESSO {progNENHUM=-1, progDEFAULT=1, progTEXTO, progTEXTO_BP};

// ------------------------------------------------------------------------------------------- class
class VTApl;

// ------------------------------------------------------------------------------------------- class
class VTProgresso  : public TObject
   {
   public:  //property
      __property int ServerPort = {read=PM_GetServerPort, write=PM_SetServerPort};

   public:
                   __fastcall  VTProgresso(void) {};          // {}: classes derivadas NÃO precisam
	  virtual      __fastcall ~VTProgresso(void) {};
	  virtual void __fastcall  Add(AnsiString texto, bool clear=false) = 0;
     virtual void __fastcall  Add(AnsiString texto, int pos, int max, bool clear=false) = 0;
	  virtual void __fastcall  Start(int tipo_progresso) = 0;
	  virtual void __fastcall  Stop(void)                = 0;
//jss20191003
	  virtual void __fastcall  NoAutoInc(void)           = 0;
	  virtual bool __fastcall  MaxStep(int max)          = 0; // =0: classes derivadas devem
	  virtual void __fastcall  Step(void)                = 0;

   protected:
      virtual int  __fastcall  PM_GetServerPort(void) = 0;
      virtual void __fastcall  PM_SetServerPort(int port) = 0;
   };

// função para criar objeto VTProgresso
VTProgresso* __fastcall NewObjProgresso(VTApl *apl=NULL);

#endif
// --------------------------------------------------------------------------------------------- eof