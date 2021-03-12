//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TServidorLocal.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\DirArq.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Micro\VTPendrive.h"
#include "..\Micro\VTPenFinder.h"
#include "..\..\DLL_Inc\BDlicenca.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Micro.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTServidor* __fastcall NewObjServidorLocal(void)
   {
   return(new TServidorLocal());
   }

//---------------------------------------------------------------------------
__fastcall TServidorLocal::TServidorLocal(void)
   {
   try{//cria objetos
      servidor  = NewObjServidor();
      penfinder = DLL_NewObjPenFinder();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TServidorLocal::~TServidorLocal(void)
   {
   //destr�i objetos
   if (servidor)  {delete servidor; servidor = NULL;}
   if (penfinder) {delete penfinder; penfinder = NULL;}
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::AlocaLicenca(VTLicenca *licenca, AnsiString &diagnostico)
   {
   //vari�veis locais
   VTPendrive *pendrive;

   //verifica se existe Pendrive servidor de licen�a
   if ((pendrive = ExisteServidorLicenca()) == NULL)
      {
      return(false);
      }
   //atualiza Licenca
   licenca->CtrlRemoto = false;
   //inicia conex�o com Servidor
   if (! Conecta(PathDatabase(pendrive))) return(false);
   //insere Licenca na base de dados
   servidor->AlocaLicenca(licenca, diagnostico);
   //apresenta diagn�stico
   //Aviso(licenca->Diagnostico());
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::Conecta(AnsiString database_name)
   {
   //inicia conex�o com base de dados
   return(servidor->Conecta(database_name));
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::ConsultaPendrive(VTPendrive *pendrive)
   {
   return(servidor->ConsultaPendrive(pendrive));
   }

//---------------------------------------------------------------------------
void __fastcall TServidorLocal::Desconecta(void)
   {
   servidor->Desconecta();
   }

//---------------------------------------------------------------------------
VTPendrive* __fastcall TServidorLocal::ExisteServidorLicenca(void)
   {
   //vari�veis locais
   AnsiString database_name;
   VTPendrive *pendrive;
   TList      *lisPEN;

   //obt�m lista de Pendrive instalados no micro
   lisPEN = penfinder->Executa();
   for (int n = 0; n < lisPEN->Count; n++)
      {
      pendrive = (VTPendrive*)lisPEN->Items[n];
      //verifica se o Pendrive possui base de dados do servidor de licen�a local
      database_name = PathDatabase(pendrive);
      if (! FileExists(database_name)) continue;
      //Pendrive possui base de dados de servidor: verifica validade
      if (ValidaPendrive(pendrive)) return(pendrive);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::LiberaLicenca(VTLicenca *licenca)
   {
   //vari�veis locais
   VTPendrive *pendrive;

   //verifica se existe Pendrive servidor de licen�a
   if ((pendrive = ExisteServidorLicenca()) == NULL)
      {
      return(false);
      }
   //atualiza Licenca
   licenca->CtrlRemoto = false;
   //inicia conex�o com Servidor
   if (! Conecta(PathDatabase(pendrive))) return(false);
   //insere Licenca na base de dados
   servidor->LiberaLicenca(licenca);
   //apresenta diagn�stico
   //Aviso(licenca->Diagnostico());
   return(true);
   }

//---------------------------------------------------------------------------
AnsiString  __fastcall TServidorLocal::PathDatabase(VTPendrive *pendrive)
   {
   return(pendrive->DiskUnit + "\\SinapsisInovacaoEmEnergia\\Data\\" + DBNAME_LICENCA);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::RenovaLicenca(VTLicenca *licenca)
   {
   return(false);
   }

//---------------------------------------------------------------------------
bool __fastcall TServidorLocal::ValidaPendrive(VTPendrive *pendrive)
   {
   //vari�veis locais
   bool       sucesso = false;
   AnsiString database_name;
   VTPendrive *pendrive_bd;

   //verifica se existe base de dados do servidor
   database_name = PathDatabase(pendrive);
   if (! FileExists(database_name)) return(false);
   //inicia conex�o com servidor
   if (! servidor->Conecta(database_name)) return(false);
   try{//cria um Pendrive local p/ ler dados gravados no servidor
      pendrive_bd = new VTPendrive();
      //l� dados do Servidor
      if (ConsultaPendrive(pendrive_bd))
         {//compara dados do Pendrive c/ os dados gravados no Servidor
         sucesso = (pendrive->Id.AnsiCompare(pendrive_bd->Id) == 0);
         }
      }catch(Exception &e)
         {
         }
   //destr�i Pendrive local
   if (pendrive_bd) delete pendrive_bd;

   return(sucesso);
   }
//-----------------------------------------------------------------------------
//eof



