
// *************************************************************************************************************** //
//                                                                                                               
//                                               XML Data Binding                                                
//                                                                                                               
//         Generated on: 24/06/2014 07:05:19                                                                     
//       Generated from: Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\InterfaceXML\Modelo\SinapModeloRede_v52.xsd   
//   Settings stored in: Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\InterfaceXML\Modelo\SinapModeloRede_v52.xdb   
//                                                                                                               
// *************************************************************************************************************** //

#ifndef   SinapModeloRede_v52H
#define   SinapModeloRede_v52H

#include <System.hpp>
#include <Xml.Xmldom.hpp>
#include <Xml.XMLIntf.hpp>
#include <Xml.XMLDoc.hpp>
#include <XMLNodeImp.h>


// Forward Decls 

__interface IXMLTConcessionaria;
typedef System::DelphiInterface<IXMLTConcessionaria> _di_IXMLTConcessionaria;
__interface IXMLTId;
typedef System::DelphiInterface<IXMLTId> _di_IXMLTId;
__interface IXMLTRede;
typedef System::DelphiInterface<IXMLTRede> _di_IXMLTRede;
__interface IXMLTRedeList;
typedef System::DelphiInterface<IXMLTRedeList> _di_IXMLTRedeList;
__interface IXMLTCnjEqBar;
typedef System::DelphiInterface<IXMLTCnjEqBar> _di_IXMLTCnjEqBar;
__interface IXMLTEqBarCapacitor;
typedef System::DelphiInterface<IXMLTEqBarCapacitor> _di_IXMLTEqBarCapacitor;
__interface IXMLTEqBarCapacitorList;
typedef System::DelphiInterface<IXMLTEqBarCapacitorList> _di_IXMLTEqBarCapacitorList;
__interface IXMLTEqpto;
typedef System::DelphiInterface<IXMLTEqpto> _di_IXMLTEqpto;
__interface IXMLTCoordId;
typedef System::DelphiInterface<IXMLTCoordId> _di_IXMLTCoordId;
__interface IXMLTCoordIdList;
typedef System::DelphiInterface<IXMLTCoordIdList> _di_IXMLTCoordIdList;
__interface IXMLTCoord;
typedef System::DelphiInterface<IXMLTCoord> _di_IXMLTCoord;
__interface IXMLTCoordIco;
typedef System::DelphiInterface<IXMLTCoordIco> _di_IXMLTCoordIco;
__interface IXMLTMedicao;
typedef System::DelphiInterface<IXMLTMedicao> _di_IXMLTMedicao;
__interface IXMLTMedicaoList;
typedef System::DelphiInterface<IXMLTMedicaoList> _di_IXMLTMedicaoList;
__interface IXMLTPatamProp;
typedef System::DelphiInterface<IXMLTPatamProp> _di_IXMLTPatamProp;
__interface IXMLTPatamPropList;
typedef System::DelphiInterface<IXMLTPatamPropList> _di_IXMLTPatamPropList;
__interface IXMLTHorMin;
typedef System::DelphiInterface<IXMLTHorMin> _di_IXMLTHorMin;
__interface IXMLTEqBarCarga;
typedef System::DelphiInterface<IXMLTEqBarCarga> _di_IXMLTEqBarCarga;
__interface IXMLTEqBarCargaList;
typedef System::DelphiInterface<IXMLTEqBarCargaList> _di_IXMLTEqBarCargaList;
__interface IXMLTEqBarCargaIP;
typedef System::DelphiInterface<IXMLTEqBarCargaIP> _di_IXMLTEqBarCargaIP;
__interface IXMLTEqBarCargaIPList;
typedef System::DelphiInterface<IXMLTEqBarCargaIPList> _di_IXMLTEqBarCargaIPList;
__interface IXMLTEqBarFiltro;
typedef System::DelphiInterface<IXMLTEqBarFiltro> _di_IXMLTEqBarFiltro;
__interface IXMLTEqBarFiltroList;
typedef System::DelphiInterface<IXMLTEqBarFiltroList> _di_IXMLTEqBarFiltroList;
__interface IXMLTZ;
typedef System::DelphiInterface<IXMLTZ> _di_IXMLTZ;
__interface IXMLTEqBarGerador;
typedef System::DelphiInterface<IXMLTEqBarGerador> _di_IXMLTEqBarGerador;
__interface IXMLTEqBarGeradorList;
typedef System::DelphiInterface<IXMLTEqBarGeradorList> _di_IXMLTEqBarGeradorList;
__interface IXMLTZseq;
typedef System::DelphiInterface<IXMLTZseq> _di_IXMLTZseq;
__interface IXMLTEqBarReator;
typedef System::DelphiInterface<IXMLTEqBarReator> _di_IXMLTEqBarReator;
__interface IXMLTEqBarReatorList;
typedef System::DelphiInterface<IXMLTEqBarReatorList> _di_IXMLTEqBarReatorList;
__interface IXMLTEqBarSuprimento;
typedef System::DelphiInterface<IXMLTEqBarSuprimento> _di_IXMLTEqBarSuprimento;
__interface IXMLTEqBarSuprimentoList;
typedef System::DelphiInterface<IXMLTEqBarSuprimentoList> _di_IXMLTEqBarSuprimentoList;
__interface IXMLTPotCpx;
typedef System::DelphiInterface<IXMLTPotCpx> _di_IXMLTPotCpx;
__interface IXMLTEqBarTrafoZZ;
typedef System::DelphiInterface<IXMLTEqBarTrafoZZ> _di_IXMLTEqBarTrafoZZ;
__interface IXMLTEqBarTrafoZZList;
typedef System::DelphiInterface<IXMLTEqBarTrafoZZList> _di_IXMLTEqBarTrafoZZList;
__interface IXMLTEqBarMedidor;
typedef System::DelphiInterface<IXMLTEqBarMedidor> _di_IXMLTEqBarMedidor;
__interface IXMLTEqBarMedidorList;
typedef System::DelphiInterface<IXMLTEqBarMedidorList> _di_IXMLTEqBarMedidorList;
__interface IXMLTCnjEqLig;
typedef System::DelphiInterface<IXMLTCnjEqLig> _di_IXMLTCnjEqLig;
__interface IXMLTEqLigCapacitorSerie;
typedef System::DelphiInterface<IXMLTEqLigCapacitorSerie> _di_IXMLTEqLigCapacitorSerie;
__interface IXMLTEqLigCapacitorSerieList;
typedef System::DelphiInterface<IXMLTEqLigCapacitorSerieList> _di_IXMLTEqLigCapacitorSerieList;
__interface IXMLTEqLigChave;
typedef System::DelphiInterface<IXMLTEqLigChave> _di_IXMLTEqLigChave;
__interface IXMLTEqLigChaveList;
typedef System::DelphiInterface<IXMLTEqLigChaveList> _di_IXMLTEqLigChaveList;
__interface IXMLTEqLigRegulador;
typedef System::DelphiInterface<IXMLTEqLigRegulador> _di_IXMLTEqLigRegulador;
__interface IXMLTEqLigReguladorList;
typedef System::DelphiInterface<IXMLTEqLigReguladorList> _di_IXMLTEqLigReguladorList;
__interface IXMLTRegTapFix;
typedef System::DelphiInterface<IXMLTRegTapFix> _di_IXMLTRegTapFix;
__interface IXMLTRegTapAjs;
typedef System::DelphiInterface<IXMLTRegTapAjs> _di_IXMLTRegTapAjs;
__interface IXMLTEqLigTrafo;
typedef System::DelphiInterface<IXMLTEqLigTrafo> _di_IXMLTEqLigTrafo;
__interface IXMLTEqLigTrafoList;
typedef System::DelphiInterface<IXMLTEqLigTrafoList> _di_IXMLTEqLigTrafoList;
__interface IXMLTEnrol;
typedef System::DelphiInterface<IXMLTEnrol> _di_IXMLTEnrol;
__interface IXMLTEnrolList;
typedef System::DelphiInterface<IXMLTEnrolList> _di_IXMLTEnrolList;
__interface IXMLTLtc;
typedef System::DelphiInterface<IXMLTLtc> _di_IXMLTLtc;
__interface IXMLTEqLigTrafo3E;
typedef System::DelphiInterface<IXMLTEqLigTrafo3E> _di_IXMLTEqLigTrafo3E;
__interface IXMLTEqLigTrafo3EList;
typedef System::DelphiInterface<IXMLTEqLigTrafo3EList> _di_IXMLTEqLigTrafo3EList;
__interface IXMLTEqLigTrecho;
typedef System::DelphiInterface<IXMLTEqLigTrecho> _di_IXMLTEqLigTrecho;
__interface IXMLTEqLigTrechoList;
typedef System::DelphiInterface<IXMLTEqLigTrechoList> _di_IXMLTEqLigTrechoList;
__interface IXMLTTrcTpo;
typedef System::DelphiInterface<IXMLTTrcTpo> _di_IXMLTTrcTpo;
__interface IXMLTTrcTpo01;
typedef System::DelphiInterface<IXMLTTrcTpo01> _di_IXMLTTrcTpo01;
__interface IXMLTCaboId;
typedef System::DelphiInterface<IXMLTCaboId> _di_IXMLTCaboId;
__interface IXMLTTrcTpo02;
typedef System::DelphiInterface<IXMLTTrcTpo02> _di_IXMLTTrcTpo02;
__interface IXMLTTrcTpo03;
typedef System::DelphiInterface<IXMLTTrcTpo03> _di_IXMLTTrcTpo03;
__interface IXMLTCSeq;
typedef System::DelphiInterface<IXMLTCSeq> _di_IXMLTCSeq;
__interface IXMLTMutua;
typedef System::DelphiInterface<IXMLTMutua> _di_IXMLTMutua;
__interface IXMLTMutuaList;
typedef System::DelphiInterface<IXMLTMutuaList> _di_IXMLTMutuaList;
__interface IXMLTMutuaTre;
typedef System::DelphiInterface<IXMLTMutuaTre> _di_IXMLTMutuaTre;
__interface IXMLTMutuaTreList;
typedef System::DelphiInterface<IXMLTMutuaTreList> _di_IXMLTMutuaTreList;
__interface IXMLTCaboRGMR;
typedef System::DelphiInterface<IXMLTCaboRGMR> _di_IXMLTCaboRGMR;
__interface IXMLTCaboRGMRList;
typedef System::DelphiInterface<IXMLTCaboRGMRList> _di_IXMLTCaboRGMRList;
__interface IXMLTCaboZ0Z1;
typedef System::DelphiInterface<IXMLTCaboZ0Z1> _di_IXMLTCaboZ0Z1;
__interface IXMLTCaboZ0Z1List;
typedef System::DelphiInterface<IXMLTCaboZ0Z1List> _di_IXMLTCaboZ0Z1List;
__interface IXMLTSuporte;
typedef System::DelphiInterface<IXMLTSuporte> _di_IXMLTSuporte;
__interface IXMLTSuporteList;
typedef System::DelphiInterface<IXMLTSuporteList> _di_IXMLTSuporteList;
__interface IXMLTSuportePtoFix;
typedef System::DelphiInterface<IXMLTSuportePtoFix> _di_IXMLTSuportePtoFix;
__interface IXMLTSuportePtoFixList;
typedef System::DelphiInterface<IXMLTSuportePtoFixList> _di_IXMLTSuportePtoFixList;
__interface IXMLTArranjoZ0Z1;
typedef System::DelphiInterface<IXMLTArranjoZ0Z1> _di_IXMLTArranjoZ0Z1;
__interface IXMLTArranjoZ0Z1List;
typedef System::DelphiInterface<IXMLTArranjoZ0Z1List> _di_IXMLTArranjoZ0Z1List;
__interface IXMLTArranjoCaboZ0Z1;
typedef System::DelphiInterface<IXMLTArranjoCaboZ0Z1> _di_IXMLTArranjoCaboZ0Z1;
__interface IXMLTArranjoCaboZ0Z1List;
typedef System::DelphiInterface<IXMLTArranjoCaboZ0Z1List> _di_IXMLTArranjoCaboZ0Z1List;
__interface IXMLTArranjoCaboRGMR;
typedef System::DelphiInterface<IXMLTArranjoCaboRGMR> _di_IXMLTArranjoCaboRGMR;
__interface IXMLTArranjoCaboRGMRList;
typedef System::DelphiInterface<IXMLTArranjoCaboRGMRList> _di_IXMLTArranjoCaboRGMRList;
__interface IXMLTSuportePtoFixCabo;
typedef System::DelphiInterface<IXMLTSuportePtoFixCabo> _di_IXMLTSuportePtoFixCabo;
__interface IXMLTSuportePtoFixCaboList;
typedef System::DelphiInterface<IXMLTSuportePtoFixCaboList> _di_IXMLTSuportePtoFixCaboList;
__interface IXMLTCurvaTipica;
typedef System::DelphiInterface<IXMLTCurvaTipica> _di_IXMLTCurvaTipica;
__interface IXMLTCurvaTipicaList;
typedef System::DelphiInterface<IXMLTCurvaTipicaList> _di_IXMLTCurvaTipicaList;
__interface IXMLTPatamTip;
typedef System::DelphiInterface<IXMLTPatamTip> _di_IXMLTPatamTip;
__interface IXMLTPatamTipList;
typedef System::DelphiInterface<IXMLTPatamTipList> _di_IXMLTPatamTipList;
__interface IXMLTZId;
typedef System::DelphiInterface<IXMLTZId> _di_IXMLTZId;
__interface IXMLdouble_List;
typedef System::DelphiInterface<IXMLdouble_List> _di_IXMLdouble_List;

// IXMLTConcessionaria 

