//---------------------------------------------------------------------------
#ifndef ParametrosH
#define ParametrosH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>
#include "VTParametros.h"

//---------------------------------------------------------------------------
class VTApl;
class VTParametro;

//---------------------------------------------------------------------------
class TParametros : public VTParametros
	{
	public:
							__fastcall  TParametros(VTApl *apl);
							__fastcall ~TParametros(void);
		bool       		__fastcall  Atualiza(int tipo, AnsiString atributo, double valor);
		VTParametro* 	__fastcall  Busca(int tipo, AnsiString atributo);
		bool			   __fastcall  Busca(int tipo, TList *lisPAR);
		bool 				__fastcall  Valor(int tipo, AnsiString atributo, double &valor);

	private: //métodos
		VTParametro* 	__fastcall CriaPar(int tipo, AnsiString atributo,
													AnsiString formato, double valor);
		void 				__fastcall Inicia(void);
		void 				__fastcall IniciaArranjo(void);
		void 				__fastcall IniciaCabo(void);
		void 				__fastcall IniciaCargaA(void);
		void 				__fastcall IniciaCargaB(void);
		void 				__fastcall IniciaCapacitor(void);
		void 				__fastcall IniciaChave(void);
		void 				__fastcall IniciaGerador(void);
		void 				__fastcall IniciaRegulador(void);
		void 				__fastcall IniciaSuporte(void);
		void 				__fastcall IniciaSuprimento(void);
		void 				__fastcall IniciaTrafoSE(void);
		void 				__fastcall IniciaTrafoMT(void);
		void 				__fastcall IniciaTrechoMT(void);
		void 				__fastcall IniciaTrechoBT(void);
		void 				__fastcall IniciaLisTodos(void);
		void 				__fastcall InsereOpcoes(VTParametro *par);
		void 				__fastcall LeValorSalvo(VTParametro *par);
		TList*   		__fastcall LisTipo(int tipo);
		TList*   		__fastcall PM_LisParametros(void);

   private: //objetos externos
		VTApl       *apl;

	private: //obj locais
//		TList 		*lisParam;
		TList*     lisARRANJO;
		TList*     lisCABO;
		TList*     lisCHAVE;
		TList*     lisCARGA_A;
		TList*     lisCARGA_B;
		TList*     lisCAPACITOR;
		TList*     lisGERADOR;
		TList*     lisREGULADOR;
		TList*     lisSUPORTE;
		TList*     lisSUPRIMENTO;
		TList*     lisTRAFOSE;
		TList*     lisTRAFOMT;
		TList*     lisTRECHOMT;
		TList*     lisTRECHOBT;
		TList*     lisTODOS; //todos os parametros de todas as listas
   };
//---------------------------------------------------------------------------
#endif
