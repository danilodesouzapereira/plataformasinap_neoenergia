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

   // Vers�es 2.0 e 2.7 do NR1
   //    Barras PQ e EXT:
   //          vet_linha_J[0]: Linha da equa��o em Ir   ; coluna da vari�vel Vr
   //          vet_linha_J[1]: Linha da equa��o em Im   ; coluna da vari�vel Vm
   //    Barras PQV:
   //          vet_linha_J[0]: Linha da equa��o em Ir   ; coluna da vari�vel Theta
   //          vet_linha_J[1]: Linha da equa��o em Im   ; coluna da vari�vel Tap
   //    Barras PV:
   //          vet_linha_J[0]: Linha da equa��o em P    ; coluna da vari�vel Theta

   // Vers�es 2.4, 2.5, 2.6 e 2.7 do NR3
   //    Barras PQ, EXT e n� de barra PQV SEM tens�o controlada:
   //       Fases A, B ou C + Neutro atIsolado ou atZ:
   //          vet_linha_J[0]: Linha da equa��o em Ir ; coluna da vari�vel Vr
   //          vet_linha_J[1]: Linha da equa��o em Im ; coluna da vari�vel Vm
   //    Barras PQV:
   //       Fases A, B ou C que possui(em) tens�o controlada:
   //          vet_linha_J[0]: Linha da equa��o em Ir   ; coluna da vari�vel Theta
   //          vet_linha_J[1]: Linha da equa��o em Im   ; coluna da vari�vel Tap
   //    Barras PV:
   //          vet_linha_J[0]: Linha da equa��o �nica em P3f ; coluna da vari�vel Theta_ref

   TBarraJ *barraJ;        // Ponteiro para o objeto TBarraJ associado ao n�
   int     tipo_no;        // Cf. enum 'enum_TIPO_NO' em VTGeralC.h
   int     fase_no;        // Fase do n� {faseA, faseB, faseC ou faseN} [NR3 apenas]
   int     vet_linha_J[2]; // Cf. descri��o acima

public:   // M�todos
        __fastcall  TYnodalJ(void);
        __fastcall ~TYnodalJ(void);
   void __fastcall  Reinicia(void);
};

#endif
//---------------------------------------------------------------------------

