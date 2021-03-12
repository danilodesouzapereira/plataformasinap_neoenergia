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

#ifndef   TReceberEstudoWebServiceH
#define   TReceberEstudoWebServiceH

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
#if !defined(IS_NLBL)
#define IS_NLBL 0x0004
#endif
#if !defined(IS_UNQL)
#define IS_UNQL 0x0008
#endif
#if !defined(IS_ATTR)
#define IS_ATTR 0x0010
#endif


namespace NS_ReceberEstudoWebService {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:base64Binary    - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:anySimpleType   - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS estudo;
class SOAP_REMOTABLE_CLASS alternativa;
class SOAP_REMOTABLE_CLASS informacaoTecnicaComercial;
class SOAP_REMOTABLE_CLASS resposta;
class SOAP_REMOTABLE_CLASS rede;
class SOAP_REMOTABLE_CLASS municipio2;
class SOAP_REMOTABLE_CLASS municipio;
class SOAP_REMOTABLE_CLASS elementoPotencia;
class SOAP_REMOTABLE_CLASS demandaPotencia;
class SOAP_REMOTABLE_CLASS valorInformacaoTecnicaComercial;
class SOAP_REMOTABLE_CLASS redeAlternativa;
class SOAP_REMOTABLE_CLASS potenciaInstalada;
class SOAP_REMOTABLE_CLASS equipamento;
class SOAP_REMOTABLE_CLASS obra;

typedef DynamicArray<informacaoTecnicaComercial*> Array_Of_informacaoTecnicaComercial; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<alternativa*> Array_Of_alternativa; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<obra*>       Array_Of_obra;  /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<elementoPotencia*> Array_Of_elementoPotencia; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<demandaPotencia*> Array_Of_demandaPotencia; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<potenciaInstalada*> Array_Of_potenciaInstalada; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<rede*>       Array_Of_rede;  /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<valorInformacaoTecnicaComercial*> Array_Of_valorInformacaoTecnicaComercial; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<redeAlternativa*> Array_Of_redeAlternativa; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<municipio2*> Array_Of_municipio; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */
typedef DynamicArray<equipamento*> Array_Of_equipamento; /* "http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/"[GblUbnd] */


// ************************************************************************ //
// XML       : estudo, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class estudo : public TRemotable {
private:
  Array_Of_alternativa Falternativa;
  bool            Falternativa_Specified;
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  int             FidDominio;
  int             FidGrupo;
  bool            FidGrupo_Specified;
  UnicodeString   Fjustificativa;
  bool            Fjustificativa_Specified;
  UnicodeString   FusuarioSinap;
  bool            FusuarioSinap_Specified;
  void __fastcall Setalternativa(int Index, Array_Of_alternativa _prop_val)
  {  Falternativa = _prop_val; Falternativa_Specified = true;  }
  bool __fastcall alternativa_Specified(int Index)
  {  return Falternativa_Specified;  } 
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall SetidGrupo(int Index, int _prop_val)
  {  FidGrupo = _prop_val; FidGrupo_Specified = true;  }
  bool __fastcall idGrupo_Specified(int Index)
  {  return FidGrupo_Specified;  }
  void __fastcall Setjustificativa(int Index, UnicodeString _prop_val)
  {  Fjustificativa = _prop_val; Fjustificativa_Specified = true;  }
  bool __fastcall justificativa_Specified(int Index)
  {  return Fjustificativa_Specified;  } 
  void __fastcall SetusuarioSinap(int Index, UnicodeString _prop_val)
  {  FusuarioSinap = _prop_val; FusuarioSinap_Specified = true;  }
  bool __fastcall usuarioSinap_Specified(int Index)
  {  return FusuarioSinap_Specified;  } 

public:
  __fastcall ~estudo();
__published:
  __property Array_Of_alternativa alternativa = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Falternativa, write=Setalternativa, stored = alternativa_Specified };
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property int         idDominio = { index=(IS_UNQL), read=FidDominio, write=FidDominio };
  __property int           idGrupo = { index=(IS_OPTN | IS_UNQL), read=FidGrupo, write=SetidGrupo, stored = idGrupo_Specified };
  __property UnicodeString justificativa = { index=(IS_OPTN | IS_UNQL), read=Fjustificativa, write=Setjustificativa, stored = justificativa_Specified };
  __property UnicodeString usuarioSinap = { index=(IS_OPTN | IS_UNQL), read=FusuarioSinap, write=SetusuarioSinap, stored = usuarioSinap_Specified };
};




