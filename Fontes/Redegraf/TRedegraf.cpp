//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include <Math.hpp>
#include "TRedegraf.h"
#include "VTCelula.h"
#include "..\Apl\VTApl.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Figura\VTLink.h"
#include "..\Figura\VTRetangulo.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Grafico\VTGrid.h"
#include "..\Rede\VTTrecho.h"
#include "..\Apl\VTApl.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTRedegraf* __fastcall NewObjRedegraf(VTApl *apl)
   {
   try{
      return(new TRedegraf(apl));
      }catch(Exception &e)
         {
         return(NULL);
         }
   }

//-----------------------------------------------------------------------------
__fastcall TRedegraf::TRedegraf(VTApl *apl)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva ponteiro p/ objetos
   this->apl = apl;
   grid      = geral->grid;
   //cria objetos
   coordenada = NewObjCoordenada();
   //inicia dados
   modo_graf    = modoESQUEMATICO;
   cor.modo_geo = clLtGray;   //clLtGray; clDkGray;  //clGray;
   cor.modo_esq = clBlack;
   //lê configuração de arquivo
   InfosetLoad();
   }

//-----------------------------------------------------------------------------
__fastcall TRedegraf::~TRedegraf(void)
   {
   //salva configuração em arquivo
   InfosetSave();
   //destrói objetos
   if (coordenada) {delete coordenada; coordenada = NULL;}
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedegraf::Area(TList *lisBAR, VTArea *area)
   {
   if (modo_graf == modoGEOREFERENCIADO) CalculaAreaGeoreferenciado(lisBAR, area);
   else                                  CalculaAreaEsquematico(lisBAR, area);
   return(true);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TRedegraf::Area(VTCelula *celula)
   {
   //retorna área da Celula
   return(celula->Area);
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TRedegraf::Area(VTRede *rede)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO) return(rede->AreaUtm_cm());
   //retorna área do diag.esquemático
   return(rede->AreaEsquematico());
   }

//-----------------------------------------------------------------------------
VTArea* __fastcall TRedegraf::Area(VTRedes *redes)
   {
   //verifica se o modoGEOREFERENCIADO está selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//ATENÇÃO truque sujo: verifica se o modoGEOREFERENCIADO é válido
      ModoGrafico = modoGEOREFERENCIADO;
      }
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO) return(redes->AreaUtm_cm());
   //retorna área do diag.esquemático
   return(redes->AreaEsquematico());
   }

//---------------------------------------------------------------------------
void __fastcall TRedegraf::CalculaAreaEsquematico(TList *lisBAR, VTArea *area)
   {
   //variáveis locais
   struct {
          int x1, y1, x2, y2;
          } area_b, area_t;
   VTBarra *pbarra;

   //inicia coordenadas c/ valor 0
   area_t.x1 = area_t.x2 = 0;
   area_t.y1 = area_t.y2 = 0;
   //verifica se rede vazia
   if (lisBAR->Count > 0)
      {//calcula área definida pelas Barras
      pbarra = (VTBarra*)lisBAR->First();
      //pbarra->AreaEsquematico(area_t.x1, area_t.y1, area_t.x2, area_t.y2);
      area_t.x1 = min(pbarra->esq.x1, pbarra->esq.x2);
      area_t.y1 = min(pbarra->esq.y1, pbarra->esq.y2);
      area_t.x2 = max(pbarra->esq.x1, pbarra->esq.x2);
      area_t.y2 = max(pbarra->esq.y1, pbarra->esq.y2);
      for (int n = 1; n < lisBAR->Count; n++)
         {//obtém área da Barra
         pbarra = (VTBarra*)lisBAR->Items[n];
         //determina área da Barra
         //pbarra->AreaEsquematico(area_b.x1, area_b.y1, area_b.x2, area_b.y2);
         area_b.x1 = min(pbarra->esq.x1, pbarra->esq.x2);
         area_b.y1 = min(pbarra->esq.y1, pbarra->esq.y2);
         area_b.x2 = max(pbarra->esq.x1, pbarra->esq.x2);
         area_b.y2 = max(pbarra->esq.y1, pbarra->esq.y2);
         //calcula área total
         area_t.x1 = min(area_t.x1, area_b.x1);
         area_t.y1 = min(area_t.y1, area_b.y1);
         area_t.x2 = max(area_t.x2, area_b.x2);
         area_t.y2 = max(area_t.y2, area_b.y2);
         }
      }
   //atualiza objeto Area
   area->x1 = area_t.x1;
   area->y1 = area_t.y1;
   area->x2 = area_t.x2;
   area->y2 = area_t.y2;
   }

