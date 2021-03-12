//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <Fontes\Apl\VTApl.h>
#include <Fontes\Classes\VTClasse.h>
#include <Fontes\Obra\VTObra.h>
#include <Fontes\Obra\VTObras.h>
#include <Fontes\Planejamento\VTAlternativa.h>
#include <Fontes\Planejamento\VTEstudo.h>
#include <Fontes\Planejamento\VTPlanejamento.h>
#include <Fontes\Rede\VTCarga.h>
#include <Fontes\Report\VTReport.h>
#include <DLL_Inc\Funcao.h>
#include <DLL_Inc\Report.h>

//---------------------------------------------------------------------------
#include "TFormAnaliseGeral.h"
#include "TFormAnaliseRT.h"
#include "..\Reclamacao\VTAjusta.h"
#include "..\Reclamacao\VTMulta.h"
#include "..\Reclamacao\VTPerfilAlt.h"
#include "..\Reclamacao\VTPerfilBarra.h"
#include "..\Reclamacao\VTPerfilRede.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormAnaliseGeral::TFormAnaliseGeral(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
   {
   //salva ponteiro
   this->apl = apl;
   Inicia();
   }
//---------------------------------------------------------------------------
__fastcall TFormAnaliseGeral::~TFormAnaliseGeral(void)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::ActionExportaExecute(TObject *Sender)
   {
   //variaveis locais
   AnsiString   nome_arq;
   VTReport      *report;

   //cria objeto report
   report = DLL_NewObjReport(apl);
   //esconde formAnaliseGeral
   Hide();
   Application->ModalPopupMode = pmAuto;
   if(! SaveDialogAnalise->Execute()) {Show(); return;}
   //exporta em arq. txt
   nome_arq = ExtractFileName(SaveDialogAnalise->FileName);
//   if (report->ArquivoTexto(nome_arq, SGridAnalise, "AnáliseRT", false))
   //DVK 2015.04.02 passa o caminho inteiro para salvar o arquivo
   if (report->ArquivoTexto(SaveDialogAnalise->FileName, SGridAnalise, "AnáliseRT", false))
      Aviso("A tabela foi salva em " + SaveDialogAnalise->FileName);
   else
      Erro("Erro ao criar arquivo.");
   //destroi report
   if(report) {DLL_DeleteObjReport(report); report = NULL;}
   //reexibe formAnaliseGeral
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::Cabecalho(void)
   {
   //preenche cabeçalhos da SGridAnalise
   SGridAnalise->Cells[0][0] = "Alternativas";
   SGridAnalise->Cells[1][0] = "Pior DRC %";
   SGridAnalise->Cells[2][0] = "Pior DRP %";
   SGridAnalise->Cells[3][0] = "Custo Alternativa (R$)";
   SGridAnalise->ColWidths[3] = 90;
   SGridAnalise->Cells[4][0] = "Multa CA (R$)";
   SGridAnalise->Cells[5][0] = "Multa PE (R$)";
   SGridAnalise->Cells[6][0] = "Multa Rede (R$)";
   SGridAnalise->Cells[7][0] = "Benefício (R$)";
   SGridAnalise->Cells[8][0] = "Relação Custo/Benefício";
   SGridAnalise->ColWidths[8] = 120;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::CalculaCustoAlternativa(VTAlternativa* alternativa)
   {
   //variaveis locais
   double    custo = 0.0;
   TList      *lisOBRA;
   VTObra   *obra;

   //lista obras
   lisOBRA = alternativa->Obras->LisObra();
   for(int no = 0; no < lisOBRA->Count; no++)
      {//soma custo de todas as obras da alternativa
      obra = (VTObra*)lisOBRA->Items[no];
      custo += obra->Custo;
      }
   //guarda valor de custo total
   alternativa->CustoTotal = custo;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::FormClose(TObject *Sender,
      TCloseAction &Action)
   {
   //cancela e sair
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::FormShow(TObject *Sender)
   {
   //reposiciona este Form
//   Top  = 0;
//   Left = 0;
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::Inicia(void)
   {
   //variaveis locais
   TList            *lisALT;
   VTAlternativa   *alternativa;
   VTEstudo         *estudo;
   VTPlanejamento *planejamento    = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));

   //preenche primeira linha do SGrid
   Cabecalho();
   estudo = planejamento->ExisteEstudo(-1);
   lisALT = estudo->LisAlternativa();
   for(int na = 0; na < lisALT->Count; na++)
      {//lista alternativas
      SGridAnalise->RowCount = lisALT->Count+1;
      alternativa = (VTAlternativa*)lisALT->Items[na];
      estudo->DefineAlternativaAtiva(alternativa, NULL);
      //soma obras e guarda em alternativa->CustoTotal
      CalculaCustoAlternativa(alternativa);
      RecalculaPreencheGrid(na+1, alternativa);
      }
   }

//---------------------------------------------------------------------------
double __fastcall TFormAnaliseGeral::MultaTotal(VTMulta *multa, VTPerfilAlt *perfil_alt)
   {
   //variaveis locais
   double         multa_total = 0.0;
   VTCarga        *carga, *carga_orig;
   VTPerfilBarra    *perfil_barra, *perfil_barra_orig;
   TList          *lisCARGA;

   //não executa se a carga da medição for A4
   if(multa->Carga->classe->Codigo.AnsiCompareIC("A4") == 0) return(multa_total);
   //salva carga e perfil_barra originais
   carga_orig = multa->Carga;
   perfil_barra_orig = multa->PerfilBarraPE;
   //lista todas as cargas
   lisCARGA = new TList();
   multa->RedeSec->LisEqbar(lisCARGA,eqptoCARGA,false);
   for(int nc = 0; nc < lisCARGA->Count; nc++)
      {
      carga = (VTCarga*)lisCARGA->Items[nc];
      //determina PerfilBarra da rede com obra
      if ((perfil_barra = perfil_alt->ExistePerfilBarra(carga->pbarra)) != NULL)
         {//guarda Perfil_barra
         multa->PerfilBarraPE = perfil_barra;
         multa->Carga = carga;
         if(! multa->Executa()) {delete lisCARGA; return(0.0);};
         //acumula multas das cargas
         multa_total += multa->ValorMultaCA;
         }
      }
   //retorna os parametros originais
   multa->Carga = carga_orig;
   multa->PerfilBarraPE = perfil_barra_orig;
   //destroi lista
   delete lisCARGA;

   return (multa_total);
   }

//---------------------------------------------------------------------------
void __fastcall TFormAnaliseGeral::RecalculaPreencheGrid(int linha, VTAlternativa *alternativa)
   {
   //variaveis locais
   double         benef, custoBenef, multaRedeOriginal, multaTotal;
   VTAjusta       *ajusta = (VTAjusta*)apl->GetObject(__classid(VTAjusta));
   VTMulta        *multa  = (VTMulta*)apl->GetObject(__classid(VTMulta));
   VTPerfilAlt    *perfil_alt;
   VTPerfilBarra  *perfil_barra;

   //verifica se existe perfil_alt
   if(! alternativa->Obras->Analisada)
      {//recalcula
      if(! ajusta->CalculaFluxoComObra()) return;
      alternativa->Obras->Analisada = true;
      }
   perfil_alt = (VTPerfilAlt*)alternativa->Object;
   perfil_alt->CalculaDRA_DRP_DRC(1008);
   //determina PerfilBarra da rede com obra
   if ((perfil_barra = perfil_alt->ExistePerfilBarra(multa->Carga->pbarra)) != NULL)
      {//guarda Perfil_barra
      multa->PerfilBarraPE = perfil_barra;
      if(! multa->Executa()) return;
      //insere nome alternativa
      SGridAnalise->Cells[0][linha] = alternativa->Codigo;
      //insere DRC, DRP
      SGridAnalise->Cells[1][linha] = FormatFloat("0.0", multa->PiorDRC);
      SGridAnalise->Cells[2][linha] = FormatFloat("0.0", multa->PiorDRP);
      //insere custo alternativa
      SGridAnalise->Cells[3][linha] = FormatFloat("0.00", alternativa->CustoTotal);
      //insere multas
      SGridAnalise->Cells[4][linha] = FormatFloat("0.00", multa->ValorMultaCA);
      SGridAnalise->Cells[5][linha] = FormatFloat("0.00", multa->ValorMultaPE);
      }
   else return;
   multaTotal = MultaTotal(multa, perfil_alt);
   SGridAnalise->Cells[6][linha] = FormatFloat("0.00", multaTotal);
   //insere beneficio
   multaRedeOriginal = StrToDouble(SGridAnalise->Cells[6][1]);
//   benef = multaRedeOriginal - (alternativa->CustoTotal + multa->ValorMultaPE);
   benef = multaRedeOriginal - (alternativa->CustoTotal + multaTotal);
   SGridAnalise->Cells[7][linha] = FormatFloat("0.00", benef);
   //insere custo/beneficio
   if(benef == 0) custoBenef = alternativa->CustoTotal;
   else           custoBenef = alternativa->CustoTotal / benef;
   SGridAnalise->Cells[8][linha] = FormatFloat("0.0", custoBenef); 
   }

//---------------------------------------------------------------------------




