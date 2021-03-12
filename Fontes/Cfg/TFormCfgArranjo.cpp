//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <DateUtils.hpp>
#include "TFormCfgArranjo.h"
#include "TAplCfg.h"
#include "TFormArranjo.h"
#include "TFormCabo.h"
#include "TFormSuporte.h"
#include "TFormVisualizarZ0Z1.h"
#include "TImportaArranjoZ0Z1.h"
#include "..\Arranjo\VTArranjo.h"
#include "..\Arranjo\VTArranjos.h"
#include "..\Arranjo\VTCabo.h"
#include "..\Arranjo\VTSuporte.h"
//#include "..\BD\VTBD.h"
//#include "..\BD\Rede\VTBDGeral.h"
//#include "..\BD\VTSNPDBConn.h"
#include "..\Diretorio\VTPath.h"
#include "..\Geral\VTGeral.h"
#include "..\InterfaceTXT\Exporta\VTExportaTXT.h"
#include "..\InterfaceTXT\Importa\VTImportaTXT.h"
#include "..\InterfaceXML\VTExportaXML.h"
#include "..\InterfaceXML\VTImportaXML.h"
#include "..\Licenca\VTLicenca.h"
#include "..\Licenca\VTLicencas.h"
#include "..\MontaZ\VTMontaZ.h"
#include "..\Planejamento\VTEstudo.h"
#include "..\Planejamento\VTPlanejamento.h"
#include "..\Progresso\VTProgresso.h"
#include "..\..\DLL_Inc\Arranjo.h"
//#include "..\..\DLL_Inc\BD.h"
#include "..\..\DLL_Inc\Funcao.h"
#include "..\..\DLL_Inc\InterfaceTXT.h"
#include "..\..\DLL_Inc\InterfaceXML.h"
#include "..\..\DLL_Inc\Progresso.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormCfgArranjo::TFormCfgArranjo(TForm *form_owner, VTApl *apl_owner, VTApl *apl_child)
   : TForm(Owner)
   {
   try{//cria  objeto AplCfg
      apl = new TAplCfg(this, apl_owner);
      //salva ponteiro
      this->apl_child = apl_child;
      //cria listas
      lisEQP = new TList();
      //salva ponteiro de objetos
		arranjos  = (VTArranjos*)apl->GetObject(__classid(VTArranjos));
      //define ordena��o do ListView pelo c�digo da Rede
      ListViewSort.ColumnIndex    = 0;
      ListViewSort.OrdemCrescente = true;
      //l� base de dados e exibe dados
      apl->LeBaseArranjo();
      ExibeDados();
      //habilita/desabilita comando em fun��o da Licen�a
      //VerificaModulos();
      }catch(Exception &e)
         {
         }
   }

