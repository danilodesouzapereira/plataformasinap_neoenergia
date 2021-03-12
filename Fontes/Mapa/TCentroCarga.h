//---------------------------------------------------------------------------
#ifndef TCentrocargaH
#define TCentrocargaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTRegiao;

//---------------------------------------------------------------------------
class TCentrocarga
   {

   public:  //m�todos
              __fastcall  TCentrocarga(VTApl *apl);
              __fastcall ~TCentrocarga(void);
      bool    __fastcall  Executa(VTRegiao *regiao, int ind_pat);

   private:  //m�todos
      void     __fastcall CalculaCoordenadas(void);

   private:  //objetos externos
      VTApl    *apl;
      VTRegiao *regiao;

   private:  //dados locais
      int        ind_pat;
      TList      *lisCAR;
   };

//---------------------------------------------------------------------------
#endif
//eof
