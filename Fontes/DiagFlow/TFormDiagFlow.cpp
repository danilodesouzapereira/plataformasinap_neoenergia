//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormDiagFlow.h"
#include "..\Apl\VTApl.h"
#include "..\Consulta\VTConsulta.h"
#include "..\Flow\VTFlow.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTLigacao.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTSuprimento.h"
#include "..\Rede\VTResFlowBar.h"
#include "..\Rede\VTResFlowLig.h"
#include "..\Rede\VTResFlowSup.h"
#include "..\..\DLL_Inc\Consulta.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormEdt"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormDiagFlow::TFormDiagFlow(TForm* Owner, VTApl* apl, TWinControl *parent)
   : TFormEdt(Owner, parent)
   {
   //inicia botao organizar e variavel organizar
   butOrganizar->Caption = optRede->Caption;
   organizar = orgRede;
   //carrega valores limite de nivel
   valor_critico = 1.0;
   valor_regular = 0.90;
   valor_normal = 0.70;
   barra_critico = 0.85;
   barra_regular = 0.90;
   barra_normal = 0.95;
   //Limites
   int min = 50;
   TrackBar1->Min = min;
   TrackBar2->Min = min;
   TrackBar3->Min = min;
   TrackBar4->Min = 0;
   TrackBar5->Min = 0;
   TrackBar6->Min = 0;
   TrackBar1->SelStart = min;
   TrackBar2->SelStart = min;
   TrackBar3->SelStart = min;
   TrackBar4->SelStart = 0;
   TrackBar5->SelStart = 0;
   TrackBar6->SelStart = 0;
   TrackBar1->SelEnd = 100;
   TrackBar2->SelEnd = 100;
   TrackBar3->SelEnd = 100;
   TrackBar4->SelEnd = 100;
   TrackBar5->SelEnd = 100;
   TrackBar6->SelEnd = 100;
   TrackBar1->Position = int(100. * barra_normal);
   TrackBar2->Position = int(100. * barra_regular);
   TrackBar3->Position = int(100. * barra_critico);
   TrackBar6->Position = int(100. * valor_critico);
   TrackBar5->Position = int(100. * valor_regular);
   TrackBar4->Position = int(100. * valor_normal);
   //Criando TList
   lisEQP = new TList();
   lisDIAG = new TList();
   lisPorRede = new TList();
   lisPorPatamar = new TList();
   lisPorCarreg = new TList();
   //guarda VTApl *apl
   this->apl = apl;
   //busca objetos necessarios
   flow  = (VTFlow*)apl->GetObject(__classid(VTFlow));
   grafico   = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
   redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   patamares = (VTPatamares*)apl->GetObject(__classid(VTPatamares));
   //realiza diagnostico
   Diagnostico();
   //preenche o TreeView
   PreencheTreeView();
   //preenche TChart provisoriamente
   PreencheTChart();
   }

//---------------------------------------------------------------------------
__fastcall TFormDiagFlow::~TFormDiagFlow(void)
   {
   if (lisEQP)  {delete lisEQP ; lisEQP  = NULL;}
   if (lisPorRede)  {delete lisPorRede ; lisPorRede  = NULL;}
   if (lisPorPatamar)  {delete lisPorPatamar ; lisPorPatamar  = NULL;}
   if (lisPorCarreg)  {delete lisPorCarreg ; lisPorCarreg  = NULL;}
   if (lisDIAG) {LimpaLisDiag(lisDIAG); delete lisDIAG; lisDIAG = NULL;}
   //limpa TreeView
   LimpaTreeView();
   }

//---------------------------------------------------------------------------
int __fastcall TFormDiagFlow::AchaNivelBarra(double valor)
   {
   if(valor < barra_normal && valor >= barra_regular)
      return nivelREGULAR;
   if(valor < barra_regular && valor >= barra_critico)
      return nivelCRITICO;
   if(valor < barra_critico)
      return nivelPROIBIDO;
   return nivelNORMAL;
   }

//---------------------------------------------------------------------------
int __fastcall TFormDiagFlow::AchaNivelCarreg(double valor)
   {
   if(valor > valor_normal && valor <= valor_regular)
      return nivelREGULAR;
   if(valor > valor_regular && valor <= valor_critico)
      return nivelCRITICO;
   if(valor > valor_critico)
      return nivelPROIBIDO;
   return nivelNORMAL;
   }

