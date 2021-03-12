//---------------------------------------------------------------------------
#ifndef TYnodalJH
#define TYnodalJH

//---------------------------------------------------------------------------
#include <vcl.h>

class TBarraJ;
class TBarraPV;

//---------------------------------------------------------------------------
class TYnodalJ : public TObject
{
public:
   // IMPORTANTE: ESTA CLASSE SERVE A NR1 E NR3

   // Versões 2.0 e 2.7 do NR1
   //    Barras PQ e EXT:
   //          vet_linha_J[0]: Linha da equação em Ir   ; coluna da variável Vr
   //          vet_linha_J[1]: Linha da equação em Im   ; coluna da variável Vm
   //    Barras PQV:
   //          vet_linha_J[0]: Linha da equação em Ir   ; coluna da variável Theta
   //          vet_linha_J[1]: Linha da equação em Im   ; coluna da variável Tap
   //    Barras PV:
   //          vet_linha_J[0]: Linha da equação em P    ; coluna da variável Theta

   // Versões 2.4, 2.5, 2.6 e 2.7 do NR3
   //    Barras PQ, EXT e nó de barra PQV SEM tensão controlada:
   //       Fases A, B ou C + Neutro atIsolado ou atZ:
   //          vet_linha_J[0]: Linha da equação em Ir ; coluna da variável Vr
   //          vet_linha_J[1]: Linha da equação em Im ; coluna da variável Vm
   //    Barras PQV:
   //       Fases A, B ou C que possui(em) tensão controlada:
   //          vet_linha_J[0]: Linha da equação em Ir   ; coluna da variável Theta
   //          vet_linha_J[1]: Linha da equação em Im   ; coluna da variável Tap
   //    Barras PV:
   //          vet_linha_J[0]: Linha da equação única em P3f ; coluna da variável Theta_ref

   TBarraJ *barraJ;        // Ponteiro para o objeto TBarraJ associado ao nó
   int     tipo_no;        // Cf. enum 'enum_TIPO_NO' em VTGeralC.h
   int     fase_no;        // Fase do nó {faseA, faseB, faseC ou faseN} [NR3 apenas]
   int     vet_linha_J[2]; // Cf. descrição acima

public:   // Métodos
        __fastcall  TYnodalJ(void);
        __fastcall ~TYnodalJ(void);
   void __fastcall  Reinicia(void);
};

#endif
//---------------------------------------------------------------------------