__interface INTERFACE_UUID("{8D6BA58E-AD79-479B-90B2-F4CCB9EB5B71}") IXMLTConcessionaria : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual System::UnicodeString __fastcall Get_strVer() = 0;
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual _di_IXMLTRedeList __fastcall Get_Rede() = 0;
  virtual _di_IXMLTCaboRGMRList __fastcall Get_CaboRGMR() = 0;
  virtual _di_IXMLTCaboZ0Z1List __fastcall Get_CaboZ0Z1() = 0;
  virtual _di_IXMLTSuporteList __fastcall Get_Suporte() = 0;
  virtual _di_IXMLTArranjoZ0Z1List __fastcall Get_ArranjoZ0Z1() = 0;
  virtual _di_IXMLTArranjoCaboZ0Z1List __fastcall Get_ArranjoCaboZ0Z1() = 0;
  virtual _di_IXMLTArranjoCaboRGMRList __fastcall Get_ArranjoCaboRGMR() = 0;
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCarga() = 0;
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCargaIP() = 0;
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCapacitor() = 0;
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaReator() = 0;
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaGeradorFoto() = 0;
  virtual void __fastcall Set_strVer(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property System::UnicodeString strVer = { read=Get_strVer, write=Set_strVer };
  __property _di_IXMLTId id = { read=Get_id };
  __property _di_IXMLTRedeList Rede = { read=Get_Rede };
  __property _di_IXMLTCaboRGMRList CaboRGMR = { read=Get_CaboRGMR };
  __property _di_IXMLTCaboZ0Z1List CaboZ0Z1 = { read=Get_CaboZ0Z1 };
  __property _di_IXMLTSuporteList Suporte = { read=Get_Suporte };
  __property _di_IXMLTArranjoZ0Z1List ArranjoZ0Z1 = { read=Get_ArranjoZ0Z1 };
  __property _di_IXMLTArranjoCaboZ0Z1List ArranjoCaboZ0Z1 = { read=Get_ArranjoCaboZ0Z1 };
  __property _di_IXMLTArranjoCaboRGMRList ArranjoCaboRGMR = { read=Get_ArranjoCaboRGMR };
  __property _di_IXMLTCurvaTipicaList CurvaTipicaCarga = { read=Get_CurvaTipicaCarga };
  __property _di_IXMLTCurvaTipicaList CurvaTipicaCargaIP = { read=Get_CurvaTipicaCargaIP };
  __property _di_IXMLTCurvaTipicaList CurvaTipicaCapacitor = { read=Get_CurvaTipicaCapacitor };
  __property _di_IXMLTCurvaTipicaList CurvaTipicaReator = { read=Get_CurvaTipicaReator };
  __property _di_IXMLTCurvaTipicaList CurvaTipicaGeradorFoto = { read=Get_CurvaTipicaGeradorFoto };
};

// IXMLTId 

__interface INTERFACE_UUID("{B9DD90B5-BBBD-4A2D-87C3-6DBD95F893D9}") IXMLTId : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intId() = 0;
  virtual System::UnicodeString __fastcall Get_strCod() = 0;
  virtual System::UnicodeString __fastcall Get_strDsc() = 0;
  virtual System::UnicodeString __fastcall Get_strExtId() = 0;
  virtual void __fastcall Set_intId(int Value) = 0;
  virtual void __fastcall Set_strCod(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_strDsc(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_strExtId(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property int intId = { read=Get_intId, write=Set_intId };
  __property System::UnicodeString strCod = { read=Get_strCod, write=Set_strCod };
  __property System::UnicodeString strDsc = { read=Get_strDsc, write=Set_strDsc };
  __property System::UnicodeString strExtId = { read=Get_strExtId, write=Set_strExtId };
};

// IXMLTRede 

__interface INTERFACE_UUID("{C34380DE-1326-4363-9BED-1011AA4C3C27}") IXMLTRede : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strTpo() = 0;
  virtual _di_IXMLTCnjEqBar __fastcall Get_cnjEqBar() = 0;
  virtual _di_IXMLTCnjEqLig __fastcall Get_cnjEqLig() = 0;
  virtual _di_IXMLTMutuaList __fastcall Get_Mutua() = 0;
  virtual System::UnicodeString __fastcall Get_strDominio() = 0;
  virtual System::UnicodeString __fastcall Get_strCluster() = 0;
  virtual void __fastcall Set_strTpo(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_strDominio(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_strCluster(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strTpo = { read=Get_strTpo, write=Set_strTpo };
  __property _di_IXMLTCnjEqBar cnjEqBar = { read=Get_cnjEqBar };
  __property _di_IXMLTCnjEqLig cnjEqLig = { read=Get_cnjEqLig };
  __property _di_IXMLTMutuaList Mutua = { read=Get_Mutua };
  __property System::UnicodeString strDominio = { read=Get_strDominio, write=Set_strDominio };
  __property System::UnicodeString strCluster = { read=Get_strCluster, write=Set_strCluster };
};

// IXMLTRedeList 

__interface INTERFACE_UUID("{49201B83-7571-4A2A-9ACD-F908F6E13B13}") IXMLTRedeList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTRede __fastcall Add() = 0;
  virtual _di_IXMLTRede __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTRede __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTRede Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCnjEqBar 

__interface INTERFACE_UUID("{C2F15273-D956-4507-9D4C-2428F428CCA9}") IXMLTCnjEqBar : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqBarCapacitorList __fastcall Get_Capacitor() = 0;
  virtual _di_IXMLTEqBarCargaList __fastcall Get_Carga() = 0;
  virtual _di_IXMLTEqBarCargaIPList __fastcall Get_CargaIP() = 0;
  virtual _di_IXMLTEqBarFiltroList __fastcall Get_Filtro() = 0;
  virtual _di_IXMLTEqBarGeradorList __fastcall Get_Gerador() = 0;
  virtual _di_IXMLTEqBarReatorList __fastcall Get_Reator() = 0;
  virtual _di_IXMLTEqBarSuprimentoList __fastcall Get_Suprimento() = 0;
  virtual _di_IXMLTEqBarTrafoZZList __fastcall Get_TrafoZZ() = 0;
  virtual _di_IXMLTEqBarMedidorList __fastcall Get_Medidor() = 0;
  // Methods & Properties 
  __property _di_IXMLTEqBarCapacitorList Capacitor = { read=Get_Capacitor };
  __property _di_IXMLTEqBarCargaList Carga = { read=Get_Carga };
  __property _di_IXMLTEqBarCargaIPList CargaIP = { read=Get_CargaIP };
  __property _di_IXMLTEqBarFiltroList Filtro = { read=Get_Filtro };
  __property _di_IXMLTEqBarGeradorList Gerador = { read=Get_Gerador };
  __property _di_IXMLTEqBarReatorList Reator = { read=Get_Reator };
  __property _di_IXMLTEqBarSuprimentoList Suprimento = { read=Get_Suprimento };
  __property _di_IXMLTEqBarTrafoZZList TrafoZZ = { read=Get_TrafoZZ };
  __property _di_IXMLTEqBarMedidorList Medidor = { read=Get_Medidor };
};

// IXMLTEqBarCapacitor 

__interface INTERFACE_UUID("{0FF013E2-98BD-45B6-B752-E73DE0AA32A3}") IXMLTEqBarCapacitor : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotRtv() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotRtv(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotRtv = { read=Get_dblPotRtv, write=Set_dblPotRtv };
};

// IXMLTEqBarCapacitorList 

__interface INTERFACE_UUID("{B8B08232-4351-4CDB-9DB4-1C40255F99C9}") IXMLTEqBarCapacitorList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarCapacitor __fastcall Add() = 0;
  virtual _di_IXMLTEqBarCapacitor __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarCapacitor __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarCapacitor Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqpto 

__interface INTERFACE_UUID("{23331BFA-0E55-450A-8FFE-1A42651489D5}") IXMLTEqpto : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual _di_IXMLTCoordIdList __fastcall Get_coordId() = 0;
  virtual _di_IXMLTMedicaoList __fastcall Get_Medicao() = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property _di_IXMLTCoordIdList coordId = { read=Get_coordId };
  __property _di_IXMLTMedicaoList Medicao = { read=Get_Medicao };
};

// IXMLTCoordId 

__interface INTERFACE_UUID("{7E4E64BC-9383-4A36-AE33-94E9C63B9052}") IXMLTCoordId : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strCod() = 0;
  virtual _di_IXMLTCoord __fastcall Get_coordUTM() = 0;
  virtual _di_IXMLTCoord __fastcall Get_coordESQ1() = 0;
  virtual _di_IXMLTCoord __fastcall Get_coordESQ2() = 0;
  virtual double __fastcall Get_dblPosBar() = 0;
  virtual _di_IXMLTCoordIco __fastcall Get_coordIco() = 0;
  virtual int __fastcall Get_intCoordNdx() = 0;
  virtual void __fastcall Set_strCod(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_dblPosBar(double Value) = 0;
  virtual void __fastcall Set_intCoordNdx(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strCod = { read=Get_strCod, write=Set_strCod };
  __property _di_IXMLTCoord coordUTM = { read=Get_coordUTM };
  __property _di_IXMLTCoord coordESQ1 = { read=Get_coordESQ1 };
  __property _di_IXMLTCoord coordESQ2 = { read=Get_coordESQ2 };
  __property double dblPosBar = { read=Get_dblPosBar, write=Set_dblPosBar };
  __property _di_IXMLTCoordIco coordIco = { read=Get_coordIco };
  __property int intCoordNdx = { read=Get_intCoordNdx, write=Set_intCoordNdx };
};

// IXMLTCoordIdList 

__interface INTERFACE_UUID("{5EBED813-57A7-4CC0-B7BF-DA5A88121BD0}") IXMLTCoordIdList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTCoordId __fastcall Add() = 0;
  virtual _di_IXMLTCoordId __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTCoordId __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTCoordId Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCoord 

__interface INTERFACE_UUID("{A8C7F948-9FE8-400A-BE93-93A3F74C4C57}") IXMLTCoord : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblX() = 0;
  virtual double __fastcall Get_dblY() = 0;
  virtual void __fastcall Set_dblX(double Value) = 0;
  virtual void __fastcall Set_dblY(double Value) = 0;
  // Methods & Properties 
  __property double dblX = { read=Get_dblX, write=Set_dblX };
  __property double dblY = { read=Get_dblY, write=Set_dblY };
};

// IXMLTCoordIco 

__interface INTERFACE_UUID("{463989A4-5C35-46DD-8089-E4B2B3E5733F}") IXMLTCoordIco : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblDxBar() = 0;
  virtual double __fastcall Get_dblDyBar() = 0;
  virtual void __fastcall Set_dblDxBar(double Value) = 0;
  virtual void __fastcall Set_dblDyBar(double Value) = 0;
  // Methods & Properties 
  __property double dblDxBar = { read=Get_dblDxBar, write=Set_dblDxBar };
  __property double dblDyBar = { read=Get_dblDyBar, write=Set_dblDyBar };
};

// IXMLTMedicao 

__interface INTERFACE_UUID("{47BFE3F0-CF88-416E-8AA1-2A713AF93121}") IXMLTMedicao : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_medidorId() = 0;
  virtual _di_IXMLTId __fastcall Get_canalId() = 0;
  virtual int __fastcall Get_intTpoCnl() = 0;
  virtual double __fastcall Get_dblSigmaCnl() = 0;
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar() = 0;
  virtual void __fastcall Set_intTpoCnl(int Value) = 0;
  virtual void __fastcall Set_dblSigmaCnl(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId medidorId = { read=Get_medidorId };
  __property _di_IXMLTId canalId = { read=Get_canalId };
  __property int intTpoCnl = { read=Get_intTpoCnl, write=Set_intTpoCnl };
  __property double dblSigmaCnl = { read=Get_dblSigmaCnl, write=Set_dblSigmaCnl };
  __property _di_IXMLTPatamPropList Patamar = { read=Get_Patamar };
};

// IXMLTMedicaoList 

__interface INTERFACE_UUID("{91FE86F8-DD07-410B-8654-FEA4EDADD624}") IXMLTMedicaoList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTMedicao __fastcall Add() = 0;
  virtual _di_IXMLTMedicao __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTMedicao __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTMedicao Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTPatamProp 

__interface INTERFACE_UUID("{A5A5F9C1-ED61-4EDC-8AB2-5E1D837156A8}") IXMLTPatamProp : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTHorMin __fastcall Get_horIni() = 0;
  virtual _di_IXMLTHorMin __fastcall Get_horFim() = 0;
  virtual double __fastcall Get_dblPotAtv() = 0;
  virtual double __fastcall Get_dblPotRtv() = 0;
  virtual double __fastcall Get_dblMdlTns() = 0;
  virtual double __fastcall Get_dblAng() = 0;
  virtual void __fastcall Set_dblPotAtv(double Value) = 0;
  virtual void __fastcall Set_dblPotRtv(double Value) = 0;
  virtual void __fastcall Set_dblMdlTns(double Value) = 0;
  virtual void __fastcall Set_dblAng(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTHorMin horIni = { read=Get_horIni };
  __property _di_IXMLTHorMin horFim = { read=Get_horFim };
  __property double dblPotAtv = { read=Get_dblPotAtv, write=Set_dblPotAtv };
  __property double dblPotRtv = { read=Get_dblPotRtv, write=Set_dblPotRtv };
  __property double dblMdlTns = { read=Get_dblMdlTns, write=Set_dblMdlTns };
  __property double dblAng = { read=Get_dblAng, write=Set_dblAng };
};

// IXMLTPatamPropList 

__interface INTERFACE_UUID("{EB0834EC-8C36-4D16-9357-322F52F1BCBE}") IXMLTPatamPropList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTPatamProp __fastcall Add() = 0;
  virtual _di_IXMLTPatamProp __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTPatamProp __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTPatamProp Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTHorMin 

__interface INTERFACE_UUID("{2DDA46A6-98AD-4BA1-9E8C-953029AB1847}") IXMLTHorMin : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intHor() = 0;
  virtual int __fastcall Get_intMin() = 0;
  virtual void __fastcall Set_intHor(int Value) = 0;
  virtual void __fastcall Set_intMin(int Value) = 0;
  // Methods & Properties 
  __property int intHor = { read=Get_intHor, write=Set_intHor };
  __property int intMin = { read=Get_intMin, write=Set_intMin };
};

// IXMLTEqBarCarga 

