//-----------------------------------------------------------------------------
#ifndef TAlertaH
#define TAlertaH

//-----------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include <map>
#include "VTAlerta.h"

//---------------------------------------------------------------------------
class TAlerta : public VTAlerta
   {
   public:
           __fastcall  TAlerta(VTApl *apl_owner);
           __fastcall ~TAlerta(void);
	  int  __fastcall  Add(AnsiString msg);
	  int  __fastcall  AddMsgMalha(VTRede *rede, AnsiString msg);
		void __fastcall  Clear(bool completo = false);
	  AnsiString __fastcall ExisteAvisoMalha(VTRede *rede);
		int  __fastcall  NumeroDeMensagens(void);
	  void __fastcall  ReexibeAlertas(bool somenteMalhas);
      void __fastcall  SaveToFile(AnsiString filename);
      void __fastcall  ShowModal(void);

   private: //métodos

   private: //objetos externos
      VTApl *apl;

   private: //dados
	  TStrings *lines;
	  TStrings *lines_aux;
      std::map<int, AnsiString> mapMsgMalha;
   };

#endif
//---------------------------------------------------------------------------
//eof
