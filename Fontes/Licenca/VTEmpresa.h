//---------------------------------------------------------------------------
#ifndef VTEmpresaH
#define VTEmpresaH
enum empresaID {
			   SINAPSIS             =  1,
			   ENERQ                =  2,
			   ELETROPAULO          =  3,
			   MACKENZIE            =  4,
			   UFABC                =  5,
			   UFBA                 =  9,
			   COPEL                = 10,
			   RGESUL               = 11,
			   UNISINOS             = 12,
			   GRUPOREDE            = 13,
			   CEEE                 = 14,
			   FECOERGS             = 15,
			   //CEAL                 = 16,
			   CEMIG                = 17,
			   USP                  = 18,
			   CEB                  = 19,
			   CERTAJA              = 20,
			   CERTEL               = 21,
			   COPREL               = 22,
			   CRELUZ               = 23,
			   CERMISSOES           = 24,
			   CERILUZ              = 25,
			   COOPERLUZ            = 26,
			   CRERAL               = 27,
			   EDP_BANDEIRANTES     = 28,
			   EDP_ESCELSA          = 29,
			   CEPEL                = 30,
			   ELETROBRAS_HOLDING   = 31,
			   URUGUAI              = 32,
			   EDP                  = 33,
			   ELETROBRAS_ACRE      = 34,
			   ELETROBRAS_ALAGOAS   = 35,
			   ELETROBRAS_AMAZONAS  = 36,
			   ELETROBRAS_PIAUI     = 37,
			   ELETROBRAS_RONDONIA  = 38,
			   ELETROBRAS_RORAIMA   = 39,
			   UNIJORGE             = 40,
			   REINO_UNIDO          = 41,
			   COOPER               = 42,
			   IFES                 = 43,
			   UFJF                 = 44,
			   CEJAMA               = 45,
			   CERGAPA              = 46,
			   CERBRANORTE          = 47,
			   CERGRAL              = 48,
			   COOPERMILA           = 49,
			   CERPALO              = 50,
			   COOPERALIANCA        = 51,
			   CEPRAG               = 52,
			   CERSUL               = 53,
			   CERMOFUL             = 54,
			   COOPERCOCAL          = 55,
			   CEREJ                = 56,
			   CERGAL               = 57,
			   COORSEL              = 58,
			   CERAL_ANITAPOLIS     = 59,
			   CERACA               = 60,
			   FECOERUSC            = 61,
			   CEGERO               = 62,
			   CERSAD               = 63,
			   CEESAM               = 64,
			   COOPERZEM            = 65,
			   IFSP                 = 66,
			   UFES                 = 67,
			   CERTHIL              = 68,
			   CERTREL              = 69,
			   UNIVATES             = 70,
			   COELBA               = 71,
			   CELPE                = 72,
			   COSERN               = 73,
			   UNIFEI               = 74,
			   COOPERNORTE          = 75,
			   USF                  = 76,
			   CPFL                 = 79,
               LIGHT                = 83,
			   CERFOX               = 85    // nova empresa
			   };

// -----------------------------------------------------------------------------
class VTEmpresa : public TObject
{

public:
	__fastcall VTEmpresa(void){};
	virtual __fastcall ~VTEmpresa(void){};
	virtual UnicodeString __fastcall EmpresaAsStr(int empresa_id) = 0;
	virtual int __fastcall EmpresaAsInt(UnicodeString empresa_nome) = 0;

};

// -----------------------------------------------------------------------------
// função global para criar objeto VTEmpresa
// -----------------------------------------------------------------------------
VTEmpresa* __fastcall NewObjEmpresa(void);

#endif
//---------------------------------------------------------------------------
//eof