__interface INTERFACE_UUID("{31655C7F-DC58-48E4-B53E-C262AADEA2F4}") IXMLTEqBarCarga : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual int __fastcall Get_intTpoCons() = 0;
  virtual int __fastcall Get_intTpoModCrg() = 0;
  virtual double __fastcall Get_dblEne() = 0;
  virtual int __fastcall Get_intNumCons() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar() = 0;
  virtual void __fastcall Set_intTpoCons(int Value) = 0;
  virtual void __fastcall Set_intTpoModCrg(int Value) = 0;
  virtual void __fastcall Set_dblEne(double Value) = 0;
  virtual void __fastcall Set_intNumCons(int Value) = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property int intTpoCons = { read=Get_intTpoCons, write=Set_intTpoCons };
  __property int intTpoModCrg = { read=Get_intTpoModCrg, write=Set_intTpoModCrg };
  __property double dblEne = { read=Get_dblEne, write=Set_dblEne };
  __property int intNumCons = { read=Get_intNumCons, write=Set_intNumCons };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
  __property _di_IXMLTPatamPropList Patamar = { read=Get_Patamar };
};

// IXMLTEqBarCargaList 

__interface INTERFACE_UUID("{C60AFBE1-BFC0-4E63-9230-EE57C3D50DB6}") IXMLTEqBarCargaList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarCarga __fastcall Add() = 0;
  virtual _di_IXMLTEqBarCarga __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarCarga __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarCarga Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqBarCargaIP 

__interface INTERFACE_UUID("{09621802-7A8A-44EA-8775-4DAC33C03527}") IXMLTEqBarCargaIP : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual double __fastcall Get_dblPotAtv() = 0;
  virtual double __fastcall Get_dblFtrPot() = 0;
  virtual int __fastcall Get_intNumLamp() = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_dblPotAtv(double Value) = 0;
  virtual void __fastcall Set_dblFtrPot(double Value) = 0;
  virtual void __fastcall Set_intNumLamp(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
  __property double dblPotAtv = { read=Get_dblPotAtv, write=Set_dblPotAtv };
  __property double dblFtrPot = { read=Get_dblFtrPot, write=Set_dblFtrPot };
  __property int intNumLamp = { read=Get_intNumLamp, write=Set_intNumLamp };
};

// IXMLTEqBarCargaIPList 

__interface INTERFACE_UUID("{EF76ECA2-4B95-4878-8091-11CA3B7EB976}") IXMLTEqBarCargaIPList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarCargaIP __fastcall Add() = 0;
  virtual _di_IXMLTEqBarCargaIP __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarCargaIP __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarCargaIP Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqBarFiltro 

__interface INTERFACE_UUID("{858F8F0E-2FDD-401E-9C4B-1B5D3FA64EDB}") IXMLTEqBarFiltro : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual _di_IXMLTZ __fastcall Get_z0() = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property _di_IXMLTZ z0 = { read=Get_z0 };
};

// IXMLTEqBarFiltroList 

__interface INTERFACE_UUID("{0AC02F18-B7F0-4D15-8889-6CF9FD3502B4}") IXMLTEqBarFiltroList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarFiltro __fastcall Add() = 0;
  virtual _di_IXMLTEqBarFiltro __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarFiltro __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarFiltro Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTZ 

__interface INTERFACE_UUID("{BA7CA252-00C8-4480-B0DB-5F7083AFB386}") IXMLTZ : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblR() = 0;
  virtual double __fastcall Get_dblX() = 0;
  virtual void __fastcall Set_dblR(double Value) = 0;
  virtual void __fastcall Set_dblX(double Value) = 0;
  // Methods & Properties 
  __property double dblR = { read=Get_dblR, write=Set_dblR };
  __property double dblX = { read=Get_dblX, write=Set_dblX };
};

// IXMLTEqBarGerador 

__interface INTERFACE_UUID("{6EAF208D-F019-4747-B6AE-F6FC0EBAFC27}") IXMLTEqBarGerador : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual int __fastcall Get_intTpoSup() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual double __fastcall Get_dblPotMax() = 0;
  virtual double __fastcall Get_dblFtrPotLim() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_intTpoSup(int Value) = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  virtual void __fastcall Set_dblPotMax(double Value) = 0;
  virtual void __fastcall Set_dblFtrPotLim(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property int intTpoSup = { read=Get_intTpoSup, write=Set_intTpoSup };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property double dblPotMax = { read=Get_dblPotMax, write=Set_dblPotMax };
  __property double dblFtrPotLim = { read=Get_dblFtrPotLim, write=Set_dblFtrPotLim };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
  __property _di_IXMLTPatamPropList Patamar = { read=Get_Patamar };
};

// IXMLTEqBarGeradorList 

__interface INTERFACE_UUID("{D85B6403-BCE8-4D82-89F6-1E55012DB5E8}") IXMLTEqBarGeradorList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarGerador __fastcall Add() = 0;
  virtual _di_IXMLTEqBarGerador __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarGerador __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarGerador Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTZseq 

__interface INTERFACE_UUID("{C0C183B4-AFF4-42F8-9911-9C4C8913E8E6}") IXMLTZseq : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTZ __fastcall Get_z0() = 0;
  virtual _di_IXMLTZ __fastcall Get_z1() = 0;
  // Methods & Properties 
  __property _di_IXMLTZ z0 = { read=Get_z0 };
  __property _di_IXMLTZ z1 = { read=Get_z1 };
};

// IXMLTEqBarReator 

__interface INTERFACE_UUID("{34681B93-8CDD-437B-AB78-E26FDF0DE62F}") IXMLTEqBarReator : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotRtv() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotRtv(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotRtv = { read=Get_dblPotRtv, write=Set_dblPotRtv };
};

// IXMLTEqBarReatorList 

__interface INTERFACE_UUID("{580923F4-8E21-4F48-A2D4-B554B4ACD7C7}") IXMLTEqBarReatorList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarReator __fastcall Add() = 0;
  virtual _di_IXMLTEqBarReator __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarReator __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarReator Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqBarSuprimento 

__interface INTERFACE_UUID("{58A6B191-D381-4D3F-96FE-1D5DC642D6DB}") IXMLTEqBarSuprimento : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual int __fastcall Get_intTpoSup() = 0;
  virtual double __fastcall Get_dblPotMax() = 0;
  virtual _di_IXMLTPotCpx __fastcall Get_cpxPotCC3f() = 0;
  virtual _di_IXMLTPotCpx __fastcall Get_cpxPotCCFt() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zEqv() = 0;
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar() = 0;
  virtual void __fastcall Set_intTpoSup(int Value) = 0;
  virtual void __fastcall Set_dblPotMax(double Value) = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property int intTpoSup = { read=Get_intTpoSup, write=Set_intTpoSup };
  __property double dblPotMax = { read=Get_dblPotMax, write=Set_dblPotMax };
  __property _di_IXMLTPotCpx cpxPotCC3f = { read=Get_cpxPotCC3f };
  __property _di_IXMLTPotCpx cpxPotCCFt = { read=Get_cpxPotCCFt };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property _di_IXMLTZseq zEqv = { read=Get_zEqv };
  __property _di_IXMLTPatamPropList Patamar = { read=Get_Patamar };
};

// IXMLTEqBarSuprimentoList 

__interface INTERFACE_UUID("{AAB7591B-6D30-4D51-AD6B-9808A07527DE}") IXMLTEqBarSuprimentoList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarSuprimento __fastcall Add() = 0;
  virtual _di_IXMLTEqBarSuprimento __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarSuprimento __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarSuprimento Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTPotCpx 

__interface INTERFACE_UUID("{1429F7B2-6053-4D5C-9D99-4147536E329C}") IXMLTPotCpx : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblPotAtv() = 0;
  virtual double __fastcall Get_dblPotRtv() = 0;
  virtual void __fastcall Set_dblPotAtv(double Value) = 0;
  virtual void __fastcall Set_dblPotRtv(double Value) = 0;
  // Methods & Properties 
  __property double dblPotAtv = { read=Get_dblPotAtv, write=Set_dblPotAtv };
  __property double dblPotRtv = { read=Get_dblPotRtv, write=Set_dblPotRtv };
};

// IXMLTEqBarTrafoZZ 

__interface INTERFACE_UUID("{DE3A9406-7168-41EB-BB23-E1D738D896F4}") IXMLTEqBarTrafoZZ : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual _di_IXMLTZ __fastcall Get_z0() = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property _di_IXMLTZ z0 = { read=Get_z0 };
};

// IXMLTEqBarTrafoZZList 

__interface INTERFACE_UUID("{170556D0-BE78-418B-B5A5-E9B4C1D8239C}") IXMLTEqBarTrafoZZList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarTrafoZZ __fastcall Add() = 0;
  virtual _di_IXMLTEqBarTrafoZZ __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarTrafoZZ __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarTrafoZZ Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqBarMedidor 

__interface INTERFACE_UUID("{B3EC534F-BD08-41F0-B428-8D6288AF53C5}") IXMLTEqBarMedidor : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar() = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqbar = { read=Get_eqbar };
};

// IXMLTEqBarMedidorList 

__interface INTERFACE_UUID("{6715AA1C-4DEA-426C-873C-8BCDF20C855D}") IXMLTEqBarMedidorList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqBarMedidor __fastcall Add() = 0;
  virtual _di_IXMLTEqBarMedidor __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqBarMedidor __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqBarMedidor Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCnjEqLig 

__interface INTERFACE_UUID("{A33ABCAF-511A-4F49-A95E-3273D0B07958}") IXMLTCnjEqLig : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqLigCapacitorSerieList __fastcall Get_CapacitorSerie() = 0;
  virtual _di_IXMLTEqLigChaveList __fastcall Get_Chave() = 0;
  virtual _di_IXMLTEqLigReguladorList __fastcall Get_Regulador() = 0;
  virtual _di_IXMLTEqLigTrafoList __fastcall Get_Trafo() = 0;
  virtual _di_IXMLTEqLigTrafo3EList __fastcall Get_Trafo3E() = 0;
  virtual _di_IXMLTEqLigTrechoList __fastcall Get_Trecho() = 0;
  // Methods & Properties 
  __property _di_IXMLTEqLigCapacitorSerieList CapacitorSerie = { read=Get_CapacitorSerie };
  __property _di_IXMLTEqLigChaveList Chave = { read=Get_Chave };
  __property _di_IXMLTEqLigReguladorList Regulador = { read=Get_Regulador };
  __property _di_IXMLTEqLigTrafoList Trafo = { read=Get_Trafo };
  __property _di_IXMLTEqLigTrafo3EList Trafo3E = { read=Get_Trafo3E };
  __property _di_IXMLTEqLigTrechoList Trecho = { read=Get_Trecho };
};

// IXMLTEqLigCapacitorSerie 

__interface INTERFACE_UUID("{859BBEA2-5E85-4F89-8675-2B3D55F306EC}") IXMLTEqLigCapacitorSerie : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual _di_IXMLTZ __fastcall Get_z() = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property _di_IXMLTZ z = { read=Get_z };
};

// IXMLTEqLigCapacitorSerieList 

__interface INTERFACE_UUID("{41CCFDFA-2998-47E2-8561-6F01934767D5}") IXMLTEqLigCapacitorSerieList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Add() = 0;
  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigCapacitorSerie Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqLigChave 

__interface INTERFACE_UUID("{8192F376-9E46-4177-A77F-7A0C517DF397}") IXMLTEqLigChave : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual System::UnicodeString __fastcall Get_strTpoChv() = 0;
  virtual int __fastcall Get_intEst() = 0;
  virtual bool __fastcall Get_bolBlq() = 0;
  virtual double __fastcall Get_dblCorNom() = 0;
  virtual void __fastcall Set_strTpoChv(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_intEst(int Value) = 0;
  virtual void __fastcall Set_bolBlq(bool Value) = 0;
  virtual void __fastcall Set_dblCorNom(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property System::UnicodeString strTpoChv = { read=Get_strTpoChv, write=Set_strTpoChv };
  __property int intEst = { read=Get_intEst, write=Set_intEst };
  __property bool bolBlq = { read=Get_bolBlq, write=Set_bolBlq };
  __property double dblCorNom = { read=Get_dblCorNom, write=Set_dblCorNom };
};

// IXMLTEqLigChaveList 

__interface INTERFACE_UUID("{7378F3BF-3D40-4E10-ADA9-0EB6D82A34A0}") IXMLTEqLigChaveList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigChave __fastcall Add() = 0;
  virtual _di_IXMLTEqLigChave __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigChave __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigChave Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqLigRegulador 

__interface INTERFACE_UUID("{4BFA4591-83BC-4236-AE0D-0FFECFE70BA8}") IXMLTEqLigRegulador : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual bool __fastcall Get_bolBypass() = 0;
  virtual double __fastcall Get_dblFxaVar() = 0;
  virtual int __fastcall Get_intPasNum() = 0;
  virtual bool __fastcall Get_bolSntRev() = 0;
  virtual _di_IXMLTCoordId __fastcall Get_coordBarRefSnt() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual _di_IXMLTRegTapFix __fastcall Get_regTapFix() = 0;
  virtual _di_IXMLTRegTapAjs __fastcall Get_regTapAjs() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  virtual void __fastcall Set_bolBypass(bool Value) = 0;
  virtual void __fastcall Set_dblFxaVar(double Value) = 0;
  virtual void __fastcall Set_intPasNum(int Value) = 0;
  virtual void __fastcall Set_bolSntRev(bool Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property bool bolBypass = { read=Get_bolBypass, write=Set_bolBypass };
  __property double dblFxaVar = { read=Get_dblFxaVar, write=Set_dblFxaVar };
  __property int intPasNum = { read=Get_intPasNum, write=Set_intPasNum };
  __property bool bolSntRev = { read=Get_bolSntRev, write=Set_bolSntRev };
  __property _di_IXMLTCoordId coordBarRefSnt = { read=Get_coordBarRefSnt };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
  __property _di_IXMLTRegTapFix regTapFix = { read=Get_regTapFix };
  __property _di_IXMLTRegTapAjs regTapAjs = { read=Get_regTapAjs };
};

// IXMLTEqLigReguladorList 

__interface INTERFACE_UUID("{717818CE-D648-4ED2-BF6A-DE4305A37FC7}") IXMLTEqLigReguladorList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigRegulador __fastcall Add() = 0;
  virtual _di_IXMLTEqLigRegulador __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigRegulador __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigRegulador Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTRegTapFix 

__interface INTERFACE_UUID("{69FEB15B-851E-4249-85AB-03CFE528AB08}") IXMLTRegTapFix : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intPasSel() = 0;
  virtual void __fastcall Set_intPasSel(int Value) = 0;
  // Methods & Properties 
  __property int intPasSel = { read=Get_intPasSel, write=Set_intPasSel };
};

