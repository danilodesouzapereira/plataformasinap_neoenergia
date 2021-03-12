//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <objbase.h>
#include "TFormMain.h"
#include "VTLTPserver.h"
#include "..\Diretorio\DirArq.h"
#include "..\Licenca\VTPacote.h"
#include "..\Servidor\Site.h"
#include "..\DLL_Stub\Licenca\Licenca.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
   : TForm(Owner)
   {
   try{//cria ojetos
      ltp_server = NewObjLTPserver();
      pacote     = DLL_NewObjPacote();
      //habilita conexão
      ServerSocket->Active = true;
      if (ServerSocket->Active)
         {
         StatusBar->Panels->Items[0]->Text = "Conectado: " + DateTimeToStr(Now());
         StatusBar->Panels->Items[1]->Text = "PORT: " + IntToStr(SITE_PORT);
         //StatusBar->Panels->Items[2]->Text = "IP: " + ServerSocket->Socket->LocalAddress; //AnsiString(SITE_IP);
         }
      else
         {
         StatusBar->Panels->Items[0]->Text = "Inativo!";
         }
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain(void)
   {
   //desabilita conexão
   if (ServerSocket->Active) ServerSocket->Active = false;
   //destrói objetos
   if (pacote)     {delete pacote;     pacote     = NULL;}
   if (ltp_server) {delete ltp_server; ltp_server = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionClearExecute(TObject *Sender)
   {
   RichEdit->Lines->Clear();
   ActionClear->Enabled = false;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionListaIPExecute(TObject *Sender)
   {
   char               szHostName[1024] = "";
   struct sockaddr_in SocketAddress;
   struct hostent     *pHost        = 0;

   WSADATA            WSAData;

   // Initialize winsock dll
   if (WSAStartup(MAKEWORD(1, 0), &WSAData) != 0)
      {//erro
      return;
      }
   if (::gethostname(szHostName, sizeof(szHostName)) != 0)
      {//erro
      return;
      }
   // Get local IP addresses
   if ((pHost = ::gethostbyname(szHostName)) == NULL)
      {//erro
      return;
      }
   Log("Endereços de IP disponíveis:");
   for(int iCnt = 0; ((pHost->h_addr_list[iCnt]) && (iCnt < 10)); ++iCnt)
      {
      memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[iCnt], pHost->h_length);
      Log(inet_ntoa(SocketAddress.sin_addr));
      }
   // Cleanup
   WSACleanup();
   }


//---------------------------------------------------------------------------
bool __fastcall TFormMain::EnviaPacote(TCustomWinSocket *Socket, VTPacote *pacote)
   {
   //variáveis locais
   int num_byte;

   //envia Pacote p/ o Client
   num_byte = Socket->SendText(pacote->CamposAsStr);
   return(num_byte > 0);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormShow(TObject *Sender)
   {
   //variáveis locais
   AnsiString arq_db;

   //define path da base de dados
   arq_db = ExtractFilePath(GetCurrentDir()) + "Data\\" + DBNAME_LICENCA;
   if (FileExists(arq_db))
      {//inicia conexão c/ Servidor
      ltp_server->Conecta(arq_db);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::Log(AnsiString txt)
   {
   RichEdit->Lines->Add(txt);
   ActionClear->Enabled = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientConnect(TObject *Sender, TCustomWinSocket *Socket)
   {
   //gera mensagem de Log
   Log("Client conectado: ");
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientDisconnect(TObject *Sender, TCustomWinSocket *Socket)
   {
   //gera mensagem de Log
   Log("Client desconectado: ");
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientRead(TObject *Sender, TCustomWinSocket *Socket)
   {
   //variáveis locais
   AnsiString txt;

   Socket->ReceiveLength();
   //recebe o pacote enviado por um Client
   txt = Socket->ReceiveText();
   //gera mensagem de Log
   Log("Pacote recebido: " + txt);
   //retorna mesmo pacote p/ o Client
   //Socket->SendText("Server Responde: " + txt);
   //monta Pacote e passa p/ o LTPserver
   pacote->CamposAsStr = txt;
   ltp_server->TrataPacote(pacote);
   //retorna Pacote de resposta p/ o Client
   txt = pacote->CamposAsStr;
   Socket->SendText(txt);
   //Socket->SendText(pacote->CamposAsStr);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMain::ServerSocketClientWrite(TObject *Sender, TCustomWinSocket *Socket)
   {
   Log("Pacote enviado: ");
   }

//---------------------------------------------------------------------------
//eof


