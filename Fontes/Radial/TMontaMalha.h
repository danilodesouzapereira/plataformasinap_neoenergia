//---------------------------------------------------------------------------
#ifndef TMontaMalhaH
#define TMontaMalhaH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTMalha;
class VTMalhas;
class VTLigacao;
class VTNet;
class VTRede;
class VTRedes;
class VTOrdena;

//---------------------------------------------------------------------------
class TMontaMalha
	{
   private: //objetos externos
		VTApl    *apl;
		VTMalhas *malhas;

	private: //dados locais
		TList    *lisEQP;
		TList    *lisBAR;
		TList    *lisLIG;
		TList    *lisSEL;
		TList    *lisBGS;
		TList    *lisMAL;
		VTOrdena *ordena;

   public:  //métodos
			  __fastcall  TMontaMalha(VTApl *apl);
			  __fastcall ~TMontaMalha(void);
		bool __fastcall  Executa(VTMalhas *malhas, VTRedes *redes);

	private: //métodos
		bool     __fastcall Executa(VTNet *net);
		bool     __fastcall Executa(VTRede *rede);
		VTMalha* __fastcall ExisteMalhaGeracaoComLigacaoComum(VTMalha *malha_ref);
		VTMalha* __fastcall ExisteMalhaComLigacaoComum(TList *lisMALHA, VTMalha *malha_ref);
		void     __fastcall FundeMalhas(VTMalha *malha_ref);
		void     __fastcall FundeMalhasGeracao(VTMalha *malha_ref);
		void     __fastcall IdentificaMalhaExternaInterna(VTBarra *barra, TList *lisLIG_EXT);
		void     __fastcall IdentificaMalhaGeracao(TList *lisLIG_EXT);
		int      __fastcall IniciaLisBarraGeradorSuprimento(VTNet *net);
		TList*   __fastcall IniciaLisLigacaoEmMalha(VTLigacao *ligacao1, VTLigacao *ligacao2);
		void     __fastcall InsereMalha(VTLigacao *ligacao1, VTLigacao *ligacao2);
		void     __fastcall InsereMalhaGeracao(VTLigacao *ligacao);
	};

//---------------------------------------------------------------------------
#endif
//eof
