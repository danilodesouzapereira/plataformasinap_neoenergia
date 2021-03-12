//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormFinalizaObraCOPEL.h"
#include "TFormCroqui.h"
//---------------------------------------------------------------------------
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Diretorio\VTPath.h>
#include <PlataformaSinap\Fontes\Grafico\VTGrafico.h>
#include <PlataformaSinap\Fontes\Obra\VTObra.h>
#include <PlataformaSinap\Fontes\Obra\ItemObra\VTItemObraCOPEL.h>
#include <PlataformaSinap\Fontes\Planejamento\VTPlanejamento.h>
#include <PlataformaSinap\Fontes\Planejamento\VTIpe.h>
#include <PlataformaSinap\Fontes\Rede\VTMunicipio.h>
#include <PlataformaSinap\Fontes\Util\TExtraiCampos.h>
#include <ModulosOpcionais\Fontes\BaseGRP\VTIpeGRP.h>
//---------------------------------------------------------------------------
#include <DLL_Inc\Funcao.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFormFinalizaObraARIES"
#pragma link "TFormFinalizaObraARIES"
#pragma resource "*.dfm"
TFormFinalizaObraCOPEL *FormFinalizaObraCOPEL;
//---------------------------------------------------------------------------
// https://sinapsisenergia.teamwork.com/#/tasks/17112596
__fastcall TFormFinalizaObraCOPEL::TFormFinalizaObraCOPEL(TComponent* Owner, VTApl *apl_owner, VTObra *obra)
	: TFormFinalizaObraARIES(Owner,apl_owner,obra)
{
	try
	{
		PreencheMapsCOPEL();
		//inicia os combobox
		CBoxObraInicia();
		CBoxProgramaInicia();
		CBoxRegiaoInicia();
// jss20191211
//      método transportado para o ToolKit da COPEL
//      ao final do carregamento da BD
//		CarregaMunicipios();
		CBoxVPOInicia();
		CheckListMunicipioInicia();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

}
//---------------------------------------------------------------------------
__fastcall TFormFinalizaObraCOPEL::~TFormFinalizaObraCOPEL(void)
{

}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ActionCriarCroquiExecute(TObject *Sender)
{
	// variaveis locais
	AnsiString arq_croqui;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTItemObraCOPEL *item;

	try
	{
		// verifica se ja existe um croqui e se o usuario quer manter o existente
		arq_croqui = EditCroqui->Text;
		if ((!arq_croqui.IsEmpty()) && (FileExists(arq_croqui)))
		{
			if (Confirma("Já existe um croqui para esta obra.", "Deseja criar outro ?") != idYes)
				return;
		}
		// define onde salvar a imagem
		Hide();
		SaveDialog->InitialDir = path->DirExporta();
		SaveDialog->FileName = EditNome->Text;
		SaveDialog->DefaultExt = "jpg";
		if (SaveDialog->Execute())
		{ // verifica se o arquivo ja existe
			arq_croqui = SaveDialog->FileName;
			if (FileExists(arq_croqui))
			{
				if (Confirma("Já existe um arquivo com mesmo nome.", "Deseja substituir ?") != idYes)
				{
					ActionCriarCroquiExecute(NULL);
					return;
				}
			}
			// força extensão do arquivo para jpg
			ChangeFileExt(arq_croqui, SaveDialog->DefaultExt);
			EditCroqui->Text = arq_croqui;
			if (itemObra_ref != NULL)
			{
				item = (VTItemObraCOPEL*)itemObra_ref;
				item->Croqui = arq_croqui;
			}
			CriaCroqui(arq_croqui);
			MostraCroqui(arq_croqui);
		}
		Show();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ActionVisualizarCroquiExecute(TObject *Sender)
{
	// variaveis locais
	AnsiString arq_croqui;
	TFormCroqui *formCroqui;

	try
	{
		// verifica se há um croqui para a obra
		arq_croqui = EditCroqui->Text;
		if (arq_croqui.IsEmpty())
		{
			Aviso("Não existe croqui para esta obra");
			return;
		}
		else if (!FileExists(arq_croqui))
		{
			Aviso("O arquivo com croqui desta obra não existe: " + arq_croqui);
			return;
		}
		// esconde form
		Hide();
		// cria form para apresentar o croqui
		formCroqui = new TFormCroqui(NULL, apl);
		if (formCroqui->LeArquivo(arq_croqui))
			formCroqui->ShowModal();
		delete formCroqui;
		Show();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ActionCarregarCroquiExecute(TObject *Sender)
{
    // variaveis locais
	AnsiString arq_croqui, extensao;
	VTPath *path = (VTPath*)apl->GetObject(__classid(VTPath));
	VTItemObraCOPEL *item;

	try
	{
		// verifica se há um croqui para a obra
		arq_croqui = EditCroqui->Text;
		if (!arq_croqui.IsEmpty())
		{
			if (Confirma("Já existe um croqui para esta obra.", "Deseja substituir ?") != idYes)
				return;
		}
		// abre outro arquivo
		Hide();
		OpenDialog->InitialDir = path->DirExporta();
		if (OpenDialog->Execute())
		{ // verifica se foi selecionado um arquivo jpg
			arq_croqui = OpenDialog->FileName;
			extensao = ExtractFileExt(arq_croqui);
			if (extensao.AnsiCompareIC(".jpg") != 0)
			{
				Aviso("Selecione uma imagem em JPEG");
				Show();
				return;
			}
			// salva croqui selecionado na obra
			EditCroqui->Text = arq_croqui;
			if (itemObra_ref != NULL)
			{
				item = (VTItemObraCOPEL*)itemObra_ref;
				item->Croqui = arq_croqui;
			}
			MostraCroqui(arq_croqui);
		}
		Show();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ActionLimparCroquiExecute(TObject *Sender)
{	// variaveis locais
	AnsiString arq_croqui;

	try
	{
		// verifica se há um croqui para a obra
		arq_croqui = EditCroqui->Text;
		if (!arq_croqui.IsEmpty())
		{
			if (Confirma("Já existe um croqui para esta obra.", "Deseja limpar ?") != idYes)
				return;
		}
		// desassocia nome do arquivo à obra
		EditCroqui->Text = "";
		Aviso("Obra sem croqui");
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ActionSelNoneMunicpiosExecute(TObject *Sender)

{
	for (int n = 0; n < CheckListBoxMunicipios->Items->Count; n++)
	{
		CheckListBoxMunicipios->Checked[n] = false;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::CBoxObraInicia(void)
{
	try
	{
		// inicia CBoxObra
		CBoxObra->Clear();
		CBoxObra->Items->Add("AL");
		CBoxObra->Items->Add("AN");
		CBoxObra->Items->Add("LD");
		CBoxObra->Items->Add("LN");
		CBoxObra->Items->Add("LT");
		CBoxObra->Items->Add("RS");
		CBoxObra->Items->Add("SE");
		CBoxObra->Items->Add("SN");
		CBoxObra->Items->Add("ST");
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17584647
void __fastcall TFormFinalizaObraCOPEL::CBoxProgramaInicia(void)
{
	try
	{
		// inicia CBoxPrograma
		CBoxPrograma->Clear();
		CBoxPrograma->Items->Add("ATD");
		CBoxPrograma->Items->Add("Expansão");
		CBoxPrograma->Items->Add("Melhoria DEC-FEC");
		CBoxPrograma->Items->Add("Obrigatória - Expansão");
		CBoxPrograma->Items->Add("Religadores");
		CBoxPrograma->Items->Add("Projetos Especiais");
		CBoxPrograma->Items->Add("Fuse e Trip");
		CBoxPrograma->Items->Add("Mais Clic Rural");
		CBoxPrograma->Items->Add("GT CSL MET");
		CBoxPrograma->Items->Add("Revisão");
		CBoxPrograma->Items->Add("Seccionamento Tronco Al.");
		CBoxPrograma->Items->Add("Manutenção");
		CBoxPrograma->Items->Add("Obrigatória - Melhoria");
		CBoxPrograma->Items->Add("Paraná Trifásico");
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::CBoxRegiaoInicia(void)
{
	try
	{
		// inicia CBoxRegiao
		CBoxRegiao->Clear();
		CBoxRegiao->Items->Add("CI");
		CBoxRegiao->Items->Add("CP");
		CBoxRegiao->Items->Add("CU");
		CBoxRegiao->Items->Add("LC");
		CBoxRegiao->Items->Add("LI");
		CBoxRegiao->Items->Add("LN");
		CBoxRegiao->Items->Add("LS");
		CBoxRegiao->Items->Add("NC");
		CBoxRegiao->Items->Add("NM");
		CBoxRegiao->Items->Add("NP");
		CBoxRegiao->Items->Add("NU");
		CBoxRegiao->Items->Add("OB");
		CBoxRegiao->Items->Add("OC");
		CBoxRegiao->Items->Add("OF");
		CBoxRegiao->Items->Add("OP");
		CBoxRegiao->Items->Add("OT");
		CBoxRegiao->Items->Add("TA");
		CBoxRegiao->Items->Add("TC");
		CBoxRegiao->Items->Add("TL");
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::CBoxVPOChange(TObject *Sender)
{
	//seleciona a regiao
	SelecionaRegiao(CBoxVPO->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFormFinalizaObraCOPEL::CBoxVPOInicia(void)
{
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
	TStringList *sList;

	try
	{
		if (municipio != NULL)
		{
			sList = municipio->SLisNome();
			CBoxVPO->Clear();
			CBoxVPO->Items->BeginUpdate();
			//adiciona todos
			for (int ns = 0; ns < sList->Count; ns++)
			{
				CBoxVPO->Items->Add(sList->Strings[ns]);
			}
			//ordena
			CBoxVPO->Sorted = true;
			CBoxVPO->Items->EndUpdate();
		}
	}
		catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::CheckListMunicipioInicia(void)
{
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
	TStringList *sList;

	try
	{
		if (municipio != NULL)
		{
			sList = municipio->SLisNome();
			CheckListBoxMunicipios->Clear();
			CheckListBoxMunicipios->Items->BeginUpdate();
			//adiciona todos
			for (int ns = 0; ns < sList->Count; ns++)
			{
				CheckListBoxMunicipios->Items->Add(sList->Strings[ns]);
				CheckListBoxMunicipios->Checked[ns] = false;
			}
			//ordena
			CheckListBoxMunicipios->Sorted = true;
			CheckListBoxMunicipios->Items->EndUpdate();
		}
	}
		catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::CheckListBoxMunicipiosClickCheck(TObject *Sender)
{
	//variaveis locais
	int count = 0;

	for(int n = 0; n < CheckListBoxMunicipios->Count; n++)
	{
		if(CheckListBoxMunicipios->Checked[n])
			count++;
	}
	//verifica se ficou com até 3 municipios
	if(count > 3)
	{
		Aviso("O limite de municípios selecionados é 3");
		CheckListBoxMunicipios->Checked[CheckListBoxMunicipios->ItemIndex] = false;
    }
}

//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::ConfirmaItemProprio(void)
{
	VTItemObraCOPEL *item;

	try
	{
		//guarda as infos extras
		if (itemObra_ref != NULL)
		{
			item = (VTItemObraCOPEL*)itemObra_ref;

			//croqui
			item->Croqui = EditCroqui->Text;
			//regiao
//			item->SiglaObra = CBoxObra->Text;
			//obra
//			item->SiglaRegiao = CBoxRegiao->Text;
		}
		TFormFinalizaObraARIES::ConfirmaItemProprio();
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
bool __fastcall TFormFinalizaObraCOPEL::ConfirmaObraPropria(void)
{

	bool sucesso = true;

	try
	{

		//guarda as infos extras
		if (obra_ref != NULL)
		{
			//grupo
			if (EditGrupo->Text.IsEmpty())
			{
                sucesso &= InsereAtributoIPE("0", ipeOBRA_GRUPO);
			}
			else
			{
				sucesso &= InsereAtributoIPE(EditGrupo->Text, ipeOBRA_GRUPO);
			}
			//obra
			sucesso &= InsereAtributoIPE(CBoxObra->Text, ipeOBRA);
			//regiao
			sucesso &= InsereAtributoIPE(CBoxRegiao->Text, ipeREGIAO);
			//municipio principal
			sucesso &= InsereAtributoIPE(CBoxVPO->Text, ipeMUNICIPIO_VPO);
			//municipios
			//percorre todos municipios
			for (int nm = 0; nm < CheckListBoxMunicipios->Items->Count; nm++)
			{
				if (CheckListBoxMunicipios->Checked[nm] == true)
				{
					sucesso &= InsereAtributoIPE(CheckListBoxMunicipios->Items->Strings[nm], ipeMUNICIPIO);
				}
			}
			// programa Copel
			sucesso &= InsereAtributoIPE(CBoxPrograma->Text, ipePROGRAMA);

		}
		sucesso &= TFormFinalizaObraARIES::ConfirmaObraPropria();
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::CriaCroqui(AnsiString filepath)
{
	// variaveis locais
	VTGrafico *grafico = (VTGrafico*)apl->GetObject(__classid(VTGrafico));
	TFormCroqui *formCroqui;
	TRect rect;

	try
	{
		// cria form croqui
		formCroqui = new TFormCroqui(NULL, apl);
		// exporta imagem atual para o formCroqui
		rect.init(formCroqui->ImageCroqui->Left, formCroqui->ImageCroqui->Top,
			formCroqui->ImageCroqui->Left + formCroqui->ImageCroqui->Width,
			formCroqui->ImageCroqui->Top + formCroqui->ImageCroqui->Height);
		grafico->Exporta(formCroqui->ImageCroqui->Canvas, rect);
		// salva imagem
		formCroqui->SalvaArquivo(filepath);
		// destroi form
		delete formCroqui;
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return (FileExists(filepath));
}
//---------------------------------------------------------------------------
void   __fastcall TFormFinalizaObraCOPEL::EnableDetalheItem(bool enable)
{
	try
	{
		TFormFinalizaObraARIES::EnableDetalheItem(enable);

//		CBoxRegiao->Enabled = enable;
//		CBoxObra->Enabled = enable;
		ToolBar1->Enabled = enable;
		EditCroqui->Enabled = enable;
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::InsereAtributoIPE(AnsiString texto, int tipoAtributoIPE)
{
	VTIpeAtributo *atributo = NULL;
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
	VTPlanejamento *planejamento = (VTPlanejamento*)apl->GetObject(__classid(VTPlanejamento));
	VTIpeGRP *ipe_grp;
	AnsiString textoAux;
	int valor =0;

//  REVISAR
//	 if (planejamento->Ipe)
//			 {//verifica se a Obra possui IpeAtributo associado
//			 ipe_atributo = planejamento->Ipe->ExisteIpeAtributo(str.atributo_tipo, str.atributo_id, str.descricao);
//			 if (ipe_atributo != NULL)
//				obra->LisObject->Add(ipe_atributo);
//			 }
	try
	{
		if (texto.IsEmpty())
			{return false;}
		ipe_grp = (VTIpeGRP*)planejamento->Ipe;
		switch (tipoAtributoIPE)
		{
			case ipeMUNICIPIO:
//				//recupero o numero
//				valor = municipio->Codigo(texto);
//				if (valor != 0)
//				{
//					textoAux = IntToStr(valor);
//					atributo = ipe_grp->InsereAtributoTexto(textoAux, tipatributoIPE);
//				}
//				break;
			default:

				atributo = ipe_grp->InsereAtributoTexto(texto, tipoAtributoIPE, 0);
			;
		}
		if (atributo != NULL)
		{
			obra_ref->LisObject->Add(atributo);
		}
	}
    catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return(atributo != NULL);
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::PreencheInfoItemObra(VTNodeDetalhe *nodeDet)
{
	bool sucesso = true;
	VTItemObraCOPEL *item;
	std::map<AnsiString,int>::iterator it;

	try
	{
		if ((sucesso = TFormFinalizaObraARIES::PreencheInfoItemObra(nodeDet)) == false)
			{return false;}
		if (itemObra_ref != NULL)
		{
			item = (VTItemObraCOPEL*)itemObra_ref;
			//preenche croqui
			EditCroqui->Text = item->Croqui;
		}

	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}
	return sucesso;
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
bool  __fastcall TFormFinalizaObraCOPEL::PreencheInfoObra(VTObra *obra)
{
    // variaveis locais
	bool sucesso = true;
	VTIpeAtributo *atributo;
	bool vpo = true;
	int codigo = 0;

	try
	{
		if ((sucesso = TFormFinalizaObraARIES::PreencheInfoObra(obra)) == false)
			{return false;}
		for (int n = 0; n < obra->LisObject->Count; n++)
		{ // localiza atributo texto
			atributo = (VTIpeAtributo*)obra->LisObject->Items[n];
			switch (atributo->Tipo)
			{
			case ipeMUNICIPIO:
				SelecionaMunicipioExtra(atributo->Descricao);
				break;
			case ipeMUNICIPIO_VPO:
				SelecionaMunicipioPrincipal(atributo->Descricao);
				break;
			case ipePROGRAMA:
				SelecionaPrograma(atributo->Descricao);
				break;
			case ipeOBRA_GRUPO:
				SelecionaGrupo(atributo->Descricao);
				break;
			case ipeREGIAO:
				SelecionaAtributoRegiao(atributo->Descricao);
				break;
			case ipeOBRA:
				SelecionaAtributoObra(atributo->Descricao);
				break;
			default:
				break;
			}
		}
		//se nao configurou regiao, se baseia no VPO
		if (CBoxRegiao->ItemIndex < 0)
		{
			SelecionaRegiao(CBoxVPO->Text);
		}

	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#tasks/17584647
void __fastcall TFormFinalizaObraCOPEL::PreencheMapsCOPEL(void)
{
	//metodo criado para ser possivel encontrar o indice do combox mais rapidamente
	try
	{
		mapObraCOPEL.insert(std::pair<AnsiString,int>("AL",1));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("AN",2));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("LD",3));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("LN",4));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("LT",5));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("RS",6));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("SE",7));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("SN",8));
		mapObraCOPEL.insert(std::pair<AnsiString,int>("ST",9));

		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("ATD",1));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Expansão",2));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Melhoria DEC-FEC",3));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Obrigatória - Expansão",4));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Religadores",5));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Projetos Especiais",6));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Fuse e Trip",7));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Mais Clic Rural",8));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("GT CSL MET",9));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Revisão",10));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Seccionamento Tronco Al.",11));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Manutenção",12));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Obrigatória - Melhoria",13));
		mapProgramaCOPEL.insert(std::pair<AnsiString,int>("Paraná Trifásico",14));

		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("CI",1));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("CP",2));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("CU",3));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("LC",4));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("LI",5));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("LN",6));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("LS",7));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("NC",8));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("NM",9));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("NP",10));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("NU",11));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("OB",12));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("OC",13));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("OF",14));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("OP",15));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("OT",16));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("TA",17));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("TC",18));
		mapRegiaoCOPEL.insert(std::pair<AnsiString,int>("TL",19));
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
////---------------------------------------------------------------------------
//void __fastcall TFormFinalizaObraCOPEL::SelecionaMunicipioExtra(int codigo)
//{
//	// variaveis locais
//	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
//	int ind;
//	AnsiString nome ="";
//
//	try
//	{
//		nome = municipio->Nome(codigo);
//		ind = CheckListBoxMunicipios->Items->IndexOf(nome);
//		if(ind >= 0)
//		{
//			//marca como checked
//			CLBoxMunicipios->Checked[ind] = true;
//		}
//	}
//	catch(Exception &e)
//	{
//		AnsiString funcao = __PRETTY_FUNCTION__;
//		Erro(e.Message + " em " + funcao);
//	}
//	catch(...)
//	{
//		 AnsiString funcao = __PRETTY_FUNCTION__;
//		 Erro(funcao);
//	}
//}//---------------------------------------------------------------------------void __fastcall TFormFinalizaObraCOPEL::SelecionaAtributoObra(AnsiString obraCopel){    std::map<AnsiString,int>::iterator it;
	int index = -1;

	try
	{
		it = mapObraCOPEL.find(obraCopel);
		if (it != mapObraCOPEL.end())
		{
			index = it->second;
			//seleciona a regiao
			CBoxObra->ItemIndex = index - 1;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}//---------------------------------------------------------------------------void __fastcall TFormFinalizaObraCOPEL::SelecionaAtributoRegiao(AnsiString regiao){	std::map<AnsiString,int>::iterator it;
	int index = -1;
	AnsiString siglaRegiao = "";

	try
	{
		it = mapRegiaoCOPEL.find(regiao);
		if (it != mapRegiaoCOPEL.end())
		{
			index = it->second;
			//seleciona a regiao
			CBoxRegiao->ItemIndex = index - 1;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
void __fastcall TFormFinalizaObraCOPEL::SelecionaGrupo(AnsiString num_grupo)
{
	int valor = 0;

	try
	{
		valor = StrToInt(num_grupo);
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
	if (valor > 0)
	{
		EditGrupo->Text = num_grupo;
	}
	else
	{
		EditGrupo->Text = "";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::SelecionaMunicipioExtra(AnsiString nomeMunicipio)
{
	// variaveis locais
	int ind;

	try
	{
		ind = CheckListBoxMunicipios->Items->IndexOf(nomeMunicipio);
		if(ind >= 0)
		{
			//marca como checked
			CheckListBoxMunicipios->Checked[ind] = true;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
//void __fastcall TFormFinalizaObraCOPEL::SelecionaMunicipioPrincipal(int codigo)
//{
//	// variaveis locais
//	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));
//	int ind;
//	AnsiString nome ="";
//
//	try
//	{
//		nome = municipio->Nome(codigo);
//		ind = CBoxVPO->Items->IndexOf(nome);
//		if(ind >= 0)
//		{
//			//marca como checked
//			CBoxVPO->ItemIndex = ind;
//		}
//	}
//	catch(Exception &e)
//	{
//		AnsiString funcao = __PRETTY_FUNCTION__;
//		Erro(e.Message + " em " + funcao);
//	}
//	catch(...)
//	{
//		 AnsiString funcao = __PRETTY_FUNCTION__;
//		 Erro(funcao);
//	}
//}//---------------------------------------------------------------------------void __fastcall TFormFinalizaObraCOPEL::SelecionaMunicipioPrincipal(AnsiString nomeMunicipio)
{
	// variaveis locais
	int ind;

	try
	{
		ind = CBoxVPO->Items->IndexOf(nomeMunicipio);
		if(ind >= 0)
		{
			//marca como checked
			CBoxVPO->ItemIndex = ind;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::SelecionaPrograma(AnsiString programa)
{

    std::map<AnsiString,int>::iterator it;
	int index = -1;

	try
	{

		it = mapProgramaCOPEL.find(programa);
		if (it != mapProgramaCOPEL.end())
		{
			index = it->second;
			//seleciona a regiao
			CBoxPrograma->ItemIndex = index - 1;
		}

	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::SelecionaRegiao(AnsiString nomeMunicipio)
{
//	std::map<AnsiString,int>::iterator it;
//	int index = -1;
	AnsiString siglaRegiao = "";
	VTMunicipio *municipio = (VTMunicipio*)apl->GetObject(__classid(VTMunicipio));

	try
	{
		if (municipio != NULL)
		{
			siglaRegiao = municipio->Regiao(municipio->Codigo(nomeMunicipio));
			SelecionaAtributoRegiao(siglaRegiao);
//			it = mapRegiaoCOPEL.find(siglaRegiao);
//			if (it != mapRegiaoCOPEL.end())
//			{
//				index = it->second;
//				//seleciona a regiao
//				CBoxRegiao->ItemIndex = index - 1;
//			}
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
bool __fastcall TFormFinalizaObraCOPEL::ValidaInputObra(void)
{
	bool sucesso = true;
	double valor = 0;

	try
	{
		// faz as validacoes basicas
		sucesso = TFormFinalizaObraARIES::ValidaInputObra();

		if (sucesso)
		{
			try
			{
				// valida grupo
				if ((sucesso = ValidaGrupo()) == false)
				{
					return false;
				}
				// valida obra copel
				if ((sucesso = ValidaObraCOPEL()) == false)
				{
					return false;
				}
				// valida regiao copel
				if ((sucesso = ValidaRegiaoCOPEL()) == false)
				{
					return false;
				}
				// valida programa
				if ((sucesso = ValidaProgramaCOPEL()) == false)
				{
					return false;
				}
				// valida obrigacao especial
				if ((sucesso = ValidaObrigacaoEspecial(valor)) == false)
				{
					return false;
				}
				//valida municipio principal
				if ((sucesso = ValidaMunicipioPrincipal()) == false)
				{
					return false;
				}
				//valida municipios extras
				if ((sucesso = ValidaMunicipiosExtras()) == false)
				{
					return false;
				}
			}
			catch (...)
			{
				sucesso = false;
			}
		}
	}
	catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}
	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaInputItemObra(void)
{
	bool sucesso = true;

	try
	{
		// faz as validacoes basicas
		sucesso = TFormFinalizaObraARIES::ValidaInputItemObra();

		if (sucesso)
		{
			try
            {
//				// valida obra copel
//				if ((sucesso = ValidaObraCOPEL()) == false)
//				{
//					return false;
//				}
//				// valida regiao copel
//				if ((sucesso = ValidaRegiaoCOPEL()) == false)
//				{
//					return false;
//				}
				// valida croqui
				if ((sucesso = ValidaCroqui()) == false)
				{
					return false;
				}
			}
			catch (...)
			{
				sucesso = false;
			}
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaCroqui(void)
{
    // verifica se há uma imagem do croqui
	AnsiString arq_croqui;
	bool sucesso = true;
	try
	{
		arq_croqui = EditCroqui->Text;

		if (arq_croqui.IsEmpty())
		{
			if (Confirma("A obra não contém um croqui", "Confirma a obra sem croqui ?") != IDYES)
				return false;
		}
		else if (!FileExists(arq_croqui))
		{
			if (Confirma("O arquivo do croqui não existe", "Confirma a obra sem croqui ?") != IDYES)
				return false;
			else
			{
				EditCroqui->Text = "";
			}
		}
	}
    catch(Exception &e)
	{
        AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
//https://sinapsisenergia.teamwork.com/#/tasks/16176123
bool __fastcall TFormFinalizaObraCOPEL::ValidaGrupo(void)
{
    int valor = -1;
	bool valido = true;

	try
	{
		if (!EditGrupo->Text.IsEmpty())
		{
			valor = StrToInt(EditGrupo->Text);
			if (valor <= 0)
			{
				valido = false;
			}
		}
		else
		{   //vazio é valido
			valido = true;
        }

	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaObraCOPEL(void)
{
	bool sucesso = true;

	try
	{
		// verifica CBoxObra
		if (CBoxObra->ItemIndex < 0)
		{
			Aviso("Selecione uma sigla para esta obra");
			return false;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		sucesso = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
         sucesso = false;
	}

	return sucesso;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaObrigacaoEspecial(double valor)
{
	bool valido = true;

	try
	{
		if (!EditTaxaInsercao->Text.IsEmpty())
		{
			valido = StrToDouble(EditTaxaInsercao->Text,valor);
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaMunicipioPrincipal(void)
{
	try
	{
		// verifica CBoxVPO
		if (CBoxVPO->ItemIndex < 0)
		{
			Aviso("Selecione um município principal");
			return false;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}

	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaMunicipiosExtras(void)
{
	int nMuniChecked = 0;
	#define MAX_MUNI 3

	try
	{
		//verifica se existe no máximo 3 municipios
		for (int nm = 0; nm < CheckListBoxMunicipios->Items->Count; nm++)
		{
			if (CheckListBoxMunicipios->Checked[nm] == true)
			{
                nMuniChecked++;
			}
		}
		if (nMuniChecked > MAX_MUNI)
		{
			Aviso("Devem ser selecionados no máximo " + IntToStr(MAX_MUNI) + "municipios");
			return false;
		}

	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
	return true;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaProgramaCOPEL(void)
{
	bool valido = true;
	try
	{
		// verifica CBoxRegiao
		if (CBoxPrograma->ItemIndex < 0)
		{
			Aviso("Selecione um programa");
			return false;
		}
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
// ---------------------------------------------------------------------------
bool __fastcall TFormFinalizaObraCOPEL::ValidaRegiaoCOPEL(void)
{
	bool valido = true;

	try
	{
		// verifica CBoxRegiao
		if (CBoxRegiao->ItemIndex < 0)
		{
			Aviso("Selecione uma sigla para a região");
			return false;
		}
	}
    catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
		valido = false;
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
		 valido = false;
	}

	return valido;
}
// ---------------------------------------------------------------------------
void __fastcall TFormFinalizaObraCOPEL::MostraCroqui(AnsiString filepath)
{
	// variaveis locais
	TFormCroqui *formCroqui;

	try
	{
		// esconde form
		Hide();
		// cria form para apresentar o croqui
		formCroqui = new TFormCroqui(NULL, apl);
		// le arquivo formCroqui
		formCroqui->LeArquivo(filepath);
		formCroqui->ShowModal();
		delete formCroqui;
		Show();
	}
	catch(Exception &e)
	{
		AnsiString funcao = __PRETTY_FUNCTION__;
		Erro(e.Message + " em " + funcao);
	}
	catch(...)
	{
		 AnsiString funcao = __PRETTY_FUNCTION__;
		 Erro(funcao);
	}
}
//---------------------------------------------------------------------------

