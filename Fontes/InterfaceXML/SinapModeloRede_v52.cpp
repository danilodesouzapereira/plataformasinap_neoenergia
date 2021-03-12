
// *************************************************************************************************************** //
//                                                                                                               
//                                               XML Data Binding                                                
//                                                                                                               
//         Generated on: 24/06/2014 07:05:18                                                                     
//       Generated from: Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\InterfaceXML\Modelo\SinapModeloRede_v52.xsd   
//   Settings stored in: Z:\Sinap\Rev4.0_XE\PlataformaSinap\Fontes\InterfaceXML\Modelo\SinapModeloRede_v52.xdb   
//                                                                                                               
// *************************************************************************************************************** //

#include <System.hpp>
#pragma hdrstop

#include "SinapModeloRede_v52.h"


// Global Functions 

_di_IXMLTConcessionaria __fastcall GetConcessionaria(Xml::Xmlintf::_di_IXMLDocument Doc)
{
  return (_di_IXMLTConcessionaria) Doc->GetDocBinding("Concessionaria", __classid(TXMLTConcessionaria), TargetNamespace);
};

_di_IXMLTConcessionaria __fastcall GetConcessionaria(Xml::Xmldoc::TXMLDocument *Doc)
{
  Xml::Xmlintf::_di_IXMLDocument DocIntf;
  Doc->GetInterface(DocIntf);
  return GetConcessionaria(DocIntf);
};

_di_IXMLTConcessionaria __fastcall LoadConcessionaria(const System::UnicodeString& FileName)
{
  return (_di_IXMLTConcessionaria) Xml::Xmldoc::LoadXMLDocument(FileName)->GetDocBinding("Concessionaria", __classid(TXMLTConcessionaria), TargetNamespace);
};

_di_IXMLTConcessionaria __fastcall  NewConcessionaria()
{
  return (_di_IXMLTConcessionaria) Xml::Xmldoc::NewXMLDocument()->GetDocBinding("Concessionaria", __classid(TXMLTConcessionaria), TargetNamespace);
};

// TXMLTConcessionaria 

void __fastcall TXMLTConcessionaria::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("Rede"), __classid(TXMLTRede));
  RegisterChildNode(System::UnicodeString("CaboRGMR"), __classid(TXMLTCaboRGMR));
  RegisterChildNode(System::UnicodeString("CaboZ0Z1"), __classid(TXMLTCaboZ0Z1));
  RegisterChildNode(System::UnicodeString("Suporte"), __classid(TXMLTSuporte));
  RegisterChildNode(System::UnicodeString("ArranjoZ0Z1"), __classid(TXMLTArranjoZ0Z1));
  RegisterChildNode(System::UnicodeString("ArranjoCaboZ0Z1"), __classid(TXMLTArranjoCaboZ0Z1));
  RegisterChildNode(System::UnicodeString("ArranjoCaboRGMR"), __classid(TXMLTArranjoCaboRGMR));
  RegisterChildNode(System::UnicodeString("CurvaTipicaCarga"), __classid(TXMLTCurvaTipica));
  RegisterChildNode(System::UnicodeString("CurvaTipicaCargaIP"), __classid(TXMLTCurvaTipica));
  RegisterChildNode(System::UnicodeString("CurvaTipicaCapacitor"), __classid(TXMLTCurvaTipica));
  RegisterChildNode(System::UnicodeString("CurvaTipicaReator"), __classid(TXMLTCurvaTipica));
  RegisterChildNode(System::UnicodeString("CurvaTipicaGeradorFoto"), __classid(TXMLTCurvaTipica));
  CollectionCreater<_di_IXMLTRedeList, TXMLTRedeList, IXMLTRede>::DoCreate(this, FRede, "Rede");
  CollectionCreater<_di_IXMLTCaboRGMRList, TXMLTCaboRGMRList, IXMLTCaboRGMR>::DoCreate(this, FCaboRGMR, "CaboRGMR");
  CollectionCreater<_di_IXMLTCaboZ0Z1List, TXMLTCaboZ0Z1List, IXMLTCaboZ0Z1>::DoCreate(this, FCaboZ0Z1, "CaboZ0Z1");
  CollectionCreater<_di_IXMLTSuporteList, TXMLTSuporteList, IXMLTSuporte>::DoCreate(this, FSuporte, "Suporte");
  CollectionCreater<_di_IXMLTArranjoZ0Z1List, TXMLTArranjoZ0Z1List, IXMLTArranjoZ0Z1>::DoCreate(this, FArranjoZ0Z1, "ArranjoZ0Z1");
  CollectionCreater<_di_IXMLTArranjoCaboZ0Z1List, TXMLTArranjoCaboZ0Z1List, IXMLTArranjoCaboZ0Z1>::DoCreate(this, FArranjoCaboZ0Z1, "ArranjoCaboZ0Z1");
  CollectionCreater<_di_IXMLTArranjoCaboRGMRList, TXMLTArranjoCaboRGMRList, IXMLTArranjoCaboRGMR>::DoCreate(this, FArranjoCaboRGMR, "ArranjoCaboRGMR");
  CollectionCreater<_di_IXMLTCurvaTipicaList, TXMLTCurvaTipicaList, IXMLTCurvaTipica>::DoCreate(this, FCurvaTipicaCarga, "CurvaTipicaCarga");
  CollectionCreater<_di_IXMLTCurvaTipicaList, TXMLTCurvaTipicaList, IXMLTCurvaTipica>::DoCreate(this, FCurvaTipicaCargaIP, "CurvaTipicaCargaIP");
  CollectionCreater<_di_IXMLTCurvaTipicaList, TXMLTCurvaTipicaList, IXMLTCurvaTipica>::DoCreate(this, FCurvaTipicaCapacitor, "CurvaTipicaCapacitor");
  CollectionCreater<_di_IXMLTCurvaTipicaList, TXMLTCurvaTipicaList, IXMLTCurvaTipica>::DoCreate(this, FCurvaTipicaReator, "CurvaTipicaReator");
  CollectionCreater<_di_IXMLTCurvaTipicaList, TXMLTCurvaTipicaList, IXMLTCurvaTipica>::DoCreate(this, FCurvaTipicaGeradorFoto, "CurvaTipicaGeradorFoto");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

System::UnicodeString __fastcall TXMLTConcessionaria::Get_strVer()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strVer")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTConcessionaria::Set_strVer(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strVer")]->NodeValue = Value;
};

_di_IXMLTId __fastcall TXMLTConcessionaria::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

_di_IXMLTRedeList __fastcall TXMLTConcessionaria::Get_Rede()
{
  return (_di_IXMLTRedeList) FRede;
};

_di_IXMLTCaboRGMRList __fastcall TXMLTConcessionaria::Get_CaboRGMR()
{
  return (_di_IXMLTCaboRGMRList) FCaboRGMR;
};

_di_IXMLTCaboZ0Z1List __fastcall TXMLTConcessionaria::Get_CaboZ0Z1()
{
  return (_di_IXMLTCaboZ0Z1List) FCaboZ0Z1;
};

_di_IXMLTSuporteList __fastcall TXMLTConcessionaria::Get_Suporte()
{
  return (_di_IXMLTSuporteList) FSuporte;
};

_di_IXMLTArranjoZ0Z1List __fastcall TXMLTConcessionaria::Get_ArranjoZ0Z1()
{
  return (_di_IXMLTArranjoZ0Z1List) FArranjoZ0Z1;
};

_di_IXMLTArranjoCaboZ0Z1List __fastcall TXMLTConcessionaria::Get_ArranjoCaboZ0Z1()
{
  return (_di_IXMLTArranjoCaboZ0Z1List) FArranjoCaboZ0Z1;
};

_di_IXMLTArranjoCaboRGMRList __fastcall TXMLTConcessionaria::Get_ArranjoCaboRGMR()
{
  return (_di_IXMLTArranjoCaboRGMRList) FArranjoCaboRGMR;
};

_di_IXMLTCurvaTipicaList __fastcall TXMLTConcessionaria::Get_CurvaTipicaCarga()
{
  return (_di_IXMLTCurvaTipicaList) FCurvaTipicaCarga;
};

_di_IXMLTCurvaTipicaList __fastcall TXMLTConcessionaria::Get_CurvaTipicaCargaIP()
{
  return (_di_IXMLTCurvaTipicaList) FCurvaTipicaCargaIP;
};

_di_IXMLTCurvaTipicaList __fastcall TXMLTConcessionaria::Get_CurvaTipicaCapacitor()
{
  return (_di_IXMLTCurvaTipicaList) FCurvaTipicaCapacitor;
};

_di_IXMLTCurvaTipicaList __fastcall TXMLTConcessionaria::Get_CurvaTipicaReator()
{
  return (_di_IXMLTCurvaTipicaList) FCurvaTipicaReator;
};

_di_IXMLTCurvaTipicaList __fastcall TXMLTConcessionaria::Get_CurvaTipicaGeradorFoto()
{
  return (_di_IXMLTCurvaTipicaList) FCurvaTipicaGeradorFoto;
};

// TXMLTId 

int __fastcall TXMLTId::Get_intId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue.operator int();
};

void __fastcall TXMLTId::Set_intId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTId::Get_strCod()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strCod")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTId::Set_strCod(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strCod")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTId::Get_strDsc()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strDsc")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTId::Set_strDsc(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strDsc")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTId::Get_strExtId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strExtId")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTId::Set_strExtId(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strExtId")]->NodeValue = Value;
};

// TXMLTRede 

void __fastcall TXMLTRede::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("cnjEqBar"), __classid(TXMLTCnjEqBar));
  RegisterChildNode(System::UnicodeString("cnjEqLig"), __classid(TXMLTCnjEqLig));
  RegisterChildNode(System::UnicodeString("Mutua"), __classid(TXMLTMutua));
  CollectionCreater<_di_IXMLTMutuaList, TXMLTMutuaList, IXMLTMutua>::DoCreate(this, FMutua, "Mutua");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTRede::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTRede::Get_strTpo()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strTpo")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTRede::Set_strTpo(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strTpo")]->NodeValue = Value;
};

