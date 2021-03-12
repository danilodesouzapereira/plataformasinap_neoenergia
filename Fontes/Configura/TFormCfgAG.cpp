//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgAG.h"
#include "..\DLL_Stub\Basico\Basico.h"
#include "..\DLL_Stub\Funcao\Funcao.h"
#include "..\BDado\VTBDgeral.h"
#include "..\Geral\VTGeral.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TformCfgAG::TformCfgAG(TComponent* Owner, AnsiString dirBase, AnsiString dirRede)
   : TForm(Owner)
   {

   //cria objeto BDado
   obj_locais = true;
   bdGeral = DLL_NewObjBDgeral(apl);
   //cria objeto Geral
   bdGeral->LeGeral(geral);
   //cria lista de estruturas de visualização
   listaVISU = new TList();
   //proteção
   visuRef = NULL;

   }
//---------------------------------------------------------------------------
__fastcall TformCfgAG::TformCfgAG(TComponent* Owner, VTBDgeral *bdGeral, VTGeral *geral)
   : TForm(Owner)
   {

   //marca que os objetos bdado e geral não são locais
   obj_locais = false;
   //salva ponteiros p/ objetos
   this->bdGeral = bdGeral;
   this->geral = geral;
   //cria lista de estruturas de visualização
   listaVISU = new TList();
   //proteção
   visuRef = NULL;

   }
