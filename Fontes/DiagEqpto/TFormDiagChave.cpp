//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "TFormDiagChave.h"
#include "VTDiagEqpto.h"
#include "TDiagChave.h" /* TODO : Transformar num VT */
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDiagChave *FormDiagChave;
//---------------------------------------------------------------------------
__fastcall TFormDiagChave::TFormDiagChave(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
{
   this->apl = apl;

}
//---------------------------------------------------------------------------
__fastcall TFormDiagChave::~TFormDiagChave(void)
{
//
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::ActionCancelaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoChave;
   DIAG_CHAVE     *cfgDiagChave;
   int      index;
   if(editado)
   {
      if( Confirma("Todas alterações serão perdidas","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagChave = (DIAG_CHAVE*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagChave);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::ActionConfirmaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoChave;
   DIAG_CHAVE     *cfgDiagChave;
   int      index;

   //verifica se foi editado
   if(editado)
   {  //vê qual tipo de rede está sendo salvo
      index = listBox->ItemIndex;
      cfgDiagChave = (DIAG_CHAVE*) listBox->Items->Objects[index];
      //salva as alterações
      if(SalvaSGrids(cfgDiagChave))
      {  //confirma valores  em edição
         sgLimites->EditorMode = false;
         //reseta o estado editado
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::ActionDefaultExecute(TObject *Sender)
{  //variaveis locais
   DIAG_CHAVE *cfgDiagChave;

   //pega o PADRAO
   //diagChave = cfgDiagEqptoChave->diagChavePADRAO;
   for (int nc = 0; nc < diagEqpto->ListCFG->Count; nc++)
   {
      cfgDiagChave = (DIAG_CHAVE*) diagEqpto->ListCFG->Items[nc];
      if(cfgDiagChave->tipo_chave == 0)
      {  //na leitura eu garanto que há um diag_Chave padrao
         break;
      }
   }
   if(cfgDiagChave)
   {  //preenche os sgrids com o padrao
      PreencheSGrids(cfgDiagChave);
      //como os valores foram alterados por decisao do usuário
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::FormClose(TObject *Sender, TCloseAction &Action)
{
   diagEqpto->SalvaArquivo();
}

//---------------------------------------------------------------------------
//void __fastcall TFormCfgChave::Inicia(TList *LisCfgDiagChave)
void __fastcall TFormDiagChave::Inicia(TDiagChave *diagChave)
{
   this->diagEqpto   = diagChave;
   //this->LisCfgDiagChave  = diagEqptoChave->LisCfgDiagChave;
   //this->LisCfgDiagChave   = LisCfgDiagChave;
   //Preenche o Listbox com os tipos de rede
   ListBoxInicia();
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::lbTipoChaveClick(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoChave;
   DIAG_CHAVE     *cfgDiagChave;
   int      index;

   //se foi editado pergunta
   if(editado)
   {
      if( Confirma("As alterações serão perdidas.","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagChave = (DIAG_CHAVE*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagChave);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }//volta a seleção ao antigo tipo
      else{listBox->ItemIndex = ultimoTipoSelecionado;}
   }
   else
   {  //guarda ultima mudança válida
      ultimoTipoSelecionado = listBox->ItemIndex ;
      index = listBox->ItemIndex;
      cfgDiagChave = (DIAG_CHAVE*) listBox->Items->Objects[index];
      PreencheSGrids(cfgDiagChave);
      //como os valores foram "editados" por maquina...
      SGridsEditado(false);
   }

}
//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::ListBoxInicia(void)
{//variaveis locais
   TListBox        *listBox = lbTipoChave;
   //VTRedes         *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //VTRede          *rede;
   DIAG_CHAVE     *cfgDiagChave;


   if(diagEqpto->ListCFG)
   {
      for(int i = 0; i < diagEqpto->ListCFG->Count; i++)
      {
         cfgDiagChave = (DIAG_CHAVE*) diagEqpto->ListCFG->Items[i];
         listBox->AddItem(cfgDiagChave->nome_tipo, cfgDiagChave);
      }
   }
   //escolhe o 1o
   //listBox->Selected[0];
   listBox->ItemIndex = 0;
   ultimoTipoSelecionado = listBox->ItemIndex;
   listBox->Sorted = true;
   //carrega o 1o
   cfgDiagChave = (DIAG_CHAVE*) listBox->Items->Objects[0];
   //preenche os titulos e acerta o tamanho das colunas
   SGridsInicia();
   //preenche com os valores de limites do tipo de rede selecionado
   PreencheSGrids(cfgDiagChave);
}

//---------------------------------------------------------------------------
void  __fastcall TFormDiagChave::PreencheSGrids(DIAG_CHAVE *cfgDiagChave)
{
   if(cfgDiagChave)
   {  //preenche com os limites do Chave
      sgLimites->Cells[colLIMINF][1] = cfgDiagChave->inom_min_a;
      sgLimites->Cells[colLIMSUP][1] = cfgDiagChave->inom_max_a;
      sgLimites->Cells[colLIMINF][2] = "-";
      sgLimites->Cells[colLIMSUP][2] = cfgDiagChave->comp_max_m;
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagChave::SalvaSGrids(DIAG_CHAVE *cfgDiagChave)
{  //recebe o ponteiro de um DIAG_CHAVE
   if(!cfgDiagChave) return false;
   else
   {  //salva as alteraçoes dos SGrids no objeto
      //mas é necessário verificar a coerencia dos limites
      if(VerificaLimitesSGrids())
      {//se for valido salva mudanças
         cfgDiagChave->inom_min_a  = StrToDouble(sgLimites->Cells[colLIMINF][1]);
         cfgDiagChave->inom_max_a  = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
         //cfgDiagChave->comp_min_m  = StrToDouble(sgLimites->Cells[colLIMINF][1]);
         cfgDiagChave->comp_max_m  = StrToDouble(sgLimites->Cells[colLIMSUP][2]);
         return true;
      }//se nao tiver valido
      else return false;
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::sgLimitesSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{  //variaveis locais
   double   valor;

   if(!StrToDouble(Value, valor))
   {
      Aviso("Valor inválido, por favor digite um numero.");
      sgLimites->Cells[ACol][ARow] = "0";
   }
   else
   {
      //como os valores foram alterados por decisao do usuário
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::SGridsEditado(bool editado)
{  //des/habilita os botoes de confirmar e cancelar
   butConfirma->Enabled = editado;
   butCancela->Enabled  = editado;
   this->editado = editado;
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagChave::SGridsInicia(void)
{  //acerta as dimensoes das colunas
   sgLimites->ColWidths[colNOME] = 125;
   sgLimites->ColWidths[colLIMINF] = 80;
   sgLimites->ColWidths[colLIMSUP] = 80;
   //escreve os titulos
   sgLimites->Cells[colLIMINF][0] = "Limite inferior";
   sgLimites->Cells[colLIMSUP][0] = "Limite superior";
   sgLimites->Cells[colNOME][1] = "Corrente Nominal [A]";
   sgLimites->Cells[colNOME][2] = "Comprimento [m]";

}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagChave::VerificaLimites(double inf, double sup)
{
   if(sup > inf) return true;
   else return false;
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagChave::VerificaLimitesSGrids(void)
{  //variaveis locais
   double   inf, sup;
   bool valido = true;

   //verifica iNom
   inf = StrToDouble(sgLimites->Cells[colLIMINF][1]);
   sup = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
   if(!VerificaLimites(inf,sup)){valido = false;}

   if(!valido){ Aviso("Limites incoerentes, limite superior menor que limite inferior.");}
   return valido;
}

//---------------------------------------------------------------------------