_di_IXMLTCnjEqBar __fastcall TXMLTRede::Get_cnjEqBar()
{
  return (_di_IXMLTCnjEqBar) GetChildNodes()->Nodes[System::UnicodeString("cnjEqBar")];
};

_di_IXMLTCnjEqLig __fastcall TXMLTRede::Get_cnjEqLig()
{
  return (_di_IXMLTCnjEqLig) GetChildNodes()->Nodes[System::UnicodeString("cnjEqLig")];
};

_di_IXMLTMutuaList __fastcall TXMLTRede::Get_Mutua()
{
  return (_di_IXMLTMutuaList) FMutua;
};

System::UnicodeString __fastcall TXMLTRede::Get_strDominio()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strDominio")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTRede::Set_strDominio(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strDominio")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTRede::Get_strCluster()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strCluster")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTRede::Set_strCluster(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strCluster")]->NodeValue = Value;
};

// TXMLTRedeList 

_di_IXMLTRede __fastcall TXMLTRedeList::Add()
{
  return (_di_IXMLTRede) AddItem(-1);
};

_di_IXMLTRede __fastcall TXMLTRedeList::Insert(const int Index)
{
  return (_di_IXMLTRede) AddItem(Index);
};

_di_IXMLTRede __fastcall TXMLTRedeList::Get_Item(int Index)
{
  return (_di_IXMLTRede) List->Nodes[Index];
};

// TXMLTCnjEqBar 

void __fastcall TXMLTCnjEqBar::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("Capacitor"), __classid(TXMLTEqBarCapacitor));
  RegisterChildNode(System::UnicodeString("Carga"), __classid(TXMLTEqBarCarga));
  RegisterChildNode(System::UnicodeString("CargaIP"), __classid(TXMLTEqBarCargaIP));
  RegisterChildNode(System::UnicodeString("Filtro"), __classid(TXMLTEqBarFiltro));
  RegisterChildNode(System::UnicodeString("Gerador"), __classid(TXMLTEqBarGerador));
  RegisterChildNode(System::UnicodeString("Reator"), __classid(TXMLTEqBarReator));
  RegisterChildNode(System::UnicodeString("Suprimento"), __classid(TXMLTEqBarSuprimento));
  RegisterChildNode(System::UnicodeString("TrafoZZ"), __classid(TXMLTEqBarTrafoZZ));
  RegisterChildNode(System::UnicodeString("Medidor"), __classid(TXMLTEqBarMedidor));
  CollectionCreater<_di_IXMLTEqBarCapacitorList, TXMLTEqBarCapacitorList, IXMLTEqBarCapacitor>::DoCreate(this, FCapacitor, "Capacitor");
  CollectionCreater<_di_IXMLTEqBarCargaList, TXMLTEqBarCargaList, IXMLTEqBarCarga>::DoCreate(this, FCarga, "Carga");
  CollectionCreater<_di_IXMLTEqBarCargaIPList, TXMLTEqBarCargaIPList, IXMLTEqBarCargaIP>::DoCreate(this, FCargaIP, "CargaIP");
  CollectionCreater<_di_IXMLTEqBarFiltroList, TXMLTEqBarFiltroList, IXMLTEqBarFiltro>::DoCreate(this, FFiltro, "Filtro");
  CollectionCreater<_di_IXMLTEqBarGeradorList, TXMLTEqBarGeradorList, IXMLTEqBarGerador>::DoCreate(this, FGerador, "Gerador");
  CollectionCreater<_di_IXMLTEqBarReatorList, TXMLTEqBarReatorList, IXMLTEqBarReator>::DoCreate(this, FReator, "Reator");
  CollectionCreater<_di_IXMLTEqBarSuprimentoList, TXMLTEqBarSuprimentoList, IXMLTEqBarSuprimento>::DoCreate(this, FSuprimento, "Suprimento");
  CollectionCreater<_di_IXMLTEqBarTrafoZZList, TXMLTEqBarTrafoZZList, IXMLTEqBarTrafoZZ>::DoCreate(this, FTrafoZZ, "TrafoZZ");
  CollectionCreater<_di_IXMLTEqBarMedidorList, TXMLTEqBarMedidorList, IXMLTEqBarMedidor>::DoCreate(this, FMedidor, "Medidor");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqBarCapacitorList __fastcall TXMLTCnjEqBar::Get_Capacitor()
{
  return (_di_IXMLTEqBarCapacitorList) FCapacitor;
};

_di_IXMLTEqBarCargaList __fastcall TXMLTCnjEqBar::Get_Carga()
{
  return (_di_IXMLTEqBarCargaList) FCarga;
};

_di_IXMLTEqBarCargaIPList __fastcall TXMLTCnjEqBar::Get_CargaIP()
{
  return (_di_IXMLTEqBarCargaIPList) FCargaIP;
};

_di_IXMLTEqBarFiltroList __fastcall TXMLTCnjEqBar::Get_Filtro()
{
  return (_di_IXMLTEqBarFiltroList) FFiltro;
};

_di_IXMLTEqBarGeradorList __fastcall TXMLTCnjEqBar::Get_Gerador()
{
  return (_di_IXMLTEqBarGeradorList) FGerador;
};

_di_IXMLTEqBarReatorList __fastcall TXMLTCnjEqBar::Get_Reator()
{
  return (_di_IXMLTEqBarReatorList) FReator;
};

_di_IXMLTEqBarSuprimentoList __fastcall TXMLTCnjEqBar::Get_Suprimento()
{
  return (_di_IXMLTEqBarSuprimentoList) FSuprimento;
};

_di_IXMLTEqBarTrafoZZList __fastcall TXMLTCnjEqBar::Get_TrafoZZ()
{
  return (_di_IXMLTEqBarTrafoZZList) FTrafoZZ;
};

_di_IXMLTEqBarMedidorList __fastcall TXMLTCnjEqBar::Get_Medidor()
{
  return (_di_IXMLTEqBarMedidorList) FMedidor;
};

// TXMLTEqBarCapacitor 

void __fastcall TXMLTEqBarCapacitor::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarCapacitor::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

int __fastcall TXMLTEqBarCapacitor::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarCapacitor::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarCapacitor::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarCapacitor::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarCapacitor::Get_dblPotRtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarCapacitor::Set_dblPotRtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue = Value;
};

// TXMLTEqBarCapacitorList 

_di_IXMLTEqBarCapacitor __fastcall TXMLTEqBarCapacitorList::Add()
{
  return (_di_IXMLTEqBarCapacitor) AddItem(-1);
};

_di_IXMLTEqBarCapacitor __fastcall TXMLTEqBarCapacitorList::Insert(const int Index)
{
  return (_di_IXMLTEqBarCapacitor) AddItem(Index);
};

_di_IXMLTEqBarCapacitor __fastcall TXMLTEqBarCapacitorList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarCapacitor) List->Nodes[Index];
};

// TXMLTEqpto 

void __fastcall TXMLTEqpto::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("coordId"), __classid(TXMLTCoordId));
  RegisterChildNode(System::UnicodeString("Medicao"), __classid(TXMLTMedicao));
  CollectionCreater<_di_IXMLTCoordIdList, TXMLTCoordIdList, IXMLTCoordId>::DoCreate(this, FcoordId, "coordId");
  CollectionCreater<_di_IXMLTMedicaoList, TXMLTMedicaoList, IXMLTMedicao>::DoCreate(this, FMedicao, "Medicao");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTEqpto::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

_di_IXMLTCoordIdList __fastcall TXMLTEqpto::Get_coordId()
{
  return (_di_IXMLTCoordIdList) FcoordId;
};

_di_IXMLTMedicaoList __fastcall TXMLTEqpto::Get_Medicao()
{
  return (_di_IXMLTMedicaoList) FMedicao;
};

// TXMLTCoordId 

void __fastcall TXMLTCoordId::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("coordUTM"), __classid(TXMLTCoord));
  RegisterChildNode(System::UnicodeString("coordESQ1"), __classid(TXMLTCoord));
  RegisterChildNode(System::UnicodeString("coordESQ2"), __classid(TXMLTCoord));
  RegisterChildNode(System::UnicodeString("coordIco"), __classid(TXMLTCoordIco));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTCoordId::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTCoordId::Get_strCod()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strCod")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTCoordId::Set_strCod(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strCod")]->NodeValue = Value;
};

_di_IXMLTCoord __fastcall TXMLTCoordId::Get_coordUTM()
{
  return (_di_IXMLTCoord) GetChildNodes()->Nodes[System::UnicodeString("coordUTM")];
};

_di_IXMLTCoord __fastcall TXMLTCoordId::Get_coordESQ1()
{
  return (_di_IXMLTCoord) GetChildNodes()->Nodes[System::UnicodeString("coordESQ1")];
};

_di_IXMLTCoord __fastcall TXMLTCoordId::Get_coordESQ2()
{
  return (_di_IXMLTCoord) GetChildNodes()->Nodes[System::UnicodeString("coordESQ2")];
};

double __fastcall TXMLTCoordId::Get_dblPosBar()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPosBar")]->NodeValue.operator double();
};

void __fastcall TXMLTCoordId::Set_dblPosBar(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPosBar")]->NodeValue = Value;
};

_di_IXMLTCoordIco __fastcall TXMLTCoordId::Get_coordIco()
{
  return (_di_IXMLTCoordIco) GetChildNodes()->Nodes[System::UnicodeString("coordIco")];
};

int __fastcall TXMLTCoordId::Get_intCoordNdx()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intCoordNdx")]->NodeValue.operator int();
};

void __fastcall TXMLTCoordId::Set_intCoordNdx(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intCoordNdx")]->NodeValue = Value;
};

// TXMLTCoordIdList 

