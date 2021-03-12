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

#ifndef   TListarPadraoConstrutivoWebServiceH
#define   TListarPadraoConstrutivoWebServiceH

#include <System.hpp>
#include <Soap.InvokeRegistry.hpp>
#include <Soap.XSBuiltIns.hpp>
#include <Soap.SOAPHTTPClient.hpp>

#if !defined(SOAP_REMOTABLE_CLASS)
#define SOAP_REMOTABLE_CLASS __declspec(delphiclass)
#endif
#if !defined(IS_OPTN)
#define IS_OPTN 0x0001
#endif
#if !defined(IS_UNBD)
#define IS_UNBD 0x0002
#endif
#if !defined(IS_UNQL)
#define IS_UNQL 0x0008
#endif


namespace NS_ListarPadraoConstrutivoWebService {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS padraoConstrutivo;

typedef DynamicArray<padraoConstrutivo*> listarResponsePadrao; /* "http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/"[Lit][GblCplx] */


// ************************************************************************ //
// XML       : padraoConstrutivo, global, <complexType>
// Namespace : http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class padraoConstrutivo : public TRemotable {
private:
  UnicodeString   Fcodigo;
  bool            Fcodigo_Specified;
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  int             FidEquipamentoSinap;
  bool            FidEquipamentoSinap_Specified;
  int             FidPadraoConstrutivo;
  bool            FidPadraoConstrutivo_Specified;

  double          Fcusto;
  bool            Fcusto_Specified;

  void __fastcall Setcodigo(int Index, UnicodeString _prop_val)
  {  Fcodigo = _prop_val; Fcodigo_Specified = true;  }
  bool __fastcall codigo_Specified(int Index)
  {  return Fcodigo_Specified;  } 
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall SetidEquipamentoSinap(int Index, int _prop_val)
  {  FidEquipamentoSinap = _prop_val; FidEquipamentoSinap_Specified = true;  }
  bool __fastcall idEquipamentoSinap_Specified(int Index)
  {  return FidEquipamentoSinap_Specified;  } 
  void __fastcall SetidPadraoConstrutivo(int Index, int _prop_val)
  {  FidPadraoConstrutivo = _prop_val; FidPadraoConstrutivo_Specified = true;  }
  bool __fastcall idPadraoConstrutivo_Specified(int Index)
  {  return FidPadraoConstrutivo_Specified;  }

  void __fastcall Setcusto(int Index, double _prop_val)
  {  Fcusto = _prop_val; Fcusto_Specified = true;  }
  bool __fastcall custo_Specified(int Index)
  {  return Fcusto_Specified;  }

__published:
  __property UnicodeString     codigo = { index=(IS_OPTN | IS_UNQL), read=Fcodigo, write=Setcodigo, stored = codigo_Specified };
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property int        idEquipamentoSinap = { index=(IS_OPTN | IS_UNQL), read=FidEquipamentoSinap, write=SetidEquipamentoSinap, stored = idEquipamentoSinap_Specified };
  __property int        idPadraoConstrutivo = { index=(IS_OPTN | IS_UNQL), read=FidPadraoConstrutivo, write=SetidPadraoConstrutivo, stored = idPadraoConstrutivo_Specified };
  __property double     custo = { index=(IS_OPTN | IS_UNQL), read=Fcusto, write=Setcusto, stored = custo_Specified };
};



// ************************************************************************ //
// Namespace : http://listarPadraoConstrutivo.ejb.webservices.integration.aries.sinapsisenergia.com/
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// use       : literal
// binding   : ListarPadraoConstrutivoWebServicePortBinding
// service   : ariestreina
// port      : ListarPadraoConstrutivoWebServicePort
// URL       : http://200.98.139.179:80/ariestreina/ListarPadraoConstrutivoWebService
// ************************************************************************ //
__interface INTERFACE_UUID("{51E43CDC-920A-04D0-CDEB-99131646839A}") ListarPadraoConstrutivoWebService : public IInvokable
{
public:
  virtual listarResponsePadrao  listar(const int idDominio) = 0;
};
typedef DelphiInterface<ListarPadraoConstrutivoWebService> _di_ListarPadraoConstrutivoWebService;

_di_ListarPadraoConstrutivoWebService GetListarPadraoConstrutivoWebService(bool useWSDL=false, System::String addr= System::String(), Soaphttpclient::THTTPRIO* HTTPRIO=0);
_di_ListarPadraoConstrutivoWebService GetListarPadraoConstrutivoWebService(char *addr, char *service, char *port);

};     // NS_ListarPadraoConstrutivoWebService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_ListarPadraoConstrutivoWebService;
#endif



#endif // ListarPadraoConstrutivoWebServiceH
