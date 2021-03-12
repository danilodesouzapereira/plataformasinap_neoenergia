// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://www.sinapsisenergia.com/licenca/wsSinap.php?wsdl
//  >Import : http://www.sinapsisenergia.com/licenca/wsSinap.php?wsdl>0
// Encoding : ISO-8859-1
// Version  : 1.0
// (08/05/2014 09:02:01 - - $Rev: 56641 $)
// ************************************************************************ //

#ifndef   wsSinapH
#define   wsSinapH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <SOAPHTTPClient.hpp>

#if !defined(SOAP_REMOTABLE_CLASS)
#define SOAP_REMOTABLE_CLASS __declspec(delphiclass)
#endif

namespace NS_wsSinap {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS Licenca;



// ************************************************************************ //
// XML       : Licenca, global, <complexType>
// Namespace : urn:SinapLic
// ************************************************************************ //
class Licenca : public TRemotable {
private:
  int             Fid;
  UnicodeString   Fcpu_id;
  UnicodeString   Fproduto;
  UnicodeString   Flogin;
  UnicodeString   Fpassword;
  UnicodeString   Fdh_alocada;
  UnicodeString   Fdh_inicio;
  UnicodeString   Fdh_controle;
  UnicodeString   Fdh_termino;
  int             Fstatus;
  int             Ftipo;
  int             Fvalidade;
  int             Frenovada;
  bool            FloginLocal;
  bool            FctrlRemoto;
  int             Fempresa_id;
__published:
  __property int                id = { read=Fid, write=Fid };
  __property UnicodeString     cpu_id = { read=Fcpu_id, write=Fcpu_id };
  __property UnicodeString    produto = { read=Fproduto, write=Fproduto };
  __property UnicodeString      login = { read=Flogin, write=Flogin };
  __property UnicodeString   password = { read=Fpassword, write=Fpassword };
  __property UnicodeString dh_alocada = { read=Fdh_alocada, write=Fdh_alocada };
  __property UnicodeString  dh_inicio = { read=Fdh_inicio, write=Fdh_inicio };
  __property UnicodeString dh_controle = { read=Fdh_controle, write=Fdh_controle };
  __property UnicodeString dh_termino = { read=Fdh_termino, write=Fdh_termino };
  __property int            status = { read=Fstatus, write=Fstatus };
  __property int              tipo = { read=Ftipo, write=Ftipo };
  __property int          validade = { read=Fvalidade, write=Fvalidade };
  __property int          renovada = { read=Frenovada, write=Frenovada };
  __property bool       loginLocal = { read=FloginLocal, write=FloginLocal };
  __property bool       ctrlRemoto = { read=FctrlRemoto, write=FctrlRemoto };
  __property int        empresa_id = { read=Fempresa_id, write=Fempresa_id };
};



// ************************************************************************ //
// Namespace : urn:SinapLic
// soapAction: |urn:SinapLic#AtualizaLicenca|urn:SinapLic#Conecta|urn:SinapLic#LiberaLicenca
// transport : http://schemas.xmlsoap.org/soap/http
// style     : rpc
// use       : encoded
// binding   : SinapServerBinding
// service   : SinapServer
// port      : SinapServerPort
// URL       : http://www.sinapsisenergia.com/licenca/wsSinap.php
// ************************************************************************ //
__interface INTERFACE_UUID("{48965668-0AC8-0A8A-29F8-1AF7846B9C78}") SinapServerPortType : public IInvokable
{
public:
  virtual int             AtualizaLicenca(const Licenca* lic) = 0;
  virtual int             Conecta(const UnicodeString login, const UnicodeString pass) = 0;
  virtual Licenca*        LeLicenca(const Licenca* lic) = 0;
};
typedef DelphiInterface<SinapServerPortType> _di_SinapServerPortType;

_di_SinapServerPortType GetSinapServerPortType(bool useWSDL=false, System::String addr= System::String(), Soaphttpclient::THTTPRIO* HTTPRIO=0);


};     // NS_wsSinap

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_wsSinap;
#endif



#endif // wsSinapH

