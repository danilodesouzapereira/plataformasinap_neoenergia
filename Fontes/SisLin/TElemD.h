
//---------------------------------------------------------------------------
#ifndef TElemDH
#define TElemDH

//---------------------------------------------------------------------------
class TElemD : public TObject
   {
   public:
                         __fastcall  TElemD(int    coluna,
                                            double valor);
                         __fastcall ~TElemD(void);

   public: // Dados
      int    coluna;  // Coluna do elemento
      double valor;   // Valor  do elemento
   };

#endif
//---------------------------------------------------------------------------
//eof