_di_IXMLTCoordId __fastcall TXMLTCoordIdList::Add()
{
  return (_di_IXMLTCoordId) AddItem(-1);
};

_di_IXMLTCoordId __fastcall TXMLTCoordIdList::Insert(const int Index)
{
  return (_di_IXMLTCoordId) AddItem(Index);
};

_di_IXMLTCoordId __fastcall TXMLTCoordIdList::Get_Item(int Index)
{
  return (_di_IXMLTCoordId) List->Nodes[Index];
};

// TXMLTCoord 

double __fastcall TXMLTCoord::Get_dblX()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblX")]->NodeValue.operator double();
};

void __fastcall TXMLTCoord::Set_dblX(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblX")]->NodeValue = Value;
};

double __fastcall TXMLTCoord::Get_dblY()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblY")]->NodeValue.operator double();
};

void __fastcall TXMLTCoord::Set_dblY(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblY")]->NodeValue = Value;
};

// TXMLTCoordIco 

double __fastcall TXMLTCoordIco::Get_dblDxBar()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblDxBar")]->NodeValue.operator double();
};

void __fastcall TXMLTCoordIco::Set_dblDxBar(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblDxBar")]->NodeValue = Value;
};

double __fastcall TXMLTCoordIco::Get_dblDyBar()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblDyBar")]->NodeValue.operator double();
};

void __fastcall TXMLTCoordIco::Set_dblDyBar(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblDyBar")]->NodeValue = Value;
};

// TXMLTMedicao 

void __fastcall TXMLTMedicao::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("medidorId"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("canalId"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("Patamar"), __classid(TXMLTPatamProp));
  CollectionCreater<_di_IXMLTPatamPropList, TXMLTPatamPropList, IXMLTPatamProp>::DoCreate(this, FPatamar, "Patamar");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTMedicao::Get_medidorId()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("medidorId")];
};

_di_IXMLTId __fastcall TXMLTMedicao::Get_canalId()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("canalId")];
};

int __fastcall TXMLTMedicao::Get_intTpoCnl()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoCnl")]->NodeValue.operator int();
};

void __fastcall TXMLTMedicao::Set_intTpoCnl(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoCnl")]->NodeValue = Value;
};

double __fastcall TXMLTMedicao::Get_dblSigmaCnl()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblSigmaCnl")]->NodeValue.operator double();
};

void __fastcall TXMLTMedicao::Set_dblSigmaCnl(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblSigmaCnl")]->NodeValue = Value;
};

_di_IXMLTPatamPropList __fastcall TXMLTMedicao::Get_Patamar()
{
  return (_di_IXMLTPatamPropList) FPatamar;
};

// TXMLTMedicaoList 

_di_IXMLTMedicao __fastcall TXMLTMedicaoList::Add()
{
  return (_di_IXMLTMedicao) AddItem(-1);
};

_di_IXMLTMedicao __fastcall TXMLTMedicaoList::Insert(const int Index)
{
  return (_di_IXMLTMedicao) AddItem(Index);
};

_di_IXMLTMedicao __fastcall TXMLTMedicaoList::Get_Item(int Index)
{
  return (_di_IXMLTMedicao) List->Nodes[Index];
};

// TXMLTPatamProp 

void __fastcall TXMLTPatamProp::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("horIni"), __classid(TXMLTHorMin));
  RegisterChildNode(System::UnicodeString("horFim"), __classid(TXMLTHorMin));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTHorMin __fastcall TXMLTPatamProp::Get_horIni()
{
  return (_di_IXMLTHorMin) GetChildNodes()->Nodes[System::UnicodeString("horIni")];
};

_di_IXMLTHorMin __fastcall TXMLTPatamProp::Get_horFim()
{
  return (_di_IXMLTHorMin) GetChildNodes()->Nodes[System::UnicodeString("horFim")];
};

double __fastcall TXMLTPatamProp::Get_dblPotAtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue.operator double();
};

void __fastcall TXMLTPatamProp::Set_dblPotAtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue = Value;
};

double __fastcall TXMLTPatamProp::Get_dblPotRtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue.operator double();
};

void __fastcall TXMLTPatamProp::Set_dblPotRtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue = Value;
};

double __fastcall TXMLTPatamProp::Get_dblMdlTns()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblMdlTns")]->NodeValue.operator double();
};

void __fastcall TXMLTPatamProp::Set_dblMdlTns(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblMdlTns")]->NodeValue = Value;
};

double __fastcall TXMLTPatamProp::Get_dblAng()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblAng")]->NodeValue.operator double();
};

void __fastcall TXMLTPatamProp::Set_dblAng(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblAng")]->NodeValue = Value;
};

// TXMLTPatamPropList 

_di_IXMLTPatamProp __fastcall TXMLTPatamPropList::Add()
{
  return (_di_IXMLTPatamProp) AddItem(-1);
};

_di_IXMLTPatamProp __fastcall TXMLTPatamPropList::Insert(const int Index)
{
  return (_di_IXMLTPatamProp) AddItem(Index);
};

_di_IXMLTPatamProp __fastcall TXMLTPatamPropList::Get_Item(int Index)
{
  return (_di_IXMLTPatamProp) List->Nodes[Index];
};

// TXMLTHorMin 

int __fastcall TXMLTHorMin::Get_intHor()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intHor")]->NodeValue.operator int();
};

void __fastcall TXMLTHorMin::Set_intHor(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intHor")]->NodeValue = Value;
};

int __fastcall TXMLTHorMin::Get_intMin()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intMin")]->NodeValue.operator int();
};

void __fastcall TXMLTHorMin::Set_intMin(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intMin")]->NodeValue = Value;
};

// TXMLTEqBarCarga 

void __fastcall TXMLTEqBarCarga::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("Patamar"), __classid(TXMLTPatamProp));
  CollectionCreater<_di_IXMLTPatamPropList, TXMLTPatamPropList, IXMLTPatamProp>::DoCreate(this, FPatamar, "Patamar");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarCarga::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

int __fastcall TXMLTEqBarCarga::Get_intTpoCons()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoCons")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarCarga::Set_intTpoCons(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoCons")]->NodeValue = Value;
};

int __fastcall TXMLTEqBarCarga::Get_intTpoModCrg()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoModCrg")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarCarga::Set_intTpoModCrg(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoModCrg")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarCarga::Get_dblEne()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblEne")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarCarga::Set_dblEne(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblEne")]->NodeValue = Value;
};

int __fastcall TXMLTEqBarCarga::Get_intNumCons()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intNumCons")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarCarga::Set_intNumCons(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intNumCons")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTEqBarCarga::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTEqBarCarga::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

_di_IXMLTPatamPropList __fastcall TXMLTEqBarCarga::Get_Patamar()
{
  return (_di_IXMLTPatamPropList) FPatamar;
};

// TXMLTEqBarCargaList 

_di_IXMLTEqBarCarga __fastcall TXMLTEqBarCargaList::Add()
{
  return (_di_IXMLTEqBarCarga) AddItem(-1);
};

_di_IXMLTEqBarCarga __fastcall TXMLTEqBarCargaList::Insert(const int Index)
{
  return (_di_IXMLTEqBarCarga) AddItem(Index);
};

_di_IXMLTEqBarCarga __fastcall TXMLTEqBarCargaList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarCarga) List->Nodes[Index];
};

// TXMLTEqBarCargaIP 

void __fastcall TXMLTEqBarCargaIP::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarCargaIP::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

System::UnicodeString __fastcall TXMLTEqBarCargaIP::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTEqBarCargaIP::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarCargaIP::Get_dblPotAtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarCargaIP::Set_dblPotAtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarCargaIP::Get_dblFtrPot()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblFtrPot")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarCargaIP::Set_dblFtrPot(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblFtrPot")]->NodeValue = Value;
};

int __fastcall TXMLTEqBarCargaIP::Get_intNumLamp()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intNumLamp")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarCargaIP::Set_intNumLamp(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intNumLamp")]->NodeValue = Value;
};

// TXMLTEqBarCargaIPList 

_di_IXMLTEqBarCargaIP __fastcall TXMLTEqBarCargaIPList::Add()
{
  return (_di_IXMLTEqBarCargaIP) AddItem(-1);
};

_di_IXMLTEqBarCargaIP __fastcall TXMLTEqBarCargaIPList::Insert(const int Index)
{
  return (_di_IXMLTEqBarCargaIP) AddItem(Index);
};

_di_IXMLTEqBarCargaIP __fastcall TXMLTEqBarCargaIPList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarCargaIP) List->Nodes[Index];
};

// TXMLTEqBarFiltro 

void __fastcall TXMLTEqBarFiltro::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("z0"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarFiltro::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

double __fastcall TXMLTEqBarFiltro::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarFiltro::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarFiltro::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarFiltro::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

_di_IXMLTZ __fastcall TXMLTEqBarFiltro::Get_z0()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z0")];
};

// TXMLTEqBarFiltroList 

_di_IXMLTEqBarFiltro __fastcall TXMLTEqBarFiltroList::Add()
{
  return (_di_IXMLTEqBarFiltro) AddItem(-1);
};

_di_IXMLTEqBarFiltro __fastcall TXMLTEqBarFiltroList::Insert(const int Index)
{
  return (_di_IXMLTEqBarFiltro) AddItem(Index);
};

_di_IXMLTEqBarFiltro __fastcall TXMLTEqBarFiltroList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarFiltro) List->Nodes[Index];
};

// TXMLTZ 

double __fastcall TXMLTZ::Get_dblR()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblR")]->NodeValue.operator double();
};

void __fastcall TXMLTZ::Set_dblR(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblR")]->NodeValue = Value;
};

double __fastcall TXMLTZ::Get_dblX()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblX")]->NodeValue.operator double();
};

void __fastcall TXMLTZ::Set_dblX(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblX")]->NodeValue = Value;
};

// TXMLTEqBarGerador 

