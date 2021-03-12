//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormMedicao.h"
#include "TFormMedicaoDetalhe.h"
#include "TFormEdtMedicao.h"
#include "AjustaEnergia\VTAjustaEnergia.h"
#include "Medicao\TMedicao.h"
#include "Medicao\VTMedicoes.h"
#include "..\Apl\VTApl.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\Geral\VTInfoset.h"
#include "..\Patamar\VTPatamares.h"
#include "..\Patamar\VTPatamar.h"
#include "..\Progresso\VTProgresso.h"
#include "..\Rede\VTRede.h"
#include "..\Report\VTReport.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\Progresso.h"
#include "..\..\DLL_Inc\Report.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
using namespace std;
                    
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17993063
__fastcall TFormMedicao::TFormMedicao(TComponent* Owner, VTApl *apl_owner)
                         : TForm(Owner)
{
    //salva ponteiro para objetos
    this->apl = apl_owner;
    //inicia controle da ordenação dos LView
    LViewSort.column     = LViewMedicao->Column[0];
    LViewSort.ascendente = true;
    //inicia LView //DVK 2016.01.18
    LViewMedicaoInicia();
    VTMedicoes *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
    //sinaliza que ainda não houve alteração nas medições //DVK 2016.07.28
    medicoes->Alterado = false;
    // desabilita o botão de exportação (antes de qualquer importação)
    ButMedicaoExporta->Enabled = false;
}

