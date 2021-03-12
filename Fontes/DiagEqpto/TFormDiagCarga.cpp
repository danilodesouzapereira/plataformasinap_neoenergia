//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "TFormDiagCarga.h"
#include "VTDiagEqpto.h"
#include "TDiagCarga.h" /* TODO : Transformar num VT */
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDiagCarga *FormDiagCarga;
//---------------------------------------------------------------------------
__fastcall TFormDiagCarga::TFormDiagCarga(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
{
   this->apl = apl;

}
//---------------------------------------------------------------------------
__fastcall TFormDiagCarga::~TFormDiagCarga(void)
{
//
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ActionCancelaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoCarga;
   DIAG_CARGA     *cfgDiagCarga;
   int      index;
   if(editado)
   {
      if( Confirma("Todas alterações serão perdidas","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagCarga);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ActionConfirmaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoCarga;
   DIAG_CARGA     *cfgDiagCarga;
   int      index;

   //verifica se foi editado
   if(editado)
   {  //vê qual tipo de rede está sendo salvo
      index = listBox->ItemIndex;
      cfgDiagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
      //salva as alterações
      if(SalvaSGrids(cfgDiagCarga))
      {  //confirma valores  em edição
         sgLimites->EditorMode = false;
         //reseta o estado editado
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ActionDefaultExecute(TObject *Sender)
{  //variaveis locais
   DIAG_CARGA *cfgDiagCarga;

   //pega o PADRAO
   //diagCarga = cfgDiagEqptoCarga->diagCargaPADRAO;
   for (int nc = 0; nc < diagEqpto->ListCFG->Count; nc++)
   {
      cfgDiagCarga = (DIAG_CARGA*) diagEqpto->ListCFG->Items[nc];
      if(cfgDiagCarga->classe == 99)
      {  //na leitura eu garanto que há um diag_Carga padrao
         break;
      }
   }
   if(cfgDiagCarga)
   {  //preenche os sgrids com o padrao
      PreencheSGrids(cfgDiagCarga);
      //como os valores foram alterados por decisao do usuário
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::FormClose(TObject *Sender, TCloseAction &Action)
{
   diagEqpto->SalvaArquivo();
}

//---------------------------------------------------------------------------
//void __fastcall TFormCfgCarga::Inicia(TList *LisCfgDiagCarga)
void __fastcall TFormDiagCarga::Inicia(TDiagCarga *diagCarga)
{
   this->diagEqpto  = diagCarga;
   //this->LisCfgDiagCarga  = diagEqptoCarga->LisCfgDiagCarga;
   //this->LisCfgDiagCarga   = LisCfgDiagCarga;
   //Preenche o Listbox com os tipos de rede
   ListBoxInicia();
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::lbTipoCargaClick(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoCarga;
   DIAG_CARGA     *cfgDiagCarga;
   int      index;

   //se foi editado pergunta
   if(editado)
   {
      if( Confirma("As alterações serão perdidas.","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagCarga);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }//volta a seleção ao antigo tipo
      else{listBox->ItemIndex = ultimoTipoSelecionado;}
   }
   else
   {  //guarda ultima mudança válida
      ultimoTipoSelecionado = listBox->ItemIndex ;
      index = listBox->ItemIndex;
      cfgDiagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
      PreencheSGrids(cfgDiagCarga);
      //como os valores foram "editados" por maquina...
      SGridsEditado(false);
   }

}
//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ListBoxInicia(void)
{//variaveis locais
   TListBox        *listBox = lbTipoCarga;
   //VTRedes         *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //VTRede          *rede;
   DIAG_CARGA     *cfgDiagCarga;


   if(diagEqpto->ListCFG)
   {
      for(int i = 0; i < diagEqpto->ListCFG->Count; i++)
      {
         cfgDiagCarga = (DIAG_CARGA*) diagEqpto->ListCFG->Items[i];
         listBox->AddItem(cfgDiagCarga->codigo, cfgDiagCarga);
      }
   }
   //escolhe o 1o
   //listBox->Selected[0];
   listBox->ItemIndex = 0;
   ultimoTipoSelecionado = listBox->ItemIndex;
   listBox->Sorted = true;
   //carrega o 1o
   cfgDiagCarga = (DIAG_CARGA*) listBox->Items->Objects[0];
   //preenche os titulos e acerta o tamanho das colunas
   SGridsInicia();
   //preenche com os valores de limites do tipo de rede selecionado
   PreencheSGrids(cfgDiagCarga);
}

//---------------------------------------------------------------------------
void  __fastcall TFormDiagCarga::PreencheSGrids(DIAG_CARGA *cfgDiagCarga)
{
   if(cfgDiagCarga)
   {  //preenche com os limites do Carga
      sgLimites->Cells[colLIMINF][1] = 0;
      sgLimites->Cells[colLIMSUP][1] = cfgDiagCarga->energiaMax_mwh;
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagCarga::SalvaSGrids(DIAG_CARGA *cfgDiagCarga)
{  //recebe o ponteiro de um DIAG_CARGA
   if(!cfgDiagCarga) return false;
   else
   {  //salva as alteraçoes dos SGrids no objeto
      //mas é necessário verificar a coerencia dos limites
      if(VerificaLimitesSGrids())
      {//se for valido salva mudanças
         //diagCarga->energiaMax_mwh  = StrToDouble(sgLimites->Cells[colLIMINF][1]);
         cfgDiagCarga->energiaMax_mwh  = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
         return true;
      }//se nao tiver valido
      else return false;
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::sgLimitesSetEditText(TObject *Sender, int ACol, int ARow,
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
void __fastcall TFormDiagCarga::SGridsEditado(bool editado)
{  //des/habilita os botoes de confirmar e cancelar
   butConfirma->Enabled = editado;
   butCancela->Enabled  = editado;
   this->editado = editado;
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::SGridsInicia(void)
{  //acerta as dimensoes das colunas
   sgLimites->ColWidths[colNOME] = 150;
   sgLimites->ColWidths[colLIMINF] = 80;
   sgLimites->ColWidths[colLIMSUP] = 80;
   //escreve os titulos
   sgLimites->Cells[colLIMINF][0] = "Limite inferior";
   sgLimites->Cells[colLIMSUP][0] = "Limite superior";
   sgLimites->Cells[colNOME][1] = "Consumo Máximo [MWh/mês]";

}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagCarga::VerificaLimites(double inf, double sup)
{
   if(sup > inf) return true;
   else return false;
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagCarga::VerificaLimitesSGrids(void)
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