void __fastcall TXMLTEqBarGerador::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("Patamar"), __classid(TXMLTPatamProp));
  CollectionCreater<_di_IXMLTPatamPropList, TXMLTPatamPropList, IXMLTPatamProp>::DoCreate(this, FPatamar, "Patamar");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarGerador::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

int __fastcall TXMLTEqBarGerador::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarGerador::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

int __fastcall TXMLTEqBarGerador::Get_intTpoSup()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoSup")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarGerador::Set_intTpoSup(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoSup")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarGerador::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarGerador::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarGerador::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarGerador::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarGerador::Get_dblPotMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotMax")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarGerador::Set_dblPotMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotMax")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarGerador::Get_dblFtrPotLim()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblFtrPotLim")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarGerador::Set_dblFtrPotLim(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblFtrPotLim")]->NodeValue = Value;
};

_di_IXMLTZseq __fastcall TXMLTEqBarGerador::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

_di_IXMLTPatamPropList __fastcall TXMLTEqBarGerador::Get_Patamar()
{
  return (_di_IXMLTPatamPropList) FPatamar;
};

// TXMLTEqBarGeradorList 

_di_IXMLTEqBarGerador __fastcall TXMLTEqBarGeradorList::Add()
{
  return (_di_IXMLTEqBarGerador) AddItem(-1);
};

_di_IXMLTEqBarGerador __fastcall TXMLTEqBarGeradorList::Insert(const int Index)
{
  return (_di_IXMLTEqBarGerador) AddItem(Index);
};

_di_IXMLTEqBarGerador __fastcall TXMLTEqBarGeradorList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarGerador) List->Nodes[Index];
};

// TXMLTZseq 

void __fastcall TXMLTZseq::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("z0"), __classid(TXMLTZ));
  RegisterChildNode(System::UnicodeString("z1"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTZ __fastcall TXMLTZseq::Get_z0()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z0")];
};

_di_IXMLTZ __fastcall TXMLTZseq::Get_z1()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z1")];
};

// TXMLTEqBarReator 

void __fastcall TXMLTEqBarReator::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarReator::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

int __fastcall TXMLTEqBarReator::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarReator::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarReator::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarReator::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarReator::Get_dblPotRtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarReator::Set_dblPotRtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue = Value;
};

// TXMLTEqBarReatorList 

_di_IXMLTEqBarReator __fastcall TXMLTEqBarReatorList::Add()
{
  return (_di_IXMLTEqBarReator) AddItem(-1);
};

_di_IXMLTEqBarReator __fastcall TXMLTEqBarReatorList::Insert(const int Index)
{
  return (_di_IXMLTEqBarReator) AddItem(Index);
};

_di_IXMLTEqBarReator __fastcall TXMLTEqBarReatorList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarReator) List->Nodes[Index];
};

// TXMLTEqBarSuprimento 

void __fastcall TXMLTEqBarSuprimento::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("cpxPotCC3f"), __classid(TXMLTPotCpx));
  RegisterChildNode(System::UnicodeString("cpxPotCCFt"), __classid(TXMLTPotCpx));
  RegisterChildNode(System::UnicodeString("zEqv"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("Patamar"), __classid(TXMLTPatamProp));
  CollectionCreater<_di_IXMLTPatamPropList, TXMLTPatamPropList, IXMLTPatamProp>::DoCreate(this, FPatamar, "Patamar");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarSuprimento::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

int __fastcall TXMLTEqBarSuprimento::Get_intTpoSup()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoSup")]->NodeValue.operator int();
};

void __fastcall TXMLTEqBarSuprimento::Set_intTpoSup(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoSup")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarSuprimento::Get_dblPotMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotMax")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarSuprimento::Set_dblPotMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotMax")]->NodeValue = Value;
};

_di_IXMLTPotCpx __fastcall TXMLTEqBarSuprimento::Get_cpxPotCC3f()
{
  return (_di_IXMLTPotCpx) GetChildNodes()->Nodes[System::UnicodeString("cpxPotCC3f")];
};

_di_IXMLTPotCpx __fastcall TXMLTEqBarSuprimento::Get_cpxPotCCFt()
{
  return (_di_IXMLTPotCpx) GetChildNodes()->Nodes[System::UnicodeString("cpxPotCCFt")];
};

double __fastcall TXMLTEqBarSuprimento::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarSuprimento::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

_di_IXMLTZseq __fastcall TXMLTEqBarSuprimento::Get_zEqv()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zEqv")];
};

_di_IXMLTPatamPropList __fastcall TXMLTEqBarSuprimento::Get_Patamar()
{
  return (_di_IXMLTPatamPropList) FPatamar;
};

// TXMLTEqBarSuprimentoList 

_di_IXMLTEqBarSuprimento __fastcall TXMLTEqBarSuprimentoList::Add()
{
  return (_di_IXMLTEqBarSuprimento) AddItem(-1);
};

_di_IXMLTEqBarSuprimento __fastcall TXMLTEqBarSuprimentoList::Insert(const int Index)
{
  return (_di_IXMLTEqBarSuprimento) AddItem(Index);
};

_di_IXMLTEqBarSuprimento __fastcall TXMLTEqBarSuprimentoList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarSuprimento) List->Nodes[Index];
};

// TXMLTPotCpx 

double __fastcall TXMLTPotCpx::Get_dblPotAtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue.operator double();
};

void __fastcall TXMLTPotCpx::Set_dblPotAtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotAtv")]->NodeValue = Value;
};

double __fastcall TXMLTPotCpx::Get_dblPotRtv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue.operator double();
};

void __fastcall TXMLTPotCpx::Set_dblPotRtv(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotRtv")]->NodeValue = Value;
};

// TXMLTEqBarTrafoZZ 

void __fastcall TXMLTEqBarTrafoZZ::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("z0"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarTrafoZZ::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

double __fastcall TXMLTEqBarTrafoZZ::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarTrafoZZ::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqBarTrafoZZ::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqBarTrafoZZ::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

_di_IXMLTZ __fastcall TXMLTEqBarTrafoZZ::Get_z0()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z0")];
};

// TXMLTEqBarTrafoZZList 

_di_IXMLTEqBarTrafoZZ __fastcall TXMLTEqBarTrafoZZList::Add()
{
  return (_di_IXMLTEqBarTrafoZZ) AddItem(-1);
};

_di_IXMLTEqBarTrafoZZ __fastcall TXMLTEqBarTrafoZZList::Insert(const int Index)
{
  return (_di_IXMLTEqBarTrafoZZ) AddItem(Index);
};

_di_IXMLTEqBarTrafoZZ __fastcall TXMLTEqBarTrafoZZList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarTrafoZZ) List->Nodes[Index];
};

// TXMLTEqBarMedidor 

void __fastcall TXMLTEqBarMedidor::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqbar"), __classid(TXMLTEqpto));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqBarMedidor::Get_eqbar()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqbar")];
};

// TXMLTEqBarMedidorList 

_di_IXMLTEqBarMedidor __fastcall TXMLTEqBarMedidorList::Add()
{
  return (_di_IXMLTEqBarMedidor) AddItem(-1);
};

_di_IXMLTEqBarMedidor __fastcall TXMLTEqBarMedidorList::Insert(const int Index)
{
  return (_di_IXMLTEqBarMedidor) AddItem(Index);
};

_di_IXMLTEqBarMedidor __fastcall TXMLTEqBarMedidorList::Get_Item(int Index)
{
  return (_di_IXMLTEqBarMedidor) List->Nodes[Index];
};

// TXMLTCnjEqLig 

void __fastcall TXMLTCnjEqLig::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("CapacitorSerie"), __classid(TXMLTEqLigCapacitorSerie));
  RegisterChildNode(System::UnicodeString("Chave"), __classid(TXMLTEqLigChave));
  RegisterChildNode(System::UnicodeString("Regulador"), __classid(TXMLTEqLigRegulador));
  RegisterChildNode(System::UnicodeString("Trafo"), __classid(TXMLTEqLigTrafo));
  RegisterChildNode(System::UnicodeString("Trafo3E"), __classid(TXMLTEqLigTrafo3E));
  RegisterChildNode(System::UnicodeString("Trecho"), __classid(TXMLTEqLigTrecho));
  CollectionCreater<_di_IXMLTEqLigCapacitorSerieList, TXMLTEqLigCapacitorSerieList, IXMLTEqLigCapacitorSerie>::DoCreate(this, FCapacitorSerie, "CapacitorSerie");
  CollectionCreater<_di_IXMLTEqLigChaveList, TXMLTEqLigChaveList, IXMLTEqLigChave>::DoCreate(this, FChave, "Chave");
  CollectionCreater<_di_IXMLTEqLigReguladorList, TXMLTEqLigReguladorList, IXMLTEqLigRegulador>::DoCreate(this, FRegulador, "Regulador");
  CollectionCreater<_di_IXMLTEqLigTrafoList, TXMLTEqLigTrafoList, IXMLTEqLigTrafo>::DoCreate(this, FTrafo, "Trafo");
  CollectionCreater<_di_IXMLTEqLigTrafo3EList, TXMLTEqLigTrafo3EList, IXMLTEqLigTrafo3E>::DoCreate(this, FTrafo3E, "Trafo3E");
  CollectionCreater<_di_IXMLTEqLigTrechoList, TXMLTEqLigTrechoList, IXMLTEqLigTrecho>::DoCreate(this, FTrecho, "Trecho");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqLigCapacitorSerieList __fastcall TXMLTCnjEqLig::Get_CapacitorSerie()
{
  return (_di_IXMLTEqLigCapacitorSerieList) FCapacitorSerie;
};

_di_IXMLTEqLigChaveList __fastcall TXMLTCnjEqLig::Get_Chave()
{
  return (_di_IXMLTEqLigChaveList) FChave;
};

_di_IXMLTEqLigReguladorList __fastcall TXMLTCnjEqLig::Get_Regulador()
{
  return (_di_IXMLTEqLigReguladorList) FRegulador;
};

