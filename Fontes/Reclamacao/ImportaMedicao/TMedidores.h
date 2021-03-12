//---------------------------------------------------------------------------

#ifndef TMedidoresH
#define TMedidoresH
//---------------------------------------------------------------------------
#include <Classes.hpp>
class VTApl;
struct strMEDIDOR;

//---------------------------------------------------------------------------
class TMedidores
   {
	public: //Metodos
						__fastcall TMedidores(VTApl *apl_owner);
						__fastcall ~TMedidores(void);
		void 			__fastcall AddMedidor(strMEDIDOR *medidor);
		void 			__fastcall AddMedidorTXT(strMEDIDOR *medidor);
		strMEDIDOR* __fastcall BuscaPorNome(AnsiString nome);
		bool  		__fastcall Consistente(strMEDIDOR *medidor);
		TList* 		__fastcall GetListMedidores(void);

	private:	//objetos externos
		VTApl *apl;

	private: //Metodos
		void __fastcall LeArqMedidores(AnsiString filepath);

	private: //dados
		TList 		*listMedidores;
		AnsiString 	filepath;

	};

#endif
//---------------------------------------------------------------------------
//eof