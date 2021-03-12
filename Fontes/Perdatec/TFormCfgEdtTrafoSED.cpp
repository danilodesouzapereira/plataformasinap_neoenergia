//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCfgEdtTrafoSED.h"
#include "..\Geral\CfgPerdaTec\VTCfgPerdaTec.h"
#include "..\..\DLL_Inc\Funcao.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgEdtTrafoSED::TFormCfgEdtTrafoSED(TComponent* Owner)
   : TForm(Owner)
{   //variaveis locais
   int width = 0;

   //lista com somente o tipo de trafo escolhido
   LisPerdasTrafoTipo  = new TList;
   //booleano pra saber se as mudanças foram salvas
   salvo = false;
   //dimensiona o stringGrid
   SGridTrafo->ColWidths[ncSNOM] = 65;
   SGridTrafo->ColWidths[ncPERDAVAZIO] = 90;
   SGridTrafo->ColWidths[ncPERDATOTAL] = 90;
   //totaliza a largura
   for(int i = 0; i < SGridTrafo->ColCount ; i++)
   {
      width = width + SGridTrafo->ColWidths[i];
   }
   //estipula o tamano do form como sendo o da soma das colunas + o scroll
   this->Width = width + GetSystemMetrics(SM_CXVSCROLL);
}

//---------------------------------------------------------------------------
__fastcall TFormCfgEdtTrafoSED::~TFormCfgEdtTrafoSED(void)
{
   //destroi lista sem destruir os objetos
   if(LisPerdasTrafoTipo){ delete LisPerdasTrafoTipo; LisPerdasTrafoTipo = NULL;}
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::ActionCancelaExecute(TObject *Sender)
{
   this->Close();
}

//---------------------------------------------------------------------------

void __fastcall TFormCfgEdtTrafoSED::ActionConfirmaExecute(TObject *Sender)
{//variaveis locais
   bool falta = false;
   TStringGrid   *SGrid = SGridTrafo;
   AnsiString     msg1, msg2;

   //mensagens de erro
   msg1 = "Existem equipamentos com parâmetros em branco.";
   msg2 = "Eles serão excluídos, deseja continuar?";

   //primeiramente verifica se possui alguma linha com paremetros faltantes
   for(int nl = 0 ; nl < SGrid->RowCount ; nl++)
   {//se faltar um parametro e NÃO for uma linha vazia
      if( FaltaParam(nl) && !IsRowEmpty(nl) ){falta = true;}
   }
   //caso falte mostra mensagem de confirmacao do usuario
   if(falta)
   {
      if( (Confirma(msg1 , msg2, MB_YESNO)) == IDYES )
      {
         SalvaCfg();
         ModalResult = mrOk;
         this->CloseModal();
      }
      else{} //nada
   }
   else
   {
      SalvaCfg();
      ModalResult = mrOk;
      this->CloseModal();
   }

}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::ConcatenaListas(void)
{  //une a nova lista do tipo escolhido, com o resto
   //limpa a lista original;
   LisPerdasTrafo->Clear();

   for(int i = 0; i < LisPerdasTrafoTipo->Count; i++)
   {
      LisPerdasTrafo->Add( LisPerdasTrafoTipo->Items[i] );
   }
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::DestroiItemsList(TList *listaPerda)
{
   PERDA_TRAFO *perda_trafo;

   //destroi todos items da lista
   for(int i = (listaPerda->Count - 1) ; i >= 0 ; i--)
   {
      perda_trafo = (PERDA_TRAFO*)listaPerda->Items[i];
      listaPerda->Delete(i);
      if(perda_trafo) delete perda_trafo;
   }
}

//---------------------------------------------------------------------------
void    __fastcall TFormCfgEdtTrafoSED::Inicia(TList *LisPerdasTrafo)
{   //variáveis locais
   PERDA_TRAFO_SED *perda_trafo;
   TStringGrid     *SGrid = SGridTrafo;

   this->LisPerdasTrafo = LisPerdasTrafo;
   //carrega a lista
   for (int n = 0; n < LisPerdasTrafo->Count; n++)
   {
      perda_trafo = (PERDA_TRAFO_SED*)LisPerdasTrafo->Items[n];
      LisPerdasTrafoTipo->Add(perda_trafo);
   }

   //inicializa as dimensoes do SGrid
   SGrid->ColCount = ncPERDATOTAL + 1;
   SGrid->RowCount = LisPerdasTrafoTipo->Count + 1;
   //Preenche título do SGrid
   SGrid->Cells[ncSNOM ][0] = "Snom(MVA)";
   SGrid->Cells[ncPERDAVAZIO][0] = "Perda Vazio(%)";
   SGrid->Cells[ncPERDATOTAL][0] = "Perda Total(%)";
   //loop p/ todos PERDA_TRAFO em LisPerdas
   for (int n = 0; n < LisPerdasTrafoTipo->Count; n++)
      {
      perda_trafo = (PERDA_TRAFO_SED*)LisPerdasTrafoTipo->Items[n];
      SGrid->Cells[ncSNOM ][n + 1] = DoubleToStr("%2.1f", perda_trafo->snom_mva);
      SGrid->Cells[ncPERDAVAZIO][n + 1] = DoubleToStr("%2.1f",
                                                      perda_trafo->perda_vazio_perc);
      SGrid->Cells[ncPERDATOTAL][n + 1] = DoubleToStr("%2.1f",
                                                      perda_trafo->perda_total_perc);
      }
}

//---------------------------------------------------------------------------
bool __fastcall TFormCfgEdtTrafoSED::FaltaParam(int row)
{  //Variaveis locais
   bool falta = false;
   TStringGrid   *SGrid = SGridTrafo;
   AnsiString     string;

   //Percorre a linha do stringgrid verificando se falta algum parametro
   for(int nc = 0; nc < SGrid->ColCount ; nc++)
   {
      string = SGrid->Cells[nc][row];
      if( string.IsEmpty() )  falta = true;
   }

   return falta;
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::FormCloseQuery(TObject *Sender, bool &CanClose)
{    //variaveis locais
   AnsiString     msg1, msg2;

   //mensagem
   msg1 = "Todas mudanças serão descartadas.";
   msg2 = "Deseja continuar?";
   //se não foram salvas as mudanças
   if(!salvo)
   {
      if(Confirma(msg1,msg2, MB_YESNO) == IDYES) CanClose = true;
      else{CanClose = false;}
   }
}

//---------------------------------------------------------------------------
bool  __fastcall TFormCfgEdtTrafoSED::IsRowEmpty(int row)
{  //Variaveis locais
   bool isRowEmpty = true;
   TStringGrid   *SGrid = SGridTrafo;
   AnsiString      string;

   //Percorre a linha do stringGrid verificando se ela está vazia
   for(int nc = 0; nc < SGrid->ColCount ; nc++)
   {
      string = SGrid->Cells[nc][row];
      if( !string.IsEmpty() )  isRowEmpty = false;
   }
   return isRowEmpty;
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::SalvaCfg(void)
{  //variaveis locais
   TStringGrid   *SGrid = SGridTrafo;
   PERDA_TRAFO_SED *perda_trafo;

   //destroi todos items da lista
   DestroiItemsList(LisPerdasTrafoTipo);
   //cria novos objetos e preenche novamente a lista
   for(int ni = 1 ; ni < SGrid->RowCount; ni++)
   {
      //verifica se a linha está vazia
      if(IsRowEmpty(ni)) continue;
      //verifica se a linha possui espaços vazios
      if(FaltaParam(ni)) continue;
      //cria novo objeto
      perda_trafo = new PERDA_TRAFO_SED;
      /* TODO 1 -oKenndy : Verificar com o Guaraldo se existem outros parametros
      a serem configurados na criação. */
      perda_trafo->snom_mva =       StrToDouble(SGrid->Cells[ncSNOM][ni]);
      perda_trafo->perda_vazio_perc =    StrToDouble(SGrid->Cells[ncPERDAVAZIO][ni]);
      perda_trafo->perda_total_perc =  StrToDouble(SGrid->Cells[ncPERDATOTAL][ni]);
      //adiciona o novo objeto à lista
      LisPerdasTrafoTipo->Add(perda_trafo);
   }
   //une a nova lista com a resto da lista antiga
   ConcatenaListas();
   salvo = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::SGridTrafoSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect)
{   //variaveis locais
   TStringGrid   *SGrid = SGridTrafo;

   //guarda o valor da célula para posterior comparação
   valorAntigo = SGrid->Cells[ACol][ARow];
}

//---------------------------------------------------------------------------
void __fastcall TFormCfgEdtTrafoSED::SGridTrafoSetEditText(TObject *Sender, int ACol,
          int ARow, const UnicodeString Value)
{
   //apenas valida valor
   if(! ValidaValor(Value, ACol)){Aviso("Valor inválido");}
}

//---------------------------------------------------------------------------
bool __fastcall TFormCfgEdtTrafoSED::ValidaValor(AnsiString valor, int col)
{  //variaveis locais
   double    aux;
   //int      fases;
   bool       valido;

   switch (col)
   {
      case ncSNOM:
         valido = StrToDouble(valor, aux);
         return valido;

      case ncPERDAVAZIO:
         valido = StrToDouble(valor, aux);
         return valido;

      case ncPERDATOTAL:
         valido = StrToDouble(valor, aux);
         return valido;

      default:
         return false;
   }
}

//---------------------------------------------------------------------------