_di_IXMLTEqLigTrafoList __fastcall TXMLTCnjEqLig::Get_Trafo()
{
  return (_di_IXMLTEqLigTrafoList) FTrafo;
};

_di_IXMLTEqLigTrafo3EList __fastcall TXMLTCnjEqLig::Get_Trafo3E()
{
  return (_di_IXMLTEqLigTrafo3EList) FTrafo3E;
};

_di_IXMLTEqLigTrechoList __fastcall TXMLTCnjEqLig::Get_Trecho()
{
  return (_di_IXMLTEqLigTrechoList) FTrecho;
};

// TXMLTEqLigCapacitorSerie 

void __fastcall TXMLTEqLigCapacitorSerie::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("z"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigCapacitorSerie::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

double __fastcall TXMLTEqLigCapacitorSerie::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigCapacitorSerie::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigCapacitorSerie::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigCapacitorSerie::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

_di_IXMLTZ __fastcall TXMLTEqLigCapacitorSerie::Get_z()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z")];
};

// TXMLTEqLigCapacitorSerieList 

_di_IXMLTEqLigCapacitorSerie __fastcall TXMLTEqLigCapacitorSerieList::Add()
{
  return (_di_IXMLTEqLigCapacitorSerie) AddItem(-1);
};

_di_IXMLTEqLigCapacitorSerie __fastcall TXMLTEqLigCapacitorSerieList::Insert(const int Index)
{
  return (_di_IXMLTEqLigCapacitorSerie) AddItem(Index);
};

_di_IXMLTEqLigCapacitorSerie __fastcall TXMLTEqLigCapacitorSerieList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigCapacitorSerie) List->Nodes[Index];
};

// TXMLTEqLigChave 

void __fastcall TXMLTEqLigChave::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigChave::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

System::UnicodeString __fastcall TXMLTEqLigChave::Get_strTpoChv()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strTpoChv")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTEqLigChave::Set_strTpoChv(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strTpoChv")]->NodeValue = Value;
};

int __fastcall TXMLTEqLigChave::Get_intEst()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intEst")]->NodeValue.operator int();
};

void __fastcall TXMLTEqLigChave::Set_intEst(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intEst")]->NodeValue = Value;
};

bool __fastcall TXMLTEqLigChave::Get_bolBlq()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolBlq")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigChave::Set_bolBlq(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolBlq")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigChave::Get_dblCorNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCorNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigChave::Set_dblCorNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCorNom")]->NodeValue = Value;
};

// TXMLTEqLigChaveList 

_di_IXMLTEqLigChave __fastcall TXMLTEqLigChaveList::Add()
{
  return (_di_IXMLTEqLigChave) AddItem(-1);
};

_di_IXMLTEqLigChave __fastcall TXMLTEqLigChaveList::Insert(const int Index)
{
  return (_di_IXMLTEqLigChave) AddItem(Index);
};

_di_IXMLTEqLigChave __fastcall TXMLTEqLigChaveList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigChave) List->Nodes[Index];
};

// TXMLTEqLigRegulador 

void __fastcall TXMLTEqLigRegulador::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("coordBarRefSnt"), __classid(TXMLTCoordId));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("regTapFix"), __classid(TXMLTRegTapFix));
  RegisterChildNode(System::UnicodeString("regTapAjs"), __classid(TXMLTRegTapAjs));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigRegulador::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

int __fastcall TXMLTEqLigRegulador::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqLigRegulador::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigRegulador::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigRegulador::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

bool __fastcall TXMLTEqLigRegulador::Get_bolBypass()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolBypass")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigRegulador::Set_bolBypass(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolBypass")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigRegulador::Get_dblFxaVar()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblFxaVar")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigRegulador::Set_dblFxaVar(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblFxaVar")]->NodeValue = Value;
};

int __fastcall TXMLTEqLigRegulador::Get_intPasNum()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intPasNum")]->NodeValue.operator int();
};

void __fastcall TXMLTEqLigRegulador::Set_intPasNum(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intPasNum")]->NodeValue = Value;
};

bool __fastcall TXMLTEqLigRegulador::Get_bolSntRev()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolSntRev")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigRegulador::Set_bolSntRev(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolSntRev")]->NodeValue = Value;
};

_di_IXMLTCoordId __fastcall TXMLTEqLigRegulador::Get_coordBarRefSnt()
{
  return (_di_IXMLTCoordId) GetChildNodes()->Nodes[System::UnicodeString("coordBarRefSnt")];
};

_di_IXMLTZseq __fastcall TXMLTEqLigRegulador::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

_di_IXMLTRegTapFix __fastcall TXMLTEqLigRegulador::Get_regTapFix()
{
  return (_di_IXMLTRegTapFix) GetChildNodes()->Nodes[System::UnicodeString("regTapFix")];
};

_di_IXMLTRegTapAjs __fastcall TXMLTEqLigRegulador::Get_regTapAjs()
{
  return (_di_IXMLTRegTapAjs) GetChildNodes()->Nodes[System::UnicodeString("regTapAjs")];
};

// TXMLTEqLigReguladorList 

_di_IXMLTEqLigRegulador __fastcall TXMLTEqLigReguladorList::Add()
{
  return (_di_IXMLTEqLigRegulador) AddItem(-1);
};

_di_IXMLTEqLigRegulador __fastcall TXMLTEqLigReguladorList::Insert(const int Index)
{
  return (_di_IXMLTEqLigRegulador) AddItem(Index);
};

_di_IXMLTEqLigRegulador __fastcall TXMLTEqLigReguladorList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigRegulador) List->Nodes[Index];
};

// TXMLTRegTapFix 

int __fastcall TXMLTRegTapFix::Get_intPasSel()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intPasSel")]->NodeValue.operator int();
};

void __fastcall TXMLTRegTapFix::Set_intPasSel(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intPasSel")]->NodeValue = Value;
};

// TXMLTRegTapAjs 

void __fastcall TXMLTRegTapAjs::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("coordBarRefCfg"), __classid(TXMLTCoordId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

double __fastcall TXMLTRegTapAjs::Get_dblTnsAlvo()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsAlvo")]->NodeValue.operator double();
};

void __fastcall TXMLTRegTapAjs::Set_dblTnsAlvo(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsAlvo")]->NodeValue = Value;
};

_di_IXMLTCoordId __fastcall TXMLTRegTapAjs::Get_coordBarRefCfg()
{
  return (_di_IXMLTCoordId) GetChildNodes()->Nodes[System::UnicodeString("coordBarRefCfg")];
};

// TXMLTEqLigTrafo 

void __fastcall TXMLTEqLigTrafo::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("Enrol"), __classid(TXMLTEnrol));
  RegisterChildNode(System::UnicodeString("ltc"), __classid(TXMLTLtc));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  CollectionCreater<_di_IXMLTEnrolList, TXMLTEnrolList, IXMLTEnrol>::DoCreate(this, FEnrol, "Enrol");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigTrafo::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

int __fastcall TXMLTEqLigTrafo::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqLigTrafo::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

bool __fastcall TXMLTEqLigTrafo::Get_bolAuto()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolAuto")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigTrafo::Set_bolAuto(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolAuto")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigTrafo::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigTrafo::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigTrafo::Get_dblPerFer()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPerFer")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigTrafo::Set_dblPerFer(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPerFer")]->NodeValue = Value;
};

_di_IXMLTEnrolList __fastcall TXMLTEqLigTrafo::Get_Enrol()
{
  return (_di_IXMLTEnrolList) FEnrol;
};

_di_IXMLTLtc __fastcall TXMLTEqLigTrafo::Get_ltc()
{
  return (_di_IXMLTLtc) GetChildNodes()->Nodes[System::UnicodeString("ltc")];
};

_di_IXMLTZseq __fastcall TXMLTEqLigTrafo::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

// TXMLTEqLigTrafoList 

_di_IXMLTEqLigTrafo __fastcall TXMLTEqLigTrafoList::Add()
{
  return (_di_IXMLTEqLigTrafo) AddItem(-1);
};

_di_IXMLTEqLigTrafo __fastcall TXMLTEqLigTrafoList::Insert(const int Index)
{
  return (_di_IXMLTEqLigTrafo) AddItem(Index);
};

_di_IXMLTEqLigTrafo __fastcall TXMLTEqLigTrafoList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigTrafo) List->Nodes[Index];
};

// TXMLTEnrol 

void __fastcall TXMLTEnrol::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("zTerra"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

int __fastcall TXMLTEnrol::Get_intId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue.operator int();
};

void __fastcall TXMLTEnrol::Set_intId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue = Value;
};

int __fastcall TXMLTEnrol::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEnrol::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

double __fastcall TXMLTEnrol::Get_dblPotNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEnrol::Set_dblPotNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotNom")]->NodeValue = Value;
};

double __fastcall TXMLTEnrol::Get_dblTnsNom()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue.operator double();
};

void __fastcall TXMLTEnrol::Set_dblTnsNom(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsNom")]->NodeValue = Value;
};

double __fastcall TXMLTEnrol::Get_dblTnsTap()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsTap")]->NodeValue.operator double();
};

void __fastcall TXMLTEnrol::Set_dblTnsTap(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsTap")]->NodeValue = Value;
};

double __fastcall TXMLTEnrol::Get_dblAngDef()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblAngDef")]->NodeValue.operator double();
};

void __fastcall TXMLTEnrol::Set_dblAngDef(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblAngDef")]->NodeValue = Value;
};

_di_IXMLTZ __fastcall TXMLTEnrol::Get_zTerra()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("zTerra")];
};

System::UnicodeString __fastcall TXMLTEnrol::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTEnrol::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

// TXMLTEnrolList 

_di_IXMLTEnrol __fastcall TXMLTEnrolList::Add()
{
  return (_di_IXMLTEnrol) AddItem(-1);
};

