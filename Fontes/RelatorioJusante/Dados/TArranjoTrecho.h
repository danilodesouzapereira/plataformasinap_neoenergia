// ---------------------------------------------------------------------------
#ifndef TArranjoTrechoH
#define TArranjoTrechoH

// ---------------------------------------------------------------------------
#include <Classes.hpp>

// ---------------------------------------------------------------------------
class VTApl;
class VTTrecho;
class VTArranjo;

// ---------------------------------------------------------------------------
class TArranjoTrecho : public TObject
{

public:
	__fastcall TArranjoTrecho(VTApl *apl, VTTrecho *trecho);
	__fastcall ~TArranjoTrecho(void);
public: //properties
__property AnsiString   CodigoArranjo   = {read=GetCodigoArranjo,  write=SetCodigoArranjo};
__property double ComprimentoKm = {read=GetComprimentoKm, write=SetComprimentoKm};
__property VTArranjo* Arranjo = {read=arranjo};
__property int Segmento = {read=segmento};



 private:
	AnsiString __fastcall GetCodigoArranjo();
	double __fastcall GetComprimentoKm();

	void __fastcall SetCodigoArranjo(AnsiString codigoArranjo);
	void __fastcall SetComprimentoKm(double comprimentoKm);
	void __fastcall CodigoSegmento(VTTrecho *trecho);

private:

	VTApl *apl;
protected:

	AnsiString codigoArranjo;
	double comprimentoKm;
	VTArranjo *arranjo;
	int segmento;

};

#endif
// ---------------------------------------------------------------------------
// eof
