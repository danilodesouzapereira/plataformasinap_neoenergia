//---------------------------------------------------------------------------
#ifndef TTensaoH
#define TTensaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTTensao.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTFases;

//---------------------------------------------------------------------------
class TTensao : public VTTensao
   {
   public:
           __fastcall  TTensao(VTApl *apl);
           __fastcall ~TTensao(void);
		bool __fastcall  Executa(VTRede *rede, VTBarra *barra_ref, double vff_kv);
		bool __fastcall  Executa(VTZona *zona, double vff_kv);

   private: //métodos
		void __fastcall  IniciaLisEqptoParaAlterar(VTRede *rede, VTBarra *barra_ref);
		void __fastcall  IniciaLisEqptoParaAlterar(VTZona *zona);
		void __fastcall  MontaLisEqbar(TList *lisBAR, int eqpto_tipo, TList *lisEQB);
		void __fastcall  MontaLisLigacao(TList *lisBAR, int eqpto_tipo, TList *lisLIG);
		bool __fastcall  Ordena(VTRede *rede, VTBarra *barra_ref);
		void __fastcall  RedefineTensaoBarra(double vff_kv);
		void __fastcall  RedefineTensaoSuprimento(double vff_kv);
		void __fastcall  RedefineTensaoTrafo(double vff_kv, double vfn_kv);
		void __fastcall  RedefineTensaoTrafo3E(double vff_kv);

   private: //dados externos
		VTApl   *apl;
		VTFases *fases;
   private: //dados locais
      TList *lisALT;    //lista de eqptos alterados
      TList *lisBAR;    //lista de Barras
      TList *lisEQP;    //lista de Eqbar (suprimentos)
		TList *lisLIG;    //lista de Ligacoes
		TList *lisSUP;    //lista de Ligacoes
		TList *lisTRF;    //lista de Trafos
      TList *lisTRF3E;  //lista de Trafos 3E
   };

//---------------------------------------------------------------------------
#endif
//eof

