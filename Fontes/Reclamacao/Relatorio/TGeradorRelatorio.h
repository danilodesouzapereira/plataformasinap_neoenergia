//---------------------------------------------------------------------------

#ifndef TGeradorRelatorioH
#define TGeradorRelatorioH
#include <vcl.h>
#include <stdio.h>

#include "VTGeradorRelatorio.h"

struct strElemRelatorio;
struct strMiniReticulado;
class VTCluster;
class VTPatamar;
class VTDefeito;
class VTRede;
//---------------------------------------------------------------------------
class TGeradorRelatorio : public VTGeradorRelatorio
{
public:
		  __fastcall TGeradorRelatorio(VTApl* apl);
		  __fastcall ~TGeradorRelatorio();
	bool  __fastcall AbrirExcel(AnsiString FileName);
	void  __fastcall Fecha(void);
	bool  __fastcall InicioExcel();
	bool  __fastcall Executar(VTCluster* cluster, TList*  ListElementos, VTPatamar *patamar);
	bool  __fastcall ExecutarMINIDesligado(TList* lisCARCT, TList* lisCARPRIM);
	void  __fastcall Mostra(void);
	void  __fastcall Salva(AnsiString arquivo);

	Variant   __fastcall DevolveSheet(int posicao); // inicia com 1 a partir do lado esquerdo
//   bool      __fastcall  InsereFloatSimple(double valor, Variant v1, TColor color, int Fila, int Coluna);
	bool      __fastcall  InsereTexto(AnsiString texto, Variant Vx, TColor color, int Fila, int Coluna, int PosHor, int PosVer, double tamanho, bool Sombreado, bool tabelado, TColor colorInter, bool AutoFitRow, bool AutoFitCol);
	bool      __fastcall  InsereTextoSimple(AnsiString texto, Variant Vx, TColor color, int Fila, int Coluna);
	bool      __fastcall  Visualizar(bool visible);
private:

  FILE *stream;
  AnsiString  Arquivo;
  AnsiString  FDiretorio;
  TList*      FListaImprimir;
  TList* 	  FLisRedes;
  VTApl*      FApl;
  int         FposSheetDeslig;

  private:

  Variant   FExcel;
  Variant   FSheets;
  int		FCountSheets;

  private:

	bool              __fastcall AgrupaMINIS(TList* LisElementos);
	bool              __fastcall CriaSheets();
	void				  __fastcall CriaMatrizCell( int NElem, TList* MatrizCell);
	int               __fastcall DevolvePossicaoSheet(VTPatamar* patamar);
	double            __fastcall DevolveSumaINSTOT(TList* LisElem);
	double            __fastcall DevolveSumaINS(TList* LisElem);
	double            __fastcall DevolveSumaMED(TList* LisElem);
	bool              __fastcall ElementosDiferentes(TList* LisElementos, TList* LisElemDif);
	bool              __fastcall ExisteClienteVIP(VTRede* rede);

	bool              __fastcall  InsereTextoMerge(AnsiString texto, Variant Rango,TColor color, int tamVerCell, int tamHorCell, int PosHor, int PosVer, double tamanho, bool Sombreado, bool tabelado, TColor colorInter);
	bool              __fastcall  InsereNomeMINI(Variant v1, int pos, int NMaxLinhasMini, strMiniReticulado* strMINI);
	bool              __fastcall  InsereValoresExcel(int posSheets);
	bool              __fastcall  InsereValoresMINI(Variant v1, int pos, int NMaxLinhasMini, strMiniReticulado* strMINI);
	int               __fastcall  NumeroMaximoLinhas(strMiniReticulado* strMINI);
	bool              __fastcall  PrencheListStrEleRelatorio(TList* LisStrElemRelatorio, TList* LisElementos, AnsiString NomeMini, AnsiString NomeRedePrim);
	bool              __fastcall  SheetsApresentacao(AnsiString NomeAlim);
	bool              __fastcall  SheetsCarregaMinis(AnsiString NomeAlimTipo, int posSheets, AnsiString SubTitulo, AnsiString NomePatamar);
	//AnsiString        __fastcall RedesContingencia(VTDefeito *defeito);
	TColor  	        __fastcall RetornaCarregaColor(int Valor);
	void              __fastcall  limpaMatrizCell(TList* LisElem);
	void              __fastcall  LimpaListaImprimir(TList* Lista);


};
//---------------------------------------------------------------------------
#endif
