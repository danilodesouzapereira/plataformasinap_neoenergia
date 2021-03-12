//---------------------------------------------------------------------------
#ifndef VTMontaDadosH
#define VTMontaDadosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <DB.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTDados;

//---------------------------------------------------------------------------
class VTMontaDados
   {
   public:
						      __fastcall  VTMontaDados(void) {};
      virtual           __fastcall ~VTMontaDados(void) {};
		virtual  VTDados* __fastcall  Executa(TList       *lisEQP) = 0;
		virtual  VTDados* __fastcall  Executa(TDataSet    *DataSet) = 0;
		virtual  VTDados* __fastcall  Executa(TListView   *ListView) = 0;
		virtual  VTDados* __fastcall  Executa(TStringGrid *StrGrid) = 0;
	};

//---------------------------------------------------------------------------
//função global p/ criar objeto VTExporta
//---------------------------------------------------------------------------
VTMontaDados* __fastcall NewObjMontaDados(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof


