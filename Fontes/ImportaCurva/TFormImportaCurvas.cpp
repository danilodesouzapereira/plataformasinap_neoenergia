//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include <math.hpp>
#include "TFormImportaCurvas.h"
#include "..\Apl\VTApl.h"
#include "..\Constante\Const.h"
#include "..\Curvas\VTCurva.h"
#include "..\Diretorio\VTPath.h"
#include "..\Rede\VTBarra.h"
#include "..\Rede\VTCarga.h"
#include "..\Rede\VTRedes.h"
#include "..\..\DLL_Inc\Curvas.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TFormImportaCurvas::TFormImportaCurvas(TComponent* Owner, VTApl *apl_owner)
   : TForm(Owner)
   {
   //variáveis locais
   VTPath *path = (VTPath*)apl_owner->GetObject(__classid(VTPath));
   //salva ponteiro p/ objetos
   this->apl = apl_owner;
   //cria objeto TStringList
   campos   = new TStringList();
   lisCARGA = new TList();
   lisCURVA = new TList();
   //inicia lista de Cargas
   IniciaLisCarga();
   //cria uma Curva p/ cada Carga
   IniciaLisCurva();
   //inicia DirectoryListBox
   DirectoryListBox->Directory = path->DirImporta();
   //
   if (CheckListBox->Count > 0)
      {
      CheckListBox->Selected[0] = true;
      ActionSGridExecute(NULL);
      }
   }
//---------------------------------------------------------------------------
__fastcall TFormImportaCurvas::~TFormImportaCurvas(void)
   {
   //destrói lista sem destruir seus objetos
   if (lisCARGA)  {delete lisCARGA; lisCARGA = NULL;}
   //destrói lista e seus objetos
   if (lisCURVA)  {LimpaTList(lisCURVA); delete lisCURVA; lisCURVA = NULL;}
   //destroi objeto StringList
   if (campos)    {delete campos; campos = NULL;}
   }
//---------------------------------------------------------------------------
int __fastcall TFormImportaCurvas::AchaIndex(AnsiString arquivo)
   {
   int index;
   int strEnd;
   AnsiString nome;
   strEnd = arquivo.Pos(".") - 1;
   nome = arquivo.SubString(1, strEnd);
   index = CheckListBox->Items->IndexOf(nome);
   while ((index == -1) && (nome.Length()>1) && (nome.SubString(0,1)=="0"))
      {
      nome = nome.SubString (2,nome.Length()-1);
      index = CheckListBox->Items->IndexOf(nome);
      }
   return (index);
   }
