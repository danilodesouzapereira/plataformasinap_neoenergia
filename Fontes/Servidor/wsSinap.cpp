// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://www.sinapsisenergia.com/licenca/wsSinap.php?wsdl
//  >Import : http://www.sinapsisenergia.com/licenca/wsSinap.php?wsdl>0
// Encoding : ISO-8859-1
// Version  : 1.0
// (08/05/2014 09:02:01 - - $Rev: 56641 $)
// ************************************************************************ //

#include <System.hpp>
#pragma hdrstop

#if !defined(wsSinapH)
#include "wsSinap.h"
#endif



namespace NS_wsSinap {

_di_SinapServerPortType GetSinapServerPortType(bool useWSDL, System::String addr, Soaphttpclient::THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://www.sinapsisenergia.com/licenca/wsSinap.php?wsdl";
  static const char* defURL = "http://www.sinapsisenergia.com/licenca/wsSinap.php";
  static const char* defSvc = "SinapServer";
  static const char* defPrt = "SinapServerPort";
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
  _di_SinapServerPortType service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* SinapServerPortType */
  InvRegistry()->RegisterInterface(__delphirtti(SinapServerPortType), L"urn:SinapLic", L"ISO-8859-1");
  InvRegistry()->RegisterAllSOAPActions(__delphirtti(SinapServerPortType), L"|urn:SinapLic#AtualizaLicenca|urn:SinapLic#Conecta|urn:SinapLic#LiberaLicenca");
  /* SinapServerPortType->AtualizaLicenca */
  InvRegistry()->RegisterParamInfo(__delphirtti(SinapServerPortType), "AtualizaLicenca", "return_", L"return", L"");
  /* SinapServerPortType->Conecta */
  InvRegistry()->RegisterParamInfo(__delphirtti(SinapServerPortType), "Conecta", "return_", L"return", L"");
  /* SinapServerPortType->LeLicenca */
  InvRegistry()->RegisterParamInfo(__delphirtti(SinapServerPortType), "LeLicenca", "return_", L"return", L"");
  /* Licenca */
  RemClassRegistry()->RegisterXSClass(__classid(Licenca), L"urn:SinapLic", L"Licenca");
}
#pragma startup RegTypes 32

};     // NS_wsSinap


