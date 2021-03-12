// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://200.98.139.179/ariestreina/ListarPadraoConstrutivoWebService?wsdl
//  >Import : http://200.98.139.179/ariestreina/ListarPadraoConstrutivoWebService?wsdl>0
//  >Import : http://200.98.139.179:80/ariestreina/ListarPadraoConstrutivoWebService?xsd=1
// Encoding : UTF-8
// Version  : 1.0
// (21/02/2018 11:28:29 - - $Rev: 69934 $)
// ************************************************************************ //

#include <System.hpp>
#pragma hdrstop

#include "TListarPadraoConstrutivoWebService.h"



namespace NS_ListarPadraoConstrutivoWebService {
/*
_di_ListarPadraoConstrutivoWebService GetListarPadraoConstrutivoWebService(bool useWSDL, System::String addr, Soaphttpclient::THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://200.98.139.179/ariestreina/ListarPadraoConstrutivoWebService?wsdl";
  static const char* defURL = "http://200.98.139.179:80/ariestreina/ListarPadraoConstrutivoWebService";
  static const char* defSvc = "ariestreina";
  static const char* defPrt = "ListarPadraoConstrutivoWebServicePort";
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
  _di_ListarPadraoConstrutivoWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}
*/
//---------------------------------------------------------------------------
_di_ListarPadraoConstrutivoWebService GetListarPadraoConstrutivoWebService(char *addr, char *service, char *port)
   {
   //variáveis locais
   _di_ListarPadraoConstrutivoWebService WebService;
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
static void RegTypes()
{
  /* ListarPadraoConstrutivoWebService */
  InvRegistry()->RegisterInterface(__delphirtti(ListarPadraoConstrutivoWebService), L"http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"UTF-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(ListarPadraoConstrutivoWebService), L"");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(ListarPadraoConstrutivoWebService), ioDocument);
  /* ListarPadraoConstrutivoWebService.listar */
  InvRegistry()->RegisterMethodInfo(__delphirtti(ListarPadraoConstrutivoWebService), "listar", "",
                                    "[ReturnName='padraoConstrutivo']", IS_OPTN | IS_UNBD | IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ListarPadraoConstrutivoWebService), "listar", "idDominio", L"",
                                   L"", IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ListarPadraoConstrutivoWebService), "listar", "padraoConstrutivo", L"",
                                   L"[ArrayItemName='padraoConstrutivo']", IS_UNBD | IS_UNQL);
  /* listarResponsePadrao */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(listarResponsePadrao), L"http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"listarResponse");
  /* listarResponsePadrao */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(listarResponsePadrao), L"http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"listarResponse");
  /* padraoConstrutivo */
  RemClassRegistry()->RegisterXSClass(__classid(padraoConstrutivo), L"http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"padraoConstrutivo");
}
#pragma startup RegTypes 32

};     // NS_ListarPadraoConstrutivoWebService

