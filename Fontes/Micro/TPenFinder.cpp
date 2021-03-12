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
   //destrói lista e seus objetos
   if (lisPEN) {LimpaTList(lisPEN); delete lisPEN; lisPEN = NULL;}
   }

//---------------------------------------------------------------------------
TList* __fastcall TPenFinder::Executa(void)
   {
   //reinicia lisPEN
   LimpaTList(lisPEN);
   //executa função do MarceloLopes
   MLopes_GetPenDriveList();
   
   return(lisPEN);
   }

//---------------------------------------------------------------------------
VTPendrive* __fastcall TPenFinder::ExistePendrive(AnsiString pen_id)
   {
   //variáveis locais
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
   //variáveis locais
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
  IEnumWbemClassObject* ptrWbmEnmDskPhy; // Enumeração de classes WBEM - Disco físico
  IWbemClassObject* ptrWbmClsDskPhy;     // Ponteiro para o objeto que armazena info sobre disco físico
  IEnumWbemClassObject* ptrWbmEnmPrtPhy; // Enumeração de classes WBEM - Partição física
  IWbemClassObject* ptrWbmClsPrtPhy;     // Ponteiro para o objeto que armazena info sobre partição física
  IEnumWbemClassObject* ptrWbmEnmPrtLog; // Enumeração de classes WBEM - Partição lógica
  IWbemClassObject* ptrWbmClsPrtLog;     // Ponteiro para o objeto que armazena info sobre partição lógica

  VARIANT Val;                           // Valor de uma propriedade
  CIMTYPE pvtType;                       // Tipo de uma propriedade
  ULONG uReturned;                       // Código de retorno
  HRESULT hres;                          // Handle do resultado da query
  long lngret;                           // Retorno das chamadas das propriedades WBEM
  LPSTR ptrCStr;                         // Ponteiro para string (constante)
  BSTR ptrBStr;                          // Ponteiro para uma string wide
  int nLen ;                             // Comprimento final da string wide
  AnsiString straux;                     // string auxiliar
  VTPendrive *pendrive;        // Elemento da lista
  AnsiString strDsp;                     // Identificador intermediário

  try
  {
   /*
    // Inicializa COM (Common Object Model) para a Thread corrente.
    // Atenção: tem que ser COINIT_MULTITHREADED
    hres = CoInitializeEx(0, COINIT_MULTITHREADED); //Initialize COM.
    if (FAILED(hres))
    {
      throw(Exception("Falha na iniciação da biblioteca COM. Código de erro = " + IntToHex((int)hres, 8)) );
    }
    // Configura o nível de segurança para conexão
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
      throw(Exception("Falha na iniciação da segurança. Código de erro = " + IntToHex((int)hres, 8)));
    }
    */

    // IWbemLocator interface para otenção do ponteiro para o Namespace do WBEM no computador
    // corrente. No windows, a implementação do WBEM corresponde ao WMI.
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID *) &ptrLoc);
    if (FAILED(hres))
    {
      throw(Exception("Falha na criação do objeto IWbemLocator. Código de erro = " + IntToHex((int)hres, 8)));
    }

    // Conexão ao repositório
    hres = ptrLoc->ConnectServer(L"\\\\.\\root\\cimv2", NULL, NULL, 0, NULL, 0, 0, &ptrSvc);
    if (FAILED(hres))
    {
      throw(Exception("Não foi possível efetuar a conexão ao repositório. Código de erro = " + IntToHex((int)hres, 8)));
    }

    // "Impersonation" da conexão...
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
      throw(Exception("Erro na conexão ao Proxy (impersonation). Código de erro = " + IntToHex((int)hres, 8)));
    }

    // Descobre os volumes para cada drive
    // Conecta-se ao namespace Win32_LogicalDisk, onde estão os dados sobre os discos
    // Cria enumeração dos dispositivos para loop
    hres = ptrSvc->CreateInstanceEnum(L"Win32_DiskDrive", NULL, NULL, &ptrWbmEnmDskPhy);
    if (hres != 0)
    {
      throw(Exception("Erro na criação da lista de dispositivos físicos."));
    }

    hres = ptrSvc->CreateInstanceEnum(L"Win32_LogicalDiskToPartition", NULL, NULL, &ptrWbmEnmPrtLog);
    if (hres != 0)
    {
      throw(Exception("Erro na criação da lista de dispositivos lógicos."));
    }

    hres = ptrSvc->CreateInstanceEnum(L"Win32_DiskDriveToDiskPartition", NULL, NULL, &ptrWbmEnmPrtPhy);
    if (hres != 0)
    {
      throw(Exception("Erro na criação da lista de partições."));
    }

    hres = ptrSvc->ExecQuery( L"WQL", L"SELECT * FROM Win32_DiskDrive WHERE InterfaceType = \"USB\"", WBEM_FLAG_FORWARD_ONLY, NULL, &ptrWbmEnmDskPhy);
    if (hres != 0)
    {
      throw(Exception("Não foi possível a criação da entidade PnP. Código de erro = " + IntToHex((int)hres, 8)));
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

      // Busca partições físicas
      strDsp = TrocaBarrasSimplesPorBarrasDuplas(pendrive->DeviceID);

      straux = "ASSOCIATORS OF {Win32_DiskDrive.DeviceID=\"" + strDsp + "\"} WHERE AssocClass = Win32_DiskDriveToDiskPartition";
      // O malabarismo ocorre porque AnsiToWide não compila. Bug da Borland???
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
        // Partição física válida (ini)
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
            // Partição associação física válida (ini)
            lngret = ptrWbmClsPrtPhy->Get(L"DeviceID", 0, &Val, &pvtType, NULL);
            if (lngret == 0)
            {
              ptrCStr = WideToAnsi(Val.bstrVal);
              straux = ptrCStr;
              delete[] ptrCStr;
            }

            // Busca drives lógicos
            straux = "ASSOCIATORS OF {Win32_DiskPartition.DeviceID=\"" + straux + "\"} WHERE AssocClass = Win32_LogicalDiskToPartition";
            // O malabarismo ocorre porque AnsiToWide não compila. Bug da Borland???
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
              // Partição lógica válida (ini)
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
                  // Partição associação lógica válida (ini)
                  lngret = ptrWbmClsPrtLog->Get(L"DeviceID", 0, &Val, &pvtType, NULL);
                  if (lngret == 0)
                  {
                    ptrCStr = WideToAnsi(Val.bstrVal);
                    //pendrive->m_strDiskUnit = ptrCStr;
                    pendrive->DiskUnit = ptrCStr;
                    delete[] ptrCStr;
                  }
                  // Partição associação lógica válida (fim)
                  // ---------------------------------------
                }
                // Loop Win32_LogicalDiskToPartition (ini)
                // ---------------------------------------
              }
              // Partição lógica válida (fim)
              // ----------------------------
            }
            // Partição associação física válida (end)
            // ---------------------------------------
          }
          // Loop Win32_DiskDriveToDiskPartition (fim)
          // -----------------------------------------
        }
        // Partição física válida (fim)
        // ----------------------------
      }

      lisPEN->Add(pendrive);
      // Loop Win32_DiskDrive (fim)
      // --------------------------
    }

    // Libera recursos que não serão mais utilizados
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

