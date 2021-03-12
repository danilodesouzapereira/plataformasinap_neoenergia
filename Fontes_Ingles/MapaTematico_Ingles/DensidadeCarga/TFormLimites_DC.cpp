//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFaixa.h"
#include "TFormLimites_DC.h"
#include "VTTema_DC.h"
#include "..\..\Apl\TApl.h"
#include "..\..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormLimites_DC::TFormLimites_DC(TComponent* Owner, VTApl *apl, TAction *ActionRemontaMapaPadrao)
   : TForm(Owner)
   {
   //variáveis locais
   VTTema_DC   *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList       *lisFAIXA = tema_DC->LisFaixa();
   TFaixa      *faixa;

   //salva ponteiro p/ objetos
   this->apl = apl;
   this->ActionRemontaMapaPadrao = ActionRemontaMapaPadrao;
   //define número mínimo e máximo de faixas
   num_faixa.min     = 2;
   num_faixa.max     = lisFAIXA->Count;
   num_faixa.enabled = 0;
   for (int n = 0; n < lisFAIXA->Count;  n++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[n];
      if (faixa->enabled) num_faixa.enabled++;
      }
   //habilita/desabilita comandos para inserir/retirar faixas
   HabilitaInsereRetiraFaixa();
   //insere Form em seu Parent
   //if (parent) Parent = parent;
   //inicia StringGrid
//   StringGridInicia();
   StringGridIniciaMVA();
   }

