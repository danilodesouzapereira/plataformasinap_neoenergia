// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://192.168.1.44/aries/ReceberEstudoWebService?WSDL
//  >Import : http://192.168.1.44/aries/ReceberEstudoWebService?WSDL>0
//  >Import : http://192.168.1.44:80/aries/ReceberEstudoWebService?xsd=1
// Encoding : UTF-8
// Version  : 1.0
// (14/01/2020 13:42:25 - - $Rev: 90173 $)
// ************************************************************************ //

#include <System.hpp>
#pragma hdrstop

#include "TReceberEstudoWebService.h"



namespace NS_ReceberEstudoWebService {
 /*
_di_ReceberEstudoWebService GetReceberEstudoWebService(bool useWSDL, System::String addr, Soaphttpclient::THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://192.168.1.44/aries/ReceberEstudoWebService?WSDL";
  static const char* defURL = "http://192.168.1.44:80/aries/ReceberEstudoWebService";
  static const char* defSvc = "aries";
  static const char* defPrt = "ReceberEstudoWebServicePort";
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
  _di_ReceberEstudoWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}
*/
_di_ReceberEstudoWebService GetReceberEstudoWebService(char *addr, char *service, char *port)
{
	// variáveis locais
	_di_ReceberEstudoWebService WebService;
	Soaphttpclient::THTTPRIO* rio = new Soaphttpclient::THTTPRIO(0);

	rio->WSDLLocation = addr;
	rio->Service = service;
	rio->Port = port;
	rio->HTTPWebNode->SendTimeout = 300000; // 5 min
	rio->HTTPWebNode->ReceiveTimeout = 1200000; // 20 min
	rio->QueryInterface(WebService);
	if (WebService == NULL)
		delete rio;
	return WebService;
}

__fastcall estudo::~estudo()
{
  for(int i=0; i<Falternativa.Length; i++)
    if (Falternativa[i])
      delete Falternativa[i];
}

__fastcall alternativa::~alternativa()
{
  for(int i=0; i<FinformacaoTecnica.Length; i++)
    if (FinformacaoTecnica[i])
      delete FinformacaoTecnica[i];
  for(int i=0; i<Fobra.Length; i++)
    if (Fobra[i])
      delete Fobra[i];
  for(int i=0; i<Frede.Length; i++)
    if (Frede[i])
      delete Frede[i];
}

__fastcall informacaoTecnicaComercial::~informacaoTecnicaComercial()
{
  for(int i=0; i<Fvalor.Length; i++)
    if (Fvalor[i])
      delete Fvalor[i];
}

__fastcall elementoPotencia::~elementoPotencia()
{
  delete FredeAlternativa;
  for(int i=0; i<FpotenciaInstalada.Length; i++)
    if (FpotenciaInstalada[i])
      delete FpotenciaInstalada[i];
}

__fastcall redeAlternativa::~redeAlternativa()
{
  for(int i=0; i<FdemandaPotencia.Length; i++)
    if (FdemandaPotencia[i])
      delete FdemandaPotencia[i];
  for(int i=0; i<FelementoPotencia.Length; i++)
    if (FelementoPotencia[i])
      delete FelementoPotencia[i];
}

__fastcall equipamento::~equipamento()
{
  for(int i=0; i<Frede.Length; i++)
    if (Frede[i])
      delete Frede[i];
}

__fastcall obra::~obra()
{
  for(int i=0; i<Fequipamento.Length; i++)
    if (Fequipamento[i])
      delete Fequipamento[i];
  for(int i=0; i<Fmunicipio.Length; i++)
    if (Fmunicipio[i])
      delete Fmunicipio[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* ReceberEstudoWebService */
  InvRegistry()->RegisterInterface(__delphirtti(ReceberEstudoWebService), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"UTF-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(ReceberEstudoWebService), L"");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(ReceberEstudoWebService), ioDocument);
  /* ReceberEstudoWebService.receber */
  InvRegistry()->RegisterMethodInfo(__delphirtti(ReceberEstudoWebService), "receber", "",
                                    "[ReturnName='return']", IS_OPTN | IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ReceberEstudoWebService), "receber", "estudo", L"",
                                   L"", IS_UNQL);
  InvRegistry()->RegisterParamInfo(__delphirtti(ReceberEstudoWebService), "receber", "return_", L"return",
                                   L"", IS_UNQL);
  /* Array_Of_informacaoTecnicaComercial */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_informacaoTecnicaComercial), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_informacaoTecnicaComercial");
  /* Array_Of_alternativa */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_alternativa), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_alternativa");
  /* Array_Of_obra */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_obra), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_obra");
  /* Array_Of_elementoPotencia */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_elementoPotencia), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_elementoPotencia");
  /* Array_Of_demandaPotencia */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_demandaPotencia), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_demandaPotencia");
  /* Array_Of_potenciaInstalada */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_potenciaInstalada), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_potenciaInstalada");
  /* Array_Of_rede */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_rede), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_rede");
  /* Array_Of_valorInformacaoTecnicaComercial */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_valorInformacaoTecnicaComercial), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_valorInformacaoTecnicaComercial");
  /* Array_Of_redeAlternativa */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_redeAlternativa), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_redeAlternativa");
  /* Array_Of_municipio */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_municipio), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_municipio");
  /* Array_Of_equipamento */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_equipamento), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"Array_Of_equipamento");
  /* estudo */
  RemClassRegistry()->RegisterXSClass(__classid(estudo), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"estudo");
  /* alternativa */
  RemClassRegistry()->RegisterXSClass(__classid(alternativa), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"alternativa");
  /* informacaoTecnicaComercial */
  RemClassRegistry()->RegisterXSClass(__classid(informacaoTecnicaComercial), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"informacaoTecnicaComercial");
  /* resposta */
  RemClassRegistry()->RegisterXSClass(__classid(resposta), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"resposta");
  /* rede */
  RemClassRegistry()->RegisterXSClass(__classid(rede), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"rede");
  /* municipio */
  RemClassRegistry()->RegisterXSClass(__classid(municipio2), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"municipio2", L"municipio");
  /* municipio */
  RemClassRegistry()->RegisterXSClass(__classid(municipio), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"municipio");
  /* elementoPotencia */
  RemClassRegistry()->RegisterXSClass(__classid(elementoPotencia), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"elementoPotencia");
  /* demandaPotencia */
  RemClassRegistry()->RegisterXSClass(__classid(demandaPotencia), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"demandaPotencia");
  /* valorInformacaoTecnicaComercial */
  RemClassRegistry()->RegisterXSClass(__classid(valorInformacaoTecnicaComercial), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"valorInformacaoTecnicaComercial");
  /* redeAlternativa */
  RemClassRegistry()->RegisterXSClass(__classid(redeAlternativa), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"redeAlternativa");
  /* potenciaInstalada */
  RemClassRegistry()->RegisterXSClass(__classid(potenciaInstalada), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"potenciaInstalada");
  /* equipamento */
  RemClassRegistry()->RegisterXSClass(__classid(equipamento), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"equipamento");
  /* obra */
  RemClassRegistry()->RegisterXSClass(__classid(obra), L"http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/", L"obra");
}
#pragma startup RegTypes 32

};     // NS_ReceberEstudoWebService

