//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TLTPserver.h"
#include "..\Licenca\VTCampo.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTPacote.h"
#include "..\Servidor\VTServidor.h"
#include "..\DLL_Stub\Licenca\Licenca.h"
#include "..\DLL_Stub\Servidor\Servidor.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//-----------------------------------------------------------------------------
VTLTPserver* __fastcall NewObjLTPserver(void)
   {
   return(new TLTPserver());
   }

//---------------------------------------------------------------------------
__fastcall TLTPserver::TLTPserver(void)
   {
   try{//cria objetos
      servidor  = DLL_NewObjServidorLocal();
      lic_micro = DLL_NewObjLicenca();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TLTPserver::~TLTPserver(void)
   {
   //destrói objetos
   if (servidor)  {delete servidor;  servidor  = NULL;}
   if (lic_micro) {delete lic_micro; lic_micro = NULL;}
   }

//-----------------------------------------------------------------------------
void __fastcall TLTPserver::ComandoInvalido(void)
   {
   }

//-----------------------------------------------------------------------------
void __fastcall TLTPserver::Conecta(AnsiString arq_db)
   {
   servidor->Conecta(arq_db);
   }

//-----------------------------------------------------------------------------
void __fastcall TLTPserver::MontaPacote(VTPacote *pacote, AnsiString comando, VTLicenca *licenca)
   {
   //insere Campos c/ dados da Licenca no pacote  (IMPORTANTE: o Pacote é reiniciado)
   licenca->WriteToPacote(pacote);
   //insere Campo de comando no Pacote
   pacote->Campo("cmd")->AsString = comando;
   }

//-----------------------------------------------------------------------------
bool __fastcall TLTPserver::TrataPacote(VTPacote *pacote)
   {
   //variáveis locais
   AnsiString cmd;

   //salva ponteiros p/ objetos
   this->pacote = pacote;
   //inicia Licenca do micro c/ dados do pacote
   lic_micro->ReadFromPacote(pacote);
   //trata comando e monta resposta no próprio Pacote recebido
   cmd = pacote->Campo("cmd")->AsString;
   if      (cmd.AnsiCompareIC("AlocaLicenca")  == 0) {servidor->AlocaLicenca(lic_micro);}
   else if (cmd.AnsiCompareIC("LiberaLicenca") == 0) {servidor->LiberaLicenca(lic_micro);}
   else if (cmd.AnsiCompareIC("RenovaLicenca") == 0) {servidor->RenovaLicenca(lic_micro);}
   else                                              {ComandoInvalido();}
   //monta Pacote de resposta c/ a Licenca atualizada
   MontaPacote(pacote, cmd, lic_micro);
   return(true);
   }

//-----------------------------------------------------------------------------
//eof