// ************************************************************************ //
// XML       : alternativa, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class alternativa : public TRemotable {
private:
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  Array_Of_informacaoTecnicaComercial FinformacaoTecnica;
  bool            FinformacaoTecnica_Specified;
  UnicodeString   Fjustificativa;
  bool            Fjustificativa_Specified;
  Array_Of_obra   Fobra;
  bool            Fobra_Specified;
  Array_Of_redeAlternativa Frede;
  bool            Frede_Specified;
  bool            FredeOriginal;
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall SetinformacaoTecnica(int Index, Array_Of_informacaoTecnicaComercial _prop_val)
  {  FinformacaoTecnica = _prop_val; FinformacaoTecnica_Specified = true;  }
  bool __fastcall informacaoTecnica_Specified(int Index)
  {  return FinformacaoTecnica_Specified;  } 
  void __fastcall Setjustificativa(int Index, UnicodeString _prop_val)
  {  Fjustificativa = _prop_val; Fjustificativa_Specified = true;  }
  bool __fastcall justificativa_Specified(int Index)
  {  return Fjustificativa_Specified;  } 
  void __fastcall Setobra(int Index, Array_Of_obra _prop_val)
  {  Fobra = _prop_val; Fobra_Specified = true;  }
  bool __fastcall obra_Specified(int Index)
  {  return Fobra_Specified;  } 
  void __fastcall Setrede(int Index, Array_Of_redeAlternativa _prop_val)
  {  Frede = _prop_val; Frede_Specified = true;  }
  bool __fastcall rede_Specified(int Index)
  {  return Frede_Specified;  } 

public:
  __fastcall ~alternativa();
__published:
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property Array_Of_informacaoTecnicaComercial informacaoTecnica = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=FinformacaoTecnica, write=SetinformacaoTecnica, stored = informacaoTecnica_Specified };
  __property UnicodeString justificativa = { index=(IS_OPTN | IS_UNQL), read=Fjustificativa, write=Setjustificativa, stored = justificativa_Specified };
  __property Array_Of_obra       obra = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Fobra, write=Setobra, stored = obra_Specified };
  __property Array_Of_redeAlternativa       rede = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Frede, write=Setrede, stored = rede_Specified };
  __property bool       redeOriginal = { index=(IS_UNQL), read=FredeOriginal, write=FredeOriginal };
};




// ************************************************************************ //
// XML       : informacaoTecnicaComercial, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class informacaoTecnicaComercial : public TRemotable {
private:
  UnicodeString   Ftipo;
  bool            Ftipo_Specified;
  Array_Of_valorInformacaoTecnicaComercial Fvalor;
  bool            Fvalor_Specified;
  void __fastcall Settipo(int Index, UnicodeString _prop_val)
  {  Ftipo = _prop_val; Ftipo_Specified = true;  }
  bool __fastcall tipo_Specified(int Index)
  {  return Ftipo_Specified;  } 
  void __fastcall Setvalor(int Index, Array_Of_valorInformacaoTecnicaComercial _prop_val)
  {  Fvalor = _prop_val; Fvalor_Specified = true;  }
  bool __fastcall valor_Specified(int Index)
  {  return Fvalor_Specified;  } 

public:
  __fastcall ~informacaoTecnicaComercial();
__published:
  __property UnicodeString       tipo = { index=(IS_OPTN | IS_UNQL), read=Ftipo, write=Settipo, stored = tipo_Specified };
  __property Array_Of_valorInformacaoTecnicaComercial      valor = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Fvalor, write=Setvalor, stored = valor_Specified };
};




// ************************************************************************ //
// XML       : resposta, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class resposta : public TRemotable {
private:
  UnicodeString   FmensagemErro;
  bool            FmensagemErro_Specified;
  bool            Frecebido;
  void __fastcall SetmensagemErro(int Index, UnicodeString _prop_val)
  {  FmensagemErro = _prop_val; FmensagemErro_Specified = true;  }
  bool __fastcall mensagemErro_Specified(int Index)
  {  return FmensagemErro_Specified;  } 
__published:
  __property UnicodeString mensagemErro = { index=(IS_OPTN | IS_UNQL), read=FmensagemErro, write=SetmensagemErro, stored = mensagemErro_Specified };
  __property bool         recebido = { index=(IS_UNQL), read=Frecebido, write=Frecebido };
};




