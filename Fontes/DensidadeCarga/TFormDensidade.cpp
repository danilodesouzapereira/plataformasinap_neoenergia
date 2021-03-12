//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormDensidade.h"
#include "..\Apl\VTApl.h"
#include "..\Cronometro\VTCronometro.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\BD\VTBD.h"
#include "..\BD\Hist\VTBDhist.h"
#include "..\BD\Rede\VTBDgeral.h"
#include "..\Classes\VTClasses.h"
#include "..\Curvas\VTCurvas.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Grafico\VTGrafico.h"
#include "..\Mapa\VTRegioes.h"
#include "..\Mercado\VTMercado.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Potencia\VTDemanda.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Radial\VTArvore.h"
#include "..\Rede\VTRedes.h"
#include "..\Rede\VTTipos.h"
#include "..\Redegraf\VTCelula.h"
#include "..\Redegraf\VTCelulas.h"
#include "..\Redegraf\VTRedegraf.h"
#include "..\..\DLL_Inc\Arranjo.h"
#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Classes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Grafico.h"
#include "..\..\DLL_Inc\Mapa.h"
#include "..\..\DLL_Inc\MapaTematico.h"
#include "..\..\DLL_Inc\Mercado.h"
#include "..\..\DLL_Inc\MontaZ.h"
#include "..\..\DLL_Inc\Potencia.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Radial.h"
#include "..\..\DLL_Inc\Rede.h"
#include "..\..\DLL_Inc\Redegraf.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormDensidade::TFormDensidade(TComponent *Owner, VTApl *apl_owner, TAction *ActionOnClose)
   : TForm(Owner)
   {
	//salva Action externa
   this->ActionOnClose = ActionOnClose;
   //CloseSNPChild = ActionOnClose;
   //cria objeto Apl pr�prio
   apl = NewObjApl(this, apl_owner);
   //cria e inicia demais objetos
	apl->Add(DLL_NewObjArvore(apl));
	apl->Add(DLL_NewObjClasses());
	apl->Add(DLL_NewObjArranjos());
	apl->Add(DLL_NewObjCurvas(apl));
	apl->Add(DLL_NewObjRegioes(apl));
	apl->Add(DLL_NewObjMercado(apl));
	apl->Add(DLL_NewObjDemanda(apl));
   apl->Add(DLL_NewObjMontaZ(apl));
   apl->Add(DLL_NewObjTipos());
	apl->Add(redes    = DLL_NewObjRedes(apl));
   apl->Add(redegraf = DLL_NewObjRedegraf(apl));
   apl->Add(celulas  = DLL_NewObjCelulas(apl));
   apl->Add(grafico  = DLL_NewObjGrafico(apl));
   apl->Add(bd       = DLL_NewObjBD(apl));
   //insere barra de progresso //DVK 2016.07.11
   apl->Add(DLL_NewObjProgresso(NULL, this));
   //cria lista p/ recber Arvores existentes na base
   lisARVORE = new TList();
   //inicia objeto b�sicos
   IniciaObjetosBasicos();
   //inicia conex�o com base de dados
   ConectaBaseHistorico();
   //desabilita mapemaneto dos Eqptos nas Celulas
   celulas->EqptoEnabled = false;
   //configura Grafico
   grafico->DefineParent(PanelGrafico);
   grafico->DefineParentToolBar(panelToolBarGrafico);
   //seleciona modo geo-referenciado
   redegraf->ModoGrafico = modoGEOREFERENCIADO;
   //inicia CBoxData com as datas dispon�veis na base de dados
   CBoxDataInicia();
   //simula sele��o de data p/ carregar e exibir o mapa tem�tico
   CBoxDataChange(NULL);
   }