//---------------------------------------------------------------------------
__fastcall TFormLimites_DC::~TFormLimites_DC(void)
   {
   //nada a fazer
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::ActionAdicionaExecute(TObject *Sender)
   {
   //proteção
   if (num_faixa.enabled >= num_faixa.max)
      {
      num_faixa.enabled       = num_faixa.max;
      ActionAdiciona->Enabled = false;
      Aviso("Não é possível criar novas faixas");
      return;
      }
   //incrementa número de faixas habilitadas
   num_faixa.enabled++;
   //habilita/desabilita comandos para inserir/retirar faixas
   HabilitaInsereRetiraFaixa();
   //aumenta uma linha no StringGrid
   StringGrid->RowCount++;
   //a coluna 0 já está preenchida com número da faixa, apaga aos valores de limite
   //StringGrid->Cells[1][row_count] = "";
   //StringGrid->Cells[2][row_count] = "";
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::ActionRetiraExecute(TObject *Sender)
   {
   //proteção
   if (num_faixa.enabled <= num_faixa.min)
      {
      num_faixa.enabled     = num_faixa.min;
      ActionRetira->Enabled = false;
      Aviso("Não é possível retirar mais faixas");
      return;
      }
   //decrementa número de faixas habilitadas
   num_faixa.enabled--;
   //habilita/desabilita comandos para inserir/retirar faixas
   HabilitaInsereRetiraFaixa();
   //diminui uma linha no StringGrid
   StringGrid->RowCount--;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::ActionConfirmaExecute(TObject *Sender)
   {
   //variáveis locais
   bool so_validar;

   //valida dados do StringGrid
   if (! StringGridLeValores(so_validar=true)) return;
   //lê dados do StringGrid
   if (! StringGridLeValores(so_validar=false)) return;
   //fecha o Form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::ActionFechaExecute(TObject *Sender)
   {
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::FormShow(TObject *Sender)
   {
   //
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::HabilitaInsereRetiraFaixa(void)
   {
   ActionAdiciona->Enabled = (num_faixa.enabled < num_faixa.max);
   ActionRetira->Enabled   = (num_faixa.enabled > num_faixa.min);
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::RadioGroupUnidadeClick(TObject *Sender)
   {
   //verifica unidade selecionada
   switch(RadioGroupUnidade->ItemIndex)
      {
      case 0:  StringGridIniciaKVA(); break;
      case 1:  StringGridIniciaMVA(); break;
      default: StringGridIniciaMVA(); break;
      }
   }

//---------------------------------------------------------------------------
bool __fastcall TFormLimites_DC::StringGridCellToDouble(int col, int row, double &valor)
   {
   try{//converte e corrige unidade (se necessário)
       valor = StrToDouble(StringGrid->Cells[col][row]);
       if(RadioGroupUnidade->ItemIndex == 0) valor /= 1000.;  //kVA => MVA
       return(true);
      }catch(Exception &e)
         {
         Erro("Faixa " + StringGrid->Cells[0][row] + " com valores inválidos");
         return(false);
         }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::StringGridInicia(void)
   {
   //variaveis locais
   AnsiString  txt;
   VTTema_DC   *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList       *lisFAIXA = tema_DC->LisFaixa();
   TFaixa      *faixa;

   //define número de linhas do StringGrid
   StringGrid->RowCount = num_faixa.max + 1;
   //apaga todas as linhas
   for (int nr = 1; nr < StringGrid->RowCount; nr++) StringGrid->Rows[nr]->Clear();
   //preenche títulos das colunas com intervalos das faixas de demanda
   StringGrid->Cells[0][0] = "Faixa";
   StringGrid->Cells[1][0] = "Limite inferior (MVA)";
   StringGrid->Cells[2][0] = "Limite superior (MVA)";
   for (int nr = 1, i = 0; (nr < StringGrid->RowCount)&&(i < lisFAIXA->Count); nr++, i++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[i];
      //preenche dados da faixa, mesmo que esteja desabilitada
      StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
      StringGrid->Cells[1][nr] = DoubleToStr("%5.4f", faixa->dem_min_mva);
      StringGrid->Cells[2][nr] = DoubleToStr("%5.4f", faixa->dem_max_mva);
      }
   //limita número de linhas do StringGrid ao número de faixas habilitadas
   StringGrid->RowCount = num_faixa.enabled + 1;
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::StringGridIniciaKVA(void)
   {
   //variaveis locais
   AnsiString  txt;
   VTTema_DC   *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList       *lisFAIXA = tema_DC->LisFaixa();
   TFaixa      *faixa;
   double      valor;

   //define número de linhas do StringGrid
   StringGrid->RowCount = num_faixa.max + 1;
   //apaga todas as linhas
   for (int nr = 1; nr < StringGrid->RowCount; nr++) StringGrid->Rows[nr]->Clear();
   //preenche títulos das colunas com intervalos das faixas de demanda
   StringGrid->Cells[0][0] = "Faixa";
   StringGrid->Cells[1][0] = "Limite inferior (kVA)";
   StringGrid->Cells[2][0] = "Limite superior (kVA)";
   for (int nr = 1, i = 0; (nr < StringGrid->RowCount)&&(i < lisFAIXA->Count); nr++, i++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[i];
      //preenche dados da faixa, mesmo que esteja desabilitada
      StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
      valor =  faixa->dem_min_mva * 1000;
      StringGrid->Cells[1][nr] = DoubleToStr("%5.4f", valor);
      valor =  faixa->dem_max_mva * 1000;
      StringGrid->Cells[2][nr] = DoubleToStr("%5.4f", valor);
      }
   //limita número de linhas do StringGrid ao número de faixas habilitadas
   StringGrid->RowCount = num_faixa.enabled + 1;
   }

//---------------------------------------------------------------------------
void __fastcall TFormLimites_DC::StringGridIniciaMVA(void)
   {
   //variaveis locais
   AnsiString  txt;
   VTTema_DC   *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList       *lisFAIXA = tema_DC->LisFaixa();
   TFaixa      *faixa;

   //define número de linhas do StringGrid
   StringGrid->RowCount = num_faixa.max + 1;
   //apaga todas as linhas
   for (int nr = 1; nr < StringGrid->RowCount; nr++) StringGrid->Rows[nr]->Clear();
   //preenche títulos das colunas com intervalos das faixas de demanda
   StringGrid->Cells[0][0] = "Faixa";
   StringGrid->Cells[1][0] = "Limite inferior (MVA)";
   StringGrid->Cells[2][0] = "Limite superior (MVA)";
   for (int nr = 1, i = 0; (nr < StringGrid->RowCount)&&(i < lisFAIXA->Count); nr++, i++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[i];
      //preenche dados da faixa, mesmo que esteja desabilitada
      StringGrid->Cells[0][nr] = txt.sprintf("%02d", nr);
      StringGrid->Cells[1][nr] = DoubleToStr("%5.4f", faixa->dem_min_mva);
      StringGrid->Cells[2][nr] = DoubleToStr("%5.4f", faixa->dem_max_mva);
      }
   //limita número de linhas do StringGrid ao número de faixas habilitadas
   StringGrid->RowCount = num_faixa.enabled + 1;
   StringGrid->Refresh();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormLimites_DC::StringGridLeValores(bool so_validar)
   {
   //variaveis locais
   double    lim_inf, lim_sup, lim_inf_next;
   TFaixa    *faixa;
   VTTema_DC *tema_DC  = (VTTema_DC*)apl->GetObject(__classid(VTTema_DC));
   TList     *lisFAIXA = tema_DC->LisFaixa();

   //verifica se está apenas validando os valores
   if (! so_validar)
      {//desabilita todas as faixas
      for (int nf = 0; nf < lisFAIXA->Count; nf++)
         {
         faixa = (TFaixa*)lisFAIXA->Items[nf];
         faixa->enabled = false;
         faixa->visible = false;
         }
      }
   //lê e valida todos os valores sem atualizar os objetos TFaixa
   for (int nr = 1, nf = 0; (nr < StringGrid->RowCount)&&(nf < lisFAIXA->Count); nr++, nf++)
      {
      faixa = (TFaixa*)lisFAIXA->Items[nf];
      //lê valores de limite inferior e superior da mesma faixa
      if (! StringGridCellToDouble(1, nr, lim_inf)) return(false);
      if (! StringGridCellToDouble(2, nr, lim_sup)) return(false);
      //verifica se limite inferior é menor que limite superior
      if (lim_inf >= lim_sup)
         {//valida limites
         Aviso("O limite superior da faixa " + IntToStr(nr) +
               " deve ser maior que o limite inrerior");
         StringGrid->SetFocus();
         return false;
         }
      //verifica se há mais uma faixa depois desta
      if ((nr+1) < StringGrid->RowCount)
         {//lê valor de limite inferior da próxima faixa
         if (! StringGridCellToDouble(1, nr+1, lim_inf_next)) return(false);
         //o limite superior desta faixa deve ser igual ao limite inferior da próxima faixa
         if (! IsDoubleZero(lim_sup - lim_inf_next))
            {
            Aviso("O limite inferior da faixa " + IntToStr(nr+1) +
                  " deve ser igual ao limite superior da faixa " + IntToStr(nr));
            StringGrid->SetFocus();
            return false;
            }
         }
      //verifica se está apenas validando os valores
      if (! so_validar)
         {//habilita a faixa e atualiza seus limites
         faixa->enabled     = true;
         faixa->visible     = true;
         faixa->dem_min_mva = lim_inf;
         faixa->dem_max_mva = lim_sup;
         }
      }
   return true;
   }

//---------------------------------------------------------------------------
//eof