//-----------------------------------------------------------------------------
void __fastcall TRedegraf::CalculaAreaGeoreferenciado(TList *lisBAR, VTArea *area)
   {
   //variáveis locais
   bool    first = true;
   int     x, y;
   VTBarra *barra;

   //assume valores inválido
   area->x1 = area->y1 = area->x2 = area->y2 = -1;
   if (lisBAR->Count == 0) return;
   //calcula área em cm
   for (int nb = 0; nb < lisBAR->Count; nb++)
      {
      barra = (VTBarra*)lisBAR->Items[nb];
      //obtém coordenadas da Barra em cm
      if (! barra->CoordenadasUtm_cm(x, y)) continue;
      if (first)
         {//inicia área c/ as coordenadas da Barra
         area->x1 = area->x2 = x;
         area->y1 = area->y2 = y;
         first        = false;
         }
      else
         {//calcula área
         area->x1 = Min(int(area->x1), x);
         area->y1 = Min(int(area->y1), y);
         area->x2 = Max(int(area->x2), x);
         area->y2 = Max(int(area->y2), y);
         }
      }
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TRedegraf::CoordFigComentario(VTFiguraComentario *comentario, double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      comentario->Retangulo->Coordenadas_utm(escala, x1, y1, x2, y2);
      }
   else
      {//obtém coordenadas esquemático
      comentario->Retangulo->Coordenadas_esq(escala, x1, y1, x2, y2);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedegraf::CoordFigLink(VTLink *link, double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      link->Coordenadas_utm(escala, x1, y1, x2, y2);
      }
   else
      {//obtém coordenadas esquemático
      link->Coordenadas_esq(escala, x1, y1, x2, y2);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
bool __fastcall TRedegraf::CoordFigRetangulo(VTRetangulo *retangulo, double escala, int &x1, int &y1, int &x2, int &y2)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      retangulo->Coordenadas_utm(escala, x1, y1, x2, y2);
      }
   else
      {//obtém coordenadas esquemático
      retangulo->Coordenadas_esq(escala, x1, y1, x2, y2);
      }
   return(true);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedegraf::InfosetLoad(void)
   {
   //variáveis locais
   int     modo, color;
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   geral->Infoset->LoadFromFile(this->ClassName());
   if (geral->Infoset->GetInfo("ModoGrafico",  modo )) modo_graf    = modo;
   if (geral->Infoset->GetInfo("ColorModoGeo", color)) cor.modo_geo = TColor(color);
   if (geral->Infoset->GetInfo("ColorModoEsq", color)) cor.modo_esq = TColor(color);
   }

//-----------------------------------------------------------------------------
void __fastcall TRedegraf::InfosetSave(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   geral->Infoset->Clear();
   geral->Infoset->AddInfo("ModoGrafico",  modo_graf);
   geral->Infoset->AddInfo("ColorModoGeo", int(cor.modo_geo));
   geral->Infoset->AddInfo("ColorModoEsq", int(cor.modo_esq));
   geral->Infoset->SaveToFile(this->ClassName());
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordEqpto(VTEqpto *eqpto)
   {
   //proteção
   if (eqpto == NULL) return(NULL);
   if (eqpto->TipoBarra()) return(PM_GetCoordLine(eqpto));
   if (eqpto->TipoEqbar()) return(PM_GetCoordLine(eqpto));
      switch(eqpto->Tipo())
      {
      case eqptoMUTUA:   return(PM_GetCoordMutua((VTMutua*)eqpto));
      case eqptoREDE:    return(PM_GetCoordRede((VTRede*)eqpto));
      //case eqptoTRECHO:  return(PM_GetCoordTrecho((VTTrecho*)eqpto));
      case eqptoTRAFO3E: return(PM_GetCoordTrafo3E((VTTrafo3E*)eqpto));
      default:           return(PM_GetCoordLine(eqpto));
      }
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordLine(VTEqpto *eqpto)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      if (eqpto->CoordenadasUtm_cm(line.x1, line.y1, line.x2, line.y2))
         {//configura objeto Coordenada
         coordenada->Line = &line;
         }
      else
         {//configura objeto Coordenada NULL
         coordenada->Line = NULL;
         }
      }
   else
      {//obtém coordenadas esq
      eqpto->CoordenadasEsquematico(line.x1, line.y1, line.x2, line.y2);
      //ajusta coordenadas p/ alinhamento de Grid
      grid->SnapToGrid(line.x1, line.y1, line.x2, line.y2);
      //configura objeto Coordenada
      coordenada->Line = &line;
      }
   return(coordenada);
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordMarca(VTMarca *marca)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      if (marca->CoordenadasUtm_cm(line.x1, line.y1, line.x2, line.y2))
         {//configura objeto Coordenada
         coordenada->Line = &line;
         }
      else
         {//configura objeto Coordenada NULL
         coordenada->Line = NULL;
         }
      }
   else
      {//obtém coordenadas esq
      marca->CoordenadasEsquematico(line.x1, line.y1, line.x2, line.y2);
      //ajusta coordenadas p/ alinhamento de Grid
      grid->SnapToGrid(line.x1, line.y1, line.x2, line.y2);
      //configura objeto Coordenada
      coordenada->Line = &line;
      }
   return(coordenada);
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordMutua(VTMutua *mutua)
   {
   return(PM_GetCoordLine(mutua));
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordRede(VTRede *rede)
   {
   //variáveis locais
   VTArea *area_rede;

   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém área geo-referenciada
      area_rede = rede->AreaUtm_cm();
      //copia coordenadas da área
      area.x1 = area_rede->x1;
      area.y1 = area_rede->y1;
      area.x2 = area_rede->x2;
      area.y2 = area_rede->y2;
      }
   else
      {//obtém área do diag.esquemático
      area_rede = rede->AreaEsquematico();
      //copia coordenadas da área
      area.x1 = area_rede->x1;
      area.y1 = area_rede->y1;
      area.x2 = area_rede->x2;
      area.y2 = area_rede->y2;
      //ajusta coordenadas p/ alinhamento de Grid
      grid->SnapToGrid(area.x1, area.y1);
      grid->SnapToGrid(area.x2, area.y2);
      }
   //configura objeto Coordenada
   coordenada->Area = &area;
   return(coordenada);
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordTrafo3E(VTTrafo3E *trafo3e)
   {
   //verifica o modo selecionado
   if (modo_graf == modoGEOREFERENCIADO)
      {//obtém coordenadas utm
      if (trafo3e->CoordenadasUtm_cm(line.xm, line.ym, line.x1, line.y1, line.x2, line.y2, line.x3, line.y3))
         {//configura objeto Coordenada
         coordenada->Line = &line;
         }
      else
         {//configura objeto Coordenada NULL
         coordenada->Line = NULL;
         }
      }
   else
      {//obtém coordenadas esq
      trafo3e->CoordenadasEsquematico(line.xm, line.ym, line.x1, line.y1, line.x2, line.y2, line.x3, line.y3);
      //ajusta coordenadas p/ alinhamento de Grid
      grid->SnapToGrid(line.xm, line.ym);
      grid->SnapToGrid(line.x1, line.y1);
      grid->SnapToGrid(line.x2, line.y2);
      grid->SnapToGrid(line.x3, line.y3);
      //configura objeto Coordenada
      coordenada->Line = &line;
      }
   return(coordenada);
   }

//-----------------------------------------------------------------------------
VTCoordenada* __fastcall TRedegraf::PM_GetCoordTrecho(VTTrecho *trecho)
   {
   return(PM_GetCoordLine(trecho));
   }

//---------------------------------------------------------------------------
TColor __fastcall TRedegraf::PM_GetCorFundo(void)
   {
   //variáveis locais
   TColor  color;

   if (modo_graf == modoGEOREFERENCIADO) color = cor.modo_geo;
   else                                  color = cor.modo_esq;
   return(color);
   }

//---------------------------------------------------------------------------
int __fastcall TRedegraf::PM_GetModoGrafico(void)
   {
   return(modo_graf);
   }
   
//---------------------------------------------------------------------------
void __fastcall TRedegraf::PM_SetCorFundo(TColor color)
   {
   if (modo_graf == modoGEOREFERENCIADO) cor.modo_geo = color;
   else                                  cor.modo_esq = color;
   }
/*
//-----------------------------------------------------------------------------
void __fastcall TRedegraf::PM_SetModoGrafico(int modo)
   {
   //variáveis locais
   VTRedes *redes     = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArea  *area_rede = redes->AreaUtm_cm();

   switch(modo)
      {
      case modoGEOREFERENCIADO: //verifica se a rede possui coordenadas utm definidas
         if ((area_rede->x1 > 0)||(area_rede->y1 > 0))
            {
            modo_graf = modo;
            }
         break;
      case modoESQUEMATICO:
      case modoESQ_FUNCIONAL:
         modo_graf = modo;
         break;
      default:
         modo_graf = modoESQUEMATICO;
         break;
      }
   }
*/
//-----------------------------------------------------------------------------
void __fastcall TRedegraf::PM_SetModoGrafico(int modo)
   {
   switch(modo)
      {
      case modoGEOREFERENCIADO: modo_graf = modo; break;
      case modoESQUEMATICO:     modo_graf = modo; break;
      case modoESQ_FUNCIONAL:   modo_graf = modo; break;
      default:                  modo_graf = modoESQUEMATICO; break;
      }
   //verifica se foi selecionado modoGEOREFERENCIADO
   if (modo_graf == modoGEOREFERENCIADO)
      {//verifica se a rede é geo-referenciadas
      if (! RedeGeorefrenciada()) modo_graf = modoESQUEMATICO;
      }
   }
//-----------------------------------------------------------------------------
bool __fastcall TRedegraf::RedeGeorefrenciada(void)
   {
   //variáveis locais
   VTRedes *redes  = (VTRedes*)apl->GetObject(__classid(VTRedes));
   VTArea  *area   = redes->AreaUtm_cm();

   if (area->x1 <= 0) return(false);
   if (area->y1 <= 0) return(false);
   if (area->x2 <= 0) return(false);
   if (area->y2 <= 0) return(false);

   return(true);
   }

//---------------------------------------------------------------------------
//eof