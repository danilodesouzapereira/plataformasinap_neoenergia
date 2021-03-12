//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TFormEdtCarNL.h"
#include "..\Apl\VTApl.h"
//#include "..\Rede\VTCargaNL.h"
#include "..\..\DLL_Inc\Funcao.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEdtCarNL *FormEdtCarNL;
//---------------------------------------------------------------------------
__fastcall TFormEdtCarNL::TFormEdtCarNL(TComponent* Owner, VTApl *apl,  VTCargaNL *pcargaAntiga, VTCargaNL *pcargaNova)
	: TForm(Owner)
{
	this->apl = apl;
	this->pcarga = pcargaAntiga;
	if (pcargaNova == NULL)
	{
		this->pcargaNova = pcargaAntiga;
	}
	else
	{
		this->pcargaNova = pcargaNova;
    }
	//inicia o combobox antes de preencher os dados lidos
	IniciaCboxNatureza();
	//inicia o string grid com o cabeçalho
	IniciasGridHarmonicas();
	//carega os dados da carga
	ExibeDadosCarga();
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::ActionAddHarmonicaExecute(TObject *Sender)
{
	strHRM *harmonica;
	int ordem;

	ordem = sGridHarmonicas->Row*2 + 1;
	harmonica = new strHRM;
	//Ordem
	harmonica->ordem = ordem;
	//Sa mod pu
	harmonica->sa_mod_pu = 0;
	//Sa ang pu
	harmonica->sa_ang_grau = 0;
	//Sb mod pu
	harmonica->sb_mod_pu = 0;
	//Sb ang pu
	harmonica->sb_ang_grau = 120;
	//Sc mod pu
	harmonica->sc_mod_pu = 0;
	//Sc ang pu
	harmonica->sc_ang_grau = -120;
	//adiciona a harmonica
	InsereHarmonicaSGrid(harmonica,sGridHarmonicas->Row);
	//deleta a struct
	delete harmonica;
}

//---------------------------------------------------------------------------

void __fastcall TFormEdtCarNL::ActionCancelaExecute(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::ActionConfirmaExecute(TObject *Sender)
{
	//lê e valida os dados
	if (!LeDadosCarga()) return;
	//fecha o form
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::ActionRemoveHarmonicaExecute(TObject *Sender)
{
	int index;

	index = sGridHarmonicas->Row;
	RemoveHarmonicaSGrid(index);
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::ActionRetiraExecute(TObject *Sender)
{
	//fecha o form indicando exclusão do Eqpto
   ModalResult = mrNo;
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::ExibeDadosCarga(void)
{
	strFUN *fundamental;
	strHRM *harmonica;

	edtCodigo->Text = pcarga->Codigo;

	edtFP->Text =  DoubleToStr("%4.3f", pcarga->FatPot_pu);
	//le a fundamental
	fundamental = pcarga->LeFundamental();
	edtSa->Text = DoubleToStr("%4.3f", fundamental->sa_mod_kva);
	edtSb->Text = DoubleToStr("%4.3f", fundamental->sb_mod_kva);
	edtSc->Text = DoubleToStr("%4.3f", fundamental->sc_mod_kva);
	edtAngA->Text = DoubleToStr("%4.3f", fundamental->sa_ang_grau);
	edtAngB->Text = DoubleToStr("%4.3f", fundamental->sb_ang_grau);
	edtAngC->Text = DoubleToStr("%4.3f", fundamental->sc_ang_grau);
	//percorre  as harmonicas
	for (int nh = 0; nh < pcarga->NumHarmonicas; nh++)
	{
		harmonica = pcarga->LeHarmonica(nh);
		//insere no string grid
		InsereHarmonicaSGrid(harmonica,nh+1);
	}
	//acerta o combo
	cboxNatureza->ItemIndex = (pcarga->Indutiva) ? 0 : 1;

}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::IniciaCboxNatureza(void)
{
	cboxNatureza->Clear();
	cboxNatureza->AddItem("Indutiva",NULL);
	cboxNatureza->AddItem("Capacitiva",NULL);
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::IniciasGridHarmonicas(void)
{
	sGridHarmonicas->RowCount = 2;
	sGridHarmonicas->FixedRows = 1;
    //Ordem
	sGridHarmonicas->Cells[0][0] = "Ordem";
	//Sa mod pu
	sGridHarmonicas->Cells[1][0] = "Sa [pu]";
	//Sa ang pu
	sGridHarmonicas->Cells[2][0] = "Âng A";
	//Sb mod pu
	sGridHarmonicas->Cells[3][0] = "Sb [pu]";
	//Sb ang pu
	sGridHarmonicas->Cells[4][0] = "Âng B";
	//Sc mod pu
	sGridHarmonicas->Cells[5][0] = "Sc [pu]";
	//Sc ang pu
	sGridHarmonicas->Cells[6][0] = "Âng C";

//	//valors x
//	//inicializa o vetor de harmonicas
//	//Ordem
//	sGridHarmonicas->Cells[0][1] = "3";
//	//Sa mod pu
//	sGridHarmonicas->Cells[1][1] = "0";
//	//Sa ang pu
//	sGridHarmonicas->Cells[2][1] = "0";
//	//Sb mod pu
//	sGridHarmonicas->Cells[3][1] = "0";
//	//Sb ang pu
//	sGridHarmonicas->Cells[4][1] = "120";
//	//Sc mod pu
//	sGridHarmonicas->Cells[5][1] = "0";
//	//Sc ang pu
//	sGridHarmonicas->Cells[6][1] = "-120";
}

//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::InsereHarmonicaSGrid(strHRM *harmonica, int index)
{
	int n_harmonicas = index;

	SNDMSG(sGridHarmonicas->Handle, WM_SETREDRAW, false, 0);
	try
	{
		const int row_count = sGridHarmonicas->RowCount;
		sGridHarmonicas->ColCount= 7; //protecao
		//adiciona uma nova linha
		sGridHarmonicas->RowCount = row_count + 1;
		//move as harmonicas acima
		for (int row = row_count; row >= (index + 1); --row)
		{
			sGridHarmonicas->Rows[row] = sGridHarmonicas->Rows[row - 1];
		}
		//sGridHarmonicas->Rows[index +1]->Clear();
	}
	__finally
		{
			SNDMSG(sGridHarmonicas->Handle, WM_SETREDRAW, true, 0);
		}
		//update reapaint shifted col
		RECT R = sGridHarmonicas->CellRect(0, index);
		InflateRect(&R, sGridHarmonicas->Width, sGridHarmonicas->Height);
		InvalidateRect(sGridHarmonicas->Handle, &R, false);

		//Ordem
		sGridHarmonicas->Cells[0][n_harmonicas] = IntToStr(harmonica->ordem);
		//Sa mod pu
		sGridHarmonicas->Cells[1][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sa_mod_pu);
		//Sa ang pu
		sGridHarmonicas->Cells[2][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sa_ang_grau);
		//Sb mod pu
		sGridHarmonicas->Cells[3][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sb_mod_pu);
		//Sb ang pu
		sGridHarmonicas->Cells[4][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sb_ang_grau);
		//Sc mod pu
		sGridHarmonicas->Cells[5][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sc_mod_pu);
		//Sc ang pu
		sGridHarmonicas->Cells[6][n_harmonicas] = DoubleToStr("%4.3f", harmonica->sc_ang_grau);
}

//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::IsDadosCargaValid(void)
{
	AnsiString aviso;
	bool	sucesso = true;

	//valida codigo
	if (edtCodigo->Text.IsEmpty())
	{
		aviso = "Código vazio.\n";
		sucesso = false;
	}
	//valida fator de potencia
	if (!IsDoubleValid(edtFP->Text))
	{
		aviso += "Fator de potência inválido.\n";
		sucesso = false;
	}
	//valida Sa
	if (!IsDoubleValid(edtSa->Text))
	{
		aviso += "Potência da fase A inválida.\n";
		sucesso = false;
	}
	//valida Sb
	if (!IsDoubleValid(edtSb->Text))
	{
		aviso += "Potência da fase B inválida.\n";
		sucesso = false;
	}
	//valida Sc
	if (!IsDoubleValid(edtSc->Text))
	{
		aviso += "Potência da fase C inválida.\n";
		sucesso = false;
	}
	//valida Ang A
	if (!IsDoubleValid(edtAngA->Text))
	{
		aviso += "Ângulo da potênca da fase A inválida.\n";
		sucesso = false;
	}
	//valida Ang B
	if (!IsDoubleValid(edtAngB->Text))
	{
		aviso += "Ângulo da potênca da fase B inválida.\n";
		sucesso = false;
	}
	//valida Ang C
	if (!IsDoubleValid(edtAngC->Text))
	{
		aviso += "Ângulo da potênca da fase C inválida.\n";
		sucesso = false;
	}
	//valida as harmonicas individualmente
	for (int nh = 1; nh < sGridHarmonicas->RowCount; nh++)
	{
		//se nao for vazia
		if (!IsHarmonicaEmpty(nh))
		{   //verifica se é valida
        	if (!IsHarmonicaValid(nh))
			{   //não sendo válida gera aviso e falha
				aviso += "A harmônica da linha " + IntToStr(nh) +" possui valores inválidos.";
				sucesso = false;
			}
		}
	}
	//caso sucesso false, gera aviso
	if (sucesso == false)
	{
		Aviso(aviso);
	}
	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::IsDoubleValid(AnsiString numero)
{
	double valor;
	bool sucesso = true;

	try
		{sucesso = StrToDouble(numero,valor);}
	catch (Exception &e)
		{sucesso = false;}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::IsIntValid(AnsiString numero)
{
	int valor;
	bool sucesso = true;

	try
		{valor = StrToInt(numero);}
	catch (Exception &e)
		{sucesso = false;}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::IsHarmonicaEmpty(int index)
{
	bool empty = false;

	//valida ordem
	if (sGridHarmonicas->Cells[0][index].IsEmpty())
		{empty = true;}
	//valida Sa
	if (sGridHarmonicas->Cells[1][index].IsEmpty())
		{empty = true;}
	//valida Sb
	if (sGridHarmonicas->Cells[2][index].IsEmpty())
		{empty = true;}
	//valida Sc
	if (sGridHarmonicas->Cells[3][index].IsEmpty())
		{empty = true;}
	//valida Ang A
	if (sGridHarmonicas->Cells[4][index].IsEmpty())
		{empty = true;}
	//valida Ang B
	if (sGridHarmonicas->Cells[5][index].IsEmpty())
		{empty = true;}
	//valida Ang C
	if (sGridHarmonicas->Cells[6][index].IsEmpty())
		{empty = true;}

	return empty;
}
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::IsHarmonicaValid(int index)
{
	bool sucesso = true;

	//valida ordem
	if (!IsIntValid(sGridHarmonicas->Cells[0][index]))
		{sucesso = false;}
	//valida Sa
	if (!IsDoubleValid(sGridHarmonicas->Cells[1][index]))
		{sucesso = false;}
	//valida Sb
	if (!IsDoubleValid(sGridHarmonicas->Cells[2][index]))
		{sucesso = false;}
	//valida Sc
	if (!IsDoubleValid(sGridHarmonicas->Cells[3][index]))
		{sucesso = false;}
	//valida Ang A
	if (!IsDoubleValid(sGridHarmonicas->Cells[4][index]))
		{sucesso = false;}
	//valida Ang B
	if (!IsDoubleValid(sGridHarmonicas->Cells[5][index]))
		{sucesso = false;}
	//valida Ang C
	if (!IsDoubleValid(sGridHarmonicas->Cells[6][index]))
		{sucesso = false;}

	return sucesso;
}
//---------------------------------------------------------------------------
bool __fastcall TFormEdtCarNL::LeDadosCarga(void)
{
	strFUN *fundamental;
	strHRM *harmonica;
	bool sucesso = true;

	if (IsDadosCargaValid())
	{

		pcargaNova->Codigo = edtCodigo->Text;
		pcargaNova->FatPot_pu = StrToDouble(edtFP->Text);
		//le a fundamental
		fundamental = pcargaNova->LeFundamental();
		fundamental->sa_mod_kva = StrToDouble(edtSa->Text);
		fundamental->sb_mod_kva = StrToDouble(edtSb->Text);
		fundamental->sc_mod_kva = StrToDouble(edtSc->Text);
		fundamental->sa_ang_grau = StrToDouble(edtAngA->Text);
		fundamental->sb_ang_grau = StrToDouble(edtAngB->Text);
		fundamental->sc_ang_grau = StrToDouble(edtAngC->Text);
		//acerta a natureza (ambicioso hein?)
		pcargaNova->Indutiva = (cboxNatureza->ItemIndex == 0)? true : false;
		//percorre  as harmonicas
		for (int nh = 1; nh < sGridHarmonicas->RowCount; nh++)
		{
			//se nao estiver vazia
			if (!IsHarmonicaEmpty(nh))
			{
				harmonica = new strHRM();
				harmonica->ordem = StrToInt(sGridHarmonicas->Cells[0][nh]);
				harmonica->sa_mod_pu  = StrToDouble(sGridHarmonicas->Cells[1][nh]);
				harmonica->sa_ang_grau  = StrToDouble(sGridHarmonicas->Cells[2][nh]);
				harmonica->sb_mod_pu  = StrToDouble(sGridHarmonicas->Cells[3][nh]);
				harmonica->sb_ang_grau  = StrToDouble(sGridHarmonicas->Cells[4][nh]);
				harmonica->sc_mod_pu  = StrToDouble(sGridHarmonicas->Cells[5][nh]);
				harmonica->sc_ang_grau  = StrToDouble(sGridHarmonicas->Cells[6][nh]);
				pcargaNova->InsereHarmonica(harmonica);
			}
		}
	//acerta o combo
	cboxNatureza->ItemIndex = (pcargaNova->Indutiva) ? 0 : 1;

	}
	else
	{
		sucesso = false;
	}

	return sucesso;
}
//---------------------------------------------------------------------------
void __fastcall TFormEdtCarNL::RemoveHarmonicaSGrid(int index)
{
	//nao remove o header
	if (index <= (sGridHarmonicas->FixedRows -1))
		{return;}
	//caso esteja tentando remover a ultima linha nao fixa
	if (sGridHarmonicas->RowCount == 2)
	{
		sGridHarmonicas->Rows[1]->Clear();
		return;
    }
	SNDMSG(sGridHarmonicas->Handle, WM_SETREDRAW, false, 0);
	try
	{
		const int row_count = sGridHarmonicas->RowCount;
		//sobe os valores
		for (int row = index; row < row_count -1; ++row)
		{
			sGridHarmonicas->Rows[row] = sGridHarmonicas->Rows[row + 1];
		}
		//remove a ultima
		sGridHarmonicas->RowCount = row_count -1;
	}
	__finally
	{
		SNDMSG(sGridHarmonicas->Handle, WM_SETREDRAW, true, 0);
	}
	//update reapaint shifted col
	RECT R = sGridHarmonicas->CellRect(0, index);
	InflateRect(&R, sGridHarmonicas->Width, sGridHarmonicas->Height);
	InvalidateRect(sGridHarmonicas->Handle, &R, false);
}
//---------------------------------------------------------------------------






