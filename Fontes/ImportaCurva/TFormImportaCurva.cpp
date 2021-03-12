//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TFormImportaCurva.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormImportaCurva::TFormImportaCurva(TComponent* Owner, VTApl *apl_owner,
                                                VTCarga *carga_orig, VTCurva *curva_orig)
   : TForm(Owner)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));

   //salva ponteiro p/ objetos
   this->apl        = apl_owner;
   this->carga_orig = carga_orig;
   this->curva_orig = curva_orig;
   //cria objeto TStringList
   campos = new TStringList();
   //cria e inicia objeto Curva
   curva  = DLL_NewObjCurva();
   curva->CopiaAtributosDe(*curva_orig);
   curva->Escala = pat01HORA;
   //inicia DirectoryListBox
   DirectoryListBox->Directory = path->DirImporta();
   //seleciona TabSheet
   PageControl->ActivePage = TabSheetDefault;
   }

//---------------------------------------------------------------------------
__fastcall TFormImportaCurva::~TFormImportaCurva(void)
   {
   //destrói objetos
   if (curva)  {delete curva; curva = NULL;}
   if (campos) {delete campos; campos = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ActionCancelaExecute(TObject *Sender)
   {
   //cancela importação da curva
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ActionConfirmaExecute(TObject *Sender)
   {
   //reinicia Curva original c/ dados da Curva local
   curva_orig->CopiaAtributosDe(*curva);
   //confirma inserção da curva
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ExtraiCampos(AnsiString txt, TStringList *campos)
   {
   //variáveis locais
   TSysCharSet Separators;
	TSysCharSet WhiteSpace;
	WideString  ws_txt = txt;

   //inicia conjunto de caracteres separadores
   Separators << '\/' << ':' << ' ' << '\t';
   //inicia conjunto de caracteres de espaçamento
   WhiteSpace << ' ';
   //reinicia StrinList campos
   campos->Clear();
   //separa campos do txt
	ExtractStrings(Separators, WhiteSpace, ws_txt.c_bstr(), campos);
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::FileListBoxChange(TObject *Sender)
   {
   //verifica se há um arquivo selecionado
   if (FileListBox->FileName.IsEmpty())
      {//limpa RichEdit
      RichEdit->Lines->Clear();
      }
   else
      {//carrega conteúdo do arquivo selecionado
      RichEdit->Lines->LoadFromFile(FileListBox->FileName);
      }
   //reinicia curva
   ReiniciaCurva();
   //atualiza SGrid
   ReiniciaSGrid();
   //atualiza Chart
   ReiniciaChart();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormImportaCurva::InserePontoNaCurva(int hora, int minuto, double i_amp)
   {
   //variáveis locais
   double fat_pot, s_mva, p_mw, q_mvar;
   double valor[nvCURVA_CAR];
   strHM  hm;

   //determina fator de potência selecionado
   fat_pot = StrToDouble(CBoxFatPot->Text);
   //converte corrente em potência
   s_mva   = ((sqrt(3.) * carga_orig->pbarra->vnom) * i_amp) / 1000.;
   p_mw    = s_mva * fat_pot;
   q_mvar  = s_mva * sin(ArcCos(fat_pot));
   //verifica se a Curva já possui um Ponto p/ hora indicada e minuto=0
   hm.hora   = hora;
   hm.minuto = 0; //minuto;
   if (curva->GetValor(hm, valor, sizeof(valor)/sizeof(double)))
      {//já existe um Ponto na Curva: atualiza se o novo valor for maior que o anterior
      if (p_mw <= valor[0]) return(false);
      }
   //insere ou atualiza ponto da Curva
   valor[0] = p_mw;
   valor[1] = q_mvar;
   curva->SetPonto(hm, valor, sizeof(valor)/sizeof(double));
   return(true);
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ReiniciaChart(void)
   {
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ReiniciaCurva(void)
   {
   //variáveis locais
   int    hora, minuto;
   double i_amp;

   //reinicia pontos da Curva
   curva->Reinicia();
   //verifica se há um arquivo selecionado
   if (FileListBox->FileName.IsEmpty()) return;
   //loop p/ todas linhas do arquivo selecionado
   for (int nl = 0; nl < RichEdit->Lines->Count; nl++)
      {//extrai campos da linha
      ExtraiCampos(RichEdit->Lines->Strings[nl], campos);
      //verifica se foram extraídos 7 campos: dia, mes, ano, hora, minuto, segundo, valor
      if (campos->Count != 7) continue;
      hora   = StrToInteger(campos->Strings[3]);
      minuto = StrToInteger(campos->Strings[4]);
      i_amp  = StrToDouble(campos->Strings[6]);
      //insere Ponto na Curva
      InserePontoNaCurva(hora, minuto, i_amp);
      }
   //ordena Pontos da Curva
   curva->OrdenaPontos();
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurva::ReiniciaSGrid(void)
   {
   //variáveis locais
   AnsiString txt;
   int        num_ponto;
   double     fat_pot, s_kva, i_amp;
   double     valor[nvCURVA_CAR];
   strHM      hm_ini, hm_fim;

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) SGrid->Rows[nr]->Clear();
   //verifica se a Curva está vazia
   if ((num_ponto = curva->NumeroPontos()) == 0) return;
   //dimensiona número de linhas
   SGrid->RowCount = num_ponto + 1;
   //define títulos das colunas
   SGrid->Cells[0][0] = "Patamar";
   SGrid->Cells[1][0] = "I(A)";
   SGrid->Cells[2][0] = "P(MW)";
   SGrid->Cells[3][0] = "Q(MVAr)";
   //determina fator de potência selecionado
   fat_pot = StrToDouble(CBoxFatPot->Text);
   //preenche SGrid c/ pontos da Curva
   for (int np = 0, nr = 1; np < num_ponto; np++, nr++)
      {//obtém dados do ponto
      if (! curva->GetPonto(np, hm_ini, hm_fim, valor, sizeof(valor)/sizeof(double))) continue;
      //converte potência em corrente
      s_kva = (valor[0] / fat_pot) * 1000.;
      i_amp = s_kva / (sqrt(3.) * carga_orig->pbarra->vnom);
      //atualiza SGrid
      txt.sprintf("%02d:%02d", hm_ini.hora, hm_ini.minuto);
      SGrid->Cells[0][nr] = txt;
      SGrid->Cells[1][nr] = DoubleToStr("%4.3f", i_amp);
      SGrid->Cells[2][nr] = DoubleToStr("%4.3f", valor[0]);
      SGrid->Cells[3][nr] = DoubleToStr("%4.3f", valor[1]);
      }
   }

//---------------------------------------------------------------------------
//eof


