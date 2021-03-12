//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//#include <objbase.h>
#include <wbemcli.h>
#include <iads.h>
#include <adshlp.h>
#include <utilcls.h>
#include "TPenFinder.h"
#include "VTPendrive.h"

//---------------------------------------------------------------------------
VTPenFinder* __fastcall NewObjPenFinder(void)
   {
   try{//cria objeto TPenFinder
      return(new TPenFinder());
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//---------------------------------------------------------------------------
__fastcall TPenFinder::TPenFinder(void)
   {
   lisPEN = new TList();
   }

//---------------------------------------------------------------------------
__fastcall TPenFinder::~TPenFinder(void)
   {
   //destr�i lista e seus objetos
   if (lisPEN) {LimpaTList(lisPEN); delete lisPEN; lisPEN = NULL;}
   }

//---------------------------------------------------------------------------
TList* __fastcall TPenFinder::Executa(void)
   {
   //reinicia lisPEN
   LimpaTList(lisPEN);
   //executa fun��o do MarceloLopes
   MLopes_GetPenDriveList();
   
   return(lisPEN);
   }

//---------------------------------------------------------------------------
VTPendrive* __fastcall TPenFinder::ExistePendrive(AnsiString pen_id)
   {
   //vari�veis locais
   VTPendrive *pendrive;

   //reinicia lista de Pendrive
   Executa();
   //procura Pendrive indicado
   for (int n = 0; n < lisPEN->Count; n++)
      {
      pendrive = (VTPendrive*)lisPEN->Items[n];
      if (pendrive->Id.AnsiCompareIC(pen_id) == 0) return(pendrive);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TPenFinder::LimpaTList(TList *lisOBJ)
   {
   //vari�veis locais
   TObject *object;

   for (int n = 0; n < lisOBJ->Count; n++)
      {
      object = (TObject*)lisOBJ->Items[n];
      delete object;
      }
   lisOBJ->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TPenFinder::MLopes_GetPenDriveList(void)
{
  IWbemLocator* ptrLoc;                  // Ponteiro para o WBEM Locator
  IWbemServices* ptrSvc;                 //  Ponteiro para o WBEM Services
  IEnumWbemClassObject* ptrWbmEnmDskPhy; // Enumera��o de classes WBEM - Disco f�sico
  IWbemClassObject* ptrWbmClsDskPhy;     // Ponteiro para o objeto que armazena info sobre disco f�sico
  IEnumWbemClassObject* ptrWbmEnmPrtPhy; // Enumera��o de classes WBEM - Parti��o f�sica
  IWbemClassObject* ptrWbmClsPrtPhy;     // Ponteiro para o objeto que armazena info sobre parti��o f�sica
  IEnumWbemClassObject* ptrWbmEnmPrtLog; // Enumera��o de classes WBEM - Parti��o l�gica
  IWbemClassObject* ptrWbmClsPrtLog;     // Ponteiro para o objeto que armazena info sobre parti��o l�gica

  VARIANT Val;                           // Valor de uma propriedade
  CIMTYPE pvtType;                       // Tipo de uma propriedade
  ULONG uReturned;                       // C�digo de retorno
  HRESULT hres;                          // Handle do resultado da query
  long lngret;                           // Retorno das chamadas das propriedades WBEM
  LPSTR ptrCStr;                         // Ponteiro para string (constante)
  BSTR ptrBStr;                          // Ponteiro para uma string wide
  int nLen ;                             // Comprimento final da string wide
  AnsiString straux;                     // string auxiliar
  VTPendrive *pendrive;        // Elemento da lista
  AnsiString strDsp;                     // Identificador intermedi�rio

  try
  {
   /*
    // Inicializa COM (Common Object Model) para a Thread corrente.
    // Aten��o: tem que ser COINIT_MULTITHREADED
    hres = CoInitializeEx(0, COINIT_MULTITHREADED); //Initialize COM.
    if (FAILED(hres))
    {
      throw(Exception("Falha na inicia��o da biblioteca COM. C�digo de erro = " + IntToHex((int)hres, 8)) );
    }
    // Configura o n�vel de seguran�a para conex�o
    hres =  CoInitializeSecurity(
                                 NULL,                        // security descriptor
                                   -1,                        // use this simple setting
                                 NULL,                        // use this simple setting
                                 NULL,                        // reserved
                                 RPC_C_AUTHN_LEVEL_CONNECT,   // Windows NT 4/Windows 2000 authentication level
                                 RPC_C_IMP_LEVEL_IMPERSONATE, // impersonation level
                                 NULL,                        // use this simple setting
                                 EOAC_NONE,                   // no special capabilities
                                 0                            // reserved
                                );
    if (FAILED(hres))
    {
      throw(Exception("Falha na inicia��o da seguran�a. C�digo de erro = " + IntToHex((int)hres, 8)));
    }
    */

    // IWbemLocator interface para oten��o do ponteiro para o Namespace do WBEM no computador
    // corrente. No windows, a implementa��o do WBEM corresponde ao WMI.
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &ptrLoc);
    if (FAILED(hres))
    {
      throw(Exception("Falha na cria��o do objeto IWbemLocator. C�digo de erro = " + IntToHex((int)hres, 8)));
    }

    // Conex�o ao reposit�rio
    hres = ptrLoc->ConnectServer(L"\\\\.\\root\\cimv2", NULL, NULL, 0, NULL, 0, 0, &ptrSvc);
    if (FAILED(hres))
    {
      throw(Exception("N�o foi poss�vel efetuar a conex�o ao reposit�rio. C�digo de erro = " + IntToHex((int)hres, 8)));
    }

    // "Impersonation" da conex�o...
    hres = CoSetProxyBlanket( ptrSvc,
                              RPC_C_AUTHN_WINNT,           // NTLM authentication service
                              RPC_C_AUTHZ_NONE,            // default authorization service...
                              NULL,                        // no mutual authentication
                              RPC_C_AUTHN_LEVEL_CONNECT,   // authentication level
                              RPC_C_IMP_LEVEL_IMPERSONATE, // impersonation level
                              NULL,                        // use current token
                              EOAC_NONE                    // no special capabilities
                            );
    if (FAILED(hres))
    {
      throw(Exception("Erro na conex�o ao Proxy (impersonation). C�digo de erro = " + IntToHex((int)hres, 8)));
    }

    // Descobre os volumes para cada drive
    // Conecta-se ao namespace Win32_LogicalDisk, onde est�o os dados sobre os discos
    // Cria enumera��o dos dispositivos para loop
    hres = ptrSvc->CreateInstanceEnum(L"Win32_DiskDrive", NULL, NULL, &ptrWbmEnmDskPhy);
    if (hres != 0)
    {
      throw(Exception("Erro na cria��o da lista de dispositivos f�sicos."));
    }

    hres = ptrSvc->CreateInstanceEnum(L"Win32_LogicalDiskToPartition", NULL, NULL, &ptrWbmEnmPrtLog);
    if (hres != 0)
    {
      throw(Exception("Erro na cria��o da lista de dispositivos l�gicos."));
    }

    hres = ptrSvc->CreateInstanceEnum(L"Win32_DiskDriveToDiskPartition", NULL, NULL, &ptrWbmEnmPrtPhy);
    if (hres != 0)
    {
      throw(Exception("Erro na cria��o da lista de parti��es."));
    }

    hres = ptrSvc->ExecQuery( L"WQL", L"SELECT * FROM Win32_DiskDrive WHERE InterfaceType = \"USB\"", WBEM_FLAG_FORWARD_ONLY, NULL, &ptrWbmEnmDskPhy);
    if (hres != 0)
    {
      throw(Exception("N�o foi poss�vel a cria��o da entidade PnP. C�digo de erro = " + IntToHex((int)hres, 8)));
    }

    for (;;)
    {
      // --------------------------
      // Loop Win32_DiskDrive (ini)
      hres = ptrWbmEnmDskPhy->Next(WBEM_INFINITE /* Time out */, 1 /* One object */, &ptrWbmClsDskPhy, &uReturned);
      if (uReturned == 0)
        break;
      if (hres != 0)
        break;

      // Identifica cada objeto
      pendrive = new VTPendrive();

      VariantInit(&Val);

      lngret = ptrWbmClsDskPhy->Get(L"DeviceID", 0, &Val, &pvtType, NULL);
      if (lngret == 0)
      {
        ptrCStr = WideToAnsi(Val.bstrVal);
        //pendrive->m_strDeviceID = ptrCStr;
        pendrive->DeviceID = ptrCStr;
        delete[] ptrCStr;
      }

      lngret = ptrWbmClsDskPhy->Get(L"PNPDeviceID", 0, &Val, &pvtType, NULL);
      if (lngret == 0)
      {
        ptrCStr = WideToAnsi(Val.bstrVal);
        //pendrive->m_strPNPDeviceID = ptrCStr;
        pendrive->Id = ptrCStr;
        delete[] ptrCStr;
      }

      lngret = ptrWbmClsDskPhy->Get(L"Status", 0, &Val, &pvtType, NULL);
      if (lngret == 0)
      {
        ptrCStr = WideToAnsi(Val.bstrVal);
        //pendrive->m_strStatus = ptrCStr;
        pendrive->Status = ptrCStr;
        delete[] ptrCStr;
      }

      // Busca parti��es f�sicas
      strDsp = TrocaBarrasSimplesPorBarrasDuplas(pendrive->DeviceID);

      straux = "ASSOCIATORS OF {Win32_DiskDrive.DeviceID=\"" + strDsp + "\"} WHERE AssocClass = Win32_DiskDriveToDiskPartition";
      // O malabarismo ocorre porque AnsiToWide n�o compila. Bug da Borland???
      ptrBStr = NULL;  // Zerar antes de realocar!
      nLen = MultiByteToWideChar(CP_ACP, 0, straux.c_str(), straux.Length(), NULL, 0);
      if (SysReAllocStringLen(&ptrBStr, NULL, nLen))
      {
        MultiByteToWideChar(CP_ACP, 0, straux.c_str(), straux.Length(), ptrBStr, nLen);
      }
      else
      {
        throw(Exception("Erro realocando string."));
      }

      hres = ptrSvc->ExecQuery( L"WQL", ptrBStr, WBEM_FLAG_FORWARD_ONLY, NULL, &ptrWbmEnmPrtPhy);
      SysFreeString(ptrBStr);
      if (hres == 0)
      {
        // ----------------------------
        // Parti��o f�sica v�lida (ini)
        for (;;)
        {
          // -----------------------------------------
          // Loop Win32_DiskDriveToDiskPartition (ini)
          hres = ptrWbmEnmPrtPhy->Next(WBEM_INFINITE /* Time out */, 1 /* One object */, &ptrWbmClsPrtPhy, &uReturned);
          if (uReturned == 0)
            break;
          if (hres == 0)
          {
            // ---------------------------------------
            // Parti��o associa��o f�sica v�lida (ini)
            lngret = ptrWbmClsPrtPhy->Get(L"DeviceID", 0, &Val, &pvtType, NULL);
            if (lngret == 0)
            {
              ptrCStr = WideToAnsi(Val.bstrVal);
              straux = ptrCStr;
              delete[] ptrCStr;
            }

            // Busca drives l�gicos
            straux = "ASSOCIATORS OF {Win32_DiskPartition.DeviceID=\"" + straux + "\"} WHERE AssocClass = Win32_LogicalDiskToPartition";
            // O malabarismo ocorre porque AnsiToWide n�o compila. Bug da Borland???
            ptrBStr = NULL;  // Zerar antes de realocar!
            nLen = MultiByteToWideChar(CP_ACP, 0, straux.c_str(), straux.Length(), NULL, 0);
            if (SysReAllocStringLen(&ptrBStr, NULL, nLen))
            {
              MultiByteToWideChar(CP_ACP, 0, straux.c_str(), straux.Length(), ptrBStr, nLen);
            }
            else
            {
              throw(Exception("Erro realocando string."));
            }
            hres = ptrSvc->ExecQuery( L"WQL", ptrBStr, WBEM_FLAG_FORWARD_ONLY, NULL, &ptrWbmEnmPrtLog);
            SysFreeString(ptrBStr);
            if (hres == 0)
            {
              // ----------------------------
              // Parti��o l�gica v�lida (ini)
              for (;;)
              {
                // ---------------------------------------
                // Loop Win32_LogicalDiskToPartition (ini)
                hres = ptrWbmEnmPrtLog->Next(WBEM_INFINITE /* Time out */, 1 /* One object */, &ptrWbmClsPrtLog, &uReturned);
                if (uReturned == 0)
                  break;
                if (hres == 0)
                {
                  // ---------------------------------------
                  // Parti��o associa��o l�gica v�lida (ini)
                  lngret = ptrWbmClsPrtLog->Get(L"DeviceID", 0, &Val, &pvtType, NULL);
                  if (lngret == 0)
                  {
                    ptrCStr = WideToAnsi(Val.bstrVal);
                    //pendrive->m_strDiskUnit = ptrCStr;
                    pendrive->DiskUnit = ptrCStr;
                    delete[] ptrCStr;
                  }
                  // Parti��o associa��o l�gica v�lida (fim)
                  // ---------------------------------------
                }
                // Loop Win32_LogicalDiskToPartition (ini)
                // ---------------------------------------
              }
              // Parti��o l�gica v�lida (fim)
              // ----------------------------
            }
            // Parti��o associa��o f�sica v�lida (end)
            // ---------------------------------------
          }
          // Loop Win32_DiskDriveToDiskPartition (fim)
          // -----------------------------------------
        }
        // Parti��o f�sica v�lida (fim)
        // ----------------------------
      }

      lisPEN->Add(pendrive);
      // Loop Win32_DiskDrive (fim)
      // --------------------------
    }

    // Libera recursos que n�o ser�o mais utilizados
    ptrSvc->Release();
    ptrLoc->Release();
    ptrWbmEnmDskPhy->Release();
    ptrWbmEnmPrtPhy->Release();
    ptrWbmEnmPrtLog->Release();
    //CoUninitialize();
    VariantClear(&Val);
  }
  catch (Exception &exception)
  {
    Application->ShowException(&exception);
    Application->Terminate();
  }
} // GetPenDriveList

//---------------------------------------------------------------------------
AnsiString __fastcall TPenFinder::TrocaBarrasSimplesPorBarrasDuplas(AnsiString strin)
   {
   AnsiString strret = "";
   char chraux;

   for (int i = 1; i <= strin.Length(); i++)
      {
      chraux = strin[i];
      strret = strret + chraux;
      if (chraux == '\\') strret = strret + chraux;
      }
   return(strret);
   } // TrocaBarrasSimplesPorBarrasDuplas

//---------------------------------------------------------------------------
//eof