_di_IXMLTEnrol __fastcall TXMLTEnrolList::Insert(const int Index)
{
  return (_di_IXMLTEnrol) AddItem(Index);
};

_di_IXMLTEnrol __fastcall TXMLTEnrolList::Get_Item(int Index)
{
  return (_di_IXMLTEnrol) List->Nodes[Index];
};

// TXMLTLtc 

void __fastcall TXMLTLtc::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("coordBarCtl"), __classid(TXMLTCoordId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

int __fastcall TXMLTLtc::Get_intTpoUtl()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoUtl")]->NodeValue.operator int();
};

void __fastcall TXMLTLtc::Set_intTpoUtl(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoUtl")]->NodeValue = Value;
};

double __fastcall TXMLTLtc::Get_dblTns()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTns")]->NodeValue.operator double();
};

void __fastcall TXMLTLtc::Set_dblTns(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTns")]->NodeValue = Value;
};

_di_IXMLTCoordId __fastcall TXMLTLtc::Get_coordBarCtl()
{
  return (_di_IXMLTCoordId) GetChildNodes()->Nodes[System::UnicodeString("coordBarCtl")];
};

// TXMLTEqLigTrafo3E 

void __fastcall TXMLTEqLigTrafo3E::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("Enrol"), __classid(TXMLTEnrol));
  RegisterChildNode(System::UnicodeString("zSeqPS"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("zSeqPT"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("zSeqST"), __classid(TXMLTZseq));
  CollectionCreater<_di_IXMLTEnrolList, TXMLTEnrolList, IXMLTEnrol>::DoCreate(this, FEnrol, "Enrol");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigTrafo3E::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

int __fastcall TXMLTEqLigTrafo3E::Get_intTpoLig()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue.operator int();
};

void __fastcall TXMLTEqLigTrafo3E::Set_intTpoLig(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoLig")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigTrafo3E::Get_dblPotBas()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPotBas")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigTrafo3E::Set_dblPotBas(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPotBas")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigTrafo3E::Get_dblPerFer()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPerFer")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigTrafo3E::Set_dblPerFer(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPerFer")]->NodeValue = Value;
};

_di_IXMLTEnrolList __fastcall TXMLTEqLigTrafo3E::Get_Enrol()
{
  return (_di_IXMLTEnrolList) FEnrol;
};

_di_IXMLTZseq __fastcall TXMLTEqLigTrafo3E::Get_zSeqPS()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeqPS")];
};

_di_IXMLTZseq __fastcall TXMLTEqLigTrafo3E::Get_zSeqPT()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeqPT")];
};

_di_IXMLTZseq __fastcall TXMLTEqLigTrafo3E::Get_zSeqST()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeqST")];
};

// TXMLTEqLigTrafo3EList 

_di_IXMLTEqLigTrafo3E __fastcall TXMLTEqLigTrafo3EList::Add()
{
  return (_di_IXMLTEqLigTrafo3E) AddItem(-1);
};

_di_IXMLTEqLigTrafo3E __fastcall TXMLTEqLigTrafo3EList::Insert(const int Index)
{
  return (_di_IXMLTEqLigTrafo3E) AddItem(Index);
};

_di_IXMLTEqLigTrafo3E __fastcall TXMLTEqLigTrafo3EList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigTrafo3E) List->Nodes[Index];
};

// TXMLTEqLigTrecho 

void __fastcall TXMLTEqLigTrecho::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("eqlig"), __classid(TXMLTEqpto));
  RegisterChildNode(System::UnicodeString("trcTpo"), __classid(TXMLTTrcTpo));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTEqpto __fastcall TXMLTEqLigTrecho::Get_eqlig()
{
  return (_di_IXMLTEqpto) GetChildNodes()->Nodes[System::UnicodeString("eqlig")];
};

bool __fastcall TXMLTEqLigTrecho::Get_bolTip()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolTip")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigTrecho::Set_bolTip(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolTip")]->NodeValue = Value;
};

bool __fastcall TXMLTEqLigTrecho::Get_bolEql()
{
  return GetChildNodes()->Nodes[System::UnicodeString("bolEql")]->NodeValue.operator bool();
};

void __fastcall TXMLTEqLigTrecho::Set_bolEql(bool Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("bolEql")]->NodeValue = Value;
};

double __fastcall TXMLTEqLigTrecho::Get_dblCmp()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCmp")]->NodeValue.operator double();
};

void __fastcall TXMLTEqLigTrecho::Set_dblCmp(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCmp")]->NodeValue = Value;
};

_di_IXMLTTrcTpo __fastcall TXMLTEqLigTrecho::Get_trcTpo()
{
  return (_di_IXMLTTrcTpo) GetChildNodes()->Nodes[System::UnicodeString("trcTpo")];
};

// TXMLTEqLigTrechoList 

_di_IXMLTEqLigTrecho __fastcall TXMLTEqLigTrechoList::Add()
{
  return (_di_IXMLTEqLigTrecho) AddItem(-1);
};

_di_IXMLTEqLigTrecho __fastcall TXMLTEqLigTrechoList::Insert(const int Index)
{
  return (_di_IXMLTEqLigTrecho) AddItem(Index);
};

_di_IXMLTEqLigTrecho __fastcall TXMLTEqLigTrechoList::Get_Item(int Index)
{
  return (_di_IXMLTEqLigTrecho) List->Nodes[Index];
};

// TXMLTTrcTpo 

void __fastcall TXMLTTrcTpo::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("trcTpo01"), __classid(TXMLTTrcTpo01));
  RegisterChildNode(System::UnicodeString("trcTpo02"), __classid(TXMLTTrcTpo02));
  RegisterChildNode(System::UnicodeString("trcTpo03"), __classid(TXMLTTrcTpo03));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTTrcTpo01 __fastcall TXMLTTrcTpo::Get_trcTpo01()
{
  return (_di_IXMLTTrcTpo01) GetChildNodes()->Nodes[System::UnicodeString("trcTpo01")];
};

_di_IXMLTTrcTpo02 __fastcall TXMLTTrcTpo::Get_trcTpo02()
{
  return (_di_IXMLTTrcTpo02) GetChildNodes()->Nodes[System::UnicodeString("trcTpo02")];
};

_di_IXMLTTrcTpo03 __fastcall TXMLTTrcTpo::Get_trcTpo03()
{
  return (_di_IXMLTTrcTpo03) GetChildNodes()->Nodes[System::UnicodeString("trcTpo03")];
};

System::UnicodeString __fastcall TXMLTTrcTpo::Get_strCodArr()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strCodArr")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTTrcTpo::Set_strCodArr(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strCodArr")]->NodeValue = Value;
};

// TXMLTTrcTpo01 

void __fastcall TXMLTTrcTpo01::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("caboId"), __classid(TXMLTCaboId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTCaboId __fastcall TXMLTTrcTpo01::Get_caboId()
{
  return (_di_IXMLTCaboId) GetChildNodes()->Nodes[System::UnicodeString("caboId")];
};

// TXMLTCaboId 

// TXMLTTrcTpo02 

void __fastcall TXMLTTrcTpo02::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("caboIdA"), __classid(TXMLTCaboId));
  RegisterChildNode(System::UnicodeString("caboIdB"), __classid(TXMLTCaboId));
  RegisterChildNode(System::UnicodeString("caboIdC"), __classid(TXMLTCaboId));
  RegisterChildNode(System::UnicodeString("caboIdN"), __classid(TXMLTCaboId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

System::UnicodeString __fastcall TXMLTTrcTpo02::Get_strEstGeoId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strEstGeoId")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTTrcTpo02::Set_strEstGeoId(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strEstGeoId")]->NodeValue = Value;
};

_di_IXMLTCaboId __fastcall TXMLTTrcTpo02::Get_caboIdA()
{
  return (_di_IXMLTCaboId) GetChildNodes()->Nodes[System::UnicodeString("caboIdA")];
};

_di_IXMLTCaboId __fastcall TXMLTTrcTpo02::Get_caboIdB()
{
  return (_di_IXMLTCaboId) GetChildNodes()->Nodes[System::UnicodeString("caboIdB")];
};

_di_IXMLTCaboId __fastcall TXMLTTrcTpo02::Get_caboIdC()
{
  return (_di_IXMLTCaboId) GetChildNodes()->Nodes[System::UnicodeString("caboIdC")];
};

_di_IXMLTCaboId __fastcall TXMLTTrcTpo02::Get_caboIdN()
{
  return (_di_IXMLTCaboId) GetChildNodes()->Nodes[System::UnicodeString("caboIdN")];
};

// TXMLTTrcTpo03 

void __fastcall TXMLTTrcTpo03::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("cSeq"), __classid(TXMLTCSeq));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTZseq __fastcall TXMLTTrcTpo03::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

_di_IXMLTCSeq __fastcall TXMLTTrcTpo03::Get_cSeq()
{
  return (_di_IXMLTCSeq) GetChildNodes()->Nodes[System::UnicodeString("cSeq")];
};

double __fastcall TXMLTTrcTpo03::Get_dblCorMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue.operator double();
};

void __fastcall TXMLTTrcTpo03::Set_dblCorMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue = Value;
};

// TXMLTCSeq 

double __fastcall TXMLTCSeq::Get_dblC0()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblC0")]->NodeValue.operator double();
};

void __fastcall TXMLTCSeq::Set_dblC0(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblC0")]->NodeValue = Value;
};

double __fastcall TXMLTCSeq::Get_dblC1()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblC1")]->NodeValue.operator double();
};

void __fastcall TXMLTCSeq::Set_dblC1(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblC1")]->NodeValue = Value;
};

// TXMLTMutua 

void __fastcall TXMLTMutua::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("MutuaTre"), __classid(TXMLTMutuaTre));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  CollectionCreater<_di_IXMLTMutuaTreList, TXMLTMutuaTreList, IXMLTMutuaTre>::DoCreate(this, FMutuaTre, "MutuaTre");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTMutua::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

