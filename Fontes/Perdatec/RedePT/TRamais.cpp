//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TRamais.h"
#include "..\..\Fontes\Apl\VTApl.h"
#include "..\..\Fontes\Arranjo\VTArranjo.h"
#include "..\..\Fontes\Arranjo\VTArranjos.h"
#include "..\..\..\DLL_Inc\Arranjo.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRamais* __fastcall NewObjRamais(VTApl *apl)
   {
   try{
      return(new TRamais(apl));
      }catch(Exception &e)
         {
         }
   return(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TRamais::TRamais(VTApl *apl)
   {
   //salva ponteiro p/ objeto
   this->apl = apl;
   //cria listas
   lisARRANJO = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TRamais::~TRamais(void)
   {
   //destrói lista e seus objetos
   if (lisARRANJO) {LimpaTList(lisARRANJO); delete lisARRANJO; lisARRANJO = NULL;}
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TRamais::ArranjoRamal(double r_ohm_km, int fases)
   {
   //variáveis locais
   VTArranjo  *arranjo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));


   try{//verifica se já existe o Arranjo
      if ((arranjo = ExisteArranjo(r_ohm_km, fases)) == NULL)
         {//cria um novo Arranjo e insere em lisARRANJO
         lisARRANJO->Add(arranjo = DLL_NewObjArranjo());
         arranjo->Id       = FalsoId();
         arranjo->Owner    = arranjos;
         arranjo->TipoCfg  = arranjoZ0Z1;
         arranjo->Fases    = fases;
         arranjo->Iadm_amp = 500;
         arranjo->z1.r     = r_ohm_km;
         arranjo->z1.x     = 0.;
         arranjo->c1       = 0.;
         arranjo->z0       = arranjo->z1;
         arranjo->c0       = arranjo->c1;
         arranjo->suporte  = NULL;
         arranjo->caboZ0Z1 = NULL;
         //IMPORTANTE: define como sendo ramal
         arranjo->ramal = true;
         }
      }catch(Exception &e)
         {
         arranjo = NULL;
         }
   return(arranjo);
   }

//---------------------------------------------------------------------------
VTArranjo* __fastcall TRamais::ExisteArranjo(double r_ohm_km, int fases)
   {
   //variáveis locais
   VTArranjo *arranjo;

   //procura Arranjo com resistência e fases indicadas
   for (int n = 0; n < lisARRANJO->Count; n++)
      {
      arranjo = (VTArranjo*)lisARRANJO->Items[n];
      if (arranjo->Fases == fases)
         {
         if (IsDoubleZero(arranjo->z1.r - r_ohm_km)) return(arranjo);
         }
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
//eof



