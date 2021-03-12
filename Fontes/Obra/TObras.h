//---------------------------------------------------------------------------
#ifndef TObrasH
#define TObrasH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTObras.h"

//---------------------------------------------------------------------------
class TObras : public VTObras
   {
   public:
               __fastcall  TObras(VTApl *apl);
               __fastcall ~TObras(void);
	  VTObras* __fastcall  Clone(void);
	  void     __fastcall  CopiaAtributosDe(VTObras &obras);
	  void     __fastcall DefineRedesVisiveis(VTObra *obra);
//      bool     __fastcall  EfetivaObra(VTObra *obra);
      VTObra*  __fastcall  ExisteObra(int obra_id);
	  void     __fastcall  FundeObras(void);
	  void     __fastcall  IniciaListaRedesPorObra(void);
      VTObra*  __fastcall  InsereObra(void);
      VTObra*  __fastcall  InsereObra(TDateTime data);
      bool     __fastcall  InsereObra(VTObra *obra);
      VTAcao*  __fastcall  LastAcao(int tipo, VTEqpto *eqpto);
      TList*   __fastcall  LisAcao(void);
	  TList*   __fastcall  LisObra(void);
	  void     __fastcall  LisObra(TList *lisEXT, int ano);
	  void 	   __fastcall  LisObraContinuidade(TList *lisEXT, int ano);
      TList*   __fastcall  LisObraExcluida(void);
      VTObra*  __fastcall  ObraFinal(void);
      VTObra*  __fastcall  ObraInicial(void);
      void     __fastcall  ReconfiguraRede(int ano);
      void     __fastcall  ReconfiguraRede(VTObra *obra_ant, VTObra *obra_ref);
      void     __fastcall  ReconfiguraRedeAteObra(VTObra *obra_ref);
      void     __fastcall  ReconfiguraRedeSemObra(void);
	  void     __fastcall  ReconfiguraRedeTodaObra(VTObra *obra_ref);
      void     __fastcall  Reinicia(void);
      void     __fastcall  RetiraObra(VTObra *obra);
      bool     __fastcall  VerificaEqptoBloqueado(VTEqpto *eqpto);
      bool     __fastcall  VerificaPossibilidadeExclusao(VTObra *obra);

   protected:  //métodos acessados via property
      bool      __fastcall PM_GetAnalisada(void);
      TDateTime __fastcall PM_GetDataFinal(void);
      TDateTime __fastcall PM_GetDataInicial(void);
		bool      __fastcall PM_GetModificada(void);
		//
      void      __fastcall PM_SetAnalisada(bool analisada);
		void      __fastcall PM_SetColor(TColor color);
		void      __fastcall PM_SetColorEnabled(bool enabled);
		void      __fastcall PM_SetModificada(bool modificada);

   private: //métodos
      void    __fastcall DefineObraAtiva(VTObra *obra);
	  VTObra* __fastcall ExisteObraDataMenorIgual(TDateTime data);
	  void    __fastcall MontaLisEqptoBloqueado(VTObra *obra_ativa, TList *lisEQP);
      void    __fastcall MontaLisEqptoBloqueado(TList *lisACAO, TList *lisEQP);
      void    __fastcall MontaLisEqptoBloqueado(VTEqpto *eqpto, TList *lisEQP, bool incluir_eqpto);
      VTObra* __fastcall ObraFinalIncorporada(void);

   private: // objetos externos
      VTApl *apl;
      
   private: // dados
      struct   {
               bool    analisada;
               bool    modificada;
               } PD;
      TList  *lisACAO;    //lista temporária p/ os objetos Acao de todos objetos Obra
      TList  *lisOBRA;    //lista de objetos Obra
      TList  *lisEQP_1;
      TList  *lisEQP_2;
      TList  *lisBLOCK;
      TList  *lisLIXO;
	  TList  *lisTMP;
   };

#endif
//---------------------------------------------------------------------------
//eof