int __fastcall TXMLTMutua::Get_intRedeId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intRedeId")]->NodeValue.operator int();
};

void __fastcall TXMLTMutua::Set_intRedeId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intRedeId")]->NodeValue = Value;
};

_di_IXMLTMutuaTreList __fastcall TXMLTMutua::Get_MutuaTre()
{
  return (_di_IXMLTMutuaTreList) FMutuaTre;
};

_di_IXMLTZseq __fastcall TXMLTMutua::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

// TXMLTMutuaList 

_di_IXMLTMutua __fastcall TXMLTMutuaList::Add()
{
  return (_di_IXMLTMutua) AddItem(-1);
};

_di_IXMLTMutua __fastcall TXMLTMutuaList::Insert(const int Index)
{
  return (_di_IXMLTMutua) AddItem(Index);
};

_di_IXMLTMutua __fastcall TXMLTMutuaList::Get_Item(int Index)
{
  return (_di_IXMLTMutua) List->Nodes[Index];
};

// TXMLTMutuaTre 

int __fastcall TXMLTMutuaTre::Get_intId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue.operator int();
};

void __fastcall TXMLTMutuaTre::Set_intId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue = Value;
};

int __fastcall TXMLTMutuaTre::Get_intSentido()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intSentido")]->NodeValue.operator int();
};

void __fastcall TXMLTMutuaTre::Set_intSentido(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intSentido")]->NodeValue = Value;
};

double __fastcall TXMLTMutuaTre::Get_dblPos()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblPos")]->NodeValue.operator double();
};

void __fastcall TXMLTMutuaTre::Set_dblPos(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblPos")]->NodeValue = Value;
};

// TXMLTMutuaTreList 

_di_IXMLTMutuaTre __fastcall TXMLTMutuaTreList::Add()
{
  return (_di_IXMLTMutuaTre) AddItem(-1);
};

_di_IXMLTMutuaTre __fastcall TXMLTMutuaTreList::Insert(const int Index)
{
  return (_di_IXMLTMutuaTre) AddItem(Index);
};

_di_IXMLTMutuaTre __fastcall TXMLTMutuaTreList::Get_Item(int Index)
{
  return (_di_IXMLTMutuaTre) List->Nodes[Index];
};

// TXMLTCaboRGMR 

void __fastcall TXMLTCaboRGMR::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTCaboRGMR::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

double __fastcall TXMLTCaboRGMR::Get_dblCorMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue.operator double();
};

void __fastcall TXMLTCaboRGMR::Set_dblCorMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue = Value;
};

double __fastcall TXMLTCaboRGMR::Get_dblR()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblR")]->NodeValue.operator double();
};

void __fastcall TXMLTCaboRGMR::Set_dblR(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblR")]->NodeValue = Value;
};

double __fastcall TXMLTCaboRGMR::Get_dblGMR()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblGMR")]->NodeValue.operator double();
};

void __fastcall TXMLTCaboRGMR::Set_dblGMR(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblGMR")]->NodeValue = Value;
};

// TXMLTCaboRGMRList 

_di_IXMLTCaboRGMR __fastcall TXMLTCaboRGMRList::Add()
{
  return (_di_IXMLTCaboRGMR) AddItem(-1);
};

_di_IXMLTCaboRGMR __fastcall TXMLTCaboRGMRList::Insert(const int Index)
{
  return (_di_IXMLTCaboRGMR) AddItem(Index);
};

_di_IXMLTCaboRGMR __fastcall TXMLTCaboRGMRList::Get_Item(int Index)
{
  return (_di_IXMLTCaboRGMR) List->Nodes[Index];
};

// TXMLTCaboZ0Z1 

void __fastcall TXMLTCaboZ0Z1::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("dblCSeq"), __classid(TXMLTCSeq));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTCaboZ0Z1::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

double __fastcall TXMLTCaboZ0Z1::Get_dblCorMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue.operator double();
};

void __fastcall TXMLTCaboZ0Z1::Set_dblCorMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue = Value;
};

_di_IXMLTZseq __fastcall TXMLTCaboZ0Z1::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

_di_IXMLTCSeq __fastcall TXMLTCaboZ0Z1::Get_dblCSeq()
{
  return (_di_IXMLTCSeq) GetChildNodes()->Nodes[System::UnicodeString("dblCSeq")];
};

// TXMLTCaboZ0Z1List 

_di_IXMLTCaboZ0Z1 __fastcall TXMLTCaboZ0Z1List::Add()
{
  return (_di_IXMLTCaboZ0Z1) AddItem(-1);
};

_di_IXMLTCaboZ0Z1 __fastcall TXMLTCaboZ0Z1List::Insert(const int Index)
{
  return (_di_IXMLTCaboZ0Z1) AddItem(Index);
};

_di_IXMLTCaboZ0Z1 __fastcall TXMLTCaboZ0Z1List::Get_Item(int Index)
{
  return (_di_IXMLTCaboZ0Z1) List->Nodes[Index];
};

// TXMLTSuporte 

void __fastcall TXMLTSuporte::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("PtoFix"), __classid(TXMLTSuportePtoFix));
  CollectionCreater<_di_IXMLTSuportePtoFixList, TXMLTSuportePtoFixList, IXMLTSuportePtoFix>::DoCreate(this, FPtoFix, "PtoFix");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTSuporte::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTSuporte::Get_strTpo()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strTpo")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTSuporte::Set_strTpo(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strTpo")]->NodeValue = Value;
};

double __fastcall TXMLTSuporte::Get_dblTnsMin()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsMin")]->NodeValue.operator double();
};

void __fastcall TXMLTSuporte::Set_dblTnsMin(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsMin")]->NodeValue = Value;
};

double __fastcall TXMLTSuporte::Get_dblTnsMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblTnsMax")]->NodeValue.operator double();
};

void __fastcall TXMLTSuporte::Set_dblTnsMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblTnsMax")]->NodeValue = Value;
};

_di_IXMLTSuportePtoFixList __fastcall TXMLTSuporte::Get_PtoFix()
{
  return (_di_IXMLTSuportePtoFixList) FPtoFix;
};

// TXMLTSuporteList 

_di_IXMLTSuporte __fastcall TXMLTSuporteList::Add()
{
  return (_di_IXMLTSuporte) AddItem(-1);
};

_di_IXMLTSuporte __fastcall TXMLTSuporteList::Insert(const int Index)
{
  return (_di_IXMLTSuporte) AddItem(Index);
};

_di_IXMLTSuporte __fastcall TXMLTSuporteList::Get_Item(int Index)
{
  return (_di_IXMLTSuporte) List->Nodes[Index];
};

// TXMLTSuportePtoFix 

void __fastcall TXMLTSuportePtoFix::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("coordPnt"), __classid(TXMLTCoord));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

int __fastcall TXMLTSuportePtoFix::Get_intId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue.operator int();
};

void __fastcall TXMLTSuportePtoFix::Set_intId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue = Value;
};

_di_IXMLTCoord __fastcall TXMLTSuportePtoFix::Get_coordPnt()
{
  return (_di_IXMLTCoord) GetChildNodes()->Nodes[System::UnicodeString("coordPnt")];
};

// TXMLTSuportePtoFixList 

_di_IXMLTSuportePtoFix __fastcall TXMLTSuportePtoFixList::Add()
{
  return (_di_IXMLTSuportePtoFix) AddItem(-1);
};

_di_IXMLTSuportePtoFix __fastcall TXMLTSuportePtoFixList::Insert(const int Index)
{
  return (_di_IXMLTSuportePtoFix) AddItem(Index);
};

_di_IXMLTSuportePtoFix __fastcall TXMLTSuportePtoFixList::Get_Item(int Index)
{
  return (_di_IXMLTSuportePtoFix) List->Nodes[Index];
};

// TXMLTArranjoZ0Z1 

void __fastcall TXMLTArranjoZ0Z1::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("zSeq"), __classid(TXMLTZseq));
  RegisterChildNode(System::UnicodeString("dblCSeq"), __classid(TXMLTCSeq));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTArranjoZ0Z1::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTArranjoZ0Z1::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTArranjoZ0Z1::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

double __fastcall TXMLTArranjoZ0Z1::Get_dblCorMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue.operator double();
};

void __fastcall TXMLTArranjoZ0Z1::Set_dblCorMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblCorMax")]->NodeValue = Value;
};

_di_IXMLTZseq __fastcall TXMLTArranjoZ0Z1::Get_zSeq()
{
  return (_di_IXMLTZseq) GetChildNodes()->Nodes[System::UnicodeString("zSeq")];
};

_di_IXMLTCSeq __fastcall TXMLTArranjoZ0Z1::Get_dblCSeq()
{
  return (_di_IXMLTCSeq) GetChildNodes()->Nodes[System::UnicodeString("dblCSeq")];
};

// TXMLTArranjoZ0Z1List 

_di_IXMLTArranjoZ0Z1 __fastcall TXMLTArranjoZ0Z1List::Add()
{
  return (_di_IXMLTArranjoZ0Z1) AddItem(-1);
};

_di_IXMLTArranjoZ0Z1 __fastcall TXMLTArranjoZ0Z1List::Insert(const int Index)
{
  return (_di_IXMLTArranjoZ0Z1) AddItem(Index);
};

_di_IXMLTArranjoZ0Z1 __fastcall TXMLTArranjoZ0Z1List::Get_Item(int Index)
{
  return (_di_IXMLTArranjoZ0Z1) List->Nodes[Index];
};

// TXMLTArranjoCaboZ0Z1 

void __fastcall TXMLTArranjoCaboZ0Z1::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTArranjoCaboZ0Z1::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTArranjoCaboZ0Z1::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTArranjoCaboZ0Z1::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

int __fastcall TXMLTArranjoCaboZ0Z1::Get_intCaboId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intCaboId")]->NodeValue.operator int();
};

