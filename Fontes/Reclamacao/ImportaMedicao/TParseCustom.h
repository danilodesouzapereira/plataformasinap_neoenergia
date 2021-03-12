//---------------------------------------------------------------------------
#ifndef TParseCustomH
#define TParseCustomH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Reclamacao\VTMedicao.h"
#include "EstruturaMed.h"
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class VTApl;
class VTMedicao;
class TExtraiCampos;
class TParseCustom
	{
	public:
				__fastcall  TParseCustom(VTApl *apl);
				__fastcall ~TParseCustom(void);
		bool  __fastcall  Executa(AnsiString filename, VTMedicao *medicao, strMEDIDOR *medidor);
		bool __fastcall	PreencheStringGrid(AnsiString filename, TStringGrid *grid);
		//void __fastcall	SetRelTP(double relacao);
		//void __fastcall	SetVContratada(double tensao);

	private: //métodos
		void   __fastcall ConverteTensaoParaPrimario(double rel_tp);
		//int  __fastcall ExistePalavra(AnsiString chave);
		bool 	 __fastcall ExtraiCampos(AnsiString txt, TStringList 	*lista);
      int 	 __fastcall InsereRegistroMedicao(strMEDIDOR *medidor);
		bool 	 __fastcall LeData(AnsiString data);
		bool 	 __fastcall LeHora(AnsiString hora);
//		bool 	 __fastcall LeTensao(double *campo, AnsiString tensao, double relTP);
		bool 	 __fastcall LeTensao(double *campo, AnsiString tensao);

	private: //objetos externos
		VTApl    *apl;
		VTMedicao *medicao;

	private: //dados locais
		//double relTP, vContratada;
		int 				maxCol, ang_defasagem;
		strREGISTRO  	registro;
		TStringList 	*lines;
		TStringList 	*campos;
		TStringList 	*subStrings;
		//TExtraiCampos 	*extraiCampos;
		//DynamicArray<TStringList> *arqParsed;
	};
#endif
//---------------------------------------------------------------------------
//eof