//---------------------------------------------------------------------------
VTRede* __fastcall TFormDiagFlow::AchaRedeBarra(VTBarra *barra)
   {
   TList  *lisREDE = redes->LisRede();
   VTRede *rede;
   for (int r = 0 ; r < lisREDE->Count ; r++)
      {
      rede = (VTRede*)lisREDE->Items[r];
      if(rede->ExisteBarra(barra)) return rede;
      }
   return NULL;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::ActionAtualizaExecute(TObject *Sender)   {
   //realiza diagnostico
   Diagnostico();
   //preenche o TreeView
   PreencheTreeView();
   //preenche TChart
   PreencheTChart();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::ActionTChartExecute(TObject *Sender)
   {
   PreencheTChart();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::Atualiza(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::Ativo(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
bool __fastcall TFormDiagFlow::DiagBarra(VTBarra* barra)
{
   //ponteiros e variaveis locais
   bool problema = true;
   strDIAG *diag;
   TList *lisREDE;
   VTRede *rede;
   //loop para cada patamar
   for (int p = 0 ; p < patamares->NumPatamar() ; p++)
      {
      //cria novo objeto strDIAG
      diag = new strDIAG;
      //associa um eqpto ao strDIAG
      diag->eqpto = (VTEqpto*)barra;
      diag->tipo = eqptoBARRA;
      //nivel de carregamento
      diag->nivel = AchaNivelBarra(barra->resflow->Vfn_pu_min[p]);
      //acha rede
      diag->rede = NULL;
      lisREDE = redes->LisRede();
      for (int r = 0 ; r < lisREDE->Count ; r++)
         {
         rede = (VTRede*)lisREDE->Items[r];
         if (rede->ExisteBarra(barra)) diag->rede = rede;
         }
      //associa numero do patamar e node = VAZIO
      diag->patamar = p;
      diag->node = NULL;
      //acrescenta strDIAG ao lisDIAG
      lisDIAG->Add(diag);
      }
   return problema;
}

//---------------------------------------------------------------------------
bool __fastcall TFormDiagFlow::DiagLigacao(VTLigacao* ligacao)
{
   //ponteiros e variaveis locais
   bool     problema = true;
   strDIAG  *diag;
   TList    *lisREDE;
   VTRede   *rede;
   //loop para cada patamar
   for (int p = 0 ; p < patamares->NumPatamar() ; p++)
      {
      //cria novo objeto strDIAG
      diag = new strDIAG;
      //associa um eqpto ao strDIAG
      diag->eqpto = (VTEqpto*)ligacao;
      diag->tipo = diag->eqpto->Tipo();
      //nivel de carregamento
      diag->nivel = AchaNivelCarreg(ligacao->resflow->Cargto_pu[p]);
      //acha rede
      diag->rede = NULL;
      lisREDE = redes->LisRede();
      for (int r = 0 ; r < lisREDE->Count ; r++)
         {
         rede = (VTRede*)lisREDE->Items[r];
         if (rede->ExisteLigacao(ligacao)) diag->rede = rede;
         }
      //associa numero do patamar e node = VAZIO
      diag->patamar = p;
      diag->node = NULL;
      //acrescenta strDIAG ao lisDIAG
      lisDIAG->Add(diag);
      }
   return problema;
}

//---------------------------------------------------------------------------
bool __fastcall TFormDiagFlow::DiagSuprimento(VTSuprimento* suprimento)
{
   //ponteiros e variaveis locais
   bool     problema = true;
   strDIAG  *diag;
   TList    *lisREDE;
   VTRede   *rede;
   //loop para cada patamar
   for (int p = 0 ; p < patamares->NumPatamar() ; p++)
      {
      //cria novo objeto strDIAG
      diag = new strDIAG;
      //associa um eqpto ao strDIAG
      diag->eqpto = (VTEqpto*)suprimento;
      diag->tipo = eqptoSUPRIMENTO;
      //nivel de carregamento
      diag->nivel = AchaNivelCarreg(suprimento->resflow->Cargto_pu[p]);
      //acha rede
      diag->rede = NULL;
      lisREDE = redes->LisRede();
      for (int r = 0 ; r < lisREDE->Count ; r++)
         {
         rede = (VTRede*)lisREDE->Items[r];
         if (rede->ExisteBarra(suprimento->pbarra)) diag->rede = rede;
         }
      //associa numero do patamar e node = VAZIO
      diag->patamar = p;
      diag->node = NULL;
      //acrescenta strDIAG ao lisDIAG
      lisDIAG->Add(diag);
      }
   return problema;
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::Diagnostico()
   {
   //strDIAG        *diag;
   VTBarra        *barra;
   VTLigacao      *ligacao;
   VTSuprimento   *suprimento;

   //faz fluxo de potencia
   flow->CalculaPorRede(redes, 0);
   //limpa lista de strDIAG
   LimpaLisDiag(lisDIAG);
   //barras
   lisEQP->Clear();
   redes->LisBarra(lisEQP);
   for (int e = 0 ; e < lisEQP->Count ; e++)
      {
      barra = (VTBarra*)lisEQP->Items[e];
      DiagBarra(barra);
      }
   //chaves, trechos, trafos
   lisEQP->Clear();
   redes->LisLigacao(lisEQP, eqptoCHAVE);
   redes->LisLigacao(lisEQP, eqptoTRECHO);
   redes->LisLigacao(lisEQP, eqptoTRAFO);
   for (int e = 0 ; e < lisEQP->Count ; e++)
      {
      ligacao = (VTLigacao*)lisEQP->Items[e];
      DiagLigacao(ligacao);
      }
   //suprimentos
   lisEQP->Clear();
   redes->LisEqbar(lisEQP, eqptoSUPRIMENTO);
   for (int e = 0 ; e < lisEQP->Count ; e++)
      {
      suprimento = (VTSuprimento*)lisEQP->Items[e];
      DiagSuprimento(suprimento);
      }
 }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::FormShow(TObject *Sender)
   {
   //posiciona o Form sem alinhamento
   ActionAlignNoneExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::LimpaLisDiag(TList *list)
   {
   //variáveis locais
   strDIAG *diag;

   for (int i = 0 ; i < list->Count ; i++)
      {
      diag = (strDIAG*)list->Items[i];
      delete diag;
      }
   list->Clear();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::LimpaTreeView()
   {
   while (TViewDiag->Items->Count > 0)
      {
      TViewDiag->Items->Item[0]->Delete();
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::MarcaTreeView()
{
   //variáveis locais
   TTreeNode *node;
   for (int s = 0 ; s < TViewDiag->Items->Count ; s++)
      {
      node = (TTreeNode*)TViewDiag->Items->Item[s];
      node->SelectedIndex = 2;
      node->ImageIndex = 2;
      }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::Moldura(VTEqpto *eqpto)
   {
   //exibe moldura no Eqpto indicado
   grafico->Moldura(eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::optCarregClick(TObject *Sender)
   {
   butOrganizar->Caption = optCarreg->Caption;
   organizar = orgCarreg;
   PreencheTreeView();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::optPatamarClick(TObject *Sender)
   {
   butOrganizar->Caption = optPatamar->Caption;
   organizar = orgPatamar;
   PreencheTreeView();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::optRedeClick(TObject *Sender)
   {
   butOrganizar->Caption = optRede->Caption;
   organizar = orgRede;
   PreencheTreeView();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::PreenchePorCarreg()
   {
   int image;
   AnsiString texto;
   strDIAG *diag;
   TList *lisREDE = redes->LisRede();
   TList *lisPARCIAL;
   TList *lisPARCIAL2;
   TList *lisPARCIAL3;
   TTreeNode *nodeCARREG0;
   TTreeNode *nodeCARREG;
   TTreeNode *nodePATAMAR;
   TTreeNode *nodeREDE;
   TTreeNode *node;
   TTreeNode *nodeBARRA;
   TTreeNode *nodeCHAVE;
   TTreeNode *nodeSUPRIMENTO;
   TTreeNode *nodeTRECHO;
   TTreeNode *nodeTRAFO;
   VTRede *rede;
   for (int c = 0 ; c < 4 ; c++)
      {
      switch (c)
         {
         case nivelNORMAL:
            nodeCARREG0 = TViewDiag->Items->AddFirst(NULL, "Normal" );
            nodeCARREG = nodeCARREG0;
            nodeCARREG0->ImageIndex = 2;
            nodeCARREG0->SelectedIndex = 2;
            break;
         case nivelREGULAR:
            nodeCARREG = TViewDiag->Items->Add(nodeCARREG0, "Regular");
            break;
         case nivelCRITICO:
            nodeCARREG = TViewDiag->Items->Add(nodeCARREG0, "Crítico");
            break;
         case nivelPROIBIDO:
            nodeCARREG = TViewDiag->Items->Add(nodeCARREG0, "Proibido");
            break;
         }
      nodeCARREG->ImageIndex = 2;
      nodeCARREG->SelectedIndex = 2;
      lisPARCIAL = SeparaPorCarreg(lisDIAG, c);
      for (int p = 0 ; p < patamares->NumPatamar() ; p++)
         {
         nodePATAMAR = TViewDiag->Items->AddChild(nodeCARREG, patamares->Patamar[p]->Nome);
         nodePATAMAR->ImageIndex = 2;
         nodePATAMAR->SelectedIndex = 2;
         lisPARCIAL2 = SeparaPorPatamar(lisPARCIAL, p);
         for(int r = 0 ; r < lisREDE->Count ; r++)
            {
            rede = (VTRede*)lisREDE->Items[r];
            lisPARCIAL3 = SeparaPorRede(lisPARCIAL2, rede);
            nodeREDE = TViewDiag->Items->AddChildObject(nodePATAMAR, "Rede: " + rede->Codigo, rede);
            nodeREDE->ImageIndex = 2;
            nodeREDE->SelectedIndex = 2;
            nodeBARRA = TViewDiag->Items->AddChild(nodeREDE, "Barras");
            nodeBARRA->ImageIndex = 2;
            nodeBARRA->SelectedIndex = 2;
            nodeCHAVE = TViewDiag->Items->AddChild(nodeREDE, "Chaves");
            nodeCHAVE->ImageIndex = 2;
            nodeCHAVE->SelectedIndex = 2;
            nodeSUPRIMENTO = TViewDiag->Items->AddChild(nodeREDE, "Suprimentos");
            nodeSUPRIMENTO->ImageIndex = 2;
            nodeSUPRIMENTO->SelectedIndex = 2;
            nodeTRAFO = TViewDiag->Items->AddChild(nodeREDE, "Trafos");
            nodeTRAFO->ImageIndex = 2;
            nodeTRAFO->SelectedIndex = 2;
            nodeTRECHO = TViewDiag->Items->AddChild(nodeREDE, "Trechos");
            nodeTRECHO->ImageIndex = 2;
            nodeTRECHO->SelectedIndex = 2;
            for (int e = 0 ; e < lisPARCIAL3->Count ; e++)
               {
               diag = (strDIAG*)lisPARCIAL3->Items[e];
               switch(diag->tipo)
                  {
                  case eqptoBARRA:        node = nodeBARRA;       break;
                  case eqptoCHAVE:        node = nodeCHAVE;       break;
                  case eqptoSUPRIMENTO:   node = nodeSUPRIMENTO;  break;
                  case eqptoTRAFO:        node = nodeTRAFO;       break;
                  case eqptoTRECHO:       node = nodeTRECHO;      break;
                  }
               node->ImageIndex = 2;
               node->SelectedIndex = 2;
               //node = TViewDiag->Items->AddChildObject(node, "Eqpto: " + diag->eqpto->Codigo, diag);
               if (diag->eqpto->Codigo.IsEmpty()) texto.sprintf("id: %d", diag->eqpto->Id);
               else                               texto = diag->eqpto->Codigo;
               node = TViewDiag->Items->AddChildObject(node, texto, diag);
               image = diag->nivel + 2;
               node->ImageIndex = image;
               node->SelectedIndex = image;
               for (int i = 0 ; i < 4 ; i++)
                  {
                  node = node->Parent;
                  if (node->ImageIndex < image)
                     {
                     node->SelectedIndex = image;
                     node->ImageIndex = image;
                     }
                  }
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::PreenchePorPatamar()
   {
   int image;
   AnsiString texto;
   strDIAG *diag;
   TList *lisREDE = redes->LisRede();
   TList *lisPARCIAL;
   TList *lisPARCIAL2;
   TList *lisPARCIAL3;
   TTreeNode *nodePATAMAR0;
   TTreeNode *nodeREDE;
   TTreeNode *nodeCARREG;
   TTreeNode *nodePATAMAR;
   TTreeNode *node;
   TTreeNode *nodeBARRA;
   TTreeNode *nodeCHAVE;
   TTreeNode *nodeSUPRIMENTO;
   TTreeNode *nodeTRECHO;
   TTreeNode *nodeTRAFO;
   VTRede *rede;
   AnsiString nivel;
   for (int p = 0 ; p < patamares->NumPatamar() ; p++)
      {
      if (p == 0)
         {
         nodePATAMAR0 = TViewDiag->Items->AddFirst(NULL, patamares->Patamar[p]->Nome);
         nodePATAMAR = nodePATAMAR0;
         nodePATAMAR0->ImageIndex = 2;
         nodePATAMAR0->SelectedIndex = 2;
         }
      else
         {
         nodePATAMAR = TViewDiag->Items->Add(nodePATAMAR0, patamares->Patamar[p]->Nome);
         }
      nodePATAMAR->ImageIndex = 2;
      nodePATAMAR->SelectedIndex = 2;
      lisPARCIAL = SeparaPorPatamar(lisDIAG, p);
      for (int r = 0 ; r < lisREDE->Count ; r++)
         {
         rede = (VTRede*)lisREDE->Items[r];
         lisPARCIAL2 = SeparaPorRede(lisPARCIAL, rede);
         nodeREDE = TViewDiag->Items->AddChildObject(nodePATAMAR, "Rede: " + rede->Codigo, rede);
         nodeREDE->ImageIndex = 2;
         nodeREDE->SelectedIndex = 2;
         for (int c = 0 ; c < 4 ; c++)
            {
            switch (c)
               {
               case nivelNORMAL:
                  nivel = "normal";
                  break;
               case nivelREGULAR:
                  nivel = "regular";
                  break;
               case nivelCRITICO:
                  nivel = "crítico";
                  break;
               case nivelPROIBIDO:
                  nivel = "proibido";
                  break;
               }
            nodeCARREG = TViewDiag->Items->AddChild(nodeREDE, nivel);
            nodeCARREG->ImageIndex = 2;
            nodeCARREG->SelectedIndex = 2;
            nodeBARRA = TViewDiag->Items->AddChild(nodeCARREG, "Barras");
            nodeBARRA->ImageIndex = 2;
            nodeBARRA->SelectedIndex = 2;
            nodeCHAVE = TViewDiag->Items->AddChild(nodeCARREG, "Chaves");
            nodeCHAVE->ImageIndex = 2;
            nodeCHAVE->SelectedIndex = 2;
            nodeSUPRIMENTO = TViewDiag->Items->AddChild(nodeCARREG, "Suprimentos");
            nodeSUPRIMENTO->ImageIndex = 2;
            nodeSUPRIMENTO->SelectedIndex = 2;
            nodeTRAFO = TViewDiag->Items->AddChild(nodeCARREG, "Trafos");
            nodeTRAFO->ImageIndex = 2;
            nodeTRAFO->SelectedIndex = 2;
            nodeTRECHO = TViewDiag->Items->AddChild(nodeCARREG, "Trechos");
            nodeTRECHO->ImageIndex = 2;
            nodeTRECHO->SelectedIndex = 2;
            lisPARCIAL3 = SeparaPorCarreg(lisPARCIAL2, c);
            for (int e = 0 ; e < lisPARCIAL3->Count ; e++)
               {
               diag = (strDIAG*)lisPARCIAL3->Items[e];
               switch(diag->tipo)
                  {
                  case eqptoBARRA:        node = nodeBARRA;       break;
                  case eqptoCHAVE:        node = nodeCHAVE;       break;
                  case eqptoSUPRIMENTO:   node = nodeSUPRIMENTO;  break;
                  case eqptoTRAFO:        node = nodeTRAFO;       break;
                  case eqptoTRECHO:       node = nodeTRECHO;      break;
                  }
               node->ImageIndex = 2;
               node->SelectedIndex = 2;
               //node = TViewDiag->Items->AddChildObject(node, "Eqpto: " + diag->eqpto->Codigo, diag);
               if (diag->eqpto->Codigo.IsEmpty()) texto.sprintf("id: %d", diag->eqpto->Id);
               else                               texto = diag->eqpto->Codigo;
               node = TViewDiag->Items->AddChildObject(node, texto, diag);
               image = diag->nivel + 2;
               node->ImageIndex = image;
               node->SelectedIndex = image;
               for (int i = 0 ; i < 4 ; i++)
                  {
                  node = node->Parent;
                  if (node->ImageIndex < image)
                     {
                     node->SelectedIndex = image;
                     node->ImageIndex = image;
                     }
                  }
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::PreenchePorRede()
   {
   int image;
   AnsiString texto;
   strDIAG *diag;
   TList *lisREDE = redes->LisRede();
   TList *lisPARCIAL;
   TList *lisPARCIAL2;
   TTreeNode *nodeREDE0;
   TTreeNode *nodeREDE;
   TTreeNode *nodePATAMAR;
   TTreeNode *node;
   TTreeNode *nodeBARRA;
   TTreeNode *nodeCHAVE;
   TTreeNode *nodeSUPRIMENTO;
   TTreeNode *nodeTRECHO;
   TTreeNode *nodeTRAFO;
   VTRede *rede;
   AnsiString nivel;
   for (int r = 0 ; r < lisREDE->Count ; r++)
      {
      rede = (VTRede*)lisREDE->Items[r];
      if (r == 0)
         {
         nodeREDE0 = TViewDiag->Items->AddObjectFirst(NULL, "Rede: " + rede->Codigo , rede);
         nodeREDE = nodeREDE0;
         nodeREDE0->ImageIndex = 2;
         nodeREDE0->SelectedIndex = 2;
         }
      else
         {
         nodeREDE = TViewDiag->Items->AddObject(nodeREDE0, "Rede: " + rede->Codigo , rede);
         }
      nodeREDE->ImageIndex = 2;
      nodeREDE->SelectedIndex = 2;
      lisPARCIAL = SeparaPorRede(lisDIAG, rede);
      for (int p = 0 ; p < patamares->NumPatamar() ; p++)
         {
         lisPARCIAL2 = SeparaPorPatamar(lisPARCIAL, p);
         nodePATAMAR = TViewDiag->Items->AddChild(nodeREDE, patamares->Patamar[p]->Nome);
         nodePATAMAR->ImageIndex = 2;
         nodePATAMAR->SelectedIndex = 2;
         nodeBARRA = TViewDiag->Items->AddChild(nodePATAMAR, "Barras");
         nodeBARRA->ImageIndex = 2;
         nodeBARRA->SelectedIndex = 2;
         nodeCHAVE = TViewDiag->Items->AddChild(nodePATAMAR, "Chaves");
         nodeCHAVE->ImageIndex = 2;
         nodeCHAVE->SelectedIndex = 2;
         nodeSUPRIMENTO = TViewDiag->Items->AddChild(nodePATAMAR, "Suprimentos");
         nodeSUPRIMENTO->ImageIndex = 2;
         nodeSUPRIMENTO->SelectedIndex = 2;
         nodeTRAFO = TViewDiag->Items->AddChild(nodePATAMAR, "Trafos");
         nodeTRAFO->ImageIndex = 2;
         nodeTRAFO->SelectedIndex = 2;
         nodeTRECHO = TViewDiag->Items->AddChild(nodePATAMAR, "Trechos");
         nodeTRECHO->ImageIndex = 2;
         nodeTRECHO->SelectedIndex = 2;
         for (int e = 0 ; e < lisPARCIAL2->Count ; e++)
            {
            diag = (strDIAG*)lisPARCIAL2->Items[e];
            switch (diag->nivel)
               {
               case nivelNORMAL:
                  nivel = "normal";
                  break;
               case nivelREGULAR:
                  nivel = "regular";
                  break;
               case nivelCRITICO:
                  nivel = "crítico";
                  break;
               case nivelPROIBIDO:
                  nivel = "proibido";
                  break;
               }
            switch(diag->tipo)
               {
               case eqptoBARRA:        node = nodeBARRA;       break;
               case eqptoCHAVE:        node = nodeCHAVE;       break;
               case eqptoSUPRIMENTO:   node = nodeSUPRIMENTO;  break;
               case eqptoTRAFO:        node = nodeTRAFO;       break;
               case eqptoTRECHO:       node = nodeTRECHO;      break;
               }
            //node = TViewDiag->Items->AddChildObject(node, "Eqpto: " + diag->eqpto->Codigo + " - Carregamento " + nivel, diag);
            if (diag->eqpto->Codigo.IsEmpty()) texto.sprintf("id: %d", diag->eqpto->Id);
            else                               texto = diag->eqpto->Codigo;
            node = TViewDiag->Items->AddChildObject(node, texto + " - " + nivel, diag);
            image = diag->nivel + 2;
            node->ImageIndex = image;
            node->SelectedIndex = image;
            for (int i = 0 ; i < 3 ; i++)
               {
               node = node->Parent;
               if (node->ImageIndex < image)
                  {
                  node->SelectedIndex = image;
                  node->ImageIndex = image;
                  }
               }
            }
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::PreencheTChart()
   {
   //variaveis locais
   int      nivel;
   int      tipo;
   strDIAG *diag;
   TColor   cor;

   switch(RadioGroup->ItemIndex)
      {
      case 1:  tipo = eqptoBARRA;      break;
      case 2:  tipo = eqptoCHAVE;      break;
      case 3:  tipo = eqptoTRAFO;      break;
      case 4:  tipo = eqptoTRECHO;     break;
      case 5:  tipo = eqptoSUPRIMENTO; break;
      case 0:
      default: tipo = -1; break;
      }
   //cria uma barra para cada patamar com a cor apropriada
   Series1->Clear();
   //Series1->BarWidthPercent = int(10);
   //Series1->OffsetPercent = int(0);
   //define dimensões do eixo Y
   Chart->BottomAxis->Minimum   = 0.;
   Chart->BottomAxis->Maximum   =  patamares->NumPatamar();
   Chart->LeftAxis->Minimum     = 0.;
   Chart->LeftAxis->Maximum     = 100.;
   for (int np=0 ; np < patamares->NumPatamar() ; np++)
      {
      cor = clGreen;
      nivel = nivelNORMAL;
      for (int d = 0 ; d < lisDIAG->Count ; d++)
         {
         diag = (strDIAG*)lisDIAG->Items[d];
         if (((diag->tipo == tipo)||(tipo == -1)) && (diag->patamar == np))
            if(diag->nivel > nivel)
               nivel = diag->nivel;
         }
      switch(nivel)
         {
         case nivelNORMAL:    cor = clLime;     break;
         case nivelREGULAR:   cor = clGreen;    break;
         case nivelCRITICO:   cor = clYellow;   break;
         case nivelPROIBIDO:  cor = clRed;      break;
         }
      Series1->AddXY(double(np+1),  100.0, "", cor);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::PreencheTreeView()
   {
   //limpa TreeView
   LimpaTreeView();
   //depende do Organizar por...
   switch (organizar)
      {
      case orgCarreg:
         PreenchePorCarreg();
         break;
      case orgPatamar:
         PreenchePorPatamar();
         break;
      case orgRede:
         PreenchePorRede();
         break;
      }
   }

//---------------------------------------------------------------------------//---------------------------------------------------------------------------//eof
TList* __fastcall TFormDiagFlow::SeparaPorCarreg(TList *list, int nivel)
   {
   strDIAG *diag;
   lisPorCarreg->Clear();
   for (int d = 0 ; d < list->Count ; d++)
      {
      diag = (strDIAG*)list->Items[d];
      if (diag->nivel == nivel) lisPorCarreg->Add(diag);
      }
   return lisPorCarreg;
   }

//---------------------------------------------------------------------------//---------------------------------------------------------------------------//eof
TList* __fastcall TFormDiagFlow::SeparaPorPatamar(TList *list, int patamar)
   {
   strDIAG *diag;
   lisPorPatamar->Clear();
   for (int d = 0 ; d < list->Count ; d++)
      {
      diag = (strDIAG*)list->Items[d];
      if (diag->patamar == patamar) lisPorPatamar->Add(diag);
      }
   return lisPorPatamar;
   }

//---------------------------------------------------------------------------//---------------------------------------------------------------------------//eof
TList* __fastcall TFormDiagFlow::SeparaPorRede(TList *list, VTRede *rede)
   {
   strDIAG *diag;
   lisPorRede->Clear();
   for (int d = 0 ; d < list->Count ; d++)
      {
      diag = (strDIAG*)list->Items[d];
      if (diag->rede == rede) lisPorRede->Add(diag);
      }
   return lisPorRede;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar1Change(TObject *Sender)
{
   if (TrackBar1->Position < TrackBar1->SelStart)
      TrackBar1->Position = TrackBar1->SelStart;
   Edit1->Text = (AnsiString)((TrackBar1->Position)/100.0);
   TrackBar2->SelEnd = TrackBar1->Position;
   TrackBar2->SelStart = TrackBar3->Position;
   barra_normal = double(TrackBar1->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar2Change(TObject *Sender)
{
   if (TrackBar2->Position < TrackBar2->SelStart)
      TrackBar2->Position = TrackBar2->SelStart;
   if (TrackBar2->Position > TrackBar2->SelEnd)
      TrackBar2->Position = TrackBar2->SelEnd;
//   TrackBar1->SelEnd = 100;
   TrackBar1->SelStart = TrackBar2->Position;
   TrackBar3->SelEnd = TrackBar2->Position;
   Edit2->Text = (AnsiString)((TrackBar2->Position)/100.0);
   barra_regular = double(TrackBar2->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar3Change(TObject *Sender)
{
   if (TrackBar3->Position > TrackBar3->SelEnd)
      TrackBar3->Position = TrackBar3->SelEnd;
   TrackBar2->SelStart = TrackBar3->Position;
//   TrackBar3->SelStart = 0;
   Edit3->Text = (AnsiString)((TrackBar3->Position)/100.0);
   barra_critico = double(TrackBar3->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar4Change(TObject *Sender)
{
   if (TrackBar4->Position > TrackBar4->SelEnd)
      TrackBar4->Position = TrackBar4->SelEnd;
   Edit4->Text = (AnsiString)((TrackBar4->Position)/100.0);
   TrackBar5->SelStart = TrackBar4->Position;
   TrackBar5->SelEnd = TrackBar6->Position;
   valor_normal = double(TrackBar4->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar5Change(TObject *Sender)
{
   if (TrackBar5->Position < TrackBar5->SelStart)
      TrackBar5->Position = TrackBar5->SelStart;
   if (TrackBar5->Position > TrackBar5->SelEnd)
      TrackBar5->Position = TrackBar5->SelEnd;
   TrackBar4->SelStart = 0;
   TrackBar4->SelEnd = TrackBar5->Position;
   TrackBar6->SelStart = TrackBar5->Position;
   Edit5->Text = (AnsiString)((TrackBar5->Position)/100.0);
   valor_regular = double(TrackBar5->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TrackBar6Change(TObject *Sender)
{
   if (TrackBar6->Position < TrackBar6->SelStart)
      TrackBar6->Position = TrackBar6->SelStart;
   TrackBar5->SelEnd = TrackBar6->Position;
   TrackBar6->SelEnd = 100;
   Edit6->Text = (AnsiString)((TrackBar6->Position)/100.0);
   valor_critico = double(TrackBar6->Position/100.0);
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TViewDiagClick(TObject *Sender)
   {
   //variáveis locais
   strDIAG   *diag;
   TTreeNode *node;
   TTreeNode *nodemoldura;
   //VTEqpto   *eqpto;

   //verifica se há um Eqpto selecionado
   if (TViewDiag->Selected == NULL) return;
   nodemoldura = TViewDiag->Selected;
   node = nodemoldura;
   //verifica se eh no de eqpto
   int numparent = 4;
   if (organizar == orgRede) numparent = 3;
   for (int i = 0 ; i < numparent ; i++)
      {
      node = node->Parent;
      if (node == NULL) return;
      }
   diag = (strDIAG*)nodemoldura->Data;
   //exibe moldura
   Moldura(diag->eqpto);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDiagFlow::TViewDiagDblClick(TObject *Sender)
   {
   //variáveis locais
   strDIAG   *diag;
   TTreeNode *node;
   TTreeNode *nodemoldura;
   //VTEqpto   *eqpto;

   //verifica se há um Eqpto selecionado
   if (TViewDiag->Selected == NULL) return;
   nodemoldura = TViewDiag->Selected;
   node        = nodemoldura;
   //verifica se eh no de eqpto
   int numparent = 4;
   if (organizar == orgRede) numparent = 3;
   for (int i = 0 ; i < numparent ; i++)
      {
      node = node->Parent;
      if (node == NULL) return;
      }
   if ((diag = (strDIAG*)nodemoldura->Data) == NULL) return;
   //seleciona zoom no eqpto
   grafico->SelecionaZoom(diag->eqpto);
   }

//---------------------------------------------------------------------------
//eof

