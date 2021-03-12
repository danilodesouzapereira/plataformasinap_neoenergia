//---------------------------------------------------------------------------
#ifndef TMalhasH
#define TMalhasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMalhas.h"

//---------------------------------------------------------------------------
class VTMalha;

//---------------------------------------------------------------------------
class TMalhas : public VTMalhas
	{
   private: //objetos externos
      VTApl   *apl;

   private: //dados locais
		TList  *lisMALHA_EXT;
		TList  *lisMALHA_GER;
		TList  *lisMALHA_INT;

   public:  //métodos
             __fastcall  TMalhas(VTApl *apl);
             __fastcall ~TMalhas(void);
		bool   __fastcall  Inicia(void);
		bool   __fastcall  InsereMalha(VTMalha *malha);
		bool   __fastcall  InsereMalhaGeracao(VTMalha *malha);
		TList* __fastcall  LisMalhaExterna(void);
		TList* __fastcall  LisMalhaGeracao(void);
		TList* __fastcall  LisMalhaInterna(void);
		bool   __fastcall  RemoveMalha(VTMalha *malha);
	};

//---------------------------------------------------------------------------
#endif
//eof
