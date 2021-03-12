//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <utilcls.h>
/*
#include <PlataformaSinap\Fontes\Rede\VTRede.h>
#include <PlataformaSinap\DLL_Inc\Funcao.h>
#include <PlataformaSinap\Fontes\Rede\VTCluster.h>
#include <PlataformaSinap\Fontes\Rede\VTCarga.h>
#include <PlataformaSinap\Fontes\Rede\VTTrafo.h>
#include <PlataformaSinap\Fontes\Apl\VTApl.h>
#include <PlataformaSinap\Fontes\Stk\VTStk.h>
#include <PlataformaSinap\Fontes\Progresso\VTProgresso.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamar.h>
#include <PlataformaSinap\Fontes\Patamar\VTPatamares.h>
#include <PlataformaSinap\Fontes\Rede\VTChave.h>
#include <PlataformaSinap\Fontes\Rede\VTResFlowLig.h>
#include "..\ElpaSub_Contingencia\VTContingencia.h"
#include "..\ElpaSub_Contingencia\VTDefeito.h"
*/
#include "TGeradorRelatorio.h"
#include "VTGeralRelatorio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
VTGeradorRelatorio* __fastcall NewObjGeradorRelatorio(VTApl* apl)
{
   try{
	  return(new TGeradorRelatorio(apl));
	  }catch(Exception &e)
		 {
		 return(NULL);
		 }
}
//---------------------------------------------------------------------------
__fastcall TGeradorRelatorio::TGeradorRelatorio(VTApl* apl_owner)
{
	FposSheetDeslig = -1;
	FListaImprimir = new TList;
	FApl = apl_owner;
	FCountSheets = 0;
}
//---------------------------------------------------------------------------
__fastcall TGeradorRelatorio::~TGeradorRelatorio()
{
   if(FListaImprimir) {LimpaListaImprimir(FListaImprimir); delete FListaImprimir; FListaImprimir = NULL;}
}
//---------------------------------------------------------------------------
bool  __fastcall TGeradorRelatorio::AbrirExcel(AnsiString FileName)
{

	//Variant   v0, v1, v2;
	Variant	  WorkBooks, WorkBook, Sheets;
	Variant	  Sheet1, Sheet2;
	FCountSheets = 0;
	if(!FileExists(FileName)) return false;
	try
	{
		FExcel  = Variant::CreateObject("Excel.Application");


		// destativa a visualização
		FExcel.OlePropertySet("Visible",false);
		// cria as planilas;
		//v0	=	FExcel.OlePropertyGet("Workbooks");
		//v1 	=   v0.OleProcedure("Open",FileName.c_str());

		//FExcel.OlePropertyGet("WorkBooks").OleProcedure("Open",FileName.c_str());

		WorkBooks = FExcel.OlePropertyGet("WorkBooks");
		WorkBook = WorkBooks.OleFunction("Open", FileName.c_str());
		Sheets = WorkBook.OlePropertyGet("Sheets");

		FSheets = Sheets;

		FCountSheets = Sheets.OlePropertyGet("Count");


		//InsereTexto("Cebrian", Sheet1, clBlack, 7, 3, 1, 2, 10, false, false, clNone);
		//InsereTexto("juan", Sheet2, clBlack, 5, 4, 3, 2, 10, false, false, clNone);


	}
	catch(...)
	{
		ShowMessage("Não foi possível abrir o arquivo: é provável que o OFFICE não esteja instalado no seu computador");
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGeradorRelatorio::AgrupaMINIS(TList* LisElementos)
{
	/*
	strElemRelatorio* strER;
	strElemRelatorio* strER1;
	VTRede*  rede;
	AnsiString  NomeMini;
	AnsiString  NomeRedePrim;
	strRedePrimaria*  strRP;
	strMiniReticulado*  strMIN;
	TList* LisElemDif = new TList;
	
	LimpaListaImprimir(FListaImprimir);

	ElementosDiferentes(LisElementos, LisElemDif);

	for(int i =0 ; i < LisElemDif->Count; i++)
	{
		strER = (strElemRelatorio*)LisElemDif->Items[i];
		NomeMini = strER->NomeMiniRet;
		strMIN = new strMiniReticulado;
		strMIN->NomeMiniRet = NomeMini;
		strMIN->LisStrRedePrimaria = new TList;
		for(int j = 0 ; j < FLisRedes->Count ; j++)
		{
			rede = (VTRede*)FLisRedes->Items[j];
			strRP = new strRedePrimaria;
			NomeRedePrim = rede->Codigo;
			strRP->NomeRedePrimaria = NomeRedePrim;
			strRP->LisStrElemRelatorio = new TList;
			PrencheListStrEleRelatorio(strRP->LisStrElemRelatorio, LisElementos, NomeMini, NomeRedePrim);			
			strMIN->LisStrRedePrimaria->Add(strRP);
      }

		FListaImprimir->Add(strMIN);
	}

	if(LisElemDif){delete LisElemDif; LisElemDif = NULL;}
	*/
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGeradorRelatorio::CriaSheets(void)
{
   //	VTPatamares      *patamares   = (VTPatamares*)FApl->GetObject(__classid(VTPatamares));
   //	VTPatamar        *patamar;
	AnsiString       NomeSheets;

	Variant   v0, v1, v2;
	try
	{
		// destativa a visualização
		FExcel.OlePropertySet("Visible",false);

	  // cria as planilas;
	  v0=FExcel.OlePropertyGet("Workbooks");
	  v0.OleProcedure("Add");

	  v1=v0.OlePropertyGet("Item",1);
	  v0=v1.OlePropertyGet("Worksheets") ;

	  // cria o sheet
	  v0.OleProcedure("Add");
	  NomeSheets = "Nome do Sheet"; // observação não pode inserir caracteres especiais ex. //,'
	  v0.OlePropertyGet("Item",0).OlePropertySet("Name",NomeSheets.c_str());

  }
  catch(...)
  {
	this->Fecha();
	return false;
  }


	return true;
}
//---------------------------------------------------------------------------
void __fastcall TGeradorRelatorio::CriaMatrizCell( int NElem, TList* MatrizCell)
{
	TList*   LisElem;
	if(MatrizCell == NULL) return;
	for (int i = 0; i < NElem; i++)
	{
		LisElem = new TList;
		MatrizCell->Add(LisElem);
	}
}
//---------------------------------------------------------------------------
Variant   __fastcall TGeradorRelatorio::DevolveSheet(int posicao) // inicia com 1 a partir do lado esquerdo
{
	Variant Sheet1;
	if(posicao <= 0) return NULL;
	if(posicao > FCountSheets+1) return NULL;
	Sheet1 = FSheets.OlePropertyGet("Item",posicao);
	//Sheet2 = Sheets.OlePropertyGet("Item",2);

	return Sheet1;
}
//---------------------------------------------------------------------------
int __fastcall TGeradorRelatorio::DevolvePossicaoSheet(VTPatamar* patamar)
{
	/*
	VTPatamares      *patamares   = (VTPatamares*)FApl->GetObject(__classid(VTPatamares));
	VTPatamar        *pat;
	int              posicao = -1;
	int              cont = 0;
  for (int i = 0; i < patamares->NumPatamar(); i++)
  {
		pat = patamares->Patamar[i];
		if(!pat->Enabled) continue;
		cont += 1;
		if(pat == patamar)
		{
			posicao = cont; break;
		}
  }
  return posicao;
  */
  return -1;
}
//---------------------------------------------------------------------------
double __fastcall TGeradorRelatorio::DevolveSumaINSTOT(TList* LisElem)
{
	strElemRelatorio*   ElemREL;
	double resposta = 0.0;
	if(LisElem != NULL)
	{
		for (int j = 0; j < LisElem->Count; j++)
		{
			ElemREL = (strElemRelatorio*)LisElem->Items[j];
			resposta += ElemREL->kVA_ins;
		}
	}
	return resposta;
}
//---------------------------------------------------------------------------
double __fastcall TGeradorRelatorio::DevolveSumaINS(TList* LisElem)
{
	//TList*  LisElem;
	strElemRelatorio*   ElemREL;
	double resposta = 0.0;
	if(LisElem != NULL)
	{
		for (int j = 0; j < LisElem->Count; j++)
		{
			ElemREL = (strElemRelatorio*)LisElem->Items[j];
			//ElemREL->trafo->resflow->SentidoP[0][0]
			if(ElemREL->kVA_med != 0.0)
			resposta += ElemREL->kVA_ins;
		}
	}
	return resposta;
}
//---------------------------------------------------------------------------
double __fastcall TGeradorRelatorio::DevolveSumaMED(TList* LisElem)
{
	strElemRelatorio*   ElemREL;
	double resposta = 0.0;
	if(LisElem != NULL)
	{
		for (int j = 0; j < LisElem->Count; j++)
		{
			ElemREL = (strElemRelatorio*)LisElem->Items[j];
			resposta += ElemREL->kVA_med;
		}
	}
	return resposta;
}
//---------------------------------------------------------------------------
bool __fastcall TGeradorRelatorio::ElementosDiferentes(TList* LisElementos, TList* LisElemDif)
{
	strElemRelatorio* strER;
	strElemRelatorio* strER1;
	bool diferente = false;
	for (int i = 0; i < LisElementos->Count; i++)
	{
		strER = (strElemRelatorio*)LisElementos->Items[i];
		if(LisElemDif->Count == 0) {LisElemDif->Add(strER); continue;}
		diferente = true;
		for(int j = 0 ; j < LisElemDif->Count ; j++)
		{
			strER1 = (strElemRelatorio*)LisElemDif->Items[j];
			if(strER->NomeMiniRet == strER1->NomeMiniRet)
			{diferente = false; break;}
		}
		if(diferente) LisElemDif->Add(strER);
	}

	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGeradorRelatorio::Executar(VTCluster* cluster,  TList*  ListaElementos, VTPatamar *patamar)
{
	/*
  int posSheet;
	VTContingencia *contingencia = (VTContingencia*)FApl->GetObject(__classid(VTContingencia));
	AnsiString     TipoContingencia =  contingencia->Tipo;
	AnsiString     Maniobra = RedesContingencia(contingencia->Defeito);
	AnsiString     Titulo = "";
	AnsiString     SubTitulo = "";

	FLisRedes = cluster->LisRede();

  posSheet = DevolvePossicaoSheet(patamar);
  if(posSheet == -1) return false;

  if(!AgrupaMINIS(ListaElementos)) return false;
  Titulo = TipoContingencia + "  ("+cluster->Codigo+")";
  SubTitulo = Maniobra;
  if(!SheetsCarregaMinis(Titulo, posSheet, SubTitulo, patamar->Nome)) return false;
  if(!InsereValoresExcel(posSheet)) return false;

  */
  return true;
}
//---------------------------------------------------------------------------
bool  __fastcall TGeradorRelatorio::ExecutarMINIDesligado(TList* lisCARCT, TList* lisCARPRIM)
{
	/*
	if(FposSheetDeslig == -1) return false;
	Variant   v0, v1, v2, Rango;
	AnsiString  texto = "";
	VTRede* rede;
	int pos;
	VTStk *stk = (VTStk*)FApl->GetObject(__classid(VTStk));
	VTProgresso* progresso = (VTProgresso*)stk->NewObjProgresso(NULL, NULL);
	progresso->Show("Criando Relatório de Desligamentos", 0, lisCARCT->Count+lisCARPRIM->Count);
	// captura o puntero dos Sheets;
	v0=FExcel.OlePropertyGet("Workbooks").OlePropertyGet("Item",1).OlePropertyGet("Worksheets");
	// Capura o quarto Sheets
	v1=v0.OlePropertyGet("Item",FposSheetDeslig);

	int posini = 3;
	pos = posini;
	// coloca os titulos
	InsereTexto("Número"                  , v1, clBlack, posini, 1, 3, 2, 10, false, true, clYellow);
	InsereTexto("MiniReticulado"          , v1, clBlack, posini, 2, 3, 2, 10, false, true, clYellow);
	InsereTexto("Condição de Desligamento", v1, clBlack, posini, 3, 3, 2, 10, false, true, clYellow);
	InsereTexto("Cliente VIP"             , v1, clBlack, posini, 4, 3, 2, 10, false, true, clYellow);

	// insere o desligamento pelo carregamento de CT
	for(int i = 0 ; i < lisCARCT->Count ; i++)
	{
		progresso->NextPosition();
		pos = pos+1;
		rede = (VTRede*)lisCARCT->Items[i];
		InsereTexto(IntToStr(i+1) , v1, clBlack, pos, 1, 3, 2, 10, false, true, clNone);
		InsereTexto(rede->Codigo, v1, clBlack, pos, 2, 3, 2, 10, false, true, clNone);
		texto = "Carregamento de CT";
		InsereTexto(texto, v1, clBlack, pos, 3, 3, 2, 10, false, true, clNone);
		if(ExisteClienteVIP(rede))
		{
			texto = "Existe Cliente VIP";
		}
		else
		{
			texto = "";
		}
		InsereTexto(texto, v1, clBlack, pos, 4, 3, 2, 10, false, true, clNone);

	}
	// insere o desligamento pelo carregamento do primario
	for(int i = 0 ; i < lisCARPRIM->Count ; i++)
	{
		progresso->NextPosition();
		pos = pos+1;
		rede = (VTRede*)lisCARPRIM->Items[i];
		InsereTexto(IntToStr(lisCARCT->Count+ i+1) , v1, clBlack, pos, 1, 3, 2, 10, false, true, clNone);
		InsereTexto(rede->Codigo, v1, clBlack, pos, 2, 3, 2, 10, false, true, clNone);
		texto = "Carregamento do Primário";
		InsereTexto(texto, v1, clBlack, pos, 3, 3, 2, 10, false, true, clNone);
		if(ExisteClienteVIP(rede))
		{
			texto = "Existe Cliente VIP";
		}
		else
		{
			texto = "";
		}
		InsereTexto(texto, v1, clBlack, pos, 4, 3, 2, 10, false, true, clNone);		
	}

	if(progresso){delete progresso; progresso = NULL;}
	*/
	return true;
}
//---------------------------------------------------------------------------
bool __fastcall TGeradorRelatorio::ExisteClienteVIP(VTRede* rede)
{
	TList*  lisCAR;
	bool    result = false;
	/*
	VTCarga*  carga;
	lisCAR = new TList;
	rede->LisEqpto(lisCAR, eqptoCARGA);

	for (int i = 0; i < lisCAR->Count; i++)
	{
		carga = (VTCarga*)lisCAR->Items[i];
		if(carga->VeryImportant) {result = true; break;}
	}
	
	if(lisCAR){delete lisCAR; lisCAR = NULL;}
	*/
	return result;
}
//---------------------------------------------------------------------------
void  __fastcall TGeradorRelatorio::Fecha(void)
{
   FExcel.OlePropertySet("DisplayAlerts",false);
   FExcel.OleProcedure("Save");
	FExcel.OleProcedure("Quit");
	FExcel = Unassigned;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::InicioExcel()
{
	if(FApl == NULL) return false;
	try {  FExcel  = Variant::CreateObject("Excel.Application"); }
   catch(...)
	{
	ShowMessage("Não foi possível criar o relatorio: é provável que o OFFICE não esteja instalado no seu computador");
	return false;
	}

	if(!CriaSheets()) return false;

  return true;
}
//---------------------------------------------------------------------------
void  __fastcall TGeradorRelatorio::Mostra(void)
{
	// destativa a visualização
	FExcel.OlePropertySet("Visible",true);
}
//---------------------------------------------------------------------------
int __fastcall  TGeradorRelatorio::NumeroMaximoLinhas(strMiniReticulado* strMINI)
{
	strRedePrimaria* strRP;
	int NMax = -1;

	for (int i = 0; i < strMINI->LisStrRedePrimaria->Count; i++)
	{
		strRP = (strRedePrimaria*)strMINI->LisStrRedePrimaria->Items[i];
		if(strRP->LisStrElemRelatorio->Count >= NMax)
		{
		  NMax = strRP->LisStrElemRelatorio->Count;
      }
	}

	return NMax;
}
//---------------------------------------------------------------------------
bool   __fastcall  TGeradorRelatorio::PrencheListStrEleRelatorio(TList* LisStrElemRelatorio, TList* LisElementos, AnsiString NomeMini, AnsiString NomeRedePrim)
{
	strElemRelatorio* strRel;
	for(int i = 0 ; i < LisElementos->Count ; i++)
	{
		strRel = (strElemRelatorio*)LisElementos->Items[i];
		if((strRel->NomeMiniRet == NomeMini)&&(strRel->NomeAlimPrim == NomeRedePrim))
		{
			LisStrElemRelatorio->Add(strRel);
      }		
	}

	return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::SheetsApresentacao(AnsiString NomeAlim)
{
	/*
  Variant   v0, v1, v2, Rango;
  AnsiString  texto = "";
  // captura o puntero dos Sheets;
  v0=FExcel.OlePropertyGet("Workbooks").OlePropertyGet("Item",1).OlePropertyGet("Worksheets");
  // Capura o primeiro Sheets
  v1=v0.OlePropertyGet("Item",1);

  // primeira linha
  texto = "Análise do Reticulado "+NomeAlim;
  Rango = v1.OlePropertyGet("Range", "B3:F3");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 16, 16, 3,3, 14, true, false, clNone	);

  // segunda linha
  texto = "I  - Introdução";
  InsereTexto(texto, v1, clBlack, 5, 2, 2,2,12, true, false, clNone	);
  texto = "II  - Características";
  InsereTexto(texto, v1, clBlack, 6, 2, 2,2,12, true, false, clNone	);
  texto = "III – Planilha: Regime de 1ª. Contingência ";
  InsereTexto(texto, v1, clBlack, 7, 2,  2,2,12, true, false, clNone	);
  texto = "IV - Planilha: Regime de 2ª. Contingência ";
  InsereTexto(texto, v1, clBlack, 8, 2,  2,2,12, true, false, clNone	);
  texto = "V  - Planilha: Carregamento dos Minis ";
  InsereTexto(texto, v1, clBlack, 9, 2,  2,2,12, true, false, clNone	);

  texto = "I  - Introdução ";
  InsereTexto(texto, v1, clBlack, 11, 2,  2,2,12, true, false, clNone	);

  texto =   "O estudo elaborado para o sistema Reticulado Augusta";
  texto +=  ", foi subsidiado por medições de carga, realizadas nos meses de março";
  texto +=  ", abril e maio de 2005 nos 68 minis";
  Rango = v1.OlePropertyGet("Range", "B12:F14");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto, Rango,clBlack, 16, 16, 1,1, 12, false, false, clNone	);

  texto = "II  - Características";
  InsereTexto(texto, v1, clBlack, 16, 2,  2,2,12, true, false, clNone	);

  texto =   "Conforme cálculos, o sistema em regime normal de operação apresenta o seguinte carregamento:";
  Rango = v1.OlePropertyGet("Range", "B17:F19");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto, Rango, clBlack, 16, 16, 1,1, 12, false, false, clNone	);

  // desenha a tabela
  texto =   "circuitos";
  Rango = v1.OlePropertyGet("Range", "C21:F21");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 16, 16, 3,3, 12, true, true, clNone	);
  // pinta a tabela
  Rango = v1.OlePropertyGet("Range", "B22:F25");
  InsereTextoMerge(" ", Rango, clBlack, 16, 16, 3,3, 12, false, true, clNone	);



  // parte III
  texto = "III – Planilha: Regime de 1ª. Contingência";
  InsereTexto(texto, v1, clBlack, 27, 2,  2,2,12, true, false, clNone	);

  texto =   "Esta planilha apresenta o carregamento dos circuitos troncos em 1ª. Contingência. E tambem quando existir sobrecarga em ramais.";
  Rango = v1.OlePropertyGet("Range", "B28:F30");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto, Rango, clBlack, 16, 16, 1,1, 12, false, false, clNone	);

  // parte IV
  texto = "IV – Planilha: Regime de 2ª. Contingência";
  InsereTexto(texto, v1, clBlack, 32, 2,  2,2,12, true, false, clNone	);

  texto =   "Esta planilha apresenta o carregamento dos circuitos troncos e ramais em 2ª";
  texto +=  ". Contingência. E tambem a orientação das ações para eliminar as respectivas ";
  Rango = v1.OlePropertyGet("Range", "B32:F35");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto, Rango, clBlack, 16, 16, 1,1, 12, false, false, clNone	);

  texto = "·        Minis a serem Desligados (Mini em  Sobrecarga)";
  InsereTexto(texto, v1, clRed, 36, 2,  2,2,12, true, false, clNone	);
  texto = "·        Minis a serem Desligados (Cabos  em  Sobrecarga)";
  InsereTexto(texto, v1, clYellow	, 37, 2,  2,2,12, true, false, clNone	);
  texto = "·        Minis para Controle de Carga";
  InsereTexto(texto, v1, clBlue	, 38, 2,  2,2,12, true, false, clNone	);
  texto = "·        Minis Fora de Operação";
  InsereTexto(texto, v1, clGreen	, 39, 2,  2,2,12, true, false, clNone	);

  */

  return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::SheetsCarregaMinis(AnsiString NomeAlimTipo, int posSheets, AnsiString SubTitulo, AnsiString NomePatamar)
{
	/*
  Variant   v0, v1, v2, Rango;
  TColor  color;
  TColor  colorInt;
  AnsiString  texto = "";
  // captura o puntero dos Sheets;
  v0=FExcel.OlePropertyGet("Workbooks").OlePropertyGet("Item",1).OlePropertyGet("Worksheets");
  // Capura o quarto Sheets
  v1=v0.OlePropertyGet("Item",posSheets);

  // inicia prenchimento dos dados
  // primeira linha
  texto = "ANÁLISE DO CARREGAMENTO "+NomeAlimTipo;
  Rango = v1.OlePropertyGet("Range", "A2:P2");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 16, 3,3, 14, true, false, clNone		);
  // segunda linha
  texto = SubTitulo;
  Rango = v1.OlePropertyGet("Range", "A3:P3");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 16, 3,3, 12, true, false, clNone			);
  // terceira linha
  texto = "OBS: OS VALORES DO CARREGAMENTO NO PATAMAR ("+NomePatamar+")";
  Rango = v1.OlePropertyGet("Range", "A5:P6");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 16, 3,2, 12, true, true, clNone			);
  Rango.OlePropertyGet("Font").OlePropertySet("Color",clRed);
  //-------------------------------------------------------
  // monta o cabelsalho da tabela
  texto = "MINI";
  Rango = v1.OlePropertyGet("Range", "A7:A8");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 20, 3,2, 10, true, true, clNone	);
  texto = "Total kVA Inst.";
  Rango = v1.OlePropertyGet("Range", "N7:N8");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 20, 3,2, 10, true, true, color = RGB(192,192,192) );
  texto = "kVA Inst.";
  Rango = v1.OlePropertyGet("Range", "O7:O8");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 20, 3,2, 10, true, true, color = RGB(192,192,192) );
  texto = "Soma kVA Med.";
  Rango = v1.OlePropertyGet("Range", "P7:P8");
  Rango.OleFunction("Merge");
  InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 20, 3,2, 10, true, true, color = RGB(192,192,192) );

  AnsiString  posExecel = "";

  VTRede* rede;
  for (int i = 0 ; i < FLisRedes->Count ; i++)
  {
	rede = (VTRede*)FLisRedes->Items[i];
	if(i == 0) {texto = rede->Codigo; posExecel = "B7:D7";color = RGB(192,192,192); colorInt = RGB(192,192,192);}
	if(i == 1) {texto = rede->Codigo; posExecel = "E7:G7";color = RGB(192,192,192); colorInt = clNone;}
	if(i == 2) {texto = rede->Codigo; posExecel = "H7:J7";color = RGB(192,192,192); colorInt = RGB(192,192,192);}
	if(i == 3) {texto = rede->Codigo; posExecel = "K7:M7";color = RGB(192,192,192); colorInt = clNone;}

	 Rango = v1.OlePropertyGet("Range", posExecel.c_str());
	 Rango.OleFunction("Merge");
	 InsereTextoMerge(texto.UpperCase(), Rango, clBlack, 9, 18, 3,2, 10, true, true, color);

	 texto = "CT";
	 InsereTexto(texto, v1, clBlack, 8, 2 + 3*i + 0,  3,2,10, true, true, colorInt);
	 texto = "kVA inst.";
	 InsereTexto(texto, v1, clBlack, 8, 2 + 3*i + 1,  3,2,10, true, true, colorInt);
	 texto = "kVA med.";
	 InsereTexto(texto, v1, clBlack, 8, 2 + 3*i + 2,  3,2,10, true, true, colorInt);
  }
  //-----------------------------------------------------
  */
  return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::InsereNomeMINI(Variant v1, int pos, int NMaxLinhasMini, strMiniReticulado* strMINI)
{
	/*
	AnsiString texto;
	for(int i = 0 ; i < NMaxLinhasMini;i++)
	{
		texto = strMINI->NomeMiniRet;
		InsereTexto(texto, v1, clBlack, pos+9, 1, 3, 2, 10, false, true, clNone);
		pos += 1;
	}
	*/
	return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::InsereTexto(AnsiString texto, Variant v1, TColor color, int Fila, int Coluna, int PosHor, int PosVer, double tamanho, bool Sombreado, bool tabelado, TColor colorInter, bool AutoFitRow, bool AutoFitCol)
{
  Variant v2;
  try{
  v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("Value",texto.UpperCase().c_str());
  v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("HorizontalAlignment",PosHor); // direita, centro, esquerda
  v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("VerticalAlignment", PosVer);  // direita, centro, esquerda
  if(tabelado)
  {v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertyGet("Borders").OlePropertySet("LineStyle",1);}
  // muda as propiedades
  v2=v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila,Coluna);
  v2.OlePropertyGet("Font").OlePropertySet("Color",color);
  v2.OlePropertyGet("Font").OlePropertySet("Bold",Sombreado);
  v2.OlePropertyGet("Font").OlePropertySet("Size",tamanho);
  v2.OlePropertyGet("Font").OlePropertySet("Name","Arial");

  //v2.OlePropertyGet("Interior").OlePropertySet("ColorIndex",colorInter);
  v2.OlePropertyGet("Interior").OlePropertySet("Color",colorInter);

  if(AutoFitRow){v2.OlePropertyGet("EntireRow").OleProcedure("AutoFit");}
  if(AutoFitCol){v2.OlePropertyGet("EntireColumn").OleProcedure("AutoFit");}





  // sombrea a celda;
  //v2.OlePropertyGet("Interior").OlePropertySet("ColorIndex",9-3*1);
  // permite o alinhamento central horizontal o vertical
  //Cell.OlePropertySet("HorizontalAlignment",xlCenter)
  //desenha as linhas
  //borders = cell.OlePropertyGet("Borders");
  //borders.OlePropertySet("LineStyle",1);
  }
  catch(...)
  {

  }

  return true;
}/*
//---------------------------------------------------------------------------
bool      __fastcall  TGeradorRelatorio::InsereFloatSimple(double valor, Variant v1, TColor color, int Fila, int Coluna)
{
  Variant v2;
  try{
  v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("Value",valor);

  }
  catch(...)
  {
  	return false;
  }

  return true;
} */
//---------------------------------------------------------------------------
bool      __fastcall  TGeradorRelatorio::InsereTextoSimple(AnsiString texto, Variant v1, TColor color, int Fila, int Coluna)
{
  Variant v2;
  try{
  v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("Value",texto.c_str());
//   v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila, Coluna).OlePropertySet("Value",texto);
  // muda as propiedades
  //v2=v1.OlePropertyGet("Cells").OlePropertyGet("Item",Fila,Coluna);

  //v2.OlePropertyGet("Font").OlePropertySet("Size",tamanho);


  }
  catch(...)
  {
  	return false;
  }

  return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::InsereTextoMerge(AnsiString texto, Variant Rango, TColor color, int tamVerCell, int tamHorCell, int PosHor, int PosVer, double tamanho, bool Sombreado, bool tabelado, TColor colorInter)
{
  Variant v2;
  int cor;

  Rango.OlePropertySet("HorizontalAlignment", PosHor); // direita, centro, esquerda
  Rango.OlePropertySet("VerticalAlignment", PosVer);  // direita, centro, esquerda
  Rango.OlePropertySet("Value", texto.c_str());
  Rango.OlePropertySet("ColumnWidth", tamVerCell);  // tamanha da celda vertical
  Rango.OlePropertySet("RowHeight", tamHorCell);  // tamanha da celda vertical
  Rango.OlePropertySet("WrapText", true);  // quebra automatica do texto

  Rango.OlePropertyGet("Font").OlePropertySet("Color",color);
  Rango.OlePropertyGet("Font").OlePropertySet("Bold",Sombreado);
  Rango.OlePropertyGet("Font").OlePropertySet("Size",tamanho);
  Rango.OlePropertyGet("Font").OlePropertySet("Name","Arial");

  //Rango.OlePropertyGet("Interior").OlePropertySet("ColorIndex",9);
  //red, green, blue (RGB)
  //Rango.OlePropertyGet("Interior").OlePropertySet("Color",RGB(164,224,224));
  Rango.OlePropertyGet("Interior").OlePropertySet("Color",colorInter);

  if(tabelado) Rango.OlePropertyGet("Borders").OlePropertySet("LineStyle",1);

  // sombrea a celda;
  //v2.OlePropertyGet("Interior").OlePropertySet("ColorIndex",9-3*1);
  // permite o alinhamento central horizontal o vertical
  //Cell.OlePropertySet("HorizontalAlignment",xlCenter)
  //desenha as linhas
  //borders = cell.OlePropertyGet("Borders");
  //borders.OlePropertySet("LineStyle",1);
  //formato de celda
  //vCel.OlePropertySet("NumberFormat", vValue);
  //adiciona comentario
  //vRange.OleProcedure("AddComment", vText);
  //deleta comentario
  //vRange.OlePropertyGet("Comment").OleProcedure("Delete");



  return true;
}
//---------------------------------------------------------------------------
bool  __fastcall  TGeradorRelatorio::InsereValoresExcel(int posSheets)
{
  Variant   v0, v1, v2, Rango;
  AnsiString  texto = "";
  // captura o puntero dos Sheets;
  v0=FExcel.OlePropertyGet("Workbooks").OlePropertyGet("Item",1).OlePropertyGet("Worksheets");
  // Capura o quarto Sheets
  v1=v0.OlePropertyGet("Item",posSheets);
  // imprime os valores
  int NumFilas = FListaImprimir->Count;

  VTRede*  rede;
  double Vtins, Vtmed;
  double VTTins, VTTmed;
  int    pos;
  strMiniReticulado* strMINI;
  int NMaxLinhasMini = 0;

  for(int i = 0 ; i < FListaImprimir->Count ; i++)
  {
		strMINI = (strMiniReticulado*)FListaImprimir->Items[i];
		NMaxLinhasMini = NumeroMaximoLinhas(strMINI);
		// inser os valores na tabela excel
		InsereNomeMINI(v1, pos, NMaxLinhasMini, strMINI);
		InsereValoresMINI(v1, pos, NMaxLinhasMini, strMINI);
		pos += NMaxLinhasMini;
  }

  return true;
}
//---------------------------------------------------------------------------
bool __fastcall  TGeradorRelatorio::InsereValoresMINI(Variant v1, int pos, int NMaxLinhasMini, strMiniReticulado* strMINI)
{
	/*
	strRedePrimaria*  strRP;
	strElemRelatorio* strER;

	TList*			  MatrizCell = NULL;
	TList*			  LisElemTempo = NULL;
	TColor   color;
	int 					k;
	if(strMINI->LisStrRedePrimaria->Count != FLisRedes->Count) return false;

	MatrizCell = new TList;
	CriaMatrizCell(NMaxLinhasMini, MatrizCell);
	// pesquisa todas as redes primarias existentes
	for(int i = 0 ; i < strMINI->LisStrRedePrimaria->Count; i++)
	{
		strRP = (strRedePrimaria*)strMINI->LisStrRedePrimaria->Items[i];
		// analisa todas as celdas de pos até  NMaxLinhasMini
		k = 0;
		for(int j = 0 ; j < NMaxLinhasMini; j++)
		{
			LisElemTempo = (TList*)MatrizCell->Items[j];
			if(j+1 <= strRP->LisStrElemRelatorio->Count)
			{  // neste casso insere valores na celda
				strER = (strElemRelatorio*)strRP->LisStrElemRelatorio->Items[k];
				InsereTexto(strER->NomeCT                       , v1, clBlack, pos + j + 9, 2 + 3*i + 0, 3, 2, 10, false, true, clNone);
				InsereTexto(DoubleToStr("%6.0f", strER->kVA_ins), v1, clBlack, pos + j + 9, 2 + 3*i + 1, 3, 2, 10, false, true, clNone);
				color = RetornaCarregaColor(strER->Diagnostico_id);
				InsereTexto(FloatToStrF(strER->kVA_med, ffFixed, 4, 8), v1, clBlack, pos + j + 9, 2 + 3*i + 2, 3, 2, 10, false, true, color);
				k += 1;
				LisElemTempo->Add(strER);
			}
			else
			{
				// neste caso insere valores null na celda
				InsereTexto("", v1, clBlack, pos + j + 9, 2 + 3*i + 0, 3, 2, 10, false, true, clNone);
				InsereTexto("", v1, clBlack, pos + j + 9, 2 + 3*i + 1, 3, 2, 10, false, true, clNone);
				InsereTexto("", v1, clBlack, pos + j + 9, 2 + 3*i + 2, 3, 2, 10, false, true, clNone);
         }
		}
	}
	// imprime as sumas dos valores de kva inst e kva med.
   AnsiString  txtFormulaINSTOT = "";
	AnsiString  txtFormulaINS = "";
	AnsiString  txtFormulaMED = "";
	double  SumINSTOT = 	0.0;
	double  SumINS = 	0.0;
	double  SumMED	=	0.0;
	for(int i = 0 ; i < NMaxLinhasMini ; i++)
	{
		SumINSTOT = DevolveSumaINSTOT((TList*)MatrizCell->Items[i]);
		SumINS	 = DevolveSumaINS((TList*)MatrizCell->Items[i]);
		SumMED	 = DevolveSumaMED((TList*)MatrizCell->Items[i]);


		txtFormulaINSTOT	= FloatToStrF(SumINSTOT, ffFixed, 4, 8);
		txtFormulaINS		= FloatToStrF(SumINS, ffFixed, 4, 8);
		txtFormulaMED		= FloatToStrF(SumMED, ffFixed, 4, 8);
		InsereTexto(txtFormulaINSTOT, v1, clBlack, pos + i + 9, 2 + 3*4 + 0, 3, 2, 10, false, true, clNone);
		InsereTexto(txtFormulaINS, v1, clBlack, pos + i + 9, 2 + 3*4 + 1, 3, 2, 10, false, true, clNone);
		InsereTexto(txtFormulaMED, v1, clBlack, pos + i + 9, 2 + 3*4 + 2, 3, 2, 10, false, true, clNone);
	}

	if(MatrizCell){limpaMatrizCell(MatrizCell); delete MatrizCell; MatrizCell = NULL;}
	*/
	return true;
}
//---------------------------------------------------------------------------
TColor  __fastcall TGeradorRelatorio::RetornaCarregaColor(int Valor)
{
	TColor   color = clNone;
	/*
		switch(Valor)
         {
			case diagBOM: //nada a fazer
				color = clNone; break;
			case diagREGULAR: //assume diagREGULAR
				color = clYellow; break;
			case diagRUIM:
				color = clRed; break;
			default:
				color = clNone; break;
			}
	*/
	return color;
}
//---------------------------------------------------------------------------
void  __fastcall  TGeradorRelatorio::limpaMatrizCell(TList* MatrizCell)
{
	TList*  LisElem;
	for (int i = 0; i < MatrizCell->Count; i++)
	{
		LisElem = (TList*)MatrizCell->Items[i];
		if(LisElem){delete LisElem; LisElem = NULL;}
	}
   MatrizCell->Clear();
}
//---------------------------------------------------------------------------
void  __fastcall  TGeradorRelatorio::LimpaListaImprimir(TList* Lista)
{
	strMiniReticulado* strMR;
	strRedePrimaria*   strRP;

	for(int i = 0 ; i < Lista->Count ; i++)
	{
		strMR = (strMiniReticulado*)Lista->Items[i];

		for(int j = 0 ; j < strMR->LisStrRedePrimaria->Count ; j++)
		{
			strRP = (strRedePrimaria*)strMR->LisStrRedePrimaria->Items[j];
			strRP->LisStrElemRelatorio->Clear();
			delete strRP->LisStrElemRelatorio;
			delete strRP;
		}
		strMR->LisStrRedePrimaria->Clear();
		delete strMR->LisStrRedePrimaria;
		delete strMR;
	 }
	Lista->Clear();
}

//---------------------------------------------------------------------------
void __fastcall TGeradorRelatorio::Salva(AnsiString arquivo)
	{
	FExcel.OlePropertyGet("WorkBooks").OlePropertyGet("Item",1).OleProcedure("SaveAs",arquivo.c_str());
	}

//---------------------------------------------------------------------------
bool      __fastcall  TGeradorRelatorio::Visualizar(bool visible)
{
	// ativar a visualização
	FExcel.OlePropertySet("Visible",visible);
	return true;
}
//---------------------------------------------------------------------------

