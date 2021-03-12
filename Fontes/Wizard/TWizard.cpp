//-----------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <algorithm>
#include "TWizard.h"
#include "..\Apl\VTApl.h"
#include "..\Busca\VTBusca.h"
#include "..\Figura\VTFiguraComentario.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Legenda\VTDado.h"
#include "..\Legenda\VTLegenda.h"
#include "..\Rede\VTArea.h"
#include "..\Rede\VTEqpto.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Legenda.h"
#include "..\..\DLL_Inc\Rede.h"
using namespace std;

//-----------------------------------------------------------------------------
VTWizard* __fastcall NewObjWizard(VTApl *apl)
   {
   try{
      return(new TWizard(apl));
      }catch(Exception &e)
         {//nada a fazer
         }
   //erro na criação do objeto
   return(NULL);
   }

//-----------------------------------------------------------------------------
__fastcall TWizard::TWizard(VTApl *apl)
   {
   //salva ponteiro p/ objetos
   this->apl = apl;
   busca     = (VTBusca*)apl->GetObject(__classid(VTBusca));
   //cria objetos
   area          = DLL_NewObjArea();
   legenda       = DLL_NewObjLegenda(apl);
   image         = new TImage(NULL);
   image->Height = Screen->Height;
   image->Width  = Screen->Width;
   //if (image->Picture->Graphic) delete image->Picture->Graphic;
   //graph_bmp = new Graphics::TBitmap();
   //graph_bmp->Height = Screen->Height;
   //graph_bmp->Width  = Screen->Width;
   //image->Picture->Graphic = graph_bmp;
   lisDADO = new TList();
   //inicia dados
   ult_eqpto = NULL;
   //assume configuração default dos Eqptos
   SetShowDefault();
   //carrega configuração
   LoadShowFromFile();
   }

//-----------------------------------------------------------------------------
__fastcall TWizard::~TWizard(void)
   {
   //salva configuração
   SaveShowToFile();
   //destrói objetos
   if (area)    {delete area;    area    = NULL;}
   if (legenda) {delete legenda; legenda = NULL;}
   //if (image)   {delete image;   image   = NULL;}
   //memória alocada p/ bitmap de Timage
   //if (graph_bmp) {delete graph_bmp; graph_bmp = NULL;}
   //destrói lista e seus objetos
   if (lisDADO) {LimpaTList(lisDADO); delete lisDADO; lisDADO = NULL;}
   }

//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
VTFiguraComentario* __fastcall TWizard::BuscaComentario(int x, int y)
   {
   //variáveis locais
   TList *lisEQP;

   //verifica se há algum equipamento na área indicada
   area->x1 = area->x2 = x;
   area->y1 = area->y2 = y;
   lisEQP = busca->FigComentario(area);
   if (lisEQP->Count == 0) return(NULL);
   //retorna primeiro Eqpto da lista
   return((VTFiguraComentario*)lisEQP->First());
   }

//-----------------------------------------------------------------------------
VTEqpto* __fastcall TWizard::BuscaEqpto(int x, int y)
   {
   //variáveis locais
   TList *lisEQP;

   //verifica se há algum equipamento na área indicada
   area->x1 = area->x2 = x;
   area->y1 = area->y2 = y;
   lisEQP = busca->Eqpto(area);
   if (lisEQP->Count == 0) return(NULL);
   //retorna primeiro Eqpto da lista
   return((VTEqpto*)lisEQP->First());
   }


