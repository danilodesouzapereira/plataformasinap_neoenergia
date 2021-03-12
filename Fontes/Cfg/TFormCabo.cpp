//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "TFormCabo.h"
#include "..\Apl\VTApl.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCabo::TFormCabo(TComponent* Owner, VTApl *apl, VTCabo *cabo)
                        :TForm(Owner)
   {
   //salva ponteiro p/ objetos
   this->apl       = apl;
   this->cabo_orig = cabo;
   //cria uma c�pia do ojeto
   this->cabo = cabo_orig->Clone();
   }

//---------------------------------------------------------------------------
__fastcall TFormCabo::~TFormCabo(void)
   {
   //destr�i objeto criado
   if (cabo) {delete cabo; cabo = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::ActionAltCfgExecute(TObject *Sender)
   {
   switch(RGroupTipo->ItemIndex)
      {
      case 1: //caboZ0_Z1
      case 2: //caboPRE_REUNIDO
         //esconde GBoxR_GMR e exibe GBoxZ0Z1
         GBoxR_GMR->Visible = false;
         GBoxZ0Z1->Visible  = true;
         break;
      case 0: //caboR_GMR
      default:
         //esconde GBoxZ0Z1 e exibe GBoxR_GMR
         GBoxZ0Z1->Visible  = false;
         GBoxR_GMR->Visible = true;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::ActionConfirmaExecute(TObject *Sender)
   {
   //l� os dados editados
   if (! LeDados()) return;
   //atualiza status indicando que o Cabo foi alterado
   cabo->Status[sttALTERADO] = true;
   //atualiza Cabo original
   cabo_orig->CopiaAtributosDe(*cabo);
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::ExibeDados(void)
   {
   //exibe dados gerais
   EditCodigo->Text = cabo->Codigo;
   EditVmin->Text   = DoubleToStr("%4.3f", cabo->Vmin_KV);
   EditVmax->Text   = DoubleToStr("%4.3f", cabo->Vmax_KV);
   EditIadm->Text   = DoubleToStr("%2.1f", cabo->Iadm_A);
   //exibe dados de r/gmr
   EditR->Text      = DoubleToStr("%4.3f", cabo->r);
   EditGMR->Text    = DoubleToStr("%4.3f", cabo->gmr);
   //exibe SGrid c/ imped�ncias de seq��ncia
   ExibeDadosZ0_Z1();
   //seleciona tipo de configura��o
   RGroupTipo->ItemIndex = cabo->TipoCfg;
   //exibe/esconde conjunto de dados de acordo c/ tipo de configura��o
   ActionAltCfgExecute(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::ExibeDadosZ0_Z1(void)
   {
   //define dimens�o de SGridZ
   SGridZ->RowCount = 3;
   SGridZ->ColCount = 4;
   SGridZ->DefaultColWidth = 60;
   //preenche t�tulos das colunas
   SGridZ->Cells[0][0] = "";
   SGridZ->Cells[1][0] = "r(ohm/km)";
   SGridZ->Cells[2][0] = "x(ohm/km)";
   SGridZ->Cells[3][0] = "c(nF/km)";
   //preenche linha de Seq.0
   SGridZ->Cells[0][1] = "Seq.0";
   SGridZ->Cells[1][1] = DoubleToStr("%5.4f", cabo->z0.r);
   SGridZ->Cells[2][1] = DoubleToStr("%5.4f", cabo->z0.x);
   SGridZ->Cells[3][1] = DoubleToStr("%5.4f", cabo->c0);
   //preenche linha de Seq.1
   SGridZ->Cells[0][2] = "Seq.1";
   SGridZ->Cells[1][2] = DoubleToStr("%5.4f", cabo->z1.r);
   SGridZ->Cells[2][2] = DoubleToStr("%5.4f", cabo->z1.x);
   SGridZ->Cells[3][2] = DoubleToStr("%5.4f", cabo->c1);
   }

//---------------------------------------------------------------------------
VTCabo* __fastcall TFormCabo::ExisteCabo(AnsiString codigo)
   {
   //vari�veis locais
   VTCabo     *um_cabo;
   VTArranjos *arranjos = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
   TList      *lisCABO  = arranjos->LisCabo();

   //procura Cabo c/ o c�digo indicado
   for (int n = 0; n < lisCABO->Count; n++)
      {
      um_cabo = (VTCabo*)lisCABO->Items[n];
      //desconsidera o Cabo sendo editado
      if (um_cabo == cabo_orig) continue;
      if (um_cabo->Codigo.AnsiCompareIC(codigo) == 0) return(um_cabo);
      }
   return(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCabo::FormShow(TObject *Sender)
   {
   //exibe dados
   ExibeDados();
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCabo::LeDados(void)
   {
   try{//C�digo
      cabo->Codigo = EditCodigo->Text;
      if (cabo->Codigo.IsEmpty())
         {
         Erro("Defina o c�digo do cabo");
         return(false);
         }
      //verifica se existe outro Cabo c/ mesmo c�digo
      if (ExisteCabo(cabo->Codigo) != NULL)
         {
         Erro("Existe outro cabo com o c�digo definido");
         return(false);
         }
      //tens�es m�nima e m�xima
      cabo->Vmin_KV = StrToDouble(EditVmin->Text);
      cabo->Vmax_KV = StrToDouble(EditVmax->Text);
      if (cabo->Vmin_KV < 0) cabo->Vmin_KV = 0;
      if (cabo->Vmax_KV < 0) cabo->Vmax_KV = 0;
      if (cabo->Vmin_KV > cabo->Vmax_KV)
         {
         Erro("Tens�o m�nima do cabo � maior que a tens�o m�xima");
         return(false);
         }
      //corrente admiss�vel
      cabo->Iadm_A = StrToDouble(EditIadm->Text);
      if (cabo->Iadm_A <= 0.)
         {
         Erro("Corrente admiss�vel c/ valor inv�lido");
         return(false);
         }
      //determina tipo de configura��o
      cabo->TipoCfg = RGroupTipo->ItemIndex;
      if (cabo->TipoCfg < 0)
         {
         Erro("Selecione o tipo de configura��o do cabo");
         return(false);
         }
      //l� conjunto de dados mutuamente exclusivo
      if (cabo->TipoCfg == caboR_GMR)
         {//l� R e GMR
         if (! LeDadosR_GMR()) return(false);
         }
      else
         {//l� Z0 e Z1
         if (! LeDadosZ0_Z1()) return(false);
         }
      }catch(Exception &e)
         {
         return(false);
         }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCabo::LeDadosR_GMR(void)
   {
   //resist�ncia
   cabo->r = StrToDouble(EditR->Text);
   if (cabo->r <= 0.)
      {
      Erro("Resist�ncia c/ valor inv�lido");
      return(false);
      }
   //raio m�dio geom�trico
   cabo->gmr = StrToDouble(EditGMR->Text);
   if (cabo->gmr <= 0.)
      {
      Erro("Raio m�dio geom�trico c/ valor inv�lido");
      return(false);
      }
   return(true);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCabo::LeDadosZ0_Z1(void)
   {
   //vari�veis locais
   double c0, c1;
   strIMP z0, z1;
   
   try{//l� Z0
      z0.r = StrToDouble(SGridZ->Cells[1][1]);
      z0.x = StrToDouble(SGridZ->Cells[2][1]);
      c0   = StrToDouble(SGridZ->Cells[3][1]);
      }catch(Exception &e)
         {
         Erro("Imped�ncia de seq.0 c/ valor inv�lido");
         return(false);
         }
   try{//l� Z1
      z1.r = StrToDouble(SGridZ->Cells[1][2]);
      z1.x = StrToDouble(SGridZ->Cells[2][2]);
      c1   = StrToDouble(SGridZ->Cells[3][2]);
      }catch(Exception &e)
         {
         Erro("Imped�ncia de seq.1 c/ valor inv�lido");
         return(false);
         }
   if ((z0.r <= 0.)&&(z0.x <= 0.))
      {
      Erro("Imped�ncia de seq.0 c/ valor nulo ou negativo");
      return(false);
      }
   if ((z1.r <= 0.)&&(z1.x <= 0.))
      {
      Erro("Imped�ncia de seq.1 c/ valor nulo ou negativo");
      return(false);
      }
   //atualiza cabo
   cabo->z0 = z0;
   cabo->c0 = c0;
   cabo->z1 = z1;
   cabo->c1 = c1;
   return(true);
   }

//---------------------------------------------------------------------------
//eof