// ************************************************************************ //
// XML       : rede, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class rede : public TRemotable {
private:
  bool            FantesManobra;
  UnicodeString   FcodigoGIS;
  bool            FcodigoGIS_Specified;
  UnicodeString   FcodigoGisSubestacao;
  bool            FcodigoGisSubestacao_Specified;
  UnicodeString   FtipoSistema;
  bool            FtipoSistema_Specified;
  void __fastcall SetcodigoGIS(int Index, UnicodeString _prop_val)
  {  FcodigoGIS = _prop_val; FcodigoGIS_Specified = true;  }
  bool __fastcall codigoGIS_Specified(int Index)
  {  return FcodigoGIS_Specified;  } 
  void __fastcall SetcodigoGisSubestacao(int Index, UnicodeString _prop_val)
  {  FcodigoGisSubestacao = _prop_val; FcodigoGisSubestacao_Specified = true;  }
  bool __fastcall codigoGisSubestacao_Specified(int Index)
  {  return FcodigoGisSubestacao_Specified;  } 
  void __fastcall SettipoSistema(int Index, UnicodeString _prop_val)
  {  FtipoSistema = _prop_val; FtipoSistema_Specified = true;  }
  bool __fastcall tipoSistema_Specified(int Index)
  {  return FtipoSistema_Specified;  } 
__published:
  __property bool       antesManobra = { index=(IS_UNQL), read=FantesManobra, write=FantesManobra };
  __property UnicodeString  codigoGIS = { index=(IS_OPTN | IS_UNQL), read=FcodigoGIS, write=SetcodigoGIS, stored = codigoGIS_Specified };
  __property UnicodeString codigoGisSubestacao = { index=(IS_OPTN | IS_UNQL), read=FcodigoGisSubestacao, write=SetcodigoGisSubestacao, stored = codigoGisSubestacao_Specified };
  __property UnicodeString tipoSistema = { index=(IS_OPTN | IS_UNQL), read=FtipoSistema, write=SettipoSistema, stored = tipoSistema_Specified };
};




// ************************************************************************ //
// XML       : municipio, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class municipio2 : public TRemotable {
private:
  bool            Fprincipal;
  int             FidMunicipio;
__published:
  __property bool        principal = { index=(IS_ATTR), read=Fprincipal, write=Fprincipal };
  __property int        idMunicipio = { index=(IS_UNQL), read=FidMunicipio, write=FidMunicipio };
};




// ************************************************************************ //
// XML       : municipio, global, <element>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class municipio : public municipio2 {
private:
__published:
};




// ************************************************************************ //
// XML       : elementoPotencia, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class elementoPotencia : public TRemotable {
private:
  UnicodeString   Fcodigo;
  bool            Fcodigo_Specified;
  int             FidExternoElementoPotencia;
  bool            FidExternoElementoPotencia_Specified;
  Array_Of_potenciaInstalada FpotenciaInstalada;
  bool            FpotenciaInstalada_Specified;
  redeAlternativa* FredeAlternativa;
  bool            FredeAlternativa_Specified;
  void __fastcall Setcodigo(int Index, UnicodeString _prop_val)
  {  Fcodigo = _prop_val; Fcodigo_Specified = true;  }
  bool __fastcall codigo_Specified(int Index)
  {  return Fcodigo_Specified;  } 
  void __fastcall SetidExternoElementoPotencia(int Index, int _prop_val)
  {  FidExternoElementoPotencia = _prop_val; FidExternoElementoPotencia_Specified = true;  }
  bool __fastcall idExternoElementoPotencia_Specified(int Index)
  {  return FidExternoElementoPotencia_Specified;  } 
  void __fastcall SetpotenciaInstalada(int Index, Array_Of_potenciaInstalada _prop_val)
  {  FpotenciaInstalada = _prop_val; FpotenciaInstalada_Specified = true;  }
  bool __fastcall potenciaInstalada_Specified(int Index)
  {  return FpotenciaInstalada_Specified;  } 
  void __fastcall SetredeAlternativa(int Index, redeAlternativa* _prop_val)
  {  FredeAlternativa = _prop_val; FredeAlternativa_Specified = true;  }
  bool __fastcall redeAlternativa_Specified(int Index)
  {  return FredeAlternativa_Specified;  } 

public:
  __fastcall ~elementoPotencia();
