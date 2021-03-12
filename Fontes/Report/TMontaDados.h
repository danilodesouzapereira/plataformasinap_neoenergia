//---------------------------------------------------------------------------
#ifndef TMontaDadosH
#define TMontaDadosH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTMontaDados.h"

//---------------------------------------------------------------------------
class TMontaDados : public VTMontaDados
   {
   public:  // Métodos
               __fastcall  TMontaDados(VTApl *apl);
               __fastcall ~TMontaDados(void);
		VTDados* __fastcall  Executa(TList       *lisEQP);
		VTDados* __fastcall  Executa(TDataSet    *DataSet);
		VTDados* __fastcall  Executa(TListView   *ListView);
		VTDados* __fastcall  Executa(TStringGrid *StrGrid);

   private:  // Métodos

   private:  //objetos externos
      VTApl *apl;

   private:  //ddos locais
      VTDados *dados;
   };

#endif
//---------------------------------------------------------------------------
//eof