// IXMLTRegTapAjs 

__interface INTERFACE_UUID("{48DF1D79-3B96-4C65-AC8C-45084FDFB7E2}") IXMLTRegTapAjs : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblTnsAlvo() = 0;
  virtual _di_IXMLTCoordId __fastcall Get_coordBarRefCfg() = 0;
  virtual void __fastcall Set_dblTnsAlvo(double Value) = 0;
  // Methods & Properties 
  __property double dblTnsAlvo = { read=Get_dblTnsAlvo, write=Set_dblTnsAlvo };
  __property _di_IXMLTCoordId coordBarRefCfg = { read=Get_coordBarRefCfg };
};

// IXMLTEqLigTrafo 

__interface INTERFACE_UUID("{8DEF22C0-FA39-46E8-96E8-A3E2510477EA}") IXMLTEqLigTrafo : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual bool __fastcall Get_bolAuto() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual double __fastcall Get_dblPerFer() = 0;
  virtual _di_IXMLTEnrolList __fastcall Get_Enrol() = 0;
  virtual _di_IXMLTLtc __fastcall Get_ltc() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_bolAuto(bool Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  virtual void __fastcall Set_dblPerFer(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property bool bolAuto = { read=Get_bolAuto, write=Set_bolAuto };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property double dblPerFer = { read=Get_dblPerFer, write=Set_dblPerFer };
  __property _di_IXMLTEnrolList Enrol = { read=Get_Enrol };
  __property _di_IXMLTLtc ltc = { read=Get_ltc };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
};

// IXMLTEqLigTrafoList 

__interface INTERFACE_UUID("{09984FD7-F894-4CEB-99FC-BEC366DEE882}") IXMLTEqLigTrafoList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigTrafo __fastcall Add() = 0;
  virtual _di_IXMLTEqLigTrafo __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigTrafo __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigTrafo Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEnrol 

__interface INTERFACE_UUID("{3A1B937D-6C1C-488B-9F37-B1A91D263777}") IXMLTEnrol : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intId() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual double __fastcall Get_dblPotNom() = 0;
  virtual double __fastcall Get_dblTnsNom() = 0;
  virtual double __fastcall Get_dblTnsTap() = 0;
  virtual double __fastcall Get_dblAngDef() = 0;
  virtual _di_IXMLTZ __fastcall Get_zTerra() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual void __fastcall Set_intId(int Value) = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_dblPotNom(double Value) = 0;
  virtual void __fastcall Set_dblTnsNom(double Value) = 0;
  virtual void __fastcall Set_dblTnsTap(double Value) = 0;
  virtual void __fastcall Set_dblAngDef(double Value) = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property int intId = { read=Get_intId, write=Set_intId };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property double dblPotNom = { read=Get_dblPotNom, write=Set_dblPotNom };
  __property double dblTnsNom = { read=Get_dblTnsNom, write=Set_dblTnsNom };
  __property double dblTnsTap = { read=Get_dblTnsTap, write=Set_dblTnsTap };
  __property double dblAngDef = { read=Get_dblAngDef, write=Set_dblAngDef };
  __property _di_IXMLTZ zTerra = { read=Get_zTerra };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
};

// IXMLTEnrolList 

__interface INTERFACE_UUID("{4C2568FD-F6C6-4C09-8B45-16337EAC4974}") IXMLTEnrolList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEnrol __fastcall Add() = 0;
  virtual _di_IXMLTEnrol __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEnrol __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEnrol Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTLtc 

__interface INTERFACE_UUID("{1470CF54-1F9F-4005-B0AA-11DAC0720525}") IXMLTLtc : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intTpoUtl() = 0;
  virtual double __fastcall Get_dblTns() = 0;
  virtual _di_IXMLTCoordId __fastcall Get_coordBarCtl() = 0;
  virtual void __fastcall Set_intTpoUtl(int Value) = 0;
  virtual void __fastcall Set_dblTns(double Value) = 0;
  // Methods & Properties 
  __property int intTpoUtl = { read=Get_intTpoUtl, write=Set_intTpoUtl };
  __property double dblTns = { read=Get_dblTns, write=Set_dblTns };
  __property _di_IXMLTCoordId coordBarCtl = { read=Get_coordBarCtl };
};

// IXMLTEqLigTrafo3E 

__interface INTERFACE_UUID("{C86ABCA5-DAA5-4D0F-AE0C-F852BC8B532E}") IXMLTEqLigTrafo3E : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual int __fastcall Get_intTpoLig() = 0;
  virtual double __fastcall Get_dblPotBas() = 0;
  virtual double __fastcall Get_dblPerFer() = 0;
  virtual _di_IXMLTEnrolList __fastcall Get_Enrol() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeqPS() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeqPT() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeqST() = 0;
  virtual void __fastcall Set_intTpoLig(int Value) = 0;
  virtual void __fastcall Set_dblPotBas(double Value) = 0;
  virtual void __fastcall Set_dblPerFer(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property int intTpoLig = { read=Get_intTpoLig, write=Set_intTpoLig };
  __property double dblPotBas = { read=Get_dblPotBas, write=Set_dblPotBas };
  __property double dblPerFer = { read=Get_dblPerFer, write=Set_dblPerFer };
  __property _di_IXMLTEnrolList Enrol = { read=Get_Enrol };
  __property _di_IXMLTZseq zSeqPS = { read=Get_zSeqPS };
  __property _di_IXMLTZseq zSeqPT = { read=Get_zSeqPT };
  __property _di_IXMLTZseq zSeqST = { read=Get_zSeqST };
};

// IXMLTEqLigTrafo3EList 

__interface INTERFACE_UUID("{3792EFB4-150D-410C-AA51-95E9FEFC238C}") IXMLTEqLigTrafo3EList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigTrafo3E __fastcall Add() = 0;
  virtual _di_IXMLTEqLigTrafo3E __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigTrafo3E __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigTrafo3E Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTEqLigTrecho 

__interface INTERFACE_UUID("{939B19A4-0C76-4E44-98FB-2635FDC0C8F0}") IXMLTEqLigTrecho : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig() = 0;
  virtual bool __fastcall Get_bolTip() = 0;
  virtual bool __fastcall Get_bolEql() = 0;
  virtual double __fastcall Get_dblCmp() = 0;
  virtual _di_IXMLTTrcTpo __fastcall Get_trcTpo() = 0;
  virtual void __fastcall Set_bolTip(bool Value) = 0;
  virtual void __fastcall Set_bolEql(bool Value) = 0;
  virtual void __fastcall Set_dblCmp(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTEqpto eqlig = { read=Get_eqlig };
  __property bool bolTip = { read=Get_bolTip, write=Set_bolTip };
  __property bool bolEql = { read=Get_bolEql, write=Set_bolEql };
  __property double dblCmp = { read=Get_dblCmp, write=Set_dblCmp };
  __property _di_IXMLTTrcTpo trcTpo = { read=Get_trcTpo };
};

// IXMLTEqLigTrechoList 

__interface INTERFACE_UUID("{A46ACDA1-6133-44A8-86D5-7626A71D1CFA}") IXMLTEqLigTrechoList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTEqLigTrecho __fastcall Add() = 0;
  virtual _di_IXMLTEqLigTrecho __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTEqLigTrecho __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTEqLigTrecho Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTTrcTpo 

__interface INTERFACE_UUID("{D4DAAD47-6785-489A-B304-FA19FB0A6C5E}") IXMLTTrcTpo : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTTrcTpo01 __fastcall Get_trcTpo01() = 0;
  virtual _di_IXMLTTrcTpo02 __fastcall Get_trcTpo02() = 0;
  virtual _di_IXMLTTrcTpo03 __fastcall Get_trcTpo03() = 0;
  virtual System::UnicodeString __fastcall Get_strCodArr() = 0;
  virtual void __fastcall Set_strCodArr(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTTrcTpo01 trcTpo01 = { read=Get_trcTpo01 };
  __property _di_IXMLTTrcTpo02 trcTpo02 = { read=Get_trcTpo02 };
  __property _di_IXMLTTrcTpo03 trcTpo03 = { read=Get_trcTpo03 };
  __property System::UnicodeString strCodArr = { read=Get_strCodArr, write=Set_strCodArr };
};

// IXMLTTrcTpo01 

__interface INTERFACE_UUID("{65CB8A0E-6ADB-4922-8184-7E0F0C754905}") IXMLTTrcTpo01 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTCaboId __fastcall Get_caboId() = 0;
  // Methods & Properties 
  __property _di_IXMLTCaboId caboId = { read=Get_caboId };
};

// IXMLTCaboId 

__interface INTERFACE_UUID("{C6816E13-E82B-408B-82D5-859F6CF94A07}") IXMLTCaboId : public Xml::Xmlintf::IXMLNode
{
};

// IXMLTTrcTpo02 

__interface INTERFACE_UUID("{91661FFA-544B-4DD0-BFDE-FACF4D75DEEA}") IXMLTTrcTpo02 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual System::UnicodeString __fastcall Get_strEstGeoId() = 0;
  virtual _di_IXMLTCaboId __fastcall Get_caboIdA() = 0;
  virtual _di_IXMLTCaboId __fastcall Get_caboIdB() = 0;
  virtual _di_IXMLTCaboId __fastcall Get_caboIdC() = 0;
  virtual _di_IXMLTCaboId __fastcall Get_caboIdN() = 0;
  virtual void __fastcall Set_strEstGeoId(System::UnicodeString Value) = 0;
  // Methods & Properties 
  __property System::UnicodeString strEstGeoId = { read=Get_strEstGeoId, write=Set_strEstGeoId };
  __property _di_IXMLTCaboId caboIdA = { read=Get_caboIdA };
  __property _di_IXMLTCaboId caboIdB = { read=Get_caboIdB };
  __property _di_IXMLTCaboId caboIdC = { read=Get_caboIdC };
  __property _di_IXMLTCaboId caboIdN = { read=Get_caboIdN };
};

// IXMLTTrcTpo03 

__interface INTERFACE_UUID("{CE76D1D1-AF98-4462-A2B0-A3F43E2C0DAB}") IXMLTTrcTpo03 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual _di_IXMLTCSeq __fastcall Get_cSeq() = 0;
  virtual double __fastcall Get_dblCorMax() = 0;
  virtual void __fastcall Set_dblCorMax(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
  __property _di_IXMLTCSeq cSeq = { read=Get_cSeq };
  __property double dblCorMax = { read=Get_dblCorMax, write=Set_dblCorMax };
};

// IXMLTCSeq 

__interface INTERFACE_UUID("{AECCA0B8-8E14-4401-BDC9-5FCCB6DD68B6}") IXMLTCSeq : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual double __fastcall Get_dblC0() = 0;
  virtual double __fastcall Get_dblC1() = 0;
  virtual void __fastcall Set_dblC0(double Value) = 0;
  virtual void __fastcall Set_dblC1(double Value) = 0;
  // Methods & Properties 
  __property double dblC0 = { read=Get_dblC0, write=Set_dblC0 };
  __property double dblC1 = { read=Get_dblC1, write=Set_dblC1 };
};

// IXMLTMutua 

__interface INTERFACE_UUID("{7250902B-8BC2-4A07-ABBD-74CF209A2128}") IXMLTMutua : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual int __fastcall Get_intRedeId() = 0;
  virtual _di_IXMLTMutuaTreList __fastcall Get_MutuaTre() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual void __fastcall Set_intRedeId(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property int intRedeId = { read=Get_intRedeId, write=Set_intRedeId };
  __property _di_IXMLTMutuaTreList MutuaTre = { read=Get_MutuaTre };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
};

// IXMLTMutuaList 

__interface INTERFACE_UUID("{F41BD09C-242D-4FFC-9F4F-32F90783C622}") IXMLTMutuaList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTMutua __fastcall Add() = 0;
  virtual _di_IXMLTMutua __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTMutua __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTMutua Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTMutuaTre 

__interface INTERFACE_UUID("{2FA14B42-6FFF-4447-AA21-16BF867F9A93}") IXMLTMutuaTre : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intId() = 0;
  virtual int __fastcall Get_intSentido() = 0;
  virtual double __fastcall Get_dblPos() = 0;
  virtual void __fastcall Set_intId(int Value) = 0;
  virtual void __fastcall Set_intSentido(int Value) = 0;
  virtual void __fastcall Set_dblPos(double Value) = 0;
  // Methods & Properties 
  __property int intId = { read=Get_intId, write=Set_intId };
  __property int intSentido = { read=Get_intSentido, write=Set_intSentido };
  __property double dblPos = { read=Get_dblPos, write=Set_dblPos };
};

// IXMLTMutuaTreList 

__interface INTERFACE_UUID("{768A57E6-220B-4AC7-B6AF-8D8AB7116EAF}") IXMLTMutuaTreList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTMutuaTre __fastcall Add() = 0;
  virtual _di_IXMLTMutuaTre __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTMutuaTre __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTMutuaTre Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCaboRGMR 

__interface INTERFACE_UUID("{1888B2A1-0CD5-49BC-995F-21D988C56EAA}") IXMLTCaboRGMR : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual double __fastcall Get_dblCorMax() = 0;
  virtual double __fastcall Get_dblR() = 0;
  virtual double __fastcall Get_dblGMR() = 0;
  virtual void __fastcall Set_dblCorMax(double Value) = 0;
  virtual void __fastcall Set_dblR(double Value) = 0;
  virtual void __fastcall Set_dblGMR(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property double dblCorMax = { read=Get_dblCorMax, write=Set_dblCorMax };
  __property double dblR = { read=Get_dblR, write=Set_dblR };
  __property double dblGMR = { read=Get_dblGMR, write=Set_dblGMR };
};

