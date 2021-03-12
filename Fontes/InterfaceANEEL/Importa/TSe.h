//---------------------------------------------------------------------------
#ifndef TSeH
#define TSeH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;
class VTRede;
class VTRedes;

//---------------------------------------------------------------------------
class TSe : public TObject
   {
   public:
					  __fastcall  TSe(AnsiString codigo);
                 __fastcall ~TSe(void);
		VTBarra*   __fastcall  ExisteBarraMT(double vnom_kv);
		bool       __fastcall  InsereBarraAT(VTBarra *barra);
		bool       __fastcall  InsereBarraMT(VTBarra *barra);
		bool       __fastcall  InsereRedeSE(VTRede *redeSE);
		bool       __fastcall  InsereRedeMT(VTRede *redeMT);
		bool       __fastcall  IntegraComRedesPRI(void);

	public: //dados
		AnsiString codigo;
		VTRede     *redeSE;
		VTBarra    *barraAT;

	private: //dados
		TList *lisEQP;
		TList *lisBAR_MT;
		TList *lisREDE_MT;
	};

//---------------------------------------------------------------------------
TSe* __fastcall NewObjSe(AnsiString codigo);

#endif
//---------------------------------------------------------------------------
//eof