//-----------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/15761201
bool __fastcall TWizard::Executa(TWinControl *wincrtl, int x, int y, TColor cor_fundo)
   {
   //variáveis locais
   VTEqpto            *eqpto      = NULL;
   VTFiguraComentario *comentario = NULL;

   //verifica se WinControl ainda não estava definido
   if (this->wincrtl != wincrtl)
      {
      this->wincrtl = wincrtl;
      image->Parent = wincrtl;
      image->Align  = alClient;
      }
   //verifica se há algum equipamento na área indicada
   if ((eqpto = BuscaEqpto(x, y)) == NULL)
      {//verifica se há algum Comentario na área indicada
      comentario = BuscaComentario(x, y);
      //proteção: comentário com estilo diferente de flag não deve ser exibido
      if ((comentario != NULL) && (comentario->Formato != fmtFLAG))
         {//cancela Comentario
         comentario = NULL;
         }
      }
   //verifica se foi selecionado um Eqpto diferente do último
   if ((eqpto != NULL) && (eqpto != ult_eqpto))
      {//determina lista de Dado do Eqpto
      MontaLisDado(eqpto);
      //preenche TImage c/ os Dados do Eqpto
      ImagePreenche(image, cor_fundo);
      }
   //verifica se foi selecionado um Comentario diferente do último
   else if ((comentario != NULL) && (comentario != ult_comentario))
	  {//preenche TImage c/ texto do Comentario
	  ImagePreenche(image, comentario->Texto());
	  }
   //verifica se deve apagar TImage
   else if ((eqpto == NULL) && (comentario == NULL))
      {//limpa TImage
      ImageLimpa(image, clBtnFace);
      }
   //salva ult_eqpto e ult_comentario
   ult_eqpto      = eqpto;
   ult_comentario = comentario;
   return((ult_eqpto != NULL) || (ult_comentario != NULL));
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::ImageLimpa(TImage *image, TColor color)
   {
   image->Canvas->Brush->Color = color;
   image->Canvas->Brush->Style = bsSolid;
   image->Canvas->FillRect(image->ClientRect);
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::ImagePreenche(TImage *image, TColor color)
   {
   //variáveis locais
   AnsiString txt;
   int        x      = 0;
   int        y      = 0;
   int        width  = 0;
   int        dy = abs(image->Canvas->Font->Height)+1;
   VTDado     *dado;

   //limpa TImage
   ImageLimpa(image, clBtnFace);
   //configura atributos de TImage
   image->Canvas->Brush->Color = clBtnFace; //clGray;
   image->Canvas->Font->Color  = clNavy;  //clWhite;
   image->Canvas->TextFlags    = ETO_OPAQUE;
   //preenche TImage c/ os Dados
   for (int n = 0; n < lisDADO->Count; n++, y+=dy)
      {
      dado  = (VTDado*)lisDADO->Items[n];
      txt   = dado->DescValorUnid();
      width = max(width, image->Canvas->TextWidth(txt));
      image->Canvas->TextOut(x+1, y, txt);
      }
   //redefine área de TWinControl
   wincrtl->Width  = width + 5;
   wincrtl->Height = ((lisDADO->Count) * dy) + 2;
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::ImagePreenche(TImage *image, TStrings *texto)
   {
   //variáveis locais
   AnsiString txt;
   int        x      = 0;
   int        y      = 0;
   int        width  = 0;
   int        dy = abs(image->Canvas->Font->Height)+1;

   //limpa TImage
   ImageLimpa(image, clBtnFace);
   //configura atributos de TImage
   image->Canvas->Brush->Color = clBtnFace; //clGray;
   image->Canvas->Font->Color  = clNavy;  //clWhite;
   image->Canvas->TextFlags    = ETO_OPAQUE;
   //preenche TImage c/ os Dados
   for (int n = 0; n < texto->Count; n++, y+=dy)
      {
      txt   = texto->Strings[n];
      width = max(width, image->Canvas->TextWidth(txt));
      image->Canvas->TextOut(x+1, y, txt);
      }
   //redefine área de TWinControl
   wincrtl->Width  = width + 5;
   wincrtl->Height = ((texto->Count) * dy) + 2;
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::MontaLisDado(VTEqpto *eqpto)
   {
   //variáveis locais
   unsigned show_orig = eqpto->ShowAsUnsigned;

   //reinicia lisDADO
   LimpaTList(lisDADO);
   //redefine seleção de atributos a serem lidos do Eqpto
   eqpto->ShowAsUnsigned = (vet_eqpto[eqpto->Tipo()].show | eqpto->ShowAsUnsigned);
   //obtém lista de Dados do Eqpto
   legenda->LisValoresEqpto(eqpto, lisDADO);
   //redefine seleção de atributos do Eqpto
   eqpto->ShowAsUnsigned = show_orig;
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::LoadShowFromFile(void)
   {
   //variáveis locais
   AnsiString tipo_eqpto;
   unsigned   show;
   VTGeral    *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   geral->Infoset->LoadFromFile(this->ClassName());
   //loop p/ todos eqptos
   for (int n = 0; n < DIM_VET_EQPTO; n++)
      {
      if (vet_eqpto[n].enabled)
         {
         tipo_eqpto = VTEqpto::TipoEqpto_AsString(n);
         if (geral->Infoset->GetInfo(tipo_eqpto, show)) vet_eqpto[n].show = show;
         }
      }
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::SaveShowToFile(void)
   {
   //variáveis locais
   AnsiString tipo_eqpto;
   VTGeral    *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //reinicia conjunto de dados a ser salvo
   geral->Infoset->Clear();
   //loop p/ todos eqptos
   for (int n = 0; n < DIM_VET_EQPTO; n++)
      {
      if (vet_eqpto[n].enabled)
         {
         tipo_eqpto = VTEqpto::TipoEqpto_AsString(n);
         geral->Infoset->AddInfo(tipo_eqpto, vet_eqpto[n].show);
         }
      }
   //grava arquivo
   geral->Infoset->SaveToFile(this->ClassName());
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::SetShowAtributo(unsigned &show_eqpto, int bit_index, bool enabled)
   {
   if (enabled)
      {show_eqpto |= bit_index;}
   else
      {show_eqpto &= ~bit_index;}
   }

//-----------------------------------------------------------------------------
void __fastcall TWizard::SetShowDefault(void)
   {
   //variáveis locais
   int  tipo[] = {eqptoARRANJO,    eqptoBARRA,      eqptoCAPACITOR,
                  eqptoCAPSERIE,   eqptoCARGA,      eqptoCHAVE,
                  eqptoGERADOR,    eqptoFILTRO,     eqptoMEDIDOR,
                  eqptoMUTUA,      eqptoREATOR,     eqptoREDE,
                  eqptoREGULADOR,  eqptoSUPRIMENTO, eqptoTRAFO,
                  eqptoTRAFO3E,    eqptoTRAFOZZ,    eqptoTRECHO,
                  eqptoYREF
                 };

   //desabilita todos os eqptos
   for (int n = 0; n < DIM_VET_EQPTO; n++)
      {
      vet_eqpto[n].enabled = false;
      vet_eqpto[n].show    = 0;
      }
   //habilita somente os eqptos que interessam
   for (int n = 0; n < (sizeof(tipo)/sizeof(int)); n++)
      {//habilita o eqpto
      vet_eqpto[tipo[n]].enabled = true;
      //desabilita todos seus atributos
      vet_eqpto[tipo[n]].show = 0;
      //habilita atributo código
      SetShowAtributo(vet_eqpto[tipo[n]].show, eqp_CODIGO, true);
      }
   //configura atributos p/ Barra
   SetShowAtributo(vet_eqpto[eqptoBARRA].show, bar_VNOM, true);
   //configura atributos p/ Carga
   SetShowAtributo(vet_eqpto[eqptoCARGA].show, car_FASES, true);
   SetShowAtributo(vet_eqpto[eqptoCARGA].show, car_CLASSE, true);
   SetShowAtributo(vet_eqpto[eqptoCARGA].show, car_MODELO, true);
   //configura atributos p/ Chave
   SetShowAtributo(vet_eqpto[eqptoCHAVE].show, lig_REDE,    true);
   SetShowAtributo(vet_eqpto[eqptoCHAVE].show, chv_ESTADO,  true);
   //configura atributos p/ Trecho
   SetShowAtributo(vet_eqpto[eqptoTRECHO].show, lig_REDE,    true);
   SetShowAtributo(vet_eqpto[eqptoTRECHO].show, tre_COMP,    true);
   SetShowAtributo(vet_eqpto[eqptoTRECHO].show, tre_ARRANJO, true);
   SetShowAtributo(vet_eqpto[eqptoTRECHO].show, tre_FASES,   true);
   //configura atributos p/ Trafo
   SetShowAtributo(vet_eqpto[eqptoTRAFO].show, lig_REDE,    true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO].show, trf_LIGACAO, true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO].show, trf_SNOM,    true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO].show, trf_VNOM,    true);
   //configura atributos p/ Trafo3E
   SetShowAtributo(vet_eqpto[eqptoTRAFO3E].show, lig_REDE,    true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO3E].show, trf3_LIGACAO, true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO3E].show, trf3_SNOM,    true);
   SetShowAtributo(vet_eqpto[eqptoTRAFO3E].show, trf3_VNOM,    true);
   //configura atributos p/ Suprimento
   SetShowAtributo(vet_eqpto[eqptoSUPRIMENTO].show, sup_TIPO, true);
   //configura atributos p/ Gerador
   SetShowAtributo(vet_eqpto[eqptoGERADOR].show, ger_TIPO, true);
   }

//---------------------------------------------------------------------------
//eof