// IXMLTCaboRGMRList 

__interface INTERFACE_UUID("{F6F612EA-D420-4902-948B-FA1E0235C7D9}") IXMLTCaboRGMRList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTCaboRGMR __fastcall Add() = 0;
  virtual _di_IXMLTCaboRGMR __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTCaboRGMR __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTCaboRGMR Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCaboZ0Z1 

__interface INTERFACE_UUID("{96B94BAF-BF0A-406B-B7E0-B16A44605E27}") IXMLTCaboZ0Z1 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual double __fastcall Get_dblCorMax() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual _di_IXMLTCSeq __fastcall Get_dblCSeq() = 0;
  virtual void __fastcall Set_dblCorMax(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property double dblCorMax = { read=Get_dblCorMax, write=Set_dblCorMax };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
  __property _di_IXMLTCSeq dblCSeq = { read=Get_dblCSeq };
};

// IXMLTCaboZ0Z1List 

__interface INTERFACE_UUID("{736C34D0-8F47-404C-9CC8-08124E65EC3C}") IXMLTCaboZ0Z1List : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTCaboZ0Z1 __fastcall Add() = 0;
  virtual _di_IXMLTCaboZ0Z1 __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTCaboZ0Z1 __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTCaboZ0Z1 Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTSuporte 

__interface INTERFACE_UUID("{49484B93-34FA-4076-A499-395573BBAE38}") IXMLTSuporte : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strTpo() = 0;
  virtual double __fastcall Get_dblTnsMin() = 0;
  virtual double __fastcall Get_dblTnsMax() = 0;
  virtual _di_IXMLTSuportePtoFixList __fastcall Get_PtoFix() = 0;
  virtual void __fastcall Set_strTpo(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_dblTnsMin(double Value) = 0;
  virtual void __fastcall Set_dblTnsMax(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strTpo = { read=Get_strTpo, write=Set_strTpo };
  __property double dblTnsMin = { read=Get_dblTnsMin, write=Set_dblTnsMin };
  __property double dblTnsMax = { read=Get_dblTnsMax, write=Set_dblTnsMax };
  __property _di_IXMLTSuportePtoFixList PtoFix = { read=Get_PtoFix };
};

// IXMLTSuporteList 

__interface INTERFACE_UUID("{FF78ACC6-C95A-41E4-A728-8B6A3F7841C3}") IXMLTSuporteList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTSuporte __fastcall Add() = 0;
  virtual _di_IXMLTSuporte __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTSuporte __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTSuporte Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTSuportePtoFix 

__interface INTERFACE_UUID("{C8FD115B-A28F-4FE6-9449-7253A9CB53AE}") IXMLTSuportePtoFix : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intId() = 0;
  virtual _di_IXMLTCoord __fastcall Get_coordPnt() = 0;
  virtual void __fastcall Set_intId(int Value) = 0;
  // Methods & Properties 
  __property int intId = { read=Get_intId, write=Set_intId };
  __property _di_IXMLTCoord coordPnt = { read=Get_coordPnt };
};

// IXMLTSuportePtoFixList 

__interface INTERFACE_UUID("{588F3BE2-2E5E-4396-A70A-F306842E9331}") IXMLTSuportePtoFixList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTSuportePtoFix __fastcall Add() = 0;
  virtual _di_IXMLTSuportePtoFix __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTSuportePtoFix __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTSuportePtoFix Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTArranjoZ0Z1 

__interface INTERFACE_UUID("{1BB0C403-3258-44D5-B891-235068618EB9}") IXMLTArranjoZ0Z1 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual double __fastcall Get_dblCorMax() = 0;
  virtual _di_IXMLTZseq __fastcall Get_zSeq() = 0;
  virtual _di_IXMLTCSeq __fastcall Get_dblCSeq() = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_dblCorMax(double Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
  __property double dblCorMax = { read=Get_dblCorMax, write=Set_dblCorMax };
  __property _di_IXMLTZseq zSeq = { read=Get_zSeq };
  __property _di_IXMLTCSeq dblCSeq = { read=Get_dblCSeq };
};

// IXMLTArranjoZ0Z1List 

__interface INTERFACE_UUID("{7874D6BC-2239-4840-98E8-3C57ECC5A305}") IXMLTArranjoZ0Z1List : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTArranjoZ0Z1 __fastcall Add() = 0;
  virtual _di_IXMLTArranjoZ0Z1 __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTArranjoZ0Z1 __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTArranjoZ0Z1 Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTArranjoCaboZ0Z1 

__interface INTERFACE_UUID("{FD5B05DB-8DDD-4704-B85D-8F4B1323AA01}") IXMLTArranjoCaboZ0Z1 : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual int __fastcall Get_intCaboId() = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_intCaboId(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
  __property int intCaboId = { read=Get_intCaboId, write=Set_intCaboId };
};

// IXMLTArranjoCaboZ0Z1List 

__interface INTERFACE_UUID("{F9CEEE42-21F2-47A4-BBCD-395BB02010D2}") IXMLTArranjoCaboZ0Z1List : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Add() = 0;
  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTArranjoCaboZ0Z1 Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTArranjoCaboRGMR 

__interface INTERFACE_UUID("{D6FA65D3-7BC9-46E0-ABD2-904CE65141EB}") IXMLTArranjoCaboRGMR : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual System::UnicodeString __fastcall Get_strFases() = 0;
  virtual int __fastcall Get_intSuporteId() = 0;
  virtual _di_IXMLTSuportePtoFixCaboList __fastcall Get_PtoFix() = 0;
  virtual void __fastcall Set_strFases(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_intSuporteId(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property System::UnicodeString strFases = { read=Get_strFases, write=Set_strFases };
  __property int intSuporteId = { read=Get_intSuporteId, write=Set_intSuporteId };
  __property _di_IXMLTSuportePtoFixCaboList PtoFix = { read=Get_PtoFix };
};

// IXMLTArranjoCaboRGMRList 

__interface INTERFACE_UUID("{15D2C86B-6A01-41DC-8EBE-F8E45B226E05}") IXMLTArranjoCaboRGMRList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTArranjoCaboRGMR __fastcall Add() = 0;
  virtual _di_IXMLTArranjoCaboRGMR __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTArranjoCaboRGMR __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTArranjoCaboRGMR Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTSuportePtoFixCabo 

__interface INTERFACE_UUID("{B0795AF0-14B0-4876-860E-D0BC28035A47}") IXMLTSuportePtoFixCabo : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual int __fastcall Get_intId() = 0;
  virtual System::UnicodeString __fastcall Get_strFase() = 0;
  virtual int __fastcall Get_intCaboId() = 0;
  virtual void __fastcall Set_intId(int Value) = 0;
  virtual void __fastcall Set_strFase(System::UnicodeString Value) = 0;
  virtual void __fastcall Set_intCaboId(int Value) = 0;
  // Methods & Properties 
  __property int intId = { read=Get_intId, write=Set_intId };
  __property System::UnicodeString strFase = { read=Get_strFase, write=Set_strFase };
  __property int intCaboId = { read=Get_intCaboId, write=Set_intCaboId };
};

// IXMLTSuportePtoFixCaboList 

__interface INTERFACE_UUID("{8522C30C-4FB1-4513-B52A-CE9DD562EFF9}") IXMLTSuportePtoFixCaboList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTSuportePtoFixCabo __fastcall Add() = 0;
  virtual _di_IXMLTSuportePtoFixCabo __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTSuportePtoFixCabo __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTSuportePtoFixCabo Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTCurvaTipica 

__interface INTERFACE_UUID("{E33811C2-8BCA-47F9-8C8C-0A9BF05C8438}") IXMLTCurvaTipica : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual int __fastcall Get_intTpoCurva() = 0;
  virtual int __fastcall Get_intUnidade() = 0;
  virtual int __fastcall Get_intEscala() = 0;
  virtual int __fastcall Get_intTpoCons() = 0;
  virtual double __fastcall Get_dblEneMin() = 0;
  virtual double __fastcall Get_dblEneMax() = 0;
  virtual int __fastcall Get_intAno() = 0;
  virtual int __fastcall Get_intMes() = 0;
  virtual int __fastcall Get_intNumPat() = 0;
  virtual _di_IXMLTPatamTipList __fastcall Get_Patamar() = 0;
  virtual void __fastcall Set_intTpoCurva(int Value) = 0;
  virtual void __fastcall Set_intUnidade(int Value) = 0;
  virtual void __fastcall Set_intEscala(int Value) = 0;
  virtual void __fastcall Set_intTpoCons(int Value) = 0;
  virtual void __fastcall Set_dblEneMin(double Value) = 0;
  virtual void __fastcall Set_dblEneMax(double Value) = 0;
  virtual void __fastcall Set_intAno(int Value) = 0;
  virtual void __fastcall Set_intMes(int Value) = 0;
  virtual void __fastcall Set_intNumPat(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property int intTpoCurva = { read=Get_intTpoCurva, write=Set_intTpoCurva };
  __property int intUnidade = { read=Get_intUnidade, write=Set_intUnidade };
  __property int intEscala = { read=Get_intEscala, write=Set_intEscala };
  __property int intTpoCons = { read=Get_intTpoCons, write=Set_intTpoCons };
  __property double dblEneMin = { read=Get_dblEneMin, write=Set_dblEneMin };
  __property double dblEneMax = { read=Get_dblEneMax, write=Set_dblEneMax };
  __property int intAno = { read=Get_intAno, write=Set_intAno };
  __property int intMes = { read=Get_intMes, write=Set_intMes };
  __property int intNumPat = { read=Get_intNumPat, write=Set_intNumPat };
  __property _di_IXMLTPatamTipList Patamar = { read=Get_Patamar };
};

// IXMLTCurvaTipicaList 

__interface INTERFACE_UUID("{40CFD942-64DF-4976-8B5A-7E58E52AF5B4}") IXMLTCurvaTipicaList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTCurvaTipica __fastcall Add() = 0;
  virtual _di_IXMLTCurvaTipica __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTCurvaTipica __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTCurvaTipica Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTPatamTip 

__interface INTERFACE_UUID("{12D99DD0-915F-4E67-8545-51FC4494EC1E}") IXMLTPatamTip : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTHorMin __fastcall Get_horIni() = 0;
  virtual _di_IXMLTHorMin __fastcall Get_horFim() = 0;
  virtual int __fastcall Get_intNumVal() = 0;
  virtual _di_IXMLdouble_List __fastcall Get_dblVal() = 0;
  virtual void __fastcall Set_intNumVal(int Value) = 0;
  // Methods & Properties 
  __property _di_IXMLTHorMin horIni = { read=Get_horIni };
  __property _di_IXMLTHorMin horFim = { read=Get_horFim };
  __property int intNumVal = { read=Get_intNumVal, write=Set_intNumVal };
  __property _di_IXMLdouble_List dblVal = { read=Get_dblVal };
};

// IXMLTPatamTipList 

__interface INTERFACE_UUID("{C48EA550-B4A3-43AF-B5F6-6AB8C005B17D}") IXMLTPatamTipList : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual _di_IXMLTPatamTip __fastcall Add() = 0;
  virtual _di_IXMLTPatamTip __fastcall Insert(const int Index) = 0;

  virtual _di_IXMLTPatamTip __fastcall Get_Item(int Index) = 0;
  __property _di_IXMLTPatamTip Items[int Index] = { read=Get_Item /* default */ };
};

// IXMLTZId 

__interface INTERFACE_UUID("{50E19567-5EF1-4B59-981B-AD18B1A74F80}") IXMLTZId : public Xml::Xmlintf::IXMLNode
{
public:
  // Property Accessors 
  virtual _di_IXMLTId __fastcall Get_id() = 0;
  virtual _di_IXMLTZ __fastcall Get_z() = 0;
  // Methods & Properties 
  __property _di_IXMLTId id = { read=Get_id };
  __property _di_IXMLTZ z = { read=Get_z };
};

// IXMLdouble_List 

__interface INTERFACE_UUID("{499B28C7-A1E3-4B34-8557-8636AAB9D4CB}") IXMLdouble_List : public Xml::Xmlintf::IXMLNodeCollection
{
public:
  // Methods & Properties 
  virtual Xml::Xmlintf::_di_IXMLNode __fastcall Add(const double Value) = 0;
  virtual Xml::Xmlintf::_di_IXMLNode __fastcall Insert(const int Index, const double Value) = 0;

  virtual double __fastcall Get_Item(int Index) = 0;
  __property double Items[int Index] = { read=Get_Item /* default */ };
};

// Forward Decls 

