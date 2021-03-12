//---------------------------------------------------------------------------
#ifndef TGeraRedeH
#define TGeraRedeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <stdio.h>
#include "VTGeraRede.h"

//---------------------------------------------------------------------------
class TEqptoLiga;
//---------------------------------------------------------------------------
class TGeraRede : public VTGeraRede
   {
   public:
               __fastcall TGeraRede(VTApl* apl);
               __fastcall ~TGeraRede(void);
		  bool   __fastcall Executa(TList* lisLiga, TList *lisFora);
		  bool   __fastcall ExecutaBlocos(TList* lisLiga, VTBlocos* blocos);
	private: //métodos
		  //bool   __fastcall FiltroEq(TEqptoLiga* eqliga);//, TList* lisFontes, TList* lisCargas);//hk12 , TList* lisLiga);
   private: //dados externos
        VTApl* apl;

   private: //dados locais
        TList* lisAux;
        TList* lisEqLiga;
        TList* lisLigaS;  //ligações novas - série de eqptos
   public:
   };
#endif
//---------------------------------------------------------------------------
//eof

