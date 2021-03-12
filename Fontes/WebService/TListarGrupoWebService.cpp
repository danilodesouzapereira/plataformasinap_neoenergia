// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://192.168.1.44/aries/ListarGrupoWebService?WSDL
//  >Import : http://192.168.1.44/aries/ListarGrupoWebService?WSDL>0
//  >Import : http://192.168.1.44:80/aries/ListarGrupoWebService?xsd=1
// Encoding : UTF-8
// Version  : 1.0
// (13/01/2020 16:44:20 - - $Rev: 90173 $)
// ************************************************************************ //

#include <System.hpp>
#pragma hdrstop

#include "TListarGrupoWebService.h"



namespace NS_ListarGrupoWebService {
/*
_di_ListarGrupoWebService GetListarGrupoWebService(bool useWSDL, System::String addr, Soaphttpclient::THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://192.168.1.44/aries/ListarGrupoWebService?WSDL";
  static const char* defURL = "http://192.168.1.44:80/aries/ListarGrupoWebService";
  static const char* defSvc = "aries";
  static const char* defPrt = "ListarGrupoWebServicePort";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  Soaphttpclient::THTTPRIO* rio = HTTPRIO ? HTTPRIO : new Soaphttpclient::THTTPRIO(0);
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_ListarGrupoWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}
*/
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
_di_ListarGrupoWebService GetListarGrupoWebService(char *addr, char *service, char *port)
   {
   //variáveis locais
   _di_ListarGrupoWebService WebService;
   Soaphttpclient::THTTPRIO* rio = new Soaphttpclient::THTTPRIO(0);

   rio->WSDLLocation = addr;
   rio->Service      = service;
   rio->Port         = port;
   rio->QueryInterface(WebService);
   if (WebService == NULL) delete rio;
   return WebService;
   }

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
// https://sinapsisenergia.teamwork.com/#/tasks/17302005
static void RegTypes()
{
  /* ListarGrupoWebService */
  InvRegistry()->RegisterInterface(__delphirtti(ListarGrupoWebService), L"http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"UTF-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(ListarGrupoWebService), L"");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(ListarGrupoWebService), ioDocument);
  /* ListarGrupoWebService.listar */
  InvRegistry()->RegisterMethodInfo(__delphirtti(ListarGrupoWebService), "listar", "",
                                    "[ReturnName='grupo']", IS_OPTN | IS_UNBD | IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ListarGrupoWebService), "listar", "idDominio", L"",
                                   L"", IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ListarGrupoWebService), "listar", "grupo", L"",
                                   L"[ArrayItemName='grupo']", IS_UNBD | IS_UNQL);
  /* listarResponse */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(listarResponse), L"http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"listarResponse");
  /* listarResponse */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(listarResponse), L"http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"listarResponse");
  /* grupo */
  RemClassRegistry()->RegisterXSClass(__classid(grupo), L"http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"grupo");
}
#pragma startup RegTypes 32

};     // NS_ListarGrupoWebService

