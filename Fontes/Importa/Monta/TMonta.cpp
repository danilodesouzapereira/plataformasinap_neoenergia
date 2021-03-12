//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "TMonta.h"

//---------------------------------------------------------------------------
VTMonta* NewObjMonta(VTApl *apl)
   {
   return(new TMonta(apl));
   }

//---------------------------------------------------------------------------
__fastcall TMonta::TMonta(VTApl *apl)
   {
   //salva ponteiros p/ objetos
   this->apl = apl;
   //cria objetos
   monta.estudo = NewObjMontaEstudo(apl);
   monta.rede   = NewObjMontaRede(apl);
   }

//---------------------------------------------------------------------------
__fastcall TMonta::~TMonta(void)
   {
   //destrói  objetos
   if (monta.estudo) {delete monta.estudo; monta.estudo = NULL;}
   if (monta.rede)   {delete monta.rede;   monta.rede = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TMonta::DefineBarrraInicial(AnsiString rede_cod, int barra_id)
   {
   return(monta.rede->DefineBarrraInicial(rede_cod, barra_id));
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TMonta::ExisteBarra(int barra_id)
   {
   return(monta.rede->ExisteBarra(barra_id));
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TMonta::ExisteBarra(AnsiString codigo)
   {
   return(monta.rede->ExisteBarra(codigo));
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TMonta::ExisteBarraCoordUtm(int utm_x, int utm_y)
   {
   return(monta.rede->ExisteBarraCoordUtm(utm_x, utm_y));
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TMonta::ExisteBarraCoordUtm(int utm_x, int utm_y, double vnom_kv)
   {
   return(monta.rede->ExisteBarraCoordUtm(utm_x, utm_y, vnom_kv));
   }

//-----------------------------------------------------------------------------
VTBarra* __fastcall TMonta::ExisteBarraCoordEsq(int esq_x1, int esq_y1, int esq_x2, int esq_y2)
   {
   return(monta.rede->ExisteBarraCoordEsq(esq_x1, esq_y1, esq_x2, esq_y2));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::ExisteEqpto(int eqpto_tipo, AnsiString codigo)
   {
   return(monta.rede->ExisteEqpto(eqpto_tipo, codigo));
   }

//-----------------------------------------------------------------------------
int __fastcall TMonta::ExisteFase(AnsiString codigo)
   {
   return(monta.rede->ExisteFase(codigo));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::ExisteRede(AnsiString codigo)
   {
   return(monta.rede->ExisteRede(codigo));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::ExisteTrecho(int utm_x1, int utm_y1, int utm_x2, int utm_y2)
   {
   return(monta.rede->ExisteTrecho(utm_x1, utm_y1, utm_x2, utm_y2));
   }

//-----------------------------------------------------------------------------
VTArranjo* __fastcall TMonta::InsereArranjo(strARRANJO2 &str)
   {
   return(monta.rede->InsereArranjo(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereBarraEqpto(strBARRA &str)
   {
   if (str.obra) return(monta.estudo->InsereBarraEqpto(str));
   else          return(monta.rede->InsereBarraEqpto(str));
   }

//-----------------------------------------------------------------------------
void __fastcall TMonta::InsereBarraStart(void)
   {
   monta.estudo->InsereBarraStart();
   monta.rede->InsereBarraStart();
   }

//-----------------------------------------------------------------------------
void __fastcall TMonta::InsereBarraStop(void)
   {
   monta.estudo->InsereBarraStop();
   monta.rede->InsereBarraStop();
   }

 //-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereBateria(strBATERIA &str)
   {
   return(monta.rede->InsereBateria(str));
   }

 //-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCabo(strCABO &str)
   {
   return(monta.rede->InsereCabo(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCapacitor(strCAPACITOR &str)
   {
   if (str.obra) return(monta.estudo->InsereCapacitor(str));
   else          return(monta.rede->InsereCapacitor(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCapserie(strCAPSERIE &str)
   {
   if (str.obra) return(monta.estudo->InsereCapserie(str));
   else          return(monta.rede->InsereCapserie(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCarga(strCARGA &str)
   {
   if (str.obra) return(monta.estudo->InsereCarga(str));
   else          return(monta.rede->InsereCarga(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCargaNL(strCARGANL &str)
   {
   if (str.obra) return(monta.estudo->InsereCargaNL(str));
   else          return(monta.rede->InsereCargaNL(str));
   }
//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereChave(strCHAVE &str)
   {
   if (str.obra) return(monta.estudo->InsereChave(str));
   else          return(monta.rede->InsereChave(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCluster(strCLUSTER &str)
   {
   if (str.obra) return(monta.estudo->InsereCluster(str));
   else          return(monta.rede->InsereCluster(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereCorte(int corte_id, int mrede_id)
   {
   //return(monta.estudo->InsereCorte(corte_id, mrede_id));
   return(monta.rede->InsereCorte(corte_id, mrede_id));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaDemanda(VTCarga *carga, strCURVA &str)
   {
   return(monta.rede->InsereCurvaDemanda(carga, str));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaTipicaCapacitor(strCURVA_TIPICA &str)
   {
   return(monta.rede->InsereCurvaTipicaCapacitor(str));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaTipicaCarga(strCURVA_TIPICA &str)
   {
   return(monta.rede->InsereCurvaTipicaCarga(str));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaTipicaGerador(strCURVA_TIPICA &str)
   {
   return(monta.rede->InsereCurvaTipicaGerador(str));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaTipicaIP(strCURVA_TIPICA &str)
   {
   return(monta.rede->InsereCurvaTipicaIP(str));
   }

//-----------------------------------------------------------------------------
VTCurva* __fastcall  TMonta::InsereCurvaTipicaReator(strCURVA_TIPICA &str)
   {
   return(monta.rede->InsereCurvaTipicaReator(str));
   }

//-----------------------------------------------------------------------------
VTDominio* __fastcall TMonta::InsereDominio(strDOMINIO &str)
   {
   return(monta.rede->InsereDominio(str));
   }

//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereEstudo(strPLAN_ACAO &str)
   {
   return(monta.estudo->InsereEstudo(str));
   }

//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereEstudo(strPLAN_ALTERNATIVA &str)
   {
   return(monta.estudo->InsereEstudo(str));
   }

//-----------------------------------------------------------------------------
VTEstudo* __fastcall TMonta::InsereEstudo(strPLAN_ESTUDO &str)
   {
   return(monta.estudo->InsereEstudo(str));
   }

//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereEstudo(strPLAN_IPE &str)
   {
   return(monta.estudo->InsereEstudo(str));
   }

//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereEstudo(strPLAN_OBRA &str)
   {
   return(monta.estudo->InsereEstudo(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereFiltro(strFILTRO &str)
   {
   if (str.obra) return(monta.estudo->InsereFiltro(str));
   else          return(monta.rede->InsereFiltro(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereGerador(strGERADOR &str)
   {
   if (str.obra) return(monta.estudo->InsereGerador(str));
   else          return(monta.rede->InsereGerador(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereMedidor(strMEDIDOR &str)
   {
   if (str.obra) return(monta.estudo->InsereMedidor(str));
   else          return(monta.rede->InsereMedidor(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereMedidorCanal(strCANAL &str)
   {
   if (str.obra) return(monta.estudo->InsereMedidorCanal(str));
   else          return(monta.rede->InsereMedidorCanal(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereMutua(strMUTUA &str)
   {
   if (str.obra) return(monta.estudo->InsereMutua(str));
   else          return(monta.rede->InsereMutua(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereReator(strREATOR &str)
   {
   if (str.obra) return(monta.estudo->InsereReator(str));
   else          return(monta.rede->InsereReator(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereRede(strREDE &str)
   {
   if (str.obra) return(monta.estudo->InsereRede(str));
   else          return(monta.rede->InsereRede(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereReducaoCarga(int mrede_id, strCARGA &str)
   {
   if (str.obra) return(monta.estudo->InsereReducaoCarga(mrede_id, str));
   else          return(monta.rede->InsereReducaoCarga(mrede_id, str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereReducaoSuprimento(int mrede_id, strSUPRIMENTO &str)
   {
   if (str.obra) return(monta.estudo->InsereReducaoSuprimento(mrede_id, str));
   else          return(monta.rede->InsereReducaoSuprimento(mrede_id, str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereReducaoTrafo(int mrede_id, strTRAFO &str)
   {
   if (str.obra) return(monta.estudo->InsereReducaoTrafo(mrede_id, str));
   else          return(monta.rede->InsereReducaoTrafo(mrede_id, str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereReducaoYref(int mrede_id, strYREF &str)
   {
   if (str.obra) return(monta.estudo->InsereReducaoYref(mrede_id, str));
   else          return(monta.rede->InsereReducaoYref(mrede_id, str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereRegulador(strREGULADOR &str)
   {
   if (str.obra) return(monta.estudo->InsereRegulador(str));
   else          return(monta.rede->InsereRegulador(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereSuporte(strSUPORTE &str)
   {
   return(monta.rede->InsereSuporte(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereSuprimento(strSUPRIMENTO &str)
   {
   if (str.obra) return(monta.estudo->InsereSuprimento(str));
   else          return(monta.rede->InsereSuprimento(str));
   }
/*
//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereTipoChave(strTIPO_EQPTO &str)
   {
   return(monta.rede->InsereTipoChave(str));
   }
*/
//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereTipoChave(strTIPO_CHAVE &str)
   {
   return(monta.rede->InsereTipoChave(str));
   }
/*
//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereTipoRede(strTIPO_EQPTO &str)
   {
   return(monta.rede->InsereTipoRede(str));
   }
*/
//-----------------------------------------------------------------------------
bool __fastcall TMonta::InsereTipoRede(strTIPO_REDE &str)
   {
   return(monta.rede->InsereTipoRede(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrafo(strTRAFO &str)
   {
   if (str.obra) return(monta.estudo->InsereTrafo(str));
   else          return(monta.rede->InsereTrafo(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrafo(strTRAFO &str, VTBarra* barra1, VTBarra *barra2)
   {
   if (str.obra) return(monta.estudo->InsereTrafo(str, barra1, barra2));
   else          return(monta.rede->InsereTrafo(str, barra1, barra2));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrafo3E(strTRAFO3E &str)
   {
   if (str.obra) return(monta.estudo->InsereTrafo3E(str));
   else          return(monta.rede->InsereTrafo3E(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrafoZZ(strTRAFOZZ &str)
   {
   if (str.obra) return(monta.estudo->InsereTrafoZZ(str));
   else          return(monta.rede->InsereTrafoZZ(str));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrecho(strTRECHO &str, strARRANJO1 &str_arr)
   {
   if (str.obra) return(monta.estudo->InsereTrecho(str, str_arr));
   else          return(monta.rede->InsereTrecho(str, str_arr));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrecho(strTRECHO &str, strARRANJO2 &str_arr)
   {
   if (str.obra) return(monta.estudo->InsereTrecho(str, str_arr));
   else          return(monta.rede->InsereTrecho(str, str_arr));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrecho(strTRECHO &str, VTArranjo *arranjo)
   {
   if (str.obra) return(monta.estudo->InsereTrecho(str, arranjo));
   else          return(monta.rede->InsereTrecho(str, arranjo));
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TMonta::InsereTrecho(strTRECHO &str, VTBarra* barra1, VTBarra *barra2, VTArranjo *arranjo)
   {
   if (str.obra) return(monta.estudo->InsereTrecho(str, barra1, barra2, arranjo));
   else          return(monta.rede->InsereTrecho(str, barra1, barra2, arranjo));
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::RedefineId(void)
   {
   monta.estudo->RedefineId();
   monta.rede->RedefineId();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::RedefineVnomBarra(void)
   {
   monta.estudo->RedefineVnomBarra();
   monta.rede->RedefineVnomBarra();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::RetiraBarraIsolada(void)
   {
   monta.estudo->RetiraBarraIsolada();
   monta.rede->RetiraBarraIsolada();
   }

//---------------------------------------------------------------------------
void __fastcall TMonta::RetiraRede(VTRede *rede)
   {
   monta.estudo->RetiraRede(rede);
   monta.rede->RetiraRede(rede);
   }

/*
//---------------------------------------------------------------------------
bool __fastcall TMonta::TransformaIlhasEmRedes(void)
   {
   //monta.estudo->TransformaIlhasEmRedes();
   return(monta.rede->TransformaIlhasEmRedes());
   }
*/
//---------------------------------------------------------------------------
bool __fastcall TMonta::TrataFimDeDados(bool define_vnom)
   {
   if (! monta.estudo->TrataFimDeDados(define_vnom)) return(false);
   if (! monta.rede->TrataFimDeDados(define_vnom))   return(false);
   return(true);
   }

//-----------------------------------------------------------------------------
// eof

