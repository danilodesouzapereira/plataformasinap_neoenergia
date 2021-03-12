//---------------------------------------------------------------------------
#ifndef TPtoFixacaoH
#define TPtoFixacaoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTPtoFixacao.h"

//---------------------------------------------------------------------------
class TPtoFixacao : public VTPtoFixacao
   {
   public: //métodos
                    __fastcall TPtoFixacao(void);
                    __fastcall ~TPtoFixacao(void);
      VTPtoFixacao* __fastcall Clone(void);
		void          __fastcall CopiaAtributosDe(VTPtoFixacao &ref);
   };

//---------------------------------------------------------------------------
#endif
//eof

