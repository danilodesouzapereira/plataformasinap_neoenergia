//---------------------------------------------------------------------------
#ifndef TSelRedeH
#define TSelRedeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTSelRede.h"

//---------------------------------------------------------------------------
class VTRede;

//---------------------------------------------------------------------------
class TSelRede : public VTSelRede
   {
   public:
           __fastcall TSelRede(void);
           __fastcall ~TSelRede(void);
      void __fastcall Executa(VTRedes *redes, TList *lisSEL, int nivel_dependecia);

   private: //m�todos
      void __fastcall ExcluiCortes(void);
      void __fastcall ExcluiRedesReducoes(void);
      void __fastcall IdentificaRedesDoNivelDependencia(int nivel_dependencia);
      void __fastcall IdentificaRedesInterligadas(VTRede *rede);
      void __fastcall InsereRedesOrdenadas(VTRede *rede);
      void __fastcall InsereRedeVizinhaComReducaoInvalida(void);

   private: //dados externos
      VTRedes  *redes;
      TList    *lisSEL;

   private: //dados locais
      TList    *lisRND;    //lista de Redes que atendem ao n�vel de depend�ncia
      TList    *lisREDE;
      TList    *lisREDU;
      TList    *lisORD;
      TList    *lisVIZ;
      TList    *lisPAI;
      TList    *lisFILHA;
   };

//---------------------------------------------------------------------------
#endif
//eof

