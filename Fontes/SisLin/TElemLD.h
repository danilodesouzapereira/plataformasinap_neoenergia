
//---------------------------------------------------------------------------
#ifndef TElemLDH
#define TElemLDH

//---------------------------------------------------------------------------
class TElemLD : public TObject
   {
   public:
                         __fastcall  TElemLD(int         coluna,
                                             long double valor);
                         __fastcall ~TElemLD(void);

   public: // Dados
      int         coluna;  // Coluna do elemento
      long double valor;   // Valor  do elemento
   };

#endif
//---------------------------------------------------------------------------
//eof