//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::ActionCancelaExecute(TObject *Sender)
   {
   //cancela importação da curva
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::ActionConfirmaExecute(TObject *Sender)
   {
   //reinicia Curva das Cargas c/ dados da Curva lida
   for (int n = 0; n < CheckListBox->Count  ; n++)
      {//determina Curva criada neste Forms
      if (CheckListBox->Checked[n])
         {
         curva = (VTCurva*)CheckListBox->Items->Objects[n];
         //determina Carga associada à Curva
         carga = (VTCarga*)curva->Eqpto;
         //atualiza Curva original da Carga c/ dados da Curva lida
         if (carga->curva->Tipica)
            {
            carga->curva = curva;
            lisCURVA->Remove(curva);
            }
         else
            {
            carga->curva->CopiaAtributosDe(*curva);
            }
         }
      }
   //fecha o Form, confirmando atualização das Curvas
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::ActionImportaExecute(TObject *Sender)
   {
   int index;
   for (int i = 0 ; i < FileListBox->Count ; i++)
      {
      if (FileListBox->Selected[i])
         {
         index = AchaIndex(FileListBox->Items->Strings[i]);
         if (index != -1)
            {
            RichEdit->WordWrap = false;
            RichEdit->Lines->LoadFromFile(FileListBox->Directory + (char)92 + FileListBox->Items->Strings[i]);
            curva = (VTCurva*)CheckListBox->Items->Objects[index];
            carga = (VTCarga*)curva->Eqpto;
            ReiniciaCurva();
            CheckListBox->Checked[index] = true;
            }
         }
      }
   ActionSGridExecute(NULL);
   }
//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::ActionSGridExecute(TObject *Sender)
   {
   for (int i = 0 ; i < CheckListBox->Count ; i++)
      {
      if (CheckListBox->Selected[i])
         {
         curva = (VTCurva*)CheckListBox->Items->Objects[i];
         carga = (VTCarga*)curva->Eqpto;
         ReiniciaSGrid();
         GroupBox7->Caption = "Carga: " + CheckListBox->Items->Strings[i];
         }
      }
   }
//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::DirectoryListBoxChange(TObject *Sender)
{
   if (DirectoryListBox->Directory.IsEmpty())
      {
      FileListBox->Directory = NULL;
      }
   else
      {
      FileListBox->Directory = DirectoryListBox->Directory;
      FileListBox->FileType = FilterComboBox->FileList->FileType;
      }
}
//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::ExtraiCampos(AnsiString txt, TStringList *campos)
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
void __fastcall TFormImportaCurvas::IniciaLisCarga(void)
   {
   //variáveis locais
   VTRedes *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //preenche lisCARGA com as cargas das redes
   redes->LisEqbar(lisCARGA, eqptoCARGA);
   }

//---------------------------------------------------------------------------
void __fastcall TFormImportaCurvas::IniciaLisCurva(void)
   {
   //limpa lisCURVA
   lisCURVA->Clear();
   //cria uma Curva p/ cada Carga em lisCARGA
   for (int n = 0; n < lisCARGA->Count; n++)
      {
      carga = (VTCarga*)lisCARGA->Items[n];
      //cria Curva igual à da Carga
      if (carga->curva->Tipica)
         {
         curva = curva->Clone();
         curva->Tipica = false;
         }
      else
         {
         curva  = DLL_NewObjCurva();
         curva->CopiaAtributosDe(*carga->curva);
         }
      curva->Eqpto = (VTEqpto*)carga;
      //redefine escala da Curva p/ 24 pontos
      curva->Escala = pat01HORA;
      //insere Curva em lisCURVA
      lisCURVA->Add(curva);
      //add item no checklistbox para cada carga
      if (carga->Codigo != "")
         {
         CheckListBox->AddItem(carga->Codigo, curva);
         }
      else
         {
         CheckListBox->AddItem("( Id: " + AnsiString(carga->Id) + " )", curva);
         }
      CheckListBox->Checked[n] = false;
      CheckListBox->ItemEnabled[n] = false;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormImportaCurvas::InserePontoNaCurva(int hora, int minuto, double i_amp)
   {
   //variáveis locais
   double fat_pot, s_mva, p_mw, q_mvar;
   double valor[nvCURVA_CAR];
   strHM  hm;
   //determina fator de potência selecionado
   fat_pot = StrToDouble(CBoxFatPot->Text);
   //converte corrente em potência
   s_mva   = ((sqrt(3.) * carga->pbarra->vnom) * i_amp) / 1000.;
   p_mw    = s_mva * fat_pot;
   q_mvar  = s_mva * sin(ArcCos(fat_pot));
   //verifica se a Curva já possui um Ponto p/ hora/minuto indicado
   hm.hora   = hora;
   hm.minuto = 0;
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
void __fastcall TFormImportaCurvas::ReiniciaCurva()
   {
   //variáveis locais
   int    hora, minuto;
   double i_amp;
   //reinicia pontos da Curva
   curva->Reinicia();
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
void __fastcall TFormImportaCurvas::ReiniciaSGrid(void)
   {
   //variáveis locais
   AnsiString txt;
   int        num_ponto;
   double     fat_pot, s_kva, i_amp;
   double     valor[nvCURVA_CAR];
   strHM      hm_ini, hm_fim;

   //reinicia SGrid
   for (int nr = 0; nr < SGrid->RowCount; nr++) SGrid->Rows[nr]->Clear();
   //Se for curva tipica, nao mostra nada
   if (carga->curva->Tipica) return;
   //verifica se a Curva está vazia
   if ((num_ponto = curva->NumeroPontos()) == 0) return;
   //dimensiona número de linhas
   SGrid->RowCount = num_ponto + 1;
   SGrid->ColCount = 2;
   SGrid->ColWidths[0] = 50;
   SGrid->ColWidths[1] = 50;
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
      i_amp = s_kva / (sqrt(3.) * carga->pbarra->vnom);
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




