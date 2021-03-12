// ---------------------------------------------------------------------------
#ifndef TBateriaH
#define TBateriaH

// arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTBateria.h"

// ---------------------------------------------------------------------------
class TBateria : public VTBateria
{
public:
	__fastcall TBateria(int num_pat);
	__fastcall ~TBateria(void);
	VTBateria* __fastcall Clone(void);
	void __fastcall CopiaAtributosDe(VTEqpto &ref);
	void __fastcall DefineCurva(VTCurva *curva); // se refere à curva de carga e descarga
	void __fastcall DefineValoresPadrao(void);
    bool __fastcall EstadoBateria(void);
	VTCurva* __fastcall ExisteCurva(void); // se refere à curva de carga e descarga
	void __fastcall IniciaVetores();
    void __fastcall LiberaVetores(void);
    void __fastcall ReiniciaNumPat(int np);
	int __fastcall Tipo(void);
	AnsiString __fastcall TipoAsString(void);

protected: // métodos acessados via property
	// Get
	int    __fastcall PM_GetModeloCarga(void);
   int    __fastcall PM_GetModeloDesc(void);
   int    __fastcall PM_GetEstado(int ind_pat);
   int    __fastcall PM_GetModo_operacao(int ind_pat);
   double __fastcall PM_GetEarmaz_atual_kwh(int ind_pat);
   double __fastcall PM_GetPext_kw(int ind_pat);
   double __fastcall PM_GetQext_kvar(int ind_pat);
	  //Set
   void __fastcall PM_SetEstado(int ind_pat,
		  						        int estado);
   void __fastcall PM_SetModo_operacao(int ind_pat,
		  						               int modo_operacao);
   void __fastcall PM_SetEarmaz_atual_kwh(int    ind_pat,
		  								            double earmaz_kwh);
   void __fastcall PM_SetPext_kw(int    ind_pat,
		  						         double pext_kw);
   void __fastcall PM_SetQext_kvar(int    ind_pat,
											  double qext_kvar);

private: // Dados
	int    num_pat;            // Número de patamares
	int    *vet_estado;        // Vetor que contém o estado da bateria em cada patamar
	int    *vet_modo_operacao; // Vetor que contém o modo de operação da bateria em cada patamar
	double *vet_earmaz_kwh;    // Vetor que contém a energia armazenada ao fim de cada patamar
	double *vet_pext_kw;       // Vetor que contém a potência ativa   externa em cada patamar
	double *vet_qext_kvar;     // Vetor que contém a potência reativa externa em cada patamar
};

// ---------------------------------------------------------------------------
#endif
// eof
