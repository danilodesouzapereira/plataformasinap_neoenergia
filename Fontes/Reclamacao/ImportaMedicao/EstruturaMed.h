#ifndef EstruturaMEDH
#define EstruturaMEDH

//---------------------------------------------------------------------------
enum diagMED {diagCONTINUA=0, diagOK, diagINTERROMPE};

struct strMEDIDOR
{
   AnsiString   nome;
   int          numFases,colData,colHora,colV1,colV2,colV3, ang_defasagem;
   double       relTP, vContratada;
};

//---------------------------------------------------------------------------
#endif
