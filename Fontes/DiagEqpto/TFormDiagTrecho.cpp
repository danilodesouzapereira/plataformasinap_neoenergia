//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "..\Apl\VTApl.h"
#include "..\Geral\VTGeral.h"
#include "..\Rede\VTRede.h"
#include "..\Rede\VTRedes.h"
#include "TFormDiagTrecho.h"
#include "TDiagTrecho.h" /* TODO : Transformar num VT */
#include "VTDiagEqpto.h" /* TODO : Transformar num VT */
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormDiagTrecho *FormDiagTrecho;
//---------------------------------------------------------------------------
__fastcall TFormDiagTrecho::TFormDiagTrecho(TComponent* Owner, VTApl *apl)
   : TForm(Owner)
{
   this->apl = apl;

}
//---------------------------------------------------------------------------
__fastcall TFormDiagTrecho::~TFormDiagTrecho(void)
{
//
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::ActionCancelaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoRede;
   DIAG_TRECHO     *cfgDiagTrecho;
   int      index;
   if(editado)
   {
      if( Confirma("Todas alterações serão perdidas","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagTrecho = (DIAG_TRECHO*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagTrecho);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::ActionConfirmaExecute(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoRede;
   DIAG_TRECHO     *cfgDiagTrecho;
   int      index;

   //verifica se foi editado
   if(editado)
   {  //vê qual tipo de rede está sendo salvo
      index = listBox->ItemIndex;
      cfgDiagTrecho = (DIAG_TRECHO*) listBox->Items->Objects[index];
      //salva as alterações
      if(SalvaSGrids(cfgDiagTrecho))
      {  //confirma valores  em edição
         sgLimites->EditorMode = false;
         sgArranjo->EditorMode = false;
         //reseta o estado editado
         SGridsEditado(false);
      }
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::ActionDefaultExecute(TObject *Sender)
{  //variaveis locais
   DIAG_TRECHO *cfgDiagTrecho;

   //pega o PADRAO
   //diagTrecho = cfgDiagEqptoTrecho->diagTrechoPADRAO;
   for (int nc = 0; nc < diagEqpto->ListCFG->Count; nc++)
   {
      cfgDiagTrecho = (DIAG_TRECHO*) diagEqpto->ListCFG->Items[nc];
      if(cfgDiagTrecho->tipo_rede == 0)
      {  //na leitura eu garanto que há um diag_trecho padrao
         break;
      }
   }
   if(cfgDiagTrecho)
   {  //preenche os sgrids com o padrao
      PreencheSGrids(cfgDiagTrecho);
      //como os valores foram alterados por decisao do usuário
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::FormClose(TObject *Sender, TCloseAction &Action)
{
   diagEqpto->SalvaArquivo();
}

//---------------------------------------------------------------------------
//void __fastcall TFormCfgTrecho::Inicia(TList *LisCfgDiagTrecho)
void __fastcall TFormDiagTrecho::Inicia(TDiagTrecho *diagTrecho)
{
   this->diagEqpto   = diagTrecho;
   //this->LisCfgDiagTrecho  = diagEqptoTrecho->LisCFG;
   //this->LisCfgDiagTrecho   = LisCfgDiagTrecho;
   //Preenche o Listbox com os tipos de rede
   ListBoxInicia();
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::lbTipoRedeClick(TObject *Sender)
{  //variaveis locais
   TListBox *listBox = lbTipoRede;
   DIAG_TRECHO     *cfgDiagTrecho;
   int      index;

   //se foi editado pergunta
   if(editado)
   {
      if( Confirma("As alterações serão perdidas.","Deseja continuar?") == idYes)
      {
         index = listBox->ItemIndex;
         cfgDiagTrecho = (DIAG_TRECHO*) listBox->Items->Objects[index];
         PreencheSGrids(cfgDiagTrecho);
         //como os valores foram "editados" por maquina...
         SGridsEditado(false);
      }//volta a seleção ao antigo tipo
      else{listBox->ItemIndex = ultimoTipoSelecionado;}
   }
   else
   {  //guarda ultima mudança válida
      ultimoTipoSelecionado = listBox->ItemIndex ;
      index = listBox->ItemIndex;
      cfgDiagTrecho = (DIAG_TRECHO*) listBox->Items->Objects[index];
      PreencheSGrids(cfgDiagTrecho);
      //como os valores foram "editados" por maquina...
      SGridsEditado(false);
   }

}
//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::ListBoxInicia(void)
{//variaveis locais
   TListBox        *listBox = lbTipoRede;
   //VTRedes         *redes = (VTRedes*)apl->GetObject(__classid(VTRedes));
   //VTRede          *rede;
   DIAG_TRECHO     *cfgDiagTrecho;


   if(diagEqpto->ListCFG)
   {
      for(int i = 0; i < diagEqpto->ListCFG->Count; i++)
      {
         cfgDiagTrecho = (DIAG_TRECHO*) diagEqpto->ListCFG->Items[i];
         listBox->AddItem(cfgDiagTrecho->nome_tipo, cfgDiagTrecho);
      }
   }
   //escolhe o 1o
   //listBox->Selected[0];
   listBox->ItemIndex = 0;
   ultimoTipoSelecionado = listBox->ItemIndex;
   listBox->Sorted = true;
   //carrega o 1o
   cfgDiagTrecho = (DIAG_TRECHO*) listBox->Items->Objects[0];
   //preenche os titulos e acerta o tamanho das colunas
   SGridsInicia();
   //preenche com os valores de limites do tipo de rede selecionado
   PreencheSGrids(cfgDiagTrecho);
}

//---------------------------------------------------------------------------
void  __fastcall TFormDiagTrecho::PreencheSGrids(DIAG_TRECHO *cfgDiagTrecho)
{
   if(cfgDiagTrecho)
   {
      //preenche com os limites do trecho
      sgLimites->Cells[colLIMINF][1] = cfgDiagTrecho->comp_min_m;
      sgLimites->Cells[colLIMSUP][1] = cfgDiagTrecho->comp_max_m;
      //preenche com os limites inferiores do arranjo proprio
      sgArranjo->Cells[colLIMINF][1]=  cfgDiagTrecho->iadm_min_a;
      sgArranjo->Cells[colLIMINF][2]=  cfgDiagTrecho->seq0_r_min_ohmkm;
      sgArranjo->Cells[colLIMINF][3]=  cfgDiagTrecho->seq0_x_min_ohmkm;
      sgArranjo->Cells[colLIMINF][4]=  cfgDiagTrecho->seq0_c_min_nFkm;
      sgArranjo->Cells[colLIMINF][5]=  cfgDiagTrecho->seq1_r_min_ohmkm;
      sgArranjo->Cells[colLIMINF][6]=  cfgDiagTrecho->seq1_x_min_ohmkm;
      sgArranjo->Cells[colLIMINF][7]=  cfgDiagTrecho->seq1_c_min_nFkm;
      //preenche com os limites superiores do arranjo proprio
      sgArranjo->Cells[colLIMSUP][1]=  cfgDiagTrecho->iadm_max_a;
      sgArranjo->Cells[colLIMSUP][2]=  cfgDiagTrecho->seq0_r_max_ohmkm;
      sgArranjo->Cells[colLIMSUP][7]=  cfgDiagTrecho->seq0_x_max_ohmkm;
      sgArranjo->Cells[colLIMSUP][3]=  cfgDiagTrecho->seq0_c_max_nFkm;
      sgArranjo->Cells[colLIMSUP][4]=  cfgDiagTrecho->seq1_r_max_ohmkm;
      sgArranjo->Cells[colLIMSUP][5]=  cfgDiagTrecho->seq1_x_max_ohmkm;
      sgArranjo->Cells[colLIMSUP][6]=  cfgDiagTrecho->seq1_c_max_nFkm;
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagTrecho::SalvaSGrids(DIAG_TRECHO *cfgDiagTrecho)
{  //recebe o ponteiro de um DIAG_TRECHO
   if(!cfgDiagTrecho) return false;
   else
   {  //salva as alteraçoes dos SGrids no objeto
      //mas é necessário verificar a coerencia dos limites
      if(VerificaLimitesSGrids())
      {//se for valido salva mudanças
         cfgDiagTrecho->comp_min_m       = StrToDouble(sgLimites->Cells[colLIMINF][1]);
         cfgDiagTrecho->comp_max_m       = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
         cfgDiagTrecho->iadm_min_a       = StrToDouble(sgArranjo->Cells[colLIMINF][1]);
         cfgDiagTrecho->seq0_r_min_ohmkm = StrToDouble(sgArranjo->Cells[colLIMINF][2]);
         cfgDiagTrecho->seq0_x_min_ohmkm = StrToDouble(sgArranjo->Cells[colLIMINF][3]);
         cfgDiagTrecho->seq0_c_min_nFkm  = StrToDouble(sgArranjo->Cells[colLIMINF][4]);
         cfgDiagTrecho->seq1_r_min_ohmkm = StrToDouble(sgArranjo->Cells[colLIMINF][5]);
         cfgDiagTrecho->seq1_x_min_ohmkm = StrToDouble(sgArranjo->Cells[colLIMINF][6]);
         cfgDiagTrecho->seq1_c_min_nFkm  = StrToDouble(sgArranjo->Cells[colLIMINF][7]);
         cfgDiagTrecho->iadm_max_a       = StrToDouble(sgArranjo->Cells[colLIMSUP][1]);
         cfgDiagTrecho->seq0_r_max_ohmkm = StrToDouble(sgArranjo->Cells[colLIMSUP][2]);
         cfgDiagTrecho->seq0_x_max_ohmkm = StrToDouble(sgArranjo->Cells[colLIMSUP][7]);
         cfgDiagTrecho->seq0_c_max_nFkm  = StrToDouble(sgArranjo->Cells[colLIMSUP][3]);
         cfgDiagTrecho->seq1_r_max_ohmkm = StrToDouble(sgArranjo->Cells[colLIMSUP][4]);
         cfgDiagTrecho->seq1_x_max_ohmkm = StrToDouble(sgArranjo->Cells[colLIMSUP][5]);
         cfgDiagTrecho->seq1_c_max_nFkm  = StrToDouble(sgArranjo->Cells[colLIMSUP][6]);
         return true;
      }//se nao tiver valido
      else return false;
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::sgArranjoSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{  //variaveis locais
   double      valor;

   if(!StrToDouble(Value, valor))
   {  //se não for numero gera um aviso
      Aviso("Valor inválido, por favor digite um numero.");
      sgArranjo->Cells[ACol][ARow] = "0";
   }
   else
   {  //como os valores foram alterados por decisao do usuário
      SGridsEditado(true);
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::sgLimitesSetEditText(TObject *Sender, int ACol, int ARow,
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
void __fastcall TFormDiagTrecho::SGridsEditado(bool editado)
{  //des/habilita os botoes de confirmar e cancelar
   butConfirma->Enabled = editado;
   butCancela->Enabled  = editado;
   this->editado = editado;
}
//---------------------------------------------------------------------------
void __fastcall TFormDiagTrecho::SGridsInicia(void)
{  //acerta as dimensoes das colunas
   sgLimites->ColWidths[colNOME] = 125;
   sgArranjo->ColWidths[colNOME] = 125;
   sgLimites->ColWidths[colLIMINF] = 80;
   sgArranjo->ColWidths[colLIMINF] = 80;
   sgLimites->ColWidths[colLIMSUP] = 80;
   sgArranjo->ColWidths[colLIMSUP] = 80;
   //escreve os titulos
   sgLimites->Cells[colLIMINF][0] = "Limite inferior";
   sgLimites->Cells[colLIMSUP][0] = "Limite superior";
   sgLimites->Cells[colNOME][1] = "Comprimento [m]";
   //preenche o string grid dos parametros de arranjo proprio
   sgArranjo->Cells[colLIMINF][0] = "Limite inferior";
   sgArranjo->Cells[colLIMSUP][0] = "Limite superior";
   sgArranjo->Cells[colNOME][1] = "Corrente admissível [A]";
   sgArranjo->Cells[colNOME][2] = "Seq.0 R [ohm/km]";
   sgArranjo->Cells[colNOME][3] = "Seq.0 X [ohm/km]";
   sgArranjo->Cells[colNOME][4] = "Seq.0 C [nF/km]";
   sgArranjo->Cells[colNOME][5] = "Seq.1 R [ohm/km]";
   sgArranjo->Cells[colNOME][6] = "Seq.1 X [ohm/km]";
   sgArranjo->Cells[colNOME][7] = "Seq.1 C [nF/km]";
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagTrecho::VerificaLimites(double inf, double sup)
{
   if(sup > inf) return true;
   else return false;
}

//---------------------------------------------------------------------------
bool  __fastcall TFormDiagTrecho::VerificaLimitesSGrids(void)
{  //variaveis locais
   double   inf, sup;
   bool valido = true;

   //verifica comprimento
   inf = StrToDouble(sgLimites->Cells[colLIMINF][1]);
   sup = StrToDouble(sgLimites->Cells[colLIMSUP][1]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica iadm
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][1]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][1]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq0.r
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][2]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][2]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq0.x
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][3]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][3]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq0.c
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][4]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][4]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq1.r
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][5]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][5]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq1.x
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][6]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][6]);
   if(!VerificaLimites(inf,sup)){valido = false;}
   //verifica seq1.c
   inf = StrToDouble(sgArranjo->Cells[colLIMINF][7]);
   sup = StrToDouble(sgArranjo->Cells[colLIMSUP][7]);
   if(!VerificaLimites(inf,sup)){valido = false;}

   if(!valido){ Aviso("Limites incoerentes, limite superior menor que limite inferior.");}
   return valido;
}

//---------------------------------------------------------------------------


