//---------------------------------------------------------------------------
#ifndef TGrupoGerH
#define TGrupoGerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTGrupo.h"
//---------------------------------------------------------------------------
class VTEqpto;
class VTEqbar;
class VTBarra;
//---------------------------------------------------------------------------
enum tipoOtim    {
				 tipoVariaP = 0,
				 tipoVariaQ,
				 tipoVariaV,
				 tipoVariaPV,
				 tipoVariaVF,
				 tipoVariaPQ
				};
//---------------------------------------------------------------------------
class TGrupoGer : public VTGrupo
   {
   public:
      __fastcall TGrupoGer(VTEqpto* eqpto);
      __fastcall ~TGrupoGer(void);
      //funçoes globais
      void       __fastcall AddEqpto(VTEqpto* eqpto);
      void       __fastcall AtivaEqpto(VTEqpto* eqpto){return;};
      AnsiString __fastcall Codigo(void);
      void       __fastcall DesatEqpto(VTEqpto* eqpto) {return;};
      VTEqpto*   __fastcall Eqpto(int neq);
      TList*     __fastcall LisEqpto(void);
      int        __fastcall NEq(void);
      void       __fastcall RemEqpto(VTEqpto* eqpto);
      bool       __fastcall VerEqpto(VTEqpto* eqpto);
      //funcoes de tgrupoger
	  TList*     __fastcall LisChv(void);
	  double     __fastcall Snom(int iger = -1);
      int        __fastcall Tipo();
      int        __fastcall TipoSup();
      bool       __fastcall VerBarra(VTBarra* pbarra);

   private:
      TList* lisGer;//Lista de eqBar geradores e suprimentos do grupo
      TList* lisChv;//lista de chaves internas ao grupo
      TList* lisBar;//Lista de barras

   public:
      //AnsiString nome;
      //VTGrupo* pgrupo_aux;
      bool flag_carga;
		bool flag_conec;
		//double custo;


   };
//---------------------------------------------------------------------------
#endif

