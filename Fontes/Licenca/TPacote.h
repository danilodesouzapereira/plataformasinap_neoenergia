//---------------------------------------------------------------------------
#ifndef TPacoteH
#define TPacoteH

//arquivos inclu�dos---------------------------------------------------------
#include <Classes.hpp>
#include "VTPacote.h"

//---------------------------------------------------------------------------
class TPacote : public VTPacote
   {
   public :
               __fastcall TPacote(void);
               __fastcall ~TPacote(void);
      VTCampo* __fastcall Campo(AnsiString nome);
      void     __fastcall Clear(void);

   protected:  //m�todos acessados via propery
		AnsiString __fastcall PM_GetCamposAsStr(void);
		void       __fastcall PM_SetCamposAsStr(AnsiString txt);

   private: //m�todos
      VTCampo* __fastcall ExisteCampo(AnsiString nome);

   private: //dados locais
      TList *lisCAMPO;
   };

//---------------------------------------------------------------------------
#endif
//eof