//---------------------------------------------------------------------------
__fastcall TformCfgAG::~TformCfgAG(void)
   {

   //destrói objetos criados
   if (obj_locais)
      {
      if (geral) {delete geral; geral = NULL;}
      if (bdado) {delete bdado; bdado = NULL;}
      }
   //destrói lista e seus objetos
   if (listaVISU){LimpaTList(listaVISU); delete listaVISU; listaVISU = NULL;}

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ActionCancelaExecute(TObject *Sender)
   {

   ModalResult = mrCancel;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ActionConfirmaExecute(TObject *Sender)
   {

   //lê e valida dados editados
   if (! LeDados()) return;
   //atualiza base de dados
   bdado->GravaGeral(geral);
   //fecha o form
   ModalResult = mrOk;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ActionDefaultExecute(TObject *Sender)
   {

   //dados default
   AG.nger         = 20;
   AG.nindiv       = 30;
   AG.probMut      = 0.01;
   AG.probCruz     = 0.85;
   AG.cruzUniforme = 1;
   AG.tiposelecao  = 1;
   AG.perc_ind     = 0.4;
   AG.probCruzUnif = 0.65;
   AG.etaRank      = 1.0;
   AG.critParada   = 1;
   AG.ngerParada   = 8;
   AG.kcustoTotal  = 0.5;
   AG.kinvest      = 0.5;
   AG.ktensao      = 0.1;
   AG.kcarreg      = 0.4;
   AG.tipoComb     = 1;
   AG.gama         = 0.2;
   AG.kdiagBar     = 0.;
   AG.recondAuto   = 1;
   switch(geral->PAT.NUM_PAT)
      {
      case 1:
         AG.visu[0].exibeIndiv   = 1;
         AG.visu[0].tipoGrafico  = 1;
         AG.visu[0].npatExibe    = -1;
         break;
      case 2:
         AG.visu[0].exibeIndiv   = 1;
         AG.visu[0].tipoGrafico  = 1;
         AG.visu[0].npatExibe    = -1;
         AG.visu[1].exibeIndiv   = 1;
         AG.visu[1].tipoGrafico  = 2;
         AG.visu[1].npatExibe    = 1;
         break;
      case 3:
         AG.visu[0].exibeIndiv   = 1;
         AG.visu[0].tipoGrafico  = 1;
         AG.visu[0].npatExibe    = -1;
         AG.visu[1].exibeIndiv   = 1;
         AG.visu[1].tipoGrafico  = 2;
         AG.visu[1].npatExibe    = 2;
         AG.visu[2].exibeIndiv   = 1;
         AG.visu[2].tipoGrafico  = 2;
         AG.visu[2].npatExibe    = 3;
         break;
      case 4:
         AG.visu[0].exibeIndiv   = 1;
         AG.visu[0].tipoGrafico  = 1;
         AG.visu[0].npatExibe    = -1;
         AG.visu[1].exibeIndiv   = 1;
         AG.visu[1].tipoGrafico  = 2;
         AG.visu[1].npatExibe    = 1;
         AG.visu[2].exibeIndiv   = 1;
         AG.visu[2].tipoGrafico  = 2;
         AG.visu[2].npatExibe    = 2;
         AG.visu[3].exibeIndiv   = 1;
         AG.visu[3].tipoGrafico  = 2;
         AG.visu[3].npatExibe    = 3;
         break;
      default:
         break;
      }
   //atualiza geral
   geral->AG = AG;
   //necessário para não salvar no OnChange do combo
   visuRef = NULL;
   //atualiza tela
   ExibeDados();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::cboxVisuChange(TObject *Sender)
   {

   //variávais locais
   Visualiza *visu;
   if(visuRef)SalvaInfoVisu();
   visu = (Visualiza*)cboxVisu->Items->Objects[cboxVisu->ItemIndex];
   //preenche informações na tela
   rgroupExibeIndiv->ItemIndex = visu->exibeIndiv;
   rgroupTipoGraf->ItemIndex = visu->tipoGrafico;
   cboxPatamar->ItemIndex = visu->npatExibe;
   //atualiza ponteiro de referência
   visuRef = visu;
   //efetua consistência
   Consistencia();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ckboxCriterioParadaClick(TObject *Sender)
   {

   Consistencia();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ckboxCruzUnifClick(TObject *Sender)
   {

   Consistencia();

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::Consistencia(void)
   {

   //verifica qual tipo de cruzamento utilizar
   if(ckboxCruzUnif->Checked == true)
      {
      edtProbCruzUnif->Enabled = true;
      }
   else
      {
      edtProbCruzUnif->Enabled = false;
      }
   //verifica tipo de seleção requerido
   switch(rgroupSelecao->ItemIndex)
      {
      case agROLETA:
         edtnindivSub->Enabled = false;
         edtIndUnif->Enabled = false;
         break;
      case agTORNEIO:
         edtnindivSub->Enabled = true;
         edtIndUnif->Enabled = false;
         break;
      case agRANKING:
         edtnindivSub->Enabled = true;
         edtIndUnif->Enabled = true;
         break;
      default:
         break;
      }
   //critério de parada
   if(ckboxCriterioParada->Checked == true)edtngerParada->Enabled = true;
   else edtngerParada->Enabled = false;
   //gráfico
   if(rgroupTipoGraf->ItemIndex != 2)cboxPatamar->Enabled = false;
   else cboxPatamar->Enabled = true;
   //múltiplos objetivos
   if(rgroupTipoFObj->ItemIndex == 1)edtGama->Enabled = true;
   else edtGama->Enabled = false;

   }
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::edtngerKeyPress(TObject *Sender, char &Key)
{
switch (Key)
        {
        case 'E':
        case 'e':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '.':
        case ',':
        case VK_DECIMAL:
        case VK_RETURN:
        case VK_BACK:
        case VK_TAB:
                break;

        default:
                Key=0;
        }
}
//---------------------------------------------------------------------------
void __fastcall TformCfgAG::ExibeDados(void)
   {

   //exibe dados do planejamento
   edtnger->Text     = IntToStr(geral->AG.nger);
   edtnindiv->Text   = IntToStr(geral->AG.nindiv);
   edtprobMut->Text  = DoubleToStr("%5.2f", geral->AG.probMut);
   ckboxCruzUnif->Checked = (bool)geral->AG.cruzUniforme;
   ckboxCriterioParada->Checked = (bool)geral->AG.critParada;
   rgroupSelecao->ItemIndex = geral->AG.tiposelecao;
   edtProbCruz->Text = DoubleToStr("%5.2f", geral->AG.probCruz);
   edtProbCruzUnif->Text = DoubleToStr("%5.2f", geral->AG.probCruzUnif);
   edtnindivSub->Text = DoubleToStr("%5.2f",(geral->AG.perc_ind * 100));
   edtIndUnif->Text = DoubleToStr("%5.2f", geral->AG.etaRank);
   edtngerParada->Text = IntToStr(geral->AG.ngerParada);
   edtkCusto->Text = DoubleToStr("%5.2f", geral->AG.kcustoTotal);
   edtkInvest->Text = DoubleToStr("%5.2f", geral->AG.kinvest);
   edtkCar->Text = DoubleToStr("%5.2f", geral->AG.kcarreg);
   edtkDV->Text = DoubleToStr("%5.2f", geral->AG.ktensao);
   edtGama->Text = DoubleToStr("%5.2f", geral->AG.gama);
   rgroupTipoFObj->ItemIndex = geral->AG.tipoComb;
   TrackBarTensao->Position = int(geral->AG.kdiagBar*100);
   lbTrack->Caption = IntToStr(TrackBarTensao->Position) + "%";
   ckboxRecondut->Checked = (bool)geral->AG.recondAuto;
   //força onchange do combo para dados de visualização
   cboxVisuChange(NULL);
   //Verifica consistencia de dados
   Consistencia();

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::FormClose(TObject *Sender, TCloseAction &Action)
   {

   //destrói o form
   Action = caFree;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::FormShow(TObject *Sender)
   {

   //inicia estrutura de parâmetros
   AG = geral->AG;
   //preenche combo patamar
   PreencheComboPat();
   //preenche combo estrutura de visualização
   PreencheComboVisu();
   //apresenta dados na tela
   ExibeDados();

   }

//---------------------------------------------------------------------------
bool __fastcall TformCfgAG::LeDados(void)
   {

   double somaKs;
   somaKs = StrToDouble(edtkCusto->Text);
   somaKs += StrToDouble(edtkCar->Text);
   somaKs += StrToDouble(edtkInvest->Text);
   somaKs += StrToDouble(edtkDV->Text);
   //verifica se a soma dos Ks é maior que um
   if(somaKs > 1.0){Erro("Dados de ponderação incorretos!");return (false);}
   try
      {
      geral->AG.nger = edtnger->Text.ToInt();
      geral->AG.nindiv = edtnindiv->Text.ToInt();
      geral->AG.probMut = StrToDouble(edtprobMut->Text);
      geral->AG.probCruz = StrToDouble(edtProbCruz->Text);
      geral->AG.probCruzUnif = StrToDouble(edtProbCruzUnif->Text);
      geral->AG.perc_ind = (StrToDouble(edtnindivSub->Text))/100.;
      geral->AG.etaRank = StrToDouble(edtIndUnif->Text);
      geral->AG.ngerParada = edtngerParada->Text.ToInt();
      geral->AG.kcustoTotal = StrToDouble(edtkCusto->Text);
      geral->AG.kinvest = StrToDouble(edtkInvest->Text);
      geral->AG.kcarreg = StrToDouble(edtkCar->Text);
      geral->AG.ktensao = StrToDouble(edtkDV->Text);
      geral->AG.gama = StrToDouble(edtGama->Text);
      }
   catch (EConvertError &exception)
      {
      Erro("Planejamento: dados inválidos!");
      return(false);
      }
   //dados booleanos
   geral->AG.cruzUniforme = (int)ckboxCruzUnif->Checked;
   geral->AG.critParada = (int)ckboxCriterioParada->Checked;
   geral->AG.tiposelecao = rgroupSelecao->ItemIndex;
   geral->AG.tipoComb = rgroupTipoFObj->ItemIndex;
   geral->AG.kdiagBar = ((double)TrackBarTensao->Position)/100.;
   geral->AG.recondAuto = (int)ckboxRecondut->Checked;
   //lê dados de visualização
   LeDadosVisu();
   return(true);

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::LeDadosVisu(void)
   {

   //variávais locais
   Visualiza *visu;

   if(visuRef)SalvaInfoVisu();

   for(int nvisu = 0; nvisu < listaVISU->Count; nvisu++)
      {//copia informação para o geral
      visu = (Visualiza*)listaVISU->Items[nvisu];
      geral->AG.visu[nvisu].exibeIndiv = visu->exibeIndiv;
      geral->AG.visu[nvisu].tipoGrafico = visu->tipoGrafico;
      geral->AG.visu[nvisu].npatExibe = visu->npatExibe;
      }

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::PreencheComboPat(void)
   {

   //preenche cbox Patamar
   cboxPatamar->Clear();

   for(int npat = 0; npat < geral->PAT.NUM_PAT; npat++)
      {
      cboxPatamar->Items->Add(geral->PAT.NOME[npat]);
      }

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::PreencheComboVisu(void)
   {

   //variáveis locais
   Visualiza *visu;
   //limpa combo e lista
   cboxVisu->Clear();
   LimpaTList(listaVISU);

   //cria objetos para visualização
   for(int nvisu = 0; nvisu < MAX_PAT; nvisu++)
      {//copia informação do geral
      visu = new Visualiza();
      visu->exibeIndiv   =  geral->AG.visu[nvisu].exibeIndiv;
      visu->tipoGrafico  =  geral->AG.visu[nvisu].tipoGrafico;
      visu->npatExibe    =  geral->AG.visu[nvisu].npatExibe;
      listaVISU->Add(visu);
      cboxVisu->AddItem(("Quadrante " + IntToStr(nvisu)), visu);
      }
   //seta itemindex para primeiro componente
   cboxVisu->ItemIndex = 0;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::rgroupSelecaoClick(TObject *Sender)
   {

   Consistencia();

   }
//---------------------------------------------------------------------------

void __fastcall TformCfgAG::rgroupTipoGrafClick(TObject *Sender)
   {

   Consistencia();

   }
//---------------------------------------------------------------------------

void __fastcall TformCfgAG::rgroupTipoFObjClick(TObject *Sender)
   {

   Consistencia();

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::SalvaInfoVisu(void)
   {

   //obtém informação de visualização do último selecionado
   visuRef->exibeIndiv = rgroupExibeIndiv->ItemIndex;
   visuRef->tipoGrafico = rgroupTipoGraf->ItemIndex;
   visuRef->npatExibe = cboxPatamar->ItemIndex;

   }

//---------------------------------------------------------------------------
void __fastcall TformCfgAG::TrackBarTensaoChange(TObject *Sender)
   {

   lbTrack->Caption = IntToStr(TrackBarTensao->Position) + "%";

   }
//---------------------------------------------------------------------------

