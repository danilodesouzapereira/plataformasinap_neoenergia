//---------------------------------------------------------------------------
#ifndef TAngulosH
#define TAngulosH
//----------------------------------------------------------------------------
using namespace std;
#include <Classes.hpp>
#include <complex>
//----------------------------------------------------------------------------
class TAngulos : public TObject
	{
	public: //metodos
					__fastcall  TAngulos(void);
					__fastcall ~TAngulos(void);

   public:
      double ajuste_A_seq0;
      double ajuste_B_seq0;
      double ajuste_A_outros;
      double ajuste_B_outros;

	};

//---------------------------------------------------------------------------
#endif