class TXMLTConcessionaria;
class TXMLTId;
class TXMLTRede;
class TXMLTRedeList;
class TXMLTCnjEqBar;
class TXMLTEqBarCapacitor;
class TXMLTEqBarCapacitorList;
class TXMLTEqpto;
class TXMLTCoordId;
class TXMLTCoordIdList;
class TXMLTCoord;
class TXMLTCoordIco;
class TXMLTMedicao;
class TXMLTMedicaoList;
class TXMLTPatamProp;
class TXMLTPatamPropList;
class TXMLTHorMin;
class TXMLTEqBarCarga;
class TXMLTEqBarCargaList;
class TXMLTEqBarCargaIP;
class TXMLTEqBarCargaIPList;
class TXMLTEqBarFiltro;
class TXMLTEqBarFiltroList;
class TXMLTZ;
class TXMLTEqBarGerador;
class TXMLTEqBarGeradorList;
class TXMLTZseq;
class TXMLTEqBarReator;
class TXMLTEqBarReatorList;
class TXMLTEqBarSuprimento;
class TXMLTEqBarSuprimentoList;
class TXMLTPotCpx;
class TXMLTEqBarTrafoZZ;
class TXMLTEqBarTrafoZZList;
class TXMLTEqBarMedidor;
class TXMLTEqBarMedidorList;
class TXMLTCnjEqLig;
class TXMLTEqLigCapacitorSerie;
class TXMLTEqLigCapacitorSerieList;
class TXMLTEqLigChave;
class TXMLTEqLigChaveList;
class TXMLTEqLigRegulador;
class TXMLTEqLigReguladorList;
class TXMLTRegTapFix;
class TXMLTRegTapAjs;
class TXMLTEqLigTrafo;
class TXMLTEqLigTrafoList;
class TXMLTEnrol;
class TXMLTEnrolList;
class TXMLTLtc;
class TXMLTEqLigTrafo3E;
class TXMLTEqLigTrafo3EList;
class TXMLTEqLigTrecho;
class TXMLTEqLigTrechoList;
class TXMLTTrcTpo;
class TXMLTTrcTpo01;
class TXMLTCaboId;
class TXMLTTrcTpo02;
class TXMLTTrcTpo03;
class TXMLTCSeq;
class TXMLTMutua;
class TXMLTMutuaList;
class TXMLTMutuaTre;
class TXMLTMutuaTreList;
class TXMLTCaboRGMR;
class TXMLTCaboRGMRList;
class TXMLTCaboZ0Z1;
class TXMLTCaboZ0Z1List;
class TXMLTSuporte;
class TXMLTSuporteList;
class TXMLTSuportePtoFix;
class TXMLTSuportePtoFixList;
class TXMLTArranjoZ0Z1;
class TXMLTArranjoZ0Z1List;
class TXMLTArranjoCaboZ0Z1;
class TXMLTArranjoCaboZ0Z1List;
class TXMLTArranjoCaboRGMR;
class TXMLTArranjoCaboRGMRList;
class TXMLTSuportePtoFixCabo;
class TXMLTSuportePtoFixCaboList;
class TXMLTCurvaTipica;
class TXMLTCurvaTipicaList;
class TXMLTPatamTip;
class TXMLTPatamTipList;
class TXMLTZId;
class TXMLdouble_List;

// TXMLTConcessionaria 

class TXMLTConcessionaria : public Xml::Xmldoc::TXMLNode, public IXMLTConcessionaria
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTRedeList FRede;
  _di_IXMLTCaboRGMRList FCaboRGMR;
  _di_IXMLTCaboZ0Z1List FCaboZ0Z1;
  _di_IXMLTSuporteList FSuporte;
  _di_IXMLTArranjoZ0Z1List FArranjoZ0Z1;
  _di_IXMLTArranjoCaboZ0Z1List FArranjoCaboZ0Z1;
  _di_IXMLTArranjoCaboRGMRList FArranjoCaboRGMR;
  _di_IXMLTCurvaTipicaList FCurvaTipicaCarga;
  _di_IXMLTCurvaTipicaList FCurvaTipicaCargaIP;
  _di_IXMLTCurvaTipicaList FCurvaTipicaCapacitor;
  _di_IXMLTCurvaTipicaList FCurvaTipicaReator;
  _di_IXMLTCurvaTipicaList FCurvaTipicaGeradorFoto;
protected:
  // IXMLTConcessionaria 
  virtual System::UnicodeString __fastcall Get_strVer();
  virtual _di_IXMLTId __fastcall Get_id();
  virtual _di_IXMLTRedeList __fastcall Get_Rede();
  virtual _di_IXMLTCaboRGMRList __fastcall Get_CaboRGMR();
  virtual _di_IXMLTCaboZ0Z1List __fastcall Get_CaboZ0Z1();
  virtual _di_IXMLTSuporteList __fastcall Get_Suporte();
  virtual _di_IXMLTArranjoZ0Z1List __fastcall Get_ArranjoZ0Z1();
  virtual _di_IXMLTArranjoCaboZ0Z1List __fastcall Get_ArranjoCaboZ0Z1();
  virtual _di_IXMLTArranjoCaboRGMRList __fastcall Get_ArranjoCaboRGMR();
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCarga();
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCargaIP();
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaCapacitor();
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaReator();
  virtual _di_IXMLTCurvaTipicaList __fastcall Get_CurvaTipicaGeradorFoto();
  virtual void __fastcall Set_strVer(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTId 

class TXMLTId : public Xml::Xmldoc::TXMLNode, public IXMLTId
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTId 
  virtual int __fastcall Get_intId();
  virtual System::UnicodeString __fastcall Get_strCod();
  virtual System::UnicodeString __fastcall Get_strDsc();
  virtual System::UnicodeString __fastcall Get_strExtId();
  virtual void __fastcall Set_intId(int Value);
  virtual void __fastcall Set_strCod(System::UnicodeString Value);
  virtual void __fastcall Set_strDsc(System::UnicodeString Value);
  virtual void __fastcall Set_strExtId(System::UnicodeString Value);
};

// TXMLTRede 

class TXMLTRede : public Xml::Xmldoc::TXMLNode, public IXMLTRede
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTMutuaList FMutua;
protected:
  // IXMLTRede 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strTpo();
  virtual _di_IXMLTCnjEqBar __fastcall Get_cnjEqBar();
  virtual _di_IXMLTCnjEqLig __fastcall Get_cnjEqLig();
  virtual _di_IXMLTMutuaList __fastcall Get_Mutua();
  virtual System::UnicodeString __fastcall Get_strDominio();
  virtual System::UnicodeString __fastcall Get_strCluster();
  virtual void __fastcall Set_strTpo(System::UnicodeString Value);
  virtual void __fastcall Set_strDominio(System::UnicodeString Value);
  virtual void __fastcall Set_strCluster(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTRedeList 

class TXMLTRedeList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTRedeList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTRedeList 
  virtual _di_IXMLTRede __fastcall Add();
  virtual _di_IXMLTRede __fastcall Insert(const int Index);

  virtual _di_IXMLTRede __fastcall Get_Item(int Index);
};

// TXMLTCnjEqBar 

class TXMLTCnjEqBar : public Xml::Xmldoc::TXMLNode, public IXMLTCnjEqBar
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTEqBarCapacitorList FCapacitor;
  _di_IXMLTEqBarCargaList FCarga;
  _di_IXMLTEqBarCargaIPList FCargaIP;
  _di_IXMLTEqBarFiltroList FFiltro;
  _di_IXMLTEqBarGeradorList FGerador;
  _di_IXMLTEqBarReatorList FReator;
  _di_IXMLTEqBarSuprimentoList FSuprimento;
  _di_IXMLTEqBarTrafoZZList FTrafoZZ;
  _di_IXMLTEqBarMedidorList FMedidor;
protected:
  // IXMLTCnjEqBar 
  virtual _di_IXMLTEqBarCapacitorList __fastcall Get_Capacitor();
  virtual _di_IXMLTEqBarCargaList __fastcall Get_Carga();
  virtual _di_IXMLTEqBarCargaIPList __fastcall Get_CargaIP();
  virtual _di_IXMLTEqBarFiltroList __fastcall Get_Filtro();
  virtual _di_IXMLTEqBarGeradorList __fastcall Get_Gerador();
  virtual _di_IXMLTEqBarReatorList __fastcall Get_Reator();
  virtual _di_IXMLTEqBarSuprimentoList __fastcall Get_Suprimento();
  virtual _di_IXMLTEqBarTrafoZZList __fastcall Get_TrafoZZ();
  virtual _di_IXMLTEqBarMedidorList __fastcall Get_Medidor();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarCapacitor 

class TXMLTEqBarCapacitor : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarCapacitor
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarCapacitor 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual int __fastcall Get_intTpoLig();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotRtv();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotRtv(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarCapacitorList 

class TXMLTEqBarCapacitorList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarCapacitorList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarCapacitorList 
  virtual _di_IXMLTEqBarCapacitor __fastcall Add();
  virtual _di_IXMLTEqBarCapacitor __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarCapacitor __fastcall Get_Item(int Index);
};

// TXMLTEqpto 

class TXMLTEqpto : public Xml::Xmldoc::TXMLNode, public IXMLTEqpto
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTCoordIdList FcoordId;
  _di_IXMLTMedicaoList FMedicao;
protected:
  // IXMLTEqpto 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual _di_IXMLTCoordIdList __fastcall Get_coordId();
  virtual _di_IXMLTMedicaoList __fastcall Get_Medicao();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCoordId 

class TXMLTCoordId : public Xml::Xmldoc::TXMLNode, public IXMLTCoordId
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCoordId 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strCod();
  virtual _di_IXMLTCoord __fastcall Get_coordUTM();
  virtual _di_IXMLTCoord __fastcall Get_coordESQ1();
  virtual _di_IXMLTCoord __fastcall Get_coordESQ2();
  virtual double __fastcall Get_dblPosBar();
  virtual _di_IXMLTCoordIco __fastcall Get_coordIco();
  virtual int __fastcall Get_intCoordNdx();
  virtual void __fastcall Set_strCod(System::UnicodeString Value);
  virtual void __fastcall Set_dblPosBar(double Value);
  virtual void __fastcall Set_intCoordNdx(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCoordIdList 

class TXMLTCoordIdList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTCoordIdList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTCoordIdList 
  virtual _di_IXMLTCoordId __fastcall Add();
  virtual _di_IXMLTCoordId __fastcall Insert(const int Index);

  virtual _di_IXMLTCoordId __fastcall Get_Item(int Index);
};

// TXMLTCoord 

class TXMLTCoord : public Xml::Xmldoc::TXMLNode, public IXMLTCoord
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCoord 
  virtual double __fastcall Get_dblX();
  virtual double __fastcall Get_dblY();
  virtual void __fastcall Set_dblX(double Value);
  virtual void __fastcall Set_dblY(double Value);
};

// TXMLTCoordIco 

class TXMLTCoordIco : public Xml::Xmldoc::TXMLNode, public IXMLTCoordIco
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCoordIco 
  virtual double __fastcall Get_dblDxBar();
  virtual double __fastcall Get_dblDyBar();
  virtual void __fastcall Set_dblDxBar(double Value);
  virtual void __fastcall Set_dblDyBar(double Value);
};

// TXMLTMedicao 

class TXMLTMedicao : public Xml::Xmldoc::TXMLNode, public IXMLTMedicao
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTPatamPropList FPatamar;
protected:
  // IXMLTMedicao 
  virtual _di_IXMLTId __fastcall Get_medidorId();
  virtual _di_IXMLTId __fastcall Get_canalId();
  virtual int __fastcall Get_intTpoCnl();
  virtual double __fastcall Get_dblSigmaCnl();
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar();
  virtual void __fastcall Set_intTpoCnl(int Value);
  virtual void __fastcall Set_dblSigmaCnl(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTMedicaoList 

class TXMLTMedicaoList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTMedicaoList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTMedicaoList 
  virtual _di_IXMLTMedicao __fastcall Add();
  virtual _di_IXMLTMedicao __fastcall Insert(const int Index);

  virtual _di_IXMLTMedicao __fastcall Get_Item(int Index);
};

// TXMLTPatamProp 

class TXMLTPatamProp : public Xml::Xmldoc::TXMLNode, public IXMLTPatamProp
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTPatamProp 
  virtual _di_IXMLTHorMin __fastcall Get_horIni();
  virtual _di_IXMLTHorMin __fastcall Get_horFim();
  virtual double __fastcall Get_dblPotAtv();
  virtual double __fastcall Get_dblPotRtv();
  virtual double __fastcall Get_dblMdlTns();
  virtual double __fastcall Get_dblAng();
  virtual void __fastcall Set_dblPotAtv(double Value);
  virtual void __fastcall Set_dblPotRtv(double Value);
  virtual void __fastcall Set_dblMdlTns(double Value);
  virtual void __fastcall Set_dblAng(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTPatamPropList 

class TXMLTPatamPropList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTPatamPropList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTPatamPropList 
  virtual _di_IXMLTPatamProp __fastcall Add();
  virtual _di_IXMLTPatamProp __fastcall Insert(const int Index);

  virtual _di_IXMLTPatamProp __fastcall Get_Item(int Index);
};

// TXMLTHorMin 

class TXMLTHorMin : public Xml::Xmldoc::TXMLNode, public IXMLTHorMin
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTHorMin 
  virtual int __fastcall Get_intHor();
  virtual int __fastcall Get_intMin();
  virtual void __fastcall Set_intHor(int Value);
  virtual void __fastcall Set_intMin(int Value);
};

// TXMLTEqBarCarga 

class TXMLTEqBarCarga : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarCarga
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTPatamPropList FPatamar;
protected:
  // IXMLTEqBarCarga 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual int __fastcall Get_intTpoCons();
  virtual int __fastcall Get_intTpoModCrg();
  virtual double __fastcall Get_dblEne();
  virtual int __fastcall Get_intNumCons();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar();
  virtual void __fastcall Set_intTpoCons(int Value);
  virtual void __fastcall Set_intTpoModCrg(int Value);
  virtual void __fastcall Set_dblEne(double Value);
  virtual void __fastcall Set_intNumCons(int Value);
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarCargaList 

class TXMLTEqBarCargaList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarCargaList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarCargaList 
  virtual _di_IXMLTEqBarCarga __fastcall Add();
  virtual _di_IXMLTEqBarCarga __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarCarga __fastcall Get_Item(int Index);
};

// TXMLTEqBarCargaIP 

class TXMLTEqBarCargaIP : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarCargaIP
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarCargaIP 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual double __fastcall Get_dblPotAtv();
  virtual double __fastcall Get_dblFtrPot();
  virtual int __fastcall Get_intNumLamp();
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
  virtual void __fastcall Set_dblPotAtv(double Value);
  virtual void __fastcall Set_dblFtrPot(double Value);
  virtual void __fastcall Set_intNumLamp(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarCargaIPList 

class TXMLTEqBarCargaIPList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarCargaIPList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarCargaIPList 
  virtual _di_IXMLTEqBarCargaIP __fastcall Add();
  virtual _di_IXMLTEqBarCargaIP __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarCargaIP __fastcall Get_Item(int Index);
};

// TXMLTEqBarFiltro 

class TXMLTEqBarFiltro : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarFiltro
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarFiltro 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotNom();
  virtual _di_IXMLTZ __fastcall Get_z0();
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotNom(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarFiltroList 

class TXMLTEqBarFiltroList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarFiltroList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarFiltroList 
  virtual _di_IXMLTEqBarFiltro __fastcall Add();
  virtual _di_IXMLTEqBarFiltro __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarFiltro __fastcall Get_Item(int Index);
};