void __fastcall TXMLTArranjoCaboZ0Z1::Set_intCaboId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intCaboId")]->NodeValue = Value;
};

// TXMLTArranjoCaboZ0Z1List 

_di_IXMLTArranjoCaboZ0Z1 __fastcall TXMLTArranjoCaboZ0Z1List::Add()
{
  return (_di_IXMLTArranjoCaboZ0Z1) AddItem(-1);
};

_di_IXMLTArranjoCaboZ0Z1 __fastcall TXMLTArranjoCaboZ0Z1List::Insert(const int Index)
{
  return (_di_IXMLTArranjoCaboZ0Z1) AddItem(Index);
};

_di_IXMLTArranjoCaboZ0Z1 __fastcall TXMLTArranjoCaboZ0Z1List::Get_Item(int Index)
{
  return (_di_IXMLTArranjoCaboZ0Z1) List->Nodes[Index];
};

// TXMLTArranjoCaboRGMR 

void __fastcall TXMLTArranjoCaboRGMR::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("PtoFix"), __classid(TXMLTSuportePtoFixCabo));
  CollectionCreater<_di_IXMLTSuportePtoFixCaboList, TXMLTSuportePtoFixCaboList, IXMLTSuportePtoFixCabo>::DoCreate(this, FPtoFix, "PtoFix");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTArranjoCaboRGMR::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

System::UnicodeString __fastcall TXMLTArranjoCaboRGMR::Get_strFases()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTArranjoCaboRGMR::Set_strFases(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFases")]->NodeValue = Value;
};

int __fastcall TXMLTArranjoCaboRGMR::Get_intSuporteId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intSuporteId")]->NodeValue.operator int();
};

void __fastcall TXMLTArranjoCaboRGMR::Set_intSuporteId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intSuporteId")]->NodeValue = Value;
};

_di_IXMLTSuportePtoFixCaboList __fastcall TXMLTArranjoCaboRGMR::Get_PtoFix()
{
  return (_di_IXMLTSuportePtoFixCaboList) FPtoFix;
};

// TXMLTArranjoCaboRGMRList 

_di_IXMLTArranjoCaboRGMR __fastcall TXMLTArranjoCaboRGMRList::Add()
{
  return (_di_IXMLTArranjoCaboRGMR) AddItem(-1);
};

_di_IXMLTArranjoCaboRGMR __fastcall TXMLTArranjoCaboRGMRList::Insert(const int Index)
{
  return (_di_IXMLTArranjoCaboRGMR) AddItem(Index);
};

_di_IXMLTArranjoCaboRGMR __fastcall TXMLTArranjoCaboRGMRList::Get_Item(int Index)
{
  return (_di_IXMLTArranjoCaboRGMR) List->Nodes[Index];
};

// TXMLTSuportePtoFixCabo 

int __fastcall TXMLTSuportePtoFixCabo::Get_intId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue.operator int();
};

void __fastcall TXMLTSuportePtoFixCabo::Set_intId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intId")]->NodeValue = Value;
};

System::UnicodeString __fastcall TXMLTSuportePtoFixCabo::Get_strFase()
{
  return GetChildNodes()->Nodes[System::UnicodeString("strFase")]->NodeValue.operator System::UnicodeString();
};

void __fastcall TXMLTSuportePtoFixCabo::Set_strFase(System::UnicodeString Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("strFase")]->NodeValue = Value;
};

int __fastcall TXMLTSuportePtoFixCabo::Get_intCaboId()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intCaboId")]->NodeValue.operator int();
};

void __fastcall TXMLTSuportePtoFixCabo::Set_intCaboId(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intCaboId")]->NodeValue = Value;
};

// TXMLTSuportePtoFixCaboList 

_di_IXMLTSuportePtoFixCabo __fastcall TXMLTSuportePtoFixCaboList::Add()
{
  return (_di_IXMLTSuportePtoFixCabo) AddItem(-1);
};

_di_IXMLTSuportePtoFixCabo __fastcall TXMLTSuportePtoFixCaboList::Insert(const int Index)
{
  return (_di_IXMLTSuportePtoFixCabo) AddItem(Index);
};

_di_IXMLTSuportePtoFixCabo __fastcall TXMLTSuportePtoFixCaboList::Get_Item(int Index)
{
  return (_di_IXMLTSuportePtoFixCabo) List->Nodes[Index];
};

// TXMLTCurvaTipica 

void __fastcall TXMLTCurvaTipica::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("Patamar"), __classid(TXMLTPatamTip));
  CollectionCreater<_di_IXMLTPatamTipList, TXMLTPatamTipList, IXMLTPatamTip>::DoCreate(this, FPatamar, "Patamar");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTCurvaTipica::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

int __fastcall TXMLTCurvaTipica::Get_intTpoCurva()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoCurva")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intTpoCurva(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoCurva")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intUnidade()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intUnidade")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intUnidade(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intUnidade")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intEscala()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intEscala")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intEscala(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intEscala")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intTpoCons()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intTpoCons")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intTpoCons(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intTpoCons")]->NodeValue = Value;
};

double __fastcall TXMLTCurvaTipica::Get_dblEneMin()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblEneMin")]->NodeValue.operator double();
};

void __fastcall TXMLTCurvaTipica::Set_dblEneMin(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblEneMin")]->NodeValue = Value;
};

double __fastcall TXMLTCurvaTipica::Get_dblEneMax()
{
  return GetChildNodes()->Nodes[System::UnicodeString("dblEneMax")]->NodeValue.operator double();
};

void __fastcall TXMLTCurvaTipica::Set_dblEneMax(double Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("dblEneMax")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intAno()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intAno")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intAno(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intAno")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intMes()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intMes")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intMes(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intMes")]->NodeValue = Value;
};

int __fastcall TXMLTCurvaTipica::Get_intNumPat()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intNumPat")]->NodeValue.operator int();
};

void __fastcall TXMLTCurvaTipica::Set_intNumPat(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intNumPat")]->NodeValue = Value;
};

_di_IXMLTPatamTipList __fastcall TXMLTCurvaTipica::Get_Patamar()
{
  return (_di_IXMLTPatamTipList) FPatamar;
};

// TXMLTCurvaTipicaList 

_di_IXMLTCurvaTipica __fastcall TXMLTCurvaTipicaList::Add()
{
  return (_di_IXMLTCurvaTipica) AddItem(-1);
};

_di_IXMLTCurvaTipica __fastcall TXMLTCurvaTipicaList::Insert(const int Index)
{
  return (_di_IXMLTCurvaTipica) AddItem(Index);
};

_di_IXMLTCurvaTipica __fastcall TXMLTCurvaTipicaList::Get_Item(int Index)
{
  return (_di_IXMLTCurvaTipica) List->Nodes[Index];
};

// TXMLTPatamTip 

void __fastcall TXMLTPatamTip::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("horIni"), __classid(TXMLTHorMin));
  RegisterChildNode(System::UnicodeString("horFim"), __classid(TXMLTHorMin));
  CollectionCreater<_di_IXMLdouble_List, TXMLdouble_List, Xml::Xmlintf::IXMLNode>::DoCreate(this, FdblVal, "dblVal");
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTHorMin __fastcall TXMLTPatamTip::Get_horIni()
{
  return (_di_IXMLTHorMin) GetChildNodes()->Nodes[System::UnicodeString("horIni")];
};

_di_IXMLTHorMin __fastcall TXMLTPatamTip::Get_horFim()
{
  return (_di_IXMLTHorMin) GetChildNodes()->Nodes[System::UnicodeString("horFim")];
};

int __fastcall TXMLTPatamTip::Get_intNumVal()
{
  return GetChildNodes()->Nodes[System::UnicodeString("intNumVal")]->NodeValue.operator int();
};

void __fastcall TXMLTPatamTip::Set_intNumVal(int Value)
{
  GetChildNodes()->Nodes[System::UnicodeString("intNumVal")]->NodeValue = Value;
};

_di_IXMLdouble_List __fastcall TXMLTPatamTip::Get_dblVal()
{
  return FdblVal;
};

// TXMLTPatamTipList 

_di_IXMLTPatamTip __fastcall TXMLTPatamTipList::Add()
{
  return (_di_IXMLTPatamTip) AddItem(-1);
};

_di_IXMLTPatamTip __fastcall TXMLTPatamTipList::Insert(const int Index)
{
  return (_di_IXMLTPatamTip) AddItem(Index);
};

_di_IXMLTPatamTip __fastcall TXMLTPatamTipList::Get_Item(int Index)
{
  return (_di_IXMLTPatamTip) List->Nodes[Index];
};

// TXMLTZId 

void __fastcall TXMLTZId::AfterConstruction(void)
{
  RegisterChildNode(System::UnicodeString("id"), __classid(TXMLTId));
  RegisterChildNode(System::UnicodeString("z"), __classid(TXMLTZ));
  Xml::Xmldoc::TXMLNode::AfterConstruction();
};

_di_IXMLTId __fastcall TXMLTZId::Get_id()
{
  return (_di_IXMLTId) GetChildNodes()->Nodes[System::UnicodeString("id")];
};

_di_IXMLTZ __fastcall TXMLTZId::Get_z()
{
  return (_di_IXMLTZ) GetChildNodes()->Nodes[System::UnicodeString("z")];
};

// TXMLdouble_List 

Xml::Xmlintf::_di_IXMLNode __fastcall TXMLdouble_List::Add(const double Value)
{
  Xml::Xmlintf::_di_IXMLNode item = AddItem(-1);
  item->NodeValue = Value;
  return item;
};

Xml::Xmlintf::_di_IXMLNode __fastcall TXMLdouble_List::Insert(const int Index, const double Value)
{
  Xml::Xmlintf::_di_IXMLNode item = AddItem(Index);
  item->NodeValue = Value;
  return item;
};

double __fastcall TXMLdouble_List::Get_Item(int Index)
{
  return List->Nodes[Index]->NodeValue.operator double();
};