__published:
  __property UnicodeString     codigo = { index=(IS_OPTN | IS_UNQL), read=Fcodigo, write=Setcodigo, stored = codigo_Specified };
  __property int        idExternoElementoPotencia = { index=(IS_OPTN | IS_UNQL), read=FidExternoElementoPotencia, write=SetidExternoElementoPotencia, stored = idExternoElementoPotencia_Specified };
  __property Array_Of_potenciaInstalada potenciaInstalada = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=FpotenciaInstalada, write=SetpotenciaInstalada, stored = potenciaInstalada_Specified };
  __property redeAlternativa* redeAlternativa = { index=(IS_OPTN | IS_UNQL), read=FredeAlternativa, write=SetredeAlternativa, stored = redeAlternativa_Specified };
};




// ************************************************************************ //
// XML       : demandaPotencia, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class demandaPotencia : public TRemotable {
private:
  int             Fano;
  double          Fvalor;
__published:
  __property int               ano = { index=(IS_UNQL), read=Fano, write=Fano };
  __property double          valor = { index=(IS_UNQL), read=Fvalor, write=Fvalor };
};




// ************************************************************************ //
// XML       : valorInformacaoTecnicaComercial, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class valorInformacaoTecnicaComercial : public TRemotable {
private:
  int             Fano;
  double          Fvalor;
__published:
  __property int               ano = { index=(IS_UNQL), read=Fano, write=Fano };
  __property double          valor = { index=(IS_UNQL), read=Fvalor, write=Fvalor };
};




// ************************************************************************ //
// XML       : redeAlternativa, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class redeAlternativa : public TRemotable {
private:
  UnicodeString   FcodigoGIS;
  bool            FcodigoGIS_Specified;
  Array_Of_demandaPotencia FdemandaPotencia;
  bool            FdemandaPotencia_Specified;
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  Array_Of_elementoPotencia FelementoPotencia;
  bool            FelementoPotencia_Specified;
  double          FtaxaCrescimentoAnual;
  UnicodeString   FtipoSistema;
  bool            FtipoSistema_Specified;
  void __fastcall SetcodigoGIS(int Index, UnicodeString _prop_val)
  {  FcodigoGIS = _prop_val; FcodigoGIS_Specified = true;  }
  bool __fastcall codigoGIS_Specified(int Index)
  {  return FcodigoGIS_Specified;  } 
  void __fastcall SetdemandaPotencia(int Index, Array_Of_demandaPotencia _prop_val)
  {  FdemandaPotencia = _prop_val; FdemandaPotencia_Specified = true;  }
  bool __fastcall demandaPotencia_Specified(int Index)
  {  return FdemandaPotencia_Specified;  } 
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall SetelementoPotencia(int Index, Array_Of_elementoPotencia _prop_val)
  {  FelementoPotencia = _prop_val; FelementoPotencia_Specified = true;  }
  bool __fastcall elementoPotencia_Specified(int Index)
  {  return FelementoPotencia_Specified;  } 
  void __fastcall SettipoSistema(int Index, UnicodeString _prop_val)
  {  FtipoSistema = _prop_val; FtipoSistema_Specified = true;  }
  bool __fastcall tipoSistema_Specified(int Index)
  {  return FtipoSistema_Specified;  } 

public:
  __fastcall ~redeAlternativa();
__published:
  __property UnicodeString  codigoGIS = { index=(IS_OPTN | IS_UNQL), read=FcodigoGIS, write=SetcodigoGIS, stored = codigoGIS_Specified };
  __property Array_Of_demandaPotencia demandaPotencia = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=FdemandaPotencia, write=SetdemandaPotencia, stored = demandaPotencia_Specified };
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property Array_Of_elementoPotencia elementoPotencia = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=FelementoPotencia, write=SetelementoPotencia, stored = elementoPotencia_Specified };
  __property double     taxaCrescimentoAnual = { index=(IS_UNQL), read=FtaxaCrescimentoAnual, write=FtaxaCrescimentoAnual };
  __property UnicodeString tipoSistema = { index=(IS_OPTN | IS_UNQL), read=FtipoSistema, write=SettipoSistema, stored = tipoSistema_Specified };
};




// ************************************************************************ //
// XML       : potenciaInstalada, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class potenciaInstalada : public TRemotable {
private:
  int             Fano;
  double          Fvalor;
__published:
  __property int               ano = { index=(IS_UNQL), read=Fano, write=Fano };
  __property double          valor = { index=(IS_UNQL), read=Fvalor, write=Fvalor };
};