// TXMLTZ 

class TXMLTZ : public Xml::Xmldoc::TXMLNode, public IXMLTZ
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTZ 
  virtual double __fastcall Get_dblR();
  virtual double __fastcall Get_dblX();
  virtual void __fastcall Set_dblR(double Value);
  virtual void __fastcall Set_dblX(double Value);
};

// TXMLTEqBarGerador 

class TXMLTEqBarGerador : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarGerador
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTPatamPropList FPatamar;
protected:
  // IXMLTEqBarGerador 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual int __fastcall Get_intTpoLig();
  virtual int __fastcall Get_intTpoSup();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotNom();
  virtual double __fastcall Get_dblPotMax();
  virtual double __fastcall Get_dblFtrPotLim();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_intTpoSup(int Value);
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotNom(double Value);
  virtual void __fastcall Set_dblPotMax(double Value);
  virtual void __fastcall Set_dblFtrPotLim(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarGeradorList 

class TXMLTEqBarGeradorList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarGeradorList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarGeradorList 
  virtual _di_IXMLTEqBarGerador __fastcall Add();
  virtual _di_IXMLTEqBarGerador __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarGerador __fastcall Get_Item(int Index);
};

// TXMLTZseq 

class TXMLTZseq : public Xml::Xmldoc::TXMLNode, public IXMLTZseq
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTZseq 
  virtual _di_IXMLTZ __fastcall Get_z0();
  virtual _di_IXMLTZ __fastcall Get_z1();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarReator 

class TXMLTEqBarReator : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarReator
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarReator 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual int __fastcall Get_intTpoLig();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotRtv();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotRtv(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarReatorList 

class TXMLTEqBarReatorList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarReatorList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarReatorList 
  virtual _di_IXMLTEqBarReator __fastcall Add();
  virtual _di_IXMLTEqBarReator __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarReator __fastcall Get_Item(int Index);
};

// TXMLTEqBarSuprimento 

class TXMLTEqBarSuprimento : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarSuprimento
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTPatamPropList FPatamar;
protected:
  // IXMLTEqBarSuprimento 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual int __fastcall Get_intTpoSup();
  virtual double __fastcall Get_dblPotMax();
  virtual _di_IXMLTPotCpx __fastcall Get_cpxPotCC3f();
  virtual _di_IXMLTPotCpx __fastcall Get_cpxPotCCFt();
  virtual double __fastcall Get_dblTnsNom();
  virtual _di_IXMLTZseq __fastcall Get_zEqv();
  virtual _di_IXMLTPatamPropList __fastcall Get_Patamar();
  virtual void __fastcall Set_intTpoSup(int Value);
  virtual void __fastcall Set_dblPotMax(double Value);
  virtual void __fastcall Set_dblTnsNom(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarSuprimentoList 

class TXMLTEqBarSuprimentoList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarSuprimentoList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarSuprimentoList 
  virtual _di_IXMLTEqBarSuprimento __fastcall Add();
  virtual _di_IXMLTEqBarSuprimento __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarSuprimento __fastcall Get_Item(int Index);
};

// TXMLTPotCpx 

class TXMLTPotCpx : public Xml::Xmldoc::TXMLNode, public IXMLTPotCpx
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTPotCpx 
  virtual double __fastcall Get_dblPotAtv();
  virtual double __fastcall Get_dblPotRtv();
  virtual void __fastcall Set_dblPotAtv(double Value);
  virtual void __fastcall Set_dblPotRtv(double Value);
};

// TXMLTEqBarTrafoZZ 

class TXMLTEqBarTrafoZZ : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarTrafoZZ
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarTrafoZZ 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotNom();
  virtual _di_IXMLTZ __fastcall Get_z0();
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotNom(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarTrafoZZList 

class TXMLTEqBarTrafoZZList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarTrafoZZList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarTrafoZZList 
  virtual _di_IXMLTEqBarTrafoZZ __fastcall Add();
  virtual _di_IXMLTEqBarTrafoZZ __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarTrafoZZ __fastcall Get_Item(int Index);
};

// TXMLTEqBarMedidor 

class TXMLTEqBarMedidor : public Xml::Xmldoc::TXMLNode, public IXMLTEqBarMedidor
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqBarMedidor 
  virtual _di_IXMLTEqpto __fastcall Get_eqbar();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqBarMedidorList 

class TXMLTEqBarMedidorList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqBarMedidorList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqBarMedidorList 
  virtual _di_IXMLTEqBarMedidor __fastcall Add();
  virtual _di_IXMLTEqBarMedidor __fastcall Insert(const int Index);

  virtual _di_IXMLTEqBarMedidor __fastcall Get_Item(int Index);
};

// TXMLTCnjEqLig 

class TXMLTCnjEqLig : public Xml::Xmldoc::TXMLNode, public IXMLTCnjEqLig
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTEqLigCapacitorSerieList FCapacitorSerie;
  _di_IXMLTEqLigChaveList FChave;
  _di_IXMLTEqLigReguladorList FRegulador;
  _di_IXMLTEqLigTrafoList FTrafo;
  _di_IXMLTEqLigTrafo3EList FTrafo3E;
  _di_IXMLTEqLigTrechoList FTrecho;
protected:
  // IXMLTCnjEqLig 
  virtual _di_IXMLTEqLigCapacitorSerieList __fastcall Get_CapacitorSerie();
  virtual _di_IXMLTEqLigChaveList __fastcall Get_Chave();
  virtual _di_IXMLTEqLigReguladorList __fastcall Get_Regulador();
  virtual _di_IXMLTEqLigTrafoList __fastcall Get_Trafo();
  virtual _di_IXMLTEqLigTrafo3EList __fastcall Get_Trafo3E();
  virtual _di_IXMLTEqLigTrechoList __fastcall Get_Trecho();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigCapacitorSerie 

class TXMLTEqLigCapacitorSerie : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigCapacitorSerie
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqLigCapacitorSerie 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblPotNom();
  virtual _di_IXMLTZ __fastcall Get_z();
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblPotNom(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigCapacitorSerieList 

class TXMLTEqLigCapacitorSerieList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigCapacitorSerieList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigCapacitorSerieList 
  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Add();
  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigCapacitorSerie __fastcall Get_Item(int Index);
};

// TXMLTEqLigChave 

class TXMLTEqLigChave : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigChave
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqLigChave 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual System::UnicodeString __fastcall Get_strTpoChv();
  virtual int __fastcall Get_intEst();
  virtual bool __fastcall Get_bolBlq();
  virtual double __fastcall Get_dblCorNom();
  virtual void __fastcall Set_strTpoChv(System::UnicodeString Value);
  virtual void __fastcall Set_intEst(int Value);
  virtual void __fastcall Set_bolBlq(bool Value);
  virtual void __fastcall Set_dblCorNom(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigChaveList 

class TXMLTEqLigChaveList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigChaveList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigChaveList 
  virtual _di_IXMLTEqLigChave __fastcall Add();
  virtual _di_IXMLTEqLigChave __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigChave __fastcall Get_Item(int Index);
};

// TXMLTEqLigRegulador 

class TXMLTEqLigRegulador : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigRegulador
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqLigRegulador 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual int __fastcall Get_intTpoLig();
  virtual double __fastcall Get_dblPotNom();
  virtual bool __fastcall Get_bolBypass();
  virtual double __fastcall Get_dblFxaVar();
  virtual int __fastcall Get_intPasNum();
  virtual bool __fastcall Get_bolSntRev();
  virtual _di_IXMLTCoordId __fastcall Get_coordBarRefSnt();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual _di_IXMLTRegTapFix __fastcall Get_regTapFix();
  virtual _di_IXMLTRegTapAjs __fastcall Get_regTapAjs();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_dblPotNom(double Value);
  virtual void __fastcall Set_bolBypass(bool Value);
  virtual void __fastcall Set_dblFxaVar(double Value);
  virtual void __fastcall Set_intPasNum(int Value);
  virtual void __fastcall Set_bolSntRev(bool Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigReguladorList 

class TXMLTEqLigReguladorList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigReguladorList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigReguladorList 
  virtual _di_IXMLTEqLigRegulador __fastcall Add();
  virtual _di_IXMLTEqLigRegulador __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigRegulador __fastcall Get_Item(int Index);
};

// TXMLTRegTapFix 

class TXMLTRegTapFix : public Xml::Xmldoc::TXMLNode, public IXMLTRegTapFix
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTRegTapFix 
  virtual int __fastcall Get_intPasSel();
  virtual void __fastcall Set_intPasSel(int Value);
};

// TXMLTRegTapAjs 

class TXMLTRegTapAjs : public Xml::Xmldoc::TXMLNode, public IXMLTRegTapAjs
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTRegTapAjs 
  virtual double __fastcall Get_dblTnsAlvo();
  virtual _di_IXMLTCoordId __fastcall Get_coordBarRefCfg();
  virtual void __fastcall Set_dblTnsAlvo(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigTrafo 

class TXMLTEqLigTrafo : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigTrafo
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTEnrolList FEnrol;
protected:
  // IXMLTEqLigTrafo 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual int __fastcall Get_intTpoLig();
  virtual bool __fastcall Get_bolAuto();
  virtual double __fastcall Get_dblPotNom();
  virtual double __fastcall Get_dblPerFer();
  virtual _di_IXMLTEnrolList __fastcall Get_Enrol();
  virtual _di_IXMLTLtc __fastcall Get_ltc();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_bolAuto(bool Value);
  virtual void __fastcall Set_dblPotNom(double Value);
  virtual void __fastcall Set_dblPerFer(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigTrafoList 

class TXMLTEqLigTrafoList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigTrafoList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigTrafoList 
  virtual _di_IXMLTEqLigTrafo __fastcall Add();
  virtual _di_IXMLTEqLigTrafo __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigTrafo __fastcall Get_Item(int Index);
};

// TXMLTEnrol 

class TXMLTEnrol : public Xml::Xmldoc::TXMLNode, public IXMLTEnrol
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEnrol 
  virtual int __fastcall Get_intId();
  virtual int __fastcall Get_intTpoLig();
  virtual double __fastcall Get_dblPotNom();
  virtual double __fastcall Get_dblTnsNom();
  virtual double __fastcall Get_dblTnsTap();
  virtual double __fastcall Get_dblAngDef();
  virtual _di_IXMLTZ __fastcall Get_zTerra();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual void __fastcall Set_intId(int Value);
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_dblPotNom(double Value);
  virtual void __fastcall Set_dblTnsNom(double Value);
  virtual void __fastcall Set_dblTnsTap(double Value);
  virtual void __fastcall Set_dblAngDef(double Value);
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEnrolList 

class TXMLTEnrolList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEnrolList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEnrolList 
  virtual _di_IXMLTEnrol __fastcall Add();
  virtual _di_IXMLTEnrol __fastcall Insert(const int Index);

  virtual _di_IXMLTEnrol __fastcall Get_Item(int Index);
};

// TXMLTLtc 

