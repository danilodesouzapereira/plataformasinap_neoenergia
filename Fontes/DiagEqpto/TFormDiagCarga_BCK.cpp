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
   DIAG_CARGA     *diagCarga;
   int      index;
   if(editado)
   {
      if( Confirma("Todas altera��es ser�o perdidas","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         diagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
         PreencheSGrids(diagCarga);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ActionConfirmaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoCarga;
   DIAG_CARGA     *diagCarga;
   int      index;

   //verifica se foi editado
   if(editado)
   {  //v� qual tipo de rede est� sendo salvo
      index = listBox->ItemIndex;
      diagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
      //salva as altera��es
      if(SalvaSGrids(diagCarga))
      {  //confirma valores  em edi��o
         sgLimites->EditorMode = false;
         //reseta o estado editado
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::ActionDefaultExecute(TObject *Sender)
{  //variaveis locais
   DIAG_CARGA *diagCarga;

   //pega o PADRAO
   //diagCarga = cfgDiagEqptoCarga->diagCargaPADRAO;
   for (int nc = 0; nc < diagEqptoElem->ListCFG->Count; nc++)
   {
      diagCarga = (DIAG_CARGA*) diagEqptoElem->ListCFG->Items[nc];
      if(diagCarga->classe == 99)
      {  //na leitura eu garanto que h� um diag_Carga padrao
         break;
      }
   }
   if(diagCarga)
   {  //preenche os sgrids com o padrao
      PreencheSGrids(diagCarga);
      //como os valores foram alterados por decisao do usu�rio
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagCarga::FormClose(TObject *Sender, TCloseAction &Action)
{
   diagEqptoElem->SalvaArquivo();
}

//---------------------------------------------------------------------------
//void __fastcall TFormCfgCarga::Inicia(TList *LisCfgDiagCarga)
void __fastcall TFormDiagEqptoCarga::Inicia(TDiagEqptoCarga *diagEqptoCarga)
{
   this->diagEqptoElem   = diagEqptoCarga;
   //this->LisCfgDiagCarga  = diagEqptoCarga->LisCfgDiagCarga;
   //this->LisCfgDiagCarga   = LisCfgDiagCarga;
   //Preenche o Listbox com os tipos de rede
   ListBoxInicia();
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagEqptoCarga::lbTipoCargaClick(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoCarga;
   DIAG_CARGA     *diagCarga;
   int      index;

   //se foi editado pergunta
   if(editado)
   {
      if( Confirma("As altera��es ser�o perdidas.","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         diagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
         PreencheSGrids(diagCarga);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }//volta a sele��o ao antigo tipo
      else{listBox->ItemIndex = ultimoTipoSelecionado;}
   }
   else
   {  //guarda ultima mudan�a v�lida
      ultimoTipoSelecionado = listBox->ItemIndex ;
      index = listBox->ItemIndex;
      diagCarga = (DIAG_CARGA*) listBox->Items->Objects[index];
      PreencheSGrids(diagCarga);
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
   DIAG_CARGA     *diagCarga;


   if(diagEqptoElem->ListCFG)
   {
      for(int i = 0; i < diagEqptoElem->ListCFG->Count; i++)
      {
         diagCarga = (DIAG_CARGA*) diagEqptoElem->ListCFG->Items[i];
         listBox->AddItem(diagCarga->codigo, diagCarga);
      }
   }
   //escolhe o 1o
   listBox->Selected[0];
   listBox->ItemIndex = 0;
   ultimoTipoSelecionado = listBox->ItemIndex;
   listBox->Sorted = true;
   //carrega o 1o
   diagCarga = (DIAG_CARGA*) listBox->Items->Objects[0];
   //preenche os titulos e acerta o tamanho das colunas
   SGridsInicia();
   //preenche com os valores de limites do tipo de rede selecionado
   PreencheSGrids(diagCarga);
}

//---------------------------------------------------------------------------
void  __fastcall TFormDiagCarga::PreencheSGrids(DIAG_CARGA *diagCarga)
{
   if(diagCarga)
   {  //preenche com os limites do Carga
      sgLimites->Cells[colLIMINF][1] = 0;
      sgLimites->Cells[colLIMSUP][1] = diagCarga->energiaMax_mwh;
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagCarga::SalvaSGrids(DIAG_CARGA *diagCarga)
{  //recebe o ponteiro de um DIAG_CARGA
   if(!diagCarga) return false;
   else
   {  //salva as altera�oes dos SGrids no objeto
      //mas � necess�rio verificar a coerencia dos limites
      if(VerificaLimitesSGrids())
      {//se for valido salva mudan�as
         //diagCarga->energiaMax_mwh  = StrToDouble(sgLimites->Cells[colLIMINF][1]);
         diagCarga->energiaMax_mwh  = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
         return true;
      }//se nao tiver valido
      else return false;
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagEqptoCarga::sgLimitesSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{  //variaveis locais
   double   valor;

   if(!StrToDouble(Value, valor))
   {
      Aviso("Valor inv�lido, por favor digite um numero.");
      sgLimites->Cells[ACol][ARow] = "0";
   }
   else
   {
      //como os valores foram alterados por decisao do usu�rio
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
   sgLimites->Cells[colNOME][1] = "Consumo M�ximo [MWh/m�s]";

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