//---------------------------------------------------------------------------
__fastcall TFormDensidade::~TFormDensidade(void)
   {
   //vari�veis locais
   VTArvore    *arvore;
   VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //mostra barra de progresso  //DVK 2016.07.11
   if(prog) prog->Start(progDEFAULT);
   //fecha FormMT_DC
   DLL_MapaTematico_Stop(this);
   //destr�i lista e seus obejtos
   if (lisARVORE) {LimpaTList(lisARVORE); delete lisARVORE; lisARVORE = NULL;}
   //destr�i objetos VTResumoCarga armazenados nos Nodes da Arvore
   arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));
   if (arvore) {arvore->ClearObjects();}
   //esconde progresso         //DVK 2016.07.11
   if(prog) prog->Stop();
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   //executa Action externa
   if (ActionOnClose) ActionOnClose->Execute();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::ActionFechaExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::ActionSelecionaAnoExecute(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::ActionSelBaseDadoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTPath      *path = (VTPath*)apl->GetObject(__classid(VTPath));
   VTProgresso *prog = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

   //configura OpenDialog com a base atual
   OpenDialog->InitialDir = ExtractFileDir(path->ArqHistorico);
   OpenDialog->FileName   = path->ArqHistorico;
   if (OpenDialog->Execute())
      {//verifica se foi selecionada uma nova base
      if (OpenDialog->FileName.CompareIC(path->ArqHistorico) != 0)
         {//atualiza Path
         path->ArqHistorico = OpenDialog->FileName;
         //mostra barra de progresso  //DVK 2016.07.11
         if(prog) prog->Start(progDEFAULT);
         //reinicia conex�o com base de dados de perdas
         ConectaBaseHistorico();
         //inicia CBoxData
         CBoxDataInicia();
         //simula sele��o de uma Data
         CBoxDataChange(NULL);
         //esconde progresso         //DVK 2016.07.11
         if(prog) prog->Stop();
         }
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::CBoxDataChange(TObject *Sender)
   {
   //esconde o gr�fico
   grafico->Hide();
   //fecha TFormMT_DC
   DLL_MapaTematico_Stop(this);
//   //cancela exibi��o do Grafico
//   grafico->Hide();
   //carrega a Rede da data selecionada
   ReiniciaRede();
   //iniciaMapaTematico();
   //DLL_NewFormMT_DC(this, apl, PanelGrafico, ActionOnClose);
   DLL_NewFormMT_DC(this, apl, PanelGrafico, ActionFecha); //FKM n�o passo a action de fechar o formChild, passo a de fechar esse.
   //exibe gr�fico
   grafico->MapaEnabled  = true;
   grafico->Show(grafTOPO);
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::CBoxDataInicia(void)
   {
   //varia�veis locais
   VTArvore *arvore;

   // Testes preliminares
   if (! bd)       return;
   if (! bd->Hist) return;
   //reinicia CBoxData
   CBoxData->Clear();
   //obt�m lista de �rvores da base dedados
   LimpaTList(lisARVORE);
   if (! bd->Hist->LeListaArvore(lisARVORE)) return;
   //inicia CBoxData
   for (int n = 0; n < lisARVORE->Count; n++)
      {
      arvore = (VTArvore*)lisARVORE->Items[n];
      //verifica o tipo da �rvore
      if ((arvore->TipoArvore & arvoreDENSIDADE) == arvoreDENSIDADE)
         {//insere Arvore no ComboBox
         CBoxData->Items->AddObject(DateToStr(arvore->Data), arvore);
         }
      }
   //seleciona �ltima Arvore
   if (CBoxData->Items->Count > 0) CBoxData->ItemIndex = CBoxData->Items->Count - 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::ConectaBaseHistorico(void)
   {
   //vari�veis locais
   int     intTipoBaseDado;
   VTGeral *geral = (VTGeral*) apl->GetObject(__classid(VTGeral));

   bd = (VTBD*)apl->GetObject(__classid(VTBD));
   if (!bd)
      {
      apl->Add(bd = DLL_NewObjBD(apl));
      }
   if (bd)
      {
      intTipoBaseDado = SNPDBTYPE_INDEFINIDO;
      if (geral)
        {
        intTipoBaseDado = geral->BaseDado;
        }
      bd->ConectaBaseHist(intTipoBaseDado);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //destr�i o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::IniciaObjetosBasicos(void)
   {
   //vari�veis locais
   VTBD    *bd    = (VTBD*)apl->GetObject(__classid(VTBD));
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   try{//inicia conex�o
      bd->Conecta();
      //objetos a partir da nova base de dados
      bd->Geral->LeGeral(geral);
      bd->Geral->LeTipos();
      }
   catch(Exception &e)
      {
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormDensidade::ReiniciaRede(void)
   {
   //vari�veis locais
   VTArvore *arvore_aux;
   VTArvore *arvore = (VTArvore*)apl->GetObject(__classid(VTArvore));

   //prote��o
   if (!bd)       return;
   if (!bd->Hist) return;
   //reinicia Arvore e Redes
   arvore->Clear();
   redes->Clear();
   //determina Arvore selecionada e copia seus dados
   if (CBoxData->ItemIndex >= 0)
      {//reinicia Arvore com os dados da data selecionada
      arvore_aux   = (VTArvore*)(CBoxData->Items->Objects[CBoxData->ItemIndex]);
      arvore->Id   = arvore_aux->Id;
      arvore->Data = arvore_aux->Data;
      //l� Redes e Cargas
      if (! bd->Hist->LeArvoreDensidadeCarga(arvore))
         {
         Erro("Erro na leitura das cargas");
         }
      }
   //calcula �rea de Redes
   redes->CalculaArea();
   //atualiza o gr�fico
   grafico->AreaRedeDefine();
   }

//---------------------------------------------------------------------------
//eof