//---------------------------------------------------------------------------
__fastcall TFormMedicao::~TFormMedicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   //salva parâmetros do Form
   if (geral) geral->Infoset->SaveToFile(this);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionCriaMedicaoTesteExecute(TObject *Sender)
   {
   //variáveis locais
   VTAjustaEnergia *ajusta  = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));

   //TESTES
   Aviso("As medições estão sendo criadas para teste");
   ajusta->TESTE_CriaMedicoes(DaysInMonth(Date));
   //atualiza LView
   LViewMedicaoInicia();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionFecharExecute(TObject *Sender)
   {
   Close();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionMedicaoEditaExecute(TObject *Sender)
   {
   //variáveis locais
   TMedicao        *medicao;
   TFormEdtMedicao *FormEdtMedicao;
   VTAjustaEnergia *ajusta  = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

   //verifica se há uma Medicao selecionad
   if (LViewMedicao->Selected == NULL)
      {
      Aviso("Selecione a Medicao a ser editada");
      return;
      }
   medicao = (TMedicao*)LViewMedicao->Selected->Data;
   //cria TFormMedicao e exibe como janela modal
   FormEdtMedicao = new TFormEdtMedicao(this, apl, medicao);
   if (FormEdtMedicao->ShowModal() == mrOk)
      {//inicia Redes e Cargas associadas à Medicao
	  //ajusta->IniciaAllMedicao(DaysInMonth(Date));
	  //sinaliza que houve alteração nas medições //DVK 2016.07.28
      medicoes->Alterado = true;
      //atualiza LViewMedicao
      LViewMedicaoInicia();
      }
   //destrói FormMedicao
   delete FormEdtMedicao;
   }

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17993063
void __fastcall TFormMedicao::ActionMedicaoExportaExecute(TObject *Sender)
{
    VTPath      *path  = (VTPath*)apl->GetObject(__classid(VTPath));
    VTReport    *report;

    // verifica se há dados a exportar
    if(LViewMedicao->Items->Count <= 0)
    {
        Aviso("Não há dados a exportar");
        return;
    }

    // define diretório de exportação
    if (SaveDialog->InitialDir.IsEmpty())
        SaveDialog->InitialDir = path->DirExporta();

    // define nome do arquivo de exportação
    SaveDialog->FileName = "RelPerdaReg.csv";

    // seleciona o tipo de arquivo default (csv)
    SaveDialog->FilterIndex = 1;

    // propõe ao usuário o diálogo de seleção do arquivo de exportação
    if (SaveDialog->Execute())
    {
        // verifica se o arquivo existe para confirmar a substituição
        if (FileExists(SaveDialog->FileName))
        {
            if (Confirma("O arquivo de relatório já existe.", 
                         "Deseja substituir o arquivo?") != IDYES)
                return;
        }

        // cria objeto VTReport
        report = DLL_NewObjReport(apl);
        if(report)
        {
            // exporta o viewport
            report->ArquivoTexto(SaveDialog->FileName,
                                 LViewMedicao,
                                 this->Caption,
                                 false);

            Aviso("Relatório salvo com sucesso");

            // destrói objeto VTReport
            delete report;
        }
    }
}

//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17993063
void __fastcall TFormMedicao::ActionMedicaoImportaExecute(TObject *Sender)
{
    //variáveis locais
    VTAjustaEnergia *ajusta     = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
    VTMedicoes      *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
    VTProgresso     *prog       = (VTProgresso*)apl->GetObject(__classid(VTProgresso));

    if (prog == NULL)
         apl->Add(prog = DLL_NewObjProgresso(NULL,NULL));

    //importa medição de arquivos
    if (LeArquivoMedicao())
    {
        if (prog)
            prog->Start(progDEFAULT);
    
        //inicia dados das Medicoes que dependem da data (dias do mês) e da rede
        ajusta->IniciaAllMedicao(DaysInMonth(Date),false);
    
        //atualiza LView
        LViewMedicaoInicia();
        medicoes->Alterado = true;
        if (prog)
            prog->Stop();
    
        // habilista o botão de exportação ao final da importação
        ButMedicaoExporta->Enabled = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionMedicaoInsereExecute(TObject *Sender)
   {
   //variáveis locais
   TMedicao        *medicao;
   TFormEdtMedicao *FormEdtMedicao;
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

   //cria uma nova Medicao
   medicao = medicoes->InsereMedicao(NULL);
   //cria TFormMedicao e exibe como janela modal
   FormEdtMedicao = new TFormEdtMedicao(this, apl, medicao);
   if (FormEdtMedicao->ShowModal() == mrOk)
      {//inicia Redes e Cargas associadas à Medicao
      //ajusta->IniciaAllMedicao(DaysInMonth(Date));
      //atualiza LViewMedicao
      LViewMedicaoInicia();
      //sinaliza que houve alteração nas medições //DVK 2016.07.28
      medicoes->Alterado = true;
      }
   else
      {//elimina Medicao criada
      medicoes->RetiraMedicao(medicao);
      }
   //destrói FormMedicao
   delete FormEdtMedicao;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionMedicaoRetiraExecute(TObject *Sender)
   {
   //variáveis locais
   TMedicao        *medicao;
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));

   //verifica se há uma Medicao selecionada
   if (LViewMedicao->Selected == NULL)
      {
	  Aviso("Selecione a Medicao a ser retirada");
	  return;
      }
   medicao = (TMedicao*)LViewMedicao->Selected->Data;
   //elimina Medicao criada
   medicoes->RetiraMedicao(medicao);
   //inicia Redes e Cargas associadas às Medicoes //DVK 2016.07.27 comentado
//   ajusta->IniciaAllMedicao(DaysInMonth(Date));
   //atualiza LViewMedicao
   LViewMedicaoInicia();
   //sinaliza que houve alteração nas medições //DVK 2016.07.28
   medicoes->Alterado = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::ActionPropCargaExecute(TObject *Sender)
   {//DVK 2016.04.13
   //variáveis locais
   TMedicao        *medicao;
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList           *lisMED   = medicoes->LisMedicao();

   //loop para todas Medicao
   for (int n = 0; n < lisMED->Count; n++)
      {//divide a perda proporcionalmente às cargas de cada segmento
      medicao = (TMedicao*)lisMED->Items[n];
	  ajusta->IniciaDivisaoPerdaComercial(medicao);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::FormClose(TObject *Sender, TCloseAction &Action)
   {
   VTAjustaEnergia *ajusta   = (VTAjustaEnergia*)apl->GetObject(__classid(VTAjustaEnergia));

   ajusta->RemoveMedicaoInvalida();
   //destói o Form
   Action = caFree;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::FormIniciaPosicao(void)
   {
   //variáveis locais
   VTGeral *geral = (VTGeral*)apl->GetObject(__classid(VTGeral));

   if (geral)
      {//lê parâmetros do Form
      if (geral->Infoset->LoadFromFile(this)) return;
      }
   //posiciona o Form canto superior esquerdo
   Top  = 0;
   Left = 0;;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::FormShow(TObject *Sender)
   {
   //posiciona o Form
   FormIniciaPosicao();
   //desabilita tratamento deste evento novamente
   OnShow = NULL;
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMedicao::LeArquivoMedicao(void)
   {
   //variáveis locais
   VTMedicoes      *medicoes = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   VTPath          *path     = (VTPath*)apl->GetObject(__classid(VTPath));

   //define diretório inicial
   if (OpenTextFileDialog->InitialDir.IsEmpty())
      {
      OpenTextFileDialog->InitialDir = path->DirImporta();
      }
   //exibe OpenTextFileDialog
   if (! OpenTextFileDialog->Execute()) return(false);
   //faz a leitura dos arquivos selecionados
   if (! medicoes->ImportaMedicao(OpenTextFileDialog->Files)) return(false);
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::LViewMedicaoColumnClick(TObject *Sender, TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (LViewSort.column == Column)
      {//inverte ordem
      LViewSort.ascendente = ! LViewSort.ascendente;
      }
   else
      {//redefine índice da coluna usada p/ ordenação
      LViewSort.column     = Column;
      LViewSort.ascendente = true;
      }
   //força reordenação
   LViewMedicao->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::LViewMedicaoCompare(TObject *Sender, TListItem *Item1,
                                                   TListItem *Item2, int Data, int &Compare)
   {
   //variáveis locais
   int       index = LViewSort.column->Index - 1;
   double    valor1, valor2;

   //verifica Coluna sendo ordenada
   if (LViewSort.column->Index == 0)
      {//compara Caption
      Compare = CompareText(Item1->Caption,Item2->Caption);
      }
   //todas as demais colunas são numéricas
   else
      {//comparação de colunas com valor numérico
      valor1  = StrToDouble(Item1->SubItems->Strings[index]);
      valor2  = StrToDouble(Item2->SubItems->Strings[index]);
      if      (valor1 < valor2) Compare = -1;
      else if (valor1 > valor2) Compare =  1;
      else                      Compare =  0;
      }
   //verifica se ordem ascendente/descendente
   if (! LViewSort.ascendente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormMedicao::LViewMedicaoInicia(void)
   {
   //variáveis locais
   TListItem  *Item;
   TMedicao   *medicao;
   VTMedicoes *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList      *lisMEDICAO = medicoes->LisMedicao();

   //reinicia LViewMedicao
   LViewMedicao->Clear();
   LViewMedicao->Items->BeginUpdate();
   //atualiza LViewMedicao
   for (int n = 0; n < lisMEDICAO->Count; n++)
      {
      medicao       = (TMedicao*)lisMEDICAO->Items[n];
      Item          = LViewMedicao->Items->Add();
      Item->Data    = medicao;
      Item->Caption = medicao->rede->Codigo;
      Item->SubItems->Add(DoubleToStr("%4.3f", medicao->energia.medida_mwhmes));
      Item->SubItems->Add(DoubleToStr("%4.3f", medicao->energia.carga_ini_mwhmes));
      Item->SubItems->Add(DoubleToStr("%4.3f", medicao->PerdaTotal_mwhmes()));
      Item->SubItems->Add(DoubleToStr("%3.2f", medicao->PerdaTotal_perc()));
      }
   //força ordenação por nome da rede
   LViewSort.column     = LViewMedicao->Column[0];
   LViewSort.ascendente = true;
   LViewMedicao->AlphaSort();
   LViewMedicao->Items->EndUpdate();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormMedicao::ValidaMedicao(void)
   {
   //variáveis locais
   TMedicao   *medicao;
   VTMedicoes *medicoes   = (VTMedicoes*)apl->GetObject(__classid(VTMedicoes));
   TList      *lisMEDICAO = medicoes->LisMedicao();

   //verifica se há Medicao com perda negativa
   for (int n = 0; n < lisMEDICAO->Count; n++)
      {
      medicao = (TMedicao*)lisMEDICAO->Items[n];
      if (IsDoubleZero(medicao->energia.medida_mwhmes)) continue;
      if (medicao->PerdaTotal_mwhmes() <= 0)
         {
         Aviso(medicao->rede->Codigo + ": energia medida incompatível com a energia das cargas");
         return(false);
         }
      }
   return(true);
   }

//---------------------------------------------------------------------------
//eof

void __fastcall TFormMedicao::LViewMedicaoCustomDrawItem(TCustomListView *Sender,
          TListItem *Item, TCustomDrawState State, bool &DefaultDraw)

{
	TMedicao   *medicao;

	medicao = (TMedicao*) Item->Data;

	if (medicao->valida)
	{
		LViewMedicao->Canvas->Brush->Color = clMoneyGreen;
	}
	else
	{
    	LViewMedicao->Canvas->Brush->Color = clYellow;
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMedicao::ActionDetalheExecute(TObject *Sender)
{
	//variáveis locais
	TMedicao        *medicao;
	TForm *form;

	//verifica se há uma Medicao selecionada
	if (LViewMedicao->Selected == NULL)
	{
		Aviso("Selecione a Medicao a ser detalhada");
		return;
	}

	medicao = (TMedicao*)LViewMedicao->Selected->Data;
	form = new TFormMedicaoDetalhe(this, apl,medicao, DaysInMonth(Date));
    form->Show();

}
//---------------------------------------------------------------------------