// ************************************************************************ //
// XML       : equipamento, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class equipamento : public TRemotable {
private:
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  int             FidPadraoConstrutivo;
  double          Fquantidade;
  Array_Of_rede   Frede;
  bool            Frede_Specified;
  UnicodeString   FtipoAcao;
  bool            FtipoAcao_Specified;
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall Setrede(int Index, Array_Of_rede _prop_val)
  {  Frede = _prop_val; Frede_Specified = true;  }
  bool __fastcall rede_Specified(int Index)
  {  return Frede_Specified;  } 
  void __fastcall SettipoAcao(int Index, UnicodeString _prop_val)
  {  FtipoAcao = _prop_val; FtipoAcao_Specified = true;  }
  bool __fastcall tipoAcao_Specified(int Index)
  {  return FtipoAcao_Specified;  } 

public:
  __fastcall ~equipamento();
__published:
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property int        idPadraoConstrutivo = { index=(IS_UNQL), read=FidPadraoConstrutivo, write=FidPadraoConstrutivo };
  __property double     quantidade = { index=(IS_UNQL), read=Fquantidade, write=Fquantidade };
  __property Array_Of_rede       rede = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Frede, write=Setrede, stored = rede_Specified };
  __property UnicodeString   tipoAcao = { index=(IS_OPTN | IS_UNQL), read=FtipoAcao, write=SettipoAcao, stored = tipoAcao_Specified };
};




