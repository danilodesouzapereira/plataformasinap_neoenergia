// --------------------------------------------------------------------------------------------- bof
#ifndef TProgH
#define TProgH

// ---------------------------------------------------------------------------------------- includes
#include <Classes.hpp>

class TFormDefault;
class TFormTexto;

// --------------------------------------------------------------------------------------- namespace

// ------------------------------------------------------------------------------------------- class

// -------------------------------------------------------------------------------------------------
class TProg : public TThread
   {
   public:
           __fastcall  TProg(int tipo_progresso);
           __fastcall ~TProg(void);
      void __fastcall  Terminate(void);

   private: //métodos
      void __fastcall Execute(void);

   public: //objetos externos
      TFormDefault  *FormDefault;
      TFormTexto    *FormTexto;

   public: //dados locais
      TEvent *event;
   };

#endif
// --------------------------------------------------------------------------------------------- eof