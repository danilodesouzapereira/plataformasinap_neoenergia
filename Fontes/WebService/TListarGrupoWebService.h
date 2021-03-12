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

#ifndef   TListarGrupoWebServiceH
#define   TListarGrupoWebServiceH

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


namespace NS_ListarGrupoWebService {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS grupo;

typedef DynamicArray<grupo*>      listarResponse; /* "http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/"[Lit][GblCplx] */


// ************************************************************************ //
// XML       : grupo, global, <complexType>
// Namespace : http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class grupo : public TRemotable {
private:
  UnicodeString   Fnome;
  bool            Fnome_Specified;
  int             Fid;
  bool            Fid_Specified;
  void __fastcall Setnome(int Index, UnicodeString _prop_val)
  {  Fnome = _prop_val; Fnome_Specified = true;  }
  bool __fastcall nome_Specified(int Index)
  {  return Fnome_Specified;  } 
  void __fastcall Setid(int Index, int _prop_val)
  {  Fid = _prop_val; Fid_Specified = true;  }
  bool __fastcall id_Specified(int Index)
  {  return Fid_Specified;  } 
__published:
  __property UnicodeString       nome = { index=(IS_OPTN | IS_UNQL), read=Fnome, write=Setnome, stored = nome_Specified };
  __property int                id = { index=(IS_OPTN | IS_UNQL), read=Fid, write=Setid, stored = id_Specified };
};



// ************************************************************************ //
// Namespace : http://listarGrupo.ejb.webservices.integration.aries.sinapsisenergia.com/
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// use       : literal
// binding   : ListarGrupoWebServicePortBinding
// service   : aries
// port      : ListarGrupoWebServicePort
// URL       : http://192.168.1.44:80/aries/ListarGrupoWebService
// ************************************************************************ //
__interface INTERFACE_UUID("{5171AF8A-C952-1F75-A3F0-A1198D48ECB7}") ListarGrupoWebService : public IInvokable
{
public:
  virtual listarResponse  listar(const int idDominio) = 0; 
};
typedef DelphiInterface<ListarGrupoWebService> _di_ListarGrupoWebService;

_di_ListarGrupoWebService GetListarGrupoWebService(bool useWSDL=false, System::String addr= System::String(), Soaphttpclient::THTTPRIO* HTTPRIO=0);
_di_ListarGrupoWebService GetListarGrupoWebService(char *addr, char *service, char *port);

};     // NS_ListarGrupoWebService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_ListarGrupoWebService;
#endif



#endif // ListarGrupoWebServiceH