//---------------------------------------------------------------------------
__fastcall TFormCfgArranjo::~TFormCfgArranjo(void)
   {
   //destr�i lista sem destruir seus objetos
   if (lisEQP) {delete lisEQP; lisEQP = NULL;}
   //destr�i objetos
   if (apl) {delete apl; apl = NULL;}
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionCancelaExecute(TObject *Sender)
   {
   //fecha o form
   ModalResult = mrCancel;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionConfirmaExecute(TObject *Sender)
   {
   //calcula MatZ de todos Arranjos que foram alterados (ou cujos Suporte/Cabo tenham sido alterados)
   CalculaMatZ();
   //atualiza base de dados
   AtualizaBaseDado();
   //fecha o form
   ModalResult = mrOk;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionEdtArranjoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTArranjo    *arranjo;
   TFormArranjo *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Arranjo selecionado
   if (LViewArranjo->Selected == NULL) return;
   if ((arranjo = (VTArranjo*)LViewArranjo->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   try{//cria TFormArranjo como janela modal
      form = new TFormArranjo(this, apl, arranjo);
      if (form->ShowModal() == mrOk)
         {//atualiza LViewArranjo
         ExibeDadosArranjo();
         }
      delete form;
      }catch (Exception &e)
         {
         }
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionEdtCaboExecute(TObject *Sender)
   {
   //vari�veis locais
   VTCabo    *cabo;
   TFormCabo *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Cabo selecionado
   if (LViewCabo->Selected == NULL) return;
   if ((cabo = (VTCabo*)LViewCabo->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   try{//cria TFormCabo como janela modal
      form = new TFormCabo(this, apl, cabo);
      if (form->ShowModal() == mrOk)
         {//atualiza LViewCabo
         ExibeDadosCabo();
         }
      delete form;
      }catch (Exception &e)
         {
         }
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionEdtSuporteExecute(TObject *Sender)
   {
   //vari�veis locais
   VTSuporte    *suporte;
   TFormSuporte *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Suporte selecionado
   if (LViewSuporte->Selected == NULL) return;
   if ((suporte = (VTSuporte*)LViewSuporte->Selected->Data) == NULL) return;
   //esconde este form
   Visible = false;
   try{//cria TFormSuporte como janela modal
      form = new TFormSuporte(this, apl, suporte);
      //verifica se o Suporte � usado em algum Arranjo
      if (ExisteArranjo(suporte, lisEQP) > 0)
         {//bloqueia altera��o de n�mero de PtoFixacao
         form->CBoxNumPto->Enabled = false;
         }
      if (form->ShowModal() == mrOk)
         {//atualiza LViewSuporte
         ExibeDadosSuporte();
         }
      delete form;
      }catch (Exception &e)
         {
         }
   //reapresenta este form
   Visible = true;
   }

////---------------------------------------------------------------------------
//void __fastcall TFormCfgArranjo::ActionExportaTXTExecute(TObject *Sender)
//   {
//   //vari�veis locais
//   VTExportaTXT *exportaTXT;
//   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));
//
//   //esconde este Form
//   Hide();
//   //inicia diret�rio de OpenDialog
//   SaveDialog->Filter = "Arquivos Texto (*.txt)|*.txt|Todos Arquivos (*.*)|*.*";
//   if (SaveDialog->InitialDir.IsEmpty())  SaveDialog->InitialDir = path->DirExporta();
//   //exibe OpenDialog
//   if (SaveDialog->Execute())
//      {//executa importa��o dos arranjos a partir do arquivo
//      exportaTXT = DLL_NewObjExportaTXT(apl);
//      exportaTXT->ExportaArranjos(SaveDialog->FileName);
//      delete exportaTXT;
//      }
//   //reapresenta este Form
//   Show();
//   }
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionExportaTXTExecute(TObject *Sender)
   {
   //vari�veis locais
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   //inicia diret�rio de OpenDialog
   SaveDialog->Filter = "Arquivos Texto (*.txt)|*.txt|Todos Arquivos (*.*)|*.*";
   if (SaveDialog->InitialDir.IsEmpty())  SaveDialog->InitialDir = path->DirExporta();
   //exibe OpenDialog
   if (SaveDialog->Execute())
      {//executa importa��o dos arranjos a partir do arquivo

		ExportaArranjosCabosSuporte(SaveDialog->FileName);
      }
   //reapresenta este Form
   Show();
   }
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionExportaXMLExecute(TObject *Sender)
   {
   //vari�veis locais
   VTExportaXML *exportaXML;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   //inicia diret�rio de OpenDialog
   SaveDialog->Filter = "Arquivos XML (*.xml)|*.xml|Todos Arquivos (*.*)|*.*";
   if (SaveDialog->InitialDir.IsEmpty())  SaveDialog->InitialDir = path->DirExporta();
   //exibe OpenDialog
   if (SaveDialog->Execute())
      {//executa importa��o dos arranjos a partir do arquivo
      exportaXML = DLL_NewObjExportaXML(apl);
      exportaXML->ExportaArranjo     = true;
      exportaXML->ExportaTipoChave   = false;
      exportaXML->ExportaTipoRede    = false;
      exportaXML->ExportaRede        = false;
      exportaXML->ExportaCurvaTipica = false;
      exportaXML->ExportaReducao     = false;
      exportaXML->ExportaArranjos(SaveDialog->FileName);
      delete exportaXML;
      }
   //reapresenta este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionImportaTXTExecute(TObject *Sender)
   {
   //vari�veis locais
   VTImportaTXT *importaTXT;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   //inicia diret�rio de OpenDialog
   if (OpenDialog->InitialDir.IsEmpty())  OpenDialog->InitialDir = path->DirImporta();
   //configura filtro
   OpenDialog->Filter = "Arquivos TXT (*.txt)|*.txt|Todos Arquivos (*.*)|*.*";
   //exibe OpenDialog
   if (OpenDialog->Execute())
      {//executa importa��o dos arranjos a partir do arquivo
      importaTXT = DLL_NewObjImportaTXT(apl);
      importaTXT->ImportaArranjos(OpenDialog->FileName);
      delete importaTXT;
      //atualiza dados
      ExibeDados();
      }
   //reapresenta este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionImportaXMLExecute(TObject *Sender)
   {
   //vari�veis locais
   VTImportaXML *importaXML;
   VTPath       *path = (VTPath*)apl->GetObject(__classid(VTPath));

   //esconde este Form
   Hide();
   //inicia diret�rio de OpenDialog
   if (OpenDialog->InitialDir.IsEmpty())  OpenDialog->InitialDir = path->DirImporta();
   //configura filtro
   OpenDialog->Filter = "Arquivos XML (*.xml)|*.xml|Todos Arquivos (*.*)|*.*";
   //exibe OpenDialog
   if (OpenDialog->Execute())
      {//executa importa��o dos arranjos a partir do arquivo
      importaXML = DLL_NewObjImportaXML(apl);
      importaXML->ImportaArranjos(OpenDialog->FileName);
      delete importaXML;
      //atualiza dados
      ExibeDados();
      }
   //reapresenta este Form
   Show();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionInsArranjoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTArranjo    *arranjo;
   TFormArranjo *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   try{//cria um novo Arranjo
      arranjo = DLL_NewObjArranjo(NULL);
      arranjo->Status[sttNOVO] = true;
      //esconde este form
		Visible = false;
		//cria TFormArranjo como janela modal
		form = new TFormArranjo(this, apl, arranjo);
		if (form->ShowModal() == mrOk)
			{//insere novo Arranjo em Arranjos
			arranjos->InsereArranjo(arranjo);
			//atualiza LViewArranjo
			ExibeDadosArranjo();
			arranjo = NULL;
			}
		}catch (Exception &e)
			{
			}
	//destr�i objetos criados
	if (arranjo) delete arranjo;
	if (form)    delete form;
	//reapresenta este form
	Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionInsCaboExecute(TObject *Sender)
   {
   //vari�veis locais
   VTCabo    *cabo;
   TFormCabo *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   try{//cria um novo Cabo
      cabo = DLL_NewObjCabo();
      cabo->Status[sttNOVO] = true;
      //esconde este form
      Visible = false;
      //cria TFormCabo como janela modal
      form = new TFormCabo(this, apl, cabo);
      if (form->ShowModal() == mrOk)
         {//insere novo Cabo em Arranjos
         arranjos->InsereCabo(cabo);
         //atualiza LViewCabo
         ExibeDadosCabo();
         cabo = NULL;
         }
      }catch (Exception &e)
         {
         }
   //destr�i objetos criados
   if (cabo) delete cabo;
   if (form) delete form;
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionInsSuporteExecute(TObject *Sender)
   {
   //vari�veis locais
   VTSuporte    *suporte;
   TFormSuporte *form;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   try{//cria um novo Suporte
      suporte = DLL_NewObjSuporte();
      suporte->Status[sttNOVO] = true;
      //esconde este form
      Visible = false;
      //cria TFormSuporte como janela modal
      form = new TFormSuporte(this, apl, suporte);
      if (form->ShowModal() == mrOk)
         {//insere novo Suporte em Suportes
         arranjos->InsereSuporte(suporte);
         //atualiza LViewSuporte
         ExibeDadosSuporte();
         suporte = NULL;
         }
      }catch (Exception &e)
         {
         }
   //destr�i objetos criados
   if (suporte) delete suporte;
   if (form)    delete form;
   //reapresenta este form
   Visible = true;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionRetArranjoExecute(TObject *Sender)
   {
   //vari�veis locais
   VTArranjo *arranjo;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Arranjo selecionado
   if (LViewArranjo->Selected == NULL) return;
   if ((arranjo = (VTArranjo*)LViewArranjo->Selected->Data) == NULL) return;
   //pede confirma��o do usu�rio
   if (Confirma("Arranjo " + AnsiQuotedStr(arranjo->Codigo, '\''),
       "Confirma exclus�o do arranjo ?") != IDYES) return;
   //marca arranjo como exclu�do
   arranjo->Status[sttEXCLUIDO] = true;
   arranjo->Status[sttALTERADO] = true;
   //atualiza LViewArranjo
   ExibeDadosArranjo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionRetCaboExecute(TObject *Sender)
   {
   //vari�veis locais
   int       num_arranjo_excluido = 0 ;
   TList     *lisARR;
   VTArranjo *arranjo;
   VTCabo    *cabo;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Cabo selecionado
   if (LViewCabo->Selected == NULL) return;
   if ((cabo = (VTCabo*)LViewCabo->Selected->Data) == NULL) return;
   //pede confirma��o do usu�rio
   if (Confirma("Cabo " + AnsiQuotedStr(cabo->Codigo, '\''),
       "Confirma exclus�o do cabo ?") != IDYES) return;
   //marca arranjo como exclu�do
   cabo->Status[sttEXCLUIDO] = true;
   cabo->Status[sttALTERADO] = true;
   //exclui todos Arranjos que utilizam o Cabo exclu�do
   lisARR = arranjos->LisArranjo();
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      if (ArranjoUtilizaCabo(arranjo, cabo))
         {//marca arranjo como exclu�do
         arranjo->Status[sttEXCLUIDO] = true;
         arranjo->Status[sttALTERADO] = true;
         num_arranjo_excluido++;
         }
      }
   //atualiza LViewCabo
   ExibeDadosCabo();
   //atualiza LViewArranjo
   if (num_arranjo_excluido > 0) ExibeDadosArranjo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionRetSuporteExecute(TObject *Sender)
   {
   //vari�veis locais
   AnsiString msg;
   VTArranjo *arranjo;
   VTSuporte *suporte;

   //verifica se n�o tem estudo do planejamento aberto
   if(ExistePlanejamento()) return;
   //determina Suporte selecionado
   if (LViewSuporte->Selected == NULL) return;
   if ((suporte = (VTSuporte*)LViewSuporte->Selected->Data) == NULL) return;
   //determina n�mero de Arranjos que usam este Suporte
   ExisteArranjo(suporte, lisEQP);
   //pede confirma��o do usu�rio
   if (lisEQP->Count > 0) msg.sprintf(" (utilizado por %d arranjos)", lisEQP->Count);
   if (Confirma("Suporte " + AnsiQuotedStr(suporte->Codigo, '\'') + msg,
       "Confirma exclus�o do suporte ?") != IDYES) return;
   //marca Suporte como exclu�do
   suporte->Status[sttEXCLUIDO] = true;
   suporte->Status[sttALTERADO] = true;
   //exclui todos Arranjos que utilizam o Suporte exclu�do
   for (int n = 0; n < lisEQP->Count; n++)
      {
      arranjo = (VTArranjo*)lisEQP->Items[n];
      //marca arranjo como exclu�do
      arranjo->Status[sttEXCLUIDO] = true;
      arranjo->Status[sttALTERADO] = true;
      }
   //atualiza LViewSuporte
   ExibeDadosSuporte();
   //atualiza LViewArranjo
   if (lisEQP->Count > 0) ExibeDadosArranjo();
   }

/*
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionSalvaOracleExecute(TObject *Sender)
   {
   //vari�veis locais
   bool         sucesso;
   VTBD*        bdRemoto = NULL;
   VTProgresso* progresso = DLL_NewObjProgresso(this, NULL);

   if (!progresso) return;

   //calcula MatZ de todos Arranjos que foram alterados (ou cujos Suporte/Cabo tenham sido alterados)
   CalculaMatZ();
   //define Status novo p/ todos Arranjo, Cabo e Suporte
   DefineStatus(arranjos->LisArranjo(), sttNOVO, true);
   DefineStatus(arranjos->LisCabo(),    sttNOVO, true);
   DefineStatus(arranjos->LisSuporte(), sttNOVO, true);

   // Cria uma conex�o exclusiva para o Oracle. Simplesmente chavear para o remoto pode
   // causar problemas no driver, se n�o desconectar. O objeto extra, local, garante que
   // n�o ficam pend�ncias.
   // N�o estou usando mais o bd do apl
   // VTBD* bd = (VTBD*)apl->GetObject(__classid(VTBD));
   try
      {
      // Cria o objeto localmente
      bdRemoto = DLL_NewObjBD(apl);
      // Conecta ao banco remoto
      if (bdRemoto->ConectaServidorRedeRemoto())
         {
         progresso->ShowBarra = false;
         progresso->Show("Execu��o em andamento.", 3 ,10);
         sucesso = bdRemoto->Geral->SalvaArranjo();
         progresso->Hide();
         delete progresso;
         progresso = NULL;
         this->Refresh();

         if (sucesso)
            {
            //cancela Status novo p/ todos Arranjo, Cabo e Suporte
            DefineStatus(arranjos->LisArranjo(), sttNOVO, false);
            DefineStatus(arranjos->LisCabo(),    sttNOVO, false);
            DefineStatus(arranjos->LisSuporte(), sttNOVO, false);

            Aviso("Arranjos, suportes e cabos salvos com sucesso.");
            }
         else
            {
            Aviso("Erro ao salvar os arranjos, suportes e/ou cabos.");
            }
         // Desconecta do banco (for�ando um Commit)
         bdRemoto->Desconecta();
         }
      else
         {
         Aviso("Erro na conex�o com a base de dados.");
         }
      // Elimina o objeto criado
      delete bdRemoto;
      bdRemoto = NULL;
      }
   catch (Exception &e)
      {
      Aviso("Erro ao salvar os arranjos, suportes e/ou cabos. Falha na cria��o da conex�o.");
      }
   }
*/
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ActionVisualizarZ0Z1Execute(TObject *Sender)
	{
	//variaveis locais
	TFormVisualizarZ0Z1 *form;

	//esconde este form
	Visible = false;
	//cria TFormVisualizarZ0Z1 como janela modal
	form = new TFormVisualizarZ0Z1(NULL, arranjos, apl->Owner);
	form->ShowModal();
	//destr�i form
	if (form)    delete form;
	//reapresenta este form
	Visible = true;
	}

//---------------------------------------------------------------------------
bool __fastcall TFormCfgArranjo::ArranjoAlterado(VTArranjo *arranjo)
   {
   //vari�veis locais
   VTCabo *cabo;

   //verifica o tipo de configura��o do Arranjo
   switch(arranjo->TipoCfg)
      {
      case arranjoMAT_Z: //n�o utiliza Suporte ou Cabos
         return(false);
      case arranjoZ0Z1: //n�o utiliza Suporte ou Cabos
         return(false);
      case arranjoCABO_Z0Z1:
         //verifica se o caboZ0Z1 foi alterado
         if ((cabo = arranjo->caboZ0Z1) == NULL) return(false);
         return(cabo->Status[sttALTERADO]);
      case arranjoCABOxFASE:
         //verifica se o Suporte foi alterado
         if (arranjo->suporte->Status[sttALTERADO]) return(true);
         //verifica se algum Cabo foi alterado
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            if ((cabo = arranjo->fase[nf].cabo) == NULL) continue;
            if (cabo->Status[sttALTERADO]) return(true);
            }
         return(false);
      }
   return(false);
   }
//---------------------------------------------------------------------------
bool __fastcall TFormCfgArranjo::ArranjoUtilizaCabo(VTArranjo *arranjo, VTCabo *cabo)
   {
   //verifica o tipo de configura��o do Arranjo
   switch (arranjo->TipoCfg)
      {
      case arranjoCABO_Z0Z1:
            if (arranjo->caboZ0Z1 == cabo) return(true);
            break;
      case arranjoCABOxFASE:
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            if (arranjo->fase[nf].cabo == cabo) return(true);
            }
         break;
      }
   return(false);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::AtualizaBaseDado(void)
   {
   apl->SalvaBaseArranjo();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::CalculaMatZ(void)
   {
   //vari�veis locais
   VTArranjo *arranjo;
   TList     *lisARR = arranjos->LisArranjo();
   VTMontaZ  *montaZ = (VTMontaZ*)apl->GetObject(__classid(VTMontaZ));

   //loop p/ todos os Arranjos
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo = (VTArranjo*)lisARR->Items[n];
      //verifica se o Suporte ou os Cabos do Arranjo foram alterados
      if (ArranjoAlterado(arranjo))
         {//recalcula MatZ do Arranjo
         montaZ->Executa(arranjo);
         }
      }
   }
//---------------------------------------------------------------------------
int __fastcall TFormCfgArranjo::CompareNum(AnsiString S1, AnsiString S2)
   {
   //variaveis locais
   double   D1=0.0, D2=0.0;

   try{
      D1 = StrToDouble(S1);
      D2 = StrToDouble(S2);
      if(D1 > D2)          return(1);
      else if (D1 < D2)    return(-1);
      else                 return(0);
      }
   catch(Exception &e){
      //retorna 0
      return(0);
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::DefineStatus(TList *lisEQP, int bit_stt, bool enabled)
   {
   VTEqpto *eqpto;

   for (int n = 0; n < lisEQP->Count; n++)
      {
      eqpto = (VTEqpto*)lisEQP->Items[n];
      if (eqpto->Codigo.Trim().IsEmpty())
        {
        eqpto->Codigo = "???????";
        }

      eqpto->Status[bit_stt] = enabled;
      }
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ExibeDados(void)
   {
   //exibe dados de Cabos
   ExibeDadosCabo();
   //exibe dados de Suportes
   ExibeDadosSuporte();
   //exibe dados de Arranjos
   ExibeDadosArranjo();
   //seleciona TableSheet
   PageControl->ActivePage = TabSheetArranjo;
   PageControlChange(NULL);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ExibeDadosArranjo(void)
   {
   //vari�veis locais
   AnsiString ShortDateFormatOriginal;
   AnsiString txt;
   char       *vet_fase[] = {"A", "B", "C", "N"};
   VTArranjo  *arranjo;
   TListItem  *item;
   TList      *lisARR = arranjos->LisArranjo();

   //salva ShortDateFormat;
   ShortDateFormatOriginal = FormatSettings.ShortDateFormat;
   //define formato p/ data
   FormatSettings.ShortDateFormat = "dd/mm/yyyy";
   //reinicia LView
   LViewArranjo->Clear();
   LViewArranjo->SortType = (TSortType)0;//stNone;
   //preenche LView
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo      = (VTArranjo*)lisARR->Items[n];
      //descarta Arranjo excluido
      if (arranjo->Status[sttEXCLUIDO]) continue;
      item          = LViewArranjo->Items->Add();
      item->Data    = arranjo;
      item->Caption = DateToStr(arranjo->Data);
      item->SubItems->Add(arranjo->Codigo);
      //configura��o
      switch (arranjo->TipoCfg)
         {
         case arranjoCABOxFASE: txt = "Cabos/Suporte";     break;
         case arranjoCABO_Z0Z1: txt = "Cabo Multiplexado"; break;
         case arranjoZ0Z1:      txt = "Z0/Z1 (ohm/km)";    break;
         case arranjoMAT_Z:     txt = "Matriz Z";          break;
         case arranjoPU_SB100:  txt = "Z0/Z1 (pu/Sb100)";  break;
         default:               txt = "indefinido";        break;
         }
      item->SubItems->Add(txt);
      //local (a�reo, subterr�neo, submerso)
      //item->SubItems->Add(arranjo->TipoDeArranjo());
      //corrente admiss�vel
      item->SubItems->Add(DoubleToStr("%2.1f", arranjo->Iadm_amp));
		//Suporte ou Ramal de liga��o
		if      (arranjo->ramal)   txt = "(*) ramal de liga��o";
		else if (arranjo->suporte) txt = arranjo->suporte->Codigo;
		else                       txt = "-";
      item->SubItems->Add(txt);
      //Cabos
      txt = "";
      if (arranjo->TipoCfg == arranjoCABO_Z0Z1)
         {
         txt = arranjo->caboZ0Z1->Codigo;
         }
      else if (arranjo->TipoCfg == arranjoCABOxFASE)
         {
         for (int nf = 0; nf < MAX_FASE; nf++)
            {
            if (arranjo->fase[nf].cabo)
               {
               if (! txt.IsEmpty()) txt  += "/";
               txt += AnsiString(vet_fase[nf]) + ":" + arranjo->fase[nf].cabo->Codigo;
               }
            }
         }
      item->SubItems->Add(txt);
      }
   //ordena LView
   //LViewArranjo->SortType = (TSortType)2;//stText;
   //pr�-seleciona primeiro item
   if (LViewArranjo->Items->Count > 0) LViewArranjo->ItemIndex = 0;
   //restaura ShortDateFormat;
   FormatSettings.ShortDateFormat = ShortDateFormatOriginal;
   //exibe n�mero de arranjos em StatusBarArranjo
   StatusBarArranjo->SimpleText = "N�mero de Arranjos: " + IntToStr(LViewArranjo->Items->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ExibeDadosCabo(void)
   {
   //vari�veis locais
   VTCabo     *cabo;
   TListItem  *item;
   TList      *lisCAB = arranjos->LisCabo();

   //reinicia LView
   LViewCabo->Clear();
   //preenche LView
   for (int n = 0; n < lisCAB->Count; n++)
      {
      cabo          = (VTCabo*)lisCAB->Items[n];
      //descarta Cabo excluido
      if (cabo->Status[sttEXCLUIDO]) continue;
      item          = LViewCabo->Items->Add();
      item->Data    = cabo;
      item->Caption = cabo->Codigo;
      //tipo
      if      (cabo->TipoCfg == caboR_GMR)       item->SubItems->Add("r+GMR");
      else if (cabo->TipoCfg == caboZ0Z1)        item->SubItems->Add("Zseq");
      else if (cabo->TipoCfg == caboPRE_REUNIDO) item->SubItems->Add("pr�-reunido");
      else                                       item->SubItems->Add("");
      //Iadm, Vmin, Vmax
      item->SubItems->Add(DoubleToStr("%2.1f", cabo->Iadm_A));
      //item->SubItems->Add(DoubleToStr("%4.3f", cabo->Vmin_KV));
      //item->SubItems->Add(DoubleToStr("%4.3f", cabo->Vmax_KV));
      //R, GMR
      if (cabo->TipoCfg == caboR_GMR)
         {
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->r));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->gmr));
         }
      else
         {
         for (int n = 1; n <= 2; n++) {item->SubItems->Add("");}
         }
      //R0, X0, R1, X1
      if ((cabo->TipoCfg == caboZ0Z1)||(cabo->TipoCfg == caboPRE_REUNIDO))
         {
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->z0.r));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->z0.x));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->c0));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->z1.r));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->z1.x));
         item->SubItems->Add(DoubleToStr("%4.3f", cabo->c1));
         }
      else
         {
         for (int n = 1; n <= 6; n++) {item->SubItems->Add("");}
         }
      }
   //pr�-seleciona primeiro item
   if (LViewCabo->Items->Count > 0) LViewCabo->ItemIndex = 0;
   //exibe n�mero de arranjos em StatusBarArranjo
   StatusBarCabo->Panels->Items[0]->Text = "N�mero de Cabos: " + IntToStr(LViewCabo->Items->Count);
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ExibeDadosSuporte(void)
   {
   //vari�veis locais
   VTSuporte  *suporte;
   TListItem  *item;
   TList      *lisSUP = arranjos->LisSuporte();
   TList      *lisFIX;

   //reinicia LView
   LViewSuporte->Clear();
   //preenche LView
   for (int n = 0; n < lisSUP->Count; n++)
      {
      suporte      = (VTSuporte*)lisSUP->Items[n];
      //descarta Suporte excluido
      if (suporte->Status[sttEXCLUIDO]) continue;
      item          = LViewSuporte->Items->Add();
      item->Data    = suporte;
      item->Caption = suporte->Codigo;
      //tipo de suporte
      item->SubItems->Add(suporte->TipoDeSuporteAsString());
      //Vmin, Vmax
      //item->SubItems->Add(DoubleToStr("%4.3f", suporte->Vmin_KV));
      //item->SubItems->Add(DoubleToStr("%4.3f", suporte->Vmax_KV));
      //n�mero de pontos de fixa��o
      lisFIX = suporte->LisPtoFixacao();
      item->SubItems->Add(IntToStr(lisFIX->Count));
      }
   //pr�-seleciona primeiro item
   if (LViewSuporte->Items->Count > 0) LViewSuporte->ItemIndex = 0;
   //exibe n�mero de suportes em StatusBarSuporte
   StatusBarSuporte->SimpleText = "N�mero de Suportes: " + IntToStr(LViewSuporte->Items->Count);
   }

//---------------------------------------------------------------------------
int __fastcall TFormCfgArranjo::ExisteArranjo(VTSuporte *suporte, TList *lisEXT)
   {
   //vari�veis locais
   VTArranjo *arranjo;
   TList     *lisARR;

   //reinicia lisEXT
   lisEXT->Clear();
   //detemina Arranjos que utilizam o Suporte indicado
   lisARR = arranjos->LisArranjo();
   for (int n = 0; n < lisARR->Count; n++)
      {
      arranjo  = (VTArranjo*)lisARR->Items[n];
      if (arranjo->suporte == suporte) lisEXT->Add(arranjo);
      }
   return(lisEXT->Count);
   }

//---------------------------------------------------------------------------
bool __fastcall TFormCfgArranjo::ExistePlanejamento(void)
   {
   //vari�veis locais
   VTPlanejamento *planejamento;
   VTEstudo       *estudo;

   //prote��o
   if(apl_child == NULL)    return(false);
   planejamento = (VTPlanejamento*)apl_child->GetObject(__classid(VTPlanejamento));
   if(planejamento == NULL) return(false);
   if((estudo = planejamento->ExisteEstudo()) == NULL) return(false);
   //verifica se o estudo � algum tipo de planejamento
   if(estudo->Tipo != estudoREDE_ORIGINAL)
      {
      Aviso("Fun��o bloqueada dentro do m�dulo de Planejamento");
      return(true);
      }
   return(false);
   }
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::ExportaArranjosCabosSuporte(AnsiString path)
{
	TStringList *strings;
	AnsiString linhaBranco = ";;;;;;;;;;;";
	AnsiString linha;
	TListItem *item;
	UnicodeString filepath;

	strings = new TStringList;
	try {
        //exporta arranjos
		linha = "Arranjos" + linhaBranco;
		strings->Add(linha);
		strings->Add(linhaBranco);
		strings->Add(LinhaCsvHeader(LViewArranjo));
		for (int ni = 0 ; ni < LViewArranjo->Items->Count; ni++)
		{
			item = LViewArranjo->Items->Item[ni];
			strings->Add(LinhaCsvListItem(item));
		}
		strings->Add(linhaBranco);
		//exporta cabos
		linha = "Cabos" + linhaBranco;
		strings->Add(linha);
		strings->Add(linhaBranco);
		strings->Add(LinhaCsvHeader(LViewCabo));
		for (int ni = 0 ; ni < LViewCabo->Items->Count; ni++)
		{
			item = LViewCabo->Items->Item[ni];
			strings->Add(LinhaCsvListItem(item));
		}
		strings->Add(linhaBranco);
		//exporta suportes
		linha = "Suportes" + linhaBranco;
		strings->Add(linha);
		strings->Add(linhaBranco);
		strings->Add(LinhaCsvHeader(LViewSuporte));
		for (int ni = 0 ; ni < LViewSuporte->Items->Count; ni++)
		{
			item = LViewSuporte->Items->Item[ni];
			strings->Add(LinhaCsvListItem(item));
		}
		strings->Add(linhaBranco);

		filepath = ExtractFileDir(path);
		if (!DirectoryExists(filepath))
		{
			ForceDirectories(filepath);
		}

		strings->SaveToFile(path);

		Aviso("Arranjos, cabos e suportes exportados com sucesso.");
	}
	catch (Exception& e)
	{
		AnsiString erro;
		erro = "Exporta��o falhou.\nErro: ";
		erro += e.Message;
		Erro(erro) ;
	}

	delete strings;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::FormClose(TObject *Sender, TCloseAction &Action)
   {
   //esconde o form
   Action = caHide;
   }

//---------------------------------------------------------------------------
AnsiString __fastcall TFormCfgArranjo::LinhaCsvHeader(TListView *lview)
{
	TListColumn *column;
	AnsiString header;

	for (int nc = 0; nc < lview->Columns->Count; nc++)
	{
		column = lview->Columns->Items[nc];
		header += column->Caption + ";";
	}
	for (int ns = lview->Columns->Count ; ns <= 11; ns++)
	{
		header += ";";
	}

	return header;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TFormCfgArranjo::LinhaCsvListItem(TListItem *item)
{
	AnsiString linha;

	linha += item->Caption + ";";
	for (int ns = 0; ns < item->SubItems->Count; ns++)
	{
		linha += item->SubItems->Strings[ns] + ";";
	}
	for (int ns = item->SubItems->Count ; ns <= 11; ns++)
	{
		linha +=  ";";
	}
	return linha;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::LViewArranjoColumnClick(TObject *Sender,
      TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (ListViewSort.ColumnIndex == Column->Index)
      {//inverte ordem
      ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
      }
   else
      {//redefine �ndice da coluna usada p/ ordena��o
      ListViewSort.ColumnIndex = Column->Index;
      }
   //for�a reordena��o
   LViewArranjo->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::LViewArranjoCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //vari�veis locais
   int       index = ListViewSort.ColumnIndex-1;
   TDate     data1 =  ((VTArranjo*)(Item1->Data))->Data;
   TDate     data2 =  ((VTArranjo*)(Item2->Data))->Data;

   switch(ListViewSort.ColumnIndex)
      {
      case 0:  //data
         Compare = CompareDate(data1,data2);
         break;
      case 1:  //c�digo do Arranjo
      case 2:  //configura��o
      case 3:  //tipo
      default: //compara Caption
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      }
   //verifica se ordem crescente/decrescente
   if (! ListViewSort.OrdemCrescente) Compare = - Compare;
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::LViewCaboColumnClick(TObject *Sender,
      TListColumn *Column)
   {
   //verifica se selecionou a mesma coluna
   if (ListViewSort.ColumnIndex == Column->Index)
      {//inverte ordem
      ListViewSort.OrdemCrescente  = ! ListViewSort.OrdemCrescente;
      }
   else
      {//redefine �ndice da coluna usada p/ ordena��o
      ListViewSort.ColumnIndex = Column->Index;
      }
   //for�a reordena��o
   LViewCabo->AlphaSort();
   }

//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::LViewCaboCompare(TObject *Sender,
      TListItem *Item1, TListItem *Item2, int Data, int &Compare)
   {
   //vari�veis locais
   int       index = ListViewSort.ColumnIndex-1;

   switch(ListViewSort.ColumnIndex)
      {
      case 0:  //cod cabo
         Compare = CompareText(Item1->Caption, Item2->Caption);
         break;
      case 1:  //tipo
         Compare = CompareText(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      case 2:  //Iadm
      case 3:  //Resist
      case 4:  //RMG
      case 5:  //R0
      case 6:  //X0
      case 7:  //C0
      case 8:  //R1
      case 9:  //X1
      case 10:  //C1
      default: //compara Numero
         Compare = CompareNum(Item1->SubItems->Strings[index], Item2->SubItems->Strings[index]);
         break;
      }
   //verifica se ordem crescente/decrescente
   if (! ListViewSort.OrdemCrescente) Compare = - Compare;
   }
   
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::PageControlChange(TObject *Sender)
   {
   //altera fun��o dos bot�es do ToolBar de acordo c/ TableSheet selecionada
   if (PageControl->ActivePage == TabSheetArranjo)
      {
      ButEdt->Action = ActionEdtArranjo;
      ButIns->Action = ActionInsArranjo;
		ButRet->Action = ActionRetArranjo;
		//DVK 2015.09.30
		ButZ0Z1->Visible = true;
      }
   else if (PageControl->ActivePage == TabSheetCabo)
      {
      ButEdt->Action = ActionEdtCabo;
      ButIns->Action = ActionInsCabo;
		ButRet->Action = ActionRetCabo;
		//DVK 2015.09.30
		ButZ0Z1->Visible = false;
      }
   else if (PageControl->ActivePage == TabSheetSuporte)
      {
      ButEdt->Action = ActionEdtSuporte;
      ButIns->Action = ActionInsSuporte;
		ButRet->Action = ActionRetSuporte;
		//DVK 2015.09.30
		ButZ0Z1->Visible = false;
      }
   }
/*
//---------------------------------------------------------------------------
void __fastcall TFormCfgArranjo::VerificaModulos(void)
   {
   //vari�veis locais
   VTLicencas    *licencas      = (VTLicencas*)apl->GetObject(__classid(VTLicencas));
   VTLicenca     *licenca       = licencas->ExisteLicenca();

   //verifica restri��es impostas pela Licenca
   if (licenca == NULL) return;
   ActionSalvaOracle->Visible = licenca->Tipo == tipoMASTER;
   }
*/
//---------------------------------------------------------------------------
//eof





