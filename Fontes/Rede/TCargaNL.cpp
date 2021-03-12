//---------------------------------------------------------------------------
#include <vcl.h>
#include <math.h>
#pragma hdrstop
#include "TCargaNL.h"
#include "VTFases.h"
#include "..\Classes\VTClasse.h"
#include "..\Curvas\VTCurva.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTCargaNL* __fastcall NewObjCargaNL(void)
   {
   return(new TCargaNL());
   }
   
//---------------------------------------------------------------------------
__fastcall TCargaNL::TCargaNL(void)
   {
   //valores default
   indutiva  = true;
   fatpot_pu = 1.0;
   //inicia número de harmônicas
   harmonica.Length = 0;
   }

//---------------------------------------------------------------------------
__fastcall TCargaNL::~TCargaNL(void)
   {
   //libera memória
   harmonica.Length = 0;
   }

//---------------------------------------------------------------------------
VTCargaNL* __fastcall TCargaNL::Clone(void)
   {
   //variáveis locais
   VTCargaNL *carga;

   //cria uma nova CargaNL
   if ((carga = NewObjCargaNL()) != NULL)
      {//copia dados desta CargaNL p/ a nova CargaNL
      carga->CopiaAtributosDe(*this);
      }
   return(carga);
   }

//---------------------------------------------------------------------------
void __fastcall TCargaNL::CopiaAtributosDe(VTEqpto &ref)
	{
	//variáveis locais
	TCargaNL &carga = (TCargaNL&)ref;

	//proteção
	if (ref.Tipo() != eqptoCARGANL) return;
	//copia atributos de TEqbar
	TEqbar::CopiaAtributosDe(carga);
	//copia atributos da Carga não linear
	fatpot_pu = carga.fatpot_pu;
	indutiva  = carga.indutiva;
	//copia dados da fundamental
	fundamental = carga.fundamental;
	//copia dados das harmônicas
	harmonica.Length = carga.harmonica.Length;
	for (int n = 0; n < harmonica.Length; n++)
		{
		harmonica[n] = carga.harmonica[n];
		}
	}

//---------------------------------------------------------------------------
bool __fastcall TCargaNL::DefineFundamental(strFUN *str_fun)
   {
   // Verifica o ponteiro passado
   if (!str_fun) return(false);
   //salva tipo e fator de pot.
   Indutiva  = str_fun->indutiva;
   FatPot_pu = str_fun->fatpot_pu;
   //copia odados da fundamental
   fundamental.sa_mod_kva  = str_fun->sa_mod_kva;
   fundamental.sa_ang_grau = str_fun->sa_ang_grau;
   fundamental.sb_mod_kva  = str_fun->sb_mod_kva;
   fundamental.sb_ang_grau = str_fun->sb_ang_grau;
   fundamental.sc_mod_kva  = str_fun->sc_mod_kva;
   fundamental.sc_ang_grau = str_fun->sc_ang_grau;

   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TCargaNL::InsereHarmonica(strHRM *str_hrm)
   {
   int ndxNew;

   // Verifica o ponteiro passado
   if (!str_hrm) return(false);
   // Cria um novo elemento
   harmonica.Length++;
   ndxNew = harmonica.Length - 1;
   // Copia os valores
   harmonica[ndxNew].ordem       = str_hrm->ordem;
   harmonica[ndxNew].sa_mod_pu   = str_hrm->sa_mod_pu;
   harmonica[ndxNew].sa_ang_grau = str_hrm->sa_ang_grau;
   harmonica[ndxNew].sb_mod_pu   = str_hrm->sb_mod_pu;
   harmonica[ndxNew].sb_ang_grau = str_hrm->sb_ang_grau;
   harmonica[ndxNew].sc_mod_pu   = str_hrm->sc_mod_pu;
   harmonica[ndxNew].sc_ang_grau = str_hrm->sc_ang_grau;

   return(true);
   }

//---------------------------------------------------------------------------
strFUN* __fastcall TCargaNL::LeFundamental(void)
   {
   return(&fundamental);
   }

//---------------------------------------------------------------------------
strHRM* __fastcall TCargaNL::LeHarmonica(int index)
   {
   if (index < 0)                 return(NULL);
   if (index >= harmonica.Length) return(NULL);

   return(&(harmonica[index]));
   }

//---------------------------------------------------------------------------
double __fastcall TCargaNL::PM_GetFatPot_pu(void)
   {
   return(fatpot_pu);
   }

//---------------------------------------------------------------------------
bool __fastcall TCargaNL::PM_GetIndutiva(void)
   {
   return(indutiva);
   }

//---------------------------------------------------------------------------
int __fastcall TCargaNL::PM_GetNumHarmonicas(void)
   {
   return(harmonica.Length);
   }

//---------------------------------------------------------------------------
void __fastcall TCargaNL::PM_SetFatPot_pu(double fatpot_pu)
   {
   this->fatpot_pu = fatpot_pu;
   }

//---------------------------------------------------------------------------
void __fastcall TCargaNL::PM_SetIndutiva(bool indutiva)
   {
   this->indutiva = indutiva;
   }

//---------------------------------------------------------------------------
int __fastcall TCargaNL::Tipo(void)
   {
   return(eqptoCARGANL);
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TCargaNL::TipoAsString(void)
   {
   return("Carga Harmônica");
   }

//---------------------------------------------------------------------------
//eof