// ************************************************************************ //
// XML       : obra, global, <complexType>
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// ************************************************************************ //
class obra : public TRemotable {
private:
  UnicodeString   Fcodigo;
  bool            Fcodigo_Specified;
  int             Fconjunto_obra;
  bool            Fconjunto_obra_Specified;
  TByteDynArray   Fcroqui;
  bool            Fcroqui_Specified;
  Variant         FdataAtivacao;
  bool            FdataAtivacao_Specified;
  Variant         FdataInicio;
  bool            FdataInicio_Specified;
  UnicodeString   Fdescricao;
  bool            Fdescricao_Specified;
  Array_Of_equipamento Fequipamento;
  bool            Fequipamento_Specified;
  int             Fgrupo;
  bool            Fgrupo_Specified;
  Array_Of_municipio Fmunicipio;
  bool            Fmunicipio_Specified;
  double          FobrigacaoEspecial;
  double          FparticipacaoOtr;
  UnicodeString   Fprograma;
  bool            Fprograma_Specified;
  UnicodeString   FtipoObra;
  bool            FtipoObra_Specified;
  void __fastcall Setcodigo(int Index, UnicodeString _prop_val)
  {  Fcodigo = _prop_val; Fcodigo_Specified = true;  }
  bool __fastcall codigo_Specified(int Index)
  {  return Fcodigo_Specified;  } 
  void __fastcall Setconjunto_obra(int Index, int _prop_val)
  {  Fconjunto_obra = _prop_val; Fconjunto_obra_Specified = true;  }
  bool __fastcall conjunto_obra_Specified(int Index)
  {  return Fconjunto_obra_Specified;  } 
  void __fastcall Setcroqui(int Index, TByteDynArray _prop_val)
  {  Fcroqui = _prop_val; Fcroqui_Specified = true;  }
  bool __fastcall croqui_Specified(int Index)
  {  return Fcroqui_Specified;  } 
  void __fastcall SetdataAtivacao(int Index, Variant _prop_val)
  {  FdataAtivacao = _prop_val; FdataAtivacao_Specified = true;  }
  bool __fastcall dataAtivacao_Specified(int Index)
  {  return FdataAtivacao_Specified;  } 
  void __fastcall SetdataInicio(int Index, Variant _prop_val)
  {  FdataInicio = _prop_val; FdataInicio_Specified = true;  }
  bool __fastcall dataInicio_Specified(int Index)
  {  return FdataInicio_Specified;  } 
  void __fastcall Setdescricao(int Index, UnicodeString _prop_val)
  {  Fdescricao = _prop_val; Fdescricao_Specified = true;  }
  bool __fastcall descricao_Specified(int Index)
  {  return Fdescricao_Specified;  } 
  void __fastcall Setequipamento(int Index, Array_Of_equipamento _prop_val)
  {  Fequipamento = _prop_val; Fequipamento_Specified = true;  }
  bool __fastcall equipamento_Specified(int Index)
  {  return Fequipamento_Specified;  } 
  void __fastcall Setgrupo(int Index, int _prop_val)
  {  Fgrupo = _prop_val; Fgrupo_Specified = true;  }
  bool __fastcall grupo_Specified(int Index)
  {  return Fgrupo_Specified;  } 
  void __fastcall Setmunicipio(int Index, Array_Of_municipio _prop_val)
  {  Fmunicipio = _prop_val; Fmunicipio_Specified = true;  }
  bool __fastcall municipio_Specified(int Index)
  {  return Fmunicipio_Specified;  } 
  void __fastcall Setprograma(int Index, UnicodeString _prop_val)
  {  Fprograma = _prop_val; Fprograma_Specified = true;  }
  bool __fastcall programa_Specified(int Index)
  {  return Fprograma_Specified;  } 
  void __fastcall SettipoObra(int Index, UnicodeString _prop_val)
  {  FtipoObra = _prop_val; FtipoObra_Specified = true;  }
  bool __fastcall tipoObra_Specified(int Index)
  {  return FtipoObra_Specified;  } 

public:
  __fastcall ~obra();
__published:
  __property UnicodeString     codigo = { index=(IS_OPTN | IS_UNQL), read=Fcodigo, write=Setcodigo, stored = codigo_Specified };
  __property int        conjunto_obra = { index=(IS_OPTN | IS_UNQL), read=Fconjunto_obra, write=Setconjunto_obra, stored = conjunto_obra_Specified };
  __property TByteDynArray     croqui = { index=(IS_OPTN | IS_UNQL), read=Fcroqui, write=Setcroqui, stored = croqui_Specified };
  __property Variant    dataAtivacao = { index=(IS_OPTN | IS_UNQL), read=FdataAtivacao, write=SetdataAtivacao, stored = dataAtivacao_Specified };
  __property Variant    dataInicio = { index=(IS_OPTN | IS_UNQL), read=FdataInicio, write=SetdataInicio, stored = dataInicio_Specified };
  __property UnicodeString  descricao = { index=(IS_OPTN | IS_UNQL), read=Fdescricao, write=Setdescricao, stored = descricao_Specified };
  __property Array_Of_equipamento equipamento = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Fequipamento, write=Setequipamento, stored = equipamento_Specified };
  __property int             grupo = { index=(IS_OPTN | IS_UNQL), read=Fgrupo, write=Setgrupo, stored = grupo_Specified };
  __property Array_Of_municipio  municipio = { index=(IS_OPTN | IS_UNBD | IS_NLBL | IS_UNQL), read=Fmunicipio, write=Setmunicipio, stored = municipio_Specified };
  __property double     obrigacaoEspecial = { index=(IS_UNQL), read=FobrigacaoEspecial, write=FobrigacaoEspecial };
  __property double     participacaoOtr = { index=(IS_UNQL), read=FparticipacaoOtr, write=FparticipacaoOtr };
  __property UnicodeString   programa = { index=(IS_OPTN | IS_UNQL), read=Fprograma, write=Setprograma, stored = programa_Specified };
  __property UnicodeString   tipoObra = { index=(IS_OPTN | IS_UNQL), read=FtipoObra, write=SettipoObra, stored = tipoObra_Specified };
};



// ************************************************************************ //
// Namespace : http://receberestudo.ejb.webservices.integration.aries.sinapsisenergia.com/
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// use       : literal
// binding   : ReceberEstudoWebServicePortBinding
// service   : aries
// port      : ReceberEstudoWebServicePort
// URL       : http://192.168.1.44:80/aries/ReceberEstudoWebService
// ************************************************************************ //
__interface INTERFACE_UUID("{BD5AC428-427F-DBE9-D6F2-D7C2FC138AA7}") ReceberEstudoWebService : public IInvokable
{
public:
  virtual resposta*       receber(const estudo* estudo) = 0; 
};
typedef DelphiInterface<ReceberEstudoWebService> _di_ReceberEstudoWebService;

//_di_ReceberEstudoWebService GetReceberEstudoWebService(bool useWSDL=false, System::String addr= System::String(), Soaphttpclient::THTTPRIO* HTTPRIO=0);
_di_ReceberEstudoWebService GetReceberEstudoWebService(char *addr, char *service, char *port);


};     // NS_ReceberEstudoWebService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_ReceberEstudoWebService;
#endif



#endif // ReceberEstudoWebServiceH