class TXMLTLtc : public Xml::Xmldoc::TXMLNode, public IXMLTLtc
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTLtc 
  virtual int __fastcall Get_intTpoUtl();
  virtual double __fastcall Get_dblTns();
  virtual _di_IXMLTCoordId __fastcall Get_coordBarCtl();
  virtual void __fastcall Set_intTpoUtl(int Value);
  virtual void __fastcall Set_dblTns(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigTrafo3E 

class TXMLTEqLigTrafo3E : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigTrafo3E
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTEnrolList FEnrol;
protected:
  // IXMLTEqLigTrafo3E 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual int __fastcall Get_intTpoLig();
  virtual double __fastcall Get_dblPotBas();
  virtual double __fastcall Get_dblPerFer();
  virtual _di_IXMLTEnrolList __fastcall Get_Enrol();
  virtual _di_IXMLTZseq __fastcall Get_zSeqPS();
  virtual _di_IXMLTZseq __fastcall Get_zSeqPT();
  virtual _di_IXMLTZseq __fastcall Get_zSeqST();
  virtual void __fastcall Set_intTpoLig(int Value);
  virtual void __fastcall Set_dblPotBas(double Value);
  virtual void __fastcall Set_dblPerFer(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigTrafo3EList 

class TXMLTEqLigTrafo3EList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigTrafo3EList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigTrafo3EList 
  virtual _di_IXMLTEqLigTrafo3E __fastcall Add();
  virtual _di_IXMLTEqLigTrafo3E __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigTrafo3E __fastcall Get_Item(int Index);
};

// TXMLTEqLigTrecho 

class TXMLTEqLigTrecho : public Xml::Xmldoc::TXMLNode, public IXMLTEqLigTrecho
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTEqLigTrecho 
  virtual _di_IXMLTEqpto __fastcall Get_eqlig();
  virtual bool __fastcall Get_bolTip();
  virtual bool __fastcall Get_bolEql();
  virtual double __fastcall Get_dblCmp();
  virtual _di_IXMLTTrcTpo __fastcall Get_trcTpo();
  virtual void __fastcall Set_bolTip(bool Value);
  virtual void __fastcall Set_bolEql(bool Value);
  virtual void __fastcall Set_dblCmp(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTEqLigTrechoList 

class TXMLTEqLigTrechoList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTEqLigTrechoList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTEqLigTrechoList 
  virtual _di_IXMLTEqLigTrecho __fastcall Add();
  virtual _di_IXMLTEqLigTrecho __fastcall Insert(const int Index);

  virtual _di_IXMLTEqLigTrecho __fastcall Get_Item(int Index);
};

// TXMLTTrcTpo 

class TXMLTTrcTpo : public Xml::Xmldoc::TXMLNode, public IXMLTTrcTpo
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTTrcTpo 
  virtual _di_IXMLTTrcTpo01 __fastcall Get_trcTpo01();
  virtual _di_IXMLTTrcTpo02 __fastcall Get_trcTpo02();
  virtual _di_IXMLTTrcTpo03 __fastcall Get_trcTpo03();
  virtual System::UnicodeString __fastcall Get_strCodArr();
  virtual void __fastcall Set_strCodArr(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTTrcTpo01 

class TXMLTTrcTpo01 : public Xml::Xmldoc::TXMLNode, public IXMLTTrcTpo01
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTTrcTpo01 
  virtual _di_IXMLTCaboId __fastcall Get_caboId();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCaboId 

class TXMLTCaboId : public Xml::Xmldoc::TXMLNode, public IXMLTCaboId
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCaboId 
};

// TXMLTTrcTpo02 

class TXMLTTrcTpo02 : public Xml::Xmldoc::TXMLNode, public IXMLTTrcTpo02
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTTrcTpo02 
  virtual System::UnicodeString __fastcall Get_strEstGeoId();
  virtual _di_IXMLTCaboId __fastcall Get_caboIdA();
  virtual _di_IXMLTCaboId __fastcall Get_caboIdB();
  virtual _di_IXMLTCaboId __fastcall Get_caboIdC();
  virtual _di_IXMLTCaboId __fastcall Get_caboIdN();
  virtual void __fastcall Set_strEstGeoId(System::UnicodeString Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTTrcTpo03 

class TXMLTTrcTpo03 : public Xml::Xmldoc::TXMLNode, public IXMLTTrcTpo03
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTTrcTpo03 
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual _di_IXMLTCSeq __fastcall Get_cSeq();
  virtual double __fastcall Get_dblCorMax();
  virtual void __fastcall Set_dblCorMax(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCSeq 

class TXMLTCSeq : public Xml::Xmldoc::TXMLNode, public IXMLTCSeq
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCSeq 
  virtual double __fastcall Get_dblC0();
  virtual double __fastcall Get_dblC1();
  virtual void __fastcall Set_dblC0(double Value);
  virtual void __fastcall Set_dblC1(double Value);
};

// TXMLTMutua 

class TXMLTMutua : public Xml::Xmldoc::TXMLNode, public IXMLTMutua
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTMutuaTreList FMutuaTre;
protected:
  // IXMLTMutua 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual int __fastcall Get_intRedeId();
  virtual _di_IXMLTMutuaTreList __fastcall Get_MutuaTre();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual void __fastcall Set_intRedeId(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTMutuaList 

class TXMLTMutuaList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTMutuaList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTMutuaList 
  virtual _di_IXMLTMutua __fastcall Add();
  virtual _di_IXMLTMutua __fastcall Insert(const int Index);

  virtual _di_IXMLTMutua __fastcall Get_Item(int Index);
};

// TXMLTMutuaTre 

class TXMLTMutuaTre : public Xml::Xmldoc::TXMLNode, public IXMLTMutuaTre
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTMutuaTre 
  virtual int __fastcall Get_intId();
  virtual int __fastcall Get_intSentido();
  virtual double __fastcall Get_dblPos();
  virtual void __fastcall Set_intId(int Value);
  virtual void __fastcall Set_intSentido(int Value);
  virtual void __fastcall Set_dblPos(double Value);
};

// TXMLTMutuaTreList 

class TXMLTMutuaTreList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTMutuaTreList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTMutuaTreList 
  virtual _di_IXMLTMutuaTre __fastcall Add();
  virtual _di_IXMLTMutuaTre __fastcall Insert(const int Index);

  virtual _di_IXMLTMutuaTre __fastcall Get_Item(int Index);
};

// TXMLTCaboRGMR 

class TXMLTCaboRGMR : public Xml::Xmldoc::TXMLNode, public IXMLTCaboRGMR
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCaboRGMR 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual double __fastcall Get_dblCorMax();
  virtual double __fastcall Get_dblR();
  virtual double __fastcall Get_dblGMR();
  virtual void __fastcall Set_dblCorMax(double Value);
  virtual void __fastcall Set_dblR(double Value);
  virtual void __fastcall Set_dblGMR(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCaboRGMRList 

class TXMLTCaboRGMRList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTCaboRGMRList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTCaboRGMRList 
  virtual _di_IXMLTCaboRGMR __fastcall Add();
  virtual _di_IXMLTCaboRGMR __fastcall Insert(const int Index);

  virtual _di_IXMLTCaboRGMR __fastcall Get_Item(int Index);
};

// TXMLTCaboZ0Z1 

class TXMLTCaboZ0Z1 : public Xml::Xmldoc::TXMLNode, public IXMLTCaboZ0Z1
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTCaboZ0Z1 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual double __fastcall Get_dblCorMax();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual _di_IXMLTCSeq __fastcall Get_dblCSeq();
  virtual void __fastcall Set_dblCorMax(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCaboZ0Z1List 

class TXMLTCaboZ0Z1List : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTCaboZ0Z1List
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTCaboZ0Z1List 
  virtual _di_IXMLTCaboZ0Z1 __fastcall Add();
  virtual _di_IXMLTCaboZ0Z1 __fastcall Insert(const int Index);

  virtual _di_IXMLTCaboZ0Z1 __fastcall Get_Item(int Index);
};

// TXMLTSuporte 

class TXMLTSuporte : public Xml::Xmldoc::TXMLNode, public IXMLTSuporte
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTSuportePtoFixList FPtoFix;
protected:
  // IXMLTSuporte 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strTpo();
  virtual double __fastcall Get_dblTnsMin();
  virtual double __fastcall Get_dblTnsMax();
  virtual _di_IXMLTSuportePtoFixList __fastcall Get_PtoFix();
  virtual void __fastcall Set_strTpo(System::UnicodeString Value);
  virtual void __fastcall Set_dblTnsMin(double Value);
  virtual void __fastcall Set_dblTnsMax(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTSuporteList 

class TXMLTSuporteList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTSuporteList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTSuporteList 
  virtual _di_IXMLTSuporte __fastcall Add();
  virtual _di_IXMLTSuporte __fastcall Insert(const int Index);

  virtual _di_IXMLTSuporte __fastcall Get_Item(int Index);
};

// TXMLTSuportePtoFix 

class TXMLTSuportePtoFix : public Xml::Xmldoc::TXMLNode, public IXMLTSuportePtoFix
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTSuportePtoFix 
  virtual int __fastcall Get_intId();
  virtual _di_IXMLTCoord __fastcall Get_coordPnt();
  virtual void __fastcall Set_intId(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTSuportePtoFixList 

class TXMLTSuportePtoFixList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTSuportePtoFixList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTSuportePtoFixList 
  virtual _di_IXMLTSuportePtoFix __fastcall Add();
  virtual _di_IXMLTSuportePtoFix __fastcall Insert(const int Index);

  virtual _di_IXMLTSuportePtoFix __fastcall Get_Item(int Index);
};

// TXMLTArranjoZ0Z1 

class TXMLTArranjoZ0Z1 : public Xml::Xmldoc::TXMLNode, public IXMLTArranjoZ0Z1
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTArranjoZ0Z1 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual double __fastcall Get_dblCorMax();
  virtual _di_IXMLTZseq __fastcall Get_zSeq();
  virtual _di_IXMLTCSeq __fastcall Get_dblCSeq();
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
  virtual void __fastcall Set_dblCorMax(double Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTArranjoZ0Z1List 

class TXMLTArranjoZ0Z1List : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTArranjoZ0Z1List
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTArranjoZ0Z1List 
  virtual _di_IXMLTArranjoZ0Z1 __fastcall Add();
  virtual _di_IXMLTArranjoZ0Z1 __fastcall Insert(const int Index);

  virtual _di_IXMLTArranjoZ0Z1 __fastcall Get_Item(int Index);
};

// TXMLTArranjoCaboZ0Z1 

class TXMLTArranjoCaboZ0Z1 : public Xml::Xmldoc::TXMLNode, public IXMLTArranjoCaboZ0Z1
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTArranjoCaboZ0Z1 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual int __fastcall Get_intCaboId();
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
  virtual void __fastcall Set_intCaboId(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTArranjoCaboZ0Z1List 

class TXMLTArranjoCaboZ0Z1List : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTArranjoCaboZ0Z1List
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTArranjoCaboZ0Z1List 
  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Add();
  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Insert(const int Index);

  virtual _di_IXMLTArranjoCaboZ0Z1 __fastcall Get_Item(int Index);
};

// TXMLTArranjoCaboRGMR 

class TXMLTArranjoCaboRGMR : public Xml::Xmldoc::TXMLNode, public IXMLTArranjoCaboRGMR
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTSuportePtoFixCaboList FPtoFix;
protected:
  // IXMLTArranjoCaboRGMR 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual System::UnicodeString __fastcall Get_strFases();
  virtual int __fastcall Get_intSuporteId();
  virtual _di_IXMLTSuportePtoFixCaboList __fastcall Get_PtoFix();
  virtual void __fastcall Set_strFases(System::UnicodeString Value);
  virtual void __fastcall Set_intSuporteId(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTArranjoCaboRGMRList 

class TXMLTArranjoCaboRGMRList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTArranjoCaboRGMRList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTArranjoCaboRGMRList 
  virtual _di_IXMLTArranjoCaboRGMR __fastcall Add();
  virtual _di_IXMLTArranjoCaboRGMR __fastcall Insert(const int Index);

  virtual _di_IXMLTArranjoCaboRGMR __fastcall Get_Item(int Index);
};

// TXMLTSuportePtoFixCabo 

class TXMLTSuportePtoFixCabo : public Xml::Xmldoc::TXMLNode, public IXMLTSuportePtoFixCabo
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTSuportePtoFixCabo 
  virtual int __fastcall Get_intId();
  virtual System::UnicodeString __fastcall Get_strFase();
  virtual int __fastcall Get_intCaboId();
  virtual void __fastcall Set_intId(int Value);
  virtual void __fastcall Set_strFase(System::UnicodeString Value);
  virtual void __fastcall Set_intCaboId(int Value);
};

// TXMLTSuportePtoFixCaboList 

class TXMLTSuportePtoFixCaboList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTSuportePtoFixCaboList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTSuportePtoFixCaboList 
  virtual _di_IXMLTSuportePtoFixCabo __fastcall Add();
  virtual _di_IXMLTSuportePtoFixCabo __fastcall Insert(const int Index);

  virtual _di_IXMLTSuportePtoFixCabo __fastcall Get_Item(int Index);
};

// TXMLTCurvaTipica 

class TXMLTCurvaTipica : public Xml::Xmldoc::TXMLNode, public IXMLTCurvaTipica
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLTPatamTipList FPatamar;
protected:
  // IXMLTCurvaTipica 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual int __fastcall Get_intTpoCurva();
  virtual int __fastcall Get_intUnidade();
  virtual int __fastcall Get_intEscala();
  virtual int __fastcall Get_intTpoCons();
  virtual double __fastcall Get_dblEneMin();
  virtual double __fastcall Get_dblEneMax();
  virtual int __fastcall Get_intAno();
  virtual int __fastcall Get_intMes();
  virtual int __fastcall Get_intNumPat();
  virtual _di_IXMLTPatamTipList __fastcall Get_Patamar();
  virtual void __fastcall Set_intTpoCurva(int Value);
  virtual void __fastcall Set_intUnidade(int Value);
  virtual void __fastcall Set_intEscala(int Value);
  virtual void __fastcall Set_intTpoCons(int Value);
  virtual void __fastcall Set_dblEneMin(double Value);
  virtual void __fastcall Set_dblEneMax(double Value);
  virtual void __fastcall Set_intAno(int Value);
  virtual void __fastcall Set_intMes(int Value);
  virtual void __fastcall Set_intNumPat(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTCurvaTipicaList 

class TXMLTCurvaTipicaList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTCurvaTipicaList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTCurvaTipicaList 
  virtual _di_IXMLTCurvaTipica __fastcall Add();
  virtual _di_IXMLTCurvaTipica __fastcall Insert(const int Index);

  virtual _di_IXMLTCurvaTipica __fastcall Get_Item(int Index);
};

// TXMLTPatamTip 

class TXMLTPatamTip : public Xml::Xmldoc::TXMLNode, public IXMLTPatamTip
{
  __IXMLNODE_IMPL__
private:
  _di_IXMLdouble_List FdblVal;
protected:
  // IXMLTPatamTip 
  virtual _di_IXMLTHorMin __fastcall Get_horIni();
  virtual _di_IXMLTHorMin __fastcall Get_horFim();
  virtual int __fastcall Get_intNumVal();
  virtual _di_IXMLdouble_List __fastcall Get_dblVal();
  virtual void __fastcall Set_intNumVal(int Value);
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLTPatamTipList 

class TXMLTPatamTipList : public Xml::Xmldoc::TXMLNodeCollection, public IXMLTPatamTipList
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLTPatamTipList 
  virtual _di_IXMLTPatamTip __fastcall Add();
  virtual _di_IXMLTPatamTip __fastcall Insert(const int Index);

  virtual _di_IXMLTPatamTip __fastcall Get_Item(int Index);
};

// TXMLTZId 

class TXMLTZId : public Xml::Xmldoc::TXMLNode, public IXMLTZId
{
  __IXMLNODE_IMPL__
protected:
  // IXMLTZId 
  virtual _di_IXMLTId __fastcall Get_id();
  virtual _di_IXMLTZ __fastcall Get_z();
public:
  virtual void __fastcall AfterConstruction(void);
};

// TXMLdouble_List 

class TXMLdouble_List : public Xml::Xmldoc::TXMLNodeCollection, public IXMLdouble_List
{
  __IXMLNODECOLLECTION_IMPL__
protected:
  // IXMLdouble_List 
  virtual Xml::Xmlintf::_di_IXMLNode __fastcall Add(const double Value);
  virtual Xml::Xmlintf::_di_IXMLNode __fastcall Insert(const int Index, const double Value);

  virtual double __fastcall Get_Item(int Index);
};

// Global Functions 

_di_IXMLTConcessionaria __fastcall GetConcessionaria(Xml::Xmlintf::_di_IXMLDocument Doc);
_di_IXMLTConcessionaria __fastcall GetConcessionaria(Xml::Xmldoc::TXMLDocument *Doc);
_di_IXMLTConcessionaria __fastcall LoadConcessionaria(const System::UnicodeString& FileName);
_di_IXMLTConcessionaria __fastcall  NewConcessionaria();

#define TargetNamespace ""

#